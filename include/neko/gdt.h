/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : gdt.h                         |
 *    |  SRC MOD   : 09/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __K_GDT__
#define __K_GDT__

#include <std/int.h>

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
 * ====================================================
 *
 * Bits 0-1: Indica o nível de previlégio da operação
 * solicitada (0 a 3, onde 0 é o nível de maior 
 * previlégio)
 * 
 * ====================================================
 *
 * Bit 2: Indica se o segmento está na GDT ou na LDT
 *        * Quando esse bit é 0, o selecionador está
 *        apontando para o GDT
 *
 *        * Quando esse bit é 1, o selecionador está
 *        apontando para o LDT
 *        
 * ====================================================
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

struct gdtEntry
{
  __u16 __seg_limit_low;      // Limite do segmento (16 bits)
  __u16 __base_low;           // Base do segmento (parte baixa, 16 bits)
  __u8  __base_mid;           // Base do segmento (parte intermediária, 8 bits)
  __u8  __access;             // Acessibilidade e tipo do segmento (8 bits)
  __u8  __seg_limit_high : 4; // 4 bits para as flags de controle (P, DPL, S, Tipo de segmento)
  __u8  __gran : 4;           // 4 bits para granularidade (G, DB, L, AVL)
  __u8  __base_high;          // Base do segmento (parte alta, 8 bits)
} __attribute__((packed));

/*
 *
 * ====================================================
 *
 * 1. __seg_limit_low | __seg_limit_high:
 *    Define o limite (tamanho) do segmento de memória, representando a maior posição que o segmento pode alcançar.
 *    Os 16 bits de __seg_limit especificam os bits baixos do limite, enquanto os 4 bits mais altos do limite são
 *    armazenados em __seg_limit_high.
 *    
 * ====================================================
 *
 * 2. __base_low, __base_mid, __base_high:
 *    Juntos, esses três campos formam o endereço base completo de 32 bits do segmento de memória:
 *    - __base_low (16 bits): Contém os 16 bits mais baixos do endereço base.
 *    - __base_mid (8 bits): Contém os bits 16-23 do endereço base.
 *    - __base_high (8 bits): Contém os bits 24-31 do endereço base.
 *
 * ====================================================
 *    
 * 3. __access:
 *    Define as permissões de acesso e o tipo do segmento:
 *    - Bit 7 (P): Indica se o segmento está presente (1 = presente, 0 = ausente).
 *    - Bits 6-5 (DPL): Define o nível de privilégio do descritor (DPL):
 *      - 00: Ring 0 (nível mais alto)
 *      - 01: Ring 1
 *      - 10: Ring 2
 *      - 11: Ring 3 (nível mais baixo)
 *    - Bit 4 (S): Define o tipo de segmento:
 *      - 0: Segmento de sistema (ex.: TSS, LDT).
 *      - 1: Segmento de código ou dados.
 *    - Bits 3-0 (Tipo de Segmento): Define o tipo do segmento. Exemplos de valores:
 *      - 0xA: Código executável.
 *      - 0x2: Segmento de dados.
 *      
 * ====================================================
 *
 * 4. __gran (4 bits):
 *    Contém a granularidade e outras flags de controle do segmento:
 *    - Bit 7 (G): Granularidade (1 = segmento é dado em páginas de 4KB, 0 = segmento é dado em bytes).
 *    - Bit 6 (DB): Define o tamanho do segmento:
 *      - 0: Segmento de 16 bits.
 *      - 1: Segmento de 32 bits.
 *    - Bit 5 (L): Modo longo (utilizado em sistemas de 64 bits).
 *    - Bit 4 (AVL): Disponível para software, geralmente deixado como 0.
 *
 * ====================================================
 *
 * 5. __base_high:
 *    Completa o endereço base do segmento, representando os bits 24-31 do endereço base.
 *    Juntamente com __base_low e __base_mid, forma o endereço base completo de 32 bits do segmento.
 *
 * ====================================================
 *   
 */

struct gdtPtr
{
  __u16 __gdt_size;  // Tamanho do GDT
  __u32 __gdt_entry; // Endereço base do GDT
}__attribute__((packed));

/*
 *
 * ====================================================
 *
 * 1. __limit: 
 *    Tamanho total de entradas que temos no GDT
 *
 * EXEMPLO:
 *  Se temos 3 segmentos, ou seja, 3 structs gdtEntry, o __limit vai ser o seguinte: 
 *
 *  __gdtPrt.__limit = sizeof(gdt_entry) * 3 - 1;
 *
 *  "- 1" pois começamos no índice 0, igual um array
 *
 * ====================================================
 *
 * 2. __gdt_entry: 
 *    Ponteiro para o início do array de gdt_entry
 *
 * Para entender melhor veja o arquivo gdt.c
 *
 * ====================================================
 *
 */

extern void gdt_init();
extern void gdt_entry(__u8 __gdt_index__, __u32 __base__, __u32 __limit_20bits__, __u8 __gran__, __u8 __access__);
extern void gdt_update(struct gdtPtr* __gdt_ptr__);

#endif

