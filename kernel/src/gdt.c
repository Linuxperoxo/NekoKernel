/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : gdt.c                         |
 *    |  SRC MOD   : 05/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <gdt.h>

#define GDT_ENTRIES 5

static struct gdt_entry __gdt_entries[GDT_ENTRIES];
static struct gdt_ptr __gdt_ptr;

void gdtinit()
{
  __gdt_ptr.__limit = sizeof(struct gdt_entry)  * GDT_ENTRIES - 1;
  __gdt_ptr.__gdt_first_entry = (__u32) &__gdt_entries;

  /*
   * Definir  a primeira entrada (nula) do GDT, que é obrigatória
   */
  
  gdtsetentry(0x00, 0x00, 0x00, 0x00, 0x00, 0x00); // Null Segment
  
  /*
   *
   * Definir a segunda entrada - Segmento do código do Kernel (4GB)
   *
   */

  gdtsetentry(0x01, 0x00, 0xFFFF, 0x0F, 0xCF, 0x9A); // Kernel Code Segment
  
  /*
   *
   * Definir a terceira entrada - Segmento de dados do Kernel (4GB)
   *
   */

  gdtsetentry(0x02, 0x00, 0xFFFF, 0x0F, 0xCF, 0x92); // Kernel data Segment
  
  /*
   *
   * Definir a quarta entrada - Segmento do código de usuário (4GB)
   *
   */

  gdtsetentry(0x03, 0x00, 0xFFFF, 0x0F, 0xCF, 0xFA); // User code segment

  /*
   *
   * Definir a quinta entrada -  Segmento de dados de usuário (4GB)
   *
   */

  gdtsetentry(0x04, 0x00, 0xFFFF, 0x0F, 0xCF, 0xF2); // User data segment

  gdtflush((__u32) &__gdt_ptr);
}

void gdtsetentry(__u32 __index__, __u32 __base__, __u16 __limit__, __u8 __flags__, __u8 __gran__, __u8 __access__)
{
  __gdt_entries[__index__].__base_low = __base__ & 0xFFFF;
  __gdt_entries[__index__].__base_middle = (__base__ >> 16) & 0xFF;
  __gdt_entries[__index__].__base_high = (__base__ >> 24) & 0xFF;
  __gdt_entries[__index__].__limit = __limit__;
  __gdt_entries[__index__].__flags = (__flags__ & 0x0F) | (__gran__ & 0xF0);
  __gdt_entries[__index__].__access = __access__;
}

