;
;
;
;    /--------------------------------------------O
;    |                                            |
;    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
;    |  AUTHOR    : Linuxperoxo                   |
;    |  FILE      : gdb.asm                       |
;    |  SRC MOD   : 24/11/2024                    |
;    |                                            |
;    O--------------------------------------------/
;    
;
;

global gdt_flush

gdt_flush:
  mov eax, [esp + 4]
  lgdt [eax]

  mov eax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  jmp 0x08:.flush

.flush:
  ret

