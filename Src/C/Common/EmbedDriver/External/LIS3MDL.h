#ifndef _LIS3MDL_H_
#define _LIS3MDL_H_
#include "EmbedDriverExternal.h"
#include "Algorithm/Algorithm.h"
#define LIS3MDL_ADDR_WHO_AM_I										((uint8_t)0x0F)		//r	
																		
#define LIS3MDL_ADDR_CTRL_REG1       								((uint8_t)0x20)		//r/w
#define LIS3MDL_ADDR_CTRL_REG2       								((uint8_t)0x21)		//r/w
#define LIS3MDL_ADDR_CTRL_REG3       								((uint8_t)0x22)		//r/w
#define LIS3MDL_ADDR_CTRL_REG4       								((uint8_t)0x23)		//r/w
#define LIS3MDL_ADDR_CTRL_REG5       								((uint8_t)0x24)		//r/w

#define LIS3MDL_ADDR_STATUS_REG      								((uint8_t)0x27)		//r
#define LIS3MDL_ADDR_OUT_X_L         								((uint8_t)0x28)		//r
#define LIS3MDL_ADDR_OUT_X_H         								((uint8_t)0x29)		//r
#define LIS3MDL_ADDR_OUT_Y_L         								((uint8_t)0x2A)		//r
#define LIS3MDL_ADDR_OUT_Y_H         								((uint8_t)0x2B)		//r
#define LIS3MDL_ADDR_OUT_Z_L         								((uint8_t)0x2C)		//r
#define LIS3MDL_ADDR_OUT_Z_H         								((uint8_t)0x2D)		//r
#define LIS3MDL_ADDR_TEMP_OUT_L      								((uint8_t)0x2E)		//r
#define LIS3MDL_ADDR_TEMP_OUT_H      								((uint8_t)0x2F)		//r
#define LIS3MDL_ADDR_INT_CFG         								((uint8_t)0x30)		//r/w
#define LIS3MDL_ADDR_INT_SRC         								((uint8_t)0x31)		//r
#define LIS3MDL_ADDR_INT_THS_L       								((uint8_t)0x32)		//r
#define LIS3MDL_ADDR_INT_THS_H       								((uint8_t)0x33)		//r


#define LIS3MDL_READ											((uint8_t)0x80)		//

#define LIS3MDL_ADDR_INC										((uint8_t)0x40)		//

#define LIS3MDL_CTRL_REG1_TEMP_EN_MASK							((uint8_t)0x80)		//
#define LIS3MDL_CTRL_REG1_OM_MASK								((uint8_t)0x60)		//
#define LIS3MDL_CTRL_REG1_DO_MASK								((uint8_t)0x1C)		//						
#define LIS3MDL_CTRL_REG1_FAST_ODR_MASK							((uint8_t)0x02)		//						
#define LIS3MDL_CTRL_REG1_ST_MASK								((uint8_t)0x01)		//	
#define LIS3MDL_CTRL_REG1_TEMP_EN_SHIFT							((uint8_t)0x07)		//
#define LIS3MDL_CTRL_REG1_OM_SHIFT		                        ((uint8_t)0x05)		//
#define LIS3MDL_CTRL_REG1_DO_SHIFT		                        ((uint8_t)0x02)		//
#define LIS3MDL_CTRL_REG1_FAST_ODR_SHIFT	                    ((uint8_t)0x01)		//
#define LIS3MDL_CTRL_REG1_ST_SHIFT		                        ((uint8_t)0x00)		//

#define LIS3MDL_CTRL_REG2_FS_MASK								((uint8_t)0x60)		//
#define LIS3MDL_CTRL_REG2_REBOOT_MASK							((uint8_t)0x08)		//
#define LIS3MDL_CTRL_REG2_SOFT_RST_MASK							((uint8_t)0x04)		//						
#define LIS3MDL_CTRL_REG2_FS_SHIFT                       		((uint8_t)0x05)		//
#define LIS3MDL_CTRL_REG2_REBOOT_SHIFI		                    ((uint8_t)0x03)		//
#define LIS3MDL_CTRL_REG2_SOFT_RST_SHIFT                        ((uint8_t)0x02)		//

#define LIS3MDL_CTRL_REG3_LP_MASK								((uint8_t)0x20)		//
#define LIS3MDL_CTRL_REG3_SIM_MASK								((uint8_t)0x04)		//
#define LIS3MDL_CTRL_REG3_MD_MASK								((uint8_t)0x03)		//						
#define LIS3MDL_CTRL_REG3_LP_SHIFT                       		((uint8_t)0x05)		//
#define LIS3MDL_CTRL_REG3_SIM_SHIFI		                    	((uint8_t)0x02)		//
#define LIS3MDL_CTRL_REG3_MD_SHIFT                        		((uint8_t)0x00)		//

#define LIS3MDL_CTRL_REG4_OMZ_MASK								((uint8_t)0x0C)		//
#define LIS3MDL_CTRL_REG4_BLE_MASK								((uint8_t)0x02)		//
#define LIS3MDL_CTRL_REG4_OMZ_SHIFT                       		((uint8_t)0x02)		//
#define LIS3MDL_CTRL_REG4_BLE_SHIFI		                    	((uint8_t)0x01)		//

#define LIS3MDL_CTRL_REG5_FAST_READ_MASK						((uint8_t)0x80)		//
#define LIS3MDL_CTRL_REG5_BDU_MASK								((uint8_t)0x40)		//					
#define LIS3MDL_CTRL_REG5_FAST_READ_SHIFT                 		((uint8_t)0x07)		//
#define LIS3MDL_CTRL_REG5_BDU_SHIFI		                    	((uint8_t)0x06)		//

#define LIS3MDL_STATUS_REG_ZYXOR_MASK							((uint8_t)0x80)		//
#define LIS3MDL_STATUS_REG_ZOR_MASK								((uint8_t)0x40)		//
#define LIS3MDL_STATUS_REG_YOR_MASK								((uint8_t)0x20)		//
#define LIS3MDL_STATUS_REG_XOR_MASK								((uint8_t)0x10)		//
#define LIS3MDL_STATUS_REG_ZYXDA_MASK							((uint8_t)0x08)		//
#define LIS3MDL_STATUS_REG_ZDA_MASK								((uint8_t)0x04)		//
#define LIS3MDL_STATUS_REG_YDA_MASK								((uint8_t)0x02)		//
#define LIS3MDL_STATUS_REG_XDA_MASK								((uint8_t)0x01)		//

#define LIS3MDL_STATUS_REG_ZYXOR_SHIFT							((uint8_t)0x07)		//
#define LIS3MDL_STATUS_REG_ZOR_SHIFT							((uint8_t)0x06)		//
#define LIS3MDL_STATUS_REG_YOR_SHIFT							((uint8_t)0x05)		//
#define LIS3MDL_STATUS_REG_XOR_SHIFT							((uint8_t)0x04)		//
#define LIS3MDL_STATUS_REG_ZYXDA_SHIFT							((uint8_t)0x03)		//
#define LIS3MDL_STATUS_REG_ZDA_SHIFT							((uint8_t)0x02)		//
#define LIS3MDL_STATUS_REG_YDA_SHIFT							((uint8_t)0x01)		//
#define LIS3MDL_STATUS_REG_XDA_SHIFT							((uint8_t)0x00)		//


#define LIS3MDL_REG(REG, FEILD, VALUE)							(((VALUE)<<( REG##_##FEILD##_SHIFT)) & ( REG##_##FEILD##_MASK ) )
#define LIS3MDL_CLEAR_REG(TARGET, REG, FEILD)					((TARGET) &= (~( REG##_##FEILD##_MASK)))
#define LIS3MDL_SET_REG_FEILD(TARGET, REG, FEILD, VALUE)				(LIS3MDL_CLEAR_REG(TARGET, REG, FEILD), TARGET |= (LIS3MDL_REG( REG, FEILD, VALUE)) )
#define LIS3MDL_GET_REG_FEILD(TARGET, REG, FEILD)						( (TARGET &= REG##_##FEILD##_MASK) >> (REG##_##FEILD##_SHIFT))

enum LIS3MDL_PERFORMANCE {
    LIS3MDL_LOW_POWER = 0,
    LIS3MDL_MEDIUM_PERFORMANCE,
    LIS3MDL_HIGH_PERFORMANCE,
    LIS3MDL_ULTRA_HIGH_PERFORMANCE
};

enum LIS3MDL_FSR {
    LIS3MDL_FSR_4GAUSS = 0,
    LIS3MDL_FSR_8GAUSS,
	LIS3MDL_FSR_12GAUSS,
    LIS3MDL_FSR_16GGAUSS
};

enum LIS3MDL_SYS_OPERATION_MODE {
	LIS3MDL_CONTINUOUS_CONVERSION_MODE = 0,
	LIS3MDL_SINGLE_CONVERSION_MODE,
	LIS3MDL_LOW_POWER_MODE
};

enum LIS3MDL_BOOL {
	LIS3MDL_DISABLE = 0,
	LIS3MDL_ENABLE
};


struct LIS3MDL_;
typedef struct LIS3MDL_ LIS3MDL;

typedef uint8_t (*CompassReadReg)(LIS3MDL * self, uint16_t device_info,uint8_t addr);
typedef int32_t (*CompassWriteReg)(LIS3MDL * self, uint16_t device_info, uint8_t addr, uint8_t data);
typedef int32_t (*CompassReadRegs)(LIS3MDL * self, uint16_t device_info, uint8_t addr, uint8_t * data, int32_t size);
typedef int32_t (*CompassWriteRegs)(LIS3MDL * self, uint16_t device_info, uint8_t addr, const uint8_t * data, int32_t size);


struct LIS3MDL_
{
	int32_t mag[3];
	float mag_output_buffer[3];
	float * output_data[1];
	RtData output_rt_data[1];
	RegisterDeviceInterface * device_interface;
	uint32_t mode;
	uint16_t device_info;
	uint8_t mag_axis_map;
	uint8_t mag_axis_dir;
};

uint8_t LIS3MDLReadReg  (LIS3MDL * self, uint16_t device_info, uint8_t addr);
int32_t LIS3MDLWriteReg (LIS3MDL * self, uint16_t device_info, uint8_t addr, uint8_t data);
int32_t LIS3MDLReadRegs (LIS3MDL * self, uint16_t device_info, uint8_t addr, uint8_t * data, int32_t size);
int32_t LIS3MDLWriteRegs(LIS3MDL * self, uint16_t device_info, uint8_t addr, const uint8_t * data, int32_t size);
int32_t LIS3MDLInit(LIS3MDL * self);
int32_t LIS3MDLLoadMag(LIS3MDL * self);
int32_t LIS3MDLGetRawMag(LIS3MDL * self, int32_t * mag);
RtData * LIS3MDLPrepareOutputData(LIS3MDL * self);

#define LIS3MDL_READ_REG(compass, addr)                	(LIS3MDLReadReg((LIS3MDL *)(compass), ((compass)->device_info), (addr)))
#define LIS3MDL_WRITE_REG(compass, addr, data)			(LIS3MDLWriteReg((LIS3MDL *)(compass), ((compass)->device_info), (addr), (data)))
#define LIS3MDL_READ_REGS(compass, addr, data, size)    (LIS3MDLReadRegs((LIS3MDL *)(compass), ((compass)->device_info), (addr), (data), (size)))
#define LIS3MDL_WRITE_REGS(compass, addr, data, size)	(LIS3MDLWriteRegs((LIS3MDL *)(compass), ((compass)->device_info), (addr), (data), (size)))


#endif /*_LIS3MDL_H_*/








