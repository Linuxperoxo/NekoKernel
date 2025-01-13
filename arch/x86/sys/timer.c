/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : timer.c                       |
 *    |  SRC MOD   : 10/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <timer.h>
#include <std/int.h>
#include <idt.h>
#include <std/io.h>
#include <task.h>
#include <sys/kernel.h>
#include <sys/ports.h>

#define FREQUENCY 1000
#define TIMER_INT_NUM 0x00
#define DIVISOR_NUM 1193180

#define TIMER_CHANNEL_0_PORT 0x40
#define TIMER_CHANNEL_1_PORT 0x41
#define TIMER_CHANNEL_2_PORT 0x42
#define TIMER_COMMAND_PORT   0x43

/*
 *
 * O PIC trabalha a uma frequência de 119318.16666 Mhz
 *
 */

static __u32 __divisor   = DIVISOR_NUM / FREQUENCY; // Gera interrupção a cada 1ms
static __u32 __sys_clock = 0x00;
static __u16 __chrono    = 0x00;

void timer_handler(struct InterruptRegisters* __regs__)
{
  __sys_clock += 1;
  __chrono    += 1;

  task_switch(__regs__);
}

void timer_init()
{
  idt_install_coop_routine(0x20, &timer_handler);
  
  /*
   *
   * O comando 0x36 configura o PIT para gerar uma 
   * interrupção periódica no modo 3 ("Square Wave Generator").
   *
   * Bits 7-6: Seleção do canal (00: Canal 0)
   * Bits 5-4: Modo (11: Modo 3, "Square wave generator")
   * Bit 3: Leitura binária
   * Bits 2-0: Operação no registrador de dados (padrão)
   * .
   */

  outb(TIMER_COMMAND_PORT, 0x36);

  /*
   *
   * Enviandos o bytes para o canal 0 
   *
   */

  outb(TIMER_CHANNEL_0_PORT,(__u8)(__divisor & 0xFF)); // Bytes menos significativos
  outb(TIMER_CHANNEL_0_PORT,(__u8)((__divisor >> 8) & 0xFF)); // Bytes mais significativos
}

void sleep_for(__u32 __ms__)
{
  __asm__ volatile(
    "xorl %%ebx, %%ebx\n"
    "movl %%ebx, (%1)\n"

    "movl %0, %%eax\n"

    ".loop:\n"
    "cmp %%eax, %%ebx\n"
    "movl (%1), %%ebx\n"
    "nop\n"
    "nop\n"
    "jnz .loop\n"
    :
    :"m"(__ms__), "p"(&__chrono)
    :"%eax", "%ebx"
  );
}
 
