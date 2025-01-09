#
#
#
#    /--------------------------------------------O
#    |                                            |
#    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
#    |  AUTHOR    : Linuxperoxo                   |
#    |  FILE      : Makefile                      |
#    |  SRC MOD   : 08/01/2025                    |
#    |                                            |
#    O--------------------------------------------/
#
#
#

# Flags
CFLAGS = -Wall -Wextra -g -ffreestanding -v -nostdlib -nostartfiles -fno-stack-protector -fno-builtin -m32 -O0 $(INCLUDES)
ASMFLAGSBIN = -f bin
ASMFLAGSELF = -g -f elf32
LDFLAGS = -z noexecstack -nostdlib -m elf_i386 -T $(LINKER_FILE)
QEMUFLAGS = -drive file=$(NEKO_OS_IMG),format=raw -m 4G
QEMUFLAGSDEBUG = $(QEMUFLAGS) -s -S
INCLUDES = -I $(STD_INCLUDE) -I $(KERNEL_INCLUDE) -I $(KERNEL_DRIVERS)  

# Kernel Files
KERNEL_SRC = $(KERNEL_SRC_DIR)/kernel.c
KERNEL_OBJ = $(OBJ_DIR)/kernel.o
KERNEL_BIN = $(BIN_DIR)/kernel

KERNEL_LOADER_SRC = $(BOOT_DIR)/loader.s
KERNEL_LOADER_OBJ = $(OBJ_BOOT_DIR)/loader.o

TERMINAL_SRC = $(KERNEL_SRC_DIR)/terminal.c
TERMINAL_OBJ = $(OBJ_DIR)/terminal.o

SHELL_SRC = $(KERNEL_SRC_DIR)/shell.c
SHELL_OBJ = $(OBJ_DIR)/shell.o

GDT_SRC = $(KERNEL_SRC_DIR)/gdt.c
GDT_OBJ = $(OBJ_DIR)/gdt.o

IDT_SRC = $(KERNEL_SRC_DIR)/idt.c
IDT_OBJ = $(OBJ_DIR)/idt.o

ISR_SRC = $(KERNEL_SRC_DIR)/isr.s
ISR_OBJ = $(OBJ_DIR)/isr.o

SYSCALL_SRC = $(KERNEL_SRC_DIR)/syscall.c
SYSCALL_OBJ = $(OBJ_DIR)/syscall.o

TIMER_SRC = $(KERNEL_SRC_DIR)/timer.c
TIMER_OBJ = $(OBJ_DIR)/timer.o

BOOTLOADER_SRC = ./arch/x86/boot/nekonest.s
BOOTLOADER_BIN = $(BIN_DIR)/nekonest

NEKO_OS_IMG = $(IMG_DIR)/NekoOS.img

LINKER_FILE = linker.ld

# Drivers
KEYBOARD_DRIVER_SRC = $(KERNEL_DRIVERS)/device/io/keyboard/keyboard.c
KEYBOARD_DRIVER_OBJ = $(OBJ_DRIVER_DIR)/keyboard.o
ATA_DRIVER_SRC = $(KERNEL_DRIVERS)/media/ata.c
ATA_DRIVER_OBJ = $(OBJ_DRIVER_DIR)/ata.o

# Dirs
BUILD_DIR      = build
BIN_DIR        = $(BUILD_DIR)/bin
OBJ_DIR        = $(BUILD_DIR)/obj
OBJ_DRIVER_DIR = $(OBJ_DIR)/drivers
OBJ_BOOT_DIR   = $(OBJ_DIR)/boot
IMG_DIR        = $(BUILD_DIR)/img

BOOT_DIR = ./arch/x86/boot

KERNEL_SRC_DIR = ./arch/x86/kernel
KERNEL_INCLUDE = ./arch/x86/include
KERNEL_DRIVERS = drivers
STD_INCLUDE = include

# Exec
LD = /usr/bin/ld
ASM = /usr/bin/nasm
CC = /usr/bin/gcc
QEMU = /usr/bin/qemu-system-i386
STRIP = /usr/bin/strip

# ==============================================

# === BUILD_DIR

$(BUILD_DIR):
	mkdir -p $(BIN_DIR) $(OBJ_DIR) $(IMG_DIR) $(OBJ_DRIVER_DIR) $(OBJ_BOOT_DIR)

# === KERNEL_BIN

$(KERNEL_BIN): $(KERNEL_LOADER_OBJ) $(KERNEL_OBJ) $(KEYBOARD_DRIVER_OBJ) $(ATA_DRIVER_OBJ) $(GDT_OBJ) $(IDT_OBJ) $(ISR_OBJ) $(TIMER_OBJ) $(TERMINAL_OBJ) $(SHELL_OBJ) $(SYSCALL_OBJ)
	$(LD) $(LDFLAGS) $^ -o $@
$(SYSCALL_OBJ):
	$(CC) $(CFLAGS) $(SYSCALL_SRC) -c -o $@
$(SHELL_OBJ):
	$(CC) $(CFLAGS) $(SHELL_SRC) -c -o $@
$(TERMINAL_OBJ):
	$(CC) $(CFLAGS) $(TERMINAL_SRC) -c -o $@
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
$(KERNEL_OBJ):
	$(CC) $(CFLAGS) $(KERNEL_SRC) -c -o $@
$(KERNEL_LOADER_OBJ):
	$(ASM) $(ASMFLAGSELF) $(KERNEL_LOADER_SRC) -o $@

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

run: image
	$(QEMU) $(QEMUFLAGS)

debug: image
	$(QEMU) $(QEMUFLAGS) -s -S
