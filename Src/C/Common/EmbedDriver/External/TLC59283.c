#include "TLC59283.h"
#include "Algorithm/KeyFrame.h"
int32_t InitRGBLedGroup(void * self)
{
	RGBLedGroup * led_group = (RGBLedGroup *) self;
	if (led_group == NULL || \
		led_group->num_of_led == 0 || \
		led_group->leds == NULL) 
	{
		return -1;
	}
	
	InitRGBLedGroup_((*led_group));
	
	return 0;
}

int32_t RGBLedGroupSetValue(RGBLedGroup * led_group, uint32_t led_index, uint8_t color, uint8_t val) 
{
	if (led_index >= led_group->num_of_led) {
		return -1;
	}
	switch(color) 
	{
		case 0:
			led_group->leds[led_index].r_val = val;
			break;
		case 1:
			led_group->leds[led_index].g_val = val;
			break;
		case 2:
			led_group->leds[led_index].b_val = val;
			break;
	}
	return 0;
}

int32_t RGBLedGroupSetAllValue(RGBLedGroup * led_group, uint8_t color, uint8_t val)
{
	int32_t num = led_group->num_of_led;
	
	for (int i = 0; i < num; i++) {
		RGBLedGroupSetValue(led_group, i, color, val);
	}
	
	return 0;
}


int32_t RGBLedGroupSetValueRGB(RGBLedGroup * led_group, uint32_t led_index, uint8_t r_val, uint8_t g_val, uint8_t b_val)
{
	if (led_index >= led_group->num_of_led) {
		return -1;
	}
	led_group->leds[led_index].r_val = r_val;
	led_group->leds[led_index].g_val = g_val;
	led_group->leds[led_index].b_val = b_val;
	
	return 0;
}

int32_t RGBLedGroupSetAllValueRGB(RGBLedGroup * led_group, uint8_t r_val, uint8_t g_val, uint8_t b_val)
{
	int32_t num = led_group->num_of_led;
	for (int i = 0; i < num; i++) {
		RGBLedGroupSetValueRGB(led_group, i, r_val, g_val, b_val);
	}
	return 0;
}


int32_t RGBLedGroupUpdate(RGBLedGroup * led_group)
{
	int32_t num = led_group->num_of_led;
	int8_t need_update = 0;
	int8_t is_start = 0;
	int32_t current_cnt = 0;
	led_group->led_group_cnt++;
	current_cnt = led_group->led_group_cnt;

	if (current_cnt >= led_group->overflow_value) {
		//ALL ON
		led_group->led_group_cnt = 0;
		current_cnt = 0;
		led_group->current_led_bits = 0xFFFFFFFF;
		is_start = 1;
	}
	
	for (int i = 0; i < num; i++) {
		RGBLed * current_led = &led_group->leds[i];
		if (current_led->r_val == current_cnt) {
			need_update = 1;
			led_group->current_led_bits &= current_led->r_imask;
		}
		if (current_led->g_val == current_cnt) {
			need_update = 1;
			led_group->current_led_bits &= current_led->g_imask;
		}
		if(current_led->b_val == current_cnt) {
			need_update = 1;
			led_group->current_led_bits &= current_led->b_imask;
		}
	}
	
	if ((need_update || is_start)) {
		led_group->device_interface->device_send(led_group->device_interface, (uint8_t *)&(led_group->current_led_bits), 2);
	}
	
	return 0;
}









