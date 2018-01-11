# This will make both the programs

SIMC = *.c
SIMH = *.h

sml_sim: $(SIMC) $(SIMH)
	gcc -g -Wall -Wpedantic -lncurses -o FCC $(SIMC)

clean:
	rm *.o

clean-all:
	rm *.o *.core *~
