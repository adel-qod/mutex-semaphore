#include <semaphore.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 2

static void * producer(void *);
static void * consumer(void *);

static char sharedBuffer[BUFFER_SIZE];

static sem_t sem;

int main(void)
{
	pthread_t producerThread, consumerThread;
	
	sem_init(&sem, 0, 1);
	
	pthread_create(&producerThread, NULL, producer, NULL);
	pthread_create(&consumerThread, NULL, consumer, NULL);

	pthread_join(producerThread, NULL);
	pthread_join(consumerThread, NULL);
	return 0;
}

static void * producer(void *para)
{
	while(1)
	{
		sharedBuffer[0] = 'a';
		sharedBuffer[1] = '\0';
		sem_post(&sem);
	}
	return NULL;
}

static void * consumer(void *para)
{
	while(1)
	{
		sem_wait(&sem);
		printf("%s\n", sharedBuffer);
		sharedBuffer[0] = '\0';
	}
	return NULL;
}
