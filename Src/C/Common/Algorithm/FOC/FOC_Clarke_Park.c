#include "FOC_Globle.h"
#include "FOC_Clarke_Park.h"

#define SIN_MASK	 0x0300
#define U0_90    	 0x0200
#define U90_180  	 0x0300
#define U180_270 	 0x0000
#define U270_360 	 0x0100

const s16 hSin_Cos_Table[256] = SIN_COS_TABLE;

Sin_Cos_Value GetSinCosByAngle(s16 hAngle);


//Clarke变换
Curr_Components Clarke(Curr_Components Curr_Input)
{
	Curr_Components Curr_Output = {0};
	
	// Ialpha = ia
	Curr_Output.C1 = Curr_Input.C1;
	// Ibeta = (ia+2ib)/squr(3) 
	Curr_Output.C2 = ((Curr_Input.C1+(Curr_Input.C2<<1))*2365)>>12;	//1/sqrt(3) = 2365/4096 = 0.577 
	
	return(Curr_Output); 
}
//-=============================================

//Park变换
Curr_Components Park(Curr_Components Curr_Input, Sin_Cos_Value SinCosMap)
{
	Curr_Components Curr_Output = {0};
	
	SinCosMap.hSin >>= 3;	 	//防止计算溢出，电流最大输入2^19=524288（mA）
	SinCosMap.hCos >>= 3;

	//电角度定义为Q轴与Alpha轴的夹角
	//Iq = cos(theta)*Ialpha + sin(theta)*Ibeta
	Curr_Output.C1 = (SinCosMap.hCos*Curr_Input.C1 + SinCosMap.hSin*Curr_Input.C2)>>12; 	
	//Id = sin(theta)*Ialpha - cos(theta)*Ibeta
	Curr_Output.C2 = (SinCosMap.hSin*Curr_Input.C1 - SinCosMap.hCos*Curr_Input.C2)>>12;
	
	return (Curr_Output);
}

//Park变换(对应电压的数据类型)
Volt_Components Park_Volt(Volt_Components Volt_Input, Sin_Cos_Value SinCosMap)
{
	Volt_Components Volt_Output = {0};
	
	SinCosMap.hSin >>= 3;	 	//防止计算溢出，电流最大输入2^19=524288（mA）
	SinCosMap.hCos >>= 3;

	//电角度定义为Q轴与Alpha轴的夹角
	Volt_Output.V1 = (SinCosMap.hCos*Volt_Input.V1 + SinCosMap.hSin*Volt_Input.V2)>>12; 	
	Volt_Output.V2 = (SinCosMap.hSin*Volt_Input.V1 - SinCosMap.hCos*Volt_Input.V2)>>12;
	
	return (Volt_Output);
}


//==========================================================

Volt_Components Rev_Park(Volt_Components Volt_Input, Sin_Cos_Value SinCosMap)
{ 
	Volt_Components Volt_Output = {0};

	//电角度定义为Q轴与Alpha轴的夹角
	//Valpha = cos(theta)*Vq + sin(theta)*Vd
	Volt_Output.V1 = (SinCosMap.hCos*Volt_Input.V1 + SinCosMap.hSin*Volt_Input.V2)>>15; 	
	//Vbeta = sin(theta)*Vq - cos(theta)*Vd
	Volt_Output.V2 = (SinCosMap.hSin*Volt_Input.V1 - SinCosMap.hCos*Volt_Input.V2)>>15;
	
	return(Volt_Output);
}


//由电角度（-32768~32767）查表求正余弦值
Sin_Cos_Value GetSinCosByAngle(s16 hAngle)
{
  u16 hindex;
  Sin_Cos_Value Local_Components = {0};
  
  /* 10 bit index computation  */  
  hindex = (u16)(hAngle + 32768);  	//-32768~32767 -》0~1024
  hindex /= 64;      
  
  switch (hindex & SIN_MASK) 
  {
  case U0_90:
    Local_Components.hSin = hSin_Cos_Table[(u8)(hindex)];
    Local_Components.hCos = hSin_Cos_Table[(u8)(0xFF-(u8)(hindex))];
    break;
  
  case U90_180:  
     Local_Components.hSin = hSin_Cos_Table[(u8)(0xFF-(u8)(hindex))];
     Local_Components.hCos = -hSin_Cos_Table[(u8)(hindex)];
    break;
  
  case U180_270:
     Local_Components.hSin = -hSin_Cos_Table[(u8)(hindex)];
     Local_Components.hCos = -hSin_Cos_Table[(u8)(0xFF-(u8)(hindex))];
    break;
  
  case U270_360:
     Local_Components.hSin =  -hSin_Cos_Table[(u8)(0xFF-(u8)(hindex))];
     Local_Components.hCos =  hSin_Cos_Table[(u8)(hindex)]; 
    break;
  default:
    break;
  }
  return (Local_Components);
}


