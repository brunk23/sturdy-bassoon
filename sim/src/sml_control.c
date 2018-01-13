/*
 * Control related opcodes
 */

#include "sml.h"

// branch operation
int opcode_branch()
{
  // Just change the iptr, we're going to a different spot.
  sml->iptr = sml->operand;
  return 0;
}

int opcode_branch_neg()
{
  if( sml->acc < 0 ) {
    sml->iptr = sml->operand;
  } else {
    sml->iptr++;
  }
  return 0;
}

int opcode_branch_zero()
{
  if( sml->acc == 0 ) {
    sml->iptr = sml->operand;
  } else {
    sml->iptr++;
  }
  return 0;
}

int opcode_nop()
{
  sml->iptr++;
  return 0;
}

int opcode_halt()
{
  sml->running = false;
  error_message(0, "NORMAL HALT", 0);
  return sml->memory[sml->operand];
}

// Handle trying to run an unsupported operation
int opcode_invalid()
{
  error_message("INVALID OPERATION CODE:","MACHINE HALTED",0);
  sml->running = false;
  return -1;
}
