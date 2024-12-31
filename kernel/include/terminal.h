/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : terminal.h                    |
 *    |  SRC MOD   : 31/12/2024                    | 
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
 * Flags para o terminal
 *
 */

#define TERMINAL_FLAGS_STATUS (__current_terminal->__flags & 0xFF)
#define TERMINAL_BUFFER_IS_READY (TERMINAL_FLAGS_STATUS & 0x01)

struct Terminal
{
  struct VGAState __vga_state;
  struct Keyboard __keyboard;
  
  __u8 __in_buffer[MAX_IN_BUFFER_SIZE];
  __u16 __in_offset;

  __u8 __out_buffer[MAX_OUT_BUFFER_SIZE];
  __u16 __out_offset;

  __u8 __flags;
};

extern struct Terminal* __current_terminal;

extern void terminal_init(struct Terminal*);
extern void terminal_out(const char);
extern void terminal_in(const __u8);
extern void terminal_cln_flag();
extern void terminal_set_flag(const __u8);

#endif
