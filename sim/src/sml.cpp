#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include "sml.h"
#include "sml_math.h"
#include "sml_control.h"
#include "sml_memory.h"

using std::cin;
using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::ifstream;
using std::setfill;

void process(string line, machineState *sml) {
  int input;
  unsigned int i;
  bool negative = false;

  if( !( (line[0] == '-') ||
	 (line[0] >= '0' && line[0] <= '9'))) {
    return;
  }

  input = 0;
  for( i = 0; i < line.length(); i++ ) {

    if( line[i] == '-' ) {
      negative = true;
      continue;
    }
    if( (line[i] >= '0') && (line[i] <='9') ) {
      input *= 10;
      input += line[i] - '0';
    } else {
      break;
    }
  }
  if( !out_of_bounds(input,MINVAL,MAXVAL) ) {
    if( negative ) {
      input *= -1;
    }
    sml->memory[sml->counter++] = input;
  }
  if( i < line.length() ) {
    while( i < line.length() ) {
      // Ignore comments
      if( line[i] == '#' ) {
	return;
      }
      // A new number started
      if( (line[i] == '-') || (line[i] >= '0' && line[i] <= '9') ) {
	cout << "Sending: " << line.substr(i) << endl;
	process( line.substr(i), sml );
	return;
      }
      // ignore everything else.
      i++;
    }
  }
}

int main(int argc, char *argv[])
{
  int returnCode = 0;
  unsigned int n;
  string line;
  machineState smlReal;
  machineState *sml = &smlReal;
  bool debug = false;

  // If there's an error making the machine, quit.
  if ( (returnCode = init_machine(sml)) ) {
    cout << "ERROR: Failed to create SML Machine." << endl;
    return returnCode;
  }

  cout << "FCC booting . . .\n\n** ERROR: MEMORY CHIP NOT INSERTED **";
  cout << "\n\nENTERING MANUAL ENTRY MODE\n";
  cout << "PRESS THE \"GO\" BUTTON WHEN READY\n" << endl;
  
  cout << smlReal.counter << ": ";
  while( cin >> line ) {
    if( line[0] == 'g' || line[0] =='G') {
      cout << "CODE: ";
      cin >> line;
      returnCode = 0;
      n = 0;
      if( line[0] == '-' ) {
	debug = true;
	n++;
      }
      while( n < line.length() ) {
	if( line[n] >= '0' && line[n] <= '9' ) {
	  returnCode *= 10;
	  returnCode += line[n] - '0';
	  n++;
	} else {
	  break;
	}
      }
      if( debug ) {
	returnCode *= -1;
	debug = false;
      }
      sml->outbuff[0] = returnCode;
      sml->obc++;
      break;
    }
    
    process(line, sml);
    cout << smlReal.counter << ": ";
  }

  if ( argc > 1 ) {
    memory_dump(sml);
    debug = true;
  } else {
    debug = false;
  }
  smlReal.counter = 0;
  smlReal.running = true;
  while ( smlReal.running ) {
    if(smlReal.counter == MEMSIZE) {
      error_message("COUNTER OVERRAN MEMORY");
      returnCode = 1;		// magic number again
      break;
    }
    smlReal.instructionRegister = smlReal.memory[smlReal.counter];
    smlReal.operationCode = smlReal.instructionRegister / OPFACT;
    smlReal.operand = smlReal.instructionRegister % OPFACT;
    returnCode=sml->inst_tble[smlReal.operationCode](sml);
  }
  if( debug ) {
    // we only dump the memory if we input the file by hand
    // or we request it
    memory_dump(sml);
  } else {
    cout << endl << endl;
  }
  return returnCode;
}

int init_machine(machineState *sml)
{
  // Unsupported OPCODES crash the machine.
  for(int i = 0; i < MAXOP; ++i) {
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
  sml->obc = 0;
  for(int i = 0; i < MEMSIZE; ++i) {
    sml->inbuff[i] = 0;
    sml->outbuff[i] = 0;
    sml->memory[i] = 0;
  }
  return 0;
}

void error_message(string message)
{
  unsigned int y = message.size();
  cout << "\n\n\t";
  for(unsigned int i = 0; i < y + 4; ++i) {
    cout << "*";
  }
  cout << "\n\t* " << message << " *" << "\n\t";
  for(unsigned int i = 0; i < y + 4; ++i) {
    cout << "*";
  }
  cout << "\n\n\n";
}

int memory_dump(machineState *sml)
{
  cout << "\n\nREGISTERS:" << endl
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
  for(int i = 0; i < 10; ++ i) {
    cout << i << setw(7) << "  ";
  }
  cout << endl;
  for(int i = 0; i < MEMSIZE/10 ; ++i) {
    cout << setw(3) << i*10 << " ";
    for(int j = 0; j < 10; ++j) {
      cout << setw(7) << sml->memory[i*10+j] << " ";
    }
    cout << endl;
  }

  cout << "OBUFF:" << endl;
  cout << setw(10);
  for(int i = 0; i < 10; ++ i) {
    cout << i << setw(7) << "  ";
  }
  cout << endl;
  for(int i = 0; i < MEMSIZE/10 ; ++i) {
    if( i * 10 >= sml->obc ) {
      break;
    }
    cout << setw(3) << i*10 << " ";
    for(int j = 0; j < 10; ++j) {
      cout << setw(7) << sml->outbuff[i*10+j] << " ";
    }
    cout << endl;
  }
  cout << endl;
  sml->counter++;
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
