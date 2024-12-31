/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : shell.c                       |
 *    |  SRC MOD   : 30/12/2024                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <shell.h>
#include <std/str.h>
#include <std/io.h>
#include <terminal.h>

struct Shell
{
  char __buffer[SHELL_IN_BUFFER_SIZE];
};

struct Shell __shell;

void shell_init()
{
  while(1)
  {
    printf((char*)SHELL_PS1);
  }
}

