/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : vga.h                         |
 *    |  SRC MOD   : 25/11/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __VGA__
#define __VGA__

#include <types.h>

/*
 *
 * VGA funciona da seguinte forma:
 *
 * Vamos ter um bloco de memória de 80 * 25,
 * cada elemento tem o tamanho de 2 bytes,
 * o byte mais significativo representa o 
 * caractere que vai ser mostrado, e o segundo
 * byte são as cores desse caractere.
 *
 * EXEMPLO : 
 *
 * |  1  |  2  |  3  |  4  |  5  |  6  |
 *
 * Os bytes 1, 3 e 5 são os caracteres, os bytes
 * 2, 4, 6 são as cores de cada um dos caracteres,
 *
 * O byte de cor funciona da seguinte forma:
 *  
 *  |0|101|1111| -> Binário
 *  
 * 1111: Os 4 bits menos significativos representam
 * a cor do caractere, que vai de 0x00 até 0x0F, 
 * com cada hexa representando uma cor diferente.
 *
 * 101: Esses 3 bits mais significativos é a cor
 * cor de background de cada caractere.
 *
 * 0: Representa se é um caractere piscante, 0 para
 * não piscante e 1 para piscante.
 *   
 */

/*
 *
 * Cores para o backgorund e caracteres 
 *
 */

#define BLACK_CHAR_COLOR 0x00
#define BLUE_CHAR_COLOR 0x01
#define GREEN_CHAR_COLOR 0x02
#define CYAN_CHAR_COLOR 0x03
#define RED_CHAR_COLOR 0x04
#define PURPLE_CHAR_COLOR 0x05
#define BROWN_CHAR_COLOR 0x06
#define GRAY_CHAR_COLOR 0x07
#define DGRAY_CHAR_COLOR 0x08
#define YELLOW_CHAR_COLOR 0x0E
#define WHITE_CHAR_COLOR 0x0F

#define BLACK_BC_COLOR 0x00
#define BLUE_BC_COLOR 0x01
#define GREEN_BC_COLOR 0x02
#define CYAN_BC_COLOR 0x03
#define RED_BC_COLOR 0x04
#define PURPLE_BC_COLOR 0x05
#define BROWN_BC_COLOR 0x06
#define GRAY_BC_COLOR 0x07

#define DEFAULT_CHAR_COLOR WHITE_CHAR_COLOR
#define DEFAULT_BC_COLOR BLACK_BC_COLOR

/*
 *
 * Informações do VGA
 *
 */

#define DEFAULT_HEIGHT 24
#define DEFAULT_WIDTH 80
#define FRAME_BUFFER_ADDRS 0xb8000
#define VGA_CTRL_PORT 0x3D4 // Porta de controle
#define VGA_DATA_PORT 0x3D5 // Porta de dados

void init_vga();
void print_screen(const char*);
void clear_screen();
void vga_ptr(__u8, __u8);

#endif
