#include "FDC2212.h"
#include "../../DeviceSupport/CtrlMap.h"
//#include "../GlobalDefines.h"

//==================================================================================
//  I2C ??????, MCU ??????
//==================================================================================

//#define PARAM_DEBUG
//#define READ_SETTING_REG


#define LED_I2C_SUCCESS         0
#define LED_I2C_FAILED          -1

#define REGISTER_BYTE_NUM       2
#define DATA_REGISTER_BYTE_NUM  4

#define FDC2212_VALID_BIT	0x0FFFFFFF
#define FDC2212_CONS_VALUE  0xFFFF
#define FDC2212_COEFFICIENT 0.3
#define FDC2212_DIFFTIME    50
//======================================================
#define DATA_NOT_READY            0
#define DATA_CONVERSION_READY     1
//======================================================


DataAverage gReadDataObj = {
	.read_cnt = 0,
	.read_limit = DATA_NUMBER,
};

uint16_t test1 = 0xFFFF;
uint16_t test2 = 0x820D;
uint16_t test3 = 0x1C01;
uint16_t test4 = 0x2001;
uint16_t test5 = 0xB000;
uint16_t test6 = 0x0400;

uint16_t temp1 = 0;
uint16_t temp2 = 0;
uint16_t temp3 = 0;
uint16_t temp4 = 0;
uint16_t temp5 = 0;
uint16_t temp6 = 0;


//======================================================================
int FdcReadSingleRegister(SoftwareI2cClass *self, uint8_t dev_id, uint8_t reg_addr, uint16_t *data);
int FdcWriteSingleFdcRegister(SoftwareI2cClass *self, uint8_t dev_id, uint8_t reg_addr, uint16_t *data_);


uint32_t timer = 8000;
void delay(void)
{
	volatile int i = 0;
	for(i=0; i<timer; i++);
}

int Fdc2212Config(SoftwareI2cClass *self, uint8_t dev_id)
{
	delay();

	if(LED_I2C_FAILED ==  FdcWriteSingleFdcRegister(self, dev_id, FDC2x_RCOUNT_CH0, (uint16_t*)&test1) )
        return LED_I2C_FAILED;

    if(LED_I2C_FAILED ==  FdcWriteSingleFdcRegister(self, dev_id, FDC2x_RCOUNT_CH0, (uint16_t*)&test1) )
        return LED_I2C_FAILED;

    if(LED_I2C_FAILED ==  FdcWriteSingleFdcRegister(self, dev_id, FDC2x_MUX_CONFIG, (uint16_t*)&test2) )
        return LED_I2C_FAILED;

    //use chal0, Full current activation mode,  Use Internal oscil lator, INTB Disable, High Current Sensor(AUTOSCAN_EN must be 0)
    if(LED_I2C_FAILED ==  FdcWriteSingleFdcRegister(self, dev_id, FDC2x_CONFIG, (uint16_t*)&test3) )
        return LED_I2C_FAILED;

	if(LED_I2C_FAILED ==  FdcWriteSingleFdcRegister(self, dev_id, FDC2x_CLOCK_DIVIDERS_CH0, (uint16_t*)&test4) ) //select the frequency in 0-10M
        return LED_I2C_FAILED;

    if(LED_I2C_FAILED ==  FdcWriteSingleFdcRegister(self, dev_id, FDC2x_DRIVE_CURRENT_CH0, (uint16_t*)&test5) )
        return LED_I2C_FAILED;

	if(LED_I2C_FAILED ==  FdcWriteSingleFdcRegister(self, dev_id, FDC2x_SETTLECOUNT_CH0, (uint16_t*)&test6) )
        return LED_I2C_FAILED;

#ifdef READ_SETTING_REG
	FdcReadSingleRegister(self, dev_id, FDC2x_RCOUNT_CH0, &temp1);
	FdcReadSingleRegister(self, dev_id, FDC2x_MUX_CONFIG, &temp2);
	FdcReadSingleRegister(self, dev_id, FDC2x_CONFIG, &temp3);
	FdcReadSingleRegister(self, dev_id, FDC2x_CLOCK_DIVIDERS_CH0, &temp4);
	FdcReadSingleRegister(self, dev_id, FDC2x_DRIVE_CURRENT_CH0, &temp5);
	FdcReadSingleRegister(self, dev_id, FDC2x_SETTLECOUNT_CH0, &temp6);
#endif
	return LED_I2C_SUCCESS;
}


int FdcReadDataReadyFlag(SoftwareI2cClass *self, uint8_t dev_id)
{
    uint16_t temp = 0;

    if (LED_I2C_FAILED ==  FdcReadSingleRegister(self, dev_id, FDC2x_STATUS, &temp) )
        return LED_I2C_FAILED;

    //MACRO_PRINTF("The STATUS value is 0x%x .. \n", temp);

    if( temp & DATA_READY_FLAG_MASK )
		return DATA_CONVERSION_READY;

    return DATA_NOT_READY;
}

int FdcReadCh0DataRegister(SoftwareI2cClass *self, uint8_t dev_id, uint32_t *data)
{
    uint16_t ltemp=0, htemp=0;

    if (LED_I2C_FAILED ==  FdcReadSingleRegister(self, dev_id, FDC2x_DATA_CH0, &htemp) )
        return LED_I2C_FAILED;
    htemp = htemp & DATA_BIT_MASK;

    if (LED_I2C_FAILED ==  FdcReadSingleRegister(self, dev_id, FDC2x_DATA_LSB_CH0, &ltemp) )
        return LED_I2C_FAILED;

    *data = ( (uint32_t)htemp<<16) + ltemp;

    return LED_I2C_SUCCESS;
}

void Fdc2212Reset(SoftwareI2cClass *self)
{
}

//=======================================================================================================
//                                     member function
//=======================================================================================================

int FdcReadSingleRegister(SoftwareI2cClass *self, uint8_t dev_id, uint8_t reg_addr, uint16_t *data)
{
	int res = 0;
    uint16_t read_data = 0;

    res = SwI2cBatchRead(self, dev_id, reg_addr, (uint8_t*)&read_data, REGISTER_BYTE_NUM);
    if(LED_I2C_SUCCESS != res)
		return LED_I2C_FAILED;

    *data = ByteOrderTrans2Byte(read_data);

    return LED_I2C_SUCCESS;
}

int FdcWriteSingleFdcRegister(SoftwareI2cClass *self, uint8_t dev_id, uint8_t reg_addr, uint16_t *data_)
{
	int res = 0;
	uint16_t data = ByteOrderTrans2Byte(*data_);

    SwI2cBatchWrite(self, dev_id, reg_addr, (uint8_t*)&data, REGISTER_BYTE_NUM);
    if(LED_I2C_SUCCESS != res)
        return LED_I2C_FAILED;

    return LED_I2C_SUCCESS;
}

//=======================================================================================================
// If a setting on the FDC needs to be changed, return the device to sleep mode,
// change the appropriate register, and then exit sleep mode
//=======================================================================================================
void Fdc2212SetSleepMode(SoftwareI2cClass *self)
{
	int res = 0;
    uint16_t read_data=0, send_data=0, temp=0;

    res = SwI2cBatchRead(self, FDC_I2C_ADDR, FDC2x_CONFIG, (uint8_t*)&read_data, 2);
    if(LED_I2C_SUCCESS != res)
        return;

    temp = ByteOrderTrans2Byte(*((uint16_t*)&read_data));
    temp |= SLEEP_MODE_EN_MASK;
    send_data = ByteOrderTrans2Byte(temp);

    SwI2cBatchWrite(self, FDC_I2C_ADDR, FDC2x_CONFIG, (uint8_t*)&send_data, 2);

}

void Fdc2212ExitSleepMode(SoftwareI2cClass *self)
{
	int res = 0;
    uint16_t read_data=0, send_data=0, temp=0;

    res = SwI2cBatchRead(self, FDC_I2C_ADDR, FDC2x_CONFIG, (uint8_t*)&read_data, 2);
    if(LED_I2C_SUCCESS != res)
        return;

    temp = ByteOrderTrans2Byte(*((uint16_t*)&read_data));
    temp &= ~SLEEP_MODE_EN_MASK;
    send_data = ByteOrderTrans2Byte(temp);

    SwI2cBatchWrite(self, FDC_I2C_ADDR, FDC2x_CONFIG, (uint8_t*)&send_data, 2);
}


char GetFdc2212AvergeValue(SoftwareI2cClass *self)
{
	int i = 0;

	FdcReadCh0DataRegister(self, FDC_I2C_ADDR,  &gReadDataObj.data[gReadDataObj.read_cnt]);
	gReadDataObj.read_cnt ++;

	if(gReadDataObj.read_cnt >= gReadDataObj.read_limit){
		gReadDataObj.read_cnt = 0;
		for(i=0; i<gReadDataObj.read_limit; i++){
			gReadDataObj.averge += gReadDataObj.data[gReadDataObj.read_cnt];
		}
		gReadDataObj.averge /= 5;
		return 1;
	}
	return 0;
}


#ifdef PARAM_DEBUG
extern UsartClass gConsoleUsart;
extern Fdc2212Class *Fdc2212Obj;
void ScopeSend(float data)
{
	DataScope_Get_Channel_Data( data , 1 );
	DataScope_Get_Channel_Data( pTouchTrackingDiff->pos_predict , 2 );
	DataScope_Get_Channel_Data( pTouchTrackingDiff->vel_predict , 3 );
	DataScope_Get_Channel_Data( Fdc2212Obj->falling_edge, 4 );
	DataScope_Get_Channel_Data( Fdc2212Obj->rising_edge , 5 );
	DataScope_Get_Channel_Data( Fdc2212Obj->high_threshold , 6 );
	DataScope_Get_Channel_Data( Fdc2212Obj->rising_flag , 7 );
	DataScope_Get_Channel_Data( Fdc2212Obj->falling_flag , 8 );
	DataScope_Get_Channel_Data( (float)gAprCtrlMap->touch_sensor_top*100 , 9 );

	int Send_Count = DataScope_Data_Generate(9);

	for(int i = 0 ; i < Send_Count; i++) {
		UsartPutchar(gConsoleUsart.usart,DataScope_OutPut_Buffer[i]);
	}
}
#endif

//extern GpioClass gDebugPin1;
extern SoftwareI2cClass gTouchI2c;
uint32_t mDiffCnt = 0;
// 10 us without i2c read
void Fdc2212DataProc(Fdc2212Class *self, DataAverage *avg, TrackingDiffClass *diff)
{
	if(0 == GetFdc2212AvergeValue(&gTouchI2c)){
		return;
	}

	//GPIO_SET(gDebugPin1);
	self->data_buff[0] = FDC2212_CONS_VALUE;
	self->fdc_data=( avg->averge & FDC2212_VALID_BIT) >> 10;
	avg->averge = 0;

#ifdef PARAM_DEBUG
	ScopeSend(self->fdc_data);
#endif
	
	TrackingDiffUpdate(diff, self->fdc_data);
	if(mDiffCnt <= FDC2212_DIFFTIME){
		mDiffCnt ++;
		return;
	}
	
	if((diff->vel_predict > self->high_threshold) && (100 == self->rising_flag)){
			self->rising_flag = 0;
			self->rising_edge = 100;
	}
	else if(100 == self->falling_flag)
	{
		self->data_buff[self->data_cnt] = diff->vel_predict;

		if((self->data_buff[self->data_cnt] - self->data_buff[self->data_cnt-1]) > 0){
				self->falling_flag = 0;
				self->rising_flag = 100;
				self->high_threshold = ((- FDC2212_COEFFICIENT * self->data_buff[self->data_cnt-1]));   //
				if(self->threshold_limit_h < self->high_threshold){
					self->high_threshold = self->threshold_limit_h;
				}
				else if(self->threshold_limit_l > self->high_threshold){
					self->high_threshold = self->threshold_limit_l;
				}
				self->data_cnt = 0;
		}
		self->data_cnt++;
	}
	else if(diff->vel_predict < self->low_threshold && 0 == self->falling_flag){
		if (0 == self->falling_edge){
			self->falling_flag = 100;
		}
		self->falling_edge = 100;
	}
	else{
		self->falling_edge = 0;
		self->rising_edge = 0;
	}
	//GPIO_RESET(gDebugPin1);
}
