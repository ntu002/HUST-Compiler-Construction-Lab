CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: kplrun

kplrun: main.o instructions.o VM_duongnn.o
	${CC} main.o instructions.o VM_duongnn.o -lm -lncurses -o kplrun

main.o: main.c
	${CC} ${CFLAGS} main.c

instructions.o: instructions.c
	${CC} ${CFLAGS} instructions.c

VM_duongnn.o: VM_duongnn.c
	${CC} ${CFLAGS} VM_duongnn.c

clean:
	rm -f *.o *~