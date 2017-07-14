#ifndef INFRARED_H
#define INFRARED_H

#include "EmbedDriverExternal.h"

typedef struct InfraredClass_ {
	uint16_t  high_threshold;
	uint16_t  low_threshold;
	uint16_t  distance;
	uint16_t  volt_val;
}InfraredClass;

void SetInfraredVolt(InfraredClass *self, uint16_t volt);
void InfraredCalcDistByVolt(InfraredClass *self);


#endif  //INFRARED_H
