/*
The MIT License (MIT)

Copyright (c) 2015-? suhetao

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "Mpu9250.h"
//////////////////////////////////////////////////////////////////////////
//
static int16_t MPU9250_AK8963_ASA[3] = {0, 0, 0};

//////////////////////////////////////////////////////////////////////////
#define Delay_Ms(t) Mpu9250DelayUs(1000 * (t))



static void Mpu9250DelayUs(unsigned long t)
{
	volatile int i = 0;
    for(int j = 0; j < t; j ++){
		i = 0;
        while(i < 10) i ++;
	}
}



//init

int32_t Mpu9250Init(Mpu9250 * self)
{
	uint8_t data = 0, state = 0;
	uint8_t response[3] = {0, 0, 0};
	
	self->output_rt_data[0].data = (void *)self->acc;
	self->output_rt_data[0].time_stamp = 0;
	self->output_rt_data[1].data = (void *)self->gyro;
	self->output_rt_data[1].time_stamp = 0;
	self->output_rt_data[2].data = (void *)self->mag;
	self->output_rt_data[2].time_stamp = 0;
	
	//Lower level hardware Init
	
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;
	if (mpu9250_ptr->device_interface->periph_id == PERIPH_ID_SPI)
	{
		mpu9250_ptr->protocol.mag_read_reg = Mpu9250ReadMagRegSPI;
		mpu9250_ptr->protocol.mag_write_reg = Mpu9250WriteMagRegSPI;
		mpu9250_ptr->protocol.mag_read_regs = Mpu9250ReadMagRegsSPI;
		mpu9250_ptr->protocol.mag_write_regs = Mpu9250WriteMagRegsSPI;
		mpu9250_ptr->imu_device_info = mpu9250_ptr->device_info;
	}
	else if (mpu9250_ptr->device_interface->periph_id == PERIPH_ID_I2C)
	{
		mpu9250_ptr->protocol.mag_read_reg = Mpu9250ReadMagRegI2C;
		mpu9250_ptr->protocol.mag_write_reg = Mpu9250WriteMagRegI2C;
		mpu9250_ptr->protocol.mag_read_regs = Mpu9250ReadMagRegsI2C;
		mpu9250_ptr->protocol.mag_write_regs = Mpu9250WriteMagRegsI2C;

	}
	else
		return -1;
	
	mpu9250_ptr->status = 0;
	
	
	mpu9250_ptr->protocol.read_reg = Mpu9250ReadReg;
	mpu9250_ptr->protocol.write_reg = Mpu9250WriteReg;
	mpu9250_ptr->protocol.read_regs = Mpu9250ReadRegs;
	mpu9250_ptr->protocol.write_regs = Mpu9250WriteRegs;

	if(Mpu9250CheckDevice(mpu9250_ptr) == -1) {
		mpu9250_ptr->status = MPU9250_STATUS_OFFLINE;
		return -1;
	}

	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_PWR_MGMT_1, MPU9250_RESET);
	Delay_Ms(100);
	//MPU9250 Set Clock Source
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_PWR_MGMT_1,  MPU9250_CLOCK_PLLGYROZ);
	Delay_Ms(1);
	//MPU9250 Set Interrupt
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_INT_PIN_CFG,  MPU9250_INT_ANYRD_2CLEAR);
	Delay_Ms(1);
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_INT_ENABLE, ENABLE);
	Delay_Ms(1);
	//MPU9250 Set Sensors
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_PWR_MGMT_2, MPU9250_XYZ_GYRO & MPU9250_XYZ_ACCEL); //
	Delay_Ms(1);
	//MPU9250 Set SampleRate
	//SAMPLE_RATE = Internal_Sample_Rate / (1 + SMPLRT_DIV)
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_SMPLRT_DIV, SMPLRT_DIV);
	Delay_Ms(1);
	//MPU9250 Set Full Scale Gyro Range
	//Fchoice_b[1:0] = [00] enable DLPF
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_GYRO_CONFIG, (MPU9250_FSR_2000DPS << 3));
	Delay_Ms(1);
	//MPU9250 Set Full Scale Accel Range PS:2G
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_ACCEL_CONFIG, (MPU9250_FSR_8G << 3));
	Delay_Ms(1);
	//MPU9250 Set Accel DLPF
	data = MPU9250_READ_REG(mpu9250_ptr, MPU9250_ACCEL_CONFIG2);
	data |= MPU9250_ACCEL_DLPF_41HZ;
	Delay_Ms(1);
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_ACCEL_CONFIG2, data);
	Delay_Ms(1);
	//MPU9250 Set Gyro DLPF
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_CONFIG, MPU9250_GYRO_DLPF_41HZ);
	Delay_Ms(1);
	

	//MPU9250 Set SPI Mode
	if (mpu9250_ptr->device_interface->periph_id == PERIPH_ID_SPI)
	{
		state = MPU9250_READ_REG(mpu9250_ptr, MPU9250_USER_CTRL);
		Delay_Ms(1);
		MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_USER_CTRL, state | MPU9250_I2C_IF_DIS);
		Delay_Ms(1);
		state = MPU9250_READ_REG(mpu9250_ptr, MPU9250_USER_CTRL);
		Delay_Ms(1);
		MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_USER_CTRL, state | MPU9250_I2C_MST_EN);
		Delay_Ms(1);
	}
	else if(mpu9250_ptr->device_interface->periph_id == PERIPH_ID_I2C)
	{
		MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_INT_PIN_CFG, 0x02);
		Delay_Ms(1);
	}

	if (mpu9250_ptr->mode & MPU9250_MAG) {
		//////////////////////////////////////////////////////////////////////////
		//AK8963 Setup

		//reset AK8963
		MPU9250_MAG_WRITE_REG(mpu9250_ptr, MPU9250_AK8963_CNTL2, MPU9250_AK8963_CNTL2_SRST);
		Delay_Ms(2);

		MPU9250_MAG_WRITE_REG(mpu9250_ptr, MPU9250_AK8963_CNTL, MPU9250_AK8963_POWER_DOWN);
		Delay_Ms(1);
		MPU9250_MAG_WRITE_REG(mpu9250_ptr, MPU9250_AK8963_CNTL, MPU9250_AK8963_FUSE_ROM_ACCESS);
		Delay_Ms(1);
		//
		//AK8963 get calibration data
		MPU9250_MAG_READ_REGS(mpu9250_ptr, MPU9250_AK8963_ASAX, response, 3);
		//AK8963_SENSITIVITY_SCALE_FACTOR
		//AK8963_ASA[i++] = (int16_t)((data - 128.0f) / 256.0f + 1.0f) ;
		MPU9250_AK8963_ASA[0] = (int16_t)(response[0]) + 128;
		MPU9250_AK8963_ASA[1] = (int16_t)(response[1]) + 128;
		MPU9250_AK8963_ASA[2] = (int16_t)(response[2]) + 128;
		Delay_Ms(1);
		MPU9250_MAG_WRITE_REG(mpu9250_ptr, MPU9250_AK8963_CNTL, MPU9250_AK8963_POWER_DOWN);
		Delay_Ms(1);
		//
		if (mpu9250_ptr->device_interface->periph_id == PERIPH_ID_SPI) {
			MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_MST_CTRL, 0x5D);
			Delay_Ms(1);
			MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV0_ADDR, ((mpu9250_ptr->mag_device_info)>>1) | MPU9250_I2C_READ);
			Delay_Ms(1);
			MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV0_REG, MPU9250_AK8963_ST1);
			Delay_Ms(1);
			MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV0_CTRL, 0x88);
			Delay_Ms(1);
		}
		//
		MPU9250_MAG_WRITE_REG(mpu9250_ptr, MPU9250_AK8963_CNTL, MPU9250_AK8963_CONTINUOUS_MEASUREMENT);
		Delay_Ms(1);
		if (mpu9250_ptr->device_interface->periph_id == PERIPH_ID_SPI) {
			MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_CTRL, 0x09);
			Delay_Ms(1);
			//
			MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_MST_DELAY_CTRL, 0x81);
			Delay_Ms(100);
		}
	}
	
	mpu9250_ptr->status = MPU9250_STATUS_RUN;
	
	return 0;

}


void SwapAixs(int32_t * input, int32_t * output, uint8_t axis_map, uint8_t axis_dir)
{
	
	switch(axis_map)
	{
		case AXIS_MAP_XYZ:
			output[0] = input[0];
		    output[1] = input[1];
		    output[2] = input[2];
			break;
		case AXIS_MAP_XZY:
			output[0] = input[0];
		    output[1] = input[2];
		    output[2] = input[1];
			break;
		case AXIS_MAP_ZYX:	
			output[0] = input[2];
		    output[1] = input[1];
		    output[2] = input[0];
			break;
		case AXIS_MAP_ZXY:
			output[0] = input[2];
		    output[1] = input[0];
		    output[2] = input[1];
			break;
		case AXIS_MAP_YZX:
			output[0] = input[1];
		    output[1] = input[2];
		    output[2] = input[0];
			break;
		case AXIS_MAP_YXZ:
			output[0] = input[1];
		    output[1] = input[0];
		    output[2] = input[2];
			break;
		default:
			break;
	}

	if(axis_dir & AXIS_INV_X)
		output[0] = -output[0];
	if(axis_dir & AXIS_INV_Y)
		output[1] = -output[1];
	if(axis_dir & AXIS_INV_Z)
		output[2] = -output[2];
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
int32_t Mpu9250WriteReg(Mpu9250 * self, uint16_t device_info, uint8_t addr, uint8_t data)
{
	RegisterDeviceInterface * di = self->device_interface;
	return di->device_memory_write(di, device_info, addr, &data, 1);		//call the interface function to write byte
}

/**
 * @brief MPU read a byte from the register
 * @param self: Object Pointer 
 * @param device_info: use to indicate the i2c device addr in i2c mode, use to indicate the cs pin index in spe mode
 * @param addr: register addr
 * @return the value read from register
 * @author xinyu.han
 */
uint8_t Mpu9250ReadReg(Mpu9250 * self, uint16_t device_info, uint8_t addr)
{
	uint8_t temp = 0;
	RegisterDeviceInterface * di = self->device_interface;
	di->device_memory_read(di, device_info, addr, &temp, 1);		//call the interface function to read byte
	return temp;
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
int32_t Mpu9250ReadRegs(Mpu9250 * self, uint16_t device_info, uint8_t addr, uint8_t * data, int32_t size)
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
int32_t Mpu9250WriteRegs(Mpu9250 * self, uint16_t device_info, uint8_t addr, const uint8_t * data, int32_t size)
{
	RegisterDeviceInterface * di = self->device_interface;
	di->device_memory_write(di, device_info, addr, data, size);		//call the interface function to write bytes
	return 0;
}

/**
 * @brief MPU read a byte from the magnet register
 * @param self: Object Pointer 
 * @param device_info: use to indicate the i2c device addr in i2c mode, use to indicate the cs pin index in spe mode
 * @param addr: register addr
 * @return the value read from register
 * @author xinyu.han
 */
uint8_t Mpu9250ReadMagRegSPI(Mpu9250 * self, uint16_t device_info, uint8_t addr)
{
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;
	uint8_t status = 0;
	uint32_t timeout = 0;
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_REG, addr);
	Delay_Ms(1);
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_ADDR, (device_info>>1) | MPU9250_I2C_READ);
	Delay_Ms(1);
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_CTRL, MPU9250_I2C_SLV4_EN);
	Delay_Ms(1);

	do {
		if (timeout++ > 50){
			return 0xFF;
		}
		status = MPU9250_READ_REG(mpu9250_ptr, MPU9250_I2C_MST_STATUS);
		Delay_Ms(1);
	} while ((status & MPU9250_I2C_SLV4_DONE) == 0);
	return MPU9250_READ_REG(mpu9250_ptr, MPU9250_I2C_SLV4_DI);
}

/**
 * @brief MPU write a byte to the magnet register
 * @param self: Object Pointer 
 * @param device_info: use to indicate the i2c device addr in i2c mode, use to indicate the cs pin index in spe mode
 * @param addr: register addr
 * @param data: byte to write in register
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t Mpu9250WriteMagRegSPI(Mpu9250 * self, uint16_t device_info, uint8_t addr, uint8_t data)
{
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;

	uint8_t status = 0;
	uint32_t timeout = 0;

	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_REG, addr);
	Delay_Ms(1);
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_ADDR, (device_info>>1));
	Delay_Ms(1);
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_DO, data);
	Delay_Ms(1);
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_CTRL, MPU9250_I2C_SLV4_EN);
	Delay_Ms(1);

	do {
		if (timeout++ > 50){
			return -2;
		}
		status = MPU9250_READ_REG(mpu9250_ptr, MPU9250_I2C_MST_STATUS);
		Delay_Ms(1);
	} while ((status & MPU9250_I2C_SLV4_DONE) == 0);
	if (status & MPU9250_I2C_SLV4_NACK)
		return -3;
	return 0;
}

/**
 * @brief MPU read amount of data from the magnet register
 * @param self: Object Pointer 
 * @param device_info: use to indicate the i2c device addr in i2c mode, use to indicate the cs pin index in spe mode
 * @param addr: register addr
 * @param data: data block to write in register
 * @return return value of interface function
 * @author xinyu.han
 */
int32_t Mpu9250ReadMagRegsSPI(Mpu9250 * self, uint16_t device_info, uint8_t addr, uint8_t * data, int32_t size)
{
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;
	uint8_t index = 0;
	uint8_t status = 0;
	uint32_t timeout = 0;
	
	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_ADDR, (device_info>>1) | MPU9250_I2C_READ);
	Delay_Ms(1);
	while(index < size){
		MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_REG, addr + index);
		Delay_Ms(1);
		MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_CTRL, MPU9250_I2C_SLV4_EN);
		Delay_Ms(1);

		do {
			if (timeout++ > 50){
				return -2;
			}
			status = MPU9250_READ_REG(mpu9250_ptr, MPU9250_I2C_MST_STATUS);
			Delay_Ms(2);
		} while ((status & MPU9250_I2C_SLV4_DONE) == 0);
		data[index] = MPU9250_READ_REG(mpu9250_ptr, MPU9250_I2C_SLV4_DI);
		Delay_Ms(1);
		index++;
	}
	return 0;
}

/**
 * @brief MPU write amount of data to the magnet register
 * @param self: Object Pointer 
 * @param device_info: use to indicate the i2c device addr in i2c mode, use to indicate the cs pin index in spe mode
 * @param addr: register addr
 * @param data: data buffer used save the data read from register
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t Mpu9250WriteMagRegsSPI(Mpu9250 * self, uint16_t device_info, uint8_t addr, const uint8_t * data, int32_t size)
{
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;
	uint32_t timeout = 0;
	uint8_t status = 0;
	uint8_t index = 0;


	MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_ADDR, (device_info>>1));
	Delay_Ms(1);

	while(index < size){
		MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_REG, addr + index);
		Delay_Ms(1);
		MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_DO, data[index]);
		Delay_Ms(1);
		MPU9250_WRITE_REG(mpu9250_ptr, MPU9250_I2C_SLV4_CTRL, MPU9250_I2C_SLV4_EN);
		Delay_Ms(1);

		do {
			if (timeout++ > 50)
				return -2;
			status = MPU9250_READ_REG(mpu9250_ptr, MPU9250_I2C_MST_STATUS);
			Delay_Ms(1);
		} while ((status & MPU9250_I2C_SLV4_DONE) == 0);
		if (status & MPU9250_I2C_SLV4_NACK)
			return -3;
		index++;
	}
	return 0;
}

/**
 * @brief MPU load gyro raw data into its gyro buffer(.gyro)
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t Mpu9250LoadGyro(Mpu9250 * self)
{
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;
	uint8_t data[6];
	int32_t gyro[3];
	int32_t return_val = 0;
	
	return_val = MPU9250_READ_REGS(mpu9250_ptr, MPU9250_GYRO_XOUT_H, data, 6);
	
	gyro[0] = (int16_t)((data[0] << 8) | data[1]);
	gyro[1] = (int16_t)((data[2] << 8) | data[3]);
	gyro[2] = (int16_t)((data[4] << 8) | data[5]);
	
	
	
	SwapAixs(gyro, self->gyro, self->gyro_axis_map, self->gyro_axis_dir);
	
	self->gyro[0] -= self->gyro_zero[0];
	self->gyro[1] -= self->gyro_zero[1];
	self->gyro[2] -= self->gyro_zero[2];
	
	
	self->output_rt_data[1].time_stamp++;
	
	return return_val;

}


/**
 * @brief MPU load acc raw data into its acc buffer(.acc)
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t Mpu9250LoadAcc(Mpu9250 * self)
{
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;
	uint8_t data[6];
	int32_t acc[3];
	int32_t return_val = 0;
	uint32_t square_sum;
	return_val = MPU9250_READ_REGS(mpu9250_ptr, MPU9250_ACCEL_XOUT_H, data, 6);
	
	acc[0] = (int16_t)((data[0] << 8) | data[1]);
	acc[1] = (int16_t)((data[2] << 8) | data[3]);
	acc[2] = (int16_t)((data[4] << 8) | data[5]);
	
	SwapAixs(acc, self->acc, self->acc_axis_map, self->acc_axis_dir);

	self->output_rt_data[0].time_stamp++;
	square_sum =acc[0] * acc[0] +
				acc[1] * acc[1] +
				acc[2] * acc[2];
	
	if(square_sum >= 4000000) {
		self->output_rt_data[0].time_stamp++;
		self->acc_error_cnt = 0;
		self->acc_error_flag = 0;
	}
	else if(self->acc_error_flag != 0){
		self->acc_error_cnt++;
		if(self->acc_error_cnt > 2000) {
			self->acc_error_flag = 1;
		}
	}
	
	return return_val;
}

/**
 * @brief MPU load mag raw data into its mag buffer(.mag)
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t Mpu9250LoadMag(Mpu9250 * self)
{
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;
	uint8_t data[8];
	int32_t mag[3];
	int32_t return_val = 0;
	if(mpu9250_ptr->mode & MPU9250_MAG) {
		if(mpu9250_ptr->device_interface->periph_id == PERIPH_ID_SPI) {
			return_val = MPU9250_READ_REGS(mpu9250_ptr, MPU9250_EXT_SENS_DATA_00, data, 8);
		} 
		else if(mpu9250_ptr->device_interface->periph_id == PERIPH_ID_I2C) {
			return_val = MPU9250_MAG_READ_REGS(mpu9250_ptr, MPU9250_AK8963_ST1, data, 8);
		}
		if(return_val != 0) {
			return -1;
		}
//		self->output_data[0] = NULL;
//		self->output_data[1] = NULL;
		if (!(data[0] & MPU9250_AK8963_DATA_READY) || (data[0] & MPU9250_AK8963_DATA_OVERRUN)){
//			self->output_data[2] = NULL;
			return 1;
		}
		if (data[7] & MPU9250_AK8963_OVERFLOW){
//			self->output_data[2] = NULL;
			return 2;
		}

		mag[0] = (int16_t)((data[2] << 8) | data[1]);
		mag[1] = (int16_t)((data[4] << 8) | data[3]);
		mag[2] = (int16_t)((data[6] << 8) | data[5]);
		
		SwapAixs(mag, self->mag, self->mag_axis_map, self->mag_axis_dir);
		
		self->output_rt_data[2].time_stamp++;
	}
	else
		return 3;
	
	return 0;
}

/**
 * @brief MPU load 6 axis raw data into its buffer(.acc and .gyro)
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t Mpu9250Load6Axis(Mpu9250 * self)
{
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;
	uint8_t data[14];
	int32_t return_val = 0;
	int32_t gyro[3];
	int32_t acc[3];
	uint32_t square_sum = 0;
	return_val = MPU9250_READ_REGS(mpu9250_ptr, MPU9250_ACCEL_XOUT_H, data, 14);
	
	acc[0] = (int16_t)((data[0] << 8) | data[1]);
	acc[1] = (int16_t)((data[2] << 8) | data[3]);
	acc[2] = (int16_t)((data[4] << 8) | data[5]);

	gyro[0] = (int16_t)((data[8] << 8) | data[9]);
	gyro[1] = (int16_t)((data[10] << 8) | data[11]);
	gyro[2] = (int16_t)((data[12] << 8) | data[13]);
	
	square_sum = acc[0] * acc[0] +
			acc[1] * acc[1] +
			acc[2] * acc[2];
		
	if(square_sum >= 4000000) {
		self->output_rt_data[0].time_stamp++;
		self->acc_error_cnt = 0;
		self->acc_error_flag = 0;
	}
	else if(self->acc_error_flag != 0){
		self->acc_error_cnt++;
		if(self->acc_error_cnt > 2000) {
			self->acc_error_flag = 1;
		}
	}
	
	SwapAixs(acc, self->acc, self->acc_axis_map, self->acc_axis_dir);

	SwapAixs(gyro, self->gyro, self->gyro_axis_map, self->gyro_axis_dir);

	self->gyro[0] -= self->gyro_zero[0];
	self->gyro[1] -= self->gyro_zero[1];
	self->gyro[2] -= self->gyro_zero[2];
	
	self->output_rt_data[0].time_stamp++;
	self->output_rt_data[1].time_stamp++;


	return return_val;
}

/**
 * @brief MPU load 9 axis raw data into its buffer(.acc, .gyro and .mag)
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t Mpu9250Load9Axis(Mpu9250 * self)
{
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;
	uint8_t data[22];
	int32_t return_val = 0;
	int32_t acc[3];
	int32_t gyro[3];
	int32_t mag[3];
	uint32_t square_sum = 0;
	
	if(mpu9250_ptr->device_interface->periph_id == PERIPH_ID_SPI) {
		//if in spi mode, mag and imu data are in continuing register aera, so get them in one read can save time
		if(mpu9250_ptr->mode & MPU9250_MAG) {
			//if mag sensor is avilable, get all data.
			return_val = MPU9250_READ_REGS(mpu9250_ptr, MPU9250_ACCEL_XOUT_H, data,22);
			if(return_val != 0)
				return -1;
			if (!(data[14] & MPU9250_AK8963_DATA_READY) || (data[14] & MPU9250_AK8963_DATA_OVERRUN)) {
				//if mag sensor data is not be updated, abandon the data.
//				mpu9250_ptr->output_data[2] = NULL;
				return_val = 1;
			}
			else if (data[21] & MPU9250_AK8963_OVERFLOW) {
				//if mag sensor data is abnormal, abandon the data.
//				mpu9250_ptr->output_data[2] = NULL;
				return_val = 2;
			}
			else {
				//regist the pointer in the output data buffer list so that other module can access.
				mpu9250_ptr->output_data[2] = self->mag_output_buffer;			
				//if the data is good, save them to the mag buffer.
				mag[0] = (int16_t)((data[16] << 8) | data[15]);
				mag[1] = (int16_t)((data[18] << 8) | data[17]);
				mag[2] = (int16_t)((data[20] << 8) | data[19]);

				SwapAixs(mag, self->mag, self->mag_axis_map, self->mag_axis_dir);
				
				//ned x,y,z //AK8963_ASA[i++] = (int16_t)((data - 128.0f) / 256.0f + 1.0f);
				//tranform the scale of different axis.
//				mpu9250_ptr->mag[0] = ((long)mpu9250_ptr->mag[0] * MPU9250_AK8963_ASA[0]) >> 8;
//				mpu9250_ptr->mag[1] = ((long)mpu9250_ptr->mag[1] * MPU9250_AK8963_ASA[1]) >> 8;
//				mpu9250_ptr->mag[2] = ((long)mpu9250_ptr->mag[2] * MPU9250_AK8963_ASA[2]) >> 8;
				
				self->output_rt_data[2].time_stamp++;
			}
		}
		else
		{
			//if mag sensor is absent, just get the data of imu.
			MPU9250_READ_REGS(mpu9250_ptr, MPU9250_ACCEL_XOUT_H, data,14);
//			mpu9250_ptr->output_data[2] = NULL;
			return_val = 3;
		}
		//save imu data to the buffer.
		acc[0] = (int16_t)((data[0] << 8) | data[1]);
		acc[1] = (int16_t)((data[2] << 8) | data[3]);
		acc[2] = (int16_t)((data[4] << 8) | data[5]);

		gyro[0] = (int16_t)((data[8] << 8) | data[9]);
		gyro[1] = (int16_t)((data[10] << 8) | data[11]);
		gyro[2] = (int16_t)((data[12] << 8) | data[13]);

		square_sum = acc[0] * acc[0] +
			acc[1] * acc[1] +
			acc[2] * acc[2];
		
		if(square_sum >= 4000000) {
			self->output_rt_data[0].time_stamp++;
			self->acc_error_cnt = 0;
			self->acc_error_flag = 0;
		}
		else if(self->acc_error_flag != 0){
			self->acc_error_cnt++;
			if(self->acc_error_cnt > 2000) {
				self->acc_error_flag = 1;
			}
		}

		SwapAixs(acc, self->acc, self->acc_axis_map, self->acc_axis_dir);

		SwapAixs(gyro, self->gyro, self->gyro_axis_map, self->gyro_axis_dir);


		//regist the pointer in the output data buffer list so that other module can access.
//		mpu9250_ptr->output_data[0] = self->acc_output_buffer;
//		mpu9250_ptr->output_data[1] = self->gyro_output_buffer;
		
		self->output_rt_data[0].time_stamp++;
		self->output_rt_data[1].time_stamp++;

	}
	else if(mpu9250_ptr->device_interface->periph_id == PERIPH_ID_I2C) {
		//if in i2c mode, mag and imu data are in different register, so must get them in different time.
		
		//first get the imu data
		return_val = MPU9250_READ_REGS(mpu9250_ptr, MPU9250_ACCEL_XOUT_H, data,14);
		if(return_val != 0)
			return -1;
		//save imu data to the buffer.
		acc[0] = (int16_t)((data[0] << 8) | data[1]);
		acc[1] = (int16_t)((data[2] << 8) | data[3]);
		acc[2] = (int16_t)((data[4] << 8) | data[5]);

		gyro[0] = (int16_t)((data[8] << 8) | data[9]);
		gyro[1] = (int16_t)((data[10] << 8) | data[11]);
		gyro[2] = (int16_t)((data[12] << 8) | data[13]);
		square_sum = mpu9250_ptr->acc[0] * mpu9250_ptr->acc[0] +
			mpu9250_ptr->acc[1] * mpu9250_ptr->acc[1] +
			mpu9250_ptr->acc[2] * mpu9250_ptr->acc[2];
		
		if(square_sum >= 4000000) {
			self->output_rt_data[0].time_stamp++;
			self->acc_error_cnt = 0;
			self->acc_error_flag = 0;
		}
		else if(self->acc_error_flag != 0){
			self->acc_error_cnt++;
			if(self->acc_error_cnt > 2000) {
				self->acc_error_flag = 1;
			}
		}
		

		SwapAixs(acc, self->acc, self->acc_axis_map, self->acc_axis_dir);

		SwapAixs(gyro, self->gyro, self->gyro_axis_map, self->gyro_axis_dir);


		//regist the pointer in the output data buffer list so that other module can access.
//		mpu9250_ptr->output_data[0] = self->acc_output_buffer;
//		mpu9250_ptr->output_data[1] = self->gyro_output_buffer;
		self->output_rt_data[0].time_stamp++;
		self->output_rt_data[1].time_stamp++;

		
		if(mpu9250_ptr->mode & MPU9250_MAG) {
			//if mag sensor is avilable, get mag data.
			return_val = MPU9250_MAG_READ_REGS(mpu9250_ptr, MPU9250_AK8963_ST1, &data[14], 8);
			if(return_val != 0)
				return 4;
			if (!(data[14] & MPU9250_AK8963_DATA_READY) || (data[14] & MPU9250_AK8963_DATA_OVERRUN)) {
				//if mag sensor data is not be updated, abandon the data.
//				mpu9250_ptr->output_data[2] = NULL;
				return_val = 1;
			}
			else if (data[21] & MPU9250_AK8963_OVERFLOW) {
				//if mag sensor data is abnormal, abandon the data.
//				mpu9250_ptr->output_data[2] = NULL;
				return_val = 2;
			}
			else {
				//regist the pointer in the output data buffer list so that other module can access.
//				mpu9250_ptr->output_data[2] = self->mag_output_buffer;
				//if the data is good, save them to the mag buffer.
				mag[0] = (int16_t)((data[16] << 8) | data[15]);
				mag[1] = (int16_t)((data[18] << 8) | data[17]);
				mag[2] = (int16_t)((data[20] << 8) | data[19]);
				
				SwapAixs(mag, self->mag, self->mag_axis_map, self->mag_axis_dir);
				
				//ned x,y,z //AK8963_ASA[i++] = (int16_t)((data - 128.0f) / 256.0f + 1.0f) ;
				//tranform the scale of different axis.
//				mpu9250_ptr->mag[0] = ((long)mpu9250_ptr->mag[0] * MPU9250_AK8963_ASA[0]) >> 8;
//				mpu9250_ptr->mag[1] = ((long)mpu9250_ptr->mag[1] * MPU9250_AK8963_ASA[1]) >> 8;
//				mpu9250_ptr->mag[2] = ((long)mpu9250_ptr->mag[2] * MPU9250_AK8963_ASA[2]) >> 8;
				
				self->output_rt_data[2].time_stamp++;
			}
		}
		else
		{
			//if mag sensor is absent, unregist the pointer.
			//mpu9250_ptr->output_data[2] = NULL;
			return_val = 3;
		}
	}
	

	
	return return_val;


}
int32_t Mpu9250GetRawGyro(Mpu9250 * self, int32_t * gyro)
{
	if (gyro == NULL)
		return -1;
	
	gyro[0] = self->gyro[0];
	gyro[1] = self->gyro[1];
	gyro[2] = self->gyro[2];
	return 0;
}

int32_t Mpu9250GetRawAccl(Mpu9250 * self, int32_t * acc)
{
	if (acc == NULL)
		return -1;
	
	acc[0] = self->acc[0];
	acc[1] = self->acc[1];
	acc[2] = self->acc[2];
	return 0;
}

int32_t Mpu9250GetRawMag(Mpu9250 * self, int32_t * mag)
{
	if (mag == NULL)
		return -1;
	
	mag[0] = self->mag[0];
	mag[1] = self->mag[1];
	mag[2] = self->mag[2];
	return 0;
}
int32_t Mpu9250GetRaw6Axis(Mpu9250 * self, int32_t * axis6)
{
	if (axis6 == NULL)
		return -1;
	
	axis6[0] = self->acc[0];
	axis6[1] = self->acc[1];
	axis6[2] = self->acc[2];
	axis6[3] = self->gyro[0];
	axis6[4] = self->gyro[1];
	axis6[5] = self->gyro[2];
	return 0;
}
int32_t Mpu9250GetRaw9Axis(Mpu9250 * self, int32_t * axis9)
{
	if (axis9 == NULL)
		return -1;
	
	axis9[0] = self->acc[0];
	axis9[1] = self->acc[1];
	axis9[2] = self->acc[2];
	axis9[3] = self->gyro[0];
	axis9[4] = self->gyro[1];
	axis9[5] = self->gyro[2];
	axis9[6] = self->mag[0];
	axis9[7] = self->mag[1];
	axis9[8] = self->mag[2];
	return 0;
}


RtData * Mpu9250PrepareOutputData9Axis(Mpu9250 * self)
{
//	int32_t acc[3] = {0};
//	int32_t gyro[3] = {0};
//	int32_t mag[3] = {0};
//	int32_t return_val = 0;
	
//	return_val = Mpu9250Load9Axis(self);
//	if(return_val != -1) {
//		SwapAixs(self->acc, acc, self->acc_axis_map, self->acc_axis_dir);
		self->acc_output_buffer[0] = self->acc[0] / MPU9250_1G_FACTOR;
		self->acc_output_buffer[1] = self->acc[1] / MPU9250_1G_FACTOR;
		self->acc_output_buffer[2] = self->acc[2] / MPU9250_1G_FACTOR;

//		SwapAixs(self->gyro, gyro, self->gyro_axis_map, self->gyro_axis_dir);
		self->gyro_output_buffer[0] = self->gyro[0] / MPU9250_1DPS_FACTOR;
		self->gyro_output_buffer[1] = self->gyro[1] / MPU9250_1DPS_FACTOR;
		self->gyro_output_buffer[2] = self->gyro[2] / MPU9250_1DPS_FACTOR;		
		
//		if(return_val == 0) {
//			SwapAixs(self->mag, mag, self->mag_axis_map, self->mag_axis_dir);
			self->mag_output_buffer[0] = (float)self->mag[0];
			self->mag_output_buffer[1] = (float)self->mag[1];
			self->mag_output_buffer[2] = (float)self->mag[2];
//		}
		return self->output_rt_data;
//	}
//	else {
//		return NULL;
//	}
}

RtData * Mpu9250PrepareOutputData6Axis(Mpu9250 * self)
{
//	int32_t acc[3] = {0};
//	int32_t gyro[3] = {0};
	
//	if (Mpu9250Load6Axis(self) == 0) {
//		SwapAixs(self->acc, acc, self->acc_axis_map, self->acc_axis_dir);
		self->acc_output_buffer[0] = self->acc[0] / MPU9250_1G_FACTOR;
		self->acc_output_buffer[1] = self->acc[1] / MPU9250_1G_FACTOR;
		self->acc_output_buffer[2] = self->acc[2] / MPU9250_1G_FACTOR;

//		SwapAixs(self->gyro, gyro, self->gyro_axis_map, self->gyro_axis_dir);
		self->gyro_output_buffer[0] = self->gyro[0] / MPU9250_1DPS_FACTOR;
		self->gyro_output_buffer[1] = self->gyro[1] / MPU9250_1DPS_FACTOR;
		self->gyro_output_buffer[2] = self->gyro[2] / MPU9250_1DPS_FACTOR;
		return self->output_rt_data;
//	}
//	else {
//		return NULL;
//	}
}

RtData * Mpu9250PrepareOutputGyro(Mpu9250 * self)
{
//	int32_t gyro[3] = {0};

//	if (Mpu9250LoadGyro(self) == 0) {
//		SwapAixs(self->gyro, gyro, self->gyro_axis_map, self->gyro_axis_dir);
		self->gyro_output_buffer[0] = self->gyro[0] / MPU9250_1DPS_FACTOR;
		self->gyro_output_buffer[1] = self->gyro[1] / MPU9250_1DPS_FACTOR;
		self->gyro_output_buffer[2] = self->gyro[2] / MPU9250_1DPS_FACTOR;		
		return &(self->output_rt_data[1]);
//	}
//	else {
//		return NULL;
//	}
	
}

RtData * Mpu9250PrepareOutputAcc(Mpu9250 * self)
{
//	int32_t acc[3] = {0};

//	if (Mpu9250LoadAcc(self) == 0) {		
//		SwapAixs(self->acc, acc, self->acc_axis_map, self->acc_axis_dir);
		self->acc_output_buffer[0] = self->acc[0] / MPU9250_1G_FACTOR;
		self->acc_output_buffer[1] = self->acc[1] / MPU9250_1G_FACTOR;
		self->acc_output_buffer[2] = self->acc[2] / MPU9250_1G_FACTOR;
		return &(self->output_rt_data[0]);
//	}
//	else {
//		return NULL;
//	}
}

RtData * Mpu9250PrepareOutputMag(Mpu9250 * self)
{
//	int32_t mag[3] = {0};
//	if (Mpu9250LoadMag(self) == 0) {
//		SwapAixs(self->mag, mag, self->mag_axis_map, self->mag_axis_dir);
		self->mag_output_buffer[0] = (float)self->mag[0];
		self->mag_output_buffer[1] = (float)self->mag[1];
		self->mag_output_buffer[2] = (float)self->mag[2];
		return &(self->output_rt_data[2]);
//	}
//	else {
//		return NULL;
//	}
}

int32_t Mpu9250CheckDevice(Mpu9250 * self)
{
	Mpu9250 * mpu9250_ptr = (Mpu9250 *)self;
	uint8_t whoami = 0;
	whoami = MPU9250_READ_REG(mpu9250_ptr, MPU9250_WHO_AM_I);
	
	if(whoami == 0x11)
		return 0;
	else {
		mpu9250_ptr->status = MPU9250_STATUS_OFFLINE;
		return -1;
	}
}












