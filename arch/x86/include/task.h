/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : task.h                        |
 *    |  SRC MOD   : 12/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __K_TASK__
#define __K_TASK__

#include <std/int.h>
#include <sys/kernel.h>

struct Task
{
  struct Task* __prev_task;
  struct Task* __next_task;

  struct
  {
    __u32 __eax;
    __u32 __ebx;
    __u32 __ecx;
    __u32 __edx;
    __u32 __edi;
    __u32 __ebp;
    __u32 __esp;
    __u32 __esi;
    __u32 __eip;
    __u16 __ss;
    __u16 __cs;
    __u16 __ds;
    __u16 __es;
    __u16 __gs;
  };

  struct
  {
    __u8 __user_id    : 4;
    __u8 __task_state : 4;
  };
};

extern void task_init();
extern void task_switch();

#endif
