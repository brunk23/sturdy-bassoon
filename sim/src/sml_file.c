#include <stdio.h>
#include <unistd.h>
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

  if(!access(filename, F_OK)) {
    error_message("It looks like the file exists already.",
		  "Type 'y' to overwrite it. Any other key to cancel.",
		  filename);
    doupdate();
    nodelay(messagewindow, FALSE);
    i = wgetch(messagewindow);
    if( !(i == 'y' || i == 'Y') ) {
      error_message("File",filename,"NOT written.");
      return 0;
    }
  }
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

  error_message("File",filename,"written");
  fclose(fp);
  return 0;
}

int writestate(char *filename) {
  int i, j;
  FILE *fp;

  if(!access(filename, F_OK)) {
    error_message("It looks like the file exists already.",
		  "Type 'y' to overwrite it. Any other key to cancel.",
		  filename);
    doupdate();
    nodelay(messagewindow, FALSE);
    i = wgetch(messagewindow);
    if( !(i == 'y' || i == 'Y') ) {
      error_message("File",filename,"not written.");
      return 0;
    }
  }

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

  error_message("File",filename,"written");
  fclose(fp);
  return 0;
}

int writeprofile(char *filename, struct profile *p) {
  int i, j;
  long cycles = 0;
  FILE *fp;

  if(!access(filename, F_OK)) {
    error_message("It looks like the file exists already.",
		  "Type 'y' to overwrite it. Any other key to cancel.",
		  filename);
    doupdate();
    nodelay(messagewindow, FALSE);
    i = wgetch(messagewindow);
    if( !(i == 'y' || i == 'Y') ) {
      error_message("File",filename,"not written.");
      return 0;
    }
  }

  fp = fopen(filename, "w");
  if( fp == 0 ) {
    return 1;
  }

  fprintf(fp, "instmap:\n");
  j = 0;
  for( i = 0; i < MAXOP; i++ ) {
    cycles += p->instmap[i];
    if( p->instmap[i] > 0 ) {
      fprintf(fp, "%s [%li]", instruction_string(i), p->instmap[i]);
      j++;
      if( j == 5 ) {
	fprintf(fp, "\n");
	j = 0;
      } else {
	fprintf(fp, "\t");
      }
    }
  }

  fprintf(fp, "\n\nTotal Instructions Executed: %li",cycles);
  fprintf(fp, "\n\nMEMORY ACCESS\n       ");

  for(i = 0; i < 9; i++) {
    fprintf(fp, "\t %02i", i);
  }
  fprintf(fp, "\t %02i\n", i);
  for( i = 0; i < MEMSIZE/10; i++ ) {
    fprintf(fp, "%02i\t", i*10);
    for(j = 0; j < 9; j++) {
      fprintf(fp,"%04li\t", p->memmap[i*10+j]);
    }
    fprintf(fp,"%04li\n", p->memmap[i*10+j]);
  }

  fprintf(fp, "\n\nINSTRUCTION HEAT MAP\n       ");

  for(i = 0; i < 9; i++) {
    fprintf(fp, "\t %02i", i);
  }
  fprintf(fp, "\t %02i\n", i);
  for( i = 0; i < MEMSIZE/10; i++ ) {
    fprintf(fp, "%02i\t", i*10);
    for(j = 0; j < 9; j++) {
      fprintf(fp,"%04li\t", p->heatmap[i*10+j]);
    }
    fprintf(fp,"%04li\n", p->heatmap[i*10+j]);
  }

  error_message("File",filename,"written");
  fclose(fp);
  return 0;
}
