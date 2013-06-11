#include <semaphore.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 2

static void * producer(void *);
static void * consumer(void *);

static char sharedBuffer[BUFFER_SIZE];

static sem_t sem;
static sem_t sem2;
int main(void)
{
	pthread_t producerThread, consumerThread;

	sem_init(&sem, 0, 0);/* inited with 0 so consumer waits at first */
	sem_init(&sem2, 0, 1);/* 1 because producer should start first */
	pthread_create(&producerThread, NULL, producer, NULL);
	pthread_create(&consumerThread, NULL, consumer, NULL);

	pthread_join(producerThread, NULL);
	pthread_join(consumerThread, NULL);
	return 0;
}

static void * producer(void *para)
{
	int x;
	while(1)
	{
		/*
		1- Wait till a consumer consumes
		2- Fill in the buffer
		3- Signal out to the consumer that we're done
		4- Repeat 
		*/
		sem_wait(&sem2);
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
		/* 1- Wait for the producer
		   2- consume 
		   3- Singal to the producer that we've consumed
		   4- Repeat
		 */
		sem_wait(&sem);
		printf("%s\n", sharedBuffer);
		sharedBuffer[0] = '\0';
		sem_post(&sem2);
	}
	return NULL;
}
