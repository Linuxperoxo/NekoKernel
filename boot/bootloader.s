;
;
;
;    /--------------------------------------------O
;    |                                            |
;    |  COPYRIGHT : (c) 2024 per Linuxperoxo.     |
;    |  AUTHOR    : Linuxperoxo                   |
;    |  FILE      : bootloader.s                  |
;    |  SRC MOD   : 01/12/2024                    |
;    |                                            |
;    O--------------------------------------------/
;    
;
;

BITS 16
ORG 0x7C00

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
; 3. Dentro do cilindro, o cabeçote acessa a trilha correta.
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

%define DEFAULT_COLOR 0x0F ; Caractere branco com fundo preto
%define VGA_SCREEN_SIZE 80*25*2
%define VGA_FRAMEBUFFER_BASE 0xB800

%macro CLEAN_SCREEN 0
  MOV AX, VGA_FRAMEBUFFER_BASE
  MOV ES, AX
  XOR DI, DI

  .loopcmp:
    CMP DI, VGA_SCREEN_SIZE
    JZ .loopexit 
  
  .loopexec:
    MOV [ES:DI], byte 0x00
    MOV [ES:DI + 1], byte DEFAULT_COLOR
    ADD DI, 2
    JMP .loopcmp

  .loopexit:
%endmacro

SECTION .text
  
  CLEAN_SCREEN

  ;
  ; Aqui vamos começar a carregar o kernel.
  ;
  ; Primeiramente temos que carregar o binário do kernel
  ; na memória, então usamos uma interrupção de BIOS para
  ; carregar o segundo setor do disco, que é onde está o
  ; nosso loader.s do nosso kernel
  ;

  ;
  ; Se você não estiver acostumado com "funções" em assembly
  ; vou tentar explicar ao máximo.
  ;
  ; Normalmente parâmetros em assembly pode ser passadas de 
  ; duas formas diferentes:
  ;
  ; - Stack: Normalmente quando temos um OS rodando usamos a
  ;          stack para parâmetros.
  ;
  ; - Registradores: Quando não temos a certeza que um OS vai
  ;                  configurar uma stack para o programa, usamos
  ;                  os registradores como parâmetros. A BIOS sempre
  ;                  vai usar os registradores, então qualquer parâmetro
  ;                  vai ser usar sempre determinados registradores para
  ;                  fazer a função funcionar do jeito que voce espera.
  ;

  MOV AX, 0x1000 
  
  ; 
  ; ES é o endereço base de onde vamos carregar,
  ; os dados lidos do disco na RAM, ele vai ser
  ; usado juntamente com o registrador BX para 
  ; definir um endereço da RAM.
  ;
  ; EXEMPLO:
  ;   MOV [ES:BX], READ_DATA_DISK
  ;
  ; Nesse caso eu tenho que fazer 1 instrução
  ; para carregar o valor em ES.
  ;
  ; MOV AX, 0x1000
  ; MOV ES, AX
  ;
  ; Isso ocorre porque não temos um opcode válido
  ; para MOV ES, 0x1000.
  ;
  ; Logo depois de configurar o ES, vamos configurar
  ; o BX. O BX vai ser o offset do endereço ES, use
  ; o seguinte calculo para entender o que [ES:BX] faz:
  ;
  ; [ES:BX] = (ES * 16) + BX = Endereço físico
  ;

  MOV ES, AX
  XOR BX, BX

  MOV AH, 0x02 ; Definindo que operação vamos fazer com o disco, 0x02 para Read Sectors From Disk
  MOV AL, 0x01 ; Quantos setores vamos ler
  MOV CH, 0x00 ; Cilíndro de leitura
  MOV CL, 0x02 ; Setor de leitura
  MOV DH, 0x00 ; Cabeçote de leitura
  MOV DL, 0x80 ; Disco a ser lido, 0x80 é o primeiro disco
  
  ;
  ; Isso seria entre muitas "" uma chamada de função
  ; para gerenciar um disco, essa função vai se moldar
  ; com base nos parâmetros que passamos pelos registradores
  ;

  INT 0x13

  ;
  ; Para mais informações acesse: https://en.wikipedia.org/wiki/INT_13H
  ;

  ;
  ; OBS: Retorno de funções também podem ser passadas tanto
  ;      por stack quanto por registradores. 
  ;

  ;
  ; Essa parte serve para ver se ocorreu tudo bem
  ; na operação anterior, o retorno dela será pelo
  ; registrador AH, leia o link passado anteriormente
  ; para obter mais detalhes sobre todos os retornos 
  ; possíveis
  ;

  MOV AH, 0x01
  MOV DL, 0x80
  INT 0x13
  
  CMP AH, 0
  JZ .jmpk

  .errormsg:
    MOV AX, 0xB800
    MOV ES, AX
    
    MOV [ES:0x00], byte 'E'
    MOV [ES:0x02], byte 'R'
    MOV [ES:0x04], byte 'R'
    MOV [ES:0x06], byte 'O'
    MOV [ES:0x08], byte 'R'
    MOV [ES:0x0A], byte ' '
    MOV [ES:0x0C], byte 'T'
    MOV [ES:0x0E], byte 'O'
    MOV [ES:0x10], byte ' '
    MOV [ES:0x12], byte 'L'
    MOV [ES:0x14], byte 'O'
    MOV [ES:0x16], byte 'A'
    MOV [ES:0x18], byte 'D'
    MOV [ES:0x1A], byte ' '
    MOV [ES:0x1C], byte 'K'
    MOV [ES:0x1E], byte 'E'
    MOV [ES:0x20], byte 'R'
    MOV [ES:0x22], byte 'N'
    MOV [ES:0x24], byte 'E'
    MOV [ES:0x26], byte 'L'
    MOV [ES:0x28], byte ' '
    MOV [ES:0x2A], byte 'I'
    MOV [ES:0x2C], byte 'N'
    MOV [ES:0x2E], byte ' '
    MOV [ES:0x30], byte '0'
    MOV [ES:0x32], byte 'x'
    MOV [ES:0x34], byte '1'
    MOV [ES:0x36], byte '0'
    MOV [ES:0x38], byte '0'
    MOV [ES:0x3A], byte '0'
    MOV [ES:0x3C], byte '0'
  
  .errorloop:
    JMP .errorloop

  ;
  ; Como carregamos o kernel no endereço físico 0x10000,
  ; vamos passar o controle para esse endereço.
  ;
  ; Esse endereço é onde começa nossa label k_loader
  ; que é responsável por carregar tudo antes de passar
  ; o controle para o kernel
  ;

  .jmpk:
    JMP 0x1000:0x00

TIMES 510 - ($ - $$) DB 0x00 ; Garantindo que o binário tenha 512 bytes como explicado acima
DW 0xAA55 ; Assinatura de setor de boot válido
