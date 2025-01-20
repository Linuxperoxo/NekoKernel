/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : tty.h                         |
 *    |  SRC MOD   : 20/01/2025                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __K_TERMINAL__
#define __K_TERMINAL__

#include <std/int.h>
#include <video/vga/vga.h>

#define TTY_WIDTH DEFAULT_WIDTH
#define TTY_HEIGHT DEFAULT_HEIGHT
#define TTY_BF_SIZE 2056
#define TTY_DF_CH_COLOR 0x0F
#define TTY_DF_BC_COLOR 0x00
#define TTY_DF_COLOR (((TTY_DF_BC_COLOR << 4) & 0x70) | (TTY_DF_CH_COLOR & 0x0F))

#define TTY_INV_OUT_BIT 0x01
#define TTY_BF_READY_BIT 0x10

#define TTY_IS_INV_OUT(__tty) (__tty->__flags & TTY_INV_OUT_BIT)
#define TTY_BF_IS_READY(__tty) ((__tty->__flags & TTY_BF_READY_BIT) >> 4)  

#define TTY_BF_IN_OVERFLOW(__tty) (__tty->__offset >= TTY_BF_SIZE) 
#define TTY_CLEAN_BF(__tty) __tty->__offset = 0x00
#define TTY_ENABLE_BF_READY_BIT(__tty) __tty->__flags |= TTY_BF_READY_BIT
#define TTY_DISABLE_BF_READY_BIT(__tty) __tty->__flags &= (~ TTY_BF_READY_BIT)
#define TTY_CPY_CH_TO_BF(__tty, __ch) __tty->__buffer[__tty->__offset++] = __ch
#define TTY_UPDATE_PTR(__tty) vga_mov_ptr(__tty->__win.__row, __tty->__win.__col);
#define TTY_ROW_IN_COLISION(__tty) (__tty->__win.__row >= TTY_HEIGHT)
#define TTY_COL_IN_COLISION(__tty) (__tty->__win.__col >= TTY_WIDTH)

#define TTY_IF_ROW_IN_COLISION(__tty) if(TTY_ROW_IN_COLISION(__tty)) \
                                      { \
                                        __tty->__win.__row = TTY_HEIGHT - 1; \
                                        __tty->__win.__col = 0x00; \
                                        vga_screen_down(); \
                                      }

#define TTY_IF_COL_IN_COLISION(__tty) if(TTY_ROW_IN_COLISION(__tty)) \
                                      { \
                                        __tty->__win.__row += 0x01; \
                                        __tty->__win.__col  = 0x00; \
                                      }

typedef __u8 tty_bf_t;
typedef __u8 tty_flag_t;
typedef __u16 tty_off_t;
typedef __u8 screen_x_t;
typedef __u8 screen_y_t;
typedef __u8 screen_color_t;

/*
 *
 * Coordenadas x,y da tela
 *
 * __row:
 *  - Coordenada y
 *
 * __col:
 *  - Coordenada x
 *
 * __color: 
 *  - Bits (0-3): Cor do caractere
 *  - Bits (4-6): Cor do fundo
 *  - Bit (7): Caractere piscante
 *
 */

typedef struct tty_screen_t
{
  screen_x_t __col;
  screen_y_t __row;
  screen_color_t __color;
}tty_screen_t;

/*
 *
 * __buffer:
 *  - Armazena todo o buffer de IN 
 *
 * __flags:
 *  - Bits (0-3): Controla o funcionamento do terminal
 *  - Bits (4-7): Controle de ações do terminal, por exemplo, se o buffer está pronto
 *
 *  - Bit 0: Define se o output deve ser invisível
 *  - Bit 1: Reservado
 *  - Bit 2: Reservado
 *  - Bit 3: Reservado
 *  - Bit 4: Indica se o buffer está pronto
 *  - Bit 5: Reservado
 *  - Bit 6: Reservado
 *  - Bit 7: Reservado
 *
 * __offset:
 *  - Offeset de onde o buffer está, serve para controlar o tamanho do buffer
 *
 * __win:
 *  - Struct que controla as coordenadas x,y
 *
 */

typedef struct tty_t
{
  tty_bf_t     __buffer[TTY_BF_SIZE];
  tty_flag_t   __flags;
  tty_off_t    __offset;
  tty_screen_t __win;
}tty_t;

void tty_init();
void tty_switch(tty_t* __tty__);

#endif
