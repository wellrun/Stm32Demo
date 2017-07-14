#include "Algorithm.h"

Lpf1stQ31* NewLpf1stQ31(int32_t t_const, int32_t t_const_div)
{
	Lpf1stQ31* p = (Lpf1stQ31*) malloc(sizeof(Lpf1stQ31));
	
	if(p == NULL)
		THROW_MSG("Lpf1stQ31: Can't create instance!\n\r");
	
	p->t_const = t_const;
	p->t_const_div = t_const_div;
	p->prev_output = 0;
	
	return p;
}

int32_t Lpf1stQ31Update(Lpf1stQ31* self, int32_t x)
{
	self->prev_output = (x * ((1 << self->t_const_div) - self->t_const) +
						(self->prev_output * self->t_const)) >> self->t_const_div;
	
	return self->prev_output;
}

Lpf1stF32* NewLpf1stF32(float t_const)
{
	Lpf1stF32* p = (Lpf1stF32*) malloc(sizeof(Lpf1stF32));
	
	if(p == NULL)
		THROW_MSG("Lpf1stF32: Can't create instance!\n\r");
	
	p->t_const = t_const;
	p->t_const_1 = 1.0f - t_const;
	p->prev_output = 0;
	
	return p;
}

float Lpf1stF32Update(Lpf1stF32* self, float x)
{
	self->prev_output = 
		x * self->t_const_1 + self->prev_output * self->t_const;
	
	return self->prev_output;
}

void TrackObsvQ31Update(TrackObsvQ31* self, int32_t x)
{
	
}

CircleRound * NewCircleRoundFilter(int32_t up_limit, int32_t low_limit) 
{
	if(up_limit - low_limit < 32)
	{
		return NULL;
	}
	CircleRound * p = (CircleRound*) malloc(sizeof(CircleRound));
	if(p == NULL)
		THROW_MSG("CircleRoundFilter: Can't create instance!\n\r");
	
	p->up_limit = up_limit;
	p->low_limit = low_limit;
	p->range = up_limit - low_limit;
	
	p->last_output = (up_limit + low_limit) / 2;
	
	return p;
}

int32_t CircleRoundUpdate(CircleRound * self, int32_t x) 
{
	if((int32_t)(x - self->last_output) > (int32_t)(self->range>>1)) {
		self->last_output = self->low_limit - (self->up_limit - x);
	}
	else if((int32_t)(self->last_output - x) > (int32_t)(self->range>>1)) {
		self->last_output = self->up_limit + (x - self->low_limit);
	}
	else {
		self->last_output = x;
	}
	
	return self->last_output;
}


