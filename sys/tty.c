/*
 * 
 *
 *    /--------------------------------------------o
 *    |                                            |
 *    |  copyright : (c) 2024 per linuxperoxo.     |
 *    |  author    : linuxperoxo                   |
 *    |  file      : tty.c                         |
 *    |  src mod   : 18/01/2025                    | 
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

__u8 tty_write(offset_t __offset__, void* __ch__)
{
  switch(*((char*)__ch__))
  {
    case '\n':
      __tty->__win.__row += 0x01;
      __tty->__win.__col  = 0x00;

      if(__tty->__win.__row >= DEFAULT_HEIGHT)
      {
        vga_screen_down();
        
        __tty->__win.__row = DEFAULT_HEIGHT - 1;
        __tty->__win.__col = 0x00;
      }
    break;

    case '\r':
      __tty->__win.__col = 0x00;
    break;

    default:
      if(__tty->__win.__col >= DEFAULT_WIDTH)
      {
        __tty->__win.__row += 0x01;
        __tty->__win.__col  = 0x00;
      }

      if(__tty->__win.__row >= DEFAULT_HEIGHT)
      {
        vga_screen_down();

        __tty->__win.__row = DEFAULT_HEIGHT - 1;
        __tty->__win.__col = 0x00;
      }
      
      vfs_write("/dev/video", (__tty->__win.__row * DEFAULT_WIDTH * 2) + (__tty->__win.__col * 2), (char*)__ch__);
      __tty->__win.__col += 0x01;
    break;
  }
  vga_mov_ptr(__tty->__win.__row, __tty->__win.__col);   
  return 1;
}

__u8 tty_read(offset_t __offset, void* __dest__)
{
  return 1;
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
  
  __tty->__win.__row = 0x00;
  __tty->__win.__col = 0x00;

  vfs_mkbcfile("/dev/tty", ROOT_UID, ROOT_GID, READ_O | WRITE_O, &tty_read, &tty_write);
}

void tty_keyboard_in(keyboard_t* __keyboard__)
{
  if(KEY_IS_PRESS(__keyboard__))
  {
    switch(__keyboard__->__code)
    {
      case KEY_ENTER:
        tty_write(0x00, (char*)"\n");
      break;

      case KEY_BACK:
      break;

      default:
        if(KEY_IS_VISIBLE(__keyboard__))
        {
          tty_write(0x00, (char*)&__keyboard__->__char);
        }
      break;
    }
  }
}

void tty_switch(tty_t* __tty__)
{
  __tty = __tty__;
}

