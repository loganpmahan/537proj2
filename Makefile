CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = prodcomm
LP = -lpthread

all: main.o queue.o
	$(CC) -o $(EXE) main.o queue.o $(LP)

main.o: main.c queue.h
	$(CC) $(WARNING_FLAGS) -c main.c $(LP)

queue.o: queue.c
	$(CC) $(WARNING_FLAGS) -c queue.c $(LP)

clean:
	rm $(EXE) *.o


