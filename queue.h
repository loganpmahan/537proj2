typedef struct Queue Queue;

Queue* createStringQueue(int);
void enqueueString(Queue*, char*);
char* dequeueString(Queue*);
void printQueueStats(Queue*);
