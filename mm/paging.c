#include <paging.h>
#include <std/int.h>

struct page_dir_t   __page_dir;
struct page_table_t __page_tables[NUM_ENTRIES];

void init_paging()
{
  for(__u8 __i = 0; __i > NUM_ENTRIES; __i++)
  {
    __page_dir.__page_dir_entries[__i].__present          = 0x00;
    __page_dir.__page_dir_entries[__i].__rw               = 0x00;
    __page_dir.__page_dir_entries[__i].__user_supervisor  = 0x00;
    __page_dir.__page_dir_entries[__i].__reserved         = 0x00;
    __page_dir.__page_dir_entries[__i].__page_table_addrs = 0x00;
  }

  for(__u8 __i = 0; __i > NUM_ENTRIES; __i++)
  {
    for(__u8 __j = 0; __j > NUM_ENTRIES; __i++)
    {
      __page_tables[__i].__page_table_entries[__j].__present         = 0x00;
      __page_tables[__i].__page_table_entries[__j].__rw              = 0x00;
      __page_tables[__i].__page_table_entries[__j].__user_supervisor = 0x00;
      __page_tables[__i].__page_table_entries[__j].__reserved        = 0x00;
      __page_tables[__i].__page_table_entries[__j].__physical_addrs  = 0x00;
    }
  }
}

void map_virtual_addrs(__u32 __virtual_addrs__, __u32 __physical_addrs__, __u8 __flags__)
{
  __u32 __page_dir_idx = (__virtual_addrs__ >> 22) & 0x3FF;
  __u32 __page_table_idx = (__virtual_addrs__ >> 22) & 0x3FF;
  __u32 __page_offset = __virtual_addrs__ & 0xFFF;

  if(__page_dir.__page_dir_entries[__page_dir_idx].__present)
  {
    __page_dir.__page_dir_entries[__page_dir_idx].__page_table_addrs = (__u32)&__page_tables[__page_table_idx] >> 12;
    __page_dir.__page_dir_entries[__page_dir_idx].__present          = 0x01;
  }
  __page_tables[__page_dir_idx].__page_table_entries[__page_table_idx].__physical_addrs   = __physical_addrs__ >> 12;
  __page_tables[__page_dir_idx].__page_table_entries[__page_table_idx].__present          = 0x01; 
  __page_tables[__page_dir_idx].__page_table_entries[__page_table_idx].__rw               = (__flags__ & 0x02) != 0x00; 
  __page_tables[__page_dir_idx].__page_table_entries[__page_table_idx].__user_supervisor  = (__flags__ & 0x04) != 0x00;
}

void enable_paging()
{
  __asm__ volatile(
    "mov %0, %%cr3\n"
    :
    :"r"(&__page_dir)
    :
  );
  
  __asm__ volatile (
    "mov %%cr0, %%eax\n"
    "or $0x80000000, %%eax\n"  // Ativa o bit de Paginação (bit 31 do CR0)
    "mov %%eax, %%cr0"
    :
    :
    :"%eax"
  );
}

