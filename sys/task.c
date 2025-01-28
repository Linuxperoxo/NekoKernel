/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : task.c                        |
 *    |  SRC MOD   : 27/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <neko/kernel.h>
#include <std/int.h>
#include <std/io.h>
#include <std/utils.h>
#include <sys/kmem.h>
#include <sys/task.h>
#include <sys/timer.h>
#include <sys/vfs.h>

static task_t *__root_task = NULL;
static task_t *__current_task = NULL;
static task_pid_t __current_id = 0x00;
static __u32 __switch = 0x00;

/*
 *
 * Internal Functions
 *
 */

static void task_save(int_regs_t *__int_regs__) {

  /*
   *
   * Salvando os registradores gerais que está sendo usado pela task (eax, ebx,
   * ecx, edx, edi, esi)
   *
   */

  __current_task->__eax = __int_regs__->__eax;
  __current_task->__ebx = __int_regs__->__ebx;
  __current_task->__ecx = __int_regs__->__ecx;
  __current_task->__edx = __int_regs__->__edx;
  __current_task->__edi = __int_regs__->__edi;
  __current_task->__esi = __int_regs__->__esi;

  /*
   *
   * Salvando os registradores da stack (esp, ebp)
   *
   */

  __current_task->__esp = __int_regs__->__esp;
  __current_task->__ebp = __int_regs__->__ebp;

  /*
   *
   * Salvando o eip da task, reponsável por salvar onde o endereço em que
   * o código da task parou
   *
   */

  __current_task->__eip = __int_regs__->__eip;

  /*
   *
   * Salvando os registradores de segmento (cs, ds, ss)
   *
   */

  __current_task->__cs = __int_regs__->__cs;
  __current_task->__ds = __int_regs__->__ds;
  __current_task->__ss = __int_regs__->__ss;
  __current_task->__gs = __int_regs__->__gs;
  __current_task->__fs = __int_regs__->__fs;

  return;
}

static void task_load(int_regs_t *__int_regs__) {

  /*
   *
   * Restaurando registradores gerais da Task (eax, ebx, ecx, edx, edi, esi)
   *
   */

  __int_regs__->__eax = __current_task->__eax;
  __int_regs__->__ebx = __current_task->__ebx;
  __int_regs__->__ecx = __current_task->__ecx;
  __int_regs__->__edx = __current_task->__edx;
  __int_regs__->__edi = __current_task->__edi;
  __int_regs__->__esi = __current_task->__esi;

  /*
   *
   * Restaurando registradores da stack (esp, ebp)
   *
   */

  /*
   *
   * Usamos esse "- 12" pq vamos usar 12 bytes para copiar os registradores
   * que foram empilhados automaticamente para a nova stack, isso serve para
   * que conseguimos usar o iret na nova stack
   *
   */

  __int_regs__->__esp = __current_task->__esp - 12;
  __int_regs__->__ebp = __current_task->__ebp;

  /*
   *
   * Restaurando o eip, reponsável por apontar para o código da task
   *
   */

  __int_regs__->__eip = __current_task->__eip;

  /*
   *
   * Restaurando registradores de segmento (cs, ds, ss)
   *
   */

  /*
  __int_regs__->__cs = __current_task->__cs;
  __int_regs__->__ds = __current_task->__ds;
  __int_regs__->__ss = __current_task->__ss;
  __int_regs__->__gs = __current_task->__gs;
  __int_regs__->__fs = __current_task->__fs;
  */

  return;
}

/*
 *
 * task.h Functions
 *
 */

void task_switch(int_regs_t *__int_regs__) {
  if (__current_task == NULL) {
    if (__root_task == NULL)
      return;
    __current_task = __root_task;
    task_load(__int_regs__);
    return;
  }

  if (__switch++ > 0)
    task_save(__int_regs__);

  task_t *__task = __current_task->__next;

  while (__task != NULL) {
    if (__task->__state == TASK_SLEEPING) {
      __current_task = __task;
      task_load(__int_regs__);
      return;
    }
    __task = __task->__next;
  }

  if (__task == NULL)
    __current_task = __root_task;
  task_load(__int_regs__);
  return;
}

__i8 task_parent(const char *__file__, __u32 __stack__) {
  __asm__ volatile("cli\n" : : :);

  task_t *__prev = __current_task;
  task_t *__next = __current_task;

  while (__next != NULL) {
    __prev = __next;
    __next = __next->__next;
  }

  __next = (task_t *)kmalloc(sizeof(task_t));
  __next->__file = vfs_exist(__file__);

  if (__next->__file == NULL) {
    kfree(__next);
    return -1;
  }

  __next->__eip = (reg_t)__next->__file->__exec;
  __next->__child = NULL;
  __next->__prev = __prev;
  __next->__next = NULL;
  __next->__state = TASK_SLEEPING;
  __next->__pid = __current_id++;
  __next->__childs = 0x00;
  __next->__esp = __stack__;
  __prev->__next = __next;

  if (__root_task == NULL) {
    __root_task = __next;
    __root_task->__next = NULL;
    __current_task = __root_task;
  }

  __asm__ volatile("movw %%fs, %0\n sti\n" : "=r"(__next->__uid) : :);

  return __next->__pid;
}

__i8 task_fork() { return 0; }

void task_wait(task_pid_t __pid__) {}

void task_kill(task_pid_t __pid__) {}
