#include "Algorithm.h"

float SigGenUpdate(SigGenClass* self)
{
	if(!self->enable)
		return 0;

	if(!self->is_step_update){
		self->inc_step = self->freq / self->update_hz;
		self->data_src = 0;
		self->is_step_update = true;
	}

	self->data_src +=self->inc_step;

	if(self->data_src > 1.0f)
		self->data_src = 0;

	if(self->type == SigGenTriangle){
		if(self->data_src < 0.5f){
			return (2.0f * self->data_src - 0.5f) * self->amp;
		}
		else{
			return (1.5f - 2.0f * self->data_src) * self->amp;
		}
	}
	else if(self->type == SigGenSquare){
		if(self->data_src < 0.5f){
			return 0.5f * self->amp;
		}
		else{
			return -0.5f * self->amp;
		}
	}
	else
		return 0;
}

//int32_t KeyFrameGenUpdate(KeyFrameGen * self)
//{
////	static int32_t self->output = 0;
//	int32_t stage_time = 0;
//	uint32_t interval_time = 0;
//	float max_speed = 0.0;
//	uint32_t current_time = 0;
//	KeyFrame * current_frame = NULL;
//	KeyFrame * next_frame = NULL;
//	if (!self->is_start)
//		return self->output;
//	self->current_time++;
//	current_time = self->current_time;
//	if (current_time > self->key_frames[self->current_stage + 1].time) {
//		self->current_stage++;
//	}
//	if (self->current_stage >= self->num_of_frames - 1) {
//		self->is_start = 0;
//		self->current_stage = 0;
//		self->current_time = 0;
//		return self->output;
//	}
//	current_frame = &self->key_frames[self->current_stage];
//	next_frame = &self->key_frames[self->current_stage + 1];
//	interval_time = current_frame->interval_time;
//	stage_time = current_time - current_frame->time;
//	max_speed = current_frame->max_speed;
//	if (stage_time < interval_time >> 1) {
//		self->output = current_frame->position + (int)(stage_time * stage_time * max_speed / interval_time);
//	}
//	else {
//		self->output = next_frame->position - (int)((interval_time - stage_time) * (interval_time - stage_time) * max_speed / interval_time);
//	}
//	return self->output;
//}

//int32_t KeyFrameGenInit(KeyFrameGen * self)
//{
//	self->is_start = 0;
//	self->current_time = 0;
//	self->current_stage = 0;
//	self->output = 0;
//	uint32_t num = self->num_of_frames;
//	KeyFrame * key_frame_ptr = self->key_frames;
//	for (int32_t i = 1; i < num; i++) {
//		key_frame_ptr[i - 1].position_diff = key_frame_ptr[i].position - key_frame_ptr[i - 1].position;
//		key_frame_ptr[i - 1].interval_time = key_frame_ptr[i].time - key_frame_ptr[i - 1].time;
//		key_frame_ptr[i - 1].max_speed = 2.0f * key_frame_ptr[i - 1].position_diff / key_frame_ptr[i - 1].interval_time;
//	}
//	return 0;
//}
