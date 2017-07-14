#ifndef RINGBUFFER_H
#define RINGBUFFER_H
#include "Algorithm.h"

#if defined(C_PLATFORM_LINUX) || defined(C_PLATFORM_ANDROID)
#include <pthread.h>
#endif
typedef struct
{
	void* data;			 //Null pointer
	int totoal_size;
	int member_cnt;
	int member_size;
	int head_index;
	int tail_index;
	char is_empty;
	int valid_cnt;

#if defined(C_PLATFORM_LINUX) || defined(C_PLATFORM_ANDROID)
	pthread_mutex_t mutex;   //lock
	pthread_cond_t  cond;    //condition variable
	char sig_latch;       //whether the condition variable is wait? 1 : wait
#endif
}RingBuffer;

typedef enum RingBufferErr{ BufferFull = 0, BufferOk } RingBufferErr;

//private
RingBuffer* NewRingBufferFunc(int member_size, int member_cnt);
RingBuffer* NewRingBufferFromMemFunc(int member_size, int member_cnt, void* src, int src_size);
void RingBufferDeepPushFunc(RingBuffer* self, void* data);
void* RingBuffeShallowPopFunc(RingBuffer* self);

//public
#define NewRingBufferFromMem(member_class, size, src)	NewRingBufferFromMemFunc(sizeof(member_class), size, (void*)src[0], sizeof(src))
#define NewRingBuffer(member_class, size) 		 	NewRingBufferFunc(sizeof(member_class), size)
#define RingBufferDeepPush(self, data) 				RingBufferDeepPushFunc(self, (void*)&data)
#define RingBufferShallowPop(self, member_class)  	(member_class*)RingBuffeShallowPopFunc(self)

char IsRingBufferEmpty(RingBuffer* self);
void RingBufferMappingValid(RingBuffer* self, void (*accessor)(void*));
void RingBufferMappingAll(RingBuffer* self, void (*accessor)(void*));
void RingBufferClear(RingBuffer* self);
void* RingBufferPeekOldest(RingBuffer *self);
void* RingBufferPeekNewest(RingBuffer *self);
void* RingBufferIncHead(RingBuffer* self);
void RingBufferCastFunc(RingBuffer* self, void* out, int out_member_size, int out_member_cnt);
int RingBufferValidCnt(RingBuffer *self);
void RingBufferClear(RingBuffer* self);
char IsRingBufferFull(RingBuffer* self);
void RingBufferTrigger(RingBuffer *self);
void RingBufferWait(RingBuffer *self);
#endif
