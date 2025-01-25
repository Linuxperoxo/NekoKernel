/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : timer.c                       |
 *    |  SRC MOD   : 25/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <neko/idt.h>
#include <neko/kernel.h>
#include <std/int.h>
#include <std/io.h>
#include <sys/ports.h>
#include <sys/task.h>
#include <sys/timer.h>

#define FREQUENCY                                                              \
  1000 // Queremos interrupções a cada 1ms (Frequência de 1000 Hz)
#define PIT_INT_NUM 0x20
#define DIVISOR_NUM 1193180

#define TIMER_CHANNEL_0_PORT 0x40
#define TIMER_CHANNEL_1_PORT 0x41
#define TIMER_CHANNEL_2_PORT 0x42
#define TIMER_COMMAND_PORT 0x43

/*
 *
 * O PIT trabalha a uma frequência de 119318.16666 Hz
 *
 */

static __u32 __divisor = DIVISOR_NUM / FREQUENCY; // Gera interrupção a cada 1ms
static __u32 __sys_clock = 0x00;
static __u32 __chrono = 0x00;

/*
 *
 * Internal Functions
 *
 */

static void timer_handler(int_regs_t *__regs__) {

  /*
   *
   * Incremento a cada interrupção do clock
   *
   */

  __sys_clock += 1;
  __chrono += 1;

  /*
   *
   * Faço um pequeno delay aqui pra troca de
   * contexto, isso evita o problema de ficar
   * preso em apenas uma task
   *
   */

  if (__sys_clock % 1000 == 0)
    task_switch(__regs__);
}

/*
 *
 * timer.h Functions
 *
 */

void timer_init() {
  idt_install_coop_routine(PIT_INT_NUM, &timer_handler);

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
   * Enviandos o bytes para o canal 0, para configurar
   * a frequência da interrupção
   *
   */

  /*
   *
   * O divisor serve para falar ao PIT quantos Hz ele deve
   * gerar interrupção. Exemplo:
   *
   * 1193180 / 1000 = 1193 Hz, ou seja ele vai trabalha a uma frequência de
   * 1193 Hz, 1193 interrupções por segundo
   *
   */

  outb(TIMER_CHANNEL_0_PORT,
       (__u8)(__divisor & 0xFF)); // Bytes menos significativos
  outb(TIMER_CHANNEL_0_PORT,
       (__u8)((__divisor >> 8) & 0xFF)); // Bytes mais significativos
}

void sleep_for(__u32 __ms__) {
  __asm__ volatile("xorl %%ebx, %%ebx\n"
                   "movl %%ebx, (%1)\n"

                   "movl %0, %%eax\n"

                   ".loop:\n"
                   "cmp %%eax, %%ebx\n"
                   "movl (%1), %%ebx\n"
                   "jnz .loop\n"
                   :
                   : "m"(__ms__), "p"(&__chrono)
                   : "%eax", "%ebx");
}
