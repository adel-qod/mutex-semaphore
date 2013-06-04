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
	
	sem_init(&sem, 0, 0);
	
	pthread_create(&producerThread, NULL, producer, NULL);
	pthread_create(&consumerThread, NULL, consumer, NULL);
	
	pthread_join(producerThread, NULL);
	pthread_join(consumerThread, NULL);
	return 0;
}

static void * producer(void *para)
{
/*What I've done here is even worse than in the mutex example 
One possible scenario here is if the consumer went first..
it'll increment the semaphore which means when the producer starts it'll block
and keep blocking until the consumer gave it the control back!!! */
	while(1)
	{
		sem_wait(&sem);
		/* Note that since the initial value for the semaphore 
		was set to 0, we'll have a deadlock here where both threads 
		will be waiting! */
		sharedBuffer[0] = 'a';
		sharedBuffer[1] = '\0';
		sem_post(&sem);
	}
	return NULL;
}

static void * consumer(void *para)
{
/* the disaster goes on
I waited for a signal from the producer and then consumed from the buffer
then signaled again to the waiting thread.... */
	while(1)
	{
		sem_wait(&sem);
		printf("%s\n", sharedBuffer);
		sharedBuffer[0] = '\0';
		sem_post(&sem);
	}
	return NULL;
}
