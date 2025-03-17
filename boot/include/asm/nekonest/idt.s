/*
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : idt.s                         |
 *    |                                            |
 *    O--------------------------------------------/
 *
 */

.extern CODE_SEGMENT

.macro CREATE_IDT_STRUCT __num__, __seg_selector__, __flags__
.type .idt_entry\__num__, @object
.idt_entry\__num__:
  .word 0
  .word \__seg_selector__
  .byte 0
  .byte \__flags__
  .word 0
.endm

.section .data
.type .idt_start, @notype
.align 4
.idt_start:
  CREATE_IDT_STRUCT 0, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 1, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 2, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 3, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 4, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 5, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 6, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 7, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 8, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 9, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 10, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 11, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 12, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 13, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 14, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 15, CODE_SEGMENT, 0x8E 
  CREATE_IDT_STRUCT 16, CODE_SEGMENT, 0x8E 

.type .isr_end, @notype
.idt_end:

.global isr_ptr
.type isr_ptr, @object
.align 4
isr_ptr:
  .word .idt_end - .idt_start - 1 # __limit
  .long .idt_start                # __isr_ptr
