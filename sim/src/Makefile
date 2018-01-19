# This will make both the programs

SIMC = *.c
SIMH = *.h

sml: $(SIMC) $(SIMH)
	gcc -g -Wall -lncurses -o SML $(SIMC)

profile: $(SIMC) $(SIMH)
	gcc -g -pg -no-pie -Wall -lncurses -o SML $(SIMC)

release: $(SIMC) $(SIMH)
	gcc -Wall -lncurses -O3 -o SML $(SIMC)

clean:
	rm *.o

clean-all:
	rm *.o *.core *~
