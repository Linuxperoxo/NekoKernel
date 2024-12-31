/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : terminal.h                    |
 *    |  SRC MOD   : 30/12/2024                    | 
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

struct Terminal
{
  struct VGAState __vga_state;
  
  __u8 __in_buffer[MAX_IN_BUFFER_SIZE];
  __u16 __in_offset;

  __u8 __out_buffer[MAX_OUT_BUFFER_SIZE];
  __u16 __out_offset;
};

extern void terminal_init(struct Terminal*);
extern void terminal_out(const char);
extern void terminal_in(const char);

#endif
