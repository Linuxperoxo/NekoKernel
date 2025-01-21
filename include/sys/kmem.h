/*
 *
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : kmem.h                        |
 *    |  SRC MOD   : 20/01/2025                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 *
 */

#ifndef __SYS_KMEM__
#define __SYS_KMEM__

#include <std/int.h>

#define ALIGNED(__aligned) __attribute__((aligned(__aligned)))

#define BLOCK_SIZE 4096
#define BLOCKS 524288

#define B_FLAG_ALLOC 0x01

#define B_IS_ALLOC(__kmem_t) (__kmem_t->__flags & 0x01)

typedef __u32 kmem_size_t; 
typedef __u8* kmem_block_t;
typedef __u8 kmem_flags_t;

typedef struct kmem_t
{
  kmem_size_t  __size;
  kmem_flags_t __flags;
  ALIGNED(BLOCK_SIZE) kmem_block_t __block[BLOCK_SIZE];
}kmem_t;

void* kmalloc(kmem_size_t __size__);
void kfree(void* __block__);

#endif
