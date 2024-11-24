#include "../drivers/video/vga/vga.h"

void k_main(void)
{
  init_vga();
  clear_screen();
  print_screen("Hello, World! :D ");
  print_screen("Hello, From Kernel! :D");
  while(1);
}
