/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : linker.ld                     |
 *    |  SRC MOD   : 13/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

OUTPUT_FORMAT(elf32-i386)
ENTRY(k_loader)
SECTIONS
{
 
  /*
   *
   * O código no kernel será carregado nesse endereço (Code segment)
   * usaremos esse offset pois por padrão o segmento de código começa
   * no endereço 0x1000, você pode verificar isso usando o xxd no binário
   * de algo dos seus códigos, ou usando a farramenta readelf ou objdumb 
   *
   * OBS: O GCC não leva em consideração o início do arquivo, e sim o início
   * do código, então se eu colocar isso em 0x10000 vai acontecer um bug. 
   * 
   */

  /*
   *
   * O Código começa nesse endereço
   *
   */

  . = 0x11000;

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
}
