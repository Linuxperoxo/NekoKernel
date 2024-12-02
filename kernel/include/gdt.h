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

struct gdt_entry {

}__attribute__((packed));

struct gdt_prt {
  
}__attribute__((packed));

#endif
