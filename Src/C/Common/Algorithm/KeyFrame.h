#ifndef KEY_FRAME_H_
#define KEY_FRAME_H_

#include <stdint.h>

typedef struct KeyFrame_ {
	int32_t * inputs;
	uint32_t frame_num;
} KeyFrame;

typedef struct KeyFrameGen_ {
	int32_t position_min;
	int32_t position_max;
	int32_t current_time;
	int32_t current_stage;
	uint8_t time_scale;
	int8_t is_start;
	uint8_t circle_time;
	int8_t mode;
	int32_t channel_num;
	int32_t * outputs;
	KeyFrame * key_frames;
} KeyFrameGen;



int32_t KeyFrameGenInit(KeyFrameGen * frame_gen, int32_t frame_gen_num);

int32_t KeyFrameGenUpdate(KeyFrameGen * frame_gen, int32_t frame_gen_num);




#endif /*KEY_FRAME_H_*/


