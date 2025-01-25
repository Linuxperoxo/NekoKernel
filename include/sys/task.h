/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : task.h                        |
 *    |  SRC MOD   : 24/01/2025                    |
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
#define TASK_KERNEL 0b0000

#define FD_FOR_TASK 0x0A

#define TASK_PARENT 0x01
#define TASK_CHILD 0x02

#define FD_TTYOUT 0x00
#define FD_TTYIN 0x01

#define TASK_KERNEL_ID 0xA

#define TASK_IS_PARENT(__task) (__task->__type & TASK_PARENT)
#define TASK_IS_CHILD(__task) ((__task->__type & TASK_CHILD) >> 1)

#include <neko/kernel.h>
#include <std/int.h>
#include <sys/vfs.h>

typedef __u32 reg_t;
typedef __u16 reg_seg_t;
typedef __u16 task_uid_t;
typedef __u16 task_pid_t;
typedef __u8 task_state_t;
typedef __u8 task_type_t;
typedef struct task_t task_t;

typedef struct task_t {

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

  vfs_t *__file;

  task_t *__parent;
  task_t *__child;
  task_t *__prev;
  task_t *__next;

  task_type_t __type;
  task_uid_t __uid;
  task_state_t __state;
  task_pid_t __pid;
  task_pid_t __childs;

  fd_t *__fd[FD_FOR_TASK];
} task_t;

void task_switch(int_regs_t *__int_regs__);
__i8 task_parent(const char *__file__);
__i8 task_fork();
void task_wait(task_pid_t __pid__);
void task_kill(task_pid_t __pid__);

#endif
