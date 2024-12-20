/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : terminal.c                    |
 *    |  SRC MOD   : 20/12/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#include <terminal.h>
#include <std/int.h>
#include <std/utils.h>
#include <std/io.h>
#include <video/vga/vga.h>
#include <sys/ports.h>
#include <device/io/keyboard/keyboard.h>

struct Terminal __current_section;

void terminal_init()
{
  __current_section.__in_buffer_offset = 0x00;
  __current_section.__out_buffer_offset = 0x00;
  __current_section.__err_buffer_offset = 0x00;
  __current_section.__this_section_keyboard = &__keyboard;
  __current_section.__this_section_keyboard->__flags |= 0b1000;
}

void terminal_init_vga()
{
  __current_section.__this_section_vga_state->__framebuffer = (__u8*) VGA_FRAMEBUFFER_ADDRS;
  __current_section.__this_section_vga_state->__current_row = 0;
  __current_section.__this_section_vga_state->__current_col = 0;
  __current_section.__this_section_vga_state->__bc_color = DEFAULT_BC_COLOR;
  __current_section.__this_section_vga_state->__ch_color = DEFAULT_CHAR_COLOR;
}

void terminal_mov_ptr(__u8 __row__, __u8 __col__)
{ 
  /*
   *
   * Offset de 0xB80000
   *
   */

  __u16 __offset = DEFAULT_WIDTH * __row__ + __col__;

  /*
   *
   * O registrador 0x0E contém o byte mais significativo 
   * da posição do cursor (8 bits mais à esquerda).
   *
   * Aqui estamos enviando a parte mais significativa da 
   * posição do cursor para o hardware.
   *
   */

  outb(VGA_CTRL_PORT, 0x0E); // Seleciona o registrador 0x0E (parte mais significativa da posição do cursor)
  outb(VGA_DATA_PORT, (__offset >> 8) & 0xFF); // Envia o byte mais significativo da posição para o registrador de dados.

  /*
   *
   * O registrador 0x0F contém o byte menos significativo 
   * da posição do cursor (8 bits à direita).
   *
   * Aqui estamos enviando a parte menos significativa da 
   * posição do cursor para o hardware.
   *
   */

  outb(VGA_CTRL_PORT, 0x0F); // Seleciona o registrador 0x0F (parte menos significativa da posição do cursor)
  outb(VGA_DATA_PORT, __offset & 0xFF); // Envia o byte menos significativo da posição para o registrador de dados.
}

void terminal_out_write(const char __ch__)
{
  __current_section.__out_buffer_offset = __current_section.__out_buffer_offset >= MAX_IN_BUFFER_SIZE ? 0x00 : __current_section.__out_buffer_offset;
  
  switch(__ch__) 
  {
    case '\n':
      __current_section.__this_section_vga_state->__current_row += 1;
      __current_section.__this_section_vga_state->__current_col = 0;   
    break;

    case '\r':
      __current_section.__this_section_vga_state->__current_col = 0;
      return;
    break;

    default:
      if(__current_section.__this_section_vga_state->__current_col + 1 >= DEFAULT_WIDTH) {
        __current_section.__this_section_vga_state->__current_row += 1;
        __current_section.__this_section_vga_state->__current_col = 0;
      }

      __current_section.__this_section_vga_state->__framebuffer[DEFAULT_WIDTH * __current_section.__this_section_vga_state->__current_row * 2 + __current_section.__this_section_vga_state->__current_col++ * 2] = __ch__;
      __current_section.__this_section_vga_state->__last_put = __ch__;
      __current_section.__terminal_out_buffer[__current_section.__out_buffer_offset++] = __current_section.__this_section_vga_state->__last_put;
    break;
  }
  terminal_mov_ptr(__current_section.__this_section_vga_state->__current_row, __current_section.__this_section_vga_state->__current_col);
}

void terminal_in_write(const char __ch__)
{
  __current_section.__in_buffer_offset = __current_section.__in_buffer_offset >= MAX_IN_BUFFER_SIZE ? 0x00 : __current_section.__in_buffer_offset;
    
  if(__current_section.__this_section_vga_state->__current_col + 1 >= DEFAULT_WIDTH) {
    __current_section.__this_section_vga_state->__current_row += 1;
    __current_section.__this_section_vga_state->__current_col = 0;
  }

  __current_section.__this_section_vga_state->__framebuffer[DEFAULT_WIDTH * __current_section.__this_section_vga_state->__current_row * 2 + __current_section.__this_section_vga_state->__current_col++ * 2] = __ch__;
  __current_section.__this_section_vga_state->__last_put = __ch__;
  __current_section.__terminal_in_buffer[__current_section.__in_buffer_offset++] = __current_section.__this_section_vga_state->__last_put;

  terminal_mov_ptr(__current_section.__this_section_vga_state->__current_row, __current_section.__this_section_vga_state->__current_col);
}

void terminal_output_clear()
{
  __u16 __size = 0;

  while(__size < DEFAULT_WIDTH * DEFAULT_HEIGHT * 2) {
    __current_section.__this_section_vga_state->__framebuffer[__size++] = 0x00;
    __current_section.__this_section_vga_state->__framebuffer[__size++] = __current_section.__this_section_vga_state->__bc_color << 4 | __current_section.__this_section_vga_state->__ch_color;
  }
  __current_section.__this_section_vga_state->__last_put = 0x00;
  __current_section.__out_buffer_offset = 0x00;
}

void terminal_input_clear()
{
  __current_section.__this_section_keyboard->__code = 0x00;
  __current_section.__this_section_keyboard->__scan = 0x00;
  __current_section.__in_buffer_offset = 0x00;

  //memset((char*)&__current_section.__terminal_in_buffer, 0x00, MAX_IN_BUFFER_SIZE);
}

__u8 terminal_get_last_key()
{
  return __current_section.__this_section_keyboard->__code;
}

void terminal_keyboard_print()
{
  switch(__current_section.__this_section_keyboard->__code)
  {
    case KEY_ENTER:
      terminal_out_write('\n');
    break;

    case KEY_BACK:
      if(__current_section.__in_buffer_offset > 0)
      {
        if(__current_section.__this_section_vga_state->__current_col == 0)
        {
          if(__current_section.__this_section_vga_state->__current_row > 0)
          {
            __current_section.__this_section_vga_state->__current_row -= 1;
          }
        } 
        else 
        {
          __current_section.__this_section_vga_state->__current_col -= 1;
        }

        __current_section.__this_section_vga_state->__framebuffer[DEFAULT_WIDTH * __current_section.__this_section_vga_state->__current_row * 2 + __current_section.__this_section_vga_state->__current_col * 2] = '\0';
        __current_section.__this_section_vga_state->__last_put = '\0';
        __current_section.__in_buffer_offset -= __current_section.__in_buffer_offset == 0 ? 0 : 1;
        __current_section.__terminal_in_buffer[__current_section.__in_buffer_offset] = '\0';
        
        terminal_mov_ptr(__current_section.__this_section_vga_state->__current_row, __current_section.__this_section_vga_state->__current_col);
      }
    break;

    default:
      if(KEY_IS_VISIBLE)
        terminal_in_write(__current_section.__this_section_keyboard->__char);
    break;
  }
}

void terminal_keyboard_input()
{
  if(KEY_IS_PRESS)
    terminal_keyboard_print(__current_section.__this_section_keyboard->__char);
}

