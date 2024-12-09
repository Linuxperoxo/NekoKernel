;
;
;
;    /--------------------------------------------O
;    |                                            |
;    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
;    |  AUTHOR    : Linuxperoxo                   |
;    |  FILE      : nekonest.s                    |
;    |  SRC MOD   : 09/12/2024                    |
;    |  VERSION   : 0.0-1                         |
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
; Portas para manipulação do controlador ATA
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

;
; Informações do VGA para o PRINTF e CLEANF
;

%define VGA_FRAMEBUFFER_ADDRS_MAP 0xB8000
%define DEFAULT_COLOR 0x0F
%define VGA_SCREEN_H 25
%define VGA_SCREEN_W 80

GDT_kernel_code_entry EQU kernel_code_segment - GDT_entries_start ; 0x08 Primeira entrada GDT, com permissão 0 (Ring 0)
GDT_kernel_data_entry EQU kernel_data_segment - GDT_entries_start ; 0x01 Segunda entrada GDT, com permissão 0 (Ring 9)

SECTION .text
  CLI ; Desabilitando interrupções externas para não atrapalhar durante o boot 
  
  ;
  ; Carregando GDT básico
  ;

  LGDT [GDT_entries_ptr]

  ;
  ; Saindo do real mode
  ;

  MOV EAX, CR0
  OR EAX, 0x01
  MOV CR0, EAX

  ;
  ; Configurando os registradores de segmento já que estamos no modo protegido 
  ;

  MOV AX, GDT_kernel_data_entry
  MOV DS, AX
  MOV FS, AX
  MOV ES, AX
  MOV GS, AX
  MOV SS, AX

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

  JMP GDT_kernel_code_entry:protected_mode

GDT_entries_start:
  null_segment: ; Segmento obrigatório
    DD 0x00000000
    DD 0x00000000

  kernel_code_segment: ; Aqui fica o código do kernel
    DW 0xFFFF          ; __u16 limit;
    DW 0x0000          ; __u16 base_low; 
    DB 0x00            ; __u8 base_middle;
    DB 0b10011010      ; __u8 access;
    DB 0b11001111      ; __u8 flags;
    DB 0x00            ; __u8 base_high;

  kernel_data_segment: ; Aqui fica os dados do kernel, stack .bss .data
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
; Rotina CLEANF:
;
; Limpa a tela por completo e reseta sua cor para o padrão
;

CLEANF:

  ;
  ; Não tem nenhum parâmetro
  ;

  PUSH EAX
  PUSH EBX
  PUSH EDX

  MOV EDX, VGA_FRAMEBUFFER_ADDRS_MAP ; Ponteiro para o framebuffer do VGA
  XOR BX, BX                         ; Zerando BX, fazer essa operação é mais barata que MOV BX, 0x00

  .cmp:
  CMP BX, VGA_SCREEN_H * VGA_SCREEN_W * 2 ; Vendo se toda a tela foi limpa, usamos o * 2 pq temos 2 bytes para cada caractere, o primeiro sendo para o caractere e o segundo sendo para a cor
  JZ .exit
    
  MOV AL, 0x00          ; Caractere de limpeza '\0'
  MOV AH, DEFAULT_COLOR ; Cor padrão
  MOV [EDX], AX         ; Mandando para o framebuffer

  ;
  ; Incrementando para fazer a verificação em .cmp
  ;

  ADD EDX, 0x02
  ADD EBX, 0x02
  JMP .cmp
  
  .exit:
    POP EDX
    POP EBX
    POP EAX
    RET

;
; Rotina PRINTF
;
; Escreve no framebuffer do VGA
;

PRINTF:
  
  ;
  ; EBX: Ponteiro para a string
  ; AL : Cor do caractere e background
  ;      bits 0-3: Cor do caractere
  ;      bits 4-7: Cor de fundo do caractere
  ;

  PUSH EDX
  PUSH ECX

  MOV EDX, VGA_FRAMEBUFFER_ADDRS_MAP ; Ponteiro para o framebuffer do VGA 
  
  .cmp:
    CMP [EBX], byte 0x00 ; Verificando se a string acabou
    JZ .exit
      
    MOV CL, byte [EBX] 
    MOV [EDX], CL
    INC EDX
    MOV [EDX], AL
    INC EDX
    INC EBX
    JMP .cmp

  .exit:
    POP ECX
    POP EDX
    RET

;
; Rotina DISKERR
;

DISKERR:
  PUSH EAX
  PUSH EBX

  MOV AL, DEFAULT_COLOR
  MOV EBX, disk_error
  CALL PRINTF

  JMP $
    
  disk_error:
    DB "NEKONEST: READ DISK ERROR", 0x00

;
; Aqui já estamos no modo protegido 32 bits, já podemos usar os registradores
; eax, ebx, ecx e edx, a agora temos o endereçamento completo de sistemas 32 bits  
;

[BITS 32]
protected_mode:

  ;
  ; O modo protegido começa aqui
  ;

  MOV ESP, 0xFFFF ; Configurando uma stack temporária, ela vai ser substituida pelo kernel
  CALL CLEANF
  
  ;
  ; Para mais informações de como funciona a leitura e escrita de um disco CHS: https://wiki.osdev.org/ATA_read/write_sectors
  ;
  ; Lá você pode ver um código que explica bem como funciona a comunicação com o controlador ATA/SATA
  ;

  ;
  ; Rotina de leitura do setor do kernel
  ;

  MOV DX, DRIVE_HEAD ; Porta que recebe o drive e o cabeçote
  MOV AL, 0b00000000 ; O cabeçote é os 4 bits menos significativos
  OR AL, 0b10100000  ; Por default os 4 bits mais significativos são 1010 
  OUT DX, AL

  MOV DX, SECTOR_COUNT ; Porta de contagem de setores 
  MOV AL, 0x01         ; Quantos setores vamos ler
  OUT DX, AL

  MOV DX, SECTOR_NUMBER ; Porta do número do setor
  MOV AL, 0x02          ; Setor no qual vamos começar a leitura
  OUT DX, AL
    
  MOV DX, CYLINDER_LOW ; Porta cilindro baixo
  XOR AL, AL           ; Número do cilindro (0) (bits baixos)
  OUT DX, AL
  MOV DX, CYLINDER_HIGH ; Porta cilindro alto
  XOR AL, AL            ; Número do cilindro (0) (bits baixos)
  OUT DX, AL

  MOV DX, COMMAND_PORT ; Porta de comando
  MOV AL, 0x20         ; Comando de leitura
  OUT DX, AL   

  MOV DX, ERROR_PORT ; Verificando se houve algum erro
  IN AL, DX          ; Se AL voltar como 1 houve algum erro, com isso nós podemos ver a porta 0x1F7 para obter mais detalhes do error, não vamos fazer isso aqui
  CMP AL, 0x00
  JNZ DISKERR
   
  MOV DX, STATUS_PORT

  .still_going:
    IN AL, DX     ; Verificando se a leitura foi completa e se está disponivel no buffer no controlador
    TEST AL, 8    ; Fazendo operação AND para ver se o BITS DRQ está setado como 1
    JZ .still_going
   
  MOV EBX, 0x100000 ; Endereço de destino
  MOV DX, DATA_PORT 
   
  .read_sector_loop:  ; Loop de leitura de dados
    IN AX, DX         ; A cada leitura da porta de dados o controlador do disco incrementa ele automaticamente
    MOV [EBX], AX     ; Movendo a word (16 bits) para o endereço 0x100000
    ADD EBX, 0x02     ; Incrementando até ler as 256 words
    CMP EBX, 0x100100 ; Vendo se os 512 Bytes foram lidos
    JNZ .read_sector_loop

  MOV AL, DEFAULT_COLOR
  MOV EBX, boot
  CALL PRINTF
  
  JMP $
  
;
; Mensagens do NEKONEST :)
;

boot:
  DB "NEKONEST: BOOTING KERNEL...", 0x00


all_done:
  DB "NEKONEST: IS WORKING :D", 0x00

;
; Assinatura de Setor MBR bootável
;

MBR_sector_sig:
  TIMES 510 - ($ - $$) DB 0x00 ; Garantindo que o binário final tenha 512 bytes para a BIOS considerar como MBR bootável
  DW 0xAA55 ; Assinatura de setor MBR bootável válido
  
