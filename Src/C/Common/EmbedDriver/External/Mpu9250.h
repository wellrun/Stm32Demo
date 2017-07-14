#ifndef MPU9250_H_
#define MPU9250_H_

#include "EmbedDriverExternal.h"
#include "Algorithm/Algorithm.h"
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


//////////////////////////////////////////////////////////////////////////
//Register Map for Gyroscope and Accelerometer
#define MPU9250_SELF_TEST_X_GYRO        0x00
#define MPU9250_SELF_TEST_Y_GYRO        0x01
#define MPU9250_SELF_TEST_Z_GYRO        0x02

#define MPU9250_SELF_TEST_X_ACCEL       0x0D
#define MPU9250_SELF_TEST_Y_ACCEL       0x0E
#define MPU9250_SELF_TEST_Z_ACCEL       0x0F

#define MPU9250_XG_OFFSET_H             0x13
#define MPU9250_XG_OFFSET_L             0x14
#define MPU9250_YG_OFFSET_H             0x15
#define MPU9250_YG_OFFSET_L             0x16
#define MPU9250_ZG_OFFSET_H             0x17
#define MPU9250_ZG_OFFSET_L             0x18
#define MPU9250_SMPLRT_DIV              0x19
#define MPU9250_CONFIG                  0x1A
#define MPU9250_GYRO_CONFIG             0x1B
#define MPU9250_ACCEL_CONFIG            0x1C
#define MPU9250_ACCEL_CONFIG2           0x1D
#define MPU9250_LP_ACCEL_ODR            0x1E
#define MPU9250_WOM_THR                 0x1F

#define MPU9250_FIFO_EN                 0x23
#define MPU9250_I2C_MST_CTRL            0x24
#define MPU9250_I2C_SLV0_ADDR           0x25
#define MPU9250_I2C_SLV0_REG            0x26
#define MPU9250_I2C_SLV0_CTRL           0x27
#define MPU9250_I2C_SLV1_ADDR           0x28
#define MPU9250_I2C_SLV1_REG            0x29
#define MPU9250_I2C_SLV1_CTRL           0x2A
#define MPU9250_I2C_SLV2_ADDR           0x2B
#define MPU9250_I2C_SLV2_REG            0x2C
#define MPU9250_I2C_SLV2_CTRL           0x2D
#define MPU9250_I2C_SLV3_ADDR           0x2E
#define MPU9250_I2C_SLV3_REG            0x2F
#define MPU9250_I2C_SLV3_CTRL           0x30
#define MPU9250_I2C_SLV4_ADDR           0x31
#define MPU9250_I2C_SLV4_REG            0x32
#define MPU9250_I2C_SLV4_DO             0x33
#define MPU9250_I2C_SLV4_CTRL           0x34
#define MPU9250_I2C_SLV4_DI             0x35
#define MPU9250_I2C_MST_STATUS          0x36
#define MPU9250_INT_PIN_CFG             0x37
#define MPU9250_INT_ENABLE              0x38

#define MPU9250_INT_STATUS              0x3A
#define MPU9250_ACCEL_XOUT_H            0x3B
#define MPU9250_ACCEL_XOUT_L            0x3C
#define MPU9250_ACCEL_YOUT_H            0x3D
#define MPU9250_ACCEL_YOUT_L            0x3E
#define MPU9250_ACCEL_ZOUT_H            0x3F
#define MPU9250_ACCEL_ZOUT_L            0x40
#define MPU9250_TEMP_OUT_H              0x41
#define MPU9250_TEMP_OUT_L              0x42
#define MPU9250_GYRO_XOUT_H             0x43
#define MPU9250_GYRO_XOUT_L             0x44
#define MPU9250_GYRO_YOUT_H             0x45
#define MPU9250_GYRO_YOUT_L             0x46
#define MPU9250_GYRO_ZOUT_H             0x47
#define MPU9250_GYRO_ZOUT_L             0x48
#define MPU9250_EXT_SENS_DATA_00        0x49
#define MPU9250_EXT_SENS_DATA_01        0x4A
#define MPU9250_EXT_SENS_DATA_02        0x4B
#define MPU9250_EXT_SENS_DATA_03        0x4C
#define MPU9250_EXT_SENS_DATA_04        0x4D
#define MPU9250_EXT_SENS_DATA_05        0x4E
#define MPU9250_EXT_SENS_DATA_06        0x4F
#define MPU9250_EXT_SENS_DATA_07        0x50
#define MPU9250_EXT_SENS_DATA_08        0x51
#define MPU9250_EXT_SENS_DATA_09        0x52
#define MPU9250_EXT_SENS_DATA_10        0x53
#define MPU9250_EXT_SENS_DATA_11        0x54
#define MPU9250_EXT_SENS_DATA_12        0x55
#define MPU9250_EXT_SENS_DATA_13        0x56
#define MPU9250_EXT_SENS_DATA_14        0x57
#define MPU9250_EXT_SENS_DATA_15        0x58
#define MPU9250_EXT_SENS_DATA_16        0x59
#define MPU9250_EXT_SENS_DATA_17        0x5A
#define MPU9250_EXT_SENS_DATA_18        0x5B
#define MPU9250_EXT_SENS_DATA_19        0x5C
#define MPU9250_EXT_SENS_DATA_20        0x5D
#define MPU9250_EXT_SENS_DATA_21        0x5E
#define MPU9250_EXT_SENS_DATA_22        0x5F
#define MPU9250_EXT_SENS_DATA_23        0x60

#define MPU9250_I2C_SLV0_DO             0x63
#define MPU9250_I2C_SLV1_DO             0x64
#define MPU9250_I2C_SLV2_DO             0x65
#define MPU9250_I2C_SLV3_DO             0x66
#define MPU9250_I2C_MST_DELAY_CTRL      0x67
#define MPU9250_SIGNAL_PATH_RESET       0x68
#define MPU9250_MOT_DETECT_CTRL         0x69
#define MPU9250_USER_CTRL               0x6A
#define MPU9250_PWR_MGMT_1              0x6B
#define MPU9250_PWR_MGMT_2              0x6C

#define MPU9250_FIFO_COUNTH             0x72
#define MPU9250_FIFO_COUNTL             0x73
#define MPU9250_FIFO_R_W                0x74
#define MPU9250_WHO_AM_I                0x75
#define MPU9250_XA_OFFSET_H             0x77
#define MPU9250_XA_OFFSET_L             0x78

#define MPU9250_YA_OFFSET_H             0x7A
#define MPU9250_YA_OFFSET_L             0x7B

#define MPU9250_ZA_OFFSET_H             0x7D
#define MPU9250_ZA_OFFSET_L             0x7E
//
#define MPU9250_I2C_READ 0x80

//Magnetometer register maps
#define MPU9250_AK8963_WIA                 0x00
#define MPU9250_AK8963_INFO                0x01
#define MPU9250_AK8963_ST1                 0x02
#define MPU9250_AK8963_XOUT_L              0x03
#define MPU9250_AK8963_XOUT_H              0x04
#define MPU9250_AK8963_YOUT_L              0x05
#define MPU9250_AK8963_YOUT_H              0x06
#define MPU9250_AK8963_ZOUT_L              0x07
#define MPU9250_AK8963_ZOUT_H              0x08
#define MPU9250_AK8963_ST2                 0x09
#define MPU9250_AK8963_CNTL                0x0A
#define MPU9250_AK8963_CNTL2               0x0B
#define MPU9250_AK8963_RSV                 0x0B //DO NOT ACCESS <MPU9250_AK8963_CNTL2>
#define MPU9250_AK8963_ASTC                0x0C
#define MPU9250_AK8963_TS1                 0x0D //DO NOT ACCESS
#define MPU9250_AK8963_TS2                 0x0E //DO NOT ACCESS
#define MPU9250_AK8963_I2CDIS              0x0F
#define MPU9250_AK8963_ASAX                0x10
#define MPU9250_AK8963_ASAY                0x11
#define MPU9250_AK8963_ASAZ                0x12

#define MPU9250_AK8963_I2C_7BIT_ADDR (0x0C)
#define MPU9250_AK8963_I2C_8BIT_ADDR (MPU9250_AK8963_I2C_7BIT_ADDR<<1)
#define MPU9250_AK8963_POWER_DOWN 0x10
#define MPU9250_AK8963_FUSE_ROM_ACCESS 0x1F
#define MPU9250_AK8963_SINGLE_MEASUREMENT 0x11
#define MPU9250_AK8963_CONTINUOUS_MEASUREMENT 0x16 //MODE 2
#define MPU9250_AK8963_DATA_READY      (0x01)
#define MPU9250_AK8963_DATA_OVERRUN    (0x02)
#define MPU9250_AK8963_OVERFLOW        (0x80)
#define MPU9250_AK8963_DATA_ERROR      (0x40)
#define MPU9250_AK8963_CNTL2_SRST 0x01

//
#define MPU9250_I2C_SLV4_EN 0x80
#define MPU9250_I2C_SLV4_DONE 0x40
#define MPU9250_I2C_SLV4_NACK 0x10
//
#define MPU9250_I2C_IF_DIS (0x10)
#define MPU9250_I2C_MST_EN (0x20)
#define MPU9250_FIFO_RST (0x04)
#define MPU9250_FIFO_ENABLE (0x40)
//
#define MPU9250_RESET 0x80
#define MPU9250_CLOCK_MASK 0xF8
#define MPU9250_CLOCK_INTERNAL 0x00
#define MPU9250_CLOCK_PLL 0x01
#define MPU9250_CLOCK_PLLGYROZ 0x03
#define MPU9250_FS_SEL_MASK 0xE7
#define MPU9250_SLEEP_MASK 0x40
//
#define MPU9250_XYZ_GYRO 0xC7
#define MPU9250_XYZ_ACCEL 0xF8
//
#define MPU9250_RAW_RDY_EN (0x01)
#define MPU9250_RAW_DATA_RDY_INT (0x01)
#define MPU9250_FIFO_OVERFLOW (0x10)
//
#define MPU9250_INT_ANYRD_2CLEAR (0x10)
#define MPU9250_LATCH_INT_EN (0x20)
//
#define MPU9250_MAX_FIFO (1024)
#define MPU9250_FIFO_SIZE_1024  (0x40)
#define MPU9250_FIFO_SIZE_2048  (0x80)
#define MPU9250_FIFO_SIZE_4096  (0xC0)

#define MPU9250_TEMP_OUT (0x80)
#define MPU9250_GYRO_XOUT (0x40)
#define MPU9250_GYRO_YOUT (0x20)
#define MPU9250_GYRO_ZOUT (0x10)
#define MPU9250_ACCEL (0x08)

//
#define SMPLRT_DIV 0
#define MPU9250_I2C_7BIT_ADDR (0x68)
#define MPU9250_I2C_8BIT_ADDR (MPU9250_I2C_7BIT_ADDR<<1)


#define _PI  3.14159265358979
#define _2PI 6.28318530717958


//#define MPU9250_1G_FACTOR (float)16384
//#define MPU9250_1G_FACTOR (float)8192
#define MPU9250_1G_FACTOR (float)4096
//#define MPU9250_1G_FACTOR (float)2048

//#define MPU9250_1DPS_FACTOR (float)(131*57.3)
//#define MPU9250_1DPS_FACTOR (float)(65.5*57.3)
//#define MPU9250_1DPS_FACTOR (float)(32.8*57.3)
#define MPU9250_1DPS_FACTOR (float)(16.4*57.3)

#define MPU9250_1G_FACTOR_DIV1 (float)(1.0f/MPU9250_1G_FACTOR)
#define MPU9250_1DPS_FACTOR_DIV1 (float)(1.0f/MPU9250_1DPS_FACTOR)



//////////////////////////////////////////////////////////////////////////
//
enum MPU9250_GYRO_DLPF {
    MPU9250_GYRO_DLPF_250HZ = 0,
    MPU9250_GYRO_DLPF_184HZ,
    MPU9250_GYRO_DLPF_92HZ,
    MPU9250_GYRO_DLPF_41HZ,
    MPU9250_GYRO_DLPF_20HZ,
    MPU9250_GYRO_DLPF_10HZ,
    MPU9250_GYRO_DLPF_5HZ,
    MPU9250_GYRO_DLPF_3600HZ,
    NUM_GYRO_DLPF
};

enum MPU9250_GYRO_FSR {
    MPU9250_FSR_250DPS = 0,
    MPU9250_FSR_500DPS,
    MPU9250_FSR_1000DPS,
    MPU9250_FSR_2000DPS,
    MPU9250_NUM_GYRO_FSR
};

enum MPU9250_ACCEL_DLPF {
    MPU9250_ACCEL_DLPF_460HZ = 0,
    MPU9250_ACCEL_DLPF_184HZ,
    MPU9250_ACCEL_DLPF_92HZ,
    MPU9250_ACCEL_DLPF_41HZ,
    MPU9250_ACCEL_DLPF_20HZ,
    MPU9250_ACCEL_DLPF_10HZ,
    MPU9250_ACCEL_DLPF_5HZ,
    MPU9250_ACCEL_DLPF_460HZ2,
    MPU9250_NUM_ACCEL_DLPF
};

enum MPU9250_ACCEL_FSR {
    MPU9250_FSR_2G = 0,
    MPU9250_FSR_4G,
    MPU9250_FSR_8G,
    MPU9250_FSR_16G,
    MPU9250_NUM_ACCEL_FSR
};

enum MPU9250_CLK {
    MPU9250_CLK_INTERNAL = 0,
    MPU9250_CLK_PLL,
    MPU9250_NUM_CLK
};

#define AXIS_MAP_XYZ	(0)
#define AXIS_MAP_XZY	(1)
#define AXIS_MAP_ZYX	(2)
#define AXIS_MAP_ZXY	(3)
#define AXIS_MAP_YZX	(4)
#define AXIS_MAP_YXZ	(5)

#define AXIS_INV_X		(0x01)
#define AXIS_INV_Y		(0x02)
#define AXIS_INV_Z		(0x04)
#define AXIS_NO_INV 	(0x00)
#define AXIS_ALL_INV	(AXIS_INV_X | AXIS_INV_Y | AXIS_INV_Z)

#define MPU9250_STATUS_RESET	((uint8_t)0x00)
#define MPU9250_STATUS_RUN		((uint8_t)0x01)
#define MPU9250_STATUS_OFFLINE	((uint8_t)0x02)


#define MPU9250_ACCL_SHIFT (0)
#define MPU9250_ACCL ((1)<<MPU9250_ACCL_SHIFT)
#define MPU9250_GYRO_SHIFT (1)
#define MPU9250_GYRO ((1)<<MPU9250_GYRO_SHIFT)
#define MPU9250_MAG_SHIFT (2)
#define MPU9250_MAG	((1)<<MPU9250_MAG_SHIFT)
#define MPU9250_6AXIS (MPU9250_ACCL | MPU9250_GYRO)
#define MPU9250_9AXIS (MPU9250_ACCL | MPU9250_GYRO | MPU9250_MAG)



struct Mpu9250_;
typedef struct Mpu9250_ Mpu9250;

typedef uint8_t (*ImuReadReg)(Mpu9250 * self, uint16_t device_info,uint8_t addr);
typedef int32_t (*ImuWriteReg)(Mpu9250 * self, uint16_t device_info, uint8_t addr, uint8_t data);
typedef int32_t (*ImuReadRegs)(Mpu9250 * self, uint16_t device_info, uint8_t addr, uint8_t * data, int32_t size);
typedef int32_t (*ImuWriteRegs)(Mpu9250 * self, uint16_t device_info, uint8_t addr, const uint8_t * data, int32_t size);

//typedef int32_t (*ImuLoadGyro)(Mpu9250 * self);
//typedef int32_t (*ImuLoadAccl)(Mpu9250 * self);
//typedef int32_t (*ImuLoadMag)(Mpu9250 * self);
//typedef int32_t (*ImuLoad6Axis)(Mpu9250 * self);
//typedef int32_t (*ImuLoad9Axis)(Mpu9250 * self);
//typedef int32_t (*ImuGetGyro)(Mpu9250 * self, int32_t * gyro);
//typedef int32_t (*ImuGetAccl)(Mpu9250 * self, int32_t * accl);
//typedef int32_t (*ImuGetMag)(Mpu9250 * self, int32_t * mag);
//typedef int32_t (*ImuGet6Axis)(Mpu9250 * self, int32_t * axis6);
//typedef int32_t (*ImuGet9Axis)(Mpu9250 * self, int32_t * axis9);
//typedef float ** (*ImuPrepareOutputData)(Mpu9250 * self);



typedef struct Mpu9250Protocol_
{
	ImuReadReg read_reg;
	ImuWriteReg write_reg;
	ImuReadRegs read_regs;
	ImuWriteRegs write_regs;
	ImuReadReg mag_read_reg;
	ImuWriteReg mag_write_reg;
	ImuReadRegs mag_read_regs;
	ImuWriteRegs mag_write_regs;
} Mpu9250Protocol;




struct Mpu9250_
{
	int32_t gyro[3];
	int32_t acc[3];
	int32_t mag[3];
	float gyro_output_buffer[3];
	float acc_output_buffer[3];
	float mag_output_buffer[3];
	float * output_data[3];
	RtData output_rt_data[3];
	int32_t gyro_zero[3];
	RegisterDeviceInterface * device_interface;
	uint32_t mode;
	uint16_t device_info;
	uint16_t imu_device_info;
	uint16_t mag_device_info;
	uint8_t acc_axis_map;
	uint8_t acc_axis_dir;
	uint8_t gyro_axis_map;
	uint8_t gyro_axis_dir;
	uint8_t mag_axis_map;
	uint8_t mag_axis_dir;
	uint8_t status;
	uint16_t acc_error_cnt;
	uint8_t acc_error_flag;
	Mpu9250Protocol protocol;
};

#define MPU9250_WRITE_REG(mpu, addr, data) ((mpu)->protocol.write_reg((Mpu9250 *)(mpu), ((mpu)->imu_device_info), (addr), (data)))
#define MPU9250_READ_REG(mpu, addr) ((mpu)->protocol.read_reg((Mpu9250 *)(mpu),((mpu)->imu_device_info), (addr)))

#define MPU9250_WRITE_REGS(mpu, addr, data, size) ((mpu)->protocol.write_regs((Mpu9250 *)(mpu), ((mpu)->imu_device_info), (addr), (data), (size)))
#define MPU9250_READ_REGS(mpu, addr, data, size) ((mpu)->protocol.read_regs((Mpu9250 *)(mpu), ((mpu)->imu_device_info), (addr), (data), (size)))

#define MPU9250_MAG_WRITE_REG(mpu, addr, data) ((mpu)->protocol.mag_write_reg((Mpu9250 *)(mpu), ((mpu)->mag_device_info), (addr), (data)))
#define MPU9250_MAG_READ_REG(mpu, addr) ((mpu)->protocol.mag_read_reg((Mpu9250 *)(mpu),((mpu)->mag_device_info), (addr)))

#define MPU9250_MAG_WRITE_REGS(mpu, addr, data, size) ((mpu)->protocol.mag_write_regs((Mpu9250 *)(mpu), ((mpu)->mag_device_info), (addr), (data), (size)))
#define MPU9250_MAG_READ_REGS(mpu, addr, data, size) ((mpu)->protocol.mag_read_regs((Mpu9250 *)(mpu), ((mpu)->mag_device_info), (addr), (data), (size)))

uint8_t Mpu9250ReadReg(Mpu9250 * self, uint16_t device_info, uint8_t addr);
int32_t Mpu9250WriteReg(Mpu9250 * self, uint16_t device_info, uint8_t addr, uint8_t data);
int32_t Mpu9250ReadRegs(Mpu9250 * self, uint16_t device_info, uint8_t addr, uint8_t * data, int32_t size);
int32_t Mpu9250WriteRegs(Mpu9250 * self, uint16_t device_info, uint8_t addr, const uint8_t * data, int32_t size);
uint8_t Mpu9250ReadMagRegSPI(Mpu9250 * self, uint16_t device_info, uint8_t addr);
int32_t Mpu9250WriteMagRegSPI(Mpu9250 * self, uint16_t device_info, uint8_t addr, uint8_t data);
int32_t Mpu9250ReadMagRegsSPI(Mpu9250 * self, uint16_t device_info, uint8_t addr, uint8_t * data, int32_t size);
int32_t Mpu9250WriteMagRegsSPI(Mpu9250 * self, uint16_t device_info, uint8_t addr, const uint8_t * data, int32_t size);
int32_t Mpu9250Init(Mpu9250 * self);
int32_t Mpu9250LoadGyro(Mpu9250 * self);
int32_t Mpu9250LoadAcc(Mpu9250 * self);
int32_t Mpu9250LoadMag(Mpu9250 * self);
int32_t Mpu9250Load6Axis(Mpu9250 * self);
int32_t Mpu9250Load9Axis(Mpu9250 * self);
int32_t Mpu9250GetRawGyro(Mpu9250 * self, int32_t * gyro);
int32_t Mpu9250GetRawAccl(Mpu9250 * self, int32_t * accl);
int32_t Mpu9250GetRawMag(Mpu9250 * self, int32_t * mag);
int32_t Mpu9250GetRaw6Axis(Mpu9250 * self, int32_t * axis6);
int32_t Mpu9250GetRaw9Axis(Mpu9250 * self, int32_t * axis9);
RtData * Mpu9250PrepareOutputData9Axis(Mpu9250 * self);
RtData * Mpu9250PrepareOutputData6Axis(Mpu9250 * self);
RtData * Mpu9250PrepareOutputGyro(Mpu9250 * self);
RtData * Mpu9250PrepareOutputAcc(Mpu9250 * self);
RtData * Mpu9250PrepareOutputMag(Mpu9250 * self);
int32_t Mpu9250CheckDevice(Mpu9250 * self);

#define Mpu9250ReadMagRegI2C	(Mpu9250ReadReg)
#define Mpu9250WriteMagRegI2C	(Mpu9250WriteReg)
#define Mpu9250ReadMagRegsI2C	(Mpu9250ReadRegs)
#define Mpu9250WriteMagRegsI2C	(Mpu9250WriteRegs)




//#define LoadGyro(self) 		(self)->protocol.load_gyro((Mpu9250 *)(self))
//#define LoadAccl(self) 		(self)->protocol.load_accl((Mpu9250 *)(self))
//#define LoadMag(self) 		(self)->protocol.load_mag((Mpu9250 *)(self))
//#define Load6Axis(self)		(self)->protocol.load_6axis((Mpu9250 *)(self))
//#define Load9Axis(self)		(self)->protocol.load_9axis((Mpu9250 *)(self))


//#define GetGyro(self, data)	(self)->protocol.get_gyro((Mpu9250 *)(self), data)
//#define GetAccl(self, data)	(self)->protocol.get_accl((Mpu9250 *)(self), data)
//#define GetMag(self, data)	(self)->protocol.get_mag((Mpu9250 *)(self), data)
//#define Get6Axis(self, data)	(self)->protocol.get_6axis((Mpu9250 *)(self), data)
//#define Get9Axis(self, data)	(self)->protocol.get_9axis((Mpu9250 *)(self), data)

//#define LoadGyroX_()
//#define LoadGyroY_()
//#define LoadGyroZ_()

//#define LoadAccX_()
//#define LoadAccY_()
//#define LoadAccZ_()

//#define LoadMagX_()
//#define LoadMagY_()
//#define LoadMagZ_()




//#define PrepareOutoutData(self) (self)->protocol.prepare_output_data((self))

//uint32_t Mpu9250GetRawData(Mpu9250 * self, ImuDataRaw* raw);
//void Mpu9250ConvertData(ImuDataRaw* raw, ImuData* data);





#endif /*MPU9250*/
