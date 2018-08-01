# This will make both the programs

SIMC = *.c
SIMH = *.h

sml: $(SIMC) $(SIMH)
	gcc -g -Wall -Winline -o SML $(SIMC) -lncurses 

profile: $(SIMC) $(SIMH)
	gcc -g -pg -no-pie -Wall -O -Winline -o SML $(SIMC) -lncurses 

release: $(SIMC) $(SIMH)
	gcc -Wall -O3 -Winline -o SML $(SIMC) -lncurses 

clean:
	rm *.o

clean-all:
	rm *.o *.core *~
