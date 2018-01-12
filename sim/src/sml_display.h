#ifndef SML_DISPLAY_H
#define SML_DISPLAY_H

#define MINWIDTH 85
#define MINHEIGHT 24

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
void updatescreen();

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
