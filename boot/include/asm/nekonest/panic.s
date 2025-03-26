/*
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : panic.s                       |
 *    |                                            |
 *    O--------------------------------------------/
 *
 */

.ifndef PANIC
  .equ PANIC, 0

.section .panic.text, "ax", @progbits
.code32
.type panic, @function
.align 4
panic:
  cli

  pushl %ebp

  leal 8(%esp), %ebp

  pushl $.panic_str
  call printf

  pushl (%ebp)
  call printf
  
  hlt

.section .panic.string, "aS", @progbits
.type .panic_str, @object
.align 4
.panic_str:
  .asciz "NEKONEST PANIC! : "
.else 
  .warning "include/asm/lib/ata_lba.s is already defined!"
.endif
