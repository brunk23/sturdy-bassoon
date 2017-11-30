# This will make both the programs

SIMC = *.cpp
SIMH = *.h

sml_sim: $(SIMC) $(SIMH)
	c++ -g -Wall -o FCC $(SIMC)

clean:
	rm *.o

clean-all:
	rm *.o *.core *~
