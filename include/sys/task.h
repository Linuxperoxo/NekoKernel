/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : task.h                        |
 *    |  SRC MOD   : 19/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __K_TASK__
#define __K_TASK__

#define TASK_RUNNING 0b0011
#define TASK_BLOCKED 0b0010
#define TASK_SLEEPING 0b0001
#define TASK_DEAD 0b0000

#define FD_FOR_TASK 0x0A

#define FD_TTYOUT 0x00
#define FD_TTYIN  0x01

#include <std/int.h>
#include <neko/kernel.h>
#include <sys/vfs.h>

typedef __u32 reg_t;
typedef __u16 reg_seg_t;
typedef __u8 task_uid_t;
typedef __u8 task_state_t;
typedef __u8 task_id_t;

typedef struct task_t task_t;

typedef struct task_t
{
  /*
   *
   * Registradores da task
   *
   */

  reg_t __eax;
  reg_t __ebx;
  reg_t __ecx;
  reg_t __edx;
  reg_t __edi;
  reg_t __ebp;
  reg_t __esp;
  reg_t __esi;
  reg_t __eip;
  
  reg_seg_t __ss;
  reg_seg_t __cs;
  reg_seg_t __ds;
  reg_seg_t __es;
  reg_seg_t __fs;
  reg_seg_t __gs;
  
  task_t* __prev_task;
  task_t* __next_task;

  /*
   *
   * Id do usuário, status da task e id da task, essas informações
   * vão ser usadas mais no futuro
   *
   */

  task_uid_t   __uid   : 4;
  task_state_t __state : 4;
  task_id_t    __tid;

  fd_t __fd[FD_FOR_TASK];
}task_t;

void task_init();
void task_switch(int_regs_t* __int_regs__);
void task_create(const char* __file__, reg_t __eip__, __u8 __user_id);
void task_kill(__u8 __task_id__);

#endif
