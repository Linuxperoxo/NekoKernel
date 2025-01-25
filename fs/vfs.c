/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : vfs.c                         |
 *    |  SRC MOD   : 23/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <std/io.h>
#include <std/str.h>
#include <std/utils.h>
#include <sys/kmem.h>
#include <sys/vfs.h>

static vfs_t *__current_dir = NULL;
static vfs_t *__root_dir = NULL;

/*
 *
 * Internal Functions
 *
 */

static uint8_t vfs_way_cmp(const char *__restrict__ __vfs_name__,
                           const char **__restrict__ __dir_cmp__) {
  if (*__vfs_name__ == '\0' || **__dir_cmp__ == '\0')
    return 1;

  while (*__vfs_name__ != '\0' && **__dir_cmp__ != '\0') {
    if (*__vfs_name__ != **__dir_cmp__)
      return 1;

    ++__vfs_name__;
    ++(*__dir_cmp__);
  }

  if (*__vfs_name__ == '\0') {
    switch (**__dir_cmp__) {
    case '/':
      while (**__dir_cmp__ == '/')
        ++(*__dir_cmp__);

      if (**__dir_cmp__ == '\0')
        return 0;
      return 2;
      break;

    case '\0':
      return 0;
      break;

    default:
      return 1;
      break;
    }
  }
  return 0;
}

static vfs_t *vfs_search(const char *__file__) {
  vfs_t *__tmp = __current_dir;

  while (*__file__ == '/') {
    __tmp = __root_dir;
    __file__++;
  }

  if (*__file__ == '\0')
    return __tmp;

  __tmp = __tmp->__child == NULL ? NULL : __tmp->__child;

  while (__tmp != NULL) {
    const char *__bc_file = __file__;
    __u8 __return = vfs_way_cmp((char *)&__tmp->__name, &__file__);

    switch (__return) {
    case 2:
      __bc_file = __file__;
      __tmp = __tmp->__type == DIR_TYPE ? __tmp->__child : NULL;
      break;

    case 1:
      __file__ = __bc_file;
      __tmp = __tmp->__brother;
      break;

    case 0:
      return __tmp;
      break;
    }
  }
  return __tmp;
}

static __u8 vfs_create(const char *__file__, uid_t __uid__, gid_t __gid__,
                       mode_t __perm__, type_t __type__, read_t __read__,
                       write_t __write__, exec_t __exec__) {
  if (vfs_search(__file__) != NULL)
    return 1;

  __u32 __size = strlen(__file__);

  vfs_t **__interator = NULL;
  vfs_t *__parent = NULL;

  char *__buffer = (char *)kmalloc(__size + 1);
  char *__file_name = (char *)kmalloc(sizeof(char *) * MAX_NAME);
  char *__end_ptr = __buffer + (__size - 1);

  memset(__buffer, 0x00, __size + 1);
  memcpy(__buffer, (void *)__file__, __size);

  while (*__end_ptr == '/')
    *__end_ptr-- = '\0';

  while (*__end_ptr != '/' && *__end_ptr != '\0')
    --__end_ptr;

  memset(__file_name, 0x00, MAX_NAME);
  memcpy(__file_name, __end_ptr + 1, MAX_NAME);

  memset(__end_ptr + 1, 0x00, __size);

  __parent = vfs_search(__buffer);

  kfree(__buffer);

  if (__parent == NULL || __parent->__type != DIR_TYPE)
    return 1;

  __interator = &__parent->__child;

  while (*__interator != NULL)
    __interator = &(*__interator)->__brother;

  *__interator = (vfs_t *)kmalloc(sizeof(vfs_t));

  if (*__interator == NULL)
    return 1;

  memset(*__interator, 0x00, sizeof(vfs_t));
  memcpy(&(*__interator)->__name, __file_name, MAX_NAME);

  kfree(__file_name);

  (*__interator)->__uid = __uid__;
  (*__interator)->__gid = __gid__;
  (*__interator)->__permission = __perm__;
  (*__interator)->__type = __type__;
  (*__interator)->__child = NULL;
  (*__interator)->__brother = NULL;
  (*__interator)->__parent = __parent;
  (*__interator)->__write = __write__;
  (*__interator)->__read = __read__;
  (*__interator)->__exec = __exec__;

  return 0;
}

/*
 *
 * vfs.h Functions
 *
 */

void vfs_init() {
  __root_dir = (vfs_t *)kmalloc(sizeof(vfs_t));
  __current_dir = __root_dir;

  __root_dir->__name[0] = '/';
  __root_dir->__name[1] = '\0';
  __root_dir->__type = DIR_TYPE;
  __root_dir->__parent = __root_dir;
  __root_dir->__child = NULL;
  __root_dir->__brother = NULL;

  vfs_mkdir("/bin", ROOT_UID, ROOT_GID, READ_O | WRITE_O);
  vfs_mkdir("/dev", ROOT_UID, ROOT_GID, READ_O | WRITE_O);
  vfs_mkdir("/sys", ROOT_UID, ROOT_GID, READ_O | WRITE_O);
}

vfs_t *vfs_exist(const char *__file__) { return vfs_search(__file__); }

__u8 vfs_type(const char *__file__) {
  vfs_t *__vfs = vfs_search(__file__);

  return __vfs != NULL ? __vfs->__type : 0;
}

__u8 vfs_mkdir(const char *__file__, uid_t __uid__, gid_t __gid__,
               mode_t __perm__) {
  if (__file__ == NULL)
    return 1;
  return vfs_create(__file__, __uid__, __gid__, __perm__, DIR_TYPE, NULL, NULL,
                    NULL);
}

__u8 vfs_mkfile(const char *__file__, uid_t __uid__, gid_t __gid__,
                mode_t __perm__, exec_t __exec__) {
  if (__file__ == NULL)
    return 1;

  /*
   *
   * Por enquanto as operaçõoes de arquivo normal estão NULL, mas por padrão
   * esses arquivos vão ser manipulados pela função do sistema de arquivos do
   * kernel
   *
   */

  return vfs_create(__file__, __uid__, __gid__, __perm__, FILE_TYPE, NULL, NULL,
                    __exec__);
}

__u8 vfs_mkchfile(const char *__file__, uid_t __uid__, gid_t __gid__,
                  mode_t __perm__, read_t __read__, write_t __write__) {

  /*
   *
   * Criando um arquivo do tipo char, arquivos do tipo char normalmente recebem
   * e enviam apenas char
   *
   */

  if (__file__ == NULL)
    return 1;
  return vfs_create(__file__, __uid__, __gid__, __perm__, CHAR_TYPE, __read__,
                    __write__, NULL);
}

__u8 vfs_mkbcfile(const char *__file__, uid_t __uid__, gid_t __gid__,
                  mode_t __perm__, read_t __read__, write_t __write__) {

  /*
   *
   * Criando um arquivo do tipo block, arquivos do tipo block normalmente
   * recebem e enviam strings
   *
   */

  if (__file__ == NULL)
    return 1;
  return vfs_create(__file__, __uid__, __gid__, __perm__, BLOCK_TYPE, __read__,
                    __write__, NULL);
}

__u8 vfs_write(const char *__file__, offset_t __offset__, void *__buffer__) {
  vfs_t *__vfs_file = vfs_search(__file__);

  if (__vfs_file == NULL)
    return 1;
  return __vfs_file->__write(__offset__, __buffer__);
}

__u8 vfs_read(const char *__file__, offset_t __offset__, void *__buffer__) {
  vfs_t *__vfs_file = vfs_search(__file__);

  if (__vfs_file == NULL || __vfs_file->__type == DIR_TYPE ||
      __vfs_file->__read == NULL)
    return 1;
  return __vfs_file->__read(__offset__, __buffer__);
}

__u8 vfs_exec(const char *__file__, const char *__param__) {
  vfs_t *__vfs_file = vfs_search(__file__);

  if (__vfs_file == NULL || __vfs_file->__type == DIR_TYPE ||
      __vfs_file->__exec == NULL || !IS_EXEC(__vfs_file))
    return 1;
  return __vfs_file->__exec(__param__);
}
