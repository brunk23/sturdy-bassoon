#ifndef SML_H
#define SML_H

#include <ncurses.h>

#define VERSION "1.04"
#define MINWIDTH 80
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
 */
enum OPCODES {
  READ = 10, WRITE,
  LOAD = 20, STORE,
  ADD = 30, SUBTRACT, DIVIDE, MULTIPLY, MOD, INC, DEC,
  BRANCH = 40, BRANCHNEG, BRANCHZERO, HALT
};

enum PROCESS_STATES {
  BLANK, NUMBER, ALPHA, ADDRESS,
  ASSEMBLE, ASSEMBLEHELP,
  FILEIO, FILEIOHELP,
  STEP=MAXOP+1, GO, STOP, BREAK, CONTINUE, CLEAR, SET,
  DUMPMEM, DUMPSTATE, RESTOREMEM
};

struct machineState {
  int acc;			/* accumulator */
  int iptr;			/* instruction pointer */
  int instr;			/* current instruction code */
  int opcode;			/* the opcode itself */
  int operand;			/* the memory address to work on */
  int memory[MEMSIZE];		/* the memory for the machine */
  bool breaktable[MEMSIZE];	/* the places to break */
  opPtr inst_tble[MAXOP];	/* a jump table for opcodes */
  bool running;			/* is the chip running */
  bool stepping;		/* should we stop after each instruction */
  bool debug;			/* dump the state of the machine at exit */
};

struct io_buffer {
  int size;
  int *val;
  int head;
  int len;
};

extern char userline[];
extern int buffptr;
extern struct machineState *sml;
extern struct io_buffer *inbuff;
extern struct io_buffer *outbuff;
extern WINDOW *memwindow;
extern WINDOW *chipwindow;
extern WINDOW *messagewindow;
extern WINDOW *inputwindow;
extern WINDOW *outputwindow;

// Function Defintions
int init_machine();
void error_message(char *, char *, char *);
bool out_of_bounds(int, int, int);
bool is_valid_address(int);
int run_loop();
void cleanup();
int init_windows();
void displaymem();
void update_mem_addr(int);
void displaychip();
void displayoutput();
void term_resize();
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
void process(char *);

struct io_buffer *new_io_buffer(int);
void resize_io_buffer(struct io_buffer *, int);
int size_io_buffer(struct io_buffer *);
void add_io_value(struct io_buffer *, int);
int remove_io_value(struct io_buffer *);
int memory_dump();

bool allowedchar(int);
int token(char *);
bool endcond(char);
bool endstr(char);
int readfile(char *);
int writefile(char *);
int writestate(char *);

#endif
