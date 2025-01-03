/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : shell.c                       |
 *    |  SRC MOD   : 02/01/2025                    |
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
  for(;;)
  {
    printf((char*)SHELL_PS1);
    scanf((char*)&__shell.__buffer, SHELL_IN_BUFFER_SIZE);
  }
}

