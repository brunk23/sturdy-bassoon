/*
 * The math related opcodes for SML
 */

#include "sml.h"

// add operation
int opcode_add() {
  sml->iptr++;
  sml->acc += sml->memory[sml->operand];
  if(sml->acc > MAXVAL || sml->acc < MINVAL) {
    sml->acc %= (MAXOP*OPFACT);
  }
  return 0;
}

// subtract operation
int opcode_subtract() {
  sml->iptr++;
  sml->acc -= sml->memory[sml->operand];
  if(sml->acc > MAXVAL || sml->acc < MINVAL) {
    sml->acc %= (MAXOP*OPFACT);
  }
  return 0;
}

// simple operation
int opcode_multiply() {
  sml->iptr++;
  sml->acc *= sml->memory[sml->operand];
  if(sml->acc > MAXVAL || sml->acc < MINVAL) {
    sml->acc %= (MAXOP*OPFACT);
  }
  return 0;
}

// division operation
int opcode_divide() {
  if ( sml->memory[sml->operand] == 0 ) {
    error_message("DIVIDE BY ZERO:","MACHINE HALTED",0);
    sml->running = false;
    return -1;
  }
  sml->iptr++;
  sml->acc /= sml->memory[sml->operand];
  return 0;
}

// provide the modulus operation
int opcode_mod() {
  if ( sml->memory[sml->operand] == 0 ) {
    error_message("DIVIDE BY ZERO:","MACHINE HALTED",0);
    sml->running = false;
    return -1;
  }
  sml->iptr++;
  sml->acc %= sml->memory[sml->operand];
  return 0;
}

int opcode_inc() {
  sml->iptr++;
  sml->memory[sml->operand]++;
  if(sml->memory[sml->operand] > MAXVAL) {
    sml->memory[sml->operand] %= (MAXOP * OPFACT);
  }
  return 0;
}

int opcode_dec() {
  sml->iptr++;
  sml->memory[sml->operand]--;
  if(sml->memory[sml->operand] < MINVAL) {
    sml->memory[sml->operand] %= (MAXOP * OPFACT);
  }
  return 0;
}
