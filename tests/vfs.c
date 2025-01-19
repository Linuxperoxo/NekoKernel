#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_NAME 256

#define DIR_TYPE   0x01
#define FILE_TYPE  0x02
#define BLOCK_TYPE 0x04
#define CHAR_TYPE  0x08

#define ROOT_UID 0x00
#define ROOT_GID 0x00

#define READ_O  0x03
#define WRITE_O 0x02
#define EXEC_O  0x01

#define IS_DIR(__vfs) (__vfs->__type & DIR_TYPE)
#define IS_FILE(__vfs) ((__vfs->__type & FILE_TYPE) >> 1)
#define IS_BLOCK(__vfs) ((__vfs->__type & BLOCK_TYPE) >> 2)
#define IS_CHAR(__vfs) ((__vfs->__type & CHAR_TYPE) >> 3)

typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;

typedef __u8  uuid_t;
typedef __u8  ggid_t;
typedef __u8  type_t;
typedef __u8  mmode_t;
typedef __u32 offset_t;
typedef __u32 uinode_t;
typedef __u32 size_tt;
typedef __u8  opened_t;

typedef __u8 (*write_t)(offset_t, void*);
typedef __u8 (*read_t)(offset_t, void*);
typedef void (*exec_t)(const char*);

typedef struct vfs_tt vfs_tt;
typedef vfs_tt* child_t;
typedef vfs_tt* brother_t;
typedef vfs_tt* parent_t;

typedef struct vfs_tt
{
  char      __name[MAX_NAME + 1];
  uuid_t     __uid;
  ggid_t     __gid;
  mmode_t    __permission;
  type_t    __type;
  child_t   __child;
  brother_t __brother;
  parent_t  __parent;
  
  /*
   *
   * Operações para o arquivo
   *
   */

  write_t __write;
  read_t  __read;
  exec_t  __exec;
}vfs_tt;

#define ROOT_DIR    __root_dir
#define CURRENT_DIR __current_dir

typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;

vfs_tt* __root_dir    = NULL;
vfs_tt* __current_dir = NULL; 

void memset(void* __src__, __u8 __value__, __u32 __size__)
{
  while(__size__--)
  {
    *((__u8*)__src__) = __value__;    
    ++__src__;
  }
}

void memcpy(void* __dest__, void* __src__, __u32 __size__)
{
  while(__size__--)
  {
    *((__u8*)__dest__) = *((__u8*)__src__);
    __dest__++;
    __src__++;
  }
}

__u32 strlen(const char* __src__)
{
  __u32 __str_size = 0x00;

  while(*__src__ != '\0')
  {
    ++__str_size;
    ++__src__;
  }
  return __str_size;
}

__u8 strcmp(const char* __restrict __str1__, const char* __restrict __str2__)
{
  if(*__str1__ == '\0' || *__str2__ == '\0')
    return 2;

  while(*__str1__ != '\0' && *__str2__ != '\0')
  {
    if(*__str1__ != *__str2__)
      return 1;

    ++__str1__;
    ++__str2__;
  }
  
  if(*__str1__ == '\0' && *__str2__ != '\0' ||
     *__str1__ != '\0' && *__str2__ == '\0')
     return 1; 
  return 0;
}

void strcpy(char* __restrict __dest__, const char* __restrict __src__)
{
  while(*__src__ != '\0')
  {
    *__dest__ = *__src__;
    
    ++__dest__;
    ++__src__;
  }
}

__attribute__((always_inline)) 
inline uint8_t vfs_way_cmp(const char* __restrict__ __vfs_name__, const char** __restrict__ __dir_cmp__)
{
  if(*__vfs_name__ == '\0' || **__dir_cmp__ == '\0')
    return 1;

  while(*__vfs_name__ != '\0' && **__dir_cmp__ != '\0')
  {
    if(*__vfs_name__ != **__dir_cmp__)
      return 1;

    ++__vfs_name__;
    ++(*__dir_cmp__);
  }

  if(*__vfs_name__ == '\0')
  {
    switch(**__dir_cmp__) 
    {
      case '/':
        while(**__dir_cmp__ == '/')
          ++(*__dir_cmp__);
        
        if(**__dir_cmp__ == '\0')
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

__attribute__((always_inline))
inline vfs_tt* vfs_search(const char* __file__)
{
  vfs_tt* __tmp = __current_dir;

  while(*__file__ == '/')
  {
    __tmp = __root_dir;
    __file__++;
  }

  if(*__file__ == '\0')
    return __tmp;

  __tmp = __tmp->__child == NULL ? NULL : __tmp->__child;

  while(__tmp != NULL)
  {
    const char* __bc_file = __file__;
    __u8 __return = vfs_way_cmp((char*)&__tmp->__name, &__file__);

    switch(__return)
    {
      case 2:
        __bc_file = __file__;
        __tmp     = __tmp->__type == DIR_TYPE ? __tmp->__child : NULL;
      break;

      case 1:
        __file__ = __bc_file;
        __tmp    = __tmp->__brother;
      break;

      case 0:
        goto EXIT;
      break;
    }
  }
  EXIT:
  return __tmp;
}

__attribute__((always_inline))
inline __u8 vfs_create(const char* __file__, uid_t __uid__, 
                gid_t __gid__, mode_t __perm__, type_t __type__, write_t __write__, 
                read_t __read__, exec_t __exec__)
{
  if(vfs_search(__file__) != NULL)
    return 1;

  __u32 __size = strlen(__file__);  
  char* __buffer  = (char*)malloc(sizeof(char) * strlen(__file__) + 1);
  char* __file_name = (char*)malloc(sizeof(char*) * MAX_NAME); 
  char* __end_ptr = __buffer + __size - 1;

  strcpy(__buffer, __file__);

  while(*__end_ptr == '/')
  {
    *__end_ptr = '\0';
    --__end_ptr;
  }

  while(*__end_ptr != '/' && *__end_ptr != '\0')
    --__end_ptr;

  memcpy(__file_name, __end_ptr + 1, MAX_NAME);
  memset(__end_ptr + 1, 0x00, __size);

  vfs_tt* __parent = vfs_search(__buffer);

  if(__parent == NULL || __parent->__type != DIR_TYPE)
    return 1;

  vfs_tt** __interator = &__parent->__child;

  while(*__interator != NULL)
    __interator = &(*__interator)->__brother;

  *__interator = (vfs_tt*)malloc(sizeof(vfs_tt));

  if(*__interator == NULL)
    return 1;

  memset(*__interator, 0x00, sizeof(vfs_tt));
  memcpy(&(*__interator)->__name, __file_name, MAX_NAME);

  (*__interator)->__uid        = __uid__;
  (*__interator)->__gid        = __gid__;
  (*__interator)->__permission = __perm__;
  (*__interator)->__type       = __type__;
  (*__interator)->__child      = NULL;
  (*__interator)->__brother    = NULL;
  (*__interator)->__parent     = __parent;
  (*__interator)->__write      = __write__;
  (*__interator)->__read       = __read__;
  (*__interator)->__exec       = __exec__;

  return 0;
}

__u8 vfs_mkdir(const char* __file__, uid_t __uid__, gid_t __gid__, mode_t __perm__)
{
  if(__file__ == NULL)
    return 1;
  return vfs_create(__file__, __uid__, __gid__, __perm__, DIR_TYPE, NULL, NULL, NULL);
}

__u8 vfs_mkfile(const char* __file__, mmode_t __perm__)
{
  if(__file__ == NULL)
    return 1;
  return vfs_create(__file__, ROOT_UID, ROOT_UID, __perm__, FILE_TYPE, NULL, NULL, NULL);
}

int main(void)
{
  ROOT_DIR = (vfs_tt*)malloc(sizeof(vfs_tt));
  CURRENT_DIR = ROOT_DIR;
  
  ROOT_DIR->__name[0] = '/';
  ROOT_DIR->__name[1] = '\0';
  ROOT_DIR->__type    = DIR_TYPE;
  ROOT_DIR->__parent  = ROOT_DIR;
  ROOT_DIR->__brother = NULL;
  ROOT_DIR->__child   = NULL;

  vfs_tt* __tmp = NULL;
  
  if(vfs_mkdir("/bin/", ROOT_UID, ROOT_GID, 0x00)== 0)
  {
    printf("Create bin/\n");
  }

  if(vfs_mkdir("/dev/", ROOT_UID, ROOT_GID, 0x00) == 0)
  {
    printf("Create dev/\n");
  }

  if(vfs_mkfile("/dev/tty", ROOT_UID) == 0)
  {
    printf("Create /dev/tty\n");
  }
 
  __tmp = vfs_search("/bin/");

  if(__tmp)
  {
    printf("Found %s\n", __tmp->__name);
  }

  __tmp = vfs_search("/dev/");
  
  if(__tmp)
  {
    printf("Found %s\n", __tmp->__name);
  }

  __tmp = vfs_search("/dev/tty");
  
  if(__tmp)
  {
    printf("Found %s\n", __tmp->__name);
  }
  return 0;
}

