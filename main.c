#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int queueSize = 10;
int buffSize = 200;
Queue* initial;
Queue* intermediate;
Queue* final;

int main(){
	initial = createStringQueue(queueSize);
	intermediate = createStringQueue(queueSize);
	final = createStringQueue(queueSize);

	// create reader thread
}
// create reader thread and send std input

void *reader(){
	char buff[200];
	int count = read(0, buff, buffSize);
	while(count > 0){
		if(count < buffSize){
			char* line = malloc(1 + sizeof(char) * count); 
			strncpy(line, buff, count);
			*(line+count) = "\0";
			enqueueString(initial, line);
		}
		count = read(0, buff, buffSize);
	}

}

void *munch1(){
	char* string = dequeueString(initial);
	char* temp = string;
	while((temp = strchr(temp, " ")) != NULL){
		temp* = "*";
		temp++;
	}
	enqueue(intermediate, string);
}

void *munch2(){
	char* string = dequeueString(intermediate);
	for(int i = 0; i < buffSize; i++){
		if(string[i] >= 'a' && string[i] <= 'z'){
			toupper(string[i]);
		}
	}
	enqueueString(final);
}

void *writer(){
	char *print = dequeueString(final);
	fprintf(stdout, "%s\n", print);

}
