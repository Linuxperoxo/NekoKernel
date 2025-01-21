/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : idt.c                         |
 *    |  SRC MOD   : 20/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <neko/idt.h>
#include <std/utils.h>
#include <std/int.h>
#include <std/io.h>
#include <sys/ports.h>
#include <neko/kernel.h>

#define CPU_EXCEPTIONS_NUM 32
#define IDT_ENTRIES 256

#define ISR(INT_NUM) extern void isr##INT_NUM()
#define IRQ(INT_NUM) extern void irq##INT_NUM()
#define SYSCALL(INT_NUM) extern void isr_syscall##INT_NUM()

/*
 *
 * All IRQs
 *
 */

ISR(0);
ISR(1);
ISR(2);
ISR(3);
ISR(4);
ISR(5);
ISR(6);
ISR(7);
ISR(8);
ISR(9);
ISR(10);
ISR(11);
ISR(12);
ISR(13);
ISR(14);
ISR(15);
ISR(16);
ISR(17);
ISR(18);
ISR(19);
ISR(20);
ISR(21);
ISR(22);
ISR(23);
ISR(24);
ISR(25);
ISR(26);
ISR(27);
ISR(28);
ISR(29);
ISR(30);
ISR(31);

IRQ(32);
IRQ(33);
IRQ(34);
IRQ(35);
IRQ(36);
IRQ(37);
IRQ(38);
IRQ(39);
IRQ(40);
IRQ(41);
IRQ(42);
IRQ(43);
IRQ(44);
IRQ(45);
IRQ(46);
IRQ(47);

SYSCALL(128);
SYSCALL(177);

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

struct idt_entry_t __idt_entries[IDT_ENTRIES];
struct idt_ptr_t   __idt_ptr;
struct isr_t       __isr_table[IDT_ENTRIES];

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

char* __exceptions_messsagens[CPU_EXCEPTIONS_NUM] = {
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

void idt_entry(__u8 __num__, __u32 __base__, __u16 __seg_selector__, __u8 __flags__)
{
  __idt_entries[__num__].__base_low = __base__ & 0xFFFF;
  __idt_entries[__num__].__base_high = (__base__ >> 16) & 0xFFFF;
  __idt_entries[__num__].__seg_selector = __seg_selector__;
  __idt_entries[__num__].__always0 = 0x00;
  __idt_entries[__num__].__flags = __flags__ | 0x60;
}

void isr_handler(int_regs_t* __regs_struct__)
{
  if(__regs_struct__->__interrupt < CPU_EXCEPTIONS_NUM)
  {
    printf("\nNEKO PANIC! : ");
    printf(__exceptions_messsagens[__regs_struct__->__interrupt]);
    printf("\n");
    
    __asm__ volatile(
      ".LExeceptionLoop:"
      "jmp .LExeceptionLoop\n"
      :
      :
      :
    );
  } 
}

void irq_handler(int_regs_t* __regs_struct__)
{
  if(__isr_table[__regs_struct__->__interrupt].__routine)
    __isr_table[__regs_struct__->__interrupt].__routine(__regs_struct__);
  
  if(__regs_struct__-> __interrupt >= 40)
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

void syscall_handler(int_regs_t* __regs_struct__)
{
  if(__isr_table[__regs_struct__->__interrupt].__routine)
    __isr_table[__regs_struct__->__interrupt].__routine(__regs_struct__);
}

void idt_init() 
{
  __idt_ptr.__limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
  __idt_ptr.__idt_first_entry = (idt_entry_t*)&__idt_entries;

  /*
   *
   * Comando para configurar o CHIP PIC Master e Slave
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

  __isr_table[0x00].__int = (__u32)&isr0;
  __isr_table[0x01].__int = (__u32)&isr1;
  __isr_table[0x02].__int = (__u32)&isr2;
  __isr_table[0x03].__int = (__u32)&isr3;
  __isr_table[0x04].__int = (__u32)&isr4;
  __isr_table[0x05].__int = (__u32)&isr5;
  __isr_table[0x06].__int = (__u32)&isr6;
  __isr_table[0x07].__int = (__u32)&isr7;
  __isr_table[0x08].__int = (__u32)&isr8;
  __isr_table[0x09].__int = (__u32)&isr9;
  __isr_table[0x0A].__int = (__u32)&isr10;
  __isr_table[0x0B].__int = (__u32)&isr11;
  __isr_table[0x0C].__int = (__u32)&isr12;
  __isr_table[0x0D].__int = (__u32)&isr13;
  __isr_table[0x0E].__int = (__u32)&isr14;
  __isr_table[0x0F].__int = (__u32)&isr15;
  __isr_table[0x10].__int = (__u32)&isr16;
  __isr_table[0x11].__int = (__u32)&isr17;
  __isr_table[0x12].__int = (__u32)&isr18;
  __isr_table[0x13].__int = (__u32)&isr19;
  __isr_table[0x14].__int = (__u32)&isr20;
  __isr_table[0x15].__int = (__u32)&isr21;
  __isr_table[0x16].__int = (__u32)&isr22;
  __isr_table[0x17].__int = (__u32)&isr23;
  __isr_table[0x18].__int = (__u32)&isr24;
  __isr_table[0x19].__int = (__u32)&isr25;
  __isr_table[0x1A].__int = (__u32)&isr26;
  __isr_table[0x1B].__int = (__u32)&isr27;
  __isr_table[0x1C].__int = (__u32)&isr28;
  __isr_table[0x1D].__int = (__u32)&isr29;
  __isr_table[0x1E].__int = (__u32)&isr30;
  __isr_table[0x1F].__int = (__u32)&isr31;
  
  __isr_table[0x20].__int = (__u32)&irq32;
  __isr_table[0x21].__int = (__u32)&irq33;
  __isr_table[0x22].__int = (__u32)&irq34;
  __isr_table[0x23].__int = (__u32)&irq35;
  __isr_table[0x24].__int = (__u32)&irq36;
  __isr_table[0x25].__int = (__u32)&irq37;
  __isr_table[0x26].__int = (__u32)&irq38;
  __isr_table[0x27].__int = (__u32)&irq39;
  __isr_table[0x28].__int = (__u32)&irq40;
  __isr_table[0x29].__int = (__u32)&irq41;
  __isr_table[0x30].__int = (__u32)&irq42;
  __isr_table[0x31].__int = (__u32)&irq43;
  __isr_table[0x32].__int = (__u32)&irq44;
  __isr_table[0x33].__int = (__u32)&irq45;
  __isr_table[0x34].__int = (__u32)&irq46;
  __isr_table[0x35].__int = (__u32)&irq47;

  __isr_table[0x80].__int = (__u32)&isr_syscall128;
  __isr_table[0xB1].__int = (__u32)&isr_syscall177;

  /*
   *
   * Exeptions ISR
   *
   */

  idt_entry(0x00, __isr_table[0].__int, 0x08, 0x8E);
  idt_entry(0x01, __isr_table[1].__int, 0x08, 0x8E);
  idt_entry(0x02, __isr_table[2].__int, 0x08, 0x8E);
  idt_entry(0x03, __isr_table[3].__int, 0x08, 0x8E);
  idt_entry(0x04, __isr_table[4].__int, 0x08, 0x8E);
  idt_entry(0x05, __isr_table[5].__int, 0x08, 0x8E);
  idt_entry(0x06, __isr_table[6].__int, 0x08, 0x8E);
  idt_entry(0x07, __isr_table[7].__int, 0x08, 0x8E);
  idt_entry(0x08, __isr_table[8].__int, 0x08, 0x8E);
  idt_entry(0x09, __isr_table[9].__int, 0x08, 0x8E);
  idt_entry(0x0A, __isr_table[10].__int, 0x08, 0x8E);
  idt_entry(0x0B, __isr_table[11].__int, 0x08, 0x8E);
  idt_entry(0x0C, __isr_table[12].__int, 0x08, 0x8E);
  idt_entry(0x0D, __isr_table[13].__int, 0x08, 0x8E);
  idt_entry(0x0E, __isr_table[14].__int, 0x08, 0x8E);
  idt_entry(0x0F, __isr_table[15].__int, 0x08, 0x8E);
  idt_entry(0x10, __isr_table[16].__int, 0x08, 0x8E);
  idt_entry(0x11, __isr_table[17].__int, 0x08, 0x8E);
  idt_entry(0x12, __isr_table[18].__int, 0x08, 0x8E);
  idt_entry(0x13, __isr_table[19].__int, 0x08, 0x8E);
  idt_entry(0x14, __isr_table[20].__int, 0x08, 0x8E);
  idt_entry(0x15, __isr_table[21].__int, 0x08, 0x8E);
  idt_entry(0x16, __isr_table[22].__int, 0x08, 0x8E);
  idt_entry(0x17, __isr_table[23].__int, 0x08, 0x8E);
  idt_entry(0x18, __isr_table[24].__int, 0x08, 0x8E);
  idt_entry(0x19, __isr_table[25].__int, 0x08, 0x8E);
  idt_entry(0x1A, __isr_table[26].__int, 0x08, 0x8E);
  idt_entry(0x1B, __isr_table[27].__int, 0x08, 0x8E);
  idt_entry(0x1C, __isr_table[28].__int, 0x08, 0x8E);
  idt_entry(0x1D, __isr_table[29].__int, 0x08, 0x8E);
  idt_entry(0x1E, __isr_table[30].__int, 0x08, 0x8E);
  idt_entry(0x1F, __isr_table[31].__int, 0x08, 0x8E);

  /*
   *
   * IRQs ISR
   *
   */

  idt_entry(0x20, __isr_table[32].__int, 0x08, 0x8E);
  idt_entry(0x21, __isr_table[33].__int, 0x08, 0x8E);
  idt_entry(0x22, __isr_table[34].__int, 0x08, 0x8E);
  idt_entry(0x23, __isr_table[35].__int, 0x08, 0x8E);
  idt_entry(0x24, __isr_table[36].__int, 0x08, 0x8E);
  idt_entry(0x25, __isr_table[37].__int, 0x08, 0x8E);
  idt_entry(0x26, __isr_table[38].__int, 0x08, 0x8E);
  idt_entry(0x27, __isr_table[39].__int, 0x08, 0x8E);
  idt_entry(0x28, __isr_table[40].__int, 0x08, 0x8E);
  idt_entry(0x29, __isr_table[41].__int, 0x08, 0x8E);
  idt_entry(0x30, __isr_table[42].__int, 0x08, 0x8E);
  idt_entry(0x31, __isr_table[43].__int, 0x08, 0x8E);
  idt_entry(0x32, __isr_table[44].__int, 0x08, 0x8E);
  idt_entry(0x33, __isr_table[45].__int, 0x08, 0x8E);
  idt_entry(0x34, __isr_table[46].__int, 0x08, 0x8E);
  idt_entry(0x35, __isr_table[47].__int, 0x08, 0x8E);

  /*
   *
   * Syscall ISR
   *
   */

  idt_entry(0x80, __isr_table[128].__int, 0x08, 0x8E);
  idt_entry(0xB1, __isr_table[177].__int, 0x08, 0x8E);

  __asm__ volatile(
    "lidt (%0)\n"
    "sti"
    :
    :"p"(&__idt_ptr)
    :
  );
}

void idt_install_coop_routine(__u8 __index__, isr_routine_t __routine__)
{
  if(__index__ > 0x1F)
    __isr_table[__index__].__routine = __routine__;
}

void idt_unistall_coop_routine(__u8 __index__)
{
  if(__index__ > CPU_EXCEPTIONS_NUM)
    __isr_table[__index__].__routine = 0x00;
}
