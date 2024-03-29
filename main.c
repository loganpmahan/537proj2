// Written by:
// Logan Mahan, NetID: lmahan, CSID: mahan
// Sam Ware, NetID: sware2, CSID: sware
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

// Created a struct to hold the queues that are being passed to each thread of code
typedef struct QueueHolder {
    Queue* initial;
    Queue* intermediate;
    Queue* final;
}QueueHolder;

const int queueSize = 10; // Size of the newly initialized queue
const int buffSize = 1024; // Size of buffer to read input

void* reader();
void* munch1();
void* munch2();
void* writer();

/*
* Initializes the queue struct as well as the threads that the queues are
* run to read the input data and change it accordingly
*/
int main(){
    QueueHolder *qh = (QueueHolder*) malloc(sizeof(QueueHolder));
    if (qh == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(1);
    }

    // Initialize three queues for the threads
	qh->initial = createStringQueue(queueSize);
	qh->intermediate = createStringQueue(queueSize);
	qh->final = createStringQueue(queueSize);
    
    if (qh->initial == NULL || qh->intermediate == NULL || qh->final == NULL) {
        exit(1);
    }
    
    pthread_t reader_t, munch1_t, munch2_t, writer_t;
    
    pthread_create(&reader_t, NULL, reader, (void*) qh);
    pthread_create(&munch1_t, NULL, munch1, (void*) qh);
    pthread_create(&munch2_t, NULL, munch2, (void*) qh);
    pthread_create(&writer_t, NULL, writer, (void*) qh);

    pthread_join(reader_t, NULL);
    pthread_join(munch1_t, NULL);
    pthread_join(munch2_t, NULL);
    pthread_join(writer_t, NULL);    

    fprintf(stderr, "-First queue stats-\n");
    printQueueStats(qh->initial);
    fprintf(stderr, "-Second queue stats-\n");
    printQueueStats(qh->intermediate);
    fprintf(stderr, "-Third queue stats-\n");
	printQueueStats(qh->final);
}

/*
* Reads in a line from stdin and enqueues it to the initial queue
* Only accepts lines of size less than buffSize
*/
void *reader(void* arg){
    QueueHolder *qh = (QueueHolder*) arg;
	int i = 0;
	char c;
    char* buff = (char*) malloc(buffSize * sizeof(char));
    if(buff == NULL) {
        fprintf(stderr, "Out of Memory.\n");
        enqueueString(qh->initial, NULL);
        pthread_exit(0);
    }
	while((c = fgetc(stdin)) != EOF){
		if(i <= buffSize-1){
			if(c == '\n'){ // Detects the end of a line
				buff[i] = '\0';
				enqueueString(qh->initial, buff);
                buff = (char*) malloc(buffSize * sizeof(char));
                if (buff == NULL) {
                    fprintf(stderr, "Out of memory.\n");
                    enqueueString(qh->initial, NULL);
                    pthread_exit(0);
                }
				i = 0; 
			} else {
			    buff[i] = c;
			    i++;
            }
		} else { // If buffer size is exceeded
			fprintf(stderr, "Error: Input exceeded buffer size.\n");
            // Flush the rest of the line
            while((c = fgetc(stdin)) != '\n' && c != EOF);
            i = 0;
            buff = (char*) malloc(buffSize * sizeof(char));
            if (buff == NULL) {
                fprintf(stderr, "Out of memory.\n");
                enqueueString(qh->initial, NULL);
                pthread_exit(0);
            }
		}
	}
    // If the EOF was encountered at the end of a line, enqueue the line
    if (i != 0) {
        if (i <= buffSize - 1) {
            buff[i] = '\0';
            enqueueString(qh->initial, buff);
        } else {
            fprintf(stderr, "Error: Input exceeded buffer size.\n");
        }
    } 
    
    // Enqueues a NULL string to signal termination
    enqueueString(qh->initial, NULL);
	pthread_exit(0);
}

/*
* Gets a line from the initial queue and replaces spaces with "*" and sends
* to the intermediate queue
*/
void *munch1(void* arg){
    QueueHolder *qh = (QueueHolder*) arg;
	char* string;
	char* temp;
	do {
		string = dequeueString(qh->initial);
		temp = string;
        // Finds every space in the line and replaces with '*'
		while(string != NULL && (temp = strchr(temp, ' ')) != NULL){
			*(temp) = '*';
			temp++;
		}
		enqueueString(qh->intermediate, string);
	} while (string != NULL);
	pthread_exit(0);
}

/*
* Gets a line from the intermediate queue and sets everything to uppercase
* and sends to the final queue
*/
void *munch2(void* arg){
    QueueHolder *qh = (QueueHolder*) arg;
	char* string;
    do {
		string = dequeueString(qh->intermediate);
		for(int i = 0; i < buffSize && string != NULL; i++){
			if(string[i] >= 'a' && string[i] <= 'z'){
				string[i] = toupper(string[i]);
			}
		}
		enqueueString(qh->final, string);
	} while (string != NULL);
	pthread_exit(0);
}

/*
* Gets a line from the final queue and writes it to stdin, then free it
*/
void *writer(void* arg){
	QueueHolder *qh = (QueueHolder*) arg;
    char* print;
    int numStringsProcessed = 0;
	do {
		print = dequeueString(qh->final);
	    if (print == NULL)
            break;
		fprintf(stdout, "%s\n", print);
        numStringsProcessed++;
        free(print);
	} while (print != NULL);
    fprintf(stdout, "Number of Strings Processed: %d\n", numStringsProcessed);
	pthread_exit(0);
}
