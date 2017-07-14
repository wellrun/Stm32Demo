#ifndef FOC_HALL_H
#define FOC_HALL_H

#define HALL_TIMER 			TIM5

//#define ENABLE_ERROR_REC			//是否使用黑匣子功能

//硬件初始化
u32 FOC_Hall_Int(void);						//AD相关初始化

void Hall_TIM_IRQHandler(void);				//中断处理
u16 HALL_IncElectricalAngle(void);
s32 HALL_GetAvrSpeed(void);
s32 HALL_GetDirSpeed(void);
u8 Hall_GethalfWall(void);
u8 SVP_ReadHallState(void);

#endif
