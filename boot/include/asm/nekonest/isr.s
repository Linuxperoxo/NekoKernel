/*
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : isr.s                         |
 *    |                                            |
 *    O--------------------------------------------/
 *
 */

.ifndef ISR
  .equ ISR, 0

.include "asm/lib/stdio.s"

.macro MAKE_ISR_SOFTWARE __num__
  .global isr_software\__num__
  .type isr_software\__num__, @function 
  isr_software\__num__:
    cli

    pushl $.panic_str
    call printf

    pushl $.str_isr\__num__
    call printf
    
    hlt
.endm

.macro MAKE_MSG_ISR __num__, __str__
  .type .str_isr\__num__, @object
  .str_isr\__num__:
    .asciz "\__str__"
.endm

.section .isr.text, "ax", @progbits
.code32
.align 4
  MAKE_ISR_SOFTWARE 0
  MAKE_ISR_SOFTWARE 1
  MAKE_ISR_SOFTWARE 2
  MAKE_ISR_SOFTWARE 3
  MAKE_ISR_SOFTWARE 4
  MAKE_ISR_SOFTWARE 5
  MAKE_ISR_SOFTWARE 6
  MAKE_ISR_SOFTWARE 7
  MAKE_ISR_SOFTWARE 8
  MAKE_ISR_SOFTWARE 9
  MAKE_ISR_SOFTWARE 10
  MAKE_ISR_SOFTWARE 11
  MAKE_ISR_SOFTWARE 12
  MAKE_ISR_SOFTWARE 13
  MAKE_ISR_SOFTWARE 14
  MAKE_ISR_SOFTWARE 15
  MAKE_ISR_SOFTWARE 16
  
.section .isr.string, "aS", @progbits
MAKE_MSG_ISR 0, "Division by zero!"
MAKE_MSG_ISR 1, "Debug!"
MAKE_MSG_ISR 2, "Non Maskable Interrupt!"
MAKE_MSG_ISR 3, "Breakpoint!"
MAKE_MSG_ISR 4, "Into Detected Overflow!"
MAKE_MSG_ISR 5, "Out of Bounds!"
MAKE_MSG_ISR 6, "Invalid Opcode!"
MAKE_MSG_ISR 7, "No Coprocessor!"
MAKE_MSG_ISR 8, "Double fault!"
MAKE_MSG_ISR 9, "Coprocessor Segment Overrun!"
MAKE_MSG_ISR 10, "Bad TSS!"
MAKE_MSG_ISR 11, "Stack fault!"
MAKE_MSG_ISR 12, "Page Fault!"
MAKE_MSG_ISR 13, "General Protection Fault!"
MAKE_MSG_ISR 14, "Unknown Interrupt!"
MAKE_MSG_ISR 15, "Coprocessor Fault!"
MAKE_MSG_ISR 16, "Machine Check!"
.else
  .warning "include/asm/nekonest/isr.s is already defined!"
.endif
