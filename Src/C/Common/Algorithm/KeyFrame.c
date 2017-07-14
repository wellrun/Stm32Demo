#include "KeyFrame.h"
#include <stdlib.h>

int32_t KeyFrameGenInit(KeyFrameGen * frame_gen, int32_t frame_gen_num)
{
	KeyFrameGen * current_gen = frame_gen;
	int32_t num_of_channel = 0;
	int32_t return_val = 0;
	if (current_gen == NULL)
		return -1;


	for (int32_t i = 0; i < frame_gen_num; i++, current_gen++) {
		current_gen->is_start = 0;
//		current_gen->current_time = 0;
//		current_gen->current_stage = 0;
		current_gen->time_scale = 1;
		num_of_channel = current_gen->channel_num;
		
		if (current_gen->key_frames->inputs == NULL) {
			return_val |= 0x01 << i;
			continue;
		}

		if (current_gen->outputs == NULL)
			current_gen->outputs = (int32_t *)malloc(num_of_channel + 1);
	}
	return return_val;
}

int32_t KeyFrameGenUpdate(KeyFrameGen * frame_gen, int32_t frame_gen_num)
{
	int32_t stage_time = 0;
	int32_t interval_time = 0;
	int32_t position_diff = 0;
	uint32_t current_time = 0;
	int32_t num_of_channel = 0;
	int64_t long_temp = 0;
	int32_t return_val = 0;
	int32_t * current_data;
	int32_t * next_data;
	int32_t current_frame_time = 0;
	int32_t next_frame_time = 0;
	int32_t current_frame_position = 0;
	int32_t next_frame_position = 0;
	int8_t time_scale = 1;
	KeyFrameGen * current_gen = frame_gen;
	
	if (current_gen == NULL || frame_gen_num <= 0) {
		return -1;
	}

	for (int32_t k = 0; k < frame_gen_num; k++, current_gen++) {
		

		if (!current_gen->is_start) {
			return_val |= 0x01 << k;
			continue;
		}

		time_scale = current_gen->time_scale;
		
		current_time = current_gen->current_time;
		num_of_channel = current_gen->channel_num;

		current_data = &current_gen->key_frames->inputs[current_gen->current_stage * (num_of_channel + 1)];
		
		if(current_gen->key_frames->frame_num <= 2)
		{
			current_gen->outputs[0] = 0;
			for (int32_t i = 0; i < num_of_channel; i++) {
				current_gen->outputs[i + 1] = current_data[i + 1];
			}
		}
		
		current_gen->current_time++;
		
		next_data = &current_gen->key_frames->inputs[(current_gen->current_stage + 1) * (num_of_channel + 1)];

		next_frame_time = time_scale * next_data[0];

		if (current_time > next_frame_time) {
			current_gen->current_stage++;
			
			current_data = next_data;
			next_data = &current_gen->key_frames->inputs[(current_gen->current_stage + 1) * (num_of_channel + 1)];
			
			current_frame_time = next_frame_time;
			next_frame_time = time_scale * next_data[0];
		}
		else {
			current_frame_time = time_scale * current_data[0];
		}

		if (current_gen->current_stage >= current_gen->key_frames->frame_num - 1) {

			current_gen->current_stage = 0;
			current_gen->current_time = 0;
			if(current_gen->circle_time != 0xFF) {
				current_gen->circle_time--;
				if (current_gen->circle_time == 0) {
					current_gen->is_start = 0;
					for (int32_t i = 0; i < num_of_channel; i++) {
						current_gen->outputs[i + 1] = 0;
					}
				}
			}
			return_val |= 0x01 << k;
			continue;
		}

		
		interval_time = next_frame_time - current_frame_time;
		stage_time = current_time - current_frame_time;

		current_gen->outputs[0] = current_time;

		if (current_gen->mode == 0) {
			for (int32_t i = 0; i < num_of_channel; i++) {
				current_frame_position = current_data[i + 1];
				next_frame_position = next_data[i + 1];
				position_diff = next_frame_position - current_frame_position;
				current_gen->outputs[i + 1] = current_frame_position + (int)(stage_time * position_diff) / interval_time;
			}
		}
		else if (current_gen->mode == 1) {
			if (stage_time < interval_time >> 1) {
				for (int32_t i = 0; i < num_of_channel; i++) {
					current_frame_position = current_data[i + 1];
					next_frame_position = next_data[i + 1];
					position_diff = next_frame_position - current_frame_position;
					long_temp = (int64_t)((int32_t)stage_time * (int32_t)stage_time) * (int64_t)(position_diff << 1);
					current_gen->outputs[i + 1] = current_frame_position + ((int)(long_temp / (int64_t)(interval_time * interval_time)));
				}
			}
			else {
				for (int32_t i = 0; i < num_of_channel; i++) {
					current_frame_position = current_data[i + 1];
					next_frame_position = next_data[i + 1];
					position_diff = next_frame_position - current_frame_position;
					long_temp = (int64_t)((int32_t)(interval_time - stage_time) * (int32_t)(interval_time - stage_time)) * (int64_t)(position_diff << 1);
					current_gen->outputs[i + 1] = next_frame_position - ((int)(long_temp / (interval_time * interval_time)));
				}
			}
		}
	}
	return 0;
}

