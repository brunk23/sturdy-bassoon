#include <ctype.h>
#include <stdlib.h>

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
 * "go" (which sets the iptr to 0 and runs)
 * "continue" (which runs without changing iptr)
 * "step" (sets iptr to 0 and stops after each step)
 * "break ##" (sets a breakpoint when iptr hits ##)
 */
void process() {
  char str[BUFFSIZE+1];
  int value, state = BLANK;
  unsigned int lineptr, strptr = 0;
  bool negative = false;

  buffptr = 0;
  for( lineptr = 0; lineptr < BUFFSIZE; lineptr++ ) {

    if( line[lineptr] == ' ' || line[lineptr] == '\t' ) {
      if( BLANK == state ) {
	continue;
      }
      if( NUMBER == state ) {

	state = BLANK;
	continue;
      }
      if( ALPHA == state ) {
	str[strptr] = 0;

	continue;
      }
      if( ADDRESS == state ) {

	state = BLANK;
	continue;
      }
      continue;
    }

    if( isalpha( line[lineptr] ) ) {
      if( BLANK == state ) {
	strptr = 0;
	str[strptr] = line[lineptr];
	strptr++;
	state = ALPHA;
	continue;
      }
      if( ALPHA == state ) {
	str[strptr] = line[lineptr];
	strptr++;

	if(strptr > BUFFSIZE) {
	  /*
	   * This should not be possible.
	   */
	  endwin();
	  fprintf(stderr, "FATAL: Buffer overrun processing string.\n");
	  exit(1);
	}
	continue;
      }
      if( NUMBER == state ) {
	error_message("letters inside numbers!");
      }
      if( ADDRESS == state ) {
	error_message("can't go to storybook land.");
      }
      state = BLANK;
      continue;
    }

    if( isdigit( line[lineptr] ) ) {
      if( BLANK == state ) {
	negative = false;
	state = NUMBER;
	value = line[lineptr] - '0';
	state = NUMBER;
	continue;
      }
      if( NUMBER == state ) {
	value *= 10;
	value += line[lineptr] - '0';
	continue;
      }
      if( ADDRESS == state ) {
	value *= 10;
	value += line[lineptr] - '0';
	continue;
      }
      if( ALPHA == state ) {
	error_message("numbers don't belong in words");
      }
      state = BLANK;
      continue;
    }

    /* Process last state and then leave */
    if( line[lineptr] == 0 || line[lineptr] == '#' ) {
      if( NUMBER == state ) {

	return;
      }
      if( ALPHA == state ) {

	return;
      }
      if( ADDRESS == state ) {

	return;
      }
      return;
    }

    if( line[lineptr] == '@' ) {
      if( BLANK == state ) {
	negative = false;
	value = 0;
	state = ADDRESS;
	continue;
      }
      if( NUMBER == state ) {
	error_message("'@' inside a number");
      }
      if( ALPHA == state ) {
	error_message("'@' inside a word");
      }
      if( ADDRESS == state ) {
	error_message("'@' inside an address");
      }
      state = BLANK;
    }

    if( line[lineptr] == '-' || line[lineptr] == '+' ) {
      if( BLANK == state ) {
	if( line[lineptr] == '-' ) {
	  negative = true;
	} else {
	  negative = false;
	}
	value = 0;
	state = NUMBER;
	continue;
      }
      if( NUMBER == state ) {
	error_message("'-' or '+' sign inside a number.");
      }
      if( ALPHA == state ) {
	error_message("words much be alphabetical.");
      }
      if( ADDRESS == state ) {
	error_message("memory addresses don't need signs.");
      }
      state = BLANK;
      continue;
    }

    if( !out_of_bounds(value, 0, MAXVAL) ) {
      if( negative ) {
	value *= -1;
      }
      sml->memory[sml->iptr++] = value;
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

  /* digits are allowed for numbers */
  if( isdigit(a) ) {
    return true;
  }

  /* alphabet allowed for commands */
  if( isalpha(a) ) {
    return true;
  }
  switch( a ) {
  case '-':  /* Negative number */
  case '+':  /* Positive number, not needed */
  case '@':  /* Move read destination */
  case '#':  /* Comments */
  case ' ':  /* Spaces */
  case '\t': /* Tabs */
    return true;
  default:
    break;
  }
  return false;
}
