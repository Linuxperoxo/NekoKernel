#include <sys/kernel.h>

void syscall_handler(struct InterruptRegisters*)
{
  printf("syscall\n");
}
