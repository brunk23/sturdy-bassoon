#include "sml.h"

// load operation
void opcode_load() {
  sml->iptr++;
  sml->acc = sml->memory[sml->operand];
  return;
}

// store operation
void opcode_store() {
  sml->iptr++;
  sml->memory[sml->operand] = sml->acc;
  update_mem_addr(sml->operand);
  return;
}

// simple operation
void opcode_read() {
  /* There is no input at this time */
  if( size_io_buffer(inbuff) == 0 ) {
    profile_unlog(profile_data);
    return;
  }
  sml->memory[sml->operand] = remove_io_value(inbuff);
  sml->iptr++;
  update_mem_addr(sml->operand);
  return;
}

void opcode_write() {
  add_io_value(outbuff, sml->memory[sml->operand]);
  displayoutput();
  sml->iptr++;
  return;
}

inline bool is_valid_address(int address) {
  return !(out_of_bounds(address, 0, MEMSIZE - 1));
}

/*
 * Determine if a certain value is out of range. Useful for determining
 * memory validity and other functions.
 */
inline bool out_of_bounds(int value, int min, int max) {
  return ( (value > max) || (value < min) );
}
