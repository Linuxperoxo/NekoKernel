/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : terminal.h                    |
 *    |  SRC MOD   : 21/12/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __K_TERMINAL__
#define __K_TERMINAL__

#include <std/int.h>
#include <video/vga/vga.h>

#define MAX_IN_BUFFER_SIZE DEFAULT_WIDTH * DEFAULT_WIDTH
#define MAX_OUT_BUFFER_SIZE DEFAULT_HEIGHT * DEFAULT_WIDTH
#define MAX_ERR_BUFFER_SIZE 256

extern void terminal_init();
extern void terminal_init_vga();
extern void terminal_out_write(const char);
extern void terminal_output_clear();
extern void terminal_input_clear();
extern __u8 terminal_get_last_key();
extern void terminal_cpy_input(char*, __u32);

#endif
