/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : terminal.c                    |
 *    |  SRC MOD   : 30/12/2024                    | 
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

/*
 *
 * Macros
 *
 */

#ifndef NULL
#define NULL (void*)0x00
#endif

#define TERMINAL __terminal
#define TERMINAL_OUT_OFFSET __terminal->__out_offset
#define TERMINAL_IN_OFFSET __terminal->__in_offset
#define TERMINAL_VGA __terminal->__vga_state

struct Terminal* __terminal = NULL;

/*
 *
 * Internal Functions
 *
 */

__attribute__((always_inline)) inline void terminal_put_char(const char __ch__)
{
  TERMINAL_VGA.__framebuffer[(DEFAULT_WIDTH * TERMINAL_VGA.__current_row * 2) + (TERMINAL_VGA.__current_col * 2)]     = __ch__;
  TERMINAL_VGA.__framebuffer[(DEFAULT_WIDTH * TERMINAL_VGA.__current_row * 2) + (TERMINAL_VGA.__current_col * 2) + 1] = ((TERMINAL_VGA.__bc_color << 4) & 0x70) | (TERMINAL_VGA.__ch_color & 0x0F);
  
  TERMINAL_VGA.__last_put     = __ch__;
  TERMINAL_VGA.__current_col += 0x01;
}

__attribute__((always_inline)) inline void terminal_mov_ptr(__u8 __row__, __u8 __col__)
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

/*
 *
 * terminal.h
 *
 */

void terminal_init(struct Terminal* __terminal_section__)
{ 
  if(__terminal_section__ == NULL)
  {
    __asm__ volatile(
      "int $0x11\n"
      :
      :
      :
    );
  }

  /*
   *
   * Configurando struct terminal
   *
   */

  TERMINAL = __terminal_section__;
  
  TERMINAL->__in_offset  = 0x00;
  TERMINAL->__out_offset = 0x00;
  
  /*
   *
   * Configurando struct VGA linkada ao terminal
   *
   */

  TERMINAL_VGA.__framebuffer = (void*)VGA_FRAMEBUFFER_ADDRS;
  TERMINAL_VGA.__current_row = 0x00;
  TERMINAL_VGA.__current_col = 0x00;
  TERMINAL_VGA.__bc_color    = BLACK_BC_COLOR;
  TERMINAL_VGA.__ch_color    = WHITE_CHAR_COLOR;
  TERMINAL_VGA.__last_put    = 0x00;
}

void terminal_out(const char __ch__)
{
  TERMINAL->__out_buffer[TERMINAL_OUT_OFFSET] = __ch__;
  
  switch(TERMINAL->__out_buffer[TERMINAL_OUT_OFFSET])
  {
    case '\n':    
      TERMINAL_VGA.__current_col  = 0x00;
      TERMINAL_VGA.__current_row += 0x01;

      ++TERMINAL_OUT_OFFSET;
      
      terminal_mov_ptr(TERMINAL_VGA.__current_row, TERMINAL_VGA.__current_col);
      return;
    break;

    case '\r': 
      TERMINAL_VGA.__current_col = 0x00;

      ++TERMINAL_OUT_OFFSET;

      terminal_mov_ptr(TERMINAL_VGA.__current_row, TERMINAL_VGA.__current_col);
      return;
    break;
  }
  
  if(TERMINAL_VGA.__current_col >= DEFAULT_WIDTH)
  {
    TERMINAL_VGA.__current_col  = 0x00;
    TERMINAL_VGA.__current_row += 0x01;
  }
  terminal_put_char(TERMINAL->__out_buffer[TERMINAL_OUT_OFFSET++]);
  terminal_mov_ptr(TERMINAL_VGA.__current_row, TERMINAL_VGA.__current_col);
}

void terminal_in(const char __ch__)
{
  if(KEY_IS_PRESS && KEY_IS_VISIBLE)
  {
    terminal_out(__ch__);
  }
}
