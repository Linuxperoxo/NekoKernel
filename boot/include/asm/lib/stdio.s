/*
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : stdio.s                       |
 *    |                                            |
 *    O--------------------------------------------/
 *
 */

.ifndef LIBSTDIO
  .equ LIBSTDIO, 0

.equ VGA_FRAMEBUFFER, 0xB8000
.equ VGA_ROW_LEN, 25
.equ VGA_COL_LEN, 80
.equ VGA_FRAMEBUFFER_LEN, 0x7D0 # 80 * 25 = 2000
.equ VGA_CH_DF_COLOR, 0x0F # White 
.equ VGA_BC_DF_COLOR, 0x00 # Black
.equ BRK_LINE, 0x0A

.section .text
.code32
.global printf
.type printf, @function
.align 4
printf:
  pushl %ebp

  leal 8(%esp), %ebp

  pushl %eax
  pushl %ebx
  pushl %ecx
  pushl %edx
  pushl %edi
  pushl %esi

  xorl %edx, %edx
  xorl %eax, %eax

  movw vga_status, %bx
  movb %bl, %al
  movb $VGA_COL_LEN, %dl
  mull %edx
  movl %eax, %edx
  addb %bh, %dl

  movl (%ebp), %esi
  movl $VGA_FRAMEBUFFER, %edi
  movb $VGA_BC_DF_COLOR, %ah
  shlb $4, %ah
  movb $VGA_CH_DF_COLOR, %ah

  1:
    cmpb $VGA_COL_LEN, %bh # if(%bh == $VGA_COL_LEN)
    je 3f

  2:
    movb (%esi), %al
    cmpb $0x00, %al # if(%al == 0)
    je 5f
    cmpb $0x0A, %al # if(%al == 0xA)
    je 4f
    incl %esi
    movw %ax, (%edi, %edx, 2)
    incb %bh
    incl %edx
    jmp 1b
    
  3:
    call 3f
    jmp 2b

  3:
    pushl %eax

    xorl %eax, %eax
    xorl %edx, %edx

    incb %bl
    movb %bl, %al
    movb $VGA_COL_LEN, %dl
    mull %edx
    movl %eax, %edx
    xorb %bh, %bh

    popl %eax
    ret

  4:
    call 3b
    incl %esi
    jmp 2b 

  5:
    movw %bx, vga_status

    popl %esi
    popl %edi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    popl %ebp
    ret

.global clearf
.type clearf, @function
.align 4
clearf:
  pushl %eax
  pushl %ecx
  pushl %edi
  
  movl $VGA_FRAMEBUFFER_LEN, %ecx
  xorl %eax, %eax
  movl $VGA_FRAMEBUFFER, %edi
  rep stosw

  movw %ax, vga_status

  popl %edi
  popl %ecx
  popl %eax
  ret

.section .bss
.global vga_status
.type vga_status, @object
.align 2
vga_status:
  .space 1, 0 # __current_row
  .space 1, 0 # __current_col
.else 
  .warning "stdio.s is already defined!"
.endif
