#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "sml.h"

char userline[BUFFSIZE + 1];
int buffptr = 0;

void process(char *line) {
  char str[BUFFSIZE+1];
  int value = 0, opcode = 0, state = BLANK;
  unsigned int lineptr = 0;

  error_message(0,0,0);
  buffptr = 0;

  while( lineptr < BUFFSIZE ) {

    switch( state ) {
      /* BLANK state */
    case BLANK:
      value = getnextword( &line[lineptr], str);
      if( INVALID == value ) {
	return;
      }
      lineptr += value;

      if( isalpha( str[0] ) ) {
	state = ALPHA;
	break;
      }

      if( isdigit(str[0]) || str[0] == '-' || str[0] == '+') {
	state = NUMBER;
	break;
      }

      if( str[0] == '@' ) {
	state = ADDRESS;
	break;
      }
      break;

      /* NUMBER state */
    case NUMBER:
      state = BLANK;
      value = numberbetween(str, MINVAL, MAXVAL);
      if( INVALID != value ) {
	if( sml->running == false && sml->stepping == false ) {
	  sml->memory[sml->iptr] = value;
	  update_mem_addr(sml->iptr);
	  sml->iptr++;
	  sml->iptr %= MEMSIZE;
	} else {
	  add_io_value(inbuff, value);
	}
      } else {
	error_message("Error in number or out of range:","Ignoring",line);
      }
      break;

      /* ALPHA state */
    case ALPHA:
      opcode = token(str);
      if( opcode != INVALID ) {
	value = 0;
	switch( opcode ) {
	case HALT:
	  // HALT needs no arguments and is handled here
	  sml->memory[sml->iptr] = opcode * OPFACT;
	  update_mem_addr(sml->iptr);
	  sml->iptr++;
	  sml->iptr %= MEMSIZE;
	  state = BLANK;
	  break;

	case DUMPMEM:
	case DUMPSTATE:
	case RESTOREMEM:
	case DUMPPROFILE:
	  state = FILEIO;
	  break;
	case VALID:
	  state = BLANK;
	  break;
	default:
	  state = ASSEMBLE;
	  break;
	}
      } else {
	error_message("Garbage in command word:","Ignoring word",line);
	state = BLANK;
      }
      break;

      /* FILEIO state */
    case FILEIO:
      state = BLANK;
      value = getnextword( &line[lineptr], str );
      if( INVALID == value ) {
	error_message("Bad file name",line,0);
	break;
      }
      lineptr += value;
      switch( opcode ) {
      case DUMPMEM:
	if( !(writefile(str) == 0) ) {
	  error_message("Unable to save memory to file",line,0);
	}
	break;
      case DUMPSTATE:
	if( !(writestate(str) == 0) ) {
	  error_message("Unable to save state to file",line,0);
	}
	break;
      case DUMPPROFILE:
	if( !(writeprofile(str, profile_data) == 0) ) {
	  error_message("Unable to save state to file",line,0);
	}
	break;
      case RESTOREMEM:
	if( !(readfile(str) == 0 ) ) {
	  error_message("Unable to read file to memory",line,0);
	}
	break;
      default:
	error_message("Bad state in file io",line,0);
	break;
      }
      break;

      /* ADDRESS state */
    case ADDRESS:
      value = numberbetween( &str[1], 0, MEMSIZE );
      if( value != INVALID ) {
	sml->iptr = value;
      } else {
	error_message("Invalid address in @ command:","Staying put",line);
      }
      state = BLANK;
      break;

      /* ASSEMBLE state */
    case ASSEMBLE:
      state = BLANK;
      if( INVALID == ( value = getnextword(&line[lineptr], str) ) ) {
	error_message("Missing operand","Ignoring command",line);
	break;
      }
      lineptr += value;

      if( opcode == SET ) {
	value = numberbetween(str, MINVAL, MAXVAL);
	if( value != INVALID ) {
	  sml->acc = value;
	} else {
	  error_message("Unable to set accumulator",line,0);
	}
	break;
      }
      value = numberbetween(str, 0, MEMSIZE);
      if( value != INVALID ) {
	if( opcode == BREAK ) {
	  sml->breaktable[value] = true;
	  break;
	}
	if( opcode == CLEAR ) {
	  sml->breaktable[value] = false;
	  break;
	}
	opcode *= OPFACT;
	opcode += value;
	sml->memory[sml->iptr] = opcode;
	update_mem_addr(sml->iptr);
	sml->iptr++;
      } else {
	error_message("Invalid address in assembly command:",line,0);
      }
      break;

    default:
      state = BLANK;
      break;
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
  int i;

  for( i = 0; str[i]; i++ ) {
    str[i] = tolower( str[i] );
  }

  if( strcmp(str, "break") == 0 ) {
    return BREAK;
  }
  if( strcmp(str, "clear") == 0 ) {
    return CLEAR;
  }
  if( strcmp(str, "read") == 0 ) {
    return READ;
  }
  if( strcmp(str, "write") == 0 ) {
    return WRITE;
  }
  if( strcmp(str, "lda") == 0 ) {
    return LOAD;
  }
  if( strcmp(str, "sta") == 0 ) {
    return STORE;
  }
  if( strcmp(str, "add") == 0 ) {
    return ADD;
  }
  if( strcmp(str, "sub") == 0 ) {
    return SUBTRACT;
  }
  if( strcmp(str, "mult") == 0 ) {
    return MULTIPLY;
  }
  if( strcmp(str, "div") == 0 ) {
    return DIVIDE;
  }
  if( strcmp(str, "mod") == 0 ) {
    return MOD;
  }
  if( strcmp(str, "inc") == 0 ) {
    return INC;
  }
  if( strcmp(str, "dec") == 0 ) {
    return DEC;
  }
  if( strcmp(str, "jump") == 0 ) {
    return BRANCH;
  }
  if( strcmp(str, "jneg") == 0 ) {
    return BRANCHNEG;
  }
  if( strcmp(str, "jzero") == 0 ) {
    return BRANCHZERO;
  }
  if( strcmp(str, "halt") == 0 ) {
    return HALT;
  }
  if( strcmp(str, "quit") == 0 || strcmp(str, "q") == 0 ) {
    exit(0);
  }
  if( strcmp(str, "step") == 0 || strcmp(str, "s") == 0 ) {
    sml->stepping = true;
    sml->running = true;
    return VALID;
  }
  if( strcmp(str, "go") == 0 || strcmp(str, "g") == 0 ) {
    sml->iptr = 0;
    sml->stepping = false;
    sml->running = true;
    return VALID;
  }
  if( strcmp(str, "continue") == 0 || strcmp(str, "cont") == 0) {
    sml->stepping = false;
    sml->running = true;
    return VALID;
  }
  if( strcmp(str, "stop") == 0 ) {
    sml->stepping = false;
    sml->running = false;
    return VALID;
  }
  if( strcmp(str, "nodebug") == 0 ) {
    sml->debug  = false;
    return VALID;
  }
  if( strcmp(str, "debug") == 0 ) {
    sml->debug  = true;
    return VALID;
  }
  if( strcmp(str, "dumpmem") == 0 ) {
    return DUMPMEM;
  }
  if( strcmp(str, "restoremem") == 0 ) {
    return RESTOREMEM;
  }
  if( strcmp( str, "dumpstate") == 0 ) {
    return DUMPSTATE;
  }
  if( strcmp(str, "set") == 0 ) {
    return SET;
  }
  if( strcmp(str, "dumpprofile") == 0 ) {
    return DUMPPROFILE;
  }
  if( strcmp(str, "profile") == 0 ) {
    start_profiling(profile_data);
    return VALID;
  }
  if( strcmp(str, "noprofile") == 0 ) {
    stop_profiling(profile_data);
    return VALID;
  }
  if( strcmp(str, "resetprofile") == 0 ) {
    reset_profiling(profile_data);
    return VALID;
  }
  if( strcmp(str, "reset") == 0 ) {
    init_machine();
    displaymem();
    return VALID;
  }
  /* debugging purposes */
  if( strcmp(str, "displaymem") == 0 ) {
    displaymem();
    return VALID;
  }
  if( strcmp(str, "wipe") == 0 ) {
    outbuff->head = 0;
    outbuff->len = 0;
    displayoutput();
    return VALID;
  }
  error_message("Unrecognized string:",str,0);
  return INVALID;
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
  case '/':  /* slash for filenames */
  case '.':  /* dots in filenames */
    return true;
  default:
    break;
  }
  return false;
}

inline bool endcond(char a) {
  if( a == ' ' || a == '\t' || endstr(a) ) {
    return true;
  }
  return false;
}

inline bool endstr(char a) {
  if( a == '#' || a == 0 ) {
    return true;
  }
  return false;
}

/*
 * Takes a string consisting of a single word
 * Returns number >= min and <= max.
 * Returns INVALID on error
 */
int numberbetween(char *str, int min, int max) {
  int value = INVALID, i;
  bool negative = false;

  for( i = 0; i < BUFFSIZE; i++ ) {
    if( !str[i] ) {
      if( negative ) {
	value *= -1;
      }
      if( out_of_bounds(value, min, max) ) {
	return INVALID;
      }
      return value;
    }
    if( i == 0 ) {
      if( str[i] == '-' ) {
	negative = true;
	continue;
      }
      if( str[i] == '+' ) {
	negative = false;
	continue;
      }
    }
    if( value == INVALID ) {
      value = 0;
    }
    if( isdigit( str[i] ) ) {
      value *= 10;
      value += str[i] - '0';
    } else {
      // Anything unexpected in the number invalidates the whole thing
      return INVALID;
    }
  }

  // We should not leave the above loop without returning
  // If we do, it's almost certainly an error.
  return INVALID;
}

/*
 * copies the next word in src into the dest buffer
 * Returns: number of characters read or INVALID
 */
int getnextword(char *src, char *dest) {
  int len = 0, i = 0;

  // Throw out leading whitespace, INVALID if no word found
  while( endcond(src[len] ) ) {
    if( endstr( src[len] ) ) {
      return INVALID;
    }
    len++;
  }

  for( ; !endcond(src[len]); len++ ) {
    dest[i] = src[len];
    i++;
  }
  dest[i] = 0;
  return len;
}
