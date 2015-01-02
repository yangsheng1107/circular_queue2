#ifndef _RING_FIFI_H__
#define _RING_FIFI_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _node_t {
	unsigned char *buffer;
	int frame_type;
	int buffer_size;
}Node;

typedef struct _Ring_t {
	int in;
	int out;
	int count;
	int size;
	Node *ringfifo;
}Ring;


void ringinitial(int num, int size);
int addring (int i);
Node * ringget();
Node * ringput();
void ringfree();
void ringreset();


#ifdef __cplusplus
}
#endif

#endif //end of _RING_FIFI_H__

