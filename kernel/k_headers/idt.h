#ifndef __IDT__
#define __IDT__

#include <types.h>

struct idt_entry_struct {
  __u16 __base_low;
  __u16 __sel;
  __u8 __always0;
  __u8 __flags;
  __u16 __base_high;
} __attribute__((packed));

struct idt_ptr_struct {
  __u16 __limit;
  __u32 __base;
} __attribute__((packed));

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
  __u32 __int_no, __err_code;
  __u32 __eip, __csm, __eflgs, __useresp, __ss;
};

void initidt();
void setidgate(__u8, __u32, __u16, __u8);
void isr_handler(struct InterruptRegisters*);

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
 * Para syscall
 *
 */

extern void isr128();
extern void isr177();

/*
 *
 * Solicitações de interrupção
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

#endif
