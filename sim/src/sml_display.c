#include <signal.h>
#include <ctype.h>
#include <stdlib.h>

#include "ncurses.h"
#include "sml.h"

WINDOW *memwindow;
WINDOW *chipwindow;
WINDOW *messagewindow;
WINDOW *inputwindow;
WINDOW *outputwindow;

void term_resize() {
  endwin();
  refresh();
  erase();
  init_windows();
  refresh();
  updatescreen();
  displaymem();
  displayoutput();
  error_message(0,0,0);
}

void sig_int(int in) {
  sml->running = false;
  displaychip();
}

int init_windows() {
  int height, width;

  getmaxyx(stdscr, height, width);
  resize_out_buffer(height - 2);

  /*
   * These set the input handling up correctly
   */
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  cbreak();
  noecho();
  curs_set(0);

  if( height < MINHEIGHT || width < MINWIDTH ) {
    endwin();
    fprintf(stderr,"FATAL:\tScreen is not big enough for this sim.\n");
    fprintf(stderr,"\tWe need %i rows, and %i cols.\n",MINHEIGHT, MINWIDTH);
    exit(1);
  }

  /*
   * Define sizes of windows
   */
  chipwindow = newwin(4, width-10, 0, 0);
  messagewindow = newwin(5, width-10, 4, 0);
  memwindow = newwin(13, width-10, 9, 0);
  inputwindow = newwin(3, width-10, 22, 0);
  outputwindow = newwin(height, 10, 0, width - 10);

  return 0;
}

void updatescreen() {
  werase(chipwindow);
  werase(inputwindow);

  displaychip();

  wborder(chipwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  wborder(inputwindow, 0, 0, 0, 0, 0, 0, 0, 0);

  /*
   * Add titles to each window
   */
  mvwaddstr(chipwindow, 0, (getmaxx(chipwindow)-14)/2, "Simpletron CPU");
  mvwaddstr(inputwindow, 0, (getmaxx(inputwindow)-5)/2, "INPUT");

  if( sml->running ) {
    mvwprintw(inputwindow, 1, 2, "INPUT: %s_", userline);
  } else {
    mvwprintw(inputwindow, 1, 2, "COMMAND: %s_", userline);
  }

  wnoutrefresh(chipwindow);
  wnoutrefresh(inputwindow);
}

void displayoutput() {
  int i = 1;
  struct out_buffer *tmp = out_buffer_head;

  werase(outputwindow);
  wborder(outputwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  mvwaddstr(outputwindow, 0, (getmaxx(outputwindow)-6)/2, "OUTPUT");
  while(tmp) {
    mvwprintw(outputwindow, i, 2, "%4i", tmp->value);
    tmp = tmp->next;
    i++;
  }
  wnoutrefresh(outputwindow);
}

/*
 * We want to avoid calling this for every update loop.
 * Instead, we want to only update memory that changes.
 * This is not working on screen resizing. I don't know
 * why.
 */
void displaymem() {
  int i;
  werase(memwindow);
  mvwaddstr(memwindow, 1, 8, "00     01     02     03     04     05     06     07     08     09");
  for( i = 0; i < MEMSIZE; i++ ) {
    if( (i % 10) == 0 ) {
      mvwprintw(memwindow, i / 10 + 2, 1, "%02i:", i);
    }
    if(sml->memory[i] >= 0 ) {
      mvwprintw(memwindow, i / 10 + 2, (i%10)*7+6, "+%04i",sml->memory[i]);
    } else {
      mvwprintw(memwindow, i / 10 + 2, (i%10)*7+6, "-%04i",-1*sml->memory[i]);
    }
  }
  wborder(memwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  mvwaddstr(memwindow, 0, (getmaxx(memwindow)-17)/2, "Simpletron MEMORY");
  wnoutrefresh(memwindow);
}

/*
 * This will just display the memory address that has changed
 */
void update_mem_addr(int i) {
  if(sml->memory[i] >= 0 ) {
    mvwprintw(memwindow, i / 10 + 2, (i%10)*7+6, "+%04i",sml->memory[i]);
  } else {
    mvwprintw(memwindow, i / 10 + 2, (i%10)*7+6, "-%04i",-1*sml->memory[i]);
  }
  wnoutrefresh(memwindow);
}

void displaychip() {
  char *n = "DATA";
  mvwprintw(chipwindow, 1, 1, "instPtr: %02i", sml->iptr);
  if(sml->acc >= 0 ) {
    mvwprintw(chipwindow, 1, 20, "Accumulator: +%04i", sml->acc);
  } else {
    mvwprintw(chipwindow, 1, 20, "Accumulator: -%04i", -1*sml->acc);
  }
  if(sml->running == true || sml->stepping == true) {
    mvwprintw(chipwindow, 1, 40, "{RUNNING} type CTRL-C to halt");
  } else {
    mvwprintw(chipwindow, 1, 40, "{HALTED} type CTRL-G to run");
  }
  switch( sml->memory[ sml->iptr ] / OPFACT ) {
  case READ:
    n = "read";
    break;
  case WRITE:
    n = "write";
    break;
  case LOAD:
    n = "lda";
    break;
  case STORE:
    n = "sta";
    break;
  case ADD:
    n = "add";
    break;
  case SUBTRACT:
    n = "sub";
    break;
  case DIVIDE:
    n = "div";
    break;
  case MULTIPLY:
    n = "mult";
    break;
  case MOD:
    n = "mod";
    break;
  case INC:
    n = "inc";
    break;
  case DEC:
    n = "dec";
    break;
  case BRANCH:
    n = "jump";
    break;
  case BRANCHNEG:
    n = "jneg";
    break;
  case BRANCHZERO:
    n = "jzero";
    break;
  case HALT:
    n = "halt";
    break;
  default:
    mvwprintw(chipwindow,2,1, "Instruction: %s %04i",n,sml->memory[sml->iptr]);
    n = 0;
    break;
  }
  if( n != 0 ) {
    mvwprintw(chipwindow, 2, 1, "Instruction: %s %02i",
	      n, sml->memory[sml->iptr] % OPFACT);
  }
}

void error_message(char *line1, char *line2, char *line3) {
  werase(messagewindow);
  wborder(messagewindow, 0, 0, 0, 0, 0, 0, 0, 0);
  mvwaddstr(messagewindow, 0, (getmaxx(messagewindow)-8)/2, "MESSAGES");
  if( line1 ) {
    mvwaddstr(messagewindow, 1, 2, line1);
  }
  if( line2 ) {
    mvwaddstr(messagewindow, 2, 2, line2);
  }
  if( line3 ) {
    mvwaddstr(messagewindow, 3, 2, line3);
  }
  wnoutrefresh(messagewindow);
}
