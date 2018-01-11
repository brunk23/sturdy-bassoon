#ifndef SML_DISPLAY_H
#define SML_DISPLAY_H

#define BUFFSIZE 80

extern WINDOW *memwindow;
extern WINDOW *chipwindow;
extern WINDOW *messagewindow;
extern WINDOW *inputwindow;
extern WINDOW *outputwindow;

int init_windows();
void displaymem();
void displaychip();
void displayoutput();
void sig_winch(int);
void sig_int(int);
void process();
void updatescreen();
void output_value(int);
void resize_out_buffer(int);
int out_buff_len();

struct out_buffer {
  int value;
  struct out_buffer *next;
};

extern struct out_buffer *out_buffer_head;
extern struct out_buffer *out_buffer_tail;
extern int out_buffer_max_length;

extern char line[];
extern int buffptr;
/*****
 * Proposed window layout
 * +------------------+----+
 * | chip window      | O  |
 * +------------------+ U  |
 * | message window   | T  |
 * +------------------+ P  |
 * | mem 13 x 75 min  | U  |
 * +------------------+ T  |
 * | input window     |    |
 * +------------------+----+
 *****/

#endif
