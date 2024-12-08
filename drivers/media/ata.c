/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : ata.c                         |
 *    |  SRC MOD   : 08/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <media/ata.h>

__u8 ata_chs_write(__u8 __drive_head__, __u16 __sec_count_sec_num__, __u16 __cylin__) 
{
  
  __u8 __ata_controler_return = 0;

  /*
   *
   * Configurando o cabeçote e o drive
   *
   */

  __asm__ volatile(
    "movw %0, %%dx\n"
    "andb $0x0F, %1\n"
    "or $0b10100000, %1\n"
    "outb %1, %%dx\n"
    :
    : "i"(ATA_DRIVE_HEAD_PORT), "r"(__drive_head__)
    : "%dx"
  );

  /*
   *
   * Configurando número de setores que vamos ler e o número do setor
   *
   */

  __asm__ volatile(

    /*
     *
     * Sector Num
     *
     */

    "movw %2, %%dx\n"
    "movw %1, %%ax\n"
    "outb %%al, %%dx\n"
    

    /*
     *
     * Sector Count
     *
     */

    "movw %0, %%dx\n"
    "movb %%ah, %%al\n"
    "outb %%al, %%dx\n"
    :
    : "i"(ATA_SECTOR_COUNT_PORT), "r"(__sec_count_sec_num__), "i"(ATA_SECTOR_NUM_PORT)
    : "%dx", "%ax"
  );

  /*
   *
   * Configurando cilindro alto e baixo
   *
   */

  __asm__ volatile(

    /*
     *
     * Cylinder Low
     *
     */

    "movw %0, %%dx\n"
    "movw %1, %%ax\n"
    "outb %%al, %%dx\n"

    /*
     *
     * Cylinder High
     *
     */

    "movw %2, %%dx\n"
    "movb %%ah, %%al\n"
    "outb %%al, %%dx\n"
    :
    : "i"(ATA_CYLINDER_LOW_PORT), "r"(__cylin__), "i"(ATA_CYLINDER_HIGH_PORT)
    : "%dx", "%ax"
  );

  /*
   *
   * Mandando comando de leitura para o controlador ATA/SATA
   *
   */
  
  __asm__ volatile(
    "movw %0, %%dx\n"
    "movb %1, %%al\n"
    "outb %%al, %%dx\n"
    :
    : "i"(ATA_COMMAND_PORT), "i"(ATA_READ_COMMAND)
    : "%dx", "%al" 
  );

  /*
   *
   * Verificando se tudo ocorreu bem
   *
   */

  __asm__ volatile(
    "movw %1, %%dx\n"
    "xor %%eax, %%eax\n"
    "inb %%dx, %%al\n"
    : "=a"(__ata_controler_return)
    : "i"(ATA_ERROR_PORT)
    : "%dx"
  );
  return __ata_controler_return;
}
