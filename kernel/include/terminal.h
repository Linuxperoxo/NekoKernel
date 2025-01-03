/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : terminal.h                    |
 *    |  SRC MOD   : 02/01/2025                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __K_TERMINAL__
#define __K_TERMINAL__

#include <std/int.h>
#include <video/vga/vga.h>
#include <device/io/keyboard/keyboard.h>

#define MAX_IN_BUFFER_SIZE DEFAULT_WIDTH * DEFAULT_HEIGHT
#define MAX_OUT_BUFFER_SIZE DEFAULT_WIDTH * DEFAULT_HEIGHT

/*
 *
 * Runtime flags
 *
 */

#define TERMINAL_BUFFER_IS_READY ((terminal_sts_flag() >> 4) & 0x01)

/*
 *
 * Custom flags
 *
 */

#define TERMINAL_INV_INPUT (terminal_sts_flag() & 0x01)

struct Terminal
{
  struct VGAState __vga_state;
  struct Keyboard __keyboard;
  
  __u16 __in_offset;
  __u16 __out_offset;

  __u8 __out_buffer[MAX_OUT_BUFFER_SIZE];
  __u8 __in_buffer[MAX_IN_BUFFER_SIZE];
  __u8 __flags;
};

extern void terminal_init();
extern void terminal_out(const char);
extern void terminal_in(const __u8);
extern void terminal_cln_flag();
extern void terminal_set_flag(const __u8);
extern __u8 terminal_sts_flag();
extern void terminal_cpy_in(void*, __u16);

#endif
