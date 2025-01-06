#include <sys/kernel.h>

void syscall_handler(struct InterruptRegister*)
{
  printf("syscall\n");
}
