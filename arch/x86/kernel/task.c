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
#include <std/io.h>
#include <timer.h>

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

#define TASK_BLOCKED  0b0010
#define TASK_SLEEPING 0b0001
#define TASK_DEAD     0b0000

struct
{
  struct Task* __current_task;
  struct Task* __root_task;
}__TaskList;

/*
 *
 * Toda a parte comentada foi usada para testes, caso queira usar 
 * é so descomentar :)
 *
 */

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

void task_init()
{
  static struct Task  __task_1;
  static struct Task  __task_2;

  __TaskList.__current_task = &__task_1;
  __TaskList.__root_task    = &__task_1;

  __TaskList.__current_task->__prev_task  = &__task_2;
  __TaskList.__current_task->__next_task  = &__task_2;
  __TaskList.__current_task->__task_state = TASK_SLEEPING; 
  __TaskList.__current_task->__eip        = (__u32)&task1_code;

  __task_2.__prev_task  = &__task_1;
  __task_2.__next_task  = &__task_1;
  __task_2.__task_state = TASK_SLEEPING;
  __task_2.__eip        = (__u32)&task2_code;
}

void task_save(struct InterruptRegisters* __int_regs__)
{
  
  /*
   *
   * Salvando os registradores gerais que está sendo usado pela task (eax, ebx, ecx, edx, edi, esi)
   *
   */

  __TaskList.__current_task->__eax = __int_regs__->__eax;
  __TaskList.__current_task->__ebx = __int_regs__->__ebx;
  __TaskList.__current_task->__ecx = __int_regs__->__ecx;
  __TaskList.__current_task->__edx = __int_regs__->__edx;
  __TaskList.__current_task->__edi = __int_regs__->__edi;
  __TaskList.__current_task->__esi = __int_regs__->__esi;
  
  /*
   *
   * Salvando os registradores da stack (esp, ebp)
   *
   */

  __TaskList.__current_task->__esp = __int_regs__->__esp;
  __TaskList.__current_task->__ebp = __int_regs__->__ebp;
  
  /*
   *
   * Salvando o eip da task, reponsável por salvar onde o endereço em que
   * o código da task parou
   *
   */

  //__TaskList.__current_task->__eip = __int_regs__->__eip;
  
  /*
   *
   * Salvando os registradores de segmento (cs, ds, ss)
   *
   */

  __TaskList.__current_task->__cs  = __int_regs__->__cs;
  __TaskList.__current_task->__ds  = __int_regs__->__ds;
  __TaskList.__current_task->__ss  = __int_regs__->__ss;
}

void task_load(struct InterruptRegisters* __int_regs__)
{
  
  /*
   *
   * Restaurando registradores gerais da Task (eax, ebx, ecx, edx, edi, esi) 
   *
   */

  __int_regs__->__eax = __TaskList.__current_task->__eax;
  __int_regs__->__ebx = __TaskList.__current_task->__ebx;
  __int_regs__->__ecx = __TaskList.__current_task->__ecx;
  __int_regs__->__edx = __TaskList.__current_task->__edx;
  __int_regs__->__edi = __TaskList.__current_task->__edi;
  __int_regs__->__esi = __TaskList.__current_task->__esi;

  /*
   *
   * Restaurando registradores da stack (esp, ebp)
   *
   */

  __int_regs__->__esp = __TaskList.__current_task->__esp;
  __int_regs__->__ebp = __TaskList.__current_task->__ebp;

  /*
   *
   * Restaurando o eip, reponsável por apontar para o código da task
   *
   */

  __int_regs__->__eip = __TaskList.__current_task->__eip;

  /*
   *
   * Restaurando registradores de segmento (cs, ds, ss)
   *
   */

  __int_regs__->__cs  = __TaskList.__current_task->__cs;
  __int_regs__->__ds  = __TaskList.__current_task->__ds;
  __int_regs__->__ss  = __TaskList.__current_task->__ss;
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

  __TaskList.__current_task->__task_state = TASK_SLEEPING;

  while(__TaskList.__current_task->__next_task != NULL)
  {
    __TaskList.__current_task = __TaskList.__current_task->__next_task;
    
    if(__TaskList.__current_task->__task_state == TASK_SLEEPING)
      break;
  }

  /*
   *
   * Agora que achamos uma task válida, vamos restaurar seu estado
   *
   */

  task_load(__int_regs__);
}
