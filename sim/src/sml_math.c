/*
 * The math related opcodes for SML
 */

#include "sml.h"

// add operation
void opcode_add() {
  sml->iptr++;
  sml->acc += sml->memory[sml->operand];
  if(sml->acc > MAXVAL || sml->acc < MINVAL) {
    sml->acc %= (MAXOP*OPFACT);
  }
  return;
}

// subtract operation
void opcode_subtract() {
  sml->iptr++;
  sml->acc -= sml->memory[sml->operand];
  if(sml->acc > MAXVAL || sml->acc < MINVAL) {
    sml->acc %= (MAXOP*OPFACT);
  }
  return;
}

// simple operation
void opcode_multiply() {
  sml->iptr++;
  sml->acc *= sml->memory[sml->operand];
  if(sml->acc > MAXVAL || sml->acc < MINVAL) {
    sml->acc %= (MAXOP*OPFACT);
  }
  return;
}

// division operation
void opcode_divide() {
  if ( sml->memory[sml->operand] == 0 ) {
    error_message("DIVIDE BY ZERO:","MACHINE HALTED",0);
    sml->running = false;
    return;
  }
  sml->iptr++;
  sml->acc /= sml->memory[sml->operand];
  return;
}

// provide the modulus operation
void opcode_mod() {
  if ( sml->memory[sml->operand] == 0 ) {
    error_message("DIVIDE BY ZERO:","MACHINE HALTED",0);
    sml->running = false;
    return;
  }
  sml->iptr++;
  sml->acc %= sml->memory[sml->operand];
  return;
}

void opcode_inc() {
  sml->iptr++;
  sml->memory[sml->operand]++;
  if(sml->memory[sml->operand] > MAXVAL) {
    sml->memory[sml->operand] %= (MAXOP * OPFACT);
  }
  update_mem_addr(sml->operand);
  return;
}

void opcode_dec() {
  sml->iptr++;
  sml->memory[sml->operand]--;
  if(sml->memory[sml->operand] < MINVAL) {
    sml->memory[sml->operand] %= (MAXOP * OPFACT);
  }
  update_mem_addr(sml->operand);
  return;
}
