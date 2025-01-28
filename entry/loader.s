/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : loader.s                      |
 *    |  SRC MOD   : 26/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

.equ STACK_ADDRS, 0xFFFF
.extern k_main

.section .text
.global k_loader

  /*
   *
   * Como a função k_main é feita em C e está
   * em outro arquivo, falamos para o linker 
   * que ela foi declarada fora desse arquivo
   *
   */

  k_loader:
    movl $STACK_ADDRS, %esp # Configurando esp (stack)
    movl %esp, %ebp         # Configurando ebp
    cli                     # Ignorando interrupções externas
    call k_main             # Chamando função principal do kernel 
  
    1:
      jmp 1b               # Loop infinito

