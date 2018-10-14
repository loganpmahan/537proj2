#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

int queueSize = 10;
int buffSize = 20;
Queue* initial;
Queue* intermediate;
Queue* final;

void* reader();
void* munch1();
void* munch2();
void* writer();

int main(){
	initial = createStringQueue(queueSize);
	intermediate = createStringQueue(queueSize);
	final = createStringQueue(queueSize);
	reader();
	munch1();
	munch2();
	writer();
	// create reader thread
	printQueueStats(final);
}
// create reader thread and send std input

void *reader(){
	char buff[buffSize];
	while(fgets(buff, buffSize, stdin) != NULL){
//		printf("%s\n", buff);
		if((strlen(buff) < (size_t)buffSize) && (buff[buffSize] == '\0')){
			char* line = malloc(1 + strlen(buff));
			strncpy(line, buff, strlen(buff));
//			printf("%s\n", line);
			enqueueString(initial, line);
		}
			
	}
	return NULL;

}

// make sure enqueue count = dequeue count for new struct

void *munch1(){
	char* string;
	char* temp;
	while((initial->enqueueCount) > (intermediate->enqueueCount)){
	string = dequeueString(initial);
	temp = string;
	while((temp = strchr(temp, ' ')) != NULL){
		*(temp) = '*';
		temp++;
	}
	enqueueString(intermediate, string);
	}
	return NULL;
}

void *munch2(){
	char* string;
	while((intermediate->enqueueCount) > (final->enqueueCount)){
	string = dequeueString(intermediate);
	for(int i = 0; i < buffSize; i++){
		if(string[i] >= 'a' && string[i] <= 'z'){
			string[i] = toupper(string[i]);
		}
	}
	enqueueString(final, string);
	}
	return NULL;
}


void *writer(){
	char* print;
	while((final->enqueueCount) > (final->dequeueCount)){
	print = dequeueString(final);
	// print out all contents of the queue
	fprintf(stdout, "%s", print);
	}
	return NULL;
}
