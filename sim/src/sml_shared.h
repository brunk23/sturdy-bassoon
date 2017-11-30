#ifndef SML_SHARED_H
#define SML_SHARED_H

#define MEMSIZE 100
#define MAXOP 100
#define MAXVAL 9999
#define MINVAL -9999
#define OPFACT 100

/*
 * Not all these opcodes were part of the original spec.
 *   Extended Opcodes Include: INC, DEC, MOD, DUMP, NOP
 *   A_opcodes are indirect opcodes. They set a flag
 *   indirect in the machine state. The memory location
 *   pointed at is loaded, high 3 digits are added to the
 *   low 3 digits and that is used as the actual memory
 *   location
 */
enum OPCODES {
  READ = 10, WRITE,
  LOAD = 20, STORE,
  ADD = 30, SUBTRACT, DIVIDE, MULTIPLY, MOD, INC, DEC,
  BRANCH = 40, BRANCHNEG, BRANCHZERO, HALT
};

#endif
