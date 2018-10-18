#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

const int queueSize = 10;
const int buffSize = 1024;
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
    
    pthread_t reader_t, munch1_t, munch2_t, writer_t;
    
    pthread_create(&reader_t, NULL, reader, NULL);
    pthread_create(&munch1_t, NULL, munch1, NULL);
    pthread_create(&munch2_t, NULL, munch2, NULL);
    pthread_create(&writer_t, NULL, writer, NULL);

    pthread_join(reader_t, NULL);
    pthread_join(munch1_t, NULL);
    pthread_join(munch2_t, NULL);
    pthread_join(writer_t, NULL);

    printQueueStats(initial);
    printQueueStats(intermediate);
	printQueueStats(final);
}
// create reader thread and send std input

void *reader(){
//	fgetc, check for null pointer, end of line \n, EOF,  or if there is no character (empty file)
//	What about
//	Run against an empty file with no end line
	int i = 0;
	char c;
    char* buff = (char*) malloc(buffSize * sizeof(char));
    if(buff == NULL) {
        fprintf(stderr, "Out of Memory.\n");
        exit(1);
    }
	while((c = fgetc(stdin)) != EOF){
		if(i <= buffSize-1){
			if(c == '\n'){
				buff[i] = '\0';
				enqueueString(initial, buff);
                buff = (char*) malloc(buffSize * sizeof(char));
                if (buff == NULL) {
                    fprintf(stderr, "Out of memory.\n");
                    exit(1);
                }
				i = 0;
			} else {
			buff[i] = c;
			i++;
            }
		}else{
			fprintf(stderr, "Error: Input exceeded buffer size.\n");
            while((c = fgetc(stdin)) != '\n' && c != EOF);
            i = 0;
            buff = (char*) malloc(buffSize * sizeof(char));
            if (buff == NULL) {
                fprintf(stderr, "Out of memory.\n");
                exit(1);
            }
		}
	} 
    enqueueString(initial, NULL);
	pthread_exit(0);
}

// make sure enqueue count = dequeue count for new struct

void *munch1(){
	char* string;
	char* temp;
	do {
		string = dequeueString(initial);
		temp = string;
		while(string != NULL && (temp = strchr(temp, ' ')) != NULL){
			*(temp) = '*';
			temp++;
		}
		enqueueString(intermediate, string);
	} while (string != NULL);
	pthread_exit(0);
}

void *munch2(){
	char* string;
    do {
		string = dequeueString(intermediate);
		for(int i = 0; i < buffSize && string != NULL; i++){
			if(string[i] >= 'a' && string[i] <= 'z'){
				string[i] = toupper(string[i]);
			}
		}
		enqueueString(final, string);
	} while (string != NULL);
	pthread_exit(0);
}


void *writer(){
	char* print;
	do {
		print = dequeueString(final);
	    if (print == NULL)
            break;
		fprintf(stdout, "%s\n", print);
        free(print);
	} while (print != NULL);
	pthread_exit(0);
}
