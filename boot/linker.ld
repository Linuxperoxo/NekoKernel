/*
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : linker.ld                     |
 *    |                                            |
 *    O--------------------------------------------/
 *
 */

OUTPUT_FORMAT(binary)
OUTPUT(nekonest)
ENTRY(.main)

origin_text  = 0x7C00;
origin_magic = 0x7DFE;

SECTIONS {
  . = origin_text;

  .main.text. : {
    . = ALIGN(4);
    nekonest.o(.text)
  }

  .main.data : {
    . = ALIGN(4);
    nekonest.o(.data)
  }
  
  .main.string : {
    . = ALIGN(4);
    nekonest.o(.string)
  }

  .std.text : {
    . = ALIGN(4);
    stdio.o(.text)
  }

  .gdt : {
    . = ALIGN(4);
    gdt.o(.gdt)
  }

  . = origin_magic;
 
  .magic : {
    magic.o(.magic)
  }

  .idt.text : {
    . = ALIGN(4);
    idt.o(.text)
  }

  .idt.data : {
    . = ALIGN(4);
    idt.o(.data)
  }

  .isr.text : {
    . = ALIGN(4);
    isr.o(.text)
  }

  .isr.data : {
    . = ALIGN(4);
    isr.o(.string)
  }

  /DISCARD/ : {
    *(.note*)
    *(.comment*)
    *(.gnu*)
  }
} 
