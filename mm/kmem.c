/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kmem.c                        |
 *    |  SRC MOD   : 31/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#include <std/int.h>
#include <std/io.h>
#include <std/utils.h>
#include <sys/kmem.h>

#define KBLOCKS_INIT_ADDRS 0x20000000

static kmem_t __kblocks[BLOCKS];

static __u32 __allocs = 0x00;
static __u32 __used_bytes = 0x00;

void *kmalloc(kmem_size_t __size__) {
  void *__block = (void *)KBLOCKS_INIT_ADDRS;

  for (__u32 __i = 0; __i < BLOCKS; __i++) {
    if (!B_IS_ALLOC(((kmem_t *)&__kblocks[__i]))) {
      __kblocks[__i].__flags = B_FLAG_ALLOC | B_FLAG_MASTER;
      __kblocks[__i].__size = __size__;
      __kblocks[__i].__block = __block;
      __block = __block + BLOCK_SIZE;

      __u32 __interator = 0x01;

      while (__size__ > BLOCK_SIZE * __interator &&
             !B_IS_ALLOC(((kmem_t *)&__kblocks[__interator + __i].__flags))) {
        if (__interator + __i >= BLOCKS)
          return NULL;

        __kblocks[__interator + __i].__flags = B_FLAG_ALLOC | B_FLAG_SLAVE;
        __kblocks[__interator++ + __i].__size = __size__;
      }
      __allocs += 1;
      __used_bytes += __size__;

      return __block - BLOCK_SIZE;
    }
    __block += BLOCK_SIZE;
  }
  return NULL;
}

void kfree(void *__block__) {
  if (__block__ == NULL)
    return;

  for (__u32 __i = 0; __i < BLOCKS; __i++) {
    if (__kblocks[__i].__block == __block__ && B_IS_ALLOC((&__kblocks[__i]))) {
      __allocs -= 1;
      __used_bytes -= __kblocks[__i].__size;

      __kblocks[__i].__size = 0x00;
      __kblocks[__i].__flags = 0x00;
      __kblocks[__i].__block = 0x00;

      __u32 __interator = 0x01;

      while (B_IS_ALLOC(((kmem_t *)&__kblocks[__interator + __i])) &&
             B_IS_SLAVE(((kmem_t *)&__kblocks[__interator + __i])) &&
             __interator + __i < BLOCKS) {
        __used_bytes -= __kblocks[__interator + __i].__size;

        __kblocks[__interator + __i].__flags = 0x00;
        __kblocks[__interator++ + __i].__size = 0x00;
      }
      return;
    }
  }
}
