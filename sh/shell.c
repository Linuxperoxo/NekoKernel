/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : shell.c                       |
 *    |  SRC MOD   : 18/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <sys/shell.h>
#include <std/str.h>
#include <std/io.h>
#include <sys/tty.h>
#include <std/utils.h>
#include <sys/vfs.h>

#define SH_PS1 "$ "

__attribute__((always_inline)) inline void decode_buffer(shell_t* __sh__)
{
  char* __tmp_src    = (char*)&__sh__->__buffer;
  char* __tmp_dest   = (char*)&__sh__->__command;  
  __u16 __index      = 0x00;
  __u8  __command_ok = 0x00;
  
  /*
   *
   * Caso tenha espaços antes do comando vamos tirar
   *
   */

  while(*__tmp_src == ' ' && __index < SH_BUFFER_SIZE)
  {
    ++__tmp_src;
    ++__index; 
  }

  while(__index < SH_BUFFER_SIZE)
  {
    switch(*__tmp_src)
    {
      case '\0':
        return;
      break;

      case ' ':
        if(!__command_ok)
        {
          __tmp_dest   = (char*)&__sh__->__param;
          __command_ok = 0x01;
          
          while(*__tmp_src == ' ' && __index < SH_BUFFER_SIZE)
          {
            ++__tmp_src;
            ++__index; 
          }
        }
      break;
    }
    *__tmp_dest = *__tmp_src;
    
    ++__tmp_src;
    ++__tmp_dest;
    ++__index;
  }
}

__attribute__((always_inline)) inline void decode_command(shell_t* __sh__)
{  

}

void shell_init()
{
  shell_t __sh;
  
  for(;;)
  {
    printf((char*)SH_PS1);
    scanf((char*)&__sh.__buffer, SH_BUFFER_SIZE);
    decode_buffer(&__sh);
    printf((char*)&__sh.__command);
    printf((char*)&__sh.__param);
    memset((char*)&__sh.__buffer, 0x00, SH_BUFFER_SIZE);
    memset((char*)&__sh.__command, 0x00, SH_COMMAND_BUFFER_SIZE);
    printf((char*)&__sh.__command);
    memset((char*)&__sh.__param, 0x00, SH_PARAM_BUFFER_SIZE);
  }
}

