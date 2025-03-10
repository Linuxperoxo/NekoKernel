=============================================================================================
NekoKernel

O que é o NekoKernel?

O NekoKernel é um projeto pessoal que eu sempre quis realizar: a criação de um kernel do zero. 
Claro que ele não será um sistema operacional completo como o Linux, mas o objetivo principal é 
aprender os fundamentos de como um sistema operacional funciona.

O NekoKernel vem acompanhado do seu próprio bootloader, o NekoNest. Inicialmente, eu não planejava 
desenvolver um bootloader, mas a curiosidade sobre como ele funciona me levou a estudar e criar o 
meu próprio. Durante esse processo, aprendi a implementar um driver para um controlador ATA, que carrega 
a imagem do kernel e também uma GDT temporária, essencial para realizar a transição do modo real para o 
modo protegido. Para isso, também foi necessário desenvolver uma stack temporária para as chamadas de funções 
do bootloader.

Esse projeto é muito especial para mim, e estou dedicando muito tempo e amor a ele. Quem sabe, no futuro, o 
NekoKernel não se torne algo maior, como uma versão própria de um sistema operacional baseado em Linux! :D

Funcionalidades Atuais

Atualmente, o NekoKernel oferece algumas funcionalidades, incluindo:

- GDT Completa, com 3 segmentos:
    - Null Segment
    - Kernel Code
    - Kernel Data
- Driver para VGA (para exibição de vídeo)
- Driver para Controlador ATA (CHS)
- Biblioteca Padrão (Lib std)
- Driver para teclado
- Terminal
- Multitask
- IDT
- Virtual File System
- Syscall 0x80 0xB1

Funcionalidades Futuras

Estou planejando implementar várias outras funcionalidades no NekoKernel, incluindo:

- Sistema de arquivos (File System)
- Execução de programas
- Melhoria geral no código
- Paging
- User mode
- Shell

Compilação

Para compilar e executar o NekoKernel, você precisará dos seguintes requisitos:

- QEMU (i386)
- NASM (Assembler)
- GCC (Compilador)

Passos para compilar

1. Clone o repositório:
    git clone https://github.com/Linuxperoxo/NekoKernel

2. Acesse a pasta do projeto:
    cd ./NekoKernel

3. Compile o kernel:
    make clean
    make kernel

4. Compile o bootloader:
    make bootloader

5. Crie a imagem do sistema:
    make image

6. Execute o kernel:
    make run

Para Debug

Se você deseja realizar debug, você vai precisar do GDB instalado. Para isso, siga os comandos 
abaixo:

1. Compile para debug:
    make debug

2. Inicie o GDB:
    make gdb

3. Conecte-se ao QEMU:
  (gdb) target remote localhost:1234

Contribuições

Este é um projeto em desenvolvimento e todo feedback é bem-vindo! Se você quiser contribuir 
ou tem sugestões, fique à vontade para abrir uma issue ou pull request no repositório.
=============================================================================================

