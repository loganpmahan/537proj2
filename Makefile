CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = prodcomm
LP = -lpthread
SCAN_BUILD_DIR = scan-build-out

all: main.o queue.o
	$(CC) -o $(EXE) main.o queue.o $(LP)

main.o: main.c queue.h
	$(CC) $(WARNING_FLAGS) -c main.c $(LP)

queue.o: queue.c
	$(CC) $(WARNING_FLAGS) -c queue.c $(LP)

clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html
