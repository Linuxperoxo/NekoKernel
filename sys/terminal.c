/*
 * 
 *
 *    /--------------------------------------------o
 *    |                                            |
 *    |  copyright : (c) 2024 per linuxperoxo.     |
 *    |  author    : linuxperoxo                   |
 *    |  file      : terminal.c                    |
 *    |  src mod   : 14/01/2025                    | 
 *    |                                            |
 *    o--------------------------------------------/
 *    
 *
 */

#include <terminal.h>
#include <std/int.h>
#include <std/utils.h>
#include <std/io.h>
#include <video/vga/vga.h>
#include <sys/ports.h>
#include <device/io/keyboard/keyboard.h>

/*
 *
 * Macros
 *
 */

#define TERMINAL __current_terminal
#define TERMINAL_OUT_OFFSET TERMINAL->__out_offset
#define TERMINAL_IN_OFFSET TERMINAL->__in_offset
#define TERMINAL_VGA TERMINAL->__vga_state
#define TERMINAL_KB TERMINAL->__keyboard

static struct Terminal* __current_terminal;  

/*
 *
 * ========================
 *
 *  Internal Functions
 *
 * ========================
 *
 */

__attribute__((always_inline)) inline void terminal_put_char(const char __ch__)
{
  vga_put_char(&TERMINAL_VGA, __ch__);
}

__attribute__((always_inline)) inline void terminal_mov_ptr(const __u8 __row__, const __u8 __col__)
{ 
  vga_mov_ptr(__row__, __col__);
}

/*
 *
 * Essa função é responsável por manipular os inputs do teclado no terminal
 *
 */

void terminal_keyboard_handler(const __u8 __key_code__)
{
  if(KEY_IS_PRESS)
  {
    switch(__key_code__)
    {
      case KEY_ENTER:

        /*
         *
         * Caso a tecla pressionada seja Enter, vamos sinalizar que o buffer está pronto
         * e quebrar a linha
         *
         */

        TERMINAL_IN_OFFSET = 0x00; // Limpando o buffer
        TERMINAL->__flags |= 0x10; // Levantando a flag que representa que o buffer está pronto 

        terminal_out('\n'); // Quebrando a linha
      break;

      case KEY_BACK:

        /*
         *
         * Aqui fazemos uma lógica para apagar apenas caracteres de input
         *
         */

        if(TERMINAL_IN_OFFSET > 0x00)
        {
          if(TERMINAL_VGA.__current_col > 0x00)
          {
            TERMINAL_VGA.__current_col -= 1;
          }
          else
          {
            if(TERMINAL_VGA.__current_row > 0x00)
            {
              TERMINAL_VGA.__current_col  = DEFAULT_WIDTH - 1; // Como __current_col é um array devemos usando o '- 1', já que seu índice começa do 0
              TERMINAL_VGA.__current_row -= 1;
            }
          }
          
          terminal_out('\0'); // Imprimindo caractere nulo na tela
          
          TERMINAL_VGA.__current_col                   -= 1; // Voltando uma coluna já que o caractere foi apagado, eixo X
          TERMINAL_OUT_OFFSET                          -= 1; // Como apagamos um out temos que decrementar o offset out
          TERMINAL->__in_buffer[--TERMINAL_IN_OFFSET]   = 0x00; // Aqui limpamos o caractere de input, isso serve para que o caractere deixe de existir em funções como scanf

          terminal_mov_ptr(TERMINAL_VGA.__current_row, TERMINAL_VGA.__current_col); // Atualizando o ponteiro para o novo lugar
        }
      break;
      
      default:
        terminal_in(TERMINAL_KB.__char);
      break;
    }
  }
}


/*
 *
 * ==========================
 *
 *  Functions terminal.h
 *
 * ==========================
 *
 */

void terminal_init()
{

  /*
   *
   * Configurando struct terminal
   *
   */

  TERMINAL = (struct Terminal*)0xFFFFFF;
  
  TERMINAL->__in_offset  = 0x00;
  TERMINAL->__out_offset = 0x00;
  TERMINAL->__flags      = 0x00;

  /*
   *
   * Configurando struct VGA linkada ao terminal
   *
   */

  TERMINAL_VGA.__framebuffer = (void*)VGA_FRAMEBUFFER_ADDRS;
  TERMINAL_VGA.__current_row = 0x00;
  TERMINAL_VGA.__current_col = 0x00;
  TERMINAL_VGA.__bc_color    = BLACK_BC_COLOR;
  TERMINAL_VGA.__ch_color    = WHITE_CHAR_COLOR;
  TERMINAL_VGA.__last_put    = 0x00;

  /*
   *
   * Configurando struct Keyboard linkada ao terminal
   *
   */

  TERMINAL_KB.__scan = 0x00;
  TERMINAL_KB.__code = 0x00;
  TERMINAL_KB.__char = 0x00;

  /*
   *
   * Esse bit habilita um bit que faz com que uma determinada função linkada ao teclado
   * seja chamada a cada interrupção do teclado
   *
   */

  TERMINAL_KB.__flags = 0x08;

  /*
   *
   * Essa é a função que será linkada ao teclado
   *
   */

  TERMINAL_KB.__func_key_handler = &terminal_keyboard_handler; 

  /*
   *
   * Essa função serve para linkar um teclado ao driver, isso vai servir para fazer
   * várias seções para vários usuários do sistema
   *
   */

  keyboard_switch(&TERMINAL_KB);
}

void terminal_out(const char __ch__)
{
  TERMINAL_OUT_OFFSET = TERMINAL_OUT_OFFSET >= MAX_OUT_BUFFER_SIZE ? 0 : TERMINAL_OUT_OFFSET;
  TERMINAL->__out_buffer[TERMINAL_OUT_OFFSET] = __ch__;
  
  switch(TERMINAL->__out_buffer[TERMINAL_OUT_OFFSET])
  {
    case '\n':    
      TERMINAL_VGA.__current_col  = 0x00;
      TERMINAL_VGA.__current_row += 0x01;

      ++TERMINAL_OUT_OFFSET;
      
      if(TERMINAL_VGA.__current_row >= DEFAULT_HEIGHT)
      {
        vga_screen_down(&TERMINAL_VGA);  

        TERMINAL_VGA.__current_col = 0x00;
        TERMINAL_VGA.__current_row = DEFAULT_HEIGHT - 1;
      }
      
      terminal_mov_ptr(TERMINAL_VGA.__current_row, TERMINAL_VGA.__current_col);
      return;
    break;

    case '\r': 
      TERMINAL_VGA.__current_col = 0x00;

      ++TERMINAL_OUT_OFFSET;

      terminal_mov_ptr(TERMINAL_VGA.__current_row, TERMINAL_VGA.__current_col);
      return;
    break;

    default: 
      if(TERMINAL_VGA.__current_col >= DEFAULT_WIDTH)
      {
        TERMINAL_VGA.__current_col  = 0x00;
        TERMINAL_VGA.__current_row += 0x01;
      }
      
      if(TERMINAL_VGA.__current_row >= DEFAULT_HEIGHT)
      {
        vga_screen_down(&TERMINAL_VGA);  
        
        TERMINAL_VGA.__current_col = 0x00;
        TERMINAL_VGA.__current_row = DEFAULT_HEIGHT - 1;
      }

      terminal_put_char(TERMINAL->__out_buffer[TERMINAL_OUT_OFFSET++]);
      terminal_mov_ptr(TERMINAL_VGA.__current_row, TERMINAL_VGA.__current_col);
    break;
  }  
}

void terminal_in(const char __ch__)
{
  TERMINAL_IN_OFFSET = TERMINAL_IN_OFFSET >= MAX_IN_BUFFER_SIZE ? 0 : TERMINAL_IN_OFFSET; 

  if(TERMINAL_BUFFER_IS_READY)
    memset(TERMINAL->__in_buffer, 0x00, MAX_IN_BUFFER_SIZE);  

  TERMINAL->__in_buffer[TERMINAL_IN_OFFSET++] = KEY_IS_VISIBLE ? TERMINAL_KB.__char : TERMINAL_KB.__code;
  TERMINAL->__flags                          &= 0xEF; // Desabilitando a flag de buffer ready

  if(KEY_IS_VISIBLE && !TERMINAL_INV_INPUT)
    terminal_out(__ch__);
}

void terminal_cln_flag()
{
  TERMINAL->__flags = 0x00;
}

void terminal_set_flag(const __u8 __new_flag__)
{
  TERMINAL->__flags &= ((__new_flag__ << 4) & 0xF0);   
}

__u8 terminal_sts_flag()
{
  return TERMINAL->__flags;
}

void terminal_cpy_in(void* __dest__, __u16 __size__)
{
  memcpy(__dest__, &TERMINAL->__in_buffer, __size__);
}
