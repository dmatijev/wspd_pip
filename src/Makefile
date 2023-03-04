#CC   	= gcc -g
#CLINKER	= gcc -g
CC 	= g++ -std=c++14 -O3 -fPIC #-fsanitize=address
CLINKER = g++ -std=c++14 -O3 -fPIC  #-fsanitize=address
#CC   	= /usr/sww/opt/SUNWspro-4.0/bin/cc -g
#CLINKER	= /usr/sww/opt/SUNWspro-4.0/bin/cc -g
# OPTIONS	= -dalign -xchip=ultra -fast -xtarget=native -xO5 -xarch=v8plus

LIBS 	= -lm

all: wspd

wspd: Makefile main.o wsp.o wsp.h util.o gen.o findwsp.o
	$(CLINKER) -o wspd main.o wsp.o util.o gen.o findwsp.o $(LIBS)

main.o: Makefile main.cpp wsp.h
	$(CC) $(OPTIONS) -c main.cpp

wsp.o: Makefile wsp.cpp wsp.h
	$(CC) $(OPTIONS) -c wsp.cpp

util.o: Makefile util.cpp wsp.h
	$(CC) $(OPTIONS) -c util.cpp

gen.o: Makefile gen.cpp wsp.h
	$(CC) $(OPTIONS) -c gen.cpp

findwsp.o: Makefile findwsp.cpp wsp.h
	$(CC) $(OPTIONS) -c findwsp.cpp

clean:
	-rm -f *.o