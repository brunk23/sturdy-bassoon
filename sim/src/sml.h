#ifndef SML_H
#define SML_H

#include <string>
#include "sml_shared.h"

#include "ncurses.h"

using std::string;

struct machineState;		/* Forward declare */

typedef int (*opPtr)(machineState *);

struct machineState {
  int accumulator;
  int counter;
  int instructionRegister;
  int operationCode;
  int operand;
  int inbuff[MEMSIZE];
  int outbuff[MEMSIZE];
  int ibc;
  int obc;
  int memory[MEMSIZE];
  opPtr inst_tble[MAXOP];
  bool running;
};

/*****
 * Proposed window layout
 * +------------------+----+
 * | chip window      | O  |
 * +------------------+ U  |
 * | message window   | T  |
 * +------------------+ P  |
 * | mem 13 x 75 min  | U  |
 * +------------------+ T  |
 * | input window     |    |
 * +------------------+----+
 *****/

// These functions are in the main program file
int init_machine(machineState *);
int init_windows();
void displaymem(machineState *sml);
void displaychip(machineState *sml);
int memory_dump(machineState *sml);
void error_message(string message);
bool out_of_bounds(int, int, int);
bool is_valid_address(int address);

#endif
