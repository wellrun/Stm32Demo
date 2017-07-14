#include "Algorithm.h"


int32_t PidIncrementalQ31(PidQ31Class* self, int32_t target, int32_t feedback)
{
	int32_t err, derr, d2err;
    
    err = target - feedback;
    derr = err - self->err_prev;
    d2err = derr - self->derr_prev;
    
    self->output_integrator += (self->k_p * derr) >> self->k_p_div;
    self->output_integrator += (self->k_i * err)  >> self->k_i_div;
    self->output_integrator += (self->k_d * d2err) >> self->k_d_div;
    
    if(self->output_integrator > self->output_limit) {
        self->output_integrator = self->output_limit;
    }
    
    if(self->output_integrator < (-self->output_limit)) {
        self->output_integrator = -self->output_limit;
    }
    
    self->err_prev = err;
    self->derr_prev = derr;
    
    return self->output_integrator;
}

int32_t PidPositionalQ31(PidQ31Class* self, int32_t target, int32_t feedback)
{
	int32_t err, derr, output;

	err = target - feedback;
    derr = err - self->err_prev;
	
	if(self->diff_lpf != NULL)
		derr = Lpf1stQ31Update(self->diff_lpf, derr);
	
    self->integrator += err;
	if(self->intg_limit != 0)
		RangeLimit(self->integrator, -self->intg_limit, self->intg_limit);
	
	
    output = ((self->k_p * err) >> self->k_p_div)
				+ ((self->k_i * self->integrator) >> self->k_i_div)
				+ ((self->k_d * derr) >> self->k_d_div);

    self->err_prev = err;

	
	if(self->output_limit == 0)
		return output;
	
    if(output > self->output_limit) {
		if(err > 0){
			self->integrator -= err;
			output -= (self->k_i * err) >> self->k_i_div;
		}
    }
    
    else if(output < -self->output_limit) {
		if(err < 0){
			self->integrator -= err;
			output -= (self->k_i * err) >> self->k_i_div;
		}
    }

	RangeLimit(output, -self->output_limit, self->output_limit);
	
	return output;
}

float PidIncrementalF32(PidF32Class* self, float target, float feedback)
{
	return 0.0f;
}
float mSqrGain = 0.1;
float PidKpVarGain(float e, float de)
{
	//return FastSqrtF32(ABS(e)) * SIGN(e);
	//return SQUARE(e * mSqrGain) * SIGN(e) + e;
	return (1.0f + mSqrGain * de) * e;
}

/**
* @breif Calculate the output of a PID regulator's instance. 
*		 'Position' mode is used. Floating point version.
* @param 	self: 		'this pointer' to the PID instance.
* @param 	target:		value to be tracked.
* @param 	feedback: 	feedback value.
* @retval  output, or 'control effort' of the regulator.
* @author  shiqian.dong
* @version 2016/06/13 v1.0
*/
float PidPositionalF32(PidF32Class* self, float target, float feedback)
{
	float derr, output, p_term, i_term, d_term;

	self->err = target - feedback;
	//If the feedback low pass filter is defined, use it to smooth the feedback.
	if(self->feedback_lpf != NULL){
		self->err = Lpf1stF32Update(self->feedback_lpf, self->err);
	}
	
	//If the differentiator low pass filter is defined, 
	//use it to smooth the differented term.
	derr = self->err - self->err_prev;
	if(self->kd_noliear != NULL) {
		derr = self->kd_noliear(self->err, derr);
	}
	if(self->diff_lpf != NULL){
		derr = Lpf1stF32Update(self->diff_lpf, derr);
	}
	
	d_term = self->k_d * derr;
	
    self->integrator += self->err;
	//Limit the range of integrator. Limit range 0 means no limit.
	//The range limit of integrator here is a 'hard limit'(corresponding to the
	//'soft limit' below)
	if(self->intg_limit != 0){
		RangeLimit(self->integrator, -self->intg_limit, self->intg_limit);
	}
	
	//If one want's to set a nonlinearity on P term...
	if(self->kp_noliear == NULL) {
		p_term = self->k_p * self->err;
	}
	else {
		p_term = self->k_p * self->kp_noliear(self->err, derr);
	}

	//TODO: you can also add a choice to set nonlinearity on I term...		
	i_term = self->k_i * self->integrator;
	
	output = p_term + i_term + d_term;
	
	//Store current error.
    self->err_prev = self->err;
	self->derr_prev = derr;
	
	
	//Limit range 0 means no limit.
	if(self->output_limit == 0)
		return output;
	
	//The range limit of integrator here is a 'soft limit'(corresponding to the
	//'hard limit' above). When the regulator's output overflow, the integrator 
	//will stop accumulating the error that makes the output increase. 
	//In this case, integrator do the 'back integration', which means it will 
	//minus the current error which has been added to the integrator in previous code. 
	//This 'soft limit' is really useful when the response of the controlled process
	//is very slow, and the output range of the regulator is very restricted 
	//at same time.
    if(output > self->output_limit) {
		if(self->err > 0) {
			self->integrator -= self->err;
			output -= self->k_i * self->err;
		}
    }
    else if(output < -self->output_limit) {
		if(self->err < 0) {
			self->integrator -= self->err;
			output -= self->k_i * self->err;
		}
    }

	//Limit the range of output. Limit range 0 means no limit.
	RangeLimit(output, -self->output_limit, self->output_limit);
	
	return output;
}

/** 29.2 us
  * Control Block Diagram:
  *				        ----------            ----------      --------- 
  *pos_target---->o--->| pos_ctrl |---->o--->| spd_ctrl |--->| process |--->
  *			      ^     ----------      ^     ----------      ---------  |
  *			      |				 		|---------spd_feedback-----------|
  *			      |									  	                 |
  *			      |----------------------pos_feedback--------------------|
  */
float PosSpdLoopF32Update(PosSpdLoopF32* self, float pos_target, float spd_feedback, float pos_feedback)
{
	if(self->pos_enable){
		self->spd_cmd = PidPositionalF32(self->pos_ctrl, pos_target, pos_feedback);
	}
	else{
		PidReset(self->pos_ctrl);
	}
	
	return PidPositionalF32(self->spd_ctrl, self->spd_cmd, spd_feedback);
}

