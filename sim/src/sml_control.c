/*
 * Control related opcodes
 */

#include "sml.h"

void sig_int(int in) {
  sml->running = false;
  displaychip();
}

// branch operation
void opcode_branch() {
  // Just change the iptr, we're going to a different spot.
  profile_data->memmap[ sml->operand ]--;
  sml->iptr = sml->operand;
  return;
}

void opcode_branch_neg() {
  profile_data->memmap[ sml->operand ]--;
  if( sml->acc < 0 ) {
    sml->iptr = sml->operand;
  } else {
    sml->iptr++;
  }
  return;
}

void opcode_branch_zero() {
  profile_data->memmap[ sml->operand ]--;
  if( sml->acc == 0 ) {
    sml->iptr = sml->operand;
  } else {
    sml->iptr++;
  }
  return;
}

void opcode_nop() {
  profile_data->memmap[ sml->operand ]--;
  sml->iptr++;
  return;
}

void opcode_halt() {
  profile_data->memmap[ sml->operand ]--;
  sml->running = false;
  error_message(0, "NORMAL HALT", 0);
  return;
}

// Handle trying to run an unsupported operation
void opcode_invalid() {
  error_message("INVALID OPERATION CODE:","MACHINE HALTED",0);
  sml->running = false;
  return;
}
