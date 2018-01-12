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
  char str[BUFFSIZE+1];
  int value, state = BLANK;
  unsigned int lineptr, strptr = 0;
  bool negative = false;

  buffptr = 0;
  for( lineptr = 0; lineptr < BUFFSIZE; lineptr++ ) {
    value = 0;
    negative = false;

    if( line[lineptr] == 0 || line[lineptr] == '#' ) {
      return;
    }

    if( line[lineptr] == ' ' || line[lineptr] == '\t' ) {
      continue;
    }

    if( line[lineptr] == '-' ) {
      negative = true;
      lineptr++;
    }

    while( isdigit(line[lineptr]) ) {
      value *= 10;
      value += line[lineptr] - '0';
      lineptr++;
    }

    if( !out_of_bounds(value, 0, MAXVAL) ) {
      if( negative ) {
	value *= -1;
      }
      sml->memory[sml->counter++] = value;
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

/*
 * These are the characters allowed into the linebuffer
 */
bool allowedchar(int a) {
  if( isdigit(a) ) {
    return true;
  }
  if( isalpha(a) ) {
    return true;
  }
  switch( a ) {
  case '-':
  case '+':
  case '@':
  case ' ':
  case '\t':
    return true;
  default:
    break;
  }
  return false;
}
