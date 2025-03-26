/*
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : gdt.s                         |
 *    |                                            |
 *    O--------------------------------------------/
 *
 */

.ifndef GDT
  .equ GDT, 0

.global CODE_SEGMENT
.equ CODE_SEGMENT, 0b00001000

.global DATA_SEGMENT
.equ DATA_SEGMENT, 0b00010000

.section .gdt.data, "a", @progbits
.type .GDT_Entries_Start, @notype
.align 4
.GDT_Entries_Start:
  .type .null_segment, @object
  .null_segment:
    .long 0x00000000
    .long 0x00000000

  # Limit Segment (Low) - 16 bits
  # Base (Low) - 16 bits
  # Base (Mid) - 8 bits 
  # Access - 8 bits
  # Limit Segment (High) - 4 bits
  # Gran - 4 bits
  # Base (High) - 8 bits

  .type .code_segment, @object
  .code_segment:
    .word 0xFFFF
    .word 0x0000
    .byte 0x00
    .byte 0b10011010
    .byte 0b11001111
    .byte 0x00
  
  .type .data_segment, @object
  .data_segment:
    .word 0xFFFF
    .word 0x0000
    .byte 0x00
    .byte 0b10010010
    .byte 0b11001111
    .byte 0x00

.type .GDT_Entries_End, @notype
.GDT_Entries_End:

.global GDT_Ptr
.type .GDT_Ptr, @object
.align 4
GDT_Ptr:
  .word .GDT_Entries_End - .GDT_Entries_Start - 1
  .long .GDT_Entries_Start
.else
  .warning "include/asm/nekonest/gdt.s is already defined!"
.endif
