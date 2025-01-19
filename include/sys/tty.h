/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : tty.h                         |
 *    |  SRC MOD   : 19/01/2025                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __K_TERMINAL__
#define __K_TERMINAL__

#include <std/int.h>

#define TTY_BF_SIZE 1024
#define TTY_WIDTH 80
#define TTY_HEIGHT 25
#define TYY_DF_CH_COLOR 0x0F
#define TTY_DF_BC_COLOR 0x00
#define TTY_DF_COLOR (((TTY_DF_BC_COLOR << 4) & 0x70) | (TTY_DF_CH_COLOR & 0x0F))

typedef __u8 tty_bf_t;
typedef __u8 tty_flag_t;
typedef __u16 tty_off_t;
typedef __u8 win_cor_t;

typedef struct tty_win_t
{
  win_cor_t __row;
  win_cor_t __col;
}tty_win_t;

typedef struct tty_t
{
  tty_bf_t    __buffer[TTY_BF_SIZE];
  tty_flag_t  __flags;
  tty_off_t   __offset;
  tty_win_t   __win;
}tty_t;

void tty_init();
void tty_switch(tty_t* __tty__);

#endif
