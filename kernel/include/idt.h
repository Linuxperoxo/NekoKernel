/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : idt.h                         |
 *    |  SRC MOD   : 13/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <std/types.h>

struct idt_entry {
  __u16 __base_low; // Parte baixa do endereço do ISR
  __u16 __seg_selector; // Valor do seletor de segmento que esrá utilizando para acessar o código do ISR 
  __u8 __always0; // Sempre 0
  
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

  __u8 __flags; // Flags para controlar como o processador trata a interrupção, Tipo de interrupção Hardware ou Software, Privacidade 
  __u16 __base_high; // Parte alta do endereço do ISR
}__attribute__((packed));

struct idt_ptr {
  __u16 __limit; // Tamanho da IDT em Bytes (16 bits)
  __u32 __idt_first_entry; // Endereço para o ínicio do nosso array de idt_entry (32 bits)  
};

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

struct InterruptRegisters {
  __u32 __cr2;
  __u32 __ds;
  __u32 __edi;
  __u32 __esi;
  __u32 __ebp;
  __u32 __esp;
  __u32 __ebx;
  __u32 __edx;
  __u32 __ecx;
  __u32 __eax;
  __u32 __int_num;
  __u32 __error_code;
  
  /*
   *
   * Registradores que são empilhados automaticamento
   * quando ocorre uma interrupção que vai ser atendida
   *
   */

  __u32 __eip;
  __u32 __csm;
  __u32 __eflags;
  __u32 __useresp;
  __u32 __ss;
};

void idtinit(); 
//extern void idtinit();
extern void idtsetgate(__u8, __u32, __u16, __u8);
extern void idtflush(struct idt_ptr*);
extern void isr_handler(struct InterruptRegisters*);
extern void irq_install_isr_handler(__u8, void(*)(struct InterruptRegisters*));
extern void irq_unistall_isr_handler(__u8);
extern void irq_isr_handler(struct InterruptRegisters*);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/*
 *
 * IRQs
 *
 */

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/*
 *
 * Syscall 
 *
 */

extern void isr128();
extern void isr177();
