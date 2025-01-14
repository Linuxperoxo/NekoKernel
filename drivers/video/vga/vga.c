/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : vga.c                         |
 *    |  SRC MOD   : 14/01/2025                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#include <video/vga/vga.h>
#include <std/int.h>
#include <sys/ports.h>

void vga_screen_down(struct VGAState* __vga__)
{
  for(__u16 __y = 0; __y < DEFAULT_HEIGHT; __y++)
  {
    for(__u16 __x = 0; __x < DEFAULT_WIDTH; __x++)
    {
      __vga__->__framebuffer[((__y - 1) * DEFAULT_WIDTH * 2) + (__x * 2)] = __vga__->__framebuffer[(__y * DEFAULT_WIDTH * 2) + (__x * 2)];
    }
  }

  for(__u16 __x = 0; __x < DEFAULT_WIDTH; __x++)
  {
    __vga__->__framebuffer[((DEFAULT_HEIGHT - 1) * DEFAULT_WIDTH * 2) + (__x * 2)] = 0x00;
  }
}

void vga_put_char(struct VGAState* __vga__, const char __ch__)
{
  __vga__->__framebuffer[(DEFAULT_WIDTH * __vga__->__current_row * 2) + (__vga__->__current_col * 2)] = __ch__;
  __vga__->__framebuffer[(DEFAULT_WIDTH * __vga__->__current_row * 2) + ((__vga__->__current_row * 2) + 1)] = ((__vga__->__bc_color << 4) & 0x70) | (__vga__->__ch_color & 0x0F);
  __vga__->__last_put = __ch__;
  __vga__->__current_col += 1;
}

void vga_mov_ptr(const __u8 __row__, const __u8 __col__)
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

