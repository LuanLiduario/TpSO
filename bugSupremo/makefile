all: commander manager clean

commander: commander.o 
	gcc -Wall tpcommander.o -o commander

manager: manager.o
	gcc -Wall tpmanager.o -o manager

commander.o: tpcommander.c
	gcc -Wall -c tpcommander.c

manager.o: tpmanager.c tpmanager.h
	gcc -Wall -c tpmanager.c

clean:
	rm *.o
