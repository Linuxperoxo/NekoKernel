/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : vga.c                         |
 *    |  SRC MOD   : 19/01/2025                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#include <video/vga/vga.h>
#include <std/int.h>
#include <std/utils.h>
#include <sys/ports.h>
#include <sys/vfs.h>

/*
 *
 * Internal Functions
 *
 */

static __u8 vga_write(offset_t __offset__, void* __buffer__)
{
  ((__u8*)VGA_FRAMEBUFFER_ADDRS)[__offset__] = *((__u8*)__buffer__);
  
  return 0;
}

/*
 *
 * vga.h Functions
 *
 */

void vga_init()
{
  vfs_mkchfile("/dev/video", ROOT_UID, ROOT_GID, READ_O | WRITE_O, NULL, &vga_write);
}

void vga_screen_down()
{
  for(__u16 __y = 0; __y < DEFAULT_HEIGHT; __y++)
  {
    for(__u16 __x = 0; __x < DEFAULT_WIDTH; __x++)
    {
      ((__u8*)VGA_FRAMEBUFFER_ADDRS)[((__y - 1) * DEFAULT_WIDTH * 2) + (__x * 2)] = ((__u8*)VGA_FRAMEBUFFER_ADDRS)[(__y * DEFAULT_WIDTH * 2) + (__x * 2)];
    }
  }

  for(__u16 __x = 0; __x < DEFAULT_WIDTH; __x++)
  {
    ((__u8*)VGA_FRAMEBUFFER_ADDRS)[((DEFAULT_HEIGHT - 1) * DEFAULT_WIDTH * 2) + (__x * 2)] = 0x00;
  }
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

