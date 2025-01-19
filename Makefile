#
#
#
#    /--------------------------------------------O
#    |                                            |
#    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
#    |  AUTHOR    : Linuxperoxo                   |
#    |  FILE      : Makefile                      |
#    |  SRC MOD   : 19/01/2025                    |
#    |                                            |
#    O--------------------------------------------/
#
#
#

# ----------
# Flags
# ----------
CFLAGS         = -Wall -Wextra -g -ffreestanding -v -nostdlib -nostartfiles -fno-stack-protector -fno-builtin -m32 -O0 $(INCLUDES)
ASMFLAGSBIN    = -f bin
ASMFLAGSELF    = -g -f elf32
LDFLAGS        = -z noexecstack -nostdlib -m elf_i386 -T $(LINKER_FILE)
QEMUFLAGS      = -drive file=$(NEKO_OS_IMG),format=raw -m 4G
QEMUFLAGSDEBUG = -drive file=$(DEBUG_FILE),format=raw -m 4G -s -S
INCLUDES       = -I $(INCLUDE) -I $(KERNEL_DRIVERS)  

# ----------
# Kernel Files
# ----------
KERNEL_SRC        = ./init/kernel.c
KERNEL_OBJ        = $(OBJ_DIR)/kernel.o
KERNEL_BIN        = $(BIN_DIR)/kernel.bin
KERNEL_LOADER_SRC = $(ENTRY_DIR)/loader.s
KERNEL_LOADER_OBJ = $(OBJ_BOOT_DIR)/loader.o
TTY_SRC           = $(SYS_DIR)/tty.c
TTY_OBJ           = $(OBJ_DIR)/tty.o
SHELL_SRC         = $(SH_DIR)/shell.c
SHELL_OBJ         = $(OBJ_DIR)/shell.o
GDT_SRC           = $(CPU_DIR)/gdt.c
GDT_OBJ           = $(OBJ_DIR)/gdt.o
IDT_SRC           = $(CPU_DIR)/idt.c
IDT_OBJ           = $(OBJ_DIR)/idt.o
TASK_SRC          = $(SYS_DIR)/task.c
TASK_OBJ          = $(OBJ_DIR)/task.o
ISR_SRC           = $(CPU_DIR)/isr.s
ISR_OBJ           = $(OBJ_DIR)/isr.o
SYSCALL_SRC       = $(SYS_DIR)/syscall.c
SYSCALL_OBJ       = $(OBJ_DIR)/syscall.o
TIMER_SRC         = $(SYS_DIR)/timer.c
TIMER_OBJ         = $(OBJ_DIR)/timer.o
VFS_SRC           = $(FS_DIR)/vfs.c
VFS_OBJ           = $(OBJ_DIR)/vfs.o
PAGING_SRC        = $(MM_DIR)/paging.c
PAGING_OBJ        = $(OBJ_DIR)/paging.o
BOOTLOADER_SRC    = $(BOOT_DIR)/nekonest.s
BOOTLOADER_BIN    = $(BIN_DIR)/nekonest.bin
NEKO_OS_IMG       = $(IMG_DIR)/NekoOS.img
DEBUG_FILE        = $(BIN_DIR)/debug.bin
LINKER_FILE       = linker.ld

# ----------
# STDLIB FILES
# ----------
IO_SRC    = $(STD_SRC_DIR)/io.c
IO_OBJ    = $(OBJ_STD_DIR)/io.o
UTILS_SRC = $(STD_SRC_DIR)/utils.c 
UTILS_OBJ = $(OBJ_STD_DIR)/utils.o
STR_SRC   = $(STD_SRC_DIR)/str.c
STR_OBJ   = $(OBJ_STD_DIR)/str.o

# ----------
# SYSLIB FILES
# ----------
KMEM_SRC = $(SYS_SRC_DIR)/kmem.c
KMEM_OBJ = $(OBJ_SYS_DIR)/kmem.o

# ----------
# Drivers
# ----------
KEYBOARD_DRIVER_SRC = $(KERNEL_DRIVERS)/device/io/keyboard/keyboard.c
KEYBOARD_DRIVER_OBJ = $(OBJ_DRIVER_DIR)/keyboard.o
ATA_DRIVER_SRC      = $(KERNEL_DRIVERS)/media/ata.c
ATA_DRIVER_OBJ      = $(OBJ_DRIVER_DIR)/ata.o
VGA_DRIVER_SRC      = $(KERNEL_DRIVERS)/video/vga/vga.c
VGA_DRIVER_OBJ      = $(OBJ_DRIVER_DIR)/vga.o

# ----------
# Dirs
# ----------
SYS_SRC_DIR    = ./lib/sys
STD_SRC_DIR    = ./lib/std
SYS_DIR        = ./sys
MM_DIR         = ./mm
CPU_DIR        = ./cpu
INIT_DIR       = ./init
SH_DIR         = ./sh
ENTRY_DIR      = ./entry
FS_DIR         = ./fs
BUILD_DIR      = build
BIN_DIR        = $(BUILD_DIR)/bin
OBJ_DIR        = $(BUILD_DIR)/obj
OBJ_DRIVER_DIR = $(OBJ_DIR)/drivers
OBJ_BOOT_DIR   = $(OBJ_DIR)/boot
OBJ_STD_DIR    = $(OBJ_DIR)/stdlib
OBJ_SYS_DIR    = $(OBJ_DIR)/syslib
IMG_DIR        = $(BUILD_DIR)/img
BOOT_DIR       = ./boot
KERNEL_DRIVERS = ./drivers
INCLUDE        = ./include

# ----------
# Exec
# ----------
LD    = /usr/bin/ld
ASM   = /usr/bin/nasm
CC    = /usr/bin/gcc
QEMU  = /usr/bin/qemu-system-i386
STRIP = /usr/bin/strip

# ==============================================

# === BUILD_DIR

$(BUILD_DIR):
	mkdir -p $(BIN_DIR) $(OBJ_DIR) $(IMG_DIR) $(OBJ_STD_DIR) $(OBJ_SYS_DIR) $(OBJ_DRIVER_DIR) $(OBJ_BOOT_DIR)

# === KERNEL_BIN

$(KERNEL_BIN): $(KERNEL_LOADER_OBJ) $(KERNEL_OBJ) $(IO_OBJ) $(UTILS_OBJ) $(STR_OBJ) $(KMEM_OBJ) $(VGA_DRIVER_OBJ) $(KEYBOARD_DRIVER_OBJ) $(ATA_DRIVER_OBJ) $(GDT_OBJ) $(IDT_OBJ) $(ISR_OBJ) $(TIMER_OBJ) $(TTY_OBJ) $(SHELL_OBJ) $(SYSCALL_OBJ) $(TASK_OBJ) $(VFS_OBJ)
	$(LD) $(LDFLAGS) $^ -o $@
$(VFS_OBJ):
	$(CC) $(CFLAGS) $(VFS_SRC) -c -o $@
$(PAGING_OBJ):
	$(CC) $(CFLAGS) $(PAGING_SRC) -c -o $@
$(TASK_OBJ):
	$(CC) $(CFLAGS) $(TASK_SRC) -c -o $@
$(SYSCALL_OBJ):
	$(CC) $(CFLAGS) $(SYSCALL_SRC) -c -o $@
$(SHELL_OBJ):
	$(CC) $(CFLAGS) $(SHELL_SRC) -c -o $@
$(TTY_OBJ):
	$(CC) $(CFLAGS) $(TTY_SRC) -c -o $@
$(TIMER_OBJ):
	$(CC) $(CFLAGS) $(TIMER_SRC) -c -o $@
$(ISR_OBJ):
	$(ASM) $(ASMFLAGSELF) $(ISR_SRC) -o $@
$(IDT_OBJ):
	$(CC) $(CFLAGS) $(IDT_SRC) -c -o $@
$(GDT_OBJ):
	$(CC) $(CFLAGS) -c -o $@ $(GDT_SRC) 
$(ATA_DRIVER_OBJ): 
	$(CC) $(CFLAGS) $(ATA_DRIVER_SRC) -c -o $@
$(KEYBOARD_DRIVER_OBJ):
	$(CC) $(CFLAGS) $(KEYBOARD_DRIVER_SRC) -c -o $@
$(VGA_DRIVER_OBJ):
	$(CC) $(CFLAGS) $(VGA_DRIVER_SRC) -c -o $@
$(KERNEL_OBJ):
	$(CC) $(CFLAGS) $(KERNEL_SRC) -c -o $@
$(KERNEL_LOADER_OBJ):
	$(ASM) $(ASMFLAGSELF) $(KERNEL_LOADER_SRC) -o $@

# --------
# STDLIB
# --------
$(IO_OBJ):
	$(CC) $(CFLAGS) $(IO_SRC) -c -o $@
$(UTILS_OBJ):
	$(CC) $(CFLAGS) $(UTILS_SRC) -c -o $@
$(STR_OBJ):
	$(CC) $(CFLAGS) $(STR_SRC) -c -o $@

# --------
# SYSLIB
# --------
$(KMEM_OBJ):
	$(CC) $(CFLAGS) $(KMEM_SRC) -c -o $@
# ==============================================

kernel: $(BUILD_DIR) $(KERNEL_BIN)

bootloader: $(BUILD_DIR)
	$(ASM) $(ASMFLAGSBIN) $(BOOTLOADER_SRC) -o $(BOOTLOADER_BIN)

image: kernel bootloader
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $(NEKO_OS_IMG)  

strip: kernel
	$(STRIP) $(KERNEL_BIN)

clean:
	rm -rf $(BUILD_DIR)

run: strip image
	$(QEMU) $(QEMUFLAGS)

gdb:
	gdb $(DEBUG_FILE)

debug: clean image
	cp -v $(KERNEL_BIN) $(DEBUG_FILE)
	strip $(KERNEL_BIN)
	$(QEMU) $(QEMUFLAGS) -s -S
