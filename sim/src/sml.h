#ifndef SML_H
#define SML_H

#include "sml_shared.h"

#include "ncurses.h"

struct machineState;		/* Forward declare */

typedef int (*opPtr)();

struct machineState {
  int accumulator;
  int counter;
  int instructionRegister;
  int operationCode;
  int operand;
  int inbuff[MEMSIZE];
  int ibc;
  int memory[MEMSIZE];
  opPtr inst_tble[MAXOP];
  bool running;
};

// These functions are in the main program file
int init_machine();
int memory_dump();
void error_message(char *message);
bool out_of_bounds(int, int, int);
bool is_valid_address(int address);
int run_loop();

extern struct machineState *sml;

#endif
