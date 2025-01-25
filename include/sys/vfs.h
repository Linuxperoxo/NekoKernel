/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : vfs.h                         |
 *    |  SRC MOD   : 19/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __K_VFS__
#define __K_VFS__

#include <std/int.h>

#define MAX_NAME 256

#define DIR_TYPE 0x01
#define FILE_TYPE 0x02
#define BLOCK_TYPE 0x04
#define CHAR_TYPE 0x08

#define ROOT_UID 0x00
#define ROOT_GID 0x00

#define EXEC_O 0x01
#define WRITE_O 0x02
#define READ_O 0x04

#define IS_DIR(__vfs) (__vfs->__type & DIR_TYPE)
#define IS_FILE(__vfs) ((__vfs->__type & FILE_TYPE) >> 1)
#define IS_BLOCK(__vfs) ((__vfs->__type & BLOCK_TYPE) >> 2)
#define IS_CHAR(__vfs) ((__vfs->__type & CHAR_TYPE) >> 3)
#define IS_EXEC(__vfs) (__vfs->__permission & EXEC_O)
#define IS_WRITE(__vfs) ((__vfs->__permission & WRITE_O) >> 1)
#define IS_READ(__vfs) ((__vfs->__permission & READ_O) >> 2)

typedef __u8 uid_t;
typedef __u8 gid_t;
typedef __u8 type_t;
typedef __u8 mode_t;
typedef __u32 offset_t;
typedef __u32 uinode_t;
typedef __u32 ssize_t;
typedef __u8 opened_t;

typedef __u8 (*write_t)(offset_t, void *);
typedef __u8 (*read_t)(offset_t, void *);
typedef __u8 (*exec_t)(const char *);

typedef struct vfs_t vfs_t;
typedef vfs_t *child_t;
typedef vfs_t *brother_t;
typedef vfs_t *parent_t;

typedef struct vfs_t {
  char __name[MAX_NAME + 1];
  uid_t __uid;
  gid_t __gid;
  mode_t __permission : 4;
  type_t __type : 4;
  child_t __child;
  brother_t __brother;
  parent_t __parent;

  /*
   *
   * Operações para o arquivo
   *
   */

  write_t __write;
  read_t __read;
  exec_t __exec;
} vfs_t;

typedef struct inode_t {
  uinode_t __inode;
  ssize_t __size;
  type_t __type;
} inode_t;

typedef struct fd_t {
  opened_t __is_open;
  mode_t __open_mode;
  vfs_t *__vfs_file;
} fd_t;

void vfs_init();
vfs_t *vfs_exist(const char *__file__);
__u8 vfs_type(const char *__file__);
__u8 vfs_mkdir(const char *__file__, uid_t __uid__, gid_t __gid__,
               mode_t __perm__);
__u8 vfs_mkfile(const char *__file__, uid_t __uid__, gid_t __gid__,
                mode_t __perm__, exec_t __exec__);
__u8 vfs_mkchfile(const char *__file__, uid_t __uid__, gid_t __gid__,
                  mode_t __perm__, read_t __read__, write_t __write__);
__u8 vfs_mkbcfile(const char *__file__, uid_t __uid__, gid_t __gid__,
                  mode_t __perm__, read_t __read__, write_t __write__);
__u8 vfs_write(const char *__file__, offset_t __offset__, void *__dest__);
__u8 vfs_read(const char *__file__, offset_t __offset__, void *__dest__);

#endif
