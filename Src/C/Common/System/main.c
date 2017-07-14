/******************** (C) COPYRIGHT 2017  ***********************************
File name: 
Description: 
Author: duking
Version: 1.0.0
Date: 2017.2.4
History: 
*****************************************************************************/
#include "stm32f10x.h"
#include "BSP.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int main(void)
{
	SystemInit();   //STM32 RCC Init

	ProgramSetup(); //bsp && Task Init

	vTaskStartScheduler(); //Start Task
	
	while(1);
}


/******************* (C) COPYRIGHT 2017 *******END OF FILE********************/
