#ifndef SML_DISPLAY_H
#define SML_DISPLAY_H

extern WINDOW *memwindow;
extern WINDOW *chipwindow;
extern WINDOW *messagewindow;
extern WINDOW *inputwindow;
extern WINDOW *outputwindow;

int init_windows();
void displaymem();
void displaychip();
void sig_winch(int);

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
