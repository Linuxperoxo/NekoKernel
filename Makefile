#
#
#
#    /--------------------------------------------O
#    |                                            |
#    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
#    |  AUTHOR    : Linuxperoxo                   |
#    |  FILE      : Makefile                      |
#    |  SRC MOD   : 18/12/2024                    |
#    |                                            |
#    O--------------------------------------------/
#
#
#

# Flags
CFLAGS = -g -ffreestanding -v -nostdlib -nostartfiles -fno-stack-protector -fno-builtin -m32 -O0 $(INCLUDES)
ASMFLAGSBIN = -f bin
ASMFLAGSELF = -g -f elf32
LDFLAGS = -z noexecstack -nostdlib -m elf_i386 -T $(LINKER_FILE)
QEMUFLAGS = -drive file=$(NEKO_OS_IMG),format=raw
QEMUFLAGSDEBUG = $(QEMUFLAGS) -s -S
INCLUDES = -I $(STD_INCLUDE) -I $(KERNEL_INCLUDE) -I $(KERNEL_DRIVERS)  

# Kernel Files
KERNEL_SRC = $(KERNEL_SRC_DIR)/kernel.c
KERNEL_OBJ = $(OBJ_DIR)/kernel.o
KERNEL_BIN = $(BIN_DIR)/kernel

KERNEL_LOADER_SRC = $(BOOT_DIR)/loader.s
KERNEL_LOADER_OBJ = $(OBJ_DIR)/loader.o

TERMINAL_SRC = $(KERNEL_SRC_DIR)/terminal.c
TERMINAL_OBJ = $(OBJ_DIR)/terminal.o

GDT_SRC = $(KERNEL_SRC_DIR)/gdt.c
GDT_OBJ = $(OBJ_DIR)/gdt.o

IDT_SRC = $(KERNEL_SRC_DIR)/idt.c
IDT_OBJ = $(OBJ_DIR)/idt.o

ISR_SRC = $(KERNEL_SRC_DIR)/isr.s
ISR_OBJ = $(OBJ_DIR)/isr.o

NEKONEST_BOOTLOADER_SRC = $(BOOT_DIR)/NekoNest/src/nekonest.s
NEKONEST_BOOTLOADER_BIN = $(BIN_DIR)/nekonest

NEKO_OS_IMG = ./nekoOS.img

LINKER_FILE = linker.ld

# Drivers
KEYBOARD_DRIVER_SRC = $(KERNEL_DRIVERS)/device/io/keyboard/keyboard.c
KEYBOARD_DRIVER_OBJ = $(OBJ_DIR)/keyboard.o
ATA_DRIVER_SRC = $(KERNEL_DRIVERS)/media/ata.c
ATA_DRIVER_OBJ = $(OBJ_DIR)/ata.o

# Dirs
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj
IMG_DIR = $(BUILD_DIR)/img

BOOT_DIR = boot

KERNEL_SRC_DIR = kernel/src
KERNEL_INCLUDE = kernel/include
KERNEL_DRIVERS = drivers
STD_INCLUDE = include

# Exec
LD = /usr/bin/ld
ASM = /usr/bin/nasm
CC = /usr/bin/gcc
QEMU = /usr/bin/qemu-system-i386
STRIP = /usr/bin/strip

# Rules
kernel: $(BUILD_DIR) $(KERNEL_BIN)

$(BUILD_DIR):
	mkdir -p $(BIN_DIR) $(OBJ_DIR) $(IMG_DIR)

$(TERMINAL_OBJ):
	$(CC) $(CFLAGS) $(TERMINAL_SRC) -c -o $@

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

$(KERNEL_BIN): $(KERNEL_LOADER_OBJ) $(KERNEL_OBJ) $(KEYBOARD_DRIVER_OBJ) $(ATA_DRIVER_OBJ) $(GDT_OBJ) $(IDT_OBJ) $(ISR_OBJ) $(TERMINAL_OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

bootloader: $(NEKONEST_BOOTLOADER_BIN)

$(NEKONEST_BOOTLOADER_BIN): $(BUILD_DIR)
	$(ASM) $(ASMFLAGSBIN) $(NEKONEST_BOOTLOADER_SRC) -o $@

image: kernel bootloader
	cat $(NEKONEST_BOOTLOADER_BIN) $(KERNEL_BIN) > $(NEKO_OS_IMG)  

strip: kernel
	$(STRIP) $(KERNEL_BIN)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(NEKO_OS_IMG)

run: strip image
	$(QEMU) $(QEMUFLAGS)
