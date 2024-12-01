OUTPUT_FORMAT(elf32-i386)
ENTRY(k_main)
SECTIONS
{
  .text : 
  {
    *(.text)
  }

  .data :
  {
    *(.data)
  }

  .bss :
  {
    *(.bss)
  }
  /DISCARD/ : { *(.note*) *(.comment*) }
}
