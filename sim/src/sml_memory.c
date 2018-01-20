#include "sml.h"

// load operation
int opcode_load() {
  sml->iptr++;
  sml->acc = sml->memory[sml->operand];
  return 0;
}

// store operation
int opcode_store() {
  sml->iptr++;
  sml->memory[sml->operand] = sml->acc;
  update_mem_addr(sml->operand);
  return 0;
}

// simple operation
int opcode_read() {
  /* There is no input at this time */
  if( size_io_buffer(inbuff) == 0 ) {
    return 0;
  }
  sml->memory[sml->operand] = remove_io_value(inbuff);
  sml->iptr++;
  update_mem_addr(sml->operand);
  return 0;
}

int opcode_write() {
  add_io_value(outbuff, sml->memory[sml->operand]);
  displayoutput();
  sml->iptr++;
  return 0;
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
