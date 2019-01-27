#ifndef SML_H
#define SML_H

#include <ncurses.h>

#define VERSION "1.20"
#define MINWIDTH 80
#define MINHEIGHT 24
#define BUFFSIZE 80
#define MEMSIZE 100
#define MAXOP 100
#define MAXVAL (MAXOP * MEMSIZE - 1)
#define MINVAL (-1 * (MAXOP * MEMSIZE - 1))
#define OPFACT MEMSIZE

struct machineState;		/* Forward declare */
typedef void (*opPtr)();

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
  BLANK = 10000, NUMBER, ALPHA, ADDRESS,
  ASSEMBLE, ASSEMBLEHELP,
  FILEIO, FILEIOHELP, DUMPPROFILE,
  STEP, GO, STOP, BREAK, CONTINUE, CLEAR, SET,
  DUMPMEM, DUMPSTATE, RESTOREMEM, INVALID, VALID
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

struct profile {
  long heatmap[MEMSIZE];
  long memmap[MEMSIZE];
  long instmap[MAXOP];
  bool active;
};

extern char userline[];
extern struct machineState *sml;
extern struct io_buffer *inbuff;
extern struct io_buffer *outbuff;
extern struct profile *profile_data;
extern WINDOW *memwindow;
extern WINDOW *chipwindow;
extern WINDOW *messagewindow;
extern WINDOW *inputwindow;
extern WINDOW *outputwindow;

// Function Defintions
void init_machine();
void error_message(char *, char *, char *);
bool out_of_bounds(int, int, int);
bool is_valid_address(int);
void run_loop();
void cleanup();
int init_windows();
void displaymem();
void update_mem_addr(int);
void displaychip();
void displayoutput();
void term_resize();
void sig_int(int);
void updatescreen();

void opcode_branch();
void opcode_branch_neg();
void opcode_branch_zero();
void opcode_nop();
void opcode_halt();
void opcode_invalid();
void opcode_add();
void opcode_subtract();
void opcode_multiply();
void opcode_divide();
void opcode_mod();
void opcode_inc();
void opcode_dec();
void opcode_load();
void opcode_store();
void opcode_read();
void opcode_write();

void process(char *);
char *instruction_string(int);

void stop_profiling(struct profile *);
void start_profiling(struct profile *);
void reset_profiling(struct profile *);
void profile_log(struct profile *);
void profile_unlog(struct profile *);
int writeprofile(char *, struct profile *);

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
int numberbetween(char *, int, int);
int getnextword(char *, char *);

#endif
