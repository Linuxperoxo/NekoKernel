/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : shell.h                       |
 *    |  SRC MOD   : 18/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __K_SHELL__
#define __K_SHELL__

#include <sys/tty.h>

#define SH_BUFFER_SIZE 512
#define SH_COMMAND_BUFFER_SIZE 256
#define SH_PARAM_BUFFER_SIZE 256

#define DEFAULT_BIN_PATH "/bin/"

typedef struct shell_t
{
  char __buffer[SH_BUFFER_SIZE];
  char __command[SH_COMMAND_BUFFER_SIZE];
  char __param[SH_PARAM_BUFFER_SIZE];
}shell_t;

void shell_init();

#endif
