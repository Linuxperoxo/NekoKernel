#ifndef __K_VFS__
#define __K_VFS__

#include <std/int.h>

#define MAX_NAME  256
#define DIR_TYPE  0x01
#define FILE_TYPE 0x02

#define FILE_EXEC_TYPE 0x03;
#define FILE_DIR_TYPE  0x02;
#define FILE_FILE_TYPE 0x01;

typedef __u8  uid_t;
typedef __u8  gid_t;
typedef __u8  type_t;
typedef __u8  mode_t;
typedef __u32 offset_t;
typedef __u32 uinode_t;
typedef __u32 size_t;

typedef __u8 (*write_t)(__u8, offset_t, void*);
typedef __u8 (*read_t)(__u8, offset_t, void*);
typedef __u8 (*open_t)(const char*, __u8);
typedef __u8 (*mkdir_t)(const char*);
typedef void (*close_t)(__u8);
typedef void (*exec_t)(const char*);

typedef struct vfs_t vfs_t;
typedef vfs_t* child_t;
typedef vfs_t* brother_t;
typedef vfs_t* parent_t;

typedef struct vfs_t
{
  char      __name[MAX_NAME];
  uid_t     __uid;
  gid_t     __gid;
  mode_t    __permission;
  type_t    __type;
  child_t   __child;
  brother_t __brother;
  parent_t  __parent;
  
  /*
   *
   * Operações para o arquivo
   *
   */

  write_t  __write;
  read_t   __read;
  open_t   __open;
  close_t  __close;
  mkdir_t  __mkdir;
  exec_t   __exec;
}vfs_t;

typedef struct inode_t
{
  uinode_t __inode;
  size_t  __size;
  type_t  __type;
}inode_t;

typedef struct fd_t
{
  vfs_t* __file;
  mode_t __open_mode;
}fd_t;

extern void vfs_init();
extern vfs_t* vfs_search(const char* __file__);

#endif
