#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "sml.h"

struct machineState *sml;

bool debug = false;

int main(int argc, char *argv[])
{
  int returnCode = 0, i;

  out_buffer_head = 0;
  out_buffer_tail = 0;
  out_buffer_max_length = 0;
  buffptr = 0;

  for( i = 0; i < BUFFSIZE+1; i++) {
    line[i] = 0;
  }

  sml = (struct machineState *)malloc(sizeof(struct machineState));
  if( 0 == sml ) {
    fprintf(stderr,"ERROR: No memeory for SML Machine.\n");
    return 1;
  }

  // If there's an error making the machine, quit.
  if ( (returnCode = init_machine()) ) {
    fprintf(stderr,"ERROR: Failed to create SML Machine.\n");
    return returnCode;
  }

  initscr();
  refresh();
  init_windows();

  signal(SIGWINCH, sig_winch);
  signal(SIGINT, sig_int);
  atexit(cleanup);

  if ( argc > 1 ) {
    debug = true;
  } else {
    debug = false;
  }

  run_loop();

  return returnCode;
}

void cleanup() {
  /*
   * Properly free the resources we allocated
   */
  while(out_buffer_head) {
    out_buffer_tail = out_buffer_head;
    out_buffer_head = out_buffer_head->next;
    free(out_buffer_tail);
  }

  if( !isendwin() ) {
    endwin();
  }

  if(sml) {
    if( debug ) {
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
  unsigned int returnCode;
  int n, key, i;
  bool update = true;

  n = 1;
  sml->counter = 0;
  sml->running = false;
  while ( n ) {
    if( update) {
      updatescreen();
    }
    update = false;

    key = getch();
    if( key != ERR ) {
      update = true;
      if( key == '\n' ) {
	process();
	for( i = 0; i < BUFFSIZE; i++ ) {
	  line[i] = 0;
	}
	continue;
      }
      if( key == 'q' ) {
	break;
      }

      // CTRL-G == 7 and runs the machine
      if(key == 7 ) {
	sml->counter = 0;
	sml->running = true;
	continue;
      }

      if( key == KEY_BACKSPACE || key == KEY_DC ) {
	if( buffptr > 0 ) {
	  buffptr--;
	  line[buffptr] = 0;
	}
	continue;
      }

      if( allowedchar(key) ) {
	line[buffptr] = key;
	buffptr++;
	if(buffptr == BUFFSIZE-1) {
	  buffptr--;
	}
	line[buffptr] = 0;
      }
    }

    if(sml->counter == MEMSIZE) {
      error_message("COUNTER OVERRAN MEMORY");
      returnCode = 1;		// magic number again
      break;
    }
    if( sml->running ) {
      update = true;
      sml->instructionRegister = sml->memory[sml->counter];
      sml->operationCode = sml->instructionRegister / OPFACT;
      sml->operand = sml->instructionRegister % OPFACT;
      if(sml->operationCode < 0 || sml->operationCode >= OPFACT) {
	opcode_invalid();
      } else {
	returnCode=sml->inst_tble[sml->operationCode]();
      }
    }
  }
  return returnCode;
}

int init_machine()
{
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

  sml->accumulator = 0;
  sml->counter = 0;
  sml->instructionRegister = 0;
  sml->operationCode = 0;
  sml->operand = 0;
  sml->running = false;
  sml->ibc = 0;
  for(i = 0; i < MEMSIZE; ++i) {
    sml->inbuff[i] = 0;
    sml->breaktable[i] = 0;
    sml->memory[i] = 0;
  }
  return 0;
}
