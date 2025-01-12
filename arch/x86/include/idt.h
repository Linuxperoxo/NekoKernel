/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : idt.h                         |
 *    |  SRC MOD   : 03/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __K_IDT__
#define __K_IDT__

#include <std/int.h>
#include <sys/kernel.h>

struct idt_entry {
  __u16 __base_low;     // Parte baixa do endereço do ISR
  __u16 __seg_selector; // Valor do seletor de segmento que esrá utilizando para acessar o código do ISR 
  __u8 __always0;       // Sempre 0
  
  /*
   *
   * FLAGS:
   *
   * Flags que controlam como o processador deve tratar a interrupção, incluindo aspectos
   * como:
   *
   * * Tipo de interrupção (se é uma interrupção de software, hardware, etc);
   * * Privacidade (DPL): Define o nível de privilégio (nível do anel) que pode
   *                      disparar a interrupção
   * * Present (P): Indica se a interrupção está presente e válida
   *
   * __u8 __flags -> 0000 0000
   *
   * (bit 7): Present Bit
   * (bit 6-5): DPL
   * (bit 4): Sempre 0
   * (bit 3-0): Gate Type
   */

  __u8 __flags;      // Flags para controlar como o processador trata a interrupção, Tipo de interrupção Hardware ou Software, Privacidade 
  __u16 __base_high; // Parte alta do endereço do ISR
}__attribute__((packed));

struct idt_ptr {
  __u16 __limit;           // Tamanho da IDT em Bytes (16 bits)
  __u32 __idt_first_entry; // Endereço para o ínicio do nosso array de idt_entry (32 bits)  
}__attribute__((packed));

/*
 *
 * struct idt_ptr Membros:
 *
 * 1. __limit: Tamanho do IDT em Bytes.
 *
 * EXEMPLO:
 *  Se temos 3 segmentos, ou seja, 3 structs idt_entry, o __limit vai ser o seguinte: 
 *
 *  __idt_prt.__limit = sizeof(gdt_entry) * 3 - 1;
 *
 *  - 1 pois começamos no índice 0, igual um array
 *
 * 2. __idt_first_entry: Ponteiro para o início do array de idt_entry
 *
 * Para entender melhor veja o arquivo idt.c
 *
 */

extern void idt_init(); 
extern void irq_install_routine(__u8, void(*)(struct InterruptRegisters*));
extern void irq_unistall_routine(__u8);

#endif
