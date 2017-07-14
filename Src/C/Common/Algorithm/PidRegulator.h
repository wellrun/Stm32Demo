#ifndef PID_REGULATOR_H_
#define PID_REGULATOR_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Algorithm.h"

typedef struct PidF32Class_
{
	float err;
    float k_p;
    float k_i;
    float k_d;
    float output_limit;
    float target;
    float feedback;
    float integrator;
    float output_integrator;
    float antiwindup_factor;
    float output_prev;
    float err_prev;
    float derr_prev;
	float intg_limit;
	
	Lpf1stF32* feedback_lpf;
	Lpf1stF32* diff_lpf;
	
	float (*kp_noliear)(float, float); //TODO: -> nolinear
	float (*ki_noliear)(float, float);
	float (*kd_noliear)(float, float);
} PidF32Class;
	
	
typedef struct
{
    int32_t k_p;
    int32_t k_p_div;
    int32_t k_i;
    int32_t k_i_div;
    int32_t k_d;
    int32_t k_d_div;
    int32_t output_limit;
    int32_t target;
    int32_t feedback;
    int32_t integrator;
    int32_t output_integrator;
    int32_t antiwindup_factor;
    int32_t antiwindup_factor_div;
    int32_t output_prev;
    int32_t err_prev;
    int32_t derr_prev;
	int32_t intg_limit;
	Lpf1stQ31* diff_lpf;
} PidQ31Class;

int32_t PidIncrementalQ31(PidQ31Class* self, int32_t target, int32_t feedback);

int32_t PidPositionalQ31(PidQ31Class* self, int32_t target, int32_t feedback);

float PidIncrementalF32(PidF32Class* self, float target, float feedback);

float PidPositionalF32(PidF32Class* self, float target, float feedback);

float PidKpVarGain(float e, float de);

#define PidReset(self) do{ \
	self->integrator = 0;	\
	self->output_integrator = 0; \
	self->output_prev = 0;			\
    self->err_prev = 0;				\
    self->derr_prev = 0;				\
} while(0)

typedef struct PosSpdLoopF32_
{
	PidF32Class* spd_ctrl;
	PidF32Class* pos_ctrl;
	float spd_cmd;
	bool pos_enable;
} PosSpdLoopF32;

float PosSpdLoopF32Update(PosSpdLoopF32* self, float pos_target, float spd_feedback, float pos_feedback);


#ifdef __cplusplus
}
#endif

#endif // PID_REGULATOR_H_
