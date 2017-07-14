#include "Infrared.h"



void SetInfraredVolt(InfraredClass *self, uint16_t volt)
{
	self->volt_val =volt;
}

void InfraredCalcDistByVolt(InfraredClass *self)
{

	if(self->volt_val >= self->high_threshold){
		self->distance = (float)(20.1/((self->volt_val *3.3/4096)-1.767))*10;
	}
	else if((self->volt_val < self->high_threshold) && (self->volt_val > self->low_threshold)){

		self->distance=(float)(38.12/((self->volt_val *3.3/4096)-0.6177))*10;
    }
	else{
		self->distance=(float)(64.1/((self->volt_val *3.3/4096+0.1)))*10;
	}
}


