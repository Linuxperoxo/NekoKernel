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

ASMFLAGS = -nostdlib -I ./include
QEMUFLAGS = -drive file=$(BIN_FILE),format=raw -m 4G -smp 4

BIN_FILE = $(BIN_DIR)/nekonest
LINKER_FILE = ./linker.ld

BUILD_DIR = ../build/nekonest
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj
NEKO_SRC_DIR = ./include/asm/nekonest
NEKO_LIB_DIR = ./include/asm/lib

ALL_BUILD_DIR = $(BIN_DIR) $(OBJ_DIR)

NEKO_SRCS = $(wildcard $(NEKO_SRC_DIR)/*.s)
NEKO_SRCS_LIB = $(wildcard $(NEKO_LIB_DIR)/*.s)

NEKO_SRCS_OBJS = $(patsubst $(NEKO_SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(NEKO_SRCS))
NEKO_SRCS_LIB_OBJS = $(patsubst $(NEKO_LIB_DIR)/%.s, $(OBJ_DIR)/%.o, $(NEKO_SRCS_LIB))

NEKO_MAIN_SRC = ./nekonest.s
NEKO_MAIN_OBJ = $(OBJ_DIR)/nekonest.o

all: $(BIN_FILE)

make_dirs:
	mkdir -p $(ALL_BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: $(BIN_FILE)
	$(QEMU) $(QEMUFLAGS)

$(OBJ_DIR)/%.o: $(NEKO_SRC_DIR)/%.s | make_dirs
	$(ASM) $(ASMFLAGS) $< -c -o $@

$(OBJ_DIR)/%.o: $(NEKO_LIB_DIR)/%.s | make_dirs
	$(ASM) $(ASMFLAGS) $< -c -o $@

$(NEKO_MAIN_OBJ): make_dirs
	$(ASM) $(ASMFLAGS) $(NEKO_MAIN_SRC) -c -o $@

$(BIN_FILE): $(NEKO_MAIN_OBJ) $(NEKO_SRCS_OBJS) $(NEKO_SRCS_LIB_OBJS) | make_dirs
	$(LD) -L $(OBJ_DIR) -T $(LINKER_FILE) -o $(BIN_FILE)

.PHONY: all make_dirs clean run

