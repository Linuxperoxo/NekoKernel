/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : io.c                          |
 *    |  SRC MOD   : 20/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <std/int.h>
#include <std/utils.h>
#include <sys/vfs.h>
#include <std/io.h>

void printf(const char* __text__, ...)
{
  void* __args = (void*)((__u32)&__text__ + sizeof(char*)); // `&__text__ + 1` move para o primeiro parâmetro real
  char __buffer[16];
  __u8 __i = 0x00;

  while(*__text__ != '\0')
  {
    if(*__text__ == '%')
    {
      switch(*(++__text__))
      {
        case 'd':
          __i = 0x00;

          sprintf(*((__u32*)__args), "%d", (char*)&__buffer);

          while(__buffer[__i] != '\0')
          {
            vfs_write("/dev/tty", 0x00, &__buffer[__i++]);
          }
          __args = (void*)((__u32)__args + 4); // O compilador empilha 4 bytes na pilha, independente do tipo
        break;

        case 'x':
          __i = 0x00;

          sprintf(*((__u32*)__args), "%x", (char*)&__buffer);

          while(__buffer[__i] != '\0')
          {
            vfs_write("/dev/tty", 0x00, &__buffer[__i++]);
          }
          __args = (void*)((__u32)__args + 4); // O compilador empilha 4 bytes na pilha, independente do tipo
        break;

        case 's':
          char* __tmp = *((char**)__args);

          while(*__tmp != '\0')
          {
            vfs_write("/dev/tty", 0x00, __tmp);
            ++__tmp;
          }

          __args = (void*)((__u32)__args + 4); // O compilador empilha 4 bytes na pilha, independente do tipo
        break;

        case 'c':
          vfs_write("/dev/tty", 0x00, (char*)__args);
          __args = (void*)((__u32)__args + 4); // O compilador empilha 4 bytes na pilha, independente do tipo
        break;
      }
    }
    else
    {
      vfs_write("/dev/tty", 0x00, __text__);
    }
    ++__text__;
  }
}

void scanf(char* __dest__, __u16 __size__)
{
  vfs_read("/dev/tty", __size__, __dest__);
}
