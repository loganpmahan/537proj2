// Written by:
// Logan Mahan, NetID: lmahan, CSID: mahan
// Sam Ware, NetID: sware2, CSID: sware
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct Queue{
    char** buff;
    int size;
    int first;
    int last;
    int enqueueCount;
    int dequeueCount;
    int enqueueBlockCount;
    int dequeueBlockCount;
    pthread_mutex_t mutex;
    pthread_cond_t full;
    pthread_cond_t empty;
}Queue;

/*
* Creates and allocates a queue with the given size
*/
Queue* createStringQueue(int size) {
    // Allocate space for queue
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    if (queue == NULL) {
        fprintf(stderr, "Out of memory\n");
        return NULL;
    }
    queue->buff = malloc(size * sizeof(char*));
    if (queue->buff == NULL) {
        free(queue);
        fprintf(stderr, "Out of memory\n");
        return NULL;
    }

    // Initialize all values in queue to default;
    queue->size = size;
    queue->first = 0;
    queue->last = 0;
    queue->enqueueCount = 0;
    queue->dequeueCount = 0;
    queue->enqueueBlockCount = 0;
    queue->dequeueBlockCount = 0;
    pthread_mutex_init(&(queue->mutex), NULL);
    pthread_cond_init(&(queue->full), NULL);
    pthread_cond_init(&(queue->empty), NULL);
    return queue;
}

/*
* Increment val so that it circles around to 0 on overflow of size
* e.g. if val = size - 1, this would return 0
*/
int incrementVal(int val, int size) {
    return (val + 1) % size;
}

/*
* Enqueue a string to a queue. Blocks when queue is full and only allows
* one thread to enqueue/dequeue to a queue at one time
*/
void enqueueString(Queue* queue, char* str) {
    pthread_mutex_lock(&(queue->mutex));

    // Blocks when queue is full
    while (incrementVal(queue->last, queue->size) == queue->first) {
        queue->enqueueBlockCount++;
        pthread_cond_wait(&(queue->full), &(queue->mutex));
    }
    (queue->buff)[queue->last] = str;

    // Doesn't increment enqueueCount on termination
    if (str != NULL) {
        queue->enqueueCount++;
    }

    queue->last = incrementVal(queue->last, queue->size);
    pthread_cond_signal(&(queue->empty));
    pthread_mutex_unlock(&(queue->mutex));    
}

/*
* Dequeue a string from a queue. Blocks when queue is empty and only allows
* one thread to enqueue/dequeue to a queue at one time
*/
char* dequeueString(Queue* queue) {
    pthread_mutex_lock(&(queue->mutex));

    // Blocks when queue is empty
    while (queue->first == queue->last) {
        queue->dequeueBlockCount++;
        pthread_cond_wait(&(queue->empty), &(queue->mutex));
    }
    char* ret = (queue->buff)[queue->first];

    // Doesn't increment dequeueCount on termination
    if (ret != NULL) {
        queue->dequeueCount++;
    }
    queue->first = incrementVal(queue->first, queue->size);
    pthread_cond_signal(&(queue->full));   
    pthread_mutex_unlock(&(queue->mutex));
    return ret;
}

/*
* Prints out all the stats for the queue
*/
void printQueueStats(Queue* queue) {
    fprintf(stderr, "Enqueue Count: %d\n", queue->enqueueCount);
    fprintf(stderr, "Dequeue Count: %d\n", queue->dequeueCount);
    fprintf(stderr, "Enqueue Block Count: %d\n", queue->enqueueBlockCount);
    fprintf(stderr, "Dequeue Block Count: %d\n", queue->dequeueBlockCount);
}    
