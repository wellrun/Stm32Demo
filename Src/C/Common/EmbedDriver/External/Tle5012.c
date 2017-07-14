#include "Tle5012.h"

#define SPI2_GPIO	GPIOB	
#define SPI2_CLK	GPIO_Pin_13
#define SPI2_CSN	GPIO_Pin_14
#define SPI2_DATA	GPIO_Pin_15
#define SPI2_EN		GPIO_WriteBit(SPI2_GPIO, SPI2_CSN, Bit_RESET)
#define SPI2_DIS	GPIO_WriteBit(SPI2_GPIO, SPI2_CSN, Bit_SET)

int32_t Spi2SendHalfWord(uint16_t *data, uint16_t size)
{
	uint8_t retry = 0;
	uint16_t i = 0;
	SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
	
	SPI_Cmd(SPI2, ENABLE);
	
	for(i = 0; i < size; i++) {
		SPI_I2S_SendData(SPI2, data[i]); //????SPIx??????
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) {//?????SPI???????:????????
			retry++;
			if(retry>200) return -1;
		}
		while( SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET) {	
			retry++;
			if(retry>200) return -1;
		}

	}
	GPIOB->CRH |= (3<<20);
	GPIOB->CRH &= ~(3<<22); //?????????
	GPIO_ResetBits(GPIOB, GPIO_Pin_13); //??????
	
	SPI_Cmd(SPI2, DISABLE);
	return 0;
}

int32_t Spi2ReadHalfWord(uint16_t *data, uint16_t size)
{
	uint8_t retry = 0;
	SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
	uint16_t i = 0;
	
	SPI_Cmd(SPI2, ENABLE);
	GPIOB->CRH |= (3<<20); 
	GPIOB->CRH |= (3<<22); //?????????

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) {
		retry++;
		if(retry>200) return -1;
	}
	SPI_I2S_ReceiveData(SPI2);
	
	for(i = 0; i < size; i++) {
		while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) {//?????SPI???????:?????????
			retry++;
			if(retry>200) return -1;
		}	
		data[i] = SPI_I2S_ReceiveData(SPI2);
	}
	
	SPI_Cmd(SPI2, DISABLE);
	return 0;
}


int32_t Tle5012Init(Tle5012 * self)
{
//	Tle5012 * tle5012_ptr = (Tle5012 *) self;
//	tle5012_ptr->protocol.load_angle = Tle5012LoadAngle;
//	tle5012_ptr->protocol.load_angle_and_speed = Tle5012LoadAngleAndSpeed;
//	tle5012_ptr->protocol.load_angle_speed = Tle5012LoadAngleSpeed;
	
	if(self->mode == TLE5012_OD_MODE) {
		uint16_t cmmd_init[2] = {0x5061, 0x8009}; //0 0000 0 000110 0001,  10 xxxxxxxxx 0 1 0 01
		uint16_t *temp1;
		SPI2_EN;
		if( -1 == Spi2SendHalfWord(cmmd_init, 2)) return -1;
		if( -1 == Spi2ReadHalfWord(temp1,1) ) return -1;
		SPI2_DIS;
	}
	return 0;
	
}


#define READ_FLAG	(0x8000)
uint16_t Tle5012ReadReg(Tle5012 * self, uint8_t device_info, uint8_t addr)
{
	uint16_t temp = 0;
	RegisterDeviceInterface * di = self->device_interface;
	di->device_memory_read(di, device_info, addr, (uint8_t *)&temp, 1);
	return 0;
}


int32_t Tle5012WriteReg(Tle5012 * self, uint8_t device_info, uint8_t addr, uint16_t data)
{
	return 0;
}


int32_t Tle5012ReadRegs(Tle5012 * self, uint8_t device_info, uint8_t addr, uint16_t * data, int32_t size)
{
	RegisterDeviceInterface * di = self->device_interface;
	di->device_memory_read(di, device_info, addr, (uint8_t *)data, size);
	return 0;
}


int32_t Tle5012WriteRegs(Tle5012 * self, uint8_t device_info, uint8_t addr, const uint16_t * data, int32_t size);
	
int32_t Tle5012LoadAngleAndSpeed(Tle5012 * self)
{
	uint16_t temp[3] = {2};
	Tle5012ReadRegs(self, self->device_info, 2, temp, 2);
	self->angle_value = temp[0] & 0x7FFFu;
	self->angle_speed = (int32_t)(temp[1] & 0x3FFFu) * (temp[1] & 0x4000)?(-1):(1);
	return 0;
}

int32_t Tle5012LoadAngle(Tle5012 * self)
{
	uint16_t temp[2] = {0};
	Tle5012ReadRegs(self, self->device_info, 2, temp, 1);
	self->angle_value = temp[0] & 0x7FFFu;
	return 0;
}

int32_t Tle5012LoadAngleSpeed(Tle5012 * self)
{
	uint16_t temp[2] = {0};
	Tle5012ReadRegs(self, self->device_info, 3, temp, 1);
	self->angle_speed = (int32_t)(temp[1] & 0x3FFFu) * (temp[1] & 0x4000)?(-1):(1);
	return 0;
}

//int32_t Tle5012ODReadRegs(Tle5012 * self, uint8_t addr, uint16_t * data, uint16_t data_size)
//{
//	uint16_t command = 0x0000;
//	
//	command |= READ_FLAG;
//	command |= (addr<<4);
//	command |= data_size;
//	
//	SPI2_EN;
//	if( -1 == Spi2SendHalfWord(&command, 1)) return -1;
//	if( -1 == Spi2ReadHalfWord(data, data_size+1)) return -1;
//	SPI2_DIS;
//	
//	return 0;
//}


//int32_t Tle5012ODReadAngleAndSpeed(Tle5012 * self, uint16_t *encoder_angle)
//{
//	uint16_t temp[2] = {0};
//	if(-1 == Tle5012ODReadRegs(self, AVAL, temp, 1)) return -1;
//	
//	*encoder_angle = temp[0] & 0x7FFF;

//	return 0;
//}


