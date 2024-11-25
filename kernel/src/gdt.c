/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : gdt.c                         |
 *    |  SRC MOD   : 22/10/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include "gdt.h"

extern void gdt_flush(long unsigned int);

struct gdt_entry_struct gdt_entries[5];
struct gdt_ptr_struct gdt_ptr;

void initgdt()
{
  // Definir o limite e o endereço base da GDT

  gdt_ptr.__limit = (sizeof(struct gdt_entry_struct) * 5) - 1; // GDT tem 5 entradas.
  gdt_ptr.__base = (long unsigned int) &gdt_entries; // Endereço base do GDT, ou seja, primeira entrada
  
  // Definir a primeira entrada (nula) da GDT, que é obrigatória.
  setgdtgate(0, 0, 0, 0, 0); // Null segment

  // Definir a segunda entrada - Segmento de código do Kernel (4 GB)
  setgdtgate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
  
  // Definir a terceira entrada - Segmento de dados do Kernel (4 GB)
  setgdtgate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment

  // Definir a quarta entrada - Segmento de código de usuário (4 GB)
  setgdtgate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code segment
  
  // Definir a quinta entrada - Segmento de dados de usuário (4 GB)
  setgdtgate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data segment
  
  // Atualizar a GDT com o ponteiro correto
  gdt_flush((long unsigned int) &gdt_ptr);
}

// Função para configurar uma entrada na GDT
void setgdtgate(__u32 __num__, __u32 __base__, __u32 __limit__, __u8 __acess__, __u8 __gran)
{

  gdt_entries[__num__].__base_low = (__base__ & 0xFFFF);
  gdt_entries[__num__].__base_middle = (__base__ >> 16) & 0xFF;
  gdt_entries[__num__].__base_high = (__base__ >> 24) & 0xFF;
  
  gdt_entries[__num__].__limit = (__limit__ & 0xFFFF);
  gdt_entries[__num__].__flags = (__limit__ >> 16) & 0x0F;
  gdt_entries[__num__].__flags |= (__gran & 0xF0);
  
  gdt_entries[__num__].__acess = __acess__;
}
