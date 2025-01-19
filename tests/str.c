#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define DIR_TYPE 0x01
#define IS_DIR(__vfs) (__vfs->__type & DIR_TYPE)
#define MAX_NAME 256

#define ROOT_DIR    __root
#define CURRENT_DIR __current

typedef uint8_t __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;
typedef struct vfs_t vfs_t;

typedef struct vfs_t
{
  uint8_t __name[256];
  uint8_t __type;
  vfs_t*  __child;
  vfs_t*  __brother;
}vfs_t;

static vfs_t* __root    = NULL;
static vfs_t* __current = NULL; 

__attribute__((always_inline)) inline uint8_t vfs_way_cmp(const char* __restrict__ __vfs_name__, const char** __restrict__ __dir_cmp__)
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

vfs_t* vfs_search(const char* __file__)
{
  vfs_t* __tmp = CURRENT_DIR;

  while(*__file__ == '/')
  {
    __tmp = ROOT_DIR;
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

__u8 vfs_create(const char* __file__, vfs_t* __vfs_file__)
{
  if(__vfs_file__ == NULL || vfs_search(__file__) != NULL)
    return 1;

  __u32 __str_size = strlen(__file__);  
  char* __buffer  = (char*)malloc(sizeof(char) * strlen(__file__) + 1);
  char* __end_ptr = __buffer + __str_size - 1;

  memcpy(__buffer, __file__, __str_size);

  while(*__end_ptr == '/')
  {
    *__end_ptr = '\0';
    --__end_ptr;
  }

  while(*__end_ptr != '/' && *__end_ptr != '\0')
  {
    *__end_ptr = '\0';
    --__end_ptr;
  }

  vfs_t* __parent = vfs_search(__buffer);

  if(__parent == NULL || __parent->__type != DIR_TYPE)
    return 1;

  vfs_t** __next_brother = &__parent->__child;

  while(*__next_brother != NULL)
    __next_brother = &(*__next_brother)->__brother;

  *__next_brother = (vfs_t*)malloc(sizeof(vfs_t));

  if(*__next_brother == NULL)
    return 1;

  memcpy(&(*__next_brother)->__name, &__vfs_file__->__name, MAX_NAME);

  (*__next_brother)->__type    = __vfs_file__->__type;
  (*__next_brother)->__child   = NULL;
  (*__next_brother)->__brother = NULL;

  return 0;
}

int main(void)
{
  ROOT_DIR = (vfs_t*)malloc(sizeof(vfs_t));

  CURRENT_DIR = ROOT_DIR;
  
  ROOT_DIR->__name[0] = '/';
  ROOT_DIR->__name[1] = '\0';
  ROOT_DIR->__type    = DIR_TYPE;
  ROOT_DIR->__brother = NULL;
  ROOT_DIR->__child   = NULL;


  const char* __file = "/dev/";

  vfs_t __dev;

  __dev.__name[0] = 'd';
  __dev.__name[1] = 'e';
  __dev.__name[2] = 'v';
  __dev.__name[3] = '\0';
  __dev.__type    = DIR_TYPE;

  vfs_t __bin;
  
  __bin.__name[0] = 'b';
  __bin.__name[1] = 'i';
  __bin.__name[2] = 'n';
  __bin.__name[3] = '\0';
  __bin.__type    = DIR_TYPE;

  vfs_t __cd;
  
  __cd.__name[0] = 'c';
  __cd.__name[1] = 'd';
  __cd.__name[2] = '\0';
  __cd.__type    = 0x04;
  
  if(vfs_create("bin/", &__bin) == 0)
  {
    printf("Create /bin/\n");
  }
  
  if(vfs_create("/bin/cd", &__cd) == 0)
  {
    printf("Create /bin/cd\n");
  }

  if(vfs_create("/dev/", &__dev) == 0)
  {
    printf("Create /dev/\n");
  }
 
  if(vfs_search("/bin/") != NULL)
  {
    printf("Found /bin/\n");
  }
  
  if(vfs_search("/bin/cd") != NULL)
  {
    printf("Found /bin/cd\n");
  }
  
  if(vfs_search("/dev/") != NULL)
  {
    printf("Found /dev/\n");
  }
  return 0;
}

