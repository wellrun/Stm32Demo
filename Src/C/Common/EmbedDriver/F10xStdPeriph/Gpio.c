/******************** (C) COPYRIGHT 2017  ***********************************
File name: 
Description: 
Author: duking
Version: 1.0.0
Date: 2017.2.4
History: 
*****************************************************************************/
#include "F10xStdPeriph.h"

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GpioConfig(GpioClass* gpio)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(gpio->rcc_mask, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = gpio->pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = gpio->mode;
	
    GPIO_Init(gpio->port, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GpioListConfig(GpioClass* gpio_list, uint8_t len)
{
    for(int i = 0; i < len; i ++){
        GpioConfig(&(gpio_list[i]));
    }
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GpioConfigLowSpeed(GpioClass* gpio)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(gpio->rcc_mask, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = gpio->pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = gpio->mode;
	
    GPIO_Init(gpio->port, &GPIO_InitStructure);
}

/******************* (C) COPYRIGHT 2017 *******END OF FILE********************/
