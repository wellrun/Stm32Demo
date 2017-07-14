/******************************************************************/
//	Copyright (C), 2014-2016, 鼎力联合 
//  Author   	  : 陈中元  
//  Update Date   : 2014/06/25
//  Version   	  : 20140625          
//  Description   :  
/******************************************************************/

#ifndef FOC_CTRL_PARAM_H
#define FOC_CTRL_PARAM_H

#include "FOC_TYPE.h"

#define PID_CURR_KP_DEFAULT  (s16)800       
#define PID_CURR_KI_DEFAULT  (s16)500
#define PID_CURR_KD_DEFAULT  (s16)0

#define PID_CURR_KPDIV ((u16)(2048))
#define PID_CURR_KIDIV ((u16)(8192))
#define PID_CURR_KDDIV ((u16)(2048))

//正式程序：  2000   1250  3000  2000
//运动版参数：2000   4000  3000  3000
//舒适版：    2000   400   2000  1000
#define PID_SPD_KP_DEFAULT  (s16)2000       //这里470转的是2000
#define PID_SPD_KI_DEFAULT  (s16)1250
#define PID_SPD_KD_DEFAULT  (s16)3000

#define PID_SPD_KPDIV ((u16)(2048))
#define PID_SPD_KIDIV ((u16)(8192))
#define PID_SPD_KDDIV ((u16)(2048))


#define PID_CURR_OUTPUT_LIMIT (S16_MAX*98/100)	   //Q轴电流PID最大输出为满值的98%
#define PID_CURR_D_OUTPUT_LIMIT (S16_MAX*20/100)	//D轴电流PID最大输出为满值的98%

#define PID_SPD_OUTPUT_LIMIT (S16_MAX*96/100)	   //电流PID最大输出为满值的96%


#endif

