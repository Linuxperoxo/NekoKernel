# Variáveis de Diretórios
BOOT_DIR = boot
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj
KERNEL_DIR = kernel
DRIVERS_DIR = $(KERNEL_DIR)/drivers
INCLUDE_DIR = $(KERNEL_DIR)/include
SRC_DIR = $(KERNEL_DIR)/src
K_HEADERS = $(KERNEL_DIR)/k_headers

# Variáveis de Arquivos
BOOT_ASM = $(BOOT_DIR)/boot.asm
BOOT_OBJ = $(OBJ_DIR)/boot.o
KERNEL_OBJ = $(OBJ_DIR)/kernel.o
VGA_OBJ = $(OBJ_DIR)/vga.o
KERNEL_SRC = $(SRC_DIR)/kernel.c
VGA_SRC = $(DRIVERS_DIR)/video/vga/vga.c
VGA_HEADER = $(DRIVERS_DIR)/video/vga/vga.h
PORTS_HEADER = $(INCLUDE_DIR)/IO/ports.h
GDT_SRC = $(SRC_DIR)/gdt.c
GDT_HEADER = $(SRC_DIR)/gdt.h
GDT_OBJ = $(OBJ_DIR)/gdt.o
GDT_ASM = $(SRC_DIR)/gdt.asm
GDT_ASM_OBJ = $(OBJ_DIR)/gdt_asm.o

# Nome do Kernel
KERNEL_BIN = $(BIN_DIR)/kernel

# Linker Script na raiz do projeto
LINKER_SCRIPT = linker.ld

# Compilador e Flags
CC = gcc
ASM = nasm
STRIP = strip
LD = ld
CFLAGS = -ffreestanding -nostdlib -nostartfiles -fno-stack-protector -fno-builtin -m32 -O0 -I $(INCLUDE_DIR) -I $(DRIVERS_DIR) -I $(K_HEADERS)
ASFLAGS = -f elf32
LDFLAGS = -z noexecstack -nostdlib -m elf_i386 -T $(LINKER_SCRIPT)

# $^ : Todos as deps (Tudo após o ':')
# $< : Primera dep
# $@ : Alvo da regra (Tudo antes do ':') 

# Regras do Makefile
all: $(KERNEL_BIN)

# Regra para garantir que os diretórios existam antes de compilar
$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Regra para compilar o kernel binário
$(KERNEL_BIN): $(BOOT_OBJ) $(KERNEL_OBJ) $(VGA_OBJ) $(GDT_OBJ) $(GDT_ASM_OBJ)
	$(LD) $(LDFLAGS) -o $(KERNEL_BIN) $^

$(GDT_OBJ):
	$(CC) $(CFLAGS) -c -o $@ $(GDT_SRC)  

$(GDT_ASM_OBJ):
	$(ASM) $(ASFLAGS) $(GDT_ASM) -o $(GDT_ASM_OBJ)

# Regra para compilar o bootloader
$(BOOT_OBJ): $(BOOT_ASM) $(OBJ_DIR)
	$(ASM) $(ASFLAGS) -o $@ $<

# Regra para compilar o arquivo do kernel
$(KERNEL_OBJ): $(KERNEL_SRC) $(VGA_HEADER) $(PORTS_HEADER) $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Regra para compilar o driver VGA
$(VGA_OBJ): $(VGA_SRC) $(VGA_HEADER) $(PORTS_HEADER) $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

strip:
	$(STRIP) $(KERNEL_BIN)
	
# Limpeza
clean:
	rm -rf $(BUILD_DIR)

# Regra de execução no QEMU (após a construção do kernel)
run: $(KERNEL_BIN)
	qemu-system-i386 -kernel build/bin/kernel -append "vga=0x302"

# Regra para iniciar tudo (compilando e rodando)
build_and_run: all run
