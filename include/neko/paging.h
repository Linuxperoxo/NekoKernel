#ifndef __K_PAGING__
#define __K_PAGING__

#include <std/int.h>

#define PAGE_SIZE   4096
#define NUM_ENTRIES 1024

struct page_table_entry_t
{
  __u8  __present         : 1;
  __u8  __rw              : 1;
  __u8  __user_supervisor : 1;
  __u8  __reserved        : 2;
  __u32 __physical_addrs  : 28; 
};

struct page_dir_entry_t
{
  __u8  __present          : 1;
  __u8  __rw               : 1;
  __u8  __user_supervisor  : 1;
  __u8  __reserved         : 2;
  __u32 __page_table_addrs : 28;
};

struct page_table_t
{
  struct page_table_entry_t __page_table_entries[NUM_ENTRIES]; 
};

struct page_dir_t
{
  struct page_dir_entry_t __page_dir_entries[NUM_ENTRIES];
};

extern void init_paging();
extern void map_virtual_addrs(__u32 __virtual_addrs__, __u32 __physical_addrs__, __u8 __flags__);
extern void enable_paging();

#endif
