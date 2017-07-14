/******************************************************************/
//	Copyright (C), 2014-2016, 鼎力联合 
//  Author   	  : 陈中元  
//  Update Date   : 2014/06/25
//  Version   	  : 20140625          
//  Description   :  
/******************************************************************/

#ifndef FOC_TYPE_H
#define FOC_TYPE_H

#include "stm32f10x.h"

typedef struct
{
  s16 hCos;
  s16 hSin;
} Sin_Cos_Value;

typedef struct 
{
  s32 C1;
  s32 C2;
} Curr_Components;

typedef struct 
{
  s16 V1;
  s16 V2;
} Volt_Components;

typedef struct 
{
  u16 CCR1;
  u16 CCR2;
  u16 CCR3;
  u16 CCRT;			//最终输出的占空比
  u16 bOutputEN;	//是否允许输出
} Timer_CCRx_t;

typedef struct 
{ 
  s16 hKp_Gain;
  u16 hKp_Divisor;
  s16 hKi_Gain;
  u16 hKi_Divisor;  
  s32 wLower_Limit_Output;     //Lower Limit for Output limitation
  s32 wUpper_Limit_Output;     //Lower Limit for Output limitation
  s32 wLower_Limit_Integral;   //Lower Limit for Integral term limitation
  s32 wUpper_Limit_Integral;   //Lower Limit for Integral term limitation
  s32 wIntegral;
  // Actually used only if DIFFERENTIAL_TERM_ENABLED is enabled
  s16 hKd_Gain;
  u16 hKd_Divisor;
  s32 wPreviousError;
} PID_Struct_t;

#endif

