#include <signal.h>

#include "ncurses.h"
#include "sml.h"
#include "sml_display.h"

WINDOW *memwindow;
WINDOW *chipwindow;
WINDOW *messagewindow;
WINDOW *inputwindow;
WINDOW *outputwindow;

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
