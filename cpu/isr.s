/* 
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : isr.s                         |
 *    |  SRC MOD   : 28/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

.extern isr_handler
.extern irq_handler
.extern syscall_handler

.equ KERNEL_CODE_SEG, 0x08
.equ KERNEL_DATA_SEG, 0x10
.equ KERNEL_STACK_SEG, 0x10

.macro isr_exception interrupt, error
  .global isr\interrupt

  isr\interrupt:
    cli
    
    pushl $0x00
    pushl $\error
    pushl $\interrupt
    
    jmp isr_calling
.endm

.macro isr_irq interrupt
  .global irq\interrupt

  irq\interrupt:
    cli
    
    pushl %esp
    pushl $0x00
    pushl $\interrupt

    jmp irq_calling
.endm

.macro isr_syscall interrupt
  .global isr_syscall\interrupt

  isr_syscall\interrupt:
    cli

    pushl %esp
    pushl $0x00
    pushl $\interrupt

    jmp syscall_calling
.endm

.align 4
.section .bss
  __new_stack:
    .fill 4

.align 4
.section .text
  isr_calling:

    /*
     *
     * Aqui vamos empilhar os registradores para montar
     * a struct regs_int_t, olhe o arquivo include/neko/kernel.h 
     * 
     * Precisamos empilhar a struct dos membros mais baixos até os mais altos,
     * se você observou, os membros __eip, __cs e __eflags não estão sendo empilhados,
     * isso acontece pois o processador empilha esses 3 registradores automaticamente
     * quando ocorre uma interrupção
     * 
     */

    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    movw %gs, %ax
    pushl %eax
    movw %es, %ax
    pushl %eax
    movw %fs, %ax
    pushl %eax
    movw %ss, %ax
    pushl %eax
    movw %ds, %ax
    pushl %eax

    /*
     *
     * Agora que montamos a struct corretamente seguindo a ordem, vamos mudar para
     * os segmentos do kernel para poder manipular a interrupção no kernel mode 
     * 
     */ 

    movw $KERNEL_CODE_SEG, %ax
    movw %ax, %cs

    movw $KERNEL_DATA_SEG, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movw %ax, %gs
    movw %ax, %fs

    /*
     *
     * Se você percebeu, o registrador esp empilhado agora pode parecer meio confuso,
     * já que o esp é o 4 membro de baixo pra cima da struct, mas quando empilhamos esse
     * registrador não estamos passando para a struct, até pq ela já está montada, estamos 
     * na verdade passando o parâmetro para a função, que no caso é um int_regs_t*, como 
     * montamos a struct na stack, basta passar o esp como parâmetro 
     * 
     */

    pushl %esp 

    call isr_handler

    popl %esp

    /*
     *
     * Restaurando todos os registradores, dessa vez dos membros mais altos aos mais baixos
     * 
     */

    popl %eax
    movw %ax, %ds
    popl %eax
    movw %ax, %ss
    popl %eax
    movw %ax, %fs
    popl %eax
    movw %ax, %es
    popl %eax
    movw %ax, %gs

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp

    addl $0x0C, %esp

    sti  # Habilitando as instruções externas, de I/O que são enviados pelo PIC para o processador
    iret # Retornando da interrupção, isso serve para desempilhar os registradores empilhados pelo processador quando ocorre uma interrupção (EIP, CS, EFLAGS)
  irq_calling:

    /*
     *
     * Aqui vamos empilhar os registradores para montar
     * a struct regs_int_t, olhe o arquivo include/neko/kernel.h 
     * 
     * Precisamos empilhar a struct dos membros mais baixos até os mais altos,
     * se você observou, os membros __eip, __cs e __eflags não estão sendo empilhados,
     * isso acontece pois o processador empilha esses 3 registradores automaticamente
     * quando ocorre uma interrupção
     *
     */

    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    movw %gs, %ax
    pushl %eax
    movw %es, %ax
    pushl %eax
    movw %fs, %ax
    pushl %eax
    movw %ss, %ax
    pushl %eax
    movw %ds, %ax
    pushl %eax

    /*
     *
     * Agora que montamos a struct corretamente seguindo a ordem, vamos mudar para
     * os segmentos do kernel para poder manipular a interrupção no kernel mode 
     *
     */

    movw $KERNEL_CODE_SEG, %ax
    movw %ax, %cs

    movw $KERNEL_DATA_SEG, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movw %ax, %gs
    movw %ax, %fs

    /*
     *
     * se você percebeu, o registrador esp empilhado agora pode parecer meio confuso,
     * já que o esp é o 4 membro de baixo pra cima da struct, mas quando empilhamos esse
     * registrador não estamos passando para a struct, até pq ela já está montada, estamos 
     * na verdade passando o parâmetro para a função, que no caso é um int_regs_t*, como 
     * montamos a struct na stack, basta passar o esp como parâmetro 
     *
     */

    pushl %esp

    call irq_handler

    popl %esp

    /*
     *
     * Restaurando todos os registradores, dessa vez dos membros mais altos aos mais baixos
     *
     */

    popl %eax
    movw %ax, %ds
    popl %eax
    movw %ax, %ss
    popl %eax
    movw %ax, %fs
    popl %eax
    movw %ax, %es
    popl %eax
    movw %ax, %gs

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    /*
     *
     * Aqui acontece uma coisa exclusiva para o irq
     *
     * O irq 20 é o irq que vem do PIT. O PIT e um clock, que gera
     * interrupções a cada uma certa quantidade de ciclos, para entender
     * melhor veja o arquivo sys/timer.c.
     *
     * Nesse timer, vamos ter a troca de contexto do kernel, que por uma série
     * de motivos configuramos um timer para ficar trocando de contexto, cada 
     * contexto é um processo, e cada processo vai ter seus registradores que estavam
     * sendo usados durante ele, e quando uma interrupção do PIT ocorre, salvamos o estado
     * do contexto atual, e carregamos outro contexto que está esperando a atenção do kernel,
     * veja o arquivo include/sys/task.h e sys/task.c. Então como temos que trocar a task, temos
     * que carregar o estado da nova terefa nos registradores, e essa parte serve para trocar o 
     * registrador esp (stack) do contexto, não é tão simples assim pq precisamos da stack para 
     * restaurar o estado de alguns registradores antes da interrupção, então precisamos copiar
     * os dados de __eip, __cs e __eflags para a nova stack, e ai sim dar o iret para o processador
     * restaurar os registradores para a nova task e modificar o esp para o esp da nova task 
     *
     */

    movl 20(%esp), %edi    # Pegando o ponteiro para a nova stack
    movl %edi, __new_stack # Salvando a nova stack

    movl 24(%esp), %esi # Pegando o __eip da stack antiga
    movl %esi, (%edi)   # Copiando __eip para a stack nova
    addl $0x04, %edi    # Avançando para __cs

    movl 28(%esp), %esi # Pegando o __cs da stack antiga
    movl %esi, (%edi)   # Copiando __cs para a stack nova
    addl $0x04, %edi    # Avançando para __eflags

    movl 32(%esp), %esi # Pegando o __eflags da stack antiga
    movl %esi, (%edi)   # Copiando __eflgs para a stack nova

    popl %esi # Restaurando esi que está na stack antiga antes de configurar a nova stack
    popl %edi # Restaurando edi que está na stack antiga antes de configurar a nova stack
    popl %ebp # Restaurando ebp que está na stack antiga antes de configurar a nova stack, responsável pelas variáveis locais e parâmetros

    movl __new_stack, %esp # Agora estamos usando a nova stack

    sti  # Habilitando as instruções externas, de I/O que são enviados pelo PIC para o processador
    iret # Retornando da interrupção, isso serve para desempilhar os registradores empilhados pelo processador quando ocorre uma interrupção (EIP, CS, EFLAGS)
  syscall_calling:

    /*
     *
     * Aqui vamos empilhar os registradores para montar
     * a struct regs_int_t, olhe o arquivo include/neko/kernel.h 
     * 
     * Precisamos empilhar a struct dos membros mais baixos até os mais altos,
     * se você observou, os membros __eip, __cs e __eflags não estão sendo empilhados,
     * isso acontece pois o processador empilha esses 3 registradores automaticamente
     * quando ocorre uma interrupção
     *
     */

    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    movw %gs, %ax
    pushl %eax
    movw %es, %ax
    pushl %eax
    movw %fs, %ax
    pushl %eax
    movw %ss, %ax
    pushl %eax
    movw %ds, %ax
    pushl %eax

    /*
     *
     * Agora que montamos a struct corretamente seguindo a ordem, vamos mudar para
     * os segmentos do kernel para poder manipular a interrupção no kernel mode 
     *
     */

    movw $KERNEL_CODE_SEG, %ax
    movw %ax, %cs

    movw $KERNEL_DATA_SEG, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movw %ax, %gs
    movw %ax, %fs

    /*
     *
     * se você percebeu, o registrador esp empilhado agora pode parecer meio confuso,
     * já que o esp é o 4 membro de baixo pra cima da struct, mas quando empilhamos esse
     * registrador não estamos passando para a struct, até pq ela já está montada, estamos 
     * na verdade passando o parâmetro para a função, que no caso é um int_regs_t*, como 
     * montamos a struct na stack, basta passar o esp como parâmetro 
     *
     */

    pushl %esp

    call syscall_handler

    popl %esp

    /*
     *
     * Restaurando todos os registradores, dessa vez dos membros mais altos aos mais baixos
     *
     */

    popl %eax
    movw %ax, %ds
    popl %eax
    movw %ax, %ss
    popl %eax
    movw %ax, %fs
    popl %eax
    movw %ax, %es
    popl %eax
    movw %ax, %gs

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp

    addl $0x0C, %esp

    sti  # Habilitando as instruções externas, de I/O que são enviados pelo PIC para o processador
    iret # Retornando da interrupção, isso serve para desempilhar os registradores empilhados pelo processador quando ocorre uma interrupção (EIP, CS, EFLAGS)

  /*
   *
   * Aqui fica as interrupções do CPU, ou seja, 
   * interrupções geradas por software 
   *
   */

  isr_exception 0, 1 
  isr_exception 1, 2 
  isr_exception 2, 3 
  isr_exception 3, 4 
  isr_exception 4, 5 
  isr_exception 5, 6 
  isr_exception 6, 7 
  isr_exception 7, 8 
  isr_exception 8, 9 
  isr_exception 9, 10 
  isr_exception 10, 11 
  isr_exception 11, 12 
  isr_exception 12, 13 
  isr_exception 13, 14 
  isr_exception 14, 15 
  isr_exception 15, 16 
  isr_exception 16, 17 
  isr_exception 17, 18 
  isr_exception 18, 19 
  isr_exception 19, 20 
  isr_exception 20, 21 
  isr_exception 21, 22 
  isr_exception 22, 23 
  isr_exception 23, 24 
  isr_exception 24, 25 
  isr_exception 25, 26 
  isr_exception 26, 27 
  isr_exception 27, 28 
  isr_exception 28, 29 
  isr_exception 29, 30 
  isr_exception 30, 31 
  isr_exception 31, 32

  /*
   *
   * IRQ são as interrupções que vem do PIC.
   * 
   * Do 32-39 são interrupções do PIC primário
   * Do 40-47 são interrupções do PIC secundário 
   * 
   */

  isr_irq 32
  isr_irq 33
  isr_irq 34
  isr_irq 35
  isr_irq 36
  isr_irq 37
  isr_irq 38
  isr_irq 39
  isr_irq 40
  isr_irq 41
  isr_irq 42
  isr_irq 43
  isr_irq 44
  isr_irq 45
  isr_irq 46
  isr_irq 47

  /*
   *
   * Syscall do kernel
   * 
   */

  isr_syscall 128
  isr_syscall 177
