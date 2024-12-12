/===========================================================================================O
|                                                                                           |
| ** O que é o NekoKernel?                                                                  |
|                                                                                           |
| O NekoKernel é um projeto que eu estava a muito tempo querendo fazer, um                  |
| kernel do 0, lógico que ele não vai ser um linux, mas a ideia é aprender                  |
| os fundamentos e como funciona um sistema operacional. O NekoKernel tem                   |
| seu próprio bootloader, o NekoNest. No começo, eu não iria fazer um bootloader,           |
| mas a curiosidade de entender como ele funciona não deixou isso acontecer :),             |
| então eu decidi que iria estudar mais sobre e tentar fazer o meu do 0, e finalmente       |
| consegui fazer um driver para um controlador ATA que vai carregar a image do kernel,      |
| ele também carrega uma GDT temporária, apenas para mudar do modo real para o modo         |
| protegido, também tive que fazer uma stack temporária para chamadas de funções do próprio |
| bootloader. Contudo, esse projeto é meu queridinho, estou colocando muito amor nele, quem |
| sabe ele seja um futuro linux :D.                                                         |
|                                                                                           |
| ** Funcionalidades:                                                                       |
|                                                                                           |
| # O NekoKernel, atualmente, conta com algumas funcionalidades como:                       |
|                                                                                           |
| * GDT completo, com 5 segmentos, Null Seg, Kernel Code, Kernel Data,                      |
|   User Code e User Data;                                                                  |
| * Driver para o Vga;                                                                      |                          
| * Driver para controlador ATA CHS;                                                        |
| * Lib std.                                                                                |
|                                                                                           |
| # Pretendo implementar várias outras coisas, Algumas delas são:                           |
|                                                                                           |
| * TSS;                                                                                    |
| * IDT;                                                                                    |
| * Driver para teclado;                                                                    |
| * Shell;                                                                                  |
| * File System;                                                                            |
| * Rodar programas.                                                                        |
|                                                                                           |
|                                                                                           |
| ** Compilação:                                                                            |
|                                                                                           |
| # Para compilar e executar o projeto você vai precisar de:                                |
|                                                                                           |
| * QEMU i386;                                                                              |
| * NASM;                                                                                   |
| * GCC;                                                                                    |
|                                                                                           |
| # Se você já tem tudo, pode executar os comandos abaixo:                                  |
|                                                                                           |
| * git clone https://github.com/Linuxperoxo/NekoKernel                                     |
| * cd ./NekoKernel                                                                         |
| * make clean                                                                              |
| * make kernel                                                                             |
| * make bootloader                                                                         |
| * make image                                                                              |
| * make run                                                                                |
|                                                                                           |
O===========================================================================================/
