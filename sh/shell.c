/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : shell.c                       |
 *    |  SRC MOD   : 19/01/2025                    |
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

__attribute__((always_inline)) 
inline static void decode_buffer(shell_t* __sh__)
{

}
__attribute__((always_inline)) 
inline static void decode_command(shell_t* __sh__)
{ 

}

void shell_init()
{
  shell_t __sh;
  
  for(;;)
  {
    printf("$ ");
    scanf((char*)&__sh.__buffer, SH_BUFFER_SIZE);
    memset((char*)&__sh.__buffer, 0x00, SH_BUFFER_SIZE);
    memset((char*)&__sh.__command, 0x00, SH_COMMAND_BUFFER_SIZE);
    memset((char*)&__sh.__param, 0x00, SH_PARAM_BUFFER_SIZE);
  }
}

