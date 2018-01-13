#include "sml.h"

/*
 * Add all input and string processing stuff here.  Handle all
 * complete strings.  Move process(line) into here.
 */

// load operation
int opcode_load()
{
  sml->iptr++;		// increment the instruction counter
  sml->acc = sml->memory[sml->operand];
  return 0;
}

// store operation
int opcode_store()
{
  sml->iptr++;		// increment the instruction counter
  sml->memory[sml->operand] = sml->acc;
  return 0;
}

// simple operation
int opcode_read()
{
  /* There is no input at this time */
  if( sml->inbuff_start == sml->inbuff_end ) {
    return 0;
  }
  sml->memory[sml->operand] = sml->inbuff[sml->inbuff_start];
  sml->inbuff_start++;
  sml->inbuff_start %= MEMSIZE;
  sml->iptr++;
  return 0;
}

int opcode_write()
{
  output_value(sml->memory[sml->operand]);
  sml->iptr++;
  return 0;
}

bool is_valid_address(int address) {
  return !(out_of_bounds(address, 0, MEMSIZE - 1));
}

/*
 * Determine if a certain value is out of range. Useful for determining
 * memory validity and other functions.
 */
bool out_of_bounds(int value, int min, int max) {
  return ( (value > max) || (value < min) );
}
