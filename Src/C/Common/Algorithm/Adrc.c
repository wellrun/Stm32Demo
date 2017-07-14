#include "Algorithm.h"

#define FAL_SIGMA 0.1f
#define FAL_SIGMA_EXP05 0.316f
float Fal(float e)
{
	if(e < FAL_SIGMA) return e / FAL_SIGMA_EXP05;
	
	return FastSqrtF32(ABS(e)) * SIGN(e);
}

void DisturbObsvNF32Update(	DisturbObsvF32Class* self, 
							float ctrl_efrt, float sys_out)
{
	self->est_err = sys_out - self->state[0];
	
	self->state[0] += (Fal(self->est_err) * self->state_gain[0]
						+ ctrl_efrt * self->input_gain 
						+ self->disturb_estim) * self->samp_time;
	
	self->disturb_estim += (Fal(self->est_err) * self->disturb_gain) * self->samp_time;
}

void DisturbObsv2F32Update(	DisturbObsvF32Class* self, 
							float ctrl_efrt, float sys_out)
{
	self->est_err = sys_out - self->state[0];
	
	self->state[1] += (self->est_err * self->state_gain[1]
						+ ctrl_efrt * self->input_gain 
						+ self->disturb_estim) * self->samp_time;
	
	self->state[0] += (self->est_err * self->state_gain[0]
						+ self->state[1]) * self->samp_time;
	
	self->disturb_estim += (self->est_err * self->disturb_gain) * self->samp_time;
}


void DisturbObsvF32Update(	DisturbObsvF32Class* self, 
							float ctrl_efrt, float sys_out)
{
	self->est_err = sys_out - self->state[0];
	
	self->state[0] += (self->est_err * self->state_gain[0]
						+ ctrl_efrt * self->input_gain 
						+ self->disturb_estim) * self->samp_time;
	
	self->disturb_estim += (self->est_err * self->disturb_gain) * self->samp_time;
}

float AdrcF32Update(AdrcF32Class* self, float target, float feedback)
{
	float output, derr;
	
	self->err = target - feedback;
	
	derr = self->err_prev - self->err;
	
	self->ctrl_efrt = self->err * self->k_p + derr * self->k_d;
	
	
	self->err_prev = self->err;
	
	if(self->obsv->samp_time == 0) 
		self->obsv->samp_time = self->samp_time;
	
	output = self->ctrl_efrt - self->obsv->disturb_estim * self->k_obsv;
	
	RangeLimit(output, -self->out_limit, self->out_limit);
	
	DisturbObsvNF32Update(self->obsv, output, feedback);
	
//	DisturbObsvF32Update(self->obsv, output, feedback);
//	DisturbObsv2F32Update(self->obsv, self->ctrl_efrt, feedback);
	
//	output = self->ctrl_efrt;
	RangeLimit(output, -self->out_limit, self->out_limit);
	
	return output;
}	
