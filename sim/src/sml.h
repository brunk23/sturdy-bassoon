#ifndef SML_H
#define SML_H

#include "sml_shared.h"

#include "ncurses.h"

struct machineState;		/* Forward declare */

typedef int (*opPtr)();

/*
 * breaktable will hold a value for each address.
 * If the value is 0, the machine will not break
 * there. If the value is positive, it will decrement
 * and break (so it only stops once--used for stepping).
 * If it is negative it will always break there.
 */
struct machineState {
  int accumulator;
  int counter;
  int instructionRegister;
  int operationCode;
  int operand;
  int inbuff[MEMSIZE];
  int ibc;
  int memory[MEMSIZE];
  int breaktable[MEMSIZE];
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
void cleanup();

extern struct machineState *sml;

#endif
