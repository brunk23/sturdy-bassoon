#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "sml.h"

struct machineState *sml;
struct io_buffer *inbuff;

int main(int argc, char *argv[])
{
  int value = 0, i;

  out_buffer_head = 0;
  out_buffer_tail = 0;
  out_buffer_max_length = 0;
  buffptr = 0;

  for( i = 0; i < BUFFSIZE+1; i++) {
    userline[i] = 0;
  }

  sml = (struct machineState *)malloc(sizeof(struct machineState));
  if( 0 == sml ) {
    fprintf(stderr,"ERROR: No memeory for SML Machine.\n");
    return 1;
  }

  inbuff = new_io_buffer(MEMSIZE);

  // If there's an error making the machine, quit.
  if ( (value = init_machine()) ) {
    fprintf(stderr,"ERROR: Failed to create SML Machine.\n");
    return value;
  }

  init_windows();
  updatescreen();
  displaymem();
  displayoutput();
  error_message(0,0,0);


  signal(SIGINT, sig_int);
  atexit(cleanup);

  run_loop();

  return value;
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
  unsigned int value;
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
	value = sml->inst_tble[sml->opcode]();
	sml->iptr %= MEMSIZE;
      }
      if( sml->stepping || sml->breaktable[sml->iptr] ) {
	sml->running = false;
	sml->stepping = false;
      }
    }
  }
  return value;
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
  sml->inbuff_start = 0;
  sml->inbuff_end = 0;
  for(i = 0; i < MEMSIZE; ++i) {
    sml->inbuff[i] = 0;
    sml->breaktable[i] = false;
    sml->memory[i] = 0;
  }
  return 0;
}

/*
 * This will not fail. Program quits instead.
 */
struct io_buffer *new_io_buffer(int size) {
  struct io_buffer *tmp;

  tmp = (struct io_buffer *)malloc(sizeof(struct io_buffer));
  if( 0 != tmp ) {
    tmp->val = (int *)malloc(sizeof(int) * size);
    if( 0 != tmp->val ) {
      tmp->head = 0;
      tmp->size = size;
      tmp->len = 0;
      return tmp;
    }
    free(tmp);
    tmp = 0;
  }
  fprintf(stderr,"ERROR: Could not create i/o buffer.\n");
  exit(1);
}
