#include "FOC_Globle.h"
#include "FOC_SVPWM.h"
//#include "Globle.h"

static s32 SVPWM_Calculate_Sector(s32 Uref1, s32 Uref2, s32 Uref3)
{
	s32 sector = 0;
	if(Uref3 <= 0)
	{
		if(Uref2 > 0)
		{
			if(Uref1 <= 0)
				sector = 6;
			else
				sector = 1;			
		}
		else
		{
			sector = 2;	
		}
	}
	else
	{
		if(Uref2 > 0)
		{
			sector = 5;			
		}
		else
		{
			if(Uref1 <= 0)
				sector = 4;
			else
				sector = 3;	
		}
	}
	return sector;
}

static void SVPWM_Calc_CCR_by_XYZ(Timer_CCRx_t* pTimCCRx, int sector, int X, int Y, int Z)	//
{
	s32 t0 = 0;
	s32 tk = 0;
	s32 tk_1 = 0;
	s32 CCR1 = 0;
	s32 CCR2 = 0;
	s32 CCR3 = 0;
	switch(sector)
	{
	case 1:
		tk_1 = X;
		tk = -Z;
		pTimCCRx->CCRT = tk_1 + tk;
		if(tk_1 + tk > MAX_CCR)
		{
			tk_1 = tk_1*MAX_CCR/(tk_1+tk);
			tk = tk*MAX_CCR/(tk_1+tk);
		}
		t0 = TOP_CCR - tk - tk_1;
		CCR3 = t0>>1;
		CCR2 = CCR3 + tk_1;
		CCR1 = CCR2 + tk;
		break;
	case 2:
		tk_1 = Z;
		tk = Y;
		pTimCCRx->CCRT = tk_1 + tk;
		if(tk_1 + tk > MAX_CCR)
		{
			tk_1 = tk_1*MAX_CCR/(tk_1+tk);
			tk = tk*MAX_CCR/(tk_1+tk);
		}
		t0 = TOP_CCR - tk - tk_1;
		CCR3 = t0>>1;
		CCR1 = CCR3 + tk;
		CCR2 = CCR1 + tk_1;
		break;
	case 3:
		tk_1 = -Y;
		tk = X;
		pTimCCRx->CCRT = tk_1 + tk;
		if(tk_1 + tk > MAX_CCR)
		{
			tk_1 = tk_1*MAX_CCR/(tk_1+tk);
			tk = tk*MAX_CCR/(tk_1+tk);
		}
		t0 = TOP_CCR - tk - tk_1;
		CCR1 = t0>>1;
		CCR3 = CCR1 + tk_1;
		CCR2 = CCR3 + tk;
		break;
	case 4:
		tk_1 = -X;
		tk = Z;
		pTimCCRx->CCRT = tk_1 + tk;
		if(tk_1 + tk > MAX_CCR)
		{
			tk_1 = tk_1*MAX_CCR/(tk_1+tk);
			tk = tk*MAX_CCR/(tk_1+tk);
		}
		t0 = TOP_CCR - tk - tk_1;
		CCR1 = t0>>1;
		CCR2 = CCR1 + tk;
		CCR3 = CCR2 + tk_1;
		break;
	case 5:
		tk_1 = -Z;
		tk = -Y;
		pTimCCRx->CCRT = tk_1 + tk;
		if(tk_1 + tk > MAX_CCR)
		{
			tk_1 = tk_1*MAX_CCR/(tk_1+tk);
			tk = tk*MAX_CCR/(tk_1+tk);
		}
		t0 = TOP_CCR - tk - tk_1;
		CCR2 = t0>>1;
		CCR1 = CCR2 + tk_1;
		CCR3 = CCR1 + tk;
		break;
	case 6:
		tk_1 = Y;
		tk = -X;
		pTimCCRx->CCRT = tk_1 + tk;
		if(tk_1 + tk > MAX_CCR)
		{
			tk_1 = tk_1*MAX_CCR/(tk_1+tk);
			tk = tk*MAX_CCR/(tk_1+tk);
		}
		t0 = TOP_CCR - tk - tk_1;
		CCR2 = t0>>1;
		CCR3 = CCR2 + tk;
		CCR1 = CCR3 + tk_1;
		break;
	default:
		break;
	}

	pTimCCRx->CCR1 = CCR1;
	pTimCCRx->CCR2 = CCR2;
	pTimCCRx->CCR3 = CCR3;
	pTimCCRx->bOutputEN = 1;
}

void SVPWM_Calc_CCR_by_Vab(Timer_CCRx_t* pTimCCRx, Volt_Components Volt_Input)
{
	int X, Y, Z;
	int Sector;

	//SVP占空比计算输出
	X = Volt_Input.V2;
	Y = (Volt_Input.V2 + ((Volt_Input.V1*1774)>>10))>>1;	//= (Ub + sqrt(3)*Ua)/2;
	Z = (Volt_Input.V2 - ((Volt_Input.V1*1774)>>10))>>1;	//= (Ub - sqrt(3)*Ua)/2;
	Sector = SVPWM_Calculate_Sector(X, -Z, -Y);
	X = (X * TOP_CCR) >> 15;        // volt = X/32767 * 60; time = X/32767 * 2000
	Y = (Y * TOP_CCR) >> 15;
	Z = (Z * TOP_CCR) >> 15;

	SVPWM_Calc_CCR_by_XYZ(pTimCCRx, Sector, X, Y, Z);	 // */	
}

void SVPWM_Output(TIM_TypeDef * Timx, Timer_CCRx_t* pTimCCRx)
{
	if(pTimCCRx->bOutputEN)
	{
//		g_bEnableHB = 1;
		pTimCCRx->bOutputEN = 0;
//		Timx->CCER = (u16)0x555;
		Timx->CCR1 = pTimCCRx->CCR1;
		Timx->CCR2 = pTimCCRx->CCR2;
		Timx->CCR3 = pTimCCRx->CCR3;
	}
}
