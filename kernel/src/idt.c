/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : idt.c                         |
 *    |  SRC MOD   : 13/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */


#include <idt.h>
#include <std/utils.h>
#include <std/io.h>
#include <std/ports.h>

#define IDT_ENTRIES 256
#define EXCEPTIONS_MSG_NUM 28
#define ISR_FOR_IRQ 16

/*
 *
 * PIC Ports
 *
 */

#define PRI_PIC_COMMAND_PORT 0x20
#define PRI_PIC_DATA_PORT 0x21
#define SEC_PIC_COMMAND_PORT 0xA0
#define SEC_PIC_DATA_PORT 0xA1

struct idt_entry __idt_entries[IDT_ENTRIES];
struct idt_ptr __idt_ptr;

void idtinit() 
{
  __idt_ptr.__limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
  __idt_ptr.__idt_first_entry = (__u32)&__idt_entries;

  memset(&__idt_entries, 0x00, sizeof(struct idt_entry) * IDT_ENTRIES);

  /*
   *
   * Comando para configurar o CHIP PIC Master e Slave
   *
   */

  /*
   *
   * O PIC Master ou primário recebe interrupções de I/O de IRQ0 - IRQ7, porém,
   * ele usar a linha do IRQ2 para se comunicar como PIC Slave ou secundário, ou seja,
   * qualquer interrupção vinda pelo IRQ2 foir uma interrupção qe veio do PIC Slave, IRQ8 - IRQ15,
   * com isso, no total vamos ter 15 interrupções possíveis.
   *
   * O PIC recebe IRQs, envia para o processador o vetor do IDT para essa interrupção. O PIC envia 
   * através do barramento de controle. Quando o processador recebe o vetor de interrupção, ele usa o
   * vetor para procurar o endereço do ISR associada na entrada IDT, que contém o endereço da rotina de 
   * tratamento (ISR) associada à interrupção.
   *
   * EXEMPLO: Digamos que o PIC recebe uma interrupção no PIC Master, IRQ0, ele vai enviar o vetor dessa
   * interrupção, vetor 32 do IDT, o processador vai consultar o __idt_entries[32], nessa entrada vai ter 
   * um ISR associado a essa interrupção, imagine o ISR sendo um ponteiro para uma função, e depois disso
   * o processador vai dar jmp para essa rotina/função.
   *
   */

  outb(0x20, 0x11); // Inicia o modo de configuração, agora o chip primário espera 4 comandos
  outb(0x20, 0x11); // Inicia o modo de configuração, agora o chip secundário espera mais 4 comandos

  outb(0x21, 0x20); // Vetor base para o PIC primário, vai de __idt_entries[32 - 39]
  outb(0xA1, 0x28); // Vetor base para o PIC secundário, vai de __idt_entries[40 - 47]

  outb(0x21, 0x04); // Conecta o PIC secundário à linha IRQ2 do PIC primário 
  outb(0x41, 0x02); // Configura a linha de comunicação (IRQ2) do PIC primário para o secundário

  outb(0x21, 0x01); // Configura o modo de operação 8086/88 para o PIC primário
  outb(0x41, 0x01); // Configura o modo de operação 8086/88 para o PIC secundário

  outb(0x21, 0x00); // Libera as interrupções no PIC primário
  outb(0xA1, 0x00); // Libera as interrupções no PIC secundário

  idtsetgate(0x00, (__u32)isr0, 0x08, 0x8E);
  idtsetgate(0x01, (__u32)isr1, 0x08, 0x8E);
  idtsetgate(0x02, (__u32)isr2, 0x08, 0x8E);
  idtsetgate(0x03, (__u32)isr3, 0x08, 0x8E);
  idtsetgate(0x04, (__u32)isr4, 0x08, 0x8E);
  idtsetgate(0x05, (__u32)isr5, 0x08, 0x8E);
  idtsetgate(0x06, (__u32)isr6, 0x08, 0x8E);
  idtsetgate(0x07, (__u32)isr7, 0x08, 0x8E);
  idtsetgate(0x08, (__u32)isr8, 0x08, 0x8E);
  idtsetgate(0x09, (__u32)isr9, 0x08, 0x8E);
  idtsetgate(0x0A, (__u32)isr10, 0x08, 0x8E);
  idtsetgate(0x0B, (__u32)isr11, 0x08, 0x8E);
  idtsetgate(0x0C, (__u32)isr12, 0x08, 0x8E);
  idtsetgate(0x0D, (__u32)isr13, 0x08, 0x8E);
  idtsetgate(0x0E, (__u32)isr14, 0x08, 0x8E);
  idtsetgate(0x0F, (__u32)isr15, 0x08, 0x8E);
  idtsetgate(0x10, (__u32)isr16, 0x08, 0x8E);
  idtsetgate(0x11, (__u32)isr17, 0x08, 0x8E);
  idtsetgate(0x12, (__u32)isr18, 0x08, 0x8E);
  idtsetgate(0x13, (__u32)isr19, 0x08, 0x8E);
  idtsetgate(0x14, (__u32)isr20, 0x08, 0x8E);
  idtsetgate(0x15, (__u32)isr21, 0x08, 0x8E);
  idtsetgate(0x16, (__u32)isr22, 0x08, 0x8E);
  idtsetgate(0x17, (__u32)isr23, 0x08, 0x8E);
  idtsetgate(0x18, (__u32)isr24, 0x08, 0x8E);
  idtsetgate(0x19, (__u32)isr25, 0x08, 0x8E);
  idtsetgate(0x1A, (__u32)isr26, 0x08, 0x8E);
  idtsetgate(0x1B, (__u32)isr27, 0x08, 0x8E);
  idtsetgate(0x1C, (__u32)isr28, 0x08, 0x8E);
  idtsetgate(0x1D, (__u32)isr29, 0x08, 0x8E);
  idtsetgate(0x1E, (__u32)isr30, 0x08, 0x8E);
  idtsetgate(0x1F, (__u32)isr31, 0x08, 0x8E);

  /*
   *
   * IRQs
   *
   */

  idtsetgate(0x20, (__u32)irq0, 0x08, 0x8E);
  idtsetgate(0x21, (__u32)irq1, 0x08, 0x8E);
  idtsetgate(0x22, (__u32)irq2, 0x08, 0x8E);
  idtsetgate(0x23, (__u32)irq3, 0x08, 0x8E);
  idtsetgate(0x24, (__u32)irq4, 0x08, 0x8E);
  idtsetgate(0x25, (__u32)irq5, 0x08, 0x8E);
  idtsetgate(0x26, (__u32)irq6, 0x08, 0x8E);
  idtsetgate(0x27, (__u32)irq7, 0x08, 0x8E);
  idtsetgate(0x28, (__u32)irq8, 0x08, 0x8E);
  idtsetgate(0x29, (__u32)irq9, 0x08, 0x8E);
  idtsetgate(0x30, (__u32)irq10, 0x08, 0x8E);
  idtsetgate(0x31, (__u32)irq11, 0x08, 0x8E);
  idtsetgate(0x32, (__u32)irq12, 0x08, 0x8E);
  idtsetgate(0x33, (__u32)irq13, 0x08, 0x8E);
  idtsetgate(0x34, (__u32)irq14, 0x08, 0x8E);
  idtsetgate(0x35, (__u32)irq15, 0x08, 0x8E);

  /*
   *
   * Syscall
   *
   */

  idtsetgate(0x80, (__u32)isr128, 0x08, 0x8E);
  idtsetgate(0xB1, (__u32)isr177, 0x08, 0x8E);

  idtflush(&__idt_ptr);
}

void idtsetgate(__u8 __num__, __u32 __base__, __u16 __seg_selector__, __u8 __flags__)
{
  __idt_entries[__num__].__base_low = __base__ & 0xFFFF;
  __idt_entries[__num__].__base_high = (__base__ >> 16) & 0xFFFF;
  __idt_entries[__num__].__seg_selector = __seg_selector__;
  __idt_entries[__num__].__always0 = 0x00;
  __idt_entries[__num__].__flags = __flags__ | 0x60;
}

void idtflush(struct idt_ptr* __idt_ptr__)
{
  __asm__ volatile(
    "movl %0, %%eax\n"
    "lidt (%%eax)\n"
    "sti\n"
    :
    : "p"(__idt_ptr__)
    : "%eax"
  );
}

__u8* __exceptions_messsagens[EXCEPTIONS_MSG_NUM] = {
  "Division By Zero",
  "Debug",
  "Non Maskble Interrupt",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",
  "Double fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Stack Fault",
  "General Protection Fault",
  "Page Fault",
  "Unknown Interruption",
  "Coprocessor Fault",
  "Machine Check",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};

void isr_handler(struct InterruptRegisters* __regs_struct__)
{
  if(__regs_struct__->__int_num < 32)
  {
    printf("NEKO PANIC! : ");
    printf(__exceptions_messsagens[__regs_struct__->__int_num]);
    printf("\n");
    while(1);
  }
}

void* __irq_isr[ISR_FOR_IRQ] = { 
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_isr_handler(__u8 __irq__, void (*handler)(struct InterruptRegisters*))
{
  __irq_isr[__irq__] = handler;
}

void irq_unistall_isr_handler(__u8 __irq__)
{
  __irq_isr[__irq__] = 0x00;
}

void irq_isr_handler(struct InterruptRegisters* __regs_struct__)
{
  void (*handler)(struct InterruptRegisters*);

  handler = __irq_isr[__regs_struct__->__int_num - 32];

  if(handler)
  {
    handler(__regs_struct__);
  }

  if(__regs_struct__-> __int_num >= 40)
  {
    
    /*
     *
     * Enviamos o comando de EOI(End of Interruption) para o PIC Slave caso 
     * o número da interrupção esteja na faixa do PIC Slave, o EOI serve para
     * sinalizar ao PIC que a interrupção foi tratada, permitindo que novas interrupções
     * sejam processadas
     *
     */

    outb(0xA0, 0x20);
  }

  /*
   *
   * Fazemos o mesmo para o PIC Master, já que ele sempre vai ser usado, mesmo quando o PIC
   * secundário é usado, o PIC secundário passa o IRQ para o PIC primário pela linha IRQ2, 
   * então temos que sinalizar a ele também
   *
   */

  outb(0x20, 0x20);
}
