#include <stdio.h>
#include "sml.h"

int readfile(char *filename) {
  int i;
  FILE *fp;
  char fileline[BUFFSIZE+1];

  fp = fopen(filename, "r");
  if( fp == 0 ) {
    return 1;
  }
  sml->iptr = 0;

  while( fgets(fileline, BUFFSIZE, fp) ) {
    for( i = 0; i < BUFFSIZE; i++ ) {
      if( fileline[i] == '\n' ) {
	fileline[i] = 0;
	break;
      }
    }
    process(fileline);
  }

  fclose(fp);
  return 0;
}

int writefile(char *filename) {
  int i, j;
  FILE *fp;
  fp = fopen(filename, "w");
  if( fp == 0 ) {
    return 1;
  }

  for( i = 0; i < MEMSIZE/10; i++ ) {
    for(j = 0; j < 9; j++) {
      fprintf(fp,"%04i\t", sml->memory[i*10+j]);
    }
    fprintf(fp,"%04i\n", sml->memory[i*10+j]);
  }

  fclose(fp);
  return 0;
}

int writestate(char *filename) {
  int i, j;
  FILE *fp;
  fp = fopen(filename, "w");
  if( fp == 0 ) {
    return 1;
  }

  fprintf(fp, "REGISTERS\n");
  fprintf(fp, "Accumulator:       \t%04i\n", sml->acc);
  fprintf(fp, "instructionPointer:\t%02i\n", sml->iptr);
  fprintf(fp, "instruction:       \t%04i\n", sml->instr);
  fprintf(fp, "opcode:            \t%02i\n", sml->opcode);
  fprintf(fp, "operand:           \t%02i\n\nMEMORY:", sml->operand);
  for(i = 0; i < 9; i++) {
    fprintf(fp, "\t %02i", i);
  }
  fprintf(fp, "\t %02i\n", i);
  for( i = 0; i < MEMSIZE/10; i++ ) {
    fprintf(fp, "%02i\t", i*10);
    for(j = 0; j < 9; j++) {
      fprintf(fp,"%04i\t", sml->memory[i*10+j]);
    }
    fprintf(fp,"%04i\n", sml->memory[i*10+j]);
  }

  fclose(fp);
  return 0;
}
