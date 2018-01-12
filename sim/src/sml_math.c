/*
 * The math related opcodes for SML
 */

#include "sml.h"

// add operation
int opcode_add()
{
  sml->counter++;
  sml->accumulator += sml->memory[sml->operand];
  sml->accumulator %= (MAXOP * OPFACT);
  return 0;
}

// subtract operation
int opcode_subtract()
{
  sml->counter++;
  sml->accumulator -= sml->memory[sml->operand];
  sml->accumulator %= (MAXOP*OPFACT);
  return 0;
}

// simple operation
int opcode_multiply()
{
  sml->counter++;
  sml->accumulator *= sml->memory[sml->operand];
  sml->accumulator %= (MAXOP*OPFACT);
  return 0;
}

// division operation
int opcode_divide()
{
  if ( sml->memory[sml->operand] == 0 ) {
    error_message("DIVIDE BY ZERO: MACHINE HALTED");
    sml->running = false;
    return -1;
  }
  sml->counter++;
  sml->accumulator /= sml->memory[sml->operand];
  return 0;
}

// provide the modulus operation
int opcode_mod()
{
  if ( sml->memory[sml->operand] == 0 ) {
    error_message("DIVIDE BY ZERO: MACHINE HALTED");
    sml->running = false;
    return -1;
  }
  sml->counter++;
  sml->accumulator %= sml->memory[sml->operand];
  return 0;
}

int opcode_inc()
{
  sml->counter++;
  sml->memory[sml->operand]++;
  sml->memory[sml->operand] %= (MAXOP * OPFACT);
  return 0;
}

int opcode_dec()
{
  sml->counter++;
  sml->memory[sml->operand]--;
  sml->memory[sml->operand] %= (MAXOP * OPFACT);
  return 0;
}
