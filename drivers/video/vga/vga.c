/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : vga.c                         |
 *    |  SRC MOD   : 02/12/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#include <std/types.h>
#include <std/ports.h>
#include <video/vga/vga.h>

struct VGAState __vga = {
  0, 0, 0, 0, 0
};

void vga_init() {
  __vga.__framebuffer = (__u8*) VGA_FRAMEBUFFER_ADDRS;
  __vga.__current_row = 0;
  __vga.__current_col = 0;
  __vga.__bc_color = DEFAULT_BC_COLOR;
  __vga.__ch_color = DEFAULT_CHAR_COLOR;
}

void vga_reset() {
  vga_init();
  vga_clean();
}

void vga_set_color(__u16 __color__) {
  __vga.__bc_color = __color__ >> 4 & 0xFF;
  __vga.__ch_color = __color__ & 0xFF;
}

void vga_clean() {
  __u16 __size = 0;

  while(__size < DEFAULT_WIDTH * DEFAULT_HEIGHT * 2) {
    __vga.__framebuffer[__size++] = 0x00;
    __vga.__framebuffer[__size++] = __vga.__bc_color << 4 | __vga.__ch_color;
  }
}

void vga_print_char(__u8 __ch__) {
  if(__vga.__current_col + 1 >= DEFAULT_WIDTH) {
    __vga.__current_row += 1;
    __vga.__current_col = 0;
  }
  __vga.__framebuffer[DEFAULT_WIDTH * __vga.__current_row * 2 + __vga.__current_col++ * 2] = __ch__;
}

void vga_set_ptr(__u8 __row__, __u8 __col__) {
  
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
