/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : task.c                        |
 *    |  SRC MOD   : 10/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <task.h>
#include <sys/kernel.h>
#include <std/int.h>
#include <std/utils.h>

#define TASK_BLOCK 0b0011
#define TASK_TERMI 0b0010
#define TASK_SLEEP 0b0001
#define TASK_FREE  0b0000

struct
{
  struct Task* __current_task;
  struct Task* __root_task;
}__TaskList;

void task_init()
{
  static struct Task  __task_root;

  __TaskList.__current_task = &__task_root;
  __TaskList.__root_task    = &__task_root;

  __TaskList.__current_task->__prev_task  = NULL;
  __TaskList.__current_task->__next_task  = NULL;
  __TaskList.__current_task->__user_id    = 0x00;
  __TaskList.__current_task->__task_state = TASK_SLEEP; 
}

void task_save(struct InterruptRegisters* __int_regs__)
{
  __TaskList.__current_task->__eax = __int_regs__->__eax;
  __TaskList.__current_task->__ebx = __int_regs__->__ebx;
  __TaskList.__current_task->__ecx = __int_regs__->__ecx;
  __TaskList.__current_task->__edx = __int_regs__->__edx;
  __TaskList.__current_task->__edi = __int_regs__->__edi;
  __TaskList.__current_task->__esi = __int_regs__->__esi;
  __TaskList.__current_task->__eip = __int_regs__->__eip;
}

void task_load(struct InterruptRegisters* __int_regs__)
{
  __int_regs__->__eax = __TaskList.__current_task->__eax;
  __int_regs__->__ebx = __TaskList.__current_task->__ebx;
  __int_regs__->__ecx = __TaskList.__current_task->__ecx;
  __int_regs__->__edx = __TaskList.__current_task->__edx;
  __int_regs__->__edi = __TaskList.__current_task->__edi;
  __int_regs__->__esi = __TaskList.__current_task->__esi;
  __int_regs__->__eip = __TaskList.__current_task->__eip;
}

void task_switch(struct InterruptRegisters* __int_regs__)
{
  task_save(__int_regs__);

  while(__TaskList.__current_task->__prev_task != NULL)
  {
    if(__TaskList.__current_task->__prev_task->__task_state == TASK_SLEEP)
    {
      __TaskList.__current_task = __TaskList.__current_task->__prev_task;
      break;
    }
  }
  task_load(__int_regs__);
}
