/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : idt.c                         |
 *    |  SRC MOD   : 02/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */


#include <idt.h>
#include <std/utils.h>
#include <std/int.h>
#include <std/io.h>
#include <sys/ports.h>
#include <sys/kernel.h>

#define IDT_ENTRIES 256
#define EXCEPTIONS_MSG_NUM 32
#define ISR_FOR_IRQ 16

/*
 *
 * Portas para o PIC primário (master)
 *
 */

#define PRI_PIC_COMMAND_PORT 0x20
#define PRI_PIC_DATA_PORT    0x21

/*
 *
 * Portas para PIC secundário (Escravo) 
 *
 */

#define SEC_PIC_COMMAND_PORT 0xA0
#define SEC_PIC_DATA_PORT    0xA1

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

  outb(PRI_PIC_COMMAND_PORT, 0x11); // Inicia o modo de configuração, agora o chip primário espera 4 comandos
  outb(SEC_PIC_COMMAND_PORT, 0x11); // Inicia o modo de configuração, agora o chip secundário espera mais 4 comandos

  outb(PRI_PIC_DATA_PORT, 0x20); // Vetor base para o PIC primário, vai de __idt_entries[32 - 39]
  outb(SEC_PIC_DATA_PORT, 0x28); // Vetor base para o PIC secundário, vai de __idt_entries[40 - 47]

  outb(PRI_PIC_DATA_PORT, 0x04); // Conecta o PIC secundário à linha IRQ2 do PIC primário 
  outb(SEC_PIC_DATA_PORT, 0x02); // Configura a linha de comunicação (IRQ2) do PIC primário para o secundário

  outb(PRI_PIC_DATA_PORT, 0x01); // Configura o modo de operação 8086/88 para o PIC primário
  outb(SEC_PIC_DATA_PORT, 0x01); // Configura o modo de operação 8086/88 para o PIC secundário

  outb(PRI_PIC_DATA_PORT, 0x00); // Libera as interrupções no PIC primário
  outb(SEC_PIC_DATA_PORT, 0x00); // Libera as interrupções no PIC secundário

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

/* 
 *
 * Exceções:
 * 
 * Exceção 0 - Division By Zero:
 * Ocorre quando o processador tenta dividir um número por zero.
 *
 * Exceção 1 - Debug:
 * Usada pelo depurador, é gerada quando uma instrução de depuração é encontrada, como o comando `int 3`.
 *
 * Exceção 2 - Non Maskable Interrupt (NMI):
 * Uma interrupção crítica que não pode ser mascarada. Frequentemente associada a falhas de hardware, como memória defeituosa.
 *
 * Exceção 3 - Breakpoint:
 * Gerada quando o processador encontra uma instrução de breakpoint (normalmente `int 3`), usada para depuração.
 *
 * Exceção 4 - Into Detected Overflow (Overflow):
 * Ocorre quando uma operação aritmética resulta em um overflow, ou seja, o valor excede o limite que pode ser representado no tipo de dado.
 *
 * Exceção 5 - Bound Range Exceeded:
 * Gerada quando o processador detecta um erro no uso da instrução `BOUND`, que verifica se um índice está dentro de um intervalo especificado.
 *
 * Exceção 6 - Invalid Opcode:
 * Ocorre quando o processador encontra um opcode (código de operação) inválido ou desconhecido.
 *
 * Exceção 7 - No Coprocessor:
 * Gerada quando uma instrução que requer um coprocessador (como a FPU) é executada, mas o coprocessador não está presente.
 *
 * Exceção 8 - Double Fault:
 * Ocorre quando uma exceção é gerada enquanto o processador ainda está lidando com outra exceção.
 * 
 * Exceção 9 - Coprocessor Segment Overrun:
 * Relacionada a um erro de acesso ao coprocessador, como um erro ao acessar o segmento de dados do coprocessador.
 *
 * Exceção 10 - Invalid TSS (Task State Segment):
 * Gerada quando o processador encontra um erro ao acessar o Task State Segment (TSS), que mantém o estado de uma tarefa no sistema.
 *
 * Exceção 11 - Stack Fault:
 * Ocorre quando há um erro relacionado à pilha, como um estouro de pilha ou uma violação de acesso.
 *
 * Exceção 12 - Page Fault:
 * Ocorre quando o processador tenta acessar uma página de memória que não está mapeada ou que está fora dos limites da memória física.
 *
 * Exceção 13 - General Protection Fault:
 * Gerada quando o processador detecta uma violação de proteção de memória, como acessar uma área de memória protegida ou executar uma operação inválida.
 *
 * Exceção 14 - Unknown Interrupt:
 * Uma interrupção desconhecida ou não mapeada. Ocorre quando o processador recebe uma interrupção inválida.
 *
 * Exceção 15 - Coprocessor Fault:
 * Gerada quando ocorre uma falha ao tentar acessar ou utilizar um coprocessador.
 *
 * Exceção 16 - Machine Check Exception:
 * Indica uma falha crítica de hardware, como falhas de memória ou falhas no próprio processador.
 *
 * Exceções 17 a 31 - None:
 * Essas exceções não são usadas pela arquitetura x86 e são reservadas para uso futuro ou personalização. Elas podem ser usadas em implementações específicas ou em extensões do processador.
 *
 */

char* __exceptions_messsagens[EXCEPTIONS_MSG_NUM] = {
  "Division By Zero",               // Exceção 0  - Division By Zero
  "Debug",                          // Exceção 1  - Debug
  "Non Maskable Interrupt",         // Exceção 2  - Non Maskable Interrupt (NMI)
  "Breakpoint",                     // Exceção 3  - Breakpoint
  "Into Detected Overflow",         // Exceção 4  - Into Detected Overflow (Overflow)
  "Out of Bounds",                  // Exceção 5  - Bound Range Exceeded
  "Invalid Opcode",                 // Exceção 6  - Invalid Opcode
  "No Coprocessor",                 // Exceção 7  - No Coprocessor (Coprocessor Not Available)
  "Double fault",                   // Exceção 8  - Double Fault
  "Coprocessor Segment Overrun",    // Exceção 9  - Coprocessor Segment Overrun
  "Bad TSS",                        // Exceção 10 - Invalid TSS (Task State Segment)
  "Stack Fault",                    // Exceção 11 - Stack Fault
  "Page Fault",                     // Exceção 12 - Page Fault
  "General Protection Fault",       // Exceção 13 - General Protection Fault
  "Unknown Interruption",           // Exceção 14 - Unknown Interrupt
  "Coprocessor Fault",              // Exceção 15 - Coprocessor Fault
  "Machine Check",                  // Exceção 16 - Machine Check Exception
  "None",                           // Exceção 17 - None
  "None",                           // Exceção 18 - None
  "None",                           // Exceção 19 - None
  "None",                           // Exceção 20 - None
  "None",                           // Exceção 21 - None
  "None",                           // Exceção 22 - None
  "None",                           // Exceção 23 - None
  "None",                           // Exceção 24 - None
  "None",                           // Exceção 25 - None
  "None",                           // Exceção 26 - None
  "None",                           // Exceção 27 - None
  "None",                           // Exceção 28 - None
  "None",                           // Exceção 29 - None
  "None",                           // Exceção 30 - None
  "None"                            // Exceção 31 - None
};

void isr_handler(struct InterruptRegisters* __regs_struct__)
{
  if(__regs_struct__->__int_num < 32)
  {
    printf("\nNEKO PANIC! : ");
    printf(__exceptions_messsagens[__regs_struct__->__int_num]);
    printf("\n");
    while(1);
  }

  if(__regs_struct__->__int_num >= 0x80)
  {

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

    outb(SEC_PIC_COMMAND_PORT, 0x20);
  }

  /*
   *
   * Fazemos o mesmo para o PIC Master, já que ele sempre vai ser usado, mesmo quando o PIC
   * secundário é usado, o PIC secundário passa o IRQ para o PIC primário pela linha IRQ2, 
   * então temos que sinalizar a ele também
   *
   */

  outb(PRI_PIC_COMMAND_PORT, 0x20);
}
