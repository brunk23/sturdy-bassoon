#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "sml.h"
#include "sml_math.h"
#include "sml_control.h"
#include "sml_memory.h"
#include "sml_io.h"

struct machineState *sml;

int main(int argc, char *argv[])
{
  int returnCode = 0;
  struct machineState smlReal;
  bool debug = false;

  sml = &smlReal;

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

  displaychip();
  displaymem();

  error_message("FCC booting . . .");

  if ( argc > 1 ) {
    memory_dump();
    debug = true;
  } else {
    debug = false;
  }

  run_loop();

  if( debug ) {
    // we only dump the memory if we input the file by hand
    // or we request it
    memory_dump();
  } else {
    printf("\n\n");
  }

  /*
   * Properly free the resources we allocated
   */
  while(out_buffer_head) {
    out_buffer_tail = out_buffer_head;
    out_buffer_head = out_buffer_head->next;
    free(out_buffer_tail);
  }

  endwin();
  return returnCode;
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

      line[buffptr] = key;
      buffptr++;
      if(buffptr == BUFFSIZE-1) {
	buffptr--;
      }
      line[buffptr] = 0;
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
      if(sml->operationCode > 0) {
	returnCode=sml->inst_tble[sml->operationCode]();
      } else {
	opcode_invalid();
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
    sml->memory[i] = 0;
  }
  return 0;
}

void error_message(char *message)
{
  werase(messagewindow);
  wborder(messagewindow, 0, 0, 0, 0, 0, 0, 0, 0);
  mvwprintw(messagewindow, 2, 2, message);
  wrefresh(messagewindow);
}

int memory_dump()
{
  int i, j;
  printf("\n\nREGISTERS:\n");
  /*
    << setfill(' ') << setw(20) << "Accumulator" << setw(8)
    << setfill(' ') << sml->accumulator << endl
    << setfill(' ') << setw(20) << "instructionPointer" << setw(8)
    << setfill(' ') << sml->counter << endl    
    << setfill(' ') << setw(20) << "instructionRegister" << setw(8)
    << setfill(' ') << sml->instructionRegister << endl
    << setfill(' ') << setw(20) << "operationCode" << setw(8)
    << setfill(' ') << sml->operationCode << endl
    << setfill(' ') << setw(20) << "operand" << setw(8)
    << setfill(' ') << sml->operand << endl
    << endl << endl;
  
    cout << "MEMORY:" << endl;
    cout << setw(10);
    for(i = 0; i < 10; ++ i) {
    cout << i << setw(7) << "  ";
    }
    cout << endl;
    for(i = 0; i < MEMSIZE/10 ; ++i) {
    cout << setw(3) << i*10 << " ";
    for(int j = 0; j < 10; ++j) {
    cout << setw(7) << sml->memory[i*10+j] << " ";
    }
    cout << endl;
    }

    cout << "OBUFF:" << endl;
    cout << setw(10);
    for(i = 0; i < 10; ++ i) {
    cout << i << setw(7) << "  ";
    }
    cout << endl;
    for(i = 0; i < MEMSIZE/10 ; ++i) {
    if( i * 10 >= sml->obc ) {
    break;
    }
    cout << setw(3) << i*10 << " ";
    for(j = 0; j < 10; ++j) {
    cout << setw(7) << sml->outbuff[i*10+j] << " ";
    }
    cout << endl;
    }
    cout << endl;
  */
  return 0;
}

bool is_valid_address(int address) {
  return !(out_of_bounds(address, 0, MEMSIZE - 1));
}
			       
/*
 * Determine if a certain value is out of range. Useful for determining
 * memory validity and other functions.
 */
bool out_of_bounds(int value, int min, int max) {
  return ( (value > max) || (value < min) );
}
