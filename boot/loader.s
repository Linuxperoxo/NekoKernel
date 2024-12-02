;
;
;
;    /--------------------------------------------O
;    |                                            |
;    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
;    |  AUTHOR    : Linuxperoxo                   |
;    |  FILE      : loader.s                      |
;    |  SRC MOD   : 02/12/2024                    |
;    |                                            |
;    O--------------------------------------------/
;    
;
;

SECTION .text
  GLOBAL _start
  
  ;
  ; Como a função k_main é feita em C e está
  ; em outro arquivo, falamos para o linker 
  ; que ela foi declarada fora desse arquivo
  ;

  EXTERN k_main

  ;
  ; Aqui fica a assinatura de Multiboot
  ; são valores constantes, para entender
  ; melhor você pode dar uma pesquisada 
  ; sobre
  ;

  ALIGN 4

  DD 0x1BADB002
  DD 0x00
  DD - (0x1BADB002 + 0x00)

_start:
  
  ;
  ; Configurando a stack
  ;

  MOV ESP, stack_space

  CLI         ; Ignorando interrupções externas
  
  CALL k_main ; Chamando função principal do kernel 

  JMP $       ; Loop infinito

SECTION .bss

;
; Stack de 8192 Kib
;

RESB 8192
stack_space:
