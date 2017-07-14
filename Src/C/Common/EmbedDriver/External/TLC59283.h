#ifndef TLC59283_H
#define TLC59283_H

#include "Common/Lang/Lang.h"
#include "Common/EmbedDriver/F10xStdPeriph/F10xStdPeriph.h"
#include "Algorithm/KeyFrame.h"



#define InitRGBLed_(led) 									\
do {														\
	(led).r_mask = 0x00000001 << (led).r_shift;				\
	(led).g_mask = 0x00000001 << (led).g_shift;             \
	(led).b_mask = 0x00000001 << (led).b_shift;             \
	(led).r_imask = ~((led).r_mask);						\
	(led).g_imask = ~((led).g_mask);             			\
	(led).b_imask = ~((led).b_mask);						\
	(led).r_val = 0;                                        \
	(led).g_val = 0;                                        \
	(led).b_val = 0;                                        \
} while(0)

#define InitRGBLedGroup_(led_group)										\
do {																	\
	(led_group).overflow_value = 10000 / (led_group).update_frequency;	\
	(led_group).led_group_cnt = 0;										\
	(led_group).current_led_bits = 0xFFFFFFFF;							\
	for (int i = 0; i < (led_group).num_of_led; i++) {					\
		InitRGBLed_((led_group).leds[i]);								\
	}																	\
	(led_group).protocol.set_value = RGBLedGroupSetValue;				\
	(led_group).protocol.set_all_value = RGBLedGroupSetAllValue;		\
	(led_group).protocol.set_value_rgb = RGBLedGroupSetValueRGB;		\
	(led_group).protocol.set_all_value_rgb = RGBLedGroupSetAllValueRGB;	\
	(led_group).protocol.update = RGBLedGroupUpdate;					\
} while(0)

typedef struct RGBLedGroup_ RGBLedGroup;

typedef int32_t (*RGBLedGroupSetValueFunc)(RGBLedGroup * led_group, uint32_t led_index, uint8_t color, uint8_t val);
typedef int32_t (*RGBLedGroupSetAllValueFunc)(RGBLedGroup * led_group, uint8_t color, uint8_t val);
typedef int32_t (*RGBLedGroupSetValueRGBFunc)(RGBLedGroup * led_group, uint32_t led_index, uint8_t r_val, uint8_t g_val, uint8_t b_val);
typedef int32_t (*RGBLedGroupSetAllValueRGBFunc)(RGBLedGroup * led_group, uint8_t r_val, uint8_t g_val, uint8_t b_val);
typedef int32_t (*RGBLedGroupUpdataFun)(RGBLedGroup * led_group);


typedef struct RGBLedGroupProtocol_ {
	RGBLedGroupSetValueFunc set_value;
	RGBLedGroupSetAllValueFunc set_all_value;
	RGBLedGroupSetValueRGBFunc set_value_rgb;
	RGBLedGroupSetAllValueRGBFunc set_all_value_rgb;
	RGBLedGroupUpdataFun update;
} RGBLedGroupProtocol;

typedef struct RGBLed_ {
	uint8_t r_val;
	uint8_t g_val;
	uint8_t b_val;
	uint8_t r_shift;
	uint8_t g_shift;
	uint8_t b_shift;
	uint32_t r_mask;
	uint32_t g_mask;
	uint32_t b_mask;
	uint32_t r_imask;
	uint32_t g_imask;
	uint32_t b_imask;
} RGBLed;

typedef struct RGBLedGroup_ {
	uint32_t num_of_led;
	uint32_t update_frequency;
	uint32_t overflow_value;
	uint32_t led_group_cnt;
	uint32_t current_led_bits;
	RGBLed * leds;
	ClassInit Init;
	IODeviceInterface * device_interface;
	RGBLedGroupProtocol protocol;
} RGBLedGroup;




int32_t IntiRGBLed(RGBLed * led);

int32_t InitRGBLedGroup(void * self);

int32_t RGBLedGroupSetValue(RGBLedGroup * led_group, uint32_t led_index, uint8_t color, uint8_t val);

int32_t RGBLedGroupSetAllValue(RGBLedGroup * led_group, uint8_t color, uint8_t val);

int32_t RGBLedGroupSetValueRGB(RGBLedGroup * led_group, uint32_t led_index, uint8_t r_val, uint8_t g_val, uint8_t b_val);

int32_t RGBLedGroupSetAllValueRGB(RGBLedGroup * led_group, uint8_t r_val, uint8_t g_val, uint8_t b_val);

int32_t RGBLedGroupUpdate(RGBLedGroup * led_group);




#define SetValue(led_group, led_index, color, val) 				\
		(led_group)->protocol.set_value((led_group), (led_index), (color), (val))

#define SetAllValue(led_group, color, val) 						\
		(led_group)->protocol.set_all_value((led_group), (color), (val))
		
#define SetValueRGB(led_group, led_index, r_val, g_val, b_val) 	\
		(led_group)->protocol.set_value_rgb((led_group), (led_index), (r_val), (g_val), (b_val)) 

#define SetAllValueRGB(led_group, r_val, g_val, b_val)  		\
		(led_group)->protocol.set_all_value_rgb((led_group), (r_val), (g_val), (b_val)) 

#define Update(led_group) 										\
		(led_group)->protocol.update((led_group))

int32_t LedSwitchMode(
	KeyFrameGen * key_gen, 
	int32_t gen_num, 
	const int32_t (*key_frames)[4],
	int32_t frame_num,
	int32_t phase_div,
	uint8_t mode );






#endif
