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
  EXTERN k_main

  ALIGN 4

  DD 0x1BADB002
  DD 0x00
  DD - (0x1BADB002 + 0x00)

_start:
  MOV ESP, stack_space

  CLI
  
  CALL k_main

  JMP $

SECTION .bss
RESB 8192
stack_space:
