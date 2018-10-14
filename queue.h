typedef struct Queue{
	char** buff;
	int size;
	int first;
	int last;
	int enqueueCount;
	int dequeueCount;
	int enqueueBlockCount;
	int dequeueBlockCount;
//	p_thread_mutex mutex;
//	p_thread_cond_t full;
//	p_thread_cond_t empty;
}Queue;


Queue* createStringQueue(int);
void enqueueString(Queue*, char*);
char* dequeueString(Queue*);
void printQueueStats(Queue*);
