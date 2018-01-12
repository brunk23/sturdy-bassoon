/*
 * Memory related operations
 */

#include "sml.h"
#include "sml_memory.h"
#include "sml_display.h"
#include "sml_output.h"

/*
 * Add all input and string processing stuff here.  Handle all
 * complete strings.  Move process(line) into here.
 */

// load operation
int opcode_load()
{
  sml->counter++;		// increment the instruction counter	
  sml->accumulator = sml->memory[sml->operand];
  return 0;
}

// store operation
int opcode_store()
{
  sml->counter++;		// increment the instruction counter
  sml->memory[sml->operand] = sml->accumulator;
  return 0;
}

// simple operation
int opcode_read()
{
  int buffer[MEMSIZE] = {
    -9596, -3369, 2433, -5286, -3242, 1019, 3428, 5642, -1154, 1343,
    6419, -6071, -4296, -3716, 3921, 4690, -5399, -4015, 2145, 6664,
    9521, 8142, -9203, -9228, -9007, 2373, 6036, 8613, 9107, 8749,
    -799, 8173, -6579, 4321, -5226, -7949, 7353, 8351, -4054, 1522,
    -2498, -8639, -5496, 2509, 8901, 9460, -8253, 3398, -4147, 4666,
    1280, 1332, -5116, 5234, 8082, -9831, -1554, 4487, -8691, 244,
    3945, 3213, 2331, 9698, -8421, 8644, 7623, 4582, 2791, 1677,
    2007, -826, -3370, 5899, 8, 6700, -8545, -6639, 8788, -9038,
    -2309, 198, 1926, -6315, -9142, -9344, 8927, 2642, 5397, 2704,
    8120, 1553, -9745, -3243, 7441, 1848, -5353, -6898, -5518, 1099};
  int input;

  input = buffer[sml->ibc];
  sml->ibc++;
  sml->ibc %= MEMSIZE;

  sml->counter++;
  sml->memory[sml->operand] = input;
  return 0;
}
  
int opcode_write()
{
  output_value(sml->memory[sml->operand]);
  sml->counter++;
  return 0;
}
