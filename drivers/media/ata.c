/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : ata.c                         |
 *    |  SRC MOD   : 12/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <media/ata.h>

__u8 ata_chs_write(void* __dest__, __u8 __drive_head__, __u16 __sec_count_sec_num__, __u16 __cylin__) 
{
  
  __u8 __ata_controler_return = 0;

  READ_DISC:

  /*
   *
   * Configurando o cabeçote e o drive
   *
   */

  __asm__ volatile(
    "movb %1, %%al\n"
    "andb $0x0F, %%al\n"
    "orb $0b10100000, %%al\n" // Por default os 4 bits mais significativos são 1010, disco master
    "movw %0, %%dx\n"
    "outb %%al, %%dx\n"
    :
    : "i"(ATA_DRIVE_HEAD_PORT), "m"(__drive_head__)
    : "%al", "%dx"
  );

  /*
   *
   * Configurando número de setores que vamos ler e o número do setor
   *
   */

  __asm__ volatile(

    /*
     *
     * Sector Count
     *
     */

    "movw %1, %%dx\n"
    "movb $0x01, %%al\n"
    "outb %%al, %%dx\n"

    /*
     *
     * Sector Num
     *
     */

    "movw %0, %%dx\n"
    "movb %2, %%al\n"
    "outb %%al, %%dx\n"
    :
    : "i"(ATA_SECTOR_NUM_PORT), "i"(ATA_SECTOR_COUNT_PORT), "m"(__sec_count_sec_num__)
    : "%dx", "%al"
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
    "movb %2, %%al\n"
    "outb %%al, %%dx\n"

    /*
     *
     * Cylinder High
     *
     */

    "movw %1, %%dx\n"
    "movw %2, %%ax\n"
    "shr $0x08, %%ax\n"
    "outb %%al, %%dx\n"

    :
    : "i"(ATA_CYLINDER_LOW_PORT), "i"(ATA_CYLINDER_HIGH_PORT), "m"(__cylin__)
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
    "in %%dx, %%al\n"
    : "=a"(__ata_controler_return)
    : "i"(ATA_ERROR_PORT)
    : "%dx"
  );
  
  if(__ata_controler_return != 0)
    return __ata_controler_return;

  /*
   *
   * Esperando os dados serem colocados no buffer do controlador
   *
   */

  __asm__ volatile(
    "Lwait_disk:\n"
    "movw %0, %%dx\n"
    "inb %%dx, %%al\n"
    "test $0x08, %%al\n"
    "jz Lwait_disk\n"
    :
    : "i"(ATA_STATUS_PORT)
    :
  );

  /*
   *
   * Copiando os dados do buffer do controlador para o destino 
   *
   */

  __asm__ volatile(
    "movw %0, %%dx\n" 
    "xor %%si, %%si\n" // Nosso index é em SI
    "movl %1, %%edi\n" // Carregando o ponteiro de destino em EDI
    
    "Lreading_sector:\n"
    "inw %%dx, %%ax\n" // Lendo os dados do buffer do controlador
    "movw %%ax, (%%edi)\n" // Escrevendo os dados no destino
    
    /*
     *
     * Incrementando o ponteiro e o index, e depois vendo se todo o
     * setor foi lido e escrito
     *
     */

    "addl $0x02, %%edi\n"
    "addw $0x02, %%si\n"
    "cmpw $0x200, %%si\n"
    "jnz Lreading_sector\n"

    "movl %%edi, (%2)\n" // Atualizando ponteiro
    :
    : "i"(ATA_DATA_PORT), "p"(__dest__), "r"(&__dest__)
    : "%edi", "%dx", "%si", "%ax"
  );
  
  /*
   *
   * Aqui fazemos uma operação bit a bit para decrementar apenas os bits mais
   * significativos, eles representam a quantitade de setores que vamos ler, então
   * a cada leitura de setor, esse valor é decrementado
   *
   */

  __sec_count_sec_num__ = (((__sec_count_sec_num__ >> 8) - 0x01) << 8) | (__sec_count_sec_num__ & 0x00FF);

  if(((__sec_count_sec_num__ >> 8) & 0xFF) > 0)
  {
    
    /*
     *
     * Já aqui, também fazemos uma operação bit a bit, só que ela server para avançar para
     * o próximo setor de leitura, ou seja, se estamos lento o setor 1, temos que ir para o 
     * setor 2, é exatamento isso que estamos fazendo nessa operação
     *
     */

    __sec_count_sec_num__ = (__sec_count_sec_num__ & 0xFF00) | ((__sec_count_sec_num__ & 0x00FF) + 0x01); 
    
    /*
     *
     * Voltando a ler o novo setor
     *
     */

    goto READ_DISC;
  }
  return __ata_controler_return;
}
