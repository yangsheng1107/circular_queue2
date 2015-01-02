/*
* This sample is original from delow link
* https://github.com/ashwing920/rtspserver
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "ringfifo.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define DELAY 900
#define MAX_BUF_LEN 1024
#define MAX_BUF_QUEUE 32

void *producer(void *ptr)
{
	int i = 0;
	Node *ringinfo;
	printf("I'm a producer\n");
	while (1) 
	{
		pthread_mutex_lock(&mutex);
		usleep(2*DELAY);
		ringinfo = ringput();
		if(ringinfo != NULL)
		{
			ringinfo->buffer_size = sprintf((char*)ringinfo->buffer,"%02x %02x %02x %02x %02x",i ,i ,i ,i ,i);
			ringinfo->frame_type = 0;
			printf("\x1B[34min  :len:%d,ptr:%x,type:%d,data:%s\033[0m\r\n",ringinfo->buffer_size,(int)ringinfo->buffer,ringinfo->frame_type, ringinfo->buffer);fflush(stdout);
		}

		i = i + 1;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

void *consumer(void *ptr)
{
	Node *ringinfo;
	printf("I'm a consumer\n");
	while(1)
	{
		pthread_mutex_lock(&mutex);
		usleep(DELAY);
		ringinfo = ringget();
		if(ringinfo != NULL)
		{
			printf("\x1B[32mout :len:%d,ptr:%x,type:%d,data:%s\033[0m\r\n",ringinfo->buffer_size,(int)ringinfo->buffer,ringinfo->frame_type, ringinfo->buffer);fflush(stdout);
			memset(ringinfo->buffer,0x00,ringinfo->buffer_size);
			ringinfo->buffer_size = 0;
			ringinfo->frame_type = -1;
		}
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main() {
	pthread_t producer_thread;
	pthread_t consumer_thread; 

	ringinitial(MAX_BUF_QUEUE, MAX_BUF_LEN);

	pthread_mutex_init(&mutex, NULL);

	pthread_create(&consumer_thread,NULL,consumer,NULL);
	pthread_create(&producer_thread,NULL,producer,NULL);
	pthread_join(consumer_thread,NULL);

	pthread_mutex_destroy(&mutex);
	ringfree();
	return 0;
}

