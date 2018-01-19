#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "sml.h"

struct machineState *sml;
struct io_buffer *inbuff;
struct io_buffer *outbuff;

int main(int argc, char *argv[])
{
  int i;

  buffptr = 0;

  for( i = 0; i < BUFFSIZE+1; i++) {
    userline[i] = 0;
  }

  sml = (struct machineState *)malloc(sizeof(struct machineState));
  if( 0 == sml ) {
    fprintf(stderr,"ERROR: No memeory for SML Machine.\n");
    return 1;
  }

  outbuff = 0;			/* Set by init_windows */
  inbuff = new_io_buffer(MEMSIZE);

  // If there's an error making the machine, quit.
  if ( init_machine() ) {
    fprintf(stderr,"ERROR: Failed to create SML Machine.\n");
    return 1;
  }

  init_windows();
  updatescreen();
  displaymem();
  displayoutput();
  error_message(0,0,0);


  signal(SIGINT, sig_int);
  atexit(cleanup);

  run_loop();

  return 0;
}

void cleanup() {
  if( outbuff ) {
    free( outbuff->val );
    free( outbuff );
  }

  if( inbuff ) {
    free( inbuff->val );
    free( inbuff );
  }

  if( !isendwin() ) {
    endwin();
  }

  if(sml) {
    if( sml->debug ) {
      // we only dump the memory if we input the file by hand
      // or we request it
      memory_dump();
    }
  }

  if( sml ) {
    free(sml);
  }
}

/*
 * Handle running the machine. We might need a buffer for numbers
 * that haven't been read, yet. We also could use a buffer for
 * breakpoints.
 */
int run_loop() {
  int key, i;
  bool update = true;

  sml->iptr = 0;
  sml->running = false;
  while ( true ) {
    if( update ) {
      updatescreen();
    }
    doupdate();
    update = false;

    key = mvwgetch(inputwindow, 1, 1);
    if( key != ERR ) {
      update = true;
      if( key == KEY_RESIZE ) {
	term_resize();
	continue;
      }

      if( key == '\n' ) {
	process(userline);
	for( i = 0; i < BUFFSIZE; i++ ) {
	  userline[i] = 0;
	}
	continue;
      }

      // CTRL-G == 7 and runs the machine
      if(key == 7 ) {
	sml->iptr = 0;
	sml->running = true;
	continue;
      }

      if( key == KEY_BACKSPACE || key == KEY_DC || key == 127 ) {
	if( buffptr > 0 ) {
	  buffptr--;
	  userline[buffptr] = 0;
	} else {
	  beep();
	}
	continue;
      }

      if( allowedchar(key) ) {
	userline[buffptr] = key;
	buffptr++;
	if(buffptr == BUFFSIZE-1) {
	  buffptr--;
	}
	userline[buffptr] = 0;
      } else {
	beep();
      }
    }

    if( sml->running ) {
      update = true;
      sml->instr = sml->memory[sml->iptr];
      sml->opcode = sml->instr / OPFACT;
      sml->operand = sml->instr % OPFACT;
      if(sml->opcode < 0 || sml->opcode >= OPFACT) {
	opcode_invalid();
      } else {
	sml->inst_tble[sml->opcode]();
	sml->iptr %= MEMSIZE;
      }
      if( sml->stepping || sml->breaktable[sml->iptr] ) {
	sml->running = false;
	sml->stepping = false;
      }
    }
  }
  return 0;
}

int init_machine() {
  int i;
  // Unsupported OPCODES crash the machine.
  for(i = 0; i < MAXOP; ++i) {
    sml->inst_tble[i] = opcode_invalid;
  }

  // These are the currently supported instructions

  // Arithmatic
  sml->inst_tble[ADD]=opcode_add;
  sml->inst_tble[SUBTRACT]=opcode_subtract;
  sml->inst_tble[MULTIPLY]=opcode_multiply;
  sml->inst_tble[DIVIDE]=opcode_divide;
  sml->inst_tble[MOD]=opcode_mod;

  // memory access
  sml->inst_tble[LOAD]=opcode_load;
  sml->inst_tble[STORE]=opcode_store;

  // i/o
  sml->inst_tble[READ]=opcode_read;
  sml->inst_tble[WRITE]=opcode_write;

  // flow control
  sml->inst_tble[BRANCH]=opcode_branch;
  sml->inst_tble[BRANCHNEG]=opcode_branch_neg;
  sml->inst_tble[BRANCHZERO]=opcode_branch_zero;
  sml->inst_tble[HALT]=opcode_halt;

  // Extended opcodes
  sml->inst_tble[INC]=opcode_inc;
  sml->inst_tble[DEC]=opcode_dec;

  sml->acc = 0;
  sml->iptr = 0;
  sml->instr = 0;
  sml->opcode = 0;
  sml->operand = 0;
  sml->running = false;
  sml->stepping = false;
  sml->debug = false;
  for(i = 0; i < MEMSIZE; ++i) {
    sml->breaktable[i] = false;
    sml->memory[i] = 0;
  }
  return 0;
}
