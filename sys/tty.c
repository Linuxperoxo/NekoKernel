/*
 * 
 *
 *    /--------------------------------------------o
 *    |                                            |
 *    |  copyright : (c) 2024 per linuxperoxo.     |
 *    |  author    : linuxperoxo                   |
 *    |  file      : tty.c                         |
 *    |  src mod   : 20/01/2025                    | 
 *    |                                            |
 *    o--------------------------------------------/
 *    
 *
 */

#include <std/int.h>
#include <std/utils.h>
#include <sys/vfs.h>
#include <std/io.h>
#include <device/io/keyboard/keyboard.h>
#include <threads.h>
#include <sys/kmem.h>
#include <video/vga/vga.h>
#include <sys/ports.h>

/*
 *
 *  Internal Functions
 *
 */

static tty_t* __tty = NULL; 

static __u8 tty_write(offset_t __offset__, void* __buffer__)
{
  switch(*((char*)__buffer__))
  {
    case '\n':
      TTY_IF_ROW_IN_COLISION(__tty);
      
      __tty->__win.__row += 0x01;
      __tty->__win.__col  = 0x00;
    break;

    case '\r':
      __tty->__win.__col = 0x00;
    break;

    default:
      TTY_IF_COL_IN_COLISION(__tty);
      TTY_IF_ROW_IN_COLISION(__tty);
      
      vfs_write("/dev/video", (__tty->__win.__row * TTY_WIDTH * 2) + (__tty->__win.__col * 2), __buffer__);
      vfs_write("/dev/video", (__tty->__win.__row * TTY_WIDTH * 2) + (__tty->__win.__col++ * 2) + 1, &__tty->__win.__color);
    break;
  }
  TTY_UPDATE_PTR(__tty)
  return 0;
}

static __u8 tty_read(offset_t __offset, void* __dest__)
{
  if(TTY_BF_IN_OVERFLOW(__tty))
    TTY_CLEAN_BF(__tty);

  while(!TTY_BF_IS_READY(__tty));
  
  memcpy(__dest__, &__tty->__buffer, __offset);

  return 0;
}

void tty_keyboard_in(keyboard_t* __keyboard__)
{
  if(KEY_IS_PRESS(__keyboard__))
  {
    switch(__keyboard__->__code)
    {
      case KEY_ENTER:
        TTY_ENABLE_BF_READY_BIT(__tty);
        TTY_IF_ROW_IN_COLISION(__tty);
        
        __tty->__win.__row += 0x01;
        __tty->__win.__col  = 0x00;

        TTY_UPDATE_PTR(__tty);
      break;

      case KEY_BACK:
      break;

      default:
        if(TTY_BF_IS_READY(__tty))
          TTY_DISABLE_BF_READY_BIT(__tty);

        if(TTY_BF_IN_OVERFLOW(__tty))
          TTY_CLEAN_BF(__tty);

        if(KEY_IS_VISIBLE(__keyboard__))
        {
          TTY_CPY_CH_TO_BF(__tty, __keyboard__->__char);
          
          if(!TTY_IS_INV_OUT(__tty))
            tty_write(0x00, (char*)&__tty->__buffer[__tty->__offset - 1]);
        }
      break;
    }
  }
}

/*
 *
 *  Functions tty.h
 *
 */

void tty_init()
{
  __tty = (tty_t*)kmalloc(sizeof(tty_t));

  __tty->__flags  = 0x00;
  __tty->__offset = 0x00;
  __tty->__flags  = 0x01;
  
  __tty->__win.__row   = 0x00;
  __tty->__win.__col   = 0x00;
  __tty->__win.__color = (((TTY_DF_BC_COLOR << 4) & 0x70) | (TTY_DF_CH_COLOR & 0x0F));

  vfs_mkbcfile("/dev/tty", ROOT_UID, ROOT_GID, READ_O | WRITE_O, &tty_read, &tty_write);
}

void tty_switch(tty_t* __tty__)
{
  __tty = __tty__;
}

