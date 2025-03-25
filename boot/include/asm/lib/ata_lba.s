/*
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : ata_lba.s                     |
 *    |                                            |
 *    O--------------------------------------------/
 *
 */

.equ DATA_PORT, 0x1F0 # Porta de dados
.equ ERROR_PORT, 0x1F1 # Porta de erro
.equ SECTOR_COUNT, 0x1F2 # Número de setores
.equ SECTOR_NUMBER, 0x1F3 # Número do setor | LBA low
.equ CYLINDER_LOW, 0x1F4 # Cilindro (bits baixos) | LBA mid
.equ CYLINDER_HIGH, 0x1F5 # Cilindro (bits altos) | LBA high
.equ LBA_LOW, 0x1F3 # LBA low
.equ LBA_MID, 0x1F4 # LBA mid
.equ LBA_HIGH, 0x1F5 # LBA high
.equ DRIVE_HEAD, 0x1F6 # Seleção de drive e Cabeçote
.equ STATUS_PORT, 0x1F7 # Porta de status
.equ COMMAND_PORT, 0x1F7 # Porta de comando
.equ COMMAND_READ, 0x20 # Comando de leitura para a porta 0x1F7
.equ SECTOR_SIZE_BYTE, 512 # Tamanho de cada setor em bytes
.equ SECTOR_SIZE_WORD, 256 # Tamanho de cada setor em words

#
# === EXPLICAÇÃO SOBRE OS COMPONENTES DO HD ===
#
# PRATO (Platter):
# - É o disco físico onde os dados são armazenados.
# - Pode haver múltiplos pratos empilhados em um HD.
# - Cada prato tem duas superfícies (superior e inferior) que podem armazenar dados.
#
# CABEÇOTE (Head):
# - É o "braço leitor/escritor" do HD.
# - Cada face do prato tem um cabeçote associado.
# - Ele se move radialmente para acessar diferentes trilhas do prato.
#
# CILINDRO (Cylinder):
# - Conjunto de trilhas alinhadas verticalmente através dos pratos.
# - Quando o cabeçote está posicionado em um cilindro, ele pode acessar todas as trilhas no mesmo alinhamento.
#
# TRILHA (Track):
# - É um anel circular no prato onde os dados são armazenados.
# - Cada prato possui várias trilhas concêntricas.
# - As trilhas são organizadas em setores.
#
# SETOR (Sector):
# - É a menor unidade de armazenamento no HD.
# - Cada setor geralmente armazena 512 bytes ou 4 KB de dados.
# - O setor é identificado pelo endereço CHS (Cilindro, Cabeça, Setor) ou LBA (Endereçamento por Bloco Lógico)
#
# EXEMPLO DE ORGANIZAÇÃO:
# 1. O prato gira continuamente.
# 2. O cabeçote move-se para o cilindro desejado.
# 3. Dentro do cilindro, o cabeçote acessa a trilha correta.
# 4. Dentro da trilha, o cabeçote localiza o setor para ler ou escrever dados.
#
# RELAÇÃO ENTRE COMPONENTES:
# - PRATO -> Disco físico onde os dados são gravados.
# - CABEÇOTE -> "Braço leitor" que acessa as trilhas e setores.
# - CILINDRO -> Agrupamento vertical de trilhas alinhadas.
# - TRILHA -> Círculos concêntricos no prato.
# - SETOR -> "Fatia de pizza" dentro da trilha, que contém os dados.
#
# ==============================================

.section .text
.code32
.global ata_lba_read
.type ata_lba_read, @function
.align 4
ata_lba_read:

  # NOTE:
  # -   (%ebp): u8 __head__; 
  # -  4(%ebp): u8 __sectors_to_read__;
  # -  8(%ebp): u32 __drive_addrs__;
  # - 12(%ebp): u32 __dest_addrs__;
 
  # NOTE:
  # Para fazer a manipulação do disco usando o controlador ATA
  # precisamos usar instruções OUT e IN de 8 bits, também precisamos
  # mandar os comandos para as portas corretas

  pushl %ebp
  
  leal 8(%esp), %ebp

  pushl %eax
  pushl %ebx
  pushl %ecx
  pushl %edx
  pushl %edi

  # NOTE:
  # Porta 0x1F7:
  #   (Bits 0-3): Cabeçote 
  #   (Bit 4): Drive | 0 -> Master | 1 -> Slave. OBS: ATA só suporta 2 drive, o master e o slave
  #   (Bit 5): Nos primeiros padrões de discos rígidos (IDE/ATA antigos), esse bit tinha uma função específica. No entanto, com a evolução do protocolo, ele foi fixado em 1 para manter compatibilidade.
  #   (Bit 6): Tipo de endereçamento, 0 -> CHS | 1 -> LBA
  #   (Bit 7): Esse bit sempre deve ser 1 de acordo com a especificação ATA.

  movb (%ebp), %al
  andb $0b00001111, %al # NOTE: -> Pegando apenas os 4 bits mais baixos, que fala qual o cabeçote
  orb $0b11100000, %al # NOTE: -> LBA | Master, nesse vamos usar LBA em vez de CHS, mas o LBA faz um calcúlo com o CHS para chegar em um cabeçote, sector e cilindro válido no disco
  movw $DriVE_HEAD, %dx
  outb %al, %dx

  # NOTE:
  # Porta 0x1F2:
  #   Configurando a quantidade de setores para leitura

  movb 4(%ebp), %al
  movw $SECTOR_COUNT, %dx
  outb %al, %dx

  # NOTE:
  # Porta 0x1F3:
  #   Configurando o endereço LBA low

  movl 8(%ebp), %eax # NOTE: -> Agora o endereço completo LBA está em eax, %al = LBA_LOW | %ah = LBA_MID 
  movw $LBA_LOW, %dx
  outb %al, %dx

  # NOTE:
  # Porta 0x1F4: 
  #   Configurando o endereço LBA mid
  
  shrl $8, %eax # NOTE: -> %al = LBA_MID | %ah = LBA_HIGH
  movw $LBA_MID, %dx
  outb %al, %dx

  # NOTE:
  # Porta 0x1F5:
  #   Configurando o endereço LBA high
  
  shrl $8, %eax # NOTE: -> %al = LBA_HIGH
  movw $LBA_HIGH, %dx
  outb %al, %dx
  
  # NOTE:
  # Porta 0x1F7:  
  #   Enviando comando de leitura para o controlador ATA

  movb $COMMAND_READ, %al
  movw $COMMAND_PORT, %dx
  outb %al, %dx

  movw $ERROR_PORT, %dx # NOTE: -> Verificando se ocorreu algum erro
  inb %dx, %al
  cmpb $0, %al # NOTE: -> Se retornar qualquer coisa != 0 é erro
  jnz 1f 
  jmp 2f

  1:
    # TODO: Add write_screen error here!

    jmp .

  2:
    movw $STATUS_PORT, %dx

  2:
    inb %dx, %al
    testb $0b00001000, %al # NOTE: -> Bit que fala se os dados estão pronto para leitura
    jz 2b # NOTE: -> Se não estiver vamos continuar esperando

  # NOTE:
  # Aqui fazemos um calculo para usar a instrução
  # REP e INSW

  # NOTE:
  # REP: Repete uma instrução até ECX != 0
  # INSW: Lê a porta de DX, e manda os dados para ES:DI. Se DF = 0 (CLD) 
  #       DI será incrementado de maneira automática, se DF = 1 (STD) DI será decrementado
  
  # NOTE:
  # Cálculo:
  # Pegamos a quantidade de setores no parâmetro, multiplicamos pelo tamanho de cada setor
  # em byte, como vamos pegar word dividimos por 2, essa é a quantidade de vezes que o REP
  # precisa repetir INSW.
  # OBS: Incrementa automaticamente a posição interna do buffer no controlador ATA quando os dados são lido com IN

  # FIXME: Fix multisectors reading
  
  movl 4(%ebp), %ebx
  movl 12(%ebp), %edi

  1:
    movw $DATA_PORT, %dx
    decl %ebx
    movl $SECTOR_SIZE_WORD, %ecx
    rep insw
    cmpl $0, %ebx
    jz 3f  
    movw $STATUS_PORT, %dx 
  
  2:
    inb %dx, %al
    testb 0b00001000, %al
    jz 2b
    jmp 1b

  3:
    popl %edi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    popl %ebp
    ret
