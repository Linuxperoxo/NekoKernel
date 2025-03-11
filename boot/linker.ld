/*
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : nekonest.s                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 */

OUTPUT_FORMAT(binary)
ENTRY(main)

origin_text  = 0x7C00;
origin_magic = 0x7DFE;

SECTIONS {
  . = origin_text;

  .text : {
    . = ALIGN(4);
    *(.text)
  }

  .data : {
    . = ALIGN(4);
    *(.data)
    . = ALIGN(4);
    *(.gdt)
    . = ALIGN(4);
    *(.idt)
  }
  
  .string : {
    . = ALIGN(4);
    *(.string)
  }

  . = origin_magic;

  .magic : {
    *(.magic)
  }

  /DISCARD/ : {
    *(.note*)
    *(.comment*)
    *(.gnu*)
  }
} 
