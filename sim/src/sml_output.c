#include <stdlib.h>
#include <ctype.h>

#include "sml.h"

struct out_buffer *out_buffer_head;
struct out_buffer *out_buffer_tail;
int out_buffer_max_length;

/*
 * As items come in, they are added to the tail.
 * If if list is larger than the max length, throw out
 * first value.
 * exit if malloc fails.
 */
void output_value(int value) {
  int curr_length;
  struct out_buffer *tmp;

  if( out_buffer_max_length == 0 ) {
    exit(1);
  }

  curr_length = out_buff_len();
  if( curr_length == out_buffer_max_length ) {
    tmp = out_buffer_head;
    out_buffer_head = out_buffer_head->next;
    free(tmp);
  }

  if( 0 == (tmp = (struct out_buffer *)malloc(sizeof(struct out_buffer)))) {
    exit(1);
  }

  tmp->value = value;
  tmp->next = 0;

  if( out_buffer_head == 0 ) {
    out_buffer_head = tmp;
    out_buffer_tail = tmp;
    return;
  }

  out_buffer_tail->next = tmp;
  out_buffer_tail = tmp;
}

/*
 * This is only called when the program starts and on
 * resize events
 */
void resize_out_buffer(int max) {
  int i, curr_length = 0;
  struct out_buffer *tmp;

  curr_length = out_buff_len();
  out_buffer_max_length = max;

  if( curr_length > max ) {
    for(i = 0; i < (curr_length - max); i++) {
      tmp = out_buffer_head;
      out_buffer_head = out_buffer_head->next;
      free(tmp);
    }
  }
}

int out_buff_len() {
  int curr_length = 0;
  struct out_buffer *tmp = out_buffer_head;

  while(tmp) {
    curr_length++;
    tmp = tmp->next;
  }

  return curr_length;
}

int memory_dump()
{
  int i, j;
  printf("REGISTERS\n");
  printf("Accumulator:\t\t%04i\n", sml->accumulator);
  printf("instructionPointer:\t%02i\n", sml->counter);
  printf("instructionRegister:\t%04i\n", sml->instructionRegister);
  printf("operationCode:\t\t%02i\n", sml->operationCode);
  printf("operand:\t\t%02i\n\nMEMORY:", sml->operand);
  for(i = 0; i < 10; ++ i) {
    printf("  %02i   ", i);
  }
  printf("\n");
  for(i = 0; i < MEMSIZE/10 ; ++i) {
    printf("   %02i  ", i*10);
    for(j = 0; j < 10; ++j) {
      if(sml->memory[i*10+j] < 0 ) {
	printf("-%04i  ",-sml->memory[i*10+j]);
      } else {
	printf(" %04i  ",sml->memory[i*10+j]);;
      }
    }
    printf("\n");
  }
  return 0;
}
