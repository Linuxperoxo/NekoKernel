;
;
;
;    /--------------------------------------------O
;    |                                            |
;    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
;    |  AUTHOR    : Linuxperoxo                   |
;    |  FILE      : isr.s                         |
;    |  SRC MOD   : 13/12/2024                    |
;    |                                            |
;    O--------------------------------------------/
;
;
;

extern isr_handler
extern irq_isr_handler

%macro ISR_NOERRORCODE 1
  global isr%1
  
  isr%1:
    CLI
    PUSH LONG 0
    PUSH LONG %1
    JMP isr_common_stub
%endmacro

%macro ISR_ERRORCODE 1
  global isr%1

  isr%1:
    CLI
    PUSH LONG %1
    JMP isr_common_stub
%endmacro

%macro IRQ_ISR 2
  global irq%1

  irq%1:
    PUSH LONG 0
    PUSH LONG %2
    JMP irq_isr_common_stub
%endmacro

ISR_NOERRORCODE 0
ISR_NOERRORCODE 1
ISR_NOERRORCODE 2
ISR_NOERRORCODE 3
ISR_NOERRORCODE 4
ISR_NOERRORCODE 5
ISR_NOERRORCODE 6
ISR_NOERRORCODE 7
ISR_ERRORCODE 8
ISR_NOERRORCODE 9
ISR_ERRORCODE 10
ISR_ERRORCODE 11
ISR_ERRORCODE 12
ISR_ERRORCODE 13
ISR_ERRORCODE 14
ISR_NOERRORCODE 15
ISR_NOERRORCODE 16
ISR_NOERRORCODE 17
ISR_NOERRORCODE 18
ISR_NOERRORCODE 19
ISR_NOERRORCODE 20
ISR_NOERRORCODE 21
ISR_NOERRORCODE 22
ISR_NOERRORCODE 23
ISR_NOERRORCODE 24
ISR_NOERRORCODE 25
ISR_NOERRORCODE 26
ISR_NOERRORCODE 27
ISR_NOERRORCODE 28
ISR_NOERRORCODE 29
ISR_NOERRORCODE 30
ISR_NOERRORCODE 31
IRQ_ISR 0, 32
IRQ_ISR 1, 33
IRQ_ISR 2, 34
IRQ_ISR 3, 35
IRQ_ISR 4, 36
IRQ_ISR 5, 37
IRQ_ISR 6, 38
IRQ_ISR 7, 39
IRQ_ISR 8, 40
IRQ_ISR 9, 41
IRQ_ISR 10, 42
IRQ_ISR 11, 43
IRQ_ISR 12, 44
IRQ_ISR 13, 45
IRQ_ISR 14, 46
IRQ_ISR 15, 47
ISR_NOERRORCODE 128
ISR_NOERRORCODE 177

isr_common_stub:
  PUSHA
  MOV EAX, DS
  PUSH EAX
  MOV EAX, CR2
  PUSH EAX

  MOV AX, 0x10
  MOV DS, AX
  MOV FS, AX
  MOV GS, AX
  MOV ES, AX

  PUSH ESP

  CALL isr_handler

  ADD ESP, 8
  POP EBX
  MOV DS, BX
  MOV ES, BX
  MOV FS, BX
  MOV GS, BX
  
  POPA

  ADD ESP, 8
  STI
  IRET

irq_isr_common_stub:
  PUSHA
  MOV EAX, DS
  PUSH EAX
  MOV EAX, CR2
  PUSH EAX

  MOV AX, 0x10
  MOV DS, AX
  MOV FS, AX
  MOV GS, AX
  MOV ES, AX

  PUSH ESP

  CALL irq_isr_handler

  ADD ESP, 8
  POP EBX
  MOV DS, BX
  MOV ES, BX
  MOV FS, BX
  MOV GS, BX
  
  POPA

  ADD ESP, 8
  STI
  IRET
