#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 2

static void * producer(void *);
static void * consumer(void *);

static char sharedBuffer[BUFFER_SIZE];

static pthread_mutex_t mutex;

int main(void)
{
	pthread_t producerThread, consumerThread;
	
	pthread_mutex_init(&mutex, NULL);	
	
	pthread_create(&producerThread, NULL, producer, NULL);
	pthread_create(&consumerThread, NULL, consumer, NULL);
	
	pthread_join(producerThread, NULL);
	pthread_join(consumerThread, NULL);
	return 0;
}

/*Plain and simple this code is not just bugged but is disasterous
Here I have a buffer that I wanted to protect so that it'll 
only be accessed when it's got data
What I got was that it was accessed whenever one thread that was using it let
it go; obviously this means that the consumer in my case can consume many times
before a producer comes along and a producer can produce several times before
a consumer consumes..it all depends on the scheduling */

static void * producer(void *para)
{

	while(1)
	{
		pthread_mutex_lock(&mutex);
		sharedBuffer[0] = 'a';
		sharedBuffer[1] = '\0';
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

static void * consumer(void *para)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		printf("%s\n", sharedBuffer);
		sharedBuffer[0] = '\0';
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
