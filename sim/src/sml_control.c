/*
 * Control related opcodes
 */

#include "sml.h"

// branch operation
int opcode_branch()
{
  // Just change the counter, we're going to a different spot.
  sml->counter = sml->operand;
  return 0;
}

int opcode_branch_neg()
{
  if( sml->accumulator < 0 ) {
    sml->counter = sml->operand;
  } else {
    sml->counter++;
  }
  return 0;
}

int opcode_branch_zero()
{
  if( sml->accumulator == 0 ) {
    sml->counter = sml->operand;
  } else {
    sml->counter++;
  }
  return 0;
}

int opcode_nop()
{
  sml->counter++;
  return 0;
}

int opcode_halt()
{
  sml->running = false;
error_message( "NORMAL HALT" );
  return sml->memory[sml->operand];
}

// Handle trying to run an unsupported operation
int opcode_invalid()
{
  error_message("INVALID OPERATION CODE: MACHINE HALTED");
  sml->running = false;
  return -1;
}
