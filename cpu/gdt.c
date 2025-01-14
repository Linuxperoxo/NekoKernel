/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : gdt.c                         |
 *    |  SRC MOD   : 09/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <gdt.h>
#include <std/int.h>

#define GDT_ENTRIES_TOTAL   0x08
#define GDT_K_CODE_SEGMENT  0b00001000
#define GDT_K_DATA_SEGMENT  0b00010000
#define GDT_K_STACK_SEGMENT 0b00011000

static struct gdtEntry __gdt_entries[GDT_ENTRIES_TOTAL];
static struct gdtPtr   __gdt_ptr;

void gdt_init()
{
  __gdt_ptr.__gdt_size  = (sizeof(struct gdtEntry) * GDT_ENTRIES_TOTAL) - 1;
  __gdt_ptr.__gdt_entry = (__u32)&__gdt_entries;
  
  /*
   *
   * Primeira entrada (Null) do GDT, ela é obrigatória
   *
   */

  gdt_entry(0x00, 0x00, 0x00, 0x00, 0x00); // Null Segment
  
  /*
   *
   * Definir a segunda entrada - Segmento do código do Kernel (4GB)
   *
   */

  gdt_entry(0x01, 0x00, 0xFFFFF, 0x0C, 0x9A); // Kernel Code Segment
  
  /*
   *
   * Definir a terceira entrada - Segmento de dados do Kernel (4GB)
   *
   */

  gdt_entry(0x02, 0x00, 0xFFFFF, 0x0C, 0x92); // Kernel data Segment
  
  /*
   *
   * Definir a quarta entrada - Segmento do código de usuário (4GB)
   *
   */
  
  __asm__ volatile(

    /*
     *
     * Carregando lgdt
     *
     */

    "lgdt (%0)\n"
    
    /*
     * 
     * Usamos Far Jmp para modificar o registrador CS, já que 
     * não podemos manipular ele diretamente usando a instrução MOV
     *
     */

    "ljmp %1, $.L1\n" // Far Jump instruction 

    ".L1:\n" // .L = local label, without .L = global label
    
    /*
     *
     * Configurando registradores de segmento
     *
     */

    "movw %2, %%ax\n"
    "movw %%ax, %%ss\n"
    "movw %%ax, %%ds\n"
    "movw %%ax, %%es\n"
    "movw %%ax, %%fs\n"
    "movw %%ax, %%gs\n"
      
    /*
     *
     * Agora todos os outros registradores apontam para o segmento de dados
     * do kernel
     *
     */

    :
    : "r"(&__gdt_ptr), "i"(GDT_K_CODE_SEGMENT), "i"(GDT_K_DATA_SEGMENT)
    : "%ax"
  );
}

void gdt_entry(__u8 __gdt_index__, __u32 __base__, __u32 __limit_20bits__, __u8 __gran__, __u8 __access__)
{
  __gdt_entries[__gdt_index__].__seg_limit_low  = __limit_20bits__;
  __gdt_entries[__gdt_index__].__seg_limit_high = (__limit_20bits__ >> 16) & 0x0F;
  
  __gdt_entries[__gdt_index__].__base_low       = __base__ & 0xFFFF;
  __gdt_entries[__gdt_index__].__base_mid       = (__base__ >> 16) & 0xFF;
  __gdt_entries[__gdt_index__].__base_high      = (__base__ >> 24) & 0xFF;
  
  __gdt_entries[__gdt_index__].__access         = __access__;
  __gdt_entries[__gdt_index__].__gran           = __gran__ & 0x0F;
}
