section .text
  global _start
  extern k_main
  
  align 4

  dd 0x1BADB002          ; magic
  dd 0x00                ; flags
  dd - (0x1BADB002 + 0x00) ; checksum

_start:
  cli 
  call k_main
  hlt
