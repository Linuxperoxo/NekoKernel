;
;
;
;    /--------------------------------------------O
;    |                                            |
;    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
;    |  AUTHOR    : Linuxperoxo                   |
;    |  FILE      : isr.s                         |
;    |  SRC MOD   : 12/01/2025                    |
;    |                                            |
;    O--------------------------------------------/
;
;
;

[BITS 32]

%define KERNEL_CODE_SEG  0x08
%define KERNEL_DATA_SEG  0x10
%define KERNEL_STACK_SEG 0x10

extern isr_handler
extern irq_handler

%macro ISR_EXCEPTION 2
  global isr%1

  isr%1:
    CLI          ; No Interruptions :)
    
    PUSH LONG %2 ; Send Error Code to InterruptRegisters struct 
    PUSH LONG %1 ; Interrupt Number to InterruptRegisters struct
    
    JMP isr_calling
%endmacro

%macro ISR_IRQ 1
  global irq%1:

  irq%1:
    CLI            ; No Interruptions :)
    
    PUSH LONG 0x00 ; Send Error Code to InterruptRegisters struct 
    PUSH LONG %1   ; Interrupt Number to InterruptRegisters struct
    
    JMP irq_calling
%endmacro

;
; Ambas as routines abaixo servem para montar a
; struct do tipo InterruptRegisters na stack, e
; depois passar o ponteiro dessa struct montada na
; stack para a função de handler da interrupção, se for
; uma exception do processador vamos chamar a isr_handler, se for
; uma request vamos chamar irq_handler, ambas recebem esse ponteiro
;

isr_calling:

  ;
  ; Montando a struct na stack
  ;

  PUSH EDI
  PUSH ESI
  PUSH EDX
  PUSH ECX
  PUSH EBX
  PUSH EAX

  MOV AX, GS
  PUSH AX

  MOV AX, ES
  PUSH AX

  MOV AX, FS
  PUSH AX

  MOV AX, SS
  PUSH AX

  MOV AX, DS
  PUSH AX

  MOV AX, KERNEL_CODE_SEG
  MOV CS, AX

  MOV AX, KERNEL_DATA_SEG
  MOV DS, AX
  MOV SS, AX

  PUSH ESP ; Parâmetro para a função irq_handler(struct InterruptRegisters*)

  CALL isr_handler

  POP ESP

  ;
  ; Agora vamos restaurar todos os registradores
  ;
  
  POP AX
  MOV DS, AX

  POP AX
  MOV SS, AX
   
  POP AX
  MOV FS, AX

  POP AX
  MOV ES, AX

  POP AX
  MOV GS, AX

  POP EAX
  POP EBX
  POP ECX
  POP EDX
  POP ESI
  POP EDI
  
  ADD ESP, 8

  STI  ; Habilitando as interrupções novamente
  IRET ; Aqui o processador vai desempilhar o que ele empilhou automaticamente (EIP, CS, EFLAGS)

irq_calling:
  
  ;
  ; Montando a struct na stack
  ;
  
  PUSH EDI
  PUSH ESI
  PUSH EDX
  PUSH ECX
  PUSH EBX
  PUSH EAX

  MOV AX, GS
  PUSH AX

  MOV AX, ES
  PUSH AX

  MOV AX, FS
  PUSH AX

  MOV AX, SS
  PUSH AX

  MOV AX, DS
  PUSH AX

  PUSH ESP ; Parâmetro para a função irq_handler(struct InterruptRegisters*)

  CALL irq_handler

  POP ESP

  ;
  ; Agora vamos restaurar todos os registradores
  ;

  POP AX
  MOV DS, AX

  POP AX
  MOV SS, AX
    
  POP AX
  MOV FS, AX

  POP AX
  MOV ES, AX

  POP AX
  MOV GS, AX

  POP EAX
  POP EBX
  POP ECX
  POP EDX
  POP ESI
  POP EDI

  ADD ESP, 8

  STI  ; Habilitando as interrupções novamente
  IRET ; Aqui o processador vai desempilhar o que ele empilhou automaticamente (EIP, CS, EFLAGS)

ISR_EXCEPTION 0, 1 
ISR_EXCEPTION 1, 2 
ISR_EXCEPTION 2, 3 
ISR_EXCEPTION 3, 4 
ISR_EXCEPTION 4, 5 
ISR_EXCEPTION 5, 6 
ISR_EXCEPTION 6, 7 
ISR_EXCEPTION 7, 8 
ISR_EXCEPTION 8, 9 
ISR_EXCEPTION 9, 10 
ISR_EXCEPTION 10, 11 
ISR_EXCEPTION 11, 12 
ISR_EXCEPTION 12, 13 
ISR_EXCEPTION 13, 14 
ISR_EXCEPTION 14, 15 
ISR_EXCEPTION 15, 16 
ISR_EXCEPTION 16, 17 
ISR_EXCEPTION 17, 18 
ISR_EXCEPTION 18, 19 
ISR_EXCEPTION 19, 20 
ISR_EXCEPTION 20, 21 
ISR_EXCEPTION 21, 22 
ISR_EXCEPTION 22, 23 
ISR_EXCEPTION 23, 24 
ISR_EXCEPTION 24, 25 
ISR_EXCEPTION 25, 26 
ISR_EXCEPTION 26, 27 
ISR_EXCEPTION 27, 28 
ISR_EXCEPTION 28, 29 
ISR_EXCEPTION 29, 30 
ISR_EXCEPTION 30, 31 
ISR_EXCEPTION 31, 32

ISR_IRQ 0
ISR_IRQ 1
ISR_IRQ 2
ISR_IRQ 3
ISR_IRQ 4
ISR_IRQ 5
ISR_IRQ 6
ISR_IRQ 7
ISR_IRQ 8
ISR_IRQ 9
ISR_IRQ 10
ISR_IRQ 11
ISR_IRQ 12
ISR_IRQ 13
ISR_IRQ 14
ISR_IRQ 15

;
; Syscalls
;

ISR_IRQ 128
ISR_IRQ 177
