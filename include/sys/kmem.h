#ifndef __SYS_KMEM__
#define __SYS_KMEM__

#include <std/int.h>

#define BLOCK_SIZE 4096
#define BLOCKS 16384

#define B_FLAG_ALLOC 0x01
#define B_FLAG_DIV_B 0x02

#define B_IS_FREE(__kmem_t) (__kmem_t->__flags & 0x01)
#define B_IS_DIV(__kmem_t) ((__kmem_t->__flags & 0x02) >> 1)

typedef __u32 kmem_size_t; 
typedef __u8* kmem_block_t;
typedef __u8 kmem_flags_t;

typedef struct kmem_t
{
  kmem_size_t  __size;
  kmem_flags_t __flags;
  kmem_block_t __block[BLOCK_SIZE];
}__attribute__((packed))kmem_t;

extern void* kmalloc(kmem_size_t __size__);
extern void kfree(void* __block__);

#endif
