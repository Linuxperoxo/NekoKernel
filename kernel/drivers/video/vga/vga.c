/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : vga.c                         |
 *    |  SRC MOD   : 25/11/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#include <IO/ports.h>
#include "vga.h"

#define VGA_INDEX_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

struct {
  __u8* __vga_frame_buffer;
  __u8 __vga_background_color;
  __u8 __vga_char_color;
  __u8 __vga_screen_height, __vga_screen_width;
  __u8 __current_row;
  __u8 __current_col;
}__vga_struct;


void init_vga()
{
  __vga_struct.__vga_frame_buffer = (__u8*) FRAME_BUFFER_ADDRS;
  __vga_struct.__vga_char_color = DEFAULT_CHAR_COLOR;
  __vga_struct.__vga_background_color = DEFAULT_BC_COLOR;
  __vga_struct.__vga_screen_height = DEFAULT_HEIGHT;
  __vga_struct.__vga_screen_width = DEFAULT_WIDTH;
  __vga_struct.__current_row = 0;
  __vga_struct.__current_col = 0;
}

void print_screen(const char* __text__)
{
  while(*__text__ != '\0')
  {
    
    /*
     *
     * Fazendo a manipulação dos caracteres
     * de escape
     *
     */

    switch(*__text__)
    {

      /*
       *
       * '\n' para quebrar linha
       *
       */

      case '\n':
        __vga_struct.__current_row += 1;
        __vga_struct.__current_col = 0;

        ++__text__;
      break;

      /*
       *
       * Para voltar para o início da linha
       *
       */

      case '\r':
        __vga_struct.__current_col = 0;

        ++__text__;
      break;

      default:
        if(__vga_struct.__current_col + 1 >= DEFAULT_WIDTH)
        {
          __vga_struct.__current_col = 0;
          __vga_struct.__current_row += 1;
        }

        /*
         *
         * Sempre fazemos * 2 pois temos 1 byte para o caractere
         * e outro byte para a cor do caractere
         *
         */

        __vga_struct.__vga_frame_buffer[DEFAULT_WIDTH * __vga_struct.__current_row * 2 + __vga_struct.__current_col * 2] = *__text__;
        __vga_struct.__vga_frame_buffer[DEFAULT_WIDTH * __vga_struct.__current_row * 2 + __vga_struct.__current_col * 2 - 1] = __vga_struct.__vga_background_color << 4 | __vga_struct.__vga_char_color;

        __vga_struct.__current_col += 1;

        ++__text__;
      break;
    }
  }
  vga_ptr(__vga_struct.__current_row, __vga_struct.__current_col);
}

void vga_ptr(__u8 __row__, __u8 __col__)
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

  outb(VGA_INDEX_PORT, 0x0E); // Seleciona o registrador 0x0E (parte mais significativa da posição do cursor)
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

  outb(VGA_INDEX_PORT, 0x0F); // Seleciona o registrador 0x0F (parte menos significativa da posição do cursor)
  outb(VGA_DATA_PORT, __offset & 0xFF); // Envia o byte menos significativo da posição para o registrador de dados.
}

void clear_screen()
{
  __u32 __i = 0;

  while(__i < DEFAULT_HEIGHT * 2 * DEFAULT_WIDTH * 2)
  {
    __vga_struct.__vga_frame_buffer[__i++] = 0x00;
    __vga_struct.__vga_frame_buffer[__i++] = (__vga_struct.__vga_background_color << 4) | __vga_struct.__vga_char_color;
  }
  __vga_struct.__current_row = 0;
  __vga_struct.__current_col = 0;
}

