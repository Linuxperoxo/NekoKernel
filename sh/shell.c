/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : shell.c                       |
 *    |  SRC MOD   : 25/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <std/io.h>
#include <std/str.h>
#include <std/utils.h>
#include <sys/kmem.h>
#include <sys/shell.h>
#include <sys/tty.h>
#include <sys/vfs.h>

static shell_t *__sh = NULL;

__attribute__((always_inline)) inline static void
decode_buffer(shell_t *__sh__) {}
__attribute__((always_inline)) inline static void
decode_command(shell_t *__sh__) {}

__u8 shell_exec(const char *__param__) {
  for (;;) {
    printf("Dentro do shell!\n");
  }
  return 0;
}

void shell_init() {
  __sh = (shell_t *)kmalloc(sizeof(shell_t));
  vfs_mkfile("/bin/sh", ROOT_UID, ROOT_GID, READ_O | WRITE_O | EXEC_O,
             &shell_exec);
}
