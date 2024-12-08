/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : gdt.h                         |
 *    |  SRC MOD   : 06/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

/*
 *
 * === COMO FUNCIONA OS REGISTRADORES DE SEGMENTO? ===
 *
 * No modo protegido os registradores de segmento 
 * (DS, SS, CS, etc.) é composto por 16 bits e segue
 * essa estrutura.
 *
 * Vamos usar de exemplo o CS(Code segment) como 
 * exemplo:
 *     
 *     7654 3210
 * CS: 0000 1000 -> Esse seria o registrador em bin
 *               -> Em hexa é 0x08
 *
 * Bits 0-1: Indica o nível de previlégio da operação
 * solicitada (0 a 3, onde 0 é o nível de maior 
 * previlégio)
 *
 * Bit 2: Indica se o segmento está na GDT ou na LDT
 *        * Quando esse bit é 0, o selecionador está
 *        apontando para o GDT
 *
 *        * Quando esse bit é 1, o selecionador está
 *        apontando para o LDT
 *
 * Bit 3-15: Esses representam o índice da entrada GDT
 * ou LDT. No valor 0x08 (0000 1000), os bits de índice
 * são 00001, o que significa índice 1, o índice começa
 * do 0
 *
 * Cada divisão desses bits tem um nome:
 *
 * RPL - Request Privilege Level (Bits 0-1): 00 -> 
 * Privilégio 0 (nível mais alto de privilégio)
 *
 * TI - Table Indicator (Bit 2): 0 -> Usando a GDT
 *
 * SI - Segment Index (Bits 3-15): 00001 -> Entrada 
 * 1 no GDT
 *
 * ===================================================
 *
 */

#ifndef __GDT__
#define __GDT__

#include <std/types.h>

/*
 *
 * Estrutura que representa uma entrada na GDT (Global Descriptor Table)
 *
 * 1. `__limit` (16 bits):
 * Este campo define o limite do segmento de memória.
 * O limite especifica a quantidade de memória que o segmento pode acessar.
 * Representa um valor de 16 bits do valor máximo de endereços do segmento.
 *
 * 2. `__base_low` (16 bits):
 * Contém os 16 bits mais baixos do endereço base do segmento.
 * O endereço base especifica onde o segmento começa na memória.
 * É usado para definir a base do segmento no espaço de memória.
 *
 * 3. `__base_middle` (8 bits):
 * Contém os bits 16 a 23 do endereço base do segmento.
 * A base do segmento é dividida em três partes: baixo, intermediário e alto, e esse campo é responsável por armazenar a parte intermediária do endereço base.
 *
 * 4. `__access` (8 bits):
 * Define as permissões de acesso do segmento, como leitura e escrita, e os direitos de acesso para o modo de usuário ou modo kernel.
 * A estrutura deste byte é:
 * -- (bit 7) P: P: Present bit. Indica se o segmento está presente na memória. Este bit deve ser definido como 1 para que o segmento seja considerado válido e acessível. Se for 0, o segmento não está presente e, portanto, não pode ser acessado.
 * -- (bits 6-5) DPL: Descriptor privilege livel field. Define o nível de privilégio da CPU para o segmento. O valor 0 indica o nível de privilégio mais alto (modo kernel), enquanto o valor 3 representa o nível de privilégio mais baixo (modo de usuário, para aplicativos).
 * -- (bit 4) S: Descriptor type bit. Se estiver limpo (0), o descritor define um segmento de sistema (por exemplo, um Segmento de Estado de Tarefa - TSS). Se estiver definido (1), o descritor define um segmento de código ou dados.
 * -- (bit 3) E: Executable bit. Se estiver limpo (0), o descritor define um segmento de dados. Se estiver definido (1), o descritor define um segmento de código, permitindo que instruções sejam executadas a partir dele.
 * -- (bit 2) DC: Direction bit/Conforming bit. Se 0, o segmento cresce para cima; se 1, cresce para baixo (Offset > Limit). Para descritores de código: Bit de Conformidade: se 0, o código só pode ser executado no nível de privilégio definido no DPL; se 1, pode ser executado de um nível igual ou inferior, mas o nível de privilégio do processador não muda após o salto. O DPL define o nível mais alto permitido para execução do segmento.
 * -- (bit 1) RW: Readable bit/Writable bit. Bit de Leitura. Se 0, o acesso de leitura não é permitido; se 1, é permitido. O acesso de escrita nunca é permitido para segmentos de código. Para segmentos de dados: Bit de Escrita. Se 0, o acesso de escrita não é permitido; se 1, é permitido. O acesso de leitura é sempre permitido para segmentos de dados.
 * -- (bit 0) A: Accessed bit. A CPU define esse bit automaticamente quando o segmento é acessado, a menos que ele já esteja definido como 1. Se o descritor da GDT estiver em páginas somente leitura e esse bit estiver definido como 0, a tentativa da CPU de configurá-lo resultará em uma falha de página (page fault). É recomendado deixá-lo como 1, a menos que haja necessidade específica de controlá-lo manualmente.
 *
 * 5. `__flags` (8 bits):
 * Este byte contém as flags que determinam a granularidade e o tamanho do segmento.
 * A estrutura deste byte é:
 * -- (bit 3) G: Granularity flag. Indica a unidade de escala do valor do Limit. Se 0, o Limit é expresso em blocos de 1 byte (granularidade de byte). Se 1, o Limit é expresso em blocos de 4 KiB (granularidade de página).
 * -- (bit 2) DB: Size Flag. Para segmentos de código: determina o tamanho padrão das instruções. Se 0, os operandos e endereços padrão têm 16 bits; se 1, têm 32 bits. Para segmentos de dados/pilha: controla o tamanho dos ponteiros e deslocamentos usados pelo processador. Se 0, o segmento é tratado como de 16 bits, usando registradores como SP; se 1, como de 32 bits, usando registradores como ESP.
 * -- (bit 1) L: Long-mode code flags. Se 1, o descritor define um segmento de código de 64 bits. Quando definido como 1, o DB deve ser sempre 0. Para qualquer outro tipo de segmento (outros segmentos de código ou segmentos de dados), o L deve ser 0.
 * -- (bit 0) Reserved
 *
 * 6. `__base_high` (8 bits):
 * Contém os bits 24 a 31 do endereço base do segmento.
 * - Completa o endereço base, especificando a parte mais alta do endereço de onde o segmento começa.
 *
 */

struct gdt_entry {
  __u16 __limit; // Limite do segmento (16 bits) - Define o tamanho do segmento.
  __u16 __base_low; // Parte inferior do endereço base (16 bits)
  __u8 __base_middle; // Parte intermediária do endereço base (8 bits).
  __u8 __access; // Acessibilidade e tipo de segmento (8 bits).
  __u8 __flags; // Flags de controle do segmento (8 bits).
  __u8 __base_high; // Parte superior do endereço base (8 bits).
}__attribute__((packed));

/*
 * Exemplificação dos Campos:
 *
 * 1. `__limit`:
 * Definido pelos 16 bits de `__limit`, o valor máximo que o segmento pode acessar.
 * 
 * 2. `__base_low`, `__base_middle`, `__base_high`:
 * Juntos, esses três campos representam o endereço base completo (32 bits) do segmento de memória.
 * 
 * 3. `__access`:
 * Define as permissões de acesso como: se o segmento está presente, se ele pode ser acessado no modo de usuário, se ele é de código ou dados, se permite leitura e escrita, e se foi acessado.
 * 
 * 4. `__flags`:
 * A granularidade e o tamanho do segmento são controlados pelos bits de `__flags`, com a granularidade especificando se a segmentação é de 4KB ou 1 byte e o tamanho definindo se o segmento é de 16 bits ou 32 bits.
 * 
 * 5. `__base_high`:
 * Completa a base de 32 bits do segmento de memória, especificando os bits mais altos do endereço base.
 *
 */

struct gdt_ptr {
  __u16 __limit; // Tamanho da GDT em Bytes - 1 (16 bits).
  __u32 __gdt_first_entry; // Endereço base da GDT (32 bits ou 64 bits, dependendo da arquitetura).
}__attribute__((packed));

/*
 *
 * struct gdt_ptr Membros:
 *  
 * 1. __limit: Tamanho do GDT em Bytes.
 *
 * EXEMPLO:
 *  Se temos 3 segmentos, ou seja, 3 structs gdt_entry, o __limit vai ser o seguinte: 
 *
 *  __gdt_prt.__limit = sizeof(gdt_entry) * 3 - 1;
 *
 *  - 1 pois começamos no índice 0, igual um array
 *
 * 2. __gdt_first_entry: Ponteiro para o início do array de gdt_entry
 *
 * Para entender melhor veja o arquivo gdt.c
 *
 */

extern void gdtinit();
extern void gdtsetentry(__u8, __u32, __u16, __u8, __u8, __u8);
extern void gdtflush(struct gdt_ptr*);

#endif
