#include <stdlib.h>
#include <ctype.h>

#include "sml.h"

/*
 * This will not fail. Program quits instead.
 */
struct io_buffer *new_io_buffer(int size) {
  struct io_buffer *tmp;

  tmp = (struct io_buffer *)malloc(sizeof(struct io_buffer));
  if( 0 != tmp ) {
    tmp->val = (int *)malloc(sizeof(int) * size);
    if( 0 != tmp->val ) {
      tmp->head = 0;
      tmp->size = size;
      tmp->len = 0;
      return tmp;
    }
    free(tmp);
    tmp = 0;
  }
  fprintf(stderr,"ERROR: Could not create i/o buffer.\n");
  exit(1);
}

/*
 * Resize the buffer, if needed.
 * Create a new array for the buffer
 * Copy the old values into the new buffer, restarting at 0.
 * Make sure the new length is correct.
 * free the old buffer
 * save the pointer into the structure.
 */
void resize_io_buffer(struct io_buffer *old, int size) {
  int *tmp;
  int i;

  if( size == old->size ) {
    return;
  }

  tmp = (int *)malloc(sizeof(int) * size);

  if( old->len > size ) {
    old->head += (old->len - size);
    old->head %= old->size;
  }

  if( 0 == tmp ) {
    fprintf(stderr,"ERROR: Could not create i/o buffer.\n");
    exit(1);
  }

  for( i = 0; (i < old->len) && (i < size) ; i++ ) {
    tmp[i] = old->val[ (old->head + i) % old->size ];
  }

  old->head = 0;
  old->len = i;
  old->size = size;

  free(old->val);
  old->val = tmp;
}

int remove_io_value(struct io_buffer *buff) {
  int x;
  x = buff->val[buff->head];
  buff->head++;
  buff->head %= buff->size;
  buff->len--;
  return x;
}

void add_io_value(struct io_buffer *buff, int x) {
  if( buff->len == buff->size ) {
    buff->val[buff->head] = x;
    buff->head++;
    buff->head %= buff->size;
  } else {
    buff->val[(buff->head + buff->len) % buff->size] = x;
    buff->len++;
  }
}

int size_io_buffer(struct io_buffer *buff) {
  return buff->len;
}

int memory_dump()
{
  int i, j;
  printf("REGISTERS\n");
  printf("Accumulator:       \t%04i\n", sml->acc);
  printf("instructionPointer:\t%02i\n", sml->iptr);
  printf("instruction:       \t%04i\n", sml->instr);
  printf("opcode:            \t%02i\n", sml->opcode);
  printf("operand:           \t%02i\n\nMEMORY:", sml->operand);
  for(i = 0; i < 10; i++) {
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
