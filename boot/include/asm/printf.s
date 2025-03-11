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
  movl (%ebp), %esi
  movl $VGA_FRAMEBUFFER, %edi
  movl .vga_status, %ebx
  movb $VGA_BC_DF_COLOR, %ah
  shlb $4, %ah
  movb $VGA_CH_DF_COLOR, %ah

  cld
  
  1:
    movb (%esi), %al
    testb 0xFF, %al # if(%al == 0) { return; }
    jz 4f
    cmpb $BRK_LINE, %al
    je 3f  
    incl %esi
    movw %ax, (%edi, %ebx, 2)
    incl %ebx
    cmpl $VGA_FRAMEBUFFER_LEN, %ebx # if(%ebx == $VGA_FRAMEBUFFER_LEN) { %ebx = 0; }
    cmovzl %edx, %ebx
    jmp 1b

  3:
    incl %esi
    jmp 1b 

  4:
    popl %esi
    popl %edi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    popl %ebp
    ret
  
.section .bss
.type .vga_status, @object
.align 2
.vga_status:
  .space 4, 0 # Framebuffer offset

