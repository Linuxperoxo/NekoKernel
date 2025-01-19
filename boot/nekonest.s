;
;
;
;    /--------------------------------------------O
;    |                                            |
;    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
;    |  AUTHOR    : Linuxperoxo                   |
;    |  FILE      : nekonest.s                    |
;    |  SRC MOD   : 15/01/2025                    |
;    |  VERSION   : 1.0-1                         |
;    |                                            |
;    O--------------------------------------------/
;    
;
;

;
; NekoNest será nosso bootloader para o NekoKernel.
;
; === Como o NekoNest funciona? ===
;
; O NekoNest vai ser resposável por configurar as coisas mais básicas antes de 
; passar o controle para o kernel, primeira coisa que vamos fazer é configurar um
; GDT simples com apenas 3 segmentos, segmento null, segmento de código do kernel 
; e o segmento de data do kernel. Quando carregamos o GDT vamos sair do real mode
; para o protected mode, mas porque temos que fazer isso? Quando estamos no real mode
; estamos limitados em usar apenas instruções de 16 bits, e endereçamento de apenas 1MB 
; (0x00 - 0xFFFFF), então para poder usar todo nosso endereçamento de 32 bits e instruções 
; de 32 bits vamos ter que configurar o GDT.
;
; COMO CONFIGURAR O GDT? 
; 
; Para configurar o GDT temos que usar a instrução LGDT [GDL_ENTRIES_PTR]. GDL_ENTRIES_PTR é nosso
; ponteiro para os segmentos, cada segmento tem o tamanho de 8 bytes, imagine o GDL_ENTRIES_PTR sendo
; um ponteiro para um array de struct GDT_ENTRY. Cada GDT_ENTRY tem que seguir exatamente esse endereçamento 
; de struct
;
; struct GDT_ENTRY {
;   __u16 __limit;
;   __u16 __base;
;   __u8 __base_middle;
;   __u8 __access;
;   __u8 __flags;
;   __u8 __base_high
; }__attribute__((packed));
;
; GDT_ENTRIES_PTR {
;   __u16 __limit; 
;   __u32 __
; }__attribute__((packed));
;
; SIM! Ele deve seguir exatamente essa onder na memória, para mais informações sobre pra que serve
; cada membro e sobre os registradores de segmento, você pode olhar o arquivo https://github.com/Linuxperoxo/kernel/tree/main/kernel/include/gdt.h 
; 
; O __attribute__((packed)) serve para que o compilador ignore o alinhamento dos membros, isso é importante 
; nesse caso e não deve ser ignorado
;
; OBS: É extremamente difícil explicar como funciona o GDT, ele é uma mágica incrível, recomendo você estudar 
; sobre para entender bem :)
;
; Já nosso 
; =================================
;

;
; === COMO FUNCIONA O BOOTLOADER? ===
;
; 1. Inicialização do computador:
;    Quando o computador é ligado, o primeiro cóDIgo executado é o da BIOS. 
;    A BIOS realiza o processo de inicialização do hardware, realizando verificações e configurando os DIspositivos necessários, 
;    como a memória, o teclado, o processador, e os DIscos.
;
; 2. Localização do MBR:
;    Após a inicialização, a BIOS começa a procurar por um Master Boot Record (MBR) nos DIscos. O MBR está localizado no primeiro 
;    setor de cada DIsco (sector 0), que tem 512 bytes. Esse setor contém o código inicial de boot, que é o bootloader, que será 
;    executado em seguida.
;
; 3. Validação do MBR:
;    Quando a BIOS encontra o setor 0, ela verifica os últimos 2 bytes (512º byte), chamados de assinatura do MBR. Esses dois 
;    bytes devem ser **0x55** (no byte 511) e **0xAA** (no byte 512), respectivamente. Essa assinatura é o que inDIca à BIOS 
;    que o setor contém um cóDIgo de boot válido.
;
; 4. Carregamento do MBR:
;    Caso a assinatura esteja correta, a BIOS então carrega o conteúdo do MBR para o endereço **0x7C00** na memória. Este é o 
;    endereço padrão onde o bootloader será carregado, e a BIOS então passa o controle para esse endereço, ou seja, ela começa 
;    a execução do cóDIgo do bootloader.
;
; 5. O que acontece depois?
;    A partir daí, o bootloader pode começar a executar, carregando o sistema operacional ou qualquer outro cóDIgo necessário 
;    para iniciar o computador.
;
; Em resumo, o processo de boot é o seguinte:
; 1. A BIOS inicializa o hardware.
; 2. A BIOS busca o MBR no DIsco e valida a assinatura (0x55AA).
; 3. Se a assinatura for válida, a BIOS carrega o MBR para 0x7C00 e passa o controle para o bootloader.
; =============================================

;
; === EXPLICAÇÃO SOBRE OS COMPONENTES DO HD ===
;
; PRATO (Platter):
; - É o disco físico onde os dados são armazenados.
; - Pode haver múltiplos pratos empilhados em um HD.
; - Cada prato tem duas superfícies (superior e inferior) que podem armazenar dados.
;
; CABEÇOTE (Head):
; - É o "braço leitor/escritor" do HD.
; - Cada face do prato tem um cabeçote associado.
; - Ele se move radialmente para acessar diferentes trilhas do prato.
;
; CILINDRO (Cylinder):
; - Conjunto de trilhas alinhadas verticalmente através dos pratos.
; - Quando o cabeçote está posicionado em um cilindro, ele pode acessar todas as trilhas no mesmo alinhamento.
;
; TRILHA (Track):
; - É um anel circular no prato onde os dados são armazenados.
; - Cada prato possui várias trilhas concêntricas.
; - As trilhas são organizadas em setores.
;
; SETOR (Sector):
; - É a menor unidade de armazenamento no HD.
; - Cada setor geralmente armazena 512 bytes ou 4 KB de dados.
; - O setor é identificado pelo endereço CHS (Cilindro, Cabeça, Setor) ou LBA (Endereçamento por Bloco Lógico)
;
; EXEMPLO DE ORGANIZAÇÃO:
; 1. O prato gira continuamente.
; 2. O cabeçote move-se para o cilindro desejado.
;3. Dentro do cilindro, o cabeçote acessa a trilha correta.
; 4. Dentro da trilha, o cabeçote localiza o setor para ler ou escrever dados.
;
; RELAÇÃO ENTRE COMPONENTES:
; - PRATO -> Disco físico onde os dados são gravados.
; - CABEÇOTE -> "Braço leitor" que acessa as trilhas e setores.
; - CILINDRO -> Agrupamento vertical de trilhas alinhadas.
; - TRILHA -> Círculos concêntricos no prato.
; - SETOR -> "Fatia de pizza" dentro da trilha, que contém os dados.
;
; ==============================================

[ORG 0x7C00]

;
; ================ START MACROS ================
;

%define TMP_STACK_ADDRS 0xFFFF

;
; MACROS PARA O CARREGAMENTO DO KERNEL
;

%define KERNEL_DEST            0x10000
%define KERNEL_FILE_SIZE       512 * 49
%define KERNEL_K_LOADER_OFFSET 0x1000

;
; VGA MACROS
;

%define VGA_FRAMEBUFFER_ADDRS 0xB8000
%define VGA_SCREEN_WIDTH      80
%define VGA_SCREEN_HEIGHT     25
%define VGA_DEFAULT_COLOR     0x0F ; Caractere branco com fundo preto

;
; GDT MACROS
;

%define GDT_KERNEL_CODE_SEGMENT_ENTRY 0x08
%define GDT_KERNEL_DATA_SEGMENT_ENTRY 0x10

;
; Macros para manipulações do controlador ATA
;

%define DATA_PORT       0x1F0      ; Porta de dados
%define ERROR_PORT      0x1F1      ; Porta de erro
%define SECTOR_COUNT    0x1F2      ; Número de setores
%define SECTOR_NUMBER   0x1F3      ; Número do setor
%define CYLINDER_LOW    0x1F4      ; Cilindro (bits baixos)
%define CYLINDER_HIGH   0x1F5      ; Cilindro (bits altos)
%define DRIVE_HEAD      0x1F6      ; Seleção de drive e cabeça
%define STATUS_PORT     0x1F7      ; Porta de status
%define COMMAND_PORT    0x1F7      ; Porta de comando
%define COMMAND_READ    0x20
%define BYTE_PER_SECTOR 512

;
; ================ END OF MACROS ================
;

;
; ================ START MAIN ================
;

SECTION .text
  CLI ; Desabilitando todas a interrupções

  LGDT [GDT_entries_ptr]

  ;
  ; Aqui estamos saindo do modo real e indo para o 
  ; protected mode
  ;

  MOV EAX, CR0
  OR EAX, 0x01
  MOV CR0, EAX

  ;
  ; Configurando os registradores de segmento para apontar
  ; para as entradas gdt corretas
  ;

  MOV AX, GDT_KERNEL_DATA_SEGMENT_ENTRY ; Segunda entrada do GDT 
  MOV FS, AX
  MOV GS, AX
  MOV SS, AX
  MOV ES, AX
  MOV DS, AX
  
  ;
  ; Fazemos esse 'Far Jump' para alterar o registrador CS, ele não pode ser
  ; alterado usando MOV, so usando a instrução JMP
  ;
  ; EXEMPLO:
  ;   JMP 0x08:kernel_main ; CS = 0x08
  ;
  ; OUTRO EXEMPLO:
  ;   JMP 0x10:kernel_main ; CS = 0x10
  ;

  JMP GDT_KERNEL_CODE_SEGMENT_ENTRY:protected_mode

;
; ================ END MAIN ==============
;

;
; ================ START "protected_mode" ROUTINE ================
;

;
; Aqui já estamos no modo protegido 32 bits, já podemos usar os registradores
; eax, ebx, ecx e edx, a agora temos o endereçamento completo de sistemas 32 bits  
;

[BITS 32]
protected_mode:
  MOV ESP, TMP_STACK_ADDRS
  MOV EBP, ESP
  
  CALL CLEAR_SCREEN

  PUSH WORD   0x00                               ; Cabeçote
  PUSH WORD   KERNEL_FILE_SIZE / BYTE_PER_SECTOR ; Setores à serem lidos
  PUSH WORD   0x02                               ; Número do setor
  PUSH WORD   0x0000                             ; Cilíndro alto e baixo
  PUSH DWORD  KERNEL_DEST                        ; Endereço de destino
  CALL ATA_DISK_READ

  JMP KERNEL_DEST + KERNEL_K_LOADER_OFFSET

;
; ================ END "protected_mode" ROUTINE ================
;

;
; ================ START GDT_entries DATA ================
;

;
; Tabela GDT temporária para o kernel
;

GDT_entries_start:
  null_segment:
    DD 0x00
    DD 0x00
    
  kernel_code_segment: ; Aqui fica o código do kernel
    DW 0xFFFF          ; __u16 limit;
    DW 0x0000          ; __u16 base_low; 
    DB 0x00            ; __u8 base_middle;
    DB 0b10011010      ; __u8 access;
    DB 0b11001111      ; __u8 flags;
    DB 0x00            ; __u8 base_high;
  
  kernel_data_segment: ; Aqui fica os dados do kernel, .stack .bss .data
    DW 0xFFFF          ; __u16 limit;
    DW 0x0000          ; __u16 base_low;
    DB 0x00            ; __u8 base_middle;
    DB 0b10010010      ; __u8 access; 
    DB 0b11001111      ; __u8 flags;
    DB 0x00            ; __u8 base_high;
GDT_entries_end:

GDT_entries_ptr:
  DW GDT_entries_end - GDT_entries_start - 1
  DD GDT_entries_start

;
; ================ END GDT_entries DATA ================
;

;
; ================ START "CLEAR_SCREEN" ROUTINE ================
;

CLEAR_SCREEN:
  PUSH EDI
  PUSH ESI

  MOV ESI, VGA_FRAMEBUFFER_ADDRS
  XOR EDI, EDI

  .cmp:
    CMP EDI, VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT * 2
    JZ .exit

    MOV [ESI + EDI], BYTE 0x00
    MOV [ESI + EDI + 1], BYTE VGA_DEFAULT_COLOR
    ADD EDI, 0x02
    JMP .cmp

  .exit:
    POP ESI
    POP EDI
    RET

;
; ================ END "CLEAR_SCREEN" ROUTINE ================
;

;
; ================ START "PRINT_SCREEN" ROUTINE ================
;

PRINT_SCREEN:
  PUSH EDI
  PUSH EDX

  MOV EDI, VGA_FRAMEBUFFER_ADDRS

  .cmp:
    CMP [ESI], BYTE 0x00
    JZ .exit
    
    MOV DL, BYTE [ESI]
    MOV [EDI], BYTE DL
    INC ESI
    ADD EDI, 0x02
    JMP .cmp

  .exit:
    POP EDX
    POP EDI
    RET

;
; ================ END "PRINT_SCREEN" ROUTINE ================
;

;
; ================ START "ATA_DISK_READ" ROUTINE ================
;

ATA_DISK_READ:

  ;
  ; Para mais informações de como funciona a leitura e escrita de um disco CHS: https://wiki.osdev.org/ATA_read/write_sectors
  ;
  ; Lá você pode ver um código que explica bem como funciona a comunicação com o controlador ATA/SATA
  ;

  MOV EBP, ESP
  ADD EBP, 0x04 ; Os parâmetros estã́o aqui

  PUSH EAX
  PUSH EBX
  PUSH ECX
  PUSH EDX
  PUSH ESI
  PUSH EDI

  ;
  ; Capturando todos os parâmetros que estão 
  ; na stack
  ;

  MOV EDI, DWORD [EBP]
  MOV CX, WORD [EBP + 4]
  MOV AL, BYTE [EBP + 6]
  MOV AH, BYTE [EBP + 8]
  MOV BL, BYTE [EBP + 10]

  .read_sector:
  
    ;
    ; Cabeçote de leitura está em BL
    ;
    
    PUSH AX

    MOV AL, BL

    MOV DX, DRIVE_HEAD ; Porta que recebe o drive e o cabeçote 
    OR AL, 0b10100000  ; Por padrão os 4 bits mais significativos são 1010, driver master
    OUT DX, AL
    
    ;
    ; A quantidade de setores que vamos ler está em AH
    ;

    MOV AL, 0x01
    MOV DX, SECTOR_COUNT ; Porta de contagem de setores
    OUT DX, AL

    ;
    ; O Setor de leitura está em AL
    ;

    POP AX

    MOV DX, SECTOR_NUMBER ; Porta do número do setor
    OUT DX, AL

    ;
    ; O Cilindro alto está em CL e o cilindro baixo em CH 
    ;

    PUSH AX

    MOV AX, CX
    MOV DX, CYLINDER_LOW
    OUT DX, AL

    MOV AL, AH
    MOV DX, CYLINDER_HIGH
    OUT DX, AL

    ;
    ; Agora que está tudo configurado enviamos o 
    ; comando para o controlador ATA
    ;

    MOV DX, COMMAND_PORT
    MOV AL, COMMAND_READ
    OUT DX, AL

    MOV DX, ERROR_PORT
    IN AL, DX
    CMP AL, 0x00
    JNZ .error

    MOV DX, STATUS_PORT

    .still_going:
      IN AL, DX
      TEST AL, 0x08
      JZ .still_going

    MOV DX, DATA_PORT
    XOR SI, SI

    .copy_data:       ; Loop de leitura de dados
      IN AX, DX        ; A cada leitura da porta de dados o controlador retorna um word lido do setor
      MOV [EDI], AX    ; Copiando para o destino
      ADD EDI, 0x02    ; Incrementa 2 já que lemos um word
      ADD SI, 0x02     ; Incrementamos 2 no index também
      CMP SI, 512      ; Vemos se já conseguimos ler os 512 bytes do setor
      JNZ .copy_data 
     
    POP AX ; Restaurando AX para ver os argumentos de leitura de setores e número do setor 
    
    ;
    ; Aqui simplesmente vemos se já lemos a quantidade de setores, se não incrementamos o setor
    ; e voltamos para o início para ler o próximo setor
    ;

    DEC AH
    CMP AH, 0x00
    JZ .exit

    INC AL
    JMP .read_sector
     
  .error:
    MOV ESI, ERROR_DISK_MSG
    CALL PRINT_SCREEN
    JMP $
    
  .exit:
    POP EDI
    POP ESI
    POP EDX
    POP ECX
    POP EBX
    POP EAX
    RET

ERROR_DISK_MSG:
  DB "NEKOERR: ERROR TO READ DISK, KERNEL IMAGE NOT LOADED!", 0x00

;
; ================ END "ATA_DISK_READ" ROUTINE ================
;

;
; Assinatura de Setor MBR bootável
;

TIMES 510 - ($ - $$) DB 0x00 ; Garantindo que o binário final tenha 512 bytes para a BIOS considerar como MBR bootável
DW 0xAA55                    ; Assinatura de setor MBR bootável válido

