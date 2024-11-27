#include <utils.h>
#include <video/vga/vga.h>
#include <IO/ports.h>
#include <idt.h>

#define IDT_ENTRIES 256

#define PRIMARY_PIC_CHIP_COMMANDS 0x20
#define PRIMARY_PIC_CHIP_DATA 0x21
#define SECUNDARY_PIC_CHIP_COMMANDS 0xA0
#define SECUNDARY_PIC_CHIP_DATA 0xA1
#define INIT_PIC_COMMAND 0x11

struct idt_entry_struct __idt_entries[IDT_ENTRIES];
struct idt_ptr_struct __idt_entry_ptr;

extern void idt_flush(__u32);

void initidt()
{
  __idt_entry_ptr.__base = (__u32) &(__idt_entries);
  __idt_entry_ptr.__limit = sizeof(struct idt_entry_struct) * IDT_ENTRIES;

  memset(&__idt_entries, 0, sizeof(struct idt_entry_struct) * IDT_ENTRIES);

  outb(PRIMARY_PIC_CHIP_COMMANDS, INIT_PIC_COMMAND);
  outb(SECUNDARY_PIC_CHIP_COMMANDS, INIT_PIC_COMMAND);

  outb(PRIMARY_PIC_CHIP_DATA, 0x20);
  outb(SECUNDARY_PIC_CHIP_DATA, 0x28);

  outb(PRIMARY_PIC_CHIP_DATA, 0x04);
  outb(SECUNDARY_PIC_CHIP_DATA, 0x02);

  outb(PRIMARY_PIC_CHIP_DATA, 0x01);
  outb(SECUNDARY_PIC_CHIP_DATA, 0x01);
  
  outb(PRIMARY_PIC_CHIP_DATA, 0x00);
  outb(SECUNDARY_PIC_CHIP_DATA, 0x00);

  setidgate(0, (__u32)isr0, 0x08, 0x8E);
  setidgate(1, (__u32)isr1, 0x08, 0x8E);
  setidgate(2, (__u32)isr2, 0x08, 0x8E);
  setidgate(3, (__u32)isr3, 0x08, 0x8E);
  setidgate(4, (__u32)isr4, 0x08, 0x8E);
  setidgate(5, (__u32)isr5, 0x08, 0x8E);
  setidgate(6, (__u32)isr6, 0x08, 0x8E);
  setidgate(7, (__u32)isr7, 0x08, 0x8E);
  setidgate(8, (__u32)isr8, 0x08, 0x8E);
  setidgate(9, (__u32)isr9, 0x08, 0x8E);
  setidgate(10, (__u32)isr10, 0x08, 0x8E);
  setidgate(11, (__u32)isr11, 0x08, 0x8E);
  setidgate(12, (__u32)isr12, 0x08, 0x8E);
  setidgate(13, (__u32)isr13, 0x08, 0x8E);
  setidgate(14, (__u32)isr14, 0x08, 0x8E);
  setidgate(15, (__u32)isr15, 0x08, 0x8E);
  setidgate(16, (__u32)isr16, 0x08, 0x8E);
  setidgate(17, (__u32)isr17, 0x08, 0x8E);
  setidgate(18, (__u32)isr18, 0x08, 0x8E);
  setidgate(19, (__u32)isr19, 0x08, 0x8E);
  setidgate(20, (__u32)isr20, 0x08, 0x8E);
  setidgate(21, (__u32)isr21, 0x08, 0x8E);
  setidgate(22, (__u32)isr22, 0x08, 0x8E);
  setidgate(23, (__u32)isr23, 0x08, 0x8E);
  setidgate(24, (__u32)isr24, 0x08, 0x8E);
  setidgate(25, (__u32)isr25, 0x08, 0x8E);
  setidgate(26, (__u32)isr26, 0x08, 0x8E);
  setidgate(27, (__u32)isr27, 0x08, 0x8E);
  setidgate(28, (__u32)isr28, 0x08, 0x8E);
  setidgate(29, (__u32)isr29, 0x08, 0x8E);
  setidgate(30, (__u32)isr30, 0x08, 0x8E);
  setidgate(31, (__u32)isr31, 0x08, 0x8E);
  
  /*
   *
   * Solicitações de interrupção
   *
   */

  setidgate(32, (__u32)irq0, 0x08, 0x8E);
  setidgate(33, (__u32)irq1, 0x08, 0x8E);
  setidgate(34, (__u32)irq2, 0x08, 0x8E);
  setidgate(35, (__u32)irq3, 0x08, 0x8E);
  setidgate(36, (__u32)irq4, 0x08, 0x8E);
  setidgate(37, (__u32)irq5, 0x08, 0x8E);
  setidgate(38, (__u32)irq6, 0x08, 0x8E);
  setidgate(39, (__u32)irq7, 0x08, 0x8E);
  setidgate(40, (__u32)irq8, 0x08, 0x8E);
  setidgate(41, (__u32)irq9, 0x08, 0x8E);
  setidgate(42, (__u32)irq10, 0x08, 0x8E);
  setidgate(43, (__u32)irq11, 0x08, 0x8E);
  setidgate(44, (__u32)irq12, 0x08, 0x8E);
  setidgate(45, (__u32)irq13, 0x08, 0x8E);
  setidgate(46, (__u32)irq14, 0x08, 0x8E);
  setidgate(47, (__u32)irq15, 0x08, 0x8E);


  /*
   *
   * Para syscall
   * 
   */

  setidgate(128, (__u32)isr128, 0x08, 0x8E);
  setidgate(177, (__u32)isr177, 0x08, 0x8E);

  idt_flush((__u32)&__idt_entry_ptr);
}

void setidgate(__u8 __num__, __u32 __base__, __u16 __sel__, __u8 __flags__)
{
  __idt_entries[__num__].__base_low = __base__ & 0xFFFF;
  __idt_entries[__num__].__base_high = (__base__ >> 16) & 0xFFFF;
  __idt_entries[__num__].__sel = __sel__;
  __idt_entries[__num__].__always0 = 0x00;
  __idt_entries[__num__].__flags = __flags__ | 0x60;
}

__u8* __exception_messages[] = {
  "Division By Zero\n",
  "Debus\n",
  "Non Maskable Interrupt\n",
  "Breakpoint\n",
  "Into Detected Overflow\n"
  "Out of Bounds\n",
  "Invalid Opcode\n",
  "No Coprocessor\n",
  "Double fault\n",
  "Coprocessor Segment Overun\n",
  "Bad TSS\n",
  "Segment not present\n",
  "Stack fault\n",
  "General protection fault\n",
  "Page fault\n",
  "Unknown Interrupt\n",
  "Coprocessor Fault\n",
  "Aligment Fault\n",
  "Machine Check\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
  "Reserved\n",
};

void isr_handler(struct InterruptRegisters* __regs__)
{
  if(__regs__->__int_no < 32)
  {
    print_screen(__exception_messages[__regs__->__int_no]);
    print_screen("Exception! Systam Halted\n");
  }
}

void* __irq_routines[16] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_handler(__i32 __irq__, void (*handler)(struct InterruptRegisters *__regs__))
{
  __irq_routines[__irq__] = handler;
}

void irq_unistall_handler(__u32 __irq__)
{
  __irq_routines[__irq__] = 0x00; 
}

void irq_handler(struct InterruptRegisters* __regs__)
{
  void (*handler)(struct InterruptRegisters* __regs__);

  handler = __irq_routines[__regs__->__int_no - 32];

  if(handler)
  {
    handler(__regs__);
  }

  if(__regs__->__int_no >= 40)
  {
    outb(0xA0, 0x20);
  }
  outb(0x20, 0x20);
}
