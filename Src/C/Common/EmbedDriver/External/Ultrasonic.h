#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#include "EmbedDriverExternal.h"
#include "../F10xStdPeriph/F10xStdPeriph.h"

typedef struct UsartClass_ UsartClass;

uint16_t GetUltraValue(void);
void US3_Receive_Data(void);
void US3_Send_Data(void);

struct UltrSonic_;
typedef struct UltrSonic_ UltrSonic;

typedef int32_t (*UltrSonicGetDistanceFunc)(UltrSonic * self, int32_t * distance);
typedef int32_t (*UltrSonicLoadDistanceFunc)(UltrSonic * self);


typedef struct UltrSonicProtocol_ 
{
	UltrSonicLoadDistanceFunc load_distance;
	UltrSonicGetDistanceFunc get_distance;
} UltrSonicProtocol;

typedef struct UltrSonic_ {
	int32_t distance;
	uint8_t already_arrive_byte;
	uint8_t data_buf[4];
	UltrSonicProtocol protocol;
	UsartClass* usart;
} UltrSonic;

void UltrSonicDataAnalysis(UltrSonic * self, uint8_t * data, int32_t length);
int32_t UltrSonicGetDistance(UltrSonic * self, int32_t * distance);
int32_t UltrSonicLoadDistance(UltrSonic * self);
void UltrSonicSerialRegister(UltrSonic * self, UsartClass* usart_);
#endif
