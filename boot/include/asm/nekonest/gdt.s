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

.section .gdt, "a"
.GDT_Kernel_Entries_Start:
  .type .null_segment, @object
  .align 4
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

  .type .kernel_code_segment, @object
  .align 4
  .kernel_code_segment:
    .word 0xFFFF
    .word 0x0000
    .byte 0x00
    .byte 0b10011010
    .byte 0b11001111
    .byte 0x00
  
  .type .kernel_data_segment, @object
  .align 4
  .kernel_data_segment:
    .word 0xFFFF
    .word 0x0000
    .byte 0x00
    .byte 0b10010010
    .byte 0b11001111
    .byte 0x00

.type .GDT_Kernel_Entries_End, @notype
.GDT_Kernel_Entries_End:

.global GDT_Ptr
.type GDT_Ptr, @object
.align 4
GDT_Ptr:
  .word .GDT_Kernel_Entries_End - .GDT_Kernel_Entries_Start - 1
  .long .GDT_Kernel_Entries_Start
  
