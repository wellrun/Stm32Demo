#ifndef ADRC_H_
#define ADRC_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Algorithm.h"

#define ADRC_MAX_ORDER 2
typedef struct
{
	float est_err;
	float state[ADRC_MAX_ORDER];
	float disturb_estim;
	float state_gain[ADRC_MAX_ORDER];
	float disturb_gain;
	float input_gain;
	float samp_time;
} DisturbObsvF32Class;

typedef struct
{
	DisturbObsvF32Class* obsv;
	float k_p;
	float k_d;
	float k_obsv;
	float err;
	float err_prev;
	float ctrl_efrt;
	float out_limit;
	float samp_time;
} AdrcF32Class;

void DisturbObsvF32Update(DisturbObsvF32Class* self, float ctrl_efrt, float sys_out);
float AdrcF32Update(AdrcF32Class* self, float target, float feedback);
#ifdef __cplusplus
}
#endif

#endif // ADRC_H_
