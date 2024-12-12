#
#
#
#    /--------------------------------------------O
#    |                                            |
#    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
#    |  AUTHOR    : Linuxperoxo                   |
#    |  FILE      : Makefile                      |
#    |  SRC MOD   : 11/12/2024                    |
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

# Files
KERNEL_SRC = $(KERNEL_SRC_DIR)/kernel.c
KERNEL_OBJ = $(OBJ_DIR)/kernel.o
KERNEL_BIN = $(BIN_DIR)/kernel

KERNEL_LOADER_SRC = $(BOOT_DIR)/loader.s
KERNEL_LOADER_OBJ = $(OBJ_DIR)/loader.o

NEKONEST_BOOTLOADER_SRC = $(BOOT_DIR)/NekoNest/src/nekonest.s
NEKONEST_BOOTLOADER_BIN = $(BIN_DIR)/nekonest

NEKO_OS_IMG = ./nekoOS.img

# Drivers
VGA_DRIVER_SRC = $(KERNEL_DRIVERS)/video/vga/vga.c
VGA_DRIVER_OBJ = $(OBJ_DIR)/vga.o
ATA_DRIVER_SRC = $(KERNEL_DRIVERS)/media/ata.c
ATA_DRIVER_OBJ = $(OBJ_DIR)/ata.o

GDT_SRC = $(KERNEL_SRC_DIR)/gdt.c
GDT_OBJ = $(OBJ_DIR)/gdt_c.o

LINKER_FILE = linker.ld

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

$(GDT_OBJ):
	$(CC) $(CFLAGS) -c -o $@ $(GDT_SRC) 

$(KERNEL_LOADER_OBJ):
	$(ASM) $(ASMFLAGSELF) $(KERNEL_LOADER_SRC) -o $@

$(ATA_DRIVER_OBJ): 
	$(CC) $(CFLAGS) $(ATA_DRIVER_SRC) -c -o $@

$(VGA_DRIVER_OBJ):
	$(CC) $(CFLAGS) $(VGA_DRIVER_SRC) -c -o $@ 

$(KERNEL_OBJ):
	$(CC) $(CFLAGS) $(KERNEL_SRC) -c -o $@

$(KERNEL_BIN): $(KERNEL_OBJ) $(VGA_DRIVER_OBJ) $(ATA_DRIVER_OBJ) $(KERNEL_LOADER_OBJ) $(GDT_OBJ)
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
	rm $(NEKO_OS_IMG)

run: strip image
	$(QEMU) $(QEMUFLAGS)
