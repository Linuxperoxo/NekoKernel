

# Flags
CFLAGS = -g -ffreestanding -v -nostdlib -nostartfiles -fno-stack-protector -fno-builtin -m32 -O0 $(INCLUDES)
ASMFLAGSBIN = -f bin
ASMFLAGSELF = -f elf32
LDFLAGS = -z noexecstack -nostdlib -m elf_i386 -T $(LINKER_FILE)
QEMUFLAGS = -kernel $(KERNEL_BIN)
QEMUFLAGSDEBUG = -kernel $(KERNEL_BIN) -s -S
INCLUDES = -I $(STD_INCLUDE) -I $(KERNEL_INCLUDE) -I $(KERNEL_DRIVERS)  

# Files
KERNEL_SRC = $(KERNEL_SRC_DIR)/kernel.c
KERNEL_OBJ = $(OBJ_DIR)/kernel.o
KERNEL_BIN = $(BIN_DIR)/kernel

KERNEL_LOADER_SRC = $(BOOT_DIR)/loader.s
KERNEL_LOADER_OBJ = $(OBJ_DIR)/loader.o

VGA_DRIVER_SRC = $(KERNEL_DRIVERS)/video/vga/vga.c
VGA_DRIVER_OBJ = $(OBJ_DIR)/vga.o

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

# Rules
all: $(BUILD_DIR) $(KERNEL_BIN)

$(BUILD_DIR):
	mkdir -p $(BIN_DIR) $(OBJ_DIR) $(IMG_DIR)

$(KERNEL_LOADER_OBJ):
	$(ASM) $(ASMFLAGSELF) $(KERNEL_LOADER_SRC) -o $@

$(VGA_DRIVER_OBJ):
	$(CC) $(CFLAGS) $(VGA_DRIVER_SRC) -c -o $@ 

$(KERNEL_OBJ):
	$(CC) $(CFLAGS) $(KERNEL_SRC) -c -o $@ 

$(KERNEL_BIN): $(KERNEL_OBJ) $(VGA_DRIVER_OBJ) $(KERNEL_LOADER_OBJ)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)

run: all
	$(QEMU) $(QEMUFLAGS)

rund: all
	$(QEMU) $(QEMUFLAGSDEBUG)
