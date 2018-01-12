#include <ctype.h>

#include "sml.h"

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
 * This will look for commands to assemble or
 * execute and will return a token (either a
 * command that can be assembled or a command
 * that will control the simulator).
 */
int token(char *str) {
  int val = INVALID;

  return val;
}
