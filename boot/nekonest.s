.extern printf

.section .text
.code32
.global main
.type main, @function
.align 4
main:
  cli
  movl $0x1000, %eax
  call printf
  hlt

