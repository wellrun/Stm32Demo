#ifndef FOC_SVPWM_H
#define FOC_SVPWM_H


void SVPWM_Calc_CCR_by_Vab(Timer_CCRx_t* pTimCCRx, Volt_Components Volt_Input);
void SVPWM_Output(TIM_TypeDef * Timx, Timer_CCRx_t* pTimCCRx);


#endif

