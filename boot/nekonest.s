/*
 *
 *    /--------------------------------------------O
 *    |                                            |
 *    |  COPYRIGHT : (c) 2025 per Linuxperoxo.     |
 *    |  AUTHOR    : Linuxperoxo                   |
 *    |  FILE      : nekonest.s                    |
 *    |                                            |
 *    O--------------------------------------------/
 *
 */

/*
 * 
 * NekoNest será nosso bootloader para o NekoKernel.
 *
 * === Como o NekoNest funciona? ===
 *
 * O NekoNest vai ser resposável por configurar as coisas mais básicas antes de 
 * passar o controle para o kernel, primeira coisa que vamos fazer é configurar um
 * GDT simples com apenas 3 segmentos, segmento null, segmento de código do kernel 
 * e o segmento de data do kernel. Quando carregamos o GDT vamos sair do real mode
 * para o protected mode, mas porque temos que fazer isso? Quando estamos no real mode
 * estamos limitados em usar apenas instruções de 16 bits, e endereçamento de apenas 1MB 
 * (0x00 - 0xFFFFF), então para poder usar todo nosso endereçamento de 32 bits e instruções 
 * de 32 bits vamos ter que configurar o GDT.
 *
 * COMO CONFIGURAR O GDT? 
 * 
 * Para configurar o GDT temos que usar a instrução LGDT [GDL_ENTRIES_PTR]. GDL_ENTRIES_PTR é nosso
 * ponteiro para os segmentos, cada segmento tem o tamanho de 8 bytes, imagine o GDL_ENTRIES_PTR sendo
 * um ponteiro para um array de struct GDT_ENTRY. Cada GDT_ENTRY tem que seguir exatamente esse endereçamento 
 * de struct
 *
 * struct GDT_ENTRY {
 *   __u16 __limit#
 *   __u16 __base#
 *   __u8 __base_middle#
 *   __u8 __access#
 *   __u8 __flags#
 *   __u8 __base_high
 * }__attribute__((packed))#
 *
 * GDT_ENTRIES_PTR {
 *   __u16 __limit# 
 *   __u32 __
 * }__attribute__((packed))#
 *
 * SIM! Ele deve seguir exatamente essa onder na memória, para mais informações sobre pra que serve
 * cada membro e sobre os registradores de segmento, você pode olhar o arquivo https://github.com/Linuxperoxo/NekoKernel/blob/main/include/neko/gdt.h 
 * 
 * O __attribute__((packed)) serve para que o compilador ignore o alinhamento dos membros, isso é importante 
 * nesse caso ele não deve ser ignorado
 *
 * =================================
 *
 */ 

/*
 *
 * === COMO FUNCIONA O BOOTLOADER? ===
 *
 * 1. Inicialização do computador:
 *    Quando o computador é ligado, o primeiro cóDIgo executado é o da BIOS. 
 *A BIOS realiza o processo de inicialização do hardware, realizando verificações e configurando os DIspositivos necessários, 
 *    como a memória, o teclado, o processador, e os DIscos.
 *
 * 2. Localização do MBR:
 *    Após a inicialização, a BIOS começa a procurar por um Master Boot Record (MBR) nos DIscos. O MBR está localizado no primeiro 
 *    setor de cada DIsco (sector 0), que tem 512 bytes. Esse setor contém o código inicial de boot, que é o bootloader, que será 
 *    executado em seguida.
 *
 * 3. Validação do MBR:
 *    Quando a BIOS encontra o setor 0, ela verifica os últimos 2 bytes (512º byte), chamados de assinatura do MBR. Esses dois 
 *    bytes devem ser #*0x55** (no byte 511) e **0xAA** (no byte 512), respectivamente. Essa assinatura é o que inDIca à BIOS 
 *    que o setor contém um cóDIgo de boot válido.
 *
 * 4. Carregamento do MBR:
 *    Caso a assinatura esteja correta, a BIOS então carrega o conteúdo do MBR para o endereço #*0x7C00** na memória. Este é o 
 *    endereço padrão onde o bootloader será carregado, e a BIOS então passa o controle para esse endereço, ou seja, ela começa 
 *    a execução do cóDIgo do bootloader.
 *
 * 5. O que acontece depois?
 *    A partir daí, o bootloader pode começar a executar, carregando o sistema operacional ou qualquer outro cóDIgo necessário 
 *    para iniciar o computador.
 *
 * Em resumo, o processo de boot é o seguinte:
 * 1. A BIOS inicializa o hardware.
 * 2. A BIOS busca o MBR no DIsco e valida a assinatura (0x55AA).
 * 3. Se a assinatura for válida, a BIOS carrega o MBR para 0x7C00 e passa o controle para o bootloader.
 * =============================================
 * 
 */
  
.include "asm/lib/stdio.s"
.include "asm/nekonest/gdt.s"
.include "asm/nekonest/magic.s"
.include "asm/nekonest/idt.s"

.equ STACK, 0xFFFFFF

.section .text.main, "a"
.global .main
.type .main, @function
.main:
  .code16
  .type .real, @function
  .real:
    cli

    lgdt .GDT_Ptr

    # O registrador CR0 é um registrador de controle da CPU x86.
    # É por ele que ativamos o GDT, virtual memory, etc.
    # Alterações nesse registrador afetam diretamente o funcionamento do processador. 
    
    movl %cr0, %eax
    orl $1, %eax
    movl %eax, %cr0 # Agora o bit de ativação do GDT está habilitado

    movw $DATA_SEGMENT, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %es

    
    # Aqui precisamos alterar o segmento de código (CS)
    # Para alterar ele precisamos usar uma instrução de far jmp (ljmp), que 
    # serve para alterar o cs. O near jump (jmp), apenas um salto dentro do
    # segmento atual, sem alterar o cs

    ljmp $CODE_SEGMENT, $.protected # Far jmp para alterar o CS 

  # Agora estamos no modo protegindo, usando instruções de 32 bits

  .code32
  .type .protected, @function
  .protected:
    movl $STACK, %esp

    call clearf

    pushl $.neko_booting
    call printf

    pushl $2000000000
    call .sleep

    movl $0x80000002, %eax
    cpuid
    movl $.cpuid, %edi
    movl %eax, 0(%edi)
    movl %ebx, 4(%edi)
    movl %ecx, 8(%edi)
    movl %edx, 12(%edi)
     
    movl $0x80000003, %eax
    cpuid
    movl %eax, 16(%edi)
    movl %ebx, 20(%edi)
    movl %ecx, 24(%edi)
    movl %edx, 28(%edi)
  
    pushl $.cpu_detected
    call printf

    pushl $.cpuid
    call printf
     
    hlt

.code32
.type .sleep, @function
.align 4
.sleep: # Uma função de delay temporária, usada até configurar o PIT
  pushl %ebp
  
  leal 8(%esp), %ebp
  
  pushl %ecx
  
  movl (%ebp), %ecx

  1:
    loop 1b

  popl %ecx
  popl %ebp
  ret

.section .bss
.type .cpuid, @object
.align 4
.cpuid:
  .space 32, 0

.section .string, "aS"
.type .cpu_detected, @object
.align 4
.cpu_detected:
  .asciz "CPU: "

.type .neko_booting, @object
.align 4
.neko_booting:
  .asciz "NEKONEST STARTING...\n\n"
