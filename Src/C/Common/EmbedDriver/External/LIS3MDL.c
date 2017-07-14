
#include "LIS3MDL.h"


#define Delay_Ms(t) LIS3MDLDelayUs(1000 * (t))


static void LIS3MDLDelayUs(unsigned long t)
{
	volatile int i = 0;
    for(int j = 0; j < t; j ++){
		i = 0;
        while(i < 10) i ++;
	}
}

uint8_t lis3mdl_whoami = 0;
int32_t LIS3MDLInit(LIS3MDL * self)
{
	uint8_t data = 0, state = 0;
	LIS3MDL * lis3mdl_ptr = (LIS3MDL *)self;
	if (lis3mdl_ptr->device_interface->periph_id != PERIPH_ID_SPI) {
		return 1;
	}
	
	self->output_rt_data[0].data = (void *)self->mag_output_buffer;
	self->output_rt_data[0].time_stamp = 0;
	
	//TEST
	lis3mdl_whoami = LIS3MDL_READ_REG(lis3mdl_ptr, LIS3MDL_ADDR_WHO_AM_I);
	Delay_Ms(1);
	
	//RESET
	data = 0;
	LIS3MDL_SET_REG_FEILD(data, LIS3MDL_CTRL_REG2, SOFT_RST, LIS3MDL_ENABLE);					//RESET the device
	LIS3MDL_WRITE_REG(lis3mdl_ptr, LIS3MDL_ADDR_CTRL_REG2, data);
	Delay_Ms(100);
	
	//REG1
	data = 0;
	LIS3MDL_SET_REG_FEILD(data, LIS3MDL_CTRL_REG1, OM, LIS3MDL_ULTRA_HIGH_PERFORMANCE);			//set the xy axis performance ultr-high
	LIS3MDL_SET_REG_FEILD(data, LIS3MDL_CTRL_REG1, DO, 0x07);									//set the output rate as 80hz
	LIS3MDL_SET_REG_FEILD(data, LIS3MDL_CTRL_REG1, FAST_ODR, LIS3MDL_ENABLE);					//set the output data rate faster than 80hz
	LIS3MDL_WRITE_REG(lis3mdl_ptr, LIS3MDL_ADDR_CTRL_REG1, data);								//write to reg
	Delay_Ms(1);
	
	//REG2
	data = 0;
	LIS3MDL_SET_REG_FEILD(data, LIS3MDL_CTRL_REG2, FS, LIS3MDL_FSR_4GAUSS);						//set the full scale as 4 guass
	LIS3MDL_WRITE_REG(lis3mdl_ptr, LIS3MDL_ADDR_CTRL_REG2, data);								//write to reg
	Delay_Ms(1); 
	
	//REG3
	data = 0;
	LIS3MDL_SET_REG_FEILD(data, LIS3MDL_CTRL_REG3, MD, LIS3MDL_CONTINUOUS_CONVERSION_MODE);		//set the system operation mode as continuous conversion
	LIS3MDL_WRITE_REG(lis3mdl_ptr, LIS3MDL_ADDR_CTRL_REG3, data);								//write to reg
	Delay_Ms(1);

	//REG4
	data = 0;
	LIS3MDL_SET_REG_FEILD(data, LIS3MDL_CTRL_REG4, OMZ, LIS3MDL_ULTRA_HIGH_PERFORMANCE);		//set the z axis performance ultr-high
	LIS3MDL_WRITE_REG(lis3mdl_ptr, LIS3MDL_ADDR_CTRL_REG4, data);								//write to reg
	Delay_Ms(1);
	
	return 0;
}

/**
 * @brief MPU read a byte from the register
 * @param self: Object Pointer 
 * @param device_info: use to indicate the i2c device addr in i2c mode, use to indicate the cs pin index in spe mode
 * @param addr: register addr
 * @return the value read from register
 * @author xinyu.han
 */
uint8_t LIS3MDLReadReg  (LIS3MDL * self, uint16_t device_info, uint8_t addr)
{
	uint8_t temp = 0;
	RegisterDeviceInterface * di = self->device_interface;
	di->device_memory_read(di, device_info, addr, &temp, 1);				//call the interface function to read byte
	return temp;
}

/**
 * @brief MPU write a byte to the register
 * @param self: Object Pointer 
 * @param device_info: use to indicate the i2c device addr in i2c mode, use to indicate the cs pin index in spe mode
 * @param addr: register addr
 * @param data: byte to write in register
 * @return return value of interface function
 * @author xinyu.han
 */
int32_t LIS3MDLWriteReg (LIS3MDL * self, uint16_t device_info, uint8_t addr, uint8_t data)
{
	uint8_t temp = data;
	RegisterDeviceInterface * di = self->device_interface;
	return di->device_memory_write(di, device_info, addr, &temp, 1);		//call the interface function to write byte
}

/**
 * @brief MPU read amount of data from the register
 * @param self: Object Pointer 
 * @param device_info: use to indicate the i2c device addr in i2c mode, use to indicate the cs pin index in spe mode
 * @param addr: register addr
 * @param data: data block to write in register
 * @return return value of interface function
 * @author xinyu.han
 */
int32_t LIS3MDLReadRegs (LIS3MDL * self, uint16_t device_info, uint8_t addr, uint8_t * data, int32_t size)
{
	RegisterDeviceInterface * di = self->device_interface;
	return di->device_memory_read(di, device_info, addr, data, size);		//call the interface function to read bytes
}

/**
 * @brief MPU write amount of data to the register
 * @param self: Object Pointer 
 * @param device_info: use to indicate the i2c device addr in i2c mode, use to indicate the cs pin index in spe mode
 * @param addr: register addr
 * @param data: data buffer used save the data read from register
 * @return return value of interface function
 * @author xinyu.han
 */
int32_t LIS3MDLWriteRegs(LIS3MDL * self, uint16_t device_info, uint8_t addr, const uint8_t * data, int32_t size)
{
	RegisterDeviceInterface * di = self->device_interface;
	return di->device_memory_write(di, device_info, addr, data, size);		//call the interface function to write bytes
}

/**
 * @brief MPU load mag raw data into its mag buffer(.mag)
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t LIS3MDLLoadMag(LIS3MDL * self)
{
	uint8_t data[7] = {0};
	LIS3MDL_READ_REGS(self, LIS3MDL_ADDR_STATUS_REG, data, 7);
	
	if( !(LIS3MDL_GET_REG_FEILD(data[0], LIS3MDL_STATUS_REG, ZYXDA)==1 || 
		  LIS3MDL_GET_REG_FEILD(data[0], LIS3MDL_STATUS_REG, ZYXOR)==1 )) {
		return -1;
	}
	self->mag[0] = (int16_t)(data[2] << 8 | data[1]);
	self->mag[1] = (int16_t)(data[4] << 8 | data[3]);
	self->mag[2] = (int16_t)(data[6] << 8 | data[5]);
	
	self->output_rt_data[0].time_stamp++;
	
	return 0;
}


int32_t LIS3MDLGetRawMag(LIS3MDL * self, int32_t * mag)
{
	if( mag == NULL )
		return -1;
	
	mag[0] = self->mag[0];
	mag[1] = self->mag[1];
	mag[2] = self->mag[2];
	
	return 0;
}

extern void SwapAixs(int32_t * input, int32_t * output, uint8_t axis_map, uint8_t axis_dir);

RtData * LIS3MDLPrepareOutputData(LIS3MDL * self)
{
	int32_t mag[3] = {0};
	if (LIS3MDLLoadMag(self) == 0) {
		SwapAixs(self->mag, mag, self->mag_axis_map, self->mag_axis_dir);
		self->mag_output_buffer[0] = (float)mag[0];
		self->mag_output_buffer[1] = (float)mag[1];
		self->mag_output_buffer[2] = (float)mag[2];
		return &(self->output_rt_data[0]);
	}
	else {
		return NULL;
	}
	return NULL;
}









