/*
* This sample is original from delow link
* https://github.com/ashwing920/rtspserver
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include "ringfifo.h"

#define MAX_BUF_LEN 1024
#define MAX_BUF_QUEUE 10

void *producer(void *ptr)
{
	int fd = -1;
	Node *ringinfo;

	printf("I'm a producer\n");

	if ( (fd = open("music.pcm",O_RDONLY)) == -1 )
	{
		perror("open file error");
		pthread_exit(NULL);
	}

	while (1) 
	{
		ringinfo = ringputnode();
		if(ringinfo != NULL)
		{
			ringinfo->buffer_size = read(fd, ringinfo->buffer, MAX_BUF_LEN);
			if(ringinfo->buffer_size <= 0)
				break;
			ringinfo->frame_type = 1;
			printf("\x1B[34min  :len:%d,ptr:%x,type:%d,data:%x %x---%x %x\033[0m\r\n",ringinfo->buffer_size,(int)ringinfo->buffer,ringinfo->frame_type, ringinfo->buffer[0], ringinfo->buffer[1], ringinfo->buffer[ringinfo->buffer_size-2], ringinfo->buffer[ringinfo->buffer_size -1]);fflush(stdout);
			ringput();
		}

	}

	close(fd);
	pthread_exit(NULL);
}

void *consumer(void *ptr)
{
	Node *ringinfo;
	FILE  *handle;

	printf("I'm a consumer\n");

	if ( (handle = fopen("music1.pcm","w")) == NULL  )
	{
		perror("open file error");
		pthread_exit(NULL);
	}

	while(1)
	{

		ringinfo = ringgetnode();
		if(ringinfo != NULL)
		{
			printf("\x1B[32mout :len:%d,ptr:%x,type:%d,data:%x %x---%x %x\033[0m\r\n",ringinfo->buffer_size,(int)ringinfo->buffer,ringinfo->frame_type, ringinfo->buffer[0], ringinfo->buffer[1], ringinfo->buffer[ringinfo->buffer_size-2], ringinfo->buffer[ringinfo->buffer_size -1]);fflush(stdout);
			fwrite( ringinfo->buffer,1,ringinfo->buffer_size,handle);fflush(handle);

			memset(ringinfo->buffer,0x00,ringinfo->buffer_size);
			ringinfo->buffer_size = 0;
			ringinfo->frame_type = -1;
			
			ringget();
		}
	}
	fclose(handle);

	pthread_exit(NULL);
}


int main() {
	pthread_t producer_thread;
	pthread_t consumer_thread; 

	ringinitial(MAX_BUF_QUEUE, MAX_BUF_LEN);

	pthread_create(&producer_thread,NULL,producer,NULL);
	pthread_create(&consumer_thread,NULL,consumer,NULL);
	pthread_join(consumer_thread,NULL);

	ringfree();
	return 0;
}

