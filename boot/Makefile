#
#
#    /--------------------------------------------O
#    |                                            |
#    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
#    |  AUTHOR    : Linuxperoxo                   |
#    |  FILE      : Makefile                      |
#    |                                            |
#    O--------------------------------------------/
#
#

ASM = /usr/bin/gcc
LD = /usr/bin/ld
QEMU = /usr/bin/qemu-system-i386

ASMFLAGS = -nostdlib -I $(INCLUDE_DIR) -T $(LINKER_FILE)
QEMUFLAGS = -drive file=$(BIN_FILE),format=raw -m 4G -smp 4

NEKO_MAIN_SRC = ./nekonest.s
BIN_FILE = $(BIN_DIR)/nekonest
LINKER_FILE = ./linker.ld

INCLUDE_DIR = ./include
BUILD_DIR = ../build/nekonest
BIN_DIR = $(BUILD_DIR)/bin

ALL_BUILD_DIR = $(BUILD_DIR) $(BIN_DIR)

all: $(BIN_FILE)

make_dirs:
	mkdir -p $(ALL_BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: $(BIN_FILE)
	$(QEMU) $(QEMUFLAGS)

$(BIN_FILE): make_dirs
	$(ASM) $(ASMFLAGS) $(NEKO_MAIN_SRC) -o $(BIN_FILE)

.PHONY: all make_dirs clean run

