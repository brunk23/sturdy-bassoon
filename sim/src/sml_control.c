/*
 * Control related opcodes
 */

#include "sml.h"

void sig_int(int in) {
  sml->running = false;
  displaychip();
}

// branch operation
int opcode_branch() {
  // Just change the iptr, we're going to a different spot.
  profile_data->memmap[ sml->operand ]--;
  sml->iptr = sml->operand;
  return 0;
}

int opcode_branch_neg() {
  profile_data->memmap[ sml->operand ]--;
  if( sml->acc < 0 ) {
    sml->iptr = sml->operand;
  } else {
    sml->iptr++;
  }
  return 0;
}

int opcode_branch_zero() {
  profile_data->memmap[ sml->operand ]--;
  if( sml->acc == 0 ) {
    sml->iptr = sml->operand;
  } else {
    sml->iptr++;
  }
  return 0;
}

int opcode_nop() {
  profile_data->memmap[ sml->operand ]--;
  sml->iptr++;
  return 0;
}

int opcode_halt() {
  profile_data->memmap[ sml->operand ]--;
  sml->running = false;
  error_message(0, "NORMAL HALT", 0);
  return sml->memory[sml->operand];
}

// Handle trying to run an unsupported operation
int opcode_invalid() {
  error_message("INVALID OPERATION CODE:","MACHINE HALTED",0);
  sml->running = false;
  return -1;
}
