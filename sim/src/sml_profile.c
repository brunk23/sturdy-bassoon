#include "sml.h"

void reset_profiling(struct profile *p) {
  int i;
  for( i = 0; i < MEMSIZE; i++ ) {
    p->heatmap[i] = 0;
    p->memmap[i] = 0;
  }
  for( i = 0; i < MAXOP; i++ ) {
    p->instmap[i] = 0;
  }
}

void start_profiling(struct profile *p) {
  p->active = true;
  reset_profiling(p);
}

void stop_profiling(struct profile *p) {
  p->active = false;
}

void profile_log(struct profile *p) {
  p->instmap[ sml->opcode ]++;
  p->memmap[ sml->operand ]++;
  p->heatmap[ sml->iptr ]++;
}

void profile_unlog(struct profile *p) {
  p->instmap[ sml->opcode ]--;
  p->memmap[ sml->operand ]--;
  p->heatmap[ sml->iptr ]--;
}
