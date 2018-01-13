#ifndef SML_H
#define SML_H

#include <ncurses.h>

#define MINWIDTH 85
#define MINHEIGHT 24
#define INVALID -1
#define BUFFSIZE 80
#define MEMSIZE 100
#define MAXOP 100
#define MAXVAL 9999
#define MINVAL -9999
#define OPFACT 100

struct machineState;		/* Forward declare */
typedef int (*opPtr)();

/*
 * Not all these opcodes were part of the original spec.
 *   Extended Opcodes Include: INC, DEC, MOD
 *
 *   STEP, GO, STOP, BREAK, CONTINUE are simulator commands
 *   These values will never be valid memory values
 */
enum OPCODES {
  READ = 10, WRITE,
  LOAD = 20, STORE,
  ADD = 30, SUBTRACT, DIVIDE, MULTIPLY, MOD, INC, DEC,
  BRANCH = 40, BRANCHNEG, BRANCHZERO, HALT
};

enum PROCESS_STATES {
  BLANK, NUMBER, ALPHA, ADDRESS, ASSEMBLE, ASSEMBLEHELP,
  STEP=MAXOP+1, GO, STOP, BREAK, CONTINUE, CLEAR
};

/*
 * breaktable will hold a value for each address.
 * If the value is 0, the machine will not break
 * there. If the value is positive, it will decrement
 * and break (so it only stops once--used for stepping).
 * If it is negative it will always break there.
 */
struct machineState {
  int acc;
  int iptr;
  int instr;
  int opcode;
  int operand;
  int wptr;
  int inbuff[MEMSIZE];
  int inbuff_start;
  int inbuff_end;
  int memory[MEMSIZE];
  int breaktable[MEMSIZE];
  opPtr inst_tble[MAXOP];
  bool running;
  bool stepping;
};

struct out_buffer {
  int value;
  struct out_buffer *next;
};

extern struct out_buffer *out_buffer_head;
extern struct out_buffer *out_buffer_tail;
extern int out_buffer_max_length;
extern char line[];
extern int buffptr;
extern struct machineState *sml;
extern WINDOW *memwindow;
extern WINDOW *chipwindow;
extern WINDOW *messagewindow;
extern WINDOW *inputwindow;
extern WINDOW *outputwindow;

// Function Defintions
int init_machine();
int memory_dump();
void error_message(char *message);
bool out_of_bounds(int, int, int);
bool is_valid_address(int address);
int run_loop();
void cleanup();
int init_windows();
void displaymem();
void displaychip();
void displayoutput();
void sig_winch(int);
void sig_int(int);
void updatescreen();
int opcode_branch();
int opcode_branch_neg();
int opcode_branch_zero();
int opcode_nop();
int opcode_halt();
int opcode_invalid();
int opcode_add();
int opcode_subtract();
int opcode_multiply();
int opcode_divide();
int opcode_mod();
int opcode_inc();
int opcode_dec();
int opcode_load();
int opcode_store();
int opcode_read();
int opcode_write();
void process();
void output_value(int);
void resize_out_buffer(int);
int out_buff_len();
bool allowedchar(int);
int token(char *);
bool endcond(char);
bool endstr(char);

#endif
