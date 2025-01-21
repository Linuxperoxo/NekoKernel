/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : idt.h                         |
 *    |  SRC MOD   : 20/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __K_IDT__
#define __K_IDT__

#include <std/int.h>
#include <neko/kernel.h>

typedef __u16 idt_base_low;
typedef __u16 idt_seg_selec;
typedef __u8  idt_always0;
typedef __u8 idt_flags;
typedef __u16 idt_base_high;
typedef __u16 idt_limit_t;
typedef __u32 isr_int_t;
typedef void(*isr_routine_t)(int_regs_t*);

/*
 *
 * 1. __base_low: Parte baixa do endereço do ISR (Interrupt Service Routine).
 *  Representa os 16 bits menos significativos do endereço onde o manipulador de interrupção está localizado.
 *
 * 2. __seg_selector: Seletor de segmento do descritor de segmento de código.
 *  Contém o seletor de segmento usado para acessar o código do ISR na tabela de segmentos.
 *
 * 3. __always0: Este campo deve ser sempre 0.
 *  É reservado e não tem impacto no funcionamento da interrupção.
 *
 * 4. __flags: Flags que controlam como o processador deve tratar a interrupção.
 *  Inclui o tipo de interrupção (hardware/software), nível de privilégio (DPL) e a presença da interrupção (bit P).
 *
 * - Tipo de interrupção (se é uma interrupção de software, hardware, etc);
 * - Privacidade (DPL): Define o nível de privilégio (nível do anel) que pode
 *                      disparar a interrupção
 * -Present (P): Indica se a interrupção está presente e válida
 *
 * __u8 __flags -> 0000 0000
 *
 * (bit 7): Present Bit
 * (bit 6-5): DPL
 * (bit 4): Sempre 0
 * (bit 3-0): Gate Type
 *
 * 5. __base_high: Parte alta do endereço do ISR.
 *    Representa os 16 bits mais significativos do endereço do manipulador de interrupção.
 *
 */

typedef struct idt_entry_t
{
  idt_base_low  __base_low;
  idt_seg_selec __seg_selector;
  idt_always0   __always0;
  idt_flags     __flags;
  idt_base_high __base_high;
}__attribute__((packed)) idt_entry_t;

/*
 *
 * 1. __limit: Tamanho da IDT em Bytes (16 bits).
 *    Representa o tamanho total da tabela de descritores de interrupção (IDT). 
 *    O valor é expresso em bytes e é usado para definir o limite da IDT para o processador.
 *
 * 2. __idt_first_entry: Endereço para o início do nosso array de idt_entry (32 bits).
 *    Ponteiro para o primeiro elemento (entrada) da tabela de descritores de interrupção.
 *    Esse campo aponta para o local onde a IDT começa, fornecendo ao processador o endereço base da tabela.
 *
 */

typedef struct idt_ptr_t
{
  idt_limit_t  __limit;
  idt_entry_t* __idt_first_entry;
}__attribute__((packed)) idt_ptr_t;

/*
 *
 * 1. __int: Número da interrupção
 * 
 * 2. __routine: Rotina para essa interrupção
 *
 */

typedef struct isr_t
{
  isr_int_t     __int;
  isr_routine_t __routine;
}isr_t;

extern void idt_init(); 
extern void idt_install_coop_routine(__u8 __index__, isr_routine_t __routine__);
extern void idt_unistall_coop_routine(__u8 __index__);

#endif
