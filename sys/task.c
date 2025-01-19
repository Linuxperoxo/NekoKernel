/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : task.c                        |
 *    |  SRC MOD   : 17/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <sys/task.h>
#include <neko/kernel.h>
#include <std/int.h>
#include <std/utils.h>
#include <std/io.h>
#include <sys/timer.h>
#include <sys/vfs.h>
#include <sys/kmem.h>

/*
 *
 * TASK_BLOCK: Significa que essa task está bloqueada, ou seja,
 *             que está esperando alguma informação, como leitura de disco
 *
 * TASK_TERMI: Uma task que já foi terminada e não precisa mais de atenção
 *             do kernel
 *
 * TASK_SLEEP: A task precisa de atenção do kernel para executa-lá
 *
 *
 *
 */

static task_t* __current_task;
static task_t* __root_task;

/*
 *
 * Toda a parte comentada foi usada para testes, caso queira usar 
 * é so descomentar :)
 *
 */

/*

void task1_code()
{
  while(1)
  {
    printf("Running Task1... ");
    sleep_for(1000);
  }
}

void task2_code()
{
  while(1)
  {
    printf("Running Task2... ");
    sleep_for(1000);
  }
}

*/

void task_init()
{
  __root_task = (task_t*)kmalloc(sizeof(task_t));

  __root_task->__prev_task = NULL;
  __root_task->__next_task = NULL;
  __root_task->__uid       = 0x00;
  __root_task->__state     = TASK_RUNNING;
  __root_task->__tid       = 0x00;

  __current_task = __root_task;
}

void task_save(struct InterruptRegisters* __int_regs__)
{
  
  /*
   *
   * Salvando os registradores gerais que está sendo usado pela task (eax, ebx, ecx, edx, edi, esi)
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

  //__current_task->__esp = __int_regs__->__esp;
  //__current_task->__ebp = __int_regs__->__ebp;
  
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
}

void task_load(struct InterruptRegisters* __int_regs__)
{
  
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

  //__int_regs__->__esp = __current_task->__esp;
  //__int_regs__->__ebp = __current_task->__ebp;

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

  __int_regs__->__cs = __current_task->__cs;
  __int_regs__->__ds = __current_task->__ds;
  __int_regs__->__ss = __current_task->__ss;
  __int_regs__->__gs = __current_task->__gs;
  __int_regs__->__fs = __current_task->__fs;
}

void task_switch(struct InterruptRegisters* __int_regs__)
{

  /*
   *
   * Essa função faz a troca de contexto, ou seja, cada task tem
   * um limite de execução, e quando esse limite é atingido, a 
   * task é trocada, salvando o estado atual da task que está rodando,
   * e restaurando a estado da próxima task, isso se ela estiver disponível
   *
   */

  /*
   *
   * Aqui fazemos uma verificação para evitar uma falha
   *
   */

  if(__current_task == NULL)
  {
    if(__root_task == NULL)
    {
      return;
    }
    __current_task = __root_task;
  }

  /*
   *
   * Aqui salvamos o estado atual da task
   *
   */

  task_save(__int_regs__);

  /*
   *
   * Aqui fazemos um loop para trocar de contexto e verificar se é uma task
   * marcada como TASK_SLEEP, que fala que a task está dormindo e esperando
   * o sistema voltar a executa-lá
   *
   */

  __current_task->__state = TASK_SLEEPING;

  while(__current_task->__next_task != NULL)
  {
    __current_task = __current_task->__next_task;
    
    if(__current_task->__state == TASK_SLEEPING)
      break;
  }

  __current_task->__state = TASK_RUNNING;

  /*
   *
   * Agora que achamos uma task válida, vamos restaurar seu estado
   *
   */

  task_load(__int_regs__);
}

vfs_t* task_fd(__u8 __fd__)
{
  return (__current_task->__fd[__fd__].__is_open) ? __current_task->__fd[__fd__].__vfs_file : NULL;
}
