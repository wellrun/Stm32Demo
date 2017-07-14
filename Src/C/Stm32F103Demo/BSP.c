/******************** (C) COPYRIGHT 2017  ***********************************
File name: 
Description: 
Author: duking
Version: 1.0.0
Date: 2017.2.4
History: 
*****************************************************************************/
#include "BSP.h"
#include "Event.h"

//------------------------------------------------------------------------------
//             	GPIO
//------------------------------------------------------------------------------
GpioClass gStateLed = NEW_GPIO(C, 0, GPIO_Mode_Out_PP);

//------------------------------------------------------------------------------
//             	UART
//------------------------------------------------------------------------------
UsartClass gUltrasonicUsart = USART1_DMA_CFG;
UsartClass gTerminalUsart = USART2_DMA_CFG;

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BspInit(void)
{
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	//-----------------GPIO--------------------
	GpioConfig(&gStateLed);
	
		//-------------	USART 1 Config ---------------------------------------------
	gUltrasonicUsart.trans_buf_size = 0;
	gUltrasonicUsart.recv_buf_size = 16;
	gUltrasonicUsart.baud = 115200;
	UartConfig(&gUltrasonicUsart);

	//-------------	UART 2 Config ----------------------------------------------
  gTerminalUsart.trans_buf_size = 200;
	gTerminalUsart.recv_buf_size = 200;
	gTerminalUsart.baud = 115200;
	UartConfig(&gTerminalUsart);
	
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ProgramSetup(void)
{
	BspInit();
	
	AppTaskCreate();
}

/******************* (C) COPYRIGHT 2017 *******END OF FILE********************/
