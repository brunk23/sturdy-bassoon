#include <stdlib.h>
#include <ctype.h>

#include "sml.h"

struct out_buffer *out_buffer_head;
struct out_buffer *out_buffer_tail;
int out_buffer_max_length;

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

/*
 * As items come in, they are added to the tail.
 * If if list is larger than the max length, throw out
 * first value.
 * exit if malloc fails.
 */
void output_value(int value) {
  int curr_length;
  struct out_buffer *tmp;

  if( out_buffer_max_length == 0 ) {
    exit(1);
  }

  curr_length = out_buff_len();
  if( curr_length == out_buffer_max_length ) {
    tmp = out_buffer_head;
    out_buffer_head = out_buffer_head->next;
    free(tmp);
  }

  if( 0 == (tmp = (struct out_buffer *)malloc(sizeof(struct out_buffer)))) {
    exit(1);
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

/*
 * This will look for commands to assemble or
 * execute and will return a token (either a
 * command that can be assembled or a command
 * that will control the simulator).
 */
int token(char *str) {
  int val = INVALID;

  return val;
}
