#include "Com/Com.h"
#include "Algorithm.h"

extern void FatalErrorLoop(char* info);

uint16_t mRingBuffSize;

RingBuffer* NewRingBufferFromMemFunc(int member_size, int member_cnt, void* src, int src_size)
{
	if(src == NULL)
		return NULL;

	if(src_size < sizeof(RingBuffer) + member_size * member_cnt)
		return NULL;

	RingBuffer* p = (RingBuffer*)src;
	p->head_index = 0;
	p->tail_index = 0;
	p->member_cnt = member_cnt;
	p->member_size = member_size;
	p->totoal_size = member_cnt * member_size;
	p->is_empty = 1;
	p->valid_cnt = 0;
	p->data = (void*)((uint8_t*)src + sizeof(RingBuffer) + 1);

	mRingBuffSize = sizeof(RingBuffer) + p->totoal_size;
	
	memset(p->data, 0, p->totoal_size);

	return p;
}

RingBuffer* NewRingBufferFunc(int member_size, int member_cnt)
{
	RingBuffer* p = (RingBuffer*)malloc(sizeof(RingBuffer));

	if (p == NULL){
		FatalErrorLoop((char*)"RingBuffer: instance create failed...\n\r");
		return p;
	}

		
	p->head_index = 0;
	p->tail_index = 0;
	p->member_cnt = member_cnt;
	p->member_size = member_size;
	p->totoal_size = member_cnt * member_size;
	p->is_empty = 1;
	p->valid_cnt = 0;
	p->data = malloc(p->totoal_size);
	
	if(p->data == NULL){
		FatalErrorLoop((char*)"RingBuffer: instance create failed...\n\r");
		return p;
	}
	
	memset(p->data, 0, p->totoal_size);

#if defined(C_PLATFORM_LINUX) || defined(C_PLATFORM_ANDROID)
    pthread_mutex_init(&(p->mutex), NULL);
	pthread_cond_init(&(p->cond), NULL);
	p->sig_latch = false;
#endif

	return p;
}

void RingBufferDeepPushFunc(RingBuffer* self, void* data)
{
	uint8_t * p = (uint8_t*)self->data;
	if (NULL == data || NULL == self)
		return;

	memcpy(p + self->head_index * self->member_size, data, self->member_size);

	self->head_index++;

	if(self->valid_cnt < self->member_cnt)
		self->valid_cnt ++;

	if (self->head_index >= self->member_cnt) {
		self->head_index = 0;
	}

	if (self->head_index == self->tail_index) {
		self->tail_index++;
		//self->valid_cnt --;

		if (self->tail_index >= self->member_cnt)
			self->tail_index = 0;
	}


    self->is_empty = 0;
}

void* RingBufferIncHead(RingBuffer* self)
{
	uint8_t * p = (uint8_t*)self->data;

	p = p + self->head_index * self->member_size;

	self->head_index++;

	if(self->valid_cnt < self->member_cnt)
		self->valid_cnt ++;

	if (self->head_index >= self->member_cnt) {
		self->head_index = 0;
	}

	if (self->head_index == self->tail_index) {
		self->tail_index++;

		if (self->tail_index >= self->member_cnt)
			self->tail_index = 0;
	}

	self->is_empty = 0;

	return  (void*)p;
}

void* RingBuffeShallowPopFunc(RingBuffer* self)
{
	uint8_t * p = (uint8_t*)self->data;

	if(self->is_empty){
		return NULL;
	}

	p += self->member_size * self->tail_index;

	if (self->tail_index != self->head_index){
		self->tail_index++;
	}

	if (self->tail_index >= self->member_cnt)
		self->tail_index = 0;
    
    if(self->tail_index == self->head_index)
        self->is_empty = 1;

	if(self->valid_cnt > 0)
		self->valid_cnt --;

	return (void*)p;
}


void* RingBufferPeekOldest(RingBuffer *self)
{
	uint8_t * p = (uint8_t*)self->data;

	p += self->member_size * self->tail_index;

	return (void*)p;
}

void* RingBufferPeekNewest(RingBuffer *self)
{
	uint8_t * p = (uint8_t*)self->data;

	p += self->member_size * self->head_index;

	return (void*)p;
}

void RingBufferMappingValid(RingBuffer *self, void (*accessor)(void *))
{
	uint8_t* head_ptr = (uint8_t*)self->data;
	uint8_t* byte_ptr;

	int valid_cnt = RingBufferValidCnt(self);
	int ptr_cnt = self->tail_index;

	for(int i = 0; i < valid_cnt; i ++){
		byte_ptr = head_ptr + ptr_cnt * self->member_size;

		if(ptr_cnt >= self->member_cnt)
			ptr_cnt = 0;

		accessor((void*)(byte_ptr));

		ptr_cnt ++;
	}
}

void RingBufferMappingAll(RingBuffer *self, void (*accessor)(void *))
{
    uint8_t* byte_ptr = (uint8_t*)self->data;
    for(int i = 0; i < self->member_cnt; i ++){
        accessor((void*) (byte_ptr + i * self->member_size));
    }
}

void RingBufferCastFunc(RingBuffer* self, void* out, int out_member_size, int out_member_cnt)
{
	//TODO: Imply RinbufferCasting
	if(out == NULL)
		return;
	if(self->totoal_size > out_member_cnt * out_member_size)
		return;
}

int RingBufferValidCnt(RingBuffer *self)
{
	return self->valid_cnt;
}

void RingBufferClear(RingBuffer* self)
{
	self->head_index = 0;
	self->tail_index = 0;
	self->is_empty = 1;
}

char IsRingBufferEmpty(RingBuffer* self)
{
	if(NULL == self){
		return -1;
	}

	if (self->is_empty)
		return 1;
	else
		return 0;
}

char IsRingBufferFull(RingBuffer* self)
{
	if(NULL == self){
		return -1;
	}

	if (self->valid_cnt >= self->member_cnt)
		return 1;
	else
		return 0;
}

#if defined(C_PLATFORM_LINUX) || defined(C_PLATFORM_ANDROID)
void RingBufferTrigger(RingBuffer *self)
{
	pthread_mutex_lock(&self->mutex);
	self->sig_latch = true;
	pthread_cond_signal(&self->cond);       //Resume the Ringbuffer
	pthread_mutex_unlock(&self->mutex);
}
#endif

#if defined(C_PLATFORM_LINUX) || defined(C_PLATFORM_ANDROID)
void RingBufferWait(RingBuffer *self)
{
	pthread_mutex_lock(&self->mutex);

	//The is_empty flag must be checked. Otherwise, when the last item have been popped
    // and this RingBufferWait method is called, only check sig_latch may not trigger
    //the cond_wait. Because sig_latch may be set by the LAST producer, and since the
    //RingBufferWait has not been called in the consumer loop, the sig_latch will be TRUE,
    //and no cond_wait is called.
	if(!self->sig_latch|| self->is_empty){
		pthread_cond_wait(&self->cond, &self->mutex);       //Resume the DataScheduler
	}
    self->sig_latch = false;

	pthread_mutex_unlock(&self->mutex);

}
#endif

void RingBufferDestroy(RingBuffer *self)
{
#if defined(C_PLATFORM_LINUX) || defined(C_PLATFORM_ANDROID)
	pthread_cond_destroy(&self->cond);
	pthread_mutex_destroy(&self->mutex);
#endif
	free(self->data);
	free(self);
}

