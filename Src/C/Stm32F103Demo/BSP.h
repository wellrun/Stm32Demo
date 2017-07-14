/******************** (C) COPYRIGHT 2017  ***********************************
File name: 
Description: 
Author: duking
Version: 1.0.0
Date: 2017.2.4
History: 
*****************************************************************************/
#ifndef  __BSP_H__
#define  __BSP_H__

#include "F10xStdPeriph.h"

extern GpioClass gStateLed;
extern UsartClass gTerminalUsart;

void ProgramSetup(void);
void BspInit(void);

#endif
/******************* (C) COPYRIGHT 2017 *******END OF FILE********************/
