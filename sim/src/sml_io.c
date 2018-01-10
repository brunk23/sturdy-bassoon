#include <signal.h>
#include <ctype.h>

#include "ncurses.h"
#include "sml.h"
#include "sml_io.h"

WINDOW *memwindow;
WINDOW *chipwindow;
WINDOW *messagewindow;
WINDOW *inputwindow;
WINDOW *outputwindow;

char line[BUFFSIZE + 1];
int buffptr = 0;

/*
 * This will process commands and all input. Numbers
 * will be added directly to memory where readptr is
 * pointing.
 *
 * This will eventually compile very simple
 * assembly. Ex. read ## -> 10##
 *
 * This will also take commands that effect the machine
 * state.
 * "go" (which sets the counter to 0 and runs)
 * "continue" (which runs without changing counter)
 * "step" (sets counter to 0 and stops after each step)
 * "break ##" (sets a breakpoint when counter hits ##) 
 */
void process() {
  int input;
  unsigned int i;
  bool negative = false;

  if( !( (line[0] == '-') || isdigit(line[0]))) {
    return;
  }

  for( i = 0; i < BUFFSIZE; i++ ) {
    input = 0;
    negative = false;

    if( line[i] == 0 || line[i] == '#' ) {
      buffptr = 0;
      return;
    }

    if( line[i] == '-' ) {
      negative = true;
      i++;
    }

    while( isdigit(line[i]) ) {
      input *= 10;
      input += line[i] - '0';
    }

    if( !out_of_bounds(input,MINVAL,MAXVAL) ) {
      if( negative ) {
	input *= -1;
      }
      sml->memory[sml->counter++] = input;
    }
  }
}

void sig_winch(int in) {
  endwin();
  refresh();
  initscr();
  init_windows();
  displaymem();
  displaychip();
}

int init_windows() {
  int height, width;

  getmaxyx(stdscr, height, width);
  nodelay(stdscr, TRUE);
  noecho();
  chipwindow = newwin(3, width-10, 0, 0);
  messagewindow = newwin(5, width-10, 3, 0);
  memwindow = newwin(13, width-10, 8, 0);
  inputwindow = newwin(3, width-10, 21, 0);
  outputwindow = newwin(height, 10, 0, width - 10);
  wborder(chipwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  wborder(outputwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  wborder(messagewindow, 0, 0, 0, 0, 0, 0, 0, 0);
  wborder(memwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  wborder(inputwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  mvwaddstr(chipwindow, 0, width/2 - 10, "Simpletron");
  mvwaddstr(outputwindow, 0, 2, "OUTPUT");
  mvwaddstr(memwindow, 0, width/2 - 8, "MEMORY");
  mvwaddstr(inputwindow, 0, width/2 - 7, "INPUT");
  mvwaddstr(messagewindow, 0, width/2 - 10, "Messages");
  wmove(inputwindow,1,1);
  wrefresh(chipwindow);
  wrefresh(outputwindow);
  wrefresh(memwindow);
  wrefresh(inputwindow);
  wrefresh(messagewindow);
  return 0;
}

void displaymem() {
  int i;
  mvwprintw(memwindow, 1, 8, "00     01     02     03     04     05     06     07     08     09");
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
  wrefresh(memwindow);
}

void displaychip() {
  mvwprintw(chipwindow, 1, 1, "instPtr: %02i", sml->counter);
  if(sml->accumulator >= 0 ) {
    mvwprintw(chipwindow, 1, 20, "Accumulator: +%04i", sml->accumulator);
  } else {
    mvwprintw(chipwindow, 1, 20, "Accumulator: -%04i", -1*sml->accumulator);
  }
  wrefresh(chipwindow);
}
