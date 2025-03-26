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
ENTRY(main)

origin_text  = 0x7C00;
origin_magic = 0x7DFE;

SECTIONS {
  . = origin_text;

  .nekonest.text : {
    *(.nekonest.text)
    *(.stdlib.text)
    *(.panic.text)
  }

  .nekonest.data : {
    . = ALIGN(4);
    *(.nekonest.data)
    *(.nekonest.string)
    
    . = ALIGN(4);
    *(.stdlib.data)
    *(.stdlib.bss)
    *(.stdlib.string)
    *(.panic.string)
  }

  .gdt.data : {
    . = ALIGN(4);
    *(.gdt.data)
  }

  . = origin_magic;
  
  .magic.data : {
    *(.magic.data)
  }
  
  .isr.text : {
    . = ALIGN(4);
    *(.isr.text)
  }
  
  .isr.data : {
    . = ALIGN(4);
    *(.isr.string)
  }
  
  .idt.data : {
    . = ALIGN(4);
    *(.idt.data)
  }

  /DISCARD/ : {
    *(.note*)
    *(.comment*)
    *(.gnu*)
  }
} 
