#ifndef SML_OUTPUT_H
#define SML_OUTPUT_H

#define INVALID -1
#define BUFFSIZE 80

void process();
void output_value(int);
void resize_out_buffer(int);
int out_buff_len();
int token(char *);

struct out_buffer {
  int value;
  struct out_buffer *next;
};

extern struct out_buffer *out_buffer_head;
extern struct out_buffer *out_buffer_tail;
extern int out_buffer_max_length;

extern char line[];
extern int buffptr;

#endif
