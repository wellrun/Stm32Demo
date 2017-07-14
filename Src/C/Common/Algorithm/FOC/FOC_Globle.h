/******************************************************************/
//	Copyright (C), 2014-2016, 鼎力联合 
//  Author   	  : 陈中元  
//  Update Date   : 2014/06/25
//  Version   	  : 20140625          
//  Description   :  
/******************************************************************/

#ifndef FOC_GLOBLE_H
#define FOC_GLOBLE_H

#include <string.h>
#include "FOC_TYPE.h"

#define CKTIM	((s32)72000000) 			//时钟频率

//#define CKTIM	((s32)64000000) 			//时钟频率
//*
#define FOC_FREQ  	 	(10000)   			//FOC伺服频率

#define FOC_1MS_CNT		(FOC_FREQ/1000)		//1ms计数值
#define FOC_100MS_CNT	(FOC_FREQ/10)		//100ms计数值

#define FOC_TIMER_TOP_CCR			1800			//定时器计数值，中央对齐模式，周期50us
#define TOP_CCR FOC_TIMER_TOP_CCR
#define MAX_CCR			1760		 	//最大占空比计数值（98%占空比）
/*


#define FOC_FREQ  	 	(8000)   			//FOC伺服频率

#define FOC_1MS_CNT		(FOC_FREQ/800)		//1ms计数值
#define FOC_100MS_CNT	(FOC_FREQ/8)		//100ms计数值
#define FOC_TIMER_TOP_CCR			2000			//定时器计数值，中央对齐模式，周期50us
#define MAX_CCR			1960		 	//最大占空比计数值（98%占空比）

// */
#define N05_CCR			(FOC_TIMER_TOP_CCR*5/100)		 		//5%占空比
#define N08_CCR			(FOC_TIMER_TOP_CCR*8/100)		 		//8%占空比
#define N80_CCR			(FOC_TIMER_TOP_CCR*80/100)	 		//80%占空比
#define N97_CCR			(FOC_TIMER_TOP_CCR*97/100)	 		//97%占空比


//全局变量引用
extern PID_Struct_t g_PID_Stru_Iq;
extern PID_Struct_t g_PID_Stru_Id;
extern PID_Struct_t g_PID_Stru_Speed;
extern Timer_CCRx_t g_TimerCCR_Stru;

extern Timer_CCRx_t g_TimerCCR_Stru_2;

extern Curr_Components g_Curr_q_d;
extern Curr_Components g_Curr_q_d_2;


#endif

