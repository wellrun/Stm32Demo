/******************** (C) COPYRIGHT 2017  ***********************************
File name: 
Description: 
Author: duking
Version: 1.0.0
Date: 2017.2.4
History: 
*****************************************************************************/
#ifndef CONFIG_GPIO_H_
#define CONFIG_GPIO_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
		GPIO_TypeDef* port;
		uint16_t pin;
		uint32_t rcc_mask;
		GPIOMode_TypeDef mode;
} GpioClass;


#define NEW_GPIO(port_, pin_, mode_)    {         \
		.port     = GPIO##port_,                      \
		.pin      = GPIO_Pin_##pin_,                 	\
		.rcc_mask = RCC_APB2Periph_GPIO##port_,      	\
		.mode     = mode_                             \
}

#define NEW_GPIO_NOMODE(port_, pin_)    {         \
			.port     = GPIO##port_,                    \
			.pin      = GPIO_Pin_##pin_,                \
			.rcc_mask = RCC_APB2Periph_GPIO##port_      \
}
	
void GpioConfig(GpioClass* gpio);
void GpioListConfig(GpioClass* gpio_list, uint8_t len);
void GpioConfigLowSpeed(GpioClass* gpio);

#define GPIO_NO_REMAP (uint32_t)-1

#define GPIO_SET(GPIO)		GPIO_WriteBit((GPIO).port, (GPIO).pin, Bit_SET)
#define GPIO_RESET(GPIO)	GPIO_WriteBit((GPIO).port, (GPIO).pin, Bit_RESET)
#define GPIO_TOGGLE(GPIO)	GPIO_WriteBit((GPIO).port, (GPIO).pin, (BitAction)!GPIO_ReadOutputDataBit((GPIO).port, (GPIO).pin))
#define GPIO_READ(GPIO)		GPIO_ReadInputDataBit((GPIO).port, (GPIO).pin)
    
#ifdef __cplusplus
}
#endif

#endif
/******************* (C) COPYRIGHT 2017 *******END OF FILE********************/
