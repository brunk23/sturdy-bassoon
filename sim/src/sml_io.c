#include <signal.h>
#include <ctype.h>
#include <stdlib.h>

#include "ncurses.h"
#include "sml.h"
#include "sml_io.h"

WINDOW *memwindow;
WINDOW *chipwindow;
WINDOW *messagewindow;
WINDOW *inputwindow;
WINDOW *outputwindow;

struct out_buffer *out_buffer_head = 0;
struct out_buffer *out_buffer_tail = 0;
int out_buffer_max_length = 0;

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

  buffptr = 0;
  for( i = 0; i < BUFFSIZE; i++ ) {
    input = 0;
    negative = false;

    if( line[i] == 0 || line[i] == '#' ) {
      return;
    }

    if( line[i] == ' ' || line[i] == '\t' ) {
      continue;
    }

    if( line[i] == '-' ) {
      negative = true;
      i++;
    }

    while( isdigit(line[i]) ) {
      input *= 10;
      input += line[i] - '0';
      i++;
    }

    if( !out_of_bounds(input, 0, MAXVAL) ) {
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

void sig_int(int in) {
  sml->running = false;
  displaychip();
}

int init_windows() {
  int height, width;

  getmaxyx(stdscr, height, width);
  resize_out_buffer(height - 2);

  if( height < MINHEIGHT || width < MINWIDTH ) {
    endwin();
    fprintf(stderr,"FATAL: INSUFFICIENT SCREEN SIZE\n");
    abort();
  }

  /*
   * These set the input handling up correctly
   */
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  cbreak();
  noecho();

  /*
   * Define sizes of windows
   */
  chipwindow = newwin(3, width-10, 0, 0);
  messagewindow = newwin(5, width-10, 3, 0);
  memwindow = newwin(13, width-10, 8, 0);
  inputwindow = newwin(3, width-10, 21, 0);
  outputwindow = newwin(height, 10, 0, width - 10);

  /*
   * They get borders
   */
  wborder(chipwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  wborder(outputwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  wborder(messagewindow, 0, 0, 0, 0, 0, 0, 0, 0);
  wborder(memwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  wborder(inputwindow, 0, 0, 0, 0, 0, 0, 0, 0);

  /*
   * Add titles to each window
   */
  mvwaddstr(chipwindow, 0, width/2 - 10, "Simpletron");
  mvwaddstr(outputwindow, 0, 2, "OUTPUT");
  mvwaddstr(memwindow, 0, width/2 - 8, "MEMORY");
  mvwaddstr(inputwindow, 0, width/2 - 7, "INPUT");
  mvwaddstr(messagewindow, 0, width/2 - 10, "Messages");

  /*
   * Draw them on the screen
   */
  wmove(inputwindow,1,1);
  wrefresh(chipwindow);
  wrefresh(outputwindow);
  wrefresh(memwindow);
  wrefresh(inputwindow);
  wrefresh(messagewindow);

  return 0;
}

void updatescreen() {
  displaymem();
  displaychip();
  displayoutput();
  werase(inputwindow);
  wborder(inputwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  mvwprintw(inputwindow, 1, 2, "%s", line);
 wrefresh(inputwindow);
}

void displayoutput() {
  int i = 1;
  struct out_buffer *tmp = out_buffer_head;
  werase(outputwindow);
  wborder(outputwindow, 0, 0, 0, 0, 0, 0, 0, 0);

  while(tmp) {
    mvwprintw(outputwindow, i, 2, "%4i", tmp->value);
    tmp = tmp->next;
    i++;
  }
  wrefresh(outputwindow);
}

void displaymem() {
  int i;
  werase(memwindow);
  wborder(memwindow, 0, 0, 0, 0, 0, 0, 0, 0);
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
  werase(chipwindow);
  wborder(chipwindow, 0, 0, 0, 0, 0, 0, 0, 0);
  mvwprintw(chipwindow, 1, 1, "instPtr: %02i", sml->counter);
  if(sml->accumulator >= 0 ) {
    mvwprintw(chipwindow, 1, 20, "Accumulator: +%04i", sml->accumulator);
  } else {
    mvwprintw(chipwindow, 1, 20, "Accumulator: -%04i", -1*sml->accumulator);
  }
  if(sml->running == true) {
    mvwprintw(chipwindow, 1, 40, "RUNNING: type CTRL-C to halt");
  } else {
    mvwprintw(chipwindow, 1, 40, "HALTED: type CTRL-G to run");
  }
  wrefresh(chipwindow);
}

/*
 * As items come in, they are added to the tail.
 * If if list is larger than the max length, throw out
 * first value.
 * Abort if malloc fails.
 */
void output_value(int value) {
  int curr_length;
  struct out_buffer *tmp;

  if( out_buffer_max_length == 0 ) {
    endwin();
    fprintf(stderr,"FATAL: we got no lines man, no lines!\n");
    abort();
  }

  curr_length = out_buff_len();
  if( curr_length == out_buffer_max_length ) {
    tmp = out_buffer_head;
    out_buffer_head = out_buffer_head->next;
    free(tmp);
  }

  if( 0 == (tmp = malloc(sizeof(struct out_buffer)))) {
    endwin();
    fprintf(stderr,"FATAL: malloc failed to get memory!\n");
    abort();
  }

  tmp->value = value;
  tmp->next = 0;

  if( out_buffer_head == 0 ) {
    out_buffer_head = tmp;
    out_buffer_tail = tmp;
    return;
  }

  out_buffer_tail->next = tmp;
  out_buffer_tail = tmp;
}

/*
 * This is only called when the program starts and on
 * resize events
 */
void resize_out_buffer(int max) {
  int i, curr_length = 0;
  struct out_buffer *tmp;

  curr_length = out_buff_len();
  out_buffer_max_length = max;

  if( curr_length > max ) {
    for(i = 0; i < (curr_length - max); i++) {
      tmp = out_buffer_head;
      out_buffer_head = out_buffer_head->next;
      free(tmp);
    }
  }
}

int out_buff_len() {
  int curr_length = 0;
  struct out_buffer *tmp = out_buffer_head;

  while(tmp) {
    curr_length++;
    tmp = tmp->next;
  }

  return curr_length;
}
