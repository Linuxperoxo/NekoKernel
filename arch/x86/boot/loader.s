;
;
;
;    /--------------------------------------------O
;    |                                            |
;    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
;    |  AUTHOR    : Linuxperoxo                   |
;    |  FILE      : loader.s                      |
;    |  SRC MOD   : 02/01/2025                    |
;    |                                            |
;    O--------------------------------------------/
;    
;
;

%define STACK_ADDRS 0xFFFFFF

SECTION .text
  GLOBAL k_loader
  
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

  ;
  ; Essa parte será temporária, o NestKernel vai ter
  ; um bootloader próprio que eu já estou fazendo
  ;

  ;ALIGN 4

  ;DD 0x1BADB002
  ;DD 0x00
  ;DD - (0x1BADB002 + 0x00)

k_loader:
  MOV AL, 0xFF
  MOV [0xC0000000], AL
  MOV ESP, STACK_ADDRS ; Configurando a stack
  MOV EBP, ESP
  CLI                  ; Ignorando interrupções externas
  CALL k_main          ; Chamando função principal do kernel 
  JMP $                ; Loop infinito

