/******************** (C) COPYRIGHT 2017  ***********************************
File name: 
Description: 
Author: duking
Version: 1.0.0
Date: 2017.2.4
History: 
*****************************************************************************/
#include "Event.h"
#include "BSP.h"

//FreeRtos
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define UsartDebug

//任务句柄
static TaskHandle_t xHandleTaskLED1 = NULL;

//Task
void vTaskLed1(void *pvParameters);


/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AppTaskCreate(void)
{
	xTaskCreate(vTaskLed1,          
	            "Task Led1",         
	             512,                
	             NULL,              
	             1,                  
	             &xHandleTaskLED1);
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 Count = 0;
void vTaskLed1(void *pvParameters)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount ();
	
	while(1)
	{	
		Count++;
		GPIO_TOGGLE(gStateLed);
		vTaskDelayUntil( &xLastWakeTime,500 );
		
		#ifdef UsartDebug
			printf("LED Count: %d\n",Count);
		#endif
	}	
}

/******************* (C) COPYRIGHT 2017 *******END OF FILE********************/
