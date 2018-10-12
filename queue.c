//Written by Logan Mahan and Sam Ware
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

Queue* createStringQueue(int size) {
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    if (queue == NULL) {
        fprintf(stderr, "Out of memory\n");
        return NULL;
    }
    queue->buff = malloc(size * sizeof(char*));
    if (queue->buff == NULL) {
        fprintf(stderr, "Out of memory\n");
        return NULL;
    }
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

int incrementVal(int val, int size) {
    return (val + 1) % size;
}

void enqueueString(Queue* queue, char* str) {
    pthread_mutex_lock(&(queue->mutex));
    while (incrementVal(queue->last, queue->size) == queue->first) {
        queue->enqueueBlockCount++;
        pthread_cond_wait(&(queue->full), &(queue->mutex));
    }
    (queue->buff)[queue->last] = str;
    queue->enqueueCount++;
    queue->last = incrementVal(queue->last, queue->size);
    pthread_cond_signal(&(queue->empty));
    pthread_mutex_unlock(&(queue->mutex));    
}

char* dequeueString(Queue* queue) {
    pthread_mutex_lock(&(queue->mutex));
    while (queue->first == queue->last) {
        queue->dequeueBlockCount++;
        pthread_cond_wait(&(queue->empty), &(queue->mutex));
    }
    char* ret = (queue->buff)[queue->first];
    queue->dequeueCount++;
    queue->first = incrementVal(queue->first, queue->size);
    pthread_cond_signal(&(queue->full));   
    pthread_mutex_unlock(&(queue->mutex));
    return ret;
}

void printQueueStats(Queue* queue) {
    fprintf(stderr, "Enqueue Count: %d\n", queue->enqueueCount);
    fprintf(stderr, "Dequeue Count: %d\n", queue->dequeueCount);
    fprintf(stderr, "Enqueue Block Count: %d\n", queue->enqueueBlockCount);
    fprintf(stderr, "Dequeue Block Count: %d\n", queue->dequeueBlockCount);
}    
