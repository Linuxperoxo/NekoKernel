#include <sys/kernel.h>
#include <idt.h>

void syscall128(struct InterruptRegisters*)
{
  printf("syscall128\n");
}

void syscall177(struct InterruptRegisters*)
{
  printf("syscall177\n");
}

void syscall_init()
{
  idt_install_coop_routine(0x80, &syscall128);
  idt_install_coop_routine(0xB1, &syscall177);
}

