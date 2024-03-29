all: commander manager reporter clean

commander: commander.o 
	gcc -Wall tpcommander.o -o commander

manager: manager.o
	gcc -Wall tpmanager.o -o manager

reporter: reporter.o
	gcc -Wall tpreporter.o -o reporter

commander.o: tpcommander.c
	gcc -Wall -c tpcommander.c

manager.o: tpmanager.c tpmanager.h
	gcc -Wall -c tpmanager.c

reporter.o: tpreporter.c 
	gcc -Wall -c tpreporter.c

clean:
	rm *.o
