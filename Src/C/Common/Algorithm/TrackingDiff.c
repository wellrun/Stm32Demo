#include "TrackingDiff.h"
#include <stdlib.h>
#include "../Common/EmbedSystem/DefaultEvents.h"

TrackingDiffClass* pTouchTrackingDiff;

TrackingDiffClass* NewTrackingDiffClass(float vel_factor, float pos_factor, float time_factor)
{
	TrackingDiffClass* p = (TrackingDiffClass*) malloc(sizeof(TrackingDiffClass));
	
	if(p == NULL)
		FatalErrorLoop("Lpf1stQ31: Can't create instance!\n\r");
	
	p->pos_factor = pos_factor;
	p->vel_factor = vel_factor;
	p->time_correction = time_factor;
	p->factor_multi = p->vel_factor * p->time_correction;
	
	p->pos_predict = 0;
	p->vel_predict = 0;
	p->error = 0;
	
	return p;
}

void TrackingDiffUpdate(TrackingDiffClass* self, float target)
{
	self->error = target - self->pos_predict;	//the position diff

	//self->vel_predict += (self->error * self->vel_factor) * self->time_correction;
	self->vel_predict += (self->error * self->factor_multi);
	self->pos_predict += (self->vel_predict + self->error * self->pos_factor) * self->time_correction;
}
