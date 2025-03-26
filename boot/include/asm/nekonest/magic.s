/*
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : magic.s                       |
 *    |                                            |
 *    O--------------------------------------------/
 *
 */

.ifndef MAGIC
  .equ MAGIC, 0

.section .magic.data, "a", @progbits
.word 0xAA55 # Magic flag MBR boot sector 
.else
  .warning "include/asm/nekonest/magic.s is already defined!"
.endif
