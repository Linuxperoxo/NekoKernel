;
;
;
;    /--------------------------------------------O
;    |                                            |
;    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
;    |  AUTHOR    : Linuxperoxo                   |
;    |  FILE      : gdt.s                         |
;    |  SRC MOD   : 08/12/2024                    |
;    |                                            |
;    O--------------------------------------------/
;
;
;

;
; OBSOLETE FILE!!!
;

;
; Esse arquivo não esta mais sendo usado no kernel, mas
; decidi deixar aqui para ser um manual para outras pessoas,
; algumas pessoas não estão acostumadas com a sintax do AS, então
; a versão NASM ficará aqui
;

BITS 32

global gdtflush

gdtflush:
  
  ;
  ; Carregando GDT 
  ;
  
  MOV EAX, [ESP + 4] ; ESP + 4 é onde está o primeiro parâmetro da função
  LGDT [EAX] 

  ;
  ; Modificando os registradores de segmento
  ; para o segmento data do kernel
  ;

  MOV AX, 0x10
  MOV DS, AX
  MOV ES, AX
  MOV FS, AX
  MOV GS, AX
  MOV SS, AX

  JMP 0x08:.flush ; Usamos Far Jmp para modificar o registrador CS

.flush:
  RET
