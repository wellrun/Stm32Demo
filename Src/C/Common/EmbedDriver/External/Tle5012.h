#ifndef TLE5012_H_
#define TLE5012_H_

#include "EmbedDriverExternal.h"

/*-----------Read and Write-----------------*/
#define READ_FLAG	(0x8000)
#define WRITE_FLAG	(0x7FFF)

/*------------Register Map------------------*/
#define STAT 		0x00
#define ACSTAT		0x01
#define AVAL 		0x02
#define ASPD		0x03
#define AREV 		0x04
#define FSYNC		0x05
#define MOD_1		0x06
#define SIL			0x07
#define MOD_2 		0x08
#define MOD_3		0x09
#define OFFX 		0x0A
#define OFFY		0x0B
#define SYNCH 		0x0C
#define IFAB		0x0D
#define MOD_4 		0x0E
#define TCO_Y		0x0F
#define ADC_X 		0x10
#define ADC_Y		0x11
#define IIF_CNT		0x20

#define TLE5012_OD_MODE	0x01u
#define TLE5012_PP_MODE 0x00u


struct Tle5012_;
typedef struct Tle5012_ Tle5012;

typedef uint16_t (*EncReadReg)	(Tle5012 * self, uint8_t device_info, uint8_t addr);
typedef int32_t (*EncWriteReg)	(Tle5012 * self, uint8_t device_info, uint8_t addr, uint16_t data);
typedef int32_t (*EncReadRegs)	(Tle5012 * self, uint8_t device_info, uint8_t addr, uint16_t * data, int32_t size);
typedef int32_t (*EncWriteRegs)	(Tle5012 * self, uint8_t device_info, uint8_t addr, const uint16_t * data, int32_t size);
typedef int32_t (*EncLoadAngle)	(Tle5012 * self);
typedef int32_t (*EncLoadAngleSpeed)(Tle5012 * self);
typedef int32_t (*EncLoadAngleAndSpeed)	(Tle5012 * self);


typedef struct Tle5012Protocol_
{
	EncReadReg 		read_reg;
	EncWriteReg 	write_reg;
	EncReadRegs		read_regs;
	EncWriteRegs	write_regs;
	EncLoadAngle	load_angle;
	EncLoadAngleSpeed	load_angle_speed;
	EncLoadAngleAndSpeed	load_angle_and_speed;
} Tle5012Protocol;

struct Tle5012_
{
	int32_t elec_angle;
	int32_t mech_angle;
	uint8_t pole_pair;
	bool calib_run;
	int32_t angle_value;
	int32_t angle_speed;
	RegisterDeviceInterface * device_interface;
	ClassInit Init;
	uint32_t device_info;
	uint8_t mode;
};

int32_t Tle5012Init(Tle5012 * self);

uint16_t Tle5012ReadReg(Tle5012 * self, uint8_t device_info, uint8_t addr);


int32_t Tle5012WriteReg(Tle5012 * self, uint8_t device_info, uint8_t addr, uint16_t data);


int32_t Tle5012ReadRegs(Tle5012 * self, uint8_t device_info, uint8_t addr, uint16_t * data, int32_t size);


int32_t Tle5012WriteRegs(Tle5012 * self, uint8_t device_info, uint8_t addr, const uint16_t * data, int32_t size);
	
int32_t Tle5012LoadAngleAndSpeed(Tle5012 * self);

int32_t Tle5012LoadAngle(Tle5012 * self);

int32_t Tle5012LoadAngleSpeed(Tle5012 * self);

int32_t Tle5012ODReadAngleAndSpeed(Tle5012 * self, uint16_t *encoder_angle);

//#define LoadAngle(enc) (enc)->protocol.load_angle((enc))
//#define LoadAngleAndSpeed(enc) (enc)->protocol.load_angle_and_speed((enc))
//#define LoadAngleAngleSpeed(enc) (enc)->protocol.load_angle_speed((enc))


//#define GetAngle(enc) ((enc)->angle_value)
//#define GetAngleSpeed(enc) ((enc)->angle_speed)



#endif /*TLE5012_H_*/

