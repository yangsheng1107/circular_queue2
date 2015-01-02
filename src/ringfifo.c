/*ringbuf .c*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "ringfifo.h"

static Ring ring;

void ringinitial(int num, int size)
{
	int i;

	ring.ringfifo = malloc (num * sizeof(Node));
	for (i =0;i<num;i++) 
	{
		ring.ringfifo[i].buffer = malloc (size);
		ring.ringfifo[i].buffer_size = 0;
		ring.ringfifo[i].frame_type = -1;
		printf("FIFO INFO:idx:%d,len:%d,ptr:%x\n",i,ring.ringfifo[i].buffer_size,(int)(ring.ringfifo[i].buffer));
	}
	ring.size = num;
	ring.in = 0;
	ring.out = 0;
	ring.count = 0;
}
void ringreset()
{
	ring.in = 0;
	ring.out = 0;
	ring.count = 0;
}
void ringfree()
{
	int i;
	printf("begin free mem\n");
	for (i =0;i<ring.size;i++)
	{
		printf("FREE FIFO INFO:idx:%d,len:%d,ptr:%x\n",i,ring.ringfifo[i].buffer_size,(int)(ring.ringfifo[i].buffer));
		free(ring.ringfifo[i].buffer);
		ring.ringfifo[i].buffer_size = 0;
	}

	free(ring.ringfifo);
}
int addring (int i)
{
	return (i+1) == ring.size ? 0 : i+1;
}


Node * ringget()
{
	int Pos;
	Node *getinfo = NULL;
	if (ring.count>0)
	{
		Pos = ring.out;
		ring.out = addring(ring.out);
		ring.count--;
		getinfo = &ring.ringfifo[Pos];
	}
	else 
	{
		//printf("Buffer is empty\n");
	}
	return getinfo;
}


Node * ringput()
{
	Node *putinfo = NULL;
	if (ring.count<ring.size)
	{
		putinfo = &ring.ringfifo[ring.in];
		ring.in = addring(ring.in);
		ring.count++;
	}
	else 
	{
		//	printf("Buffer is full\n");
	}
	return putinfo;
}

