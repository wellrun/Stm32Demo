#ifndef FILTER_H_
#define FILTER_H_
#include "Algorithm.h"
#ifdef __cplusplus
extern "C" {
#endif
	
	typedef struct{
		int32_t prev_output;
		int32_t t_const;
		int32_t t_const_div;
	} Lpf1stQ31;
	
	typedef struct{
		float prev_output;
		float t_const;
		float t_const_1;
	} Lpf1stF32;
	
	Lpf1stQ31* NewLpf1stQ31(int32_t t_const, int32_t t_const_div);
	int32_t Lpf1stQ31Update(Lpf1stQ31* self, int32_t x);
	
	Lpf1stF32* NewLpf1stF32(float t_const);
	float Lpf1stF32Update(Lpf1stF32* self, float x);
	
	typedef struct{
		int32_t acc_estim;
		int32_t spd_estim;
		int32_t pos_estim;
	} TrackObsvQ31;	
	
	typedef struct {
		int32_t up_limit;
		int32_t low_limit;
		uint32_t range;
		int32_t mid_level;
		int32_t last_output;
	} CircleRound;
	CircleRound * NewCircleRoundFilter(int32_t up_limit, int32_t low_limit);
	int32_t CircleRoundUpdate(CircleRound * self, int32_t x);
#ifdef __cplusplus
}
#endif

#endif // FILTER_H_

