#ifndef DATAVERIFY_H
#define DATAVERIFY_H

#include "Algorithm.h"

uint8_t CheckSum1ByteIn1ByteOut(uint8_t* data, int len);
uint16_t CheckSum1ByteIn2ByteOut(uint8_t* data, int len);
uint32_t CheckSum1ByteIn4ByteOut(uint8_t* data, int len);

uint16_t Crc16(uint8_t* pdata, uint16_t DataLen);


#define GenerateDataPackCrc(pack) \
	Crc16(pack, pack[PACKFORMAT_LEN_INDEX])


#endif //DATAVERIFY_H

