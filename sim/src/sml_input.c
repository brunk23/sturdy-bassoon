#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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
  int value = 0, opcode = 0, state = BLANK;
  unsigned int lineptr, strptr = 0;
  bool negative = false;

  buffptr = 0;
  for( lineptr = 0; lineptr < BUFFSIZE; lineptr++ ) {

    switch( state ) {
      /* BLANK state */
    case BLANK:
      if( endstr( line[lineptr] ) ) {
	return;
      }
      if( isalpha( line[lineptr] ) ) {
	strptr = 0;
	str[strptr] = tolower(line[lineptr]);
	strptr++;
	state = ALPHA;
	break;
      }
      if(isdigit(line[lineptr])|| line[lineptr] == '-' || line[lineptr] == '+') {
	negative = false;
	if( line[lineptr] == '-' ) {
	  negative = true;
	}
	if( isdigit( line[lineptr] ) ) {
	  value = line[lineptr] - '0';
	} else {
	  value = 0;
	}
	state = NUMBER;
	break;
      }
      if( line[lineptr] == '@' ) {
	negative = false;
	value = 0;
	state = ADDRESS;
	break;
      }
      break;

      /* NUMBER state */
    case NUMBER:
      if( endcond( line[lineptr] ) ) {
	state = BLANK;
	if( !out_of_bounds(value, 0, MAXVAL) ) {
	  if( negative ) {
	    value *= -1;
	  }
	  if( sml->running == false && sml->stepping == false ) {
	    sml->memory[sml->iptr] = value;
	    sml->iptr++;
	    sml->iptr %= MEMSIZE;
	  } else {
	    sml->inbuff[sml->inbuff_end] = value;
	    sml->inbuff_end++;
	    sml->inbuff_end %= MEMSIZE;
	  }
	} else {
	  error_message("number out of range: ignoring");
	}
	if( endstr( line[lineptr] ) ) {
	  return;
	}
	break;
      }

      if( isdigit( line[lineptr] ) ) {
        value *= 10;
        value += line[lineptr] - '0';
        break;
      }
      error_message("errors in number format: ignoring value");
      state = BLANK;
      break;

      /* ALPHA state */
    case ALPHA:
      if( endcond( line[lineptr] ) ) {
        str[strptr] = 0;
	opcode = token(str);
	if( opcode >= 0 ) {
	  negative = false;
	  value = 0;
	  strptr = 0;
	  switch( opcode ) {
	  case HALT:
	    sml->memory[sml->iptr] = opcode * OPFACT;
	    sml->iptr++;
	    sml->iptr %= MEMSIZE;
	    state = BLANK;
	    break;
	  case DUMPMEM:
	  case DUMPSTATE:
	  case RESTOREMEM:
	    state = FILEIOHELP;
	    break;
	  default:
	    state = ASSEMBLEHELP;
	    break;
	  }
	} else {
	  state = BLANK;
	}
        if( endstr( line[lineptr] ) ) {
          return;
        }
        break;
      }
      if( isalpha( line[lineptr] ) ) {
        str[strptr] = tolower(line[lineptr]);
        strptr++;
        break;
      }
      error_message("garbage in alpha-string: ignoring word");
      state = BLANK;
      break;

      /* FILEIOHELP state */
    case FILEIOHELP:
      if( endstr(line[lineptr]) ) {
	return;
      }
      if( endcond(line[lineptr]) ) {
	break;
      }
      if( allowedchar( line[lineptr] ) ) {
	strptr = 0;
	str[strptr] = line[lineptr];
	strptr++;
	state = FILEIO;
	break;
      }
      state = BLANK;
      error_message("could not find address");
      break;

      /* FILEIO state */
    case FILEIO:
      if( endcond( line[lineptr] ) ) {
        str[strptr] = 0;
	state = BLANK;
	switch( opcode ) {
	case DUMPMEM:
	  if( !(writefile(str) == 0) ) {
	    error_message("unable to save memory to file");
	  }
	  break;
	case DUMPSTATE:
	  if( !(writestate(str) == 0) ) {
	    error_message("unable to save state to file");
	  }
	  break;
	case RESTOREMEM:
	  if( !(readfile(str) == 0 ) ) {
	    error_message("unable to read file to memory");
	  }
	  break;
	default:
	  error_message("bad state in file io");
	  break;
	}
	if( endstr( line[lineptr] ) ) {
	  return;
	}
	break;
      }
      if( allowedchar( line[lineptr] ) ) {
        str[strptr] = line[lineptr];
        strptr++;
        break;
      }
      error_message("bad file name: command ignored");
      state = BLANK;
      break;

      /* ASSEMBLEHELP state */
    case ASSEMBLEHELP:
      if( endstr(line[lineptr]) ) {
	return;
      }
      if( endcond(line[lineptr]) ) {
	break;
      }
      if( isdigit( line[lineptr]) ) {
	value = line[lineptr] - '0';
	state = ASSEMBLE;
	break;
      }
      if( line[lineptr] == '-' || line[lineptr] == '+' ) {
	if( line[lineptr] == '-' ) {
	  negative = true;
	}
	value = 0;
	state = ASSEMBLE;
	break;
      }
      state = BLANK;
      error_message("could not find address");
      break;

      /* ADDRESS state */
    case ADDRESS:
      if( endcond( line[lineptr] ) ) {
	if( !out_of_bounds(value, 0, MEMSIZE) ) {
	  sml->iptr = value;
	} else {
	  error_message("Invalid address in @ command: staying put");
	}
	if( endstr( line[lineptr] ) ) {
	  return;
	}
	state = BLANK;
	break;
      }
      if( isdigit( line[lineptr] ) ) {
        value *= 10;
        value += line[lineptr] - '0';
        break;
      }
      error_message("errors in @ command string: ignoring command");
      state = BLANK;
      break;

      /* ASSEMBLE state */
    case ASSEMBLE:
      if( endcond( line[lineptr] ) ) {
	state = BLANK;
	if( negative ) {
	  value *= -1;
	}
	if( opcode == SET ) {
	  if( !out_of_bounds(value, MINVAL, MAXVAL) ) {
	    sml->acc = value;
	  } else {
	    error_message("unable to set accumulator");
	  }
	  break;
	}
	if( !out_of_bounds(value, 0, MEMSIZE) ) {
	  if( opcode == BREAK ) {
	    sml->breaktable[value] = 1;
	    break;
	  }
	  if( opcode == CLEAR ) {
	    sml->breaktable[value] = 0;
	    break;
	  }
	  opcode *= OPFACT;
	  opcode += value;
	  sml->memory[sml->iptr] = opcode;
	  sml->iptr++;
	} else {
	  error_message("Invalid address in assembly command: what?");
	}
	if( endstr( line[lineptr] ) ) {
	  return;
	}
	break;
      }
      if( isdigit( line[lineptr] ) ) {
        value *= 10;
        value += line[lineptr] - '0';
        break;
      }
      error_message("errors in assembly command: ignoring command");
      state = BLANK;
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
  int val = INVALID;
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
  if( strcmp(str, "sto") == 0 ) {
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
  if( strcmp(str, "quit") == 0 ) {
    exit(0);
  }
  if( strcmp(str, "step") == 0 ) {
    sml->stepping = true;
    sml->running = true;
    return val;
  }
  if( strcmp(str, "go") == 0 ) {
    sml->iptr = 0;
    sml->stepping = false;
    sml->running = true;
    return val;
  }
  if( strcmp(str, "continue") == 0 ) {
    sml->stepping = false;
    sml->running = true;
    return val;
  }
  if( strcmp(str, "stop") == 0 ) {
    sml->stepping = false;
    sml->running = false;
    return val;
  }
  if( strcmp(str, "nodump") == 0 ) {
    sml->debug  = false;
    return val;
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
  if( strcmp(str, "reset") == 0 ) {
    init_machine();
    return val;
  }
  if( strcmp(str, "wipe") == 0 ) {
    error_message("");
    return val;
  }
  error_message(str);
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
  case '/':  /* slash for filenames */
  case '.':  /* dots in filenames */
    return true;
  default:
    break;
  }
  return false;
}

bool endcond(char a) {
  if( a == ' ' || a == '\t' || endstr(a) ) {
    return true;
  }
  return false;
}

bool endstr(char a) {
  if( a == '#' || a == 0 ) {
    return true;
  }
  return false;
}
