#ifndef SIGGEN_H
#define SIGGEN_H
#include "Algorithm.h"

#ifdef __cplusplus
extern "C" {
#endif
//typedef struct KeyFrame_ {
//	uint32_t time;
//	int32_t position;
//	uint32_t interval_time;
//	int32_t position_diff;
//	float max_speed;
//} KeyFrame;

//typedef struct KeyFrameGen_ {
//	uint32_t position_min;
//	uint32_t position_max;
//	uint32_t num_of_frames;
//	uint32_t current_time;
//	uint32_t current_stage;
//	int8_t is_start;
//	KeyFrame * key_frames;
//	int32_t output;
//} KeyFrameGen;

//int32_t KeyFrameGenInit(KeyFrameGen * self);
//int32_t KeyFrameGenUpdate(KeyFrameGen * self);

	typedef enum SigGenType_
	{
		SigGenTriangle,
		SigGenSquare,
		SigGenSin,
		SigGenRand
	} SigGenType;

	typedef struct{
		SigGenType type;
		bool enable;
		float update_hz;
		float freq;
		float amp;

		float inc_step;
		float data_src;
		bool is_step_update;
	} SigGenClass;

	float SigGenUpdate(SigGenClass* self);
#ifdef __cplusplus
}
#endif

#endif // SIGGEN_H
