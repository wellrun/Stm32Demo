#ifndef FDC2212_H
#define FDC2212_H

//#include "../Common/EmbedDriver/F10xStdPeriph/SoftwareI2c.h"
#include "EmbedDriverExternal.h"
#include "../../../Common/EmbedDriver/F10xStdPeriph/F10xStdPeriph.h"
#include "../../../Common/Algorithm/TrackingDiff.h"
#include "../../../Common/Algorithm/ByteOrderTransform.h"
#include "../../../Common/Debugger/DataScope_DP.h"

//The I2C Address depend on IC ADDR pin: when ADDR=L, I2C address = 0x2A, when ADDR=H, I2C address = 0x2B
#define FDC_I2C_ADDR           (0x2B<<1)


#define FDC2x_DATA_CH0            0x00    // [R]
#define FDC2x_DATA_LSB_CH0        0x01    // [R]
#define FDC2x_DATA_CH1            0x02    // [R]
#define FDC2x_DATA_LSB_CH1        0x03    // [R]
#define FDC2x_DATA_CH2            0x04    // [R]
#define FDC2x_DATA_LSB_CH2        0x05    // [R]
#define FDC2x_DATA_CH3            0x06    // [R]
#define FDC2x_DATA_LSB_CH3        0x07    // [R]
#define FDC2x_RCOUNT_CH0          0x08    // [R/W]
#define FDC2x_RCOUNT_CH1          0x09    // [R/W]
#define FDC2x_RCOUNT_CH2          0x0A    // [R/W]
#define FDC2x_RCOUNT_CH3          0x0B    // [R/W]
#define FDC2x_OFFSET_CH0          0x0C    // [R/W]
#define FDC2x_OFFSET_CH1          0x0D    // [R/W]
#define FDC2x_OFFSET_CH2          0x0E    // [R/W]
#define FDC2x_OFFSET_CH3          0x0F    // [R/W]
#define FDC2x_SETTLECOUNT_CH0     0x10    // [R/W]
#define FDC2x_SETTLECOUNT_CH1     0x11    // [R/W]
#define FDC2x_SETTLECOUNT_CH2     0x12    // [R/W]
#define FDC2x_SETTLECOUNT_CH3     0x13    // [R/W]
#define FDC2x_CLOCK_DIVIDERS_CH0  0x14    // [R/W]  [13:12] frequency select, 
#define FDC2x_CLOCK_DIVIDERS_CH1  0x15    // [R/W]
#define FDC2x_CLOCK_DIVIDERS_CH2  0x16    // [R/W]
#define FDC2x_CLOCK_DIVIDERS_CH3  0x17    // [R/W]
#define FDC2x_STATUS              0x18    // [R] 
#define FDC2x_ERROR_CONFIG        0x19    // [R/W]
#define FDC2x_CONFIG              0x1A    // [R/W]  [1514] ACTIVE_CHAN [13] SLEEP_MODE_EN, [9] REF_CLK_SRC, default value is 0x2801
#define FDC2x_MUX_CONFIG          0x1B    // [R/W]  [2:0] Input deglitch filter, selection of the lowest setting that exceeds the sensor oscillation frequency
#define FDC2x_RESET_DEV           0x1C    // [R/W]  [15] RESET_DEV, [10:9] OUTPUT_GAIN
#define FDC2x_DRIVE_CURRENT_CH0   0x1E    // [R/W]
#define FDC2x_DRIVE_CURRENT_CH1   0x1F    // [R/W]
#define FDC2x_DRIVE_CURRENT_CH2   0x20    // [R/W]
#define FDC2x_DRIVE_CURRENT_CH3   0x21    // [R/W]
#define FDC2x_MANUFACTURER_ID     0x7E    // [R]
#define FDC2x_DEVICE_ID           0x7F    // [R]


#define SLEEP_MODE_EN_MASK        0x2000
#define DATA_READY_FLAG_MASK      0x0040
#define DATA_BIT_MASK             0x0FFF

//===========================================
//#define CLOCK_DIVIDERS_PARAM      0x1001     //0x2001     //
//#define CURRENT_DRIVER_PARAM      0X7C00     // 
//#define SETTLECOUNT_PARAM         0x000A     //SET CHx_SETTLECO UNT  >3 , the CHx_SETTLECOUNT > Vpk * fREFx * C * pi *pi / (32 * IDRIVEX)
//                                             //Settle Time (tS0)= (CH0_SETTLECOUNT*16)/fREF0  and the activation time is 4us 
//#define RCOUNT_PARAM              0x2089     //SET CHx_RCOUNT  >8 ; Conversion Time (tC0) = (CH0_RCOUNT*16 + 4)/fREF0
//#define MUX_CONFIG_PARAM          0xC20D     //Enable ch0 auto scan, deglitch Filter 10M
//#define CONFIG_PARAM              0x1C01      //0x1401     //Enable cho conversion, 
#define CLOCK_DIVIDERS_PARAM      0x2001     //0x2001     //
#define CURRENT_DRIVER_PARAM      0X1440     // 
#define SETTLECOUNT_PARAM         0x0400     //SET CHx_SETTLECO UNT  >3 , the CHx_SETTLECOUNT > Vpk * fREFx * C * pi *pi / (32 * IDRIVEX)
                                             //Settle Time (tS0)= (CH0_SETTLECOUNT*16)/fREF0  and the activation time is 4us 
#define RCOUNT_PARAM              0xFFFF     //SET CHx_RCOUNT  >8 ; Conversion Time (tC0) = (CH0_RCOUNT*16 + 4)/fREF0
#define MUX_CONFIG_PARAM          0x820D     //Enable ch0 auto scan, deglitch Filter 10M
#define CONFIG_PARAM              0x1C01      //0x1401     //Enable cho conversion,   




//=============================================
#define DATA_NUMBER    5
typedef struct DataAverage_ {	
	uint8_t  read_cnt;
	uint8_t  read_limit;
	uint32_t averge;
	uint32_t data[DATA_NUMBER];
}DataAverage;


typedef struct Fdc2212Class_ {
	float fdc_data;
	float high_threshold;
	float low_threshold;
	
	uint16_t threshold_limit_h;
	uint16_t threshold_limit_l;
	
	uint8_t rising_edge;
	uint8_t falling_edge;
	
	uint8_t  rising_flag;
	uint8_t  falling_flag;
	uint16_t data_cnt;
	float data_buff[100];
	
} Fdc2212Class;


//============================================================================================
//     							Public function
//============================================================================================
char GetFdc2212AvergeValue(SoftwareI2cClass *self);
void Fdc2212DataProc(Fdc2212Class *self, DataAverage *avg, TrackingDiffClass *diff);



int Fdc2212Config(SoftwareI2cClass *self, uint8_t dev_id);


int FdcReadDataReadyFlag(SoftwareI2cClass *self, uint8_t dev_id);
int FdcReadCh0DataRegister(SoftwareI2cClass *self, uint8_t dev_id, uint32_t *data);


void Fdc2212SetSleepMode(SoftwareI2cClass *self);
void Fdc2212ExitSleepMode(SoftwareI2cClass *self);


void TestReadConfigRegister(SoftwareI2cClass *self);

void Fdc2212DataProcessing(Fdc2212Class *self,uint32_t *data);
	
void Fdc2212EdgeTest(Fdc2212Class *self);

  
#endif
