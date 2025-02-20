/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kmem.c                        |
 *    |  SRC MOD   : 19/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <sys/kmem.h>
#include <std/int.h>
#include <std/utils.h>

#define KBLOCKS_INIT_ADDRS 0x20000000

static kmem_t* __kblocks = (kmem_t*)KBLOCKS_INIT_ADDRS;

/*
 *
 * Funçẽs temporárias
 *
 */

void* kmalloc(kmem_size_t __size__)
{
  void* __block_free = NULL;
  
  for(__u32 __i = 0; __i < BLOCKS; __i++)
  {
    if(!B_IS_ALLOC(((kmem_t*)&__kblocks[__i])))
    {
      __kblocks[__i].__size   = __size__;
      __kblocks[__i].__flags |= B_FLAG_ALLOC;

      __block_free = &__kblocks[__i].__block;
      break;
    }
  }
  return __block_free;
}

void kfree(void* __block__)
{

  if(__block__ == NULL)
    return;

  kmem_t* __ptr1 = &__kblocks[BLOCKS / 2 * 0];
  kmem_t* __ptr2 = &__kblocks[BLOCKS / 2 * 1];

  for(__u32 __i = 0; __i < BLOCKS / 2; __i++)
  {
    if(__ptr1[__i].__block == __block__)
    {
      __ptr1[__i].__size   = 0x00;
      __ptr1[__i].__flags &= 0xFE;
      return;
    }
    
    if(__ptr2[__i].__block == __block__)
    {
      __ptr2[__i].__size   = 0x00;
      __ptr2[__i].__flags &= 0xFE;
      return;
    }
  }
}

