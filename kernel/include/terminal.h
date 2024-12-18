/*
 * 
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : terminal.h                    |
 *    |  SRC MOD   : 18/12/2024                    | 
 *    |                                            |
 *    O--------------------------------------------/
 *    
 *
 */

#ifndef __K_TERMINAL__
#define __K_TERMINAL__

#include <std/types.h>

#define MAX_IN_BUFFER_SIZE 512
#define MAX_OUT_BUFFER_SIZE 512
#define MAX_ERR_BUFFER_SIZE 256

struct VGAState;
struct Keyboard;

struct Terminal
{
  struct VGAState* __this_section_vga_state;
  struct Keyboard* __this_section_keyboard;
  
  __u8 __terminal_in_buffer[MAX_IN_BUFFER_SIZE];
  __u16 __in_buffer_offset;

  __u8 __terminal_out_buffer[MAX_OUT_BUFFER_SIZE];
  __u16 __out_buffer_offset;

  __u8 __terminal_err_buffer[MAX_ERR_BUFFER_SIZE];
  __u16 __err_buffer_offset;
};

extern struct Terminal __current_section;

extern void terminal_init();
extern void terminal_init_vga();
extern void terminal_out_write(const char);
extern void terminal_clean(); 

#endif
