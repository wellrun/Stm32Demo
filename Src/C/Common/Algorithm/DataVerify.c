#include "Algorithm.h"


const uint16_t wCRCTalbeAbs[] =
{
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400,
};

//==============================================================
//				CRC Functions
//==============================================================
uint16_t Crc16(uint8_t* pdata, uint16_t DataLen)
{
	uint16_t wCRC = 0xFFFF;
	uint16_t i;
	uint8_t  chChar;
	for (i = 0; i < DataLen; i++) {
		chChar = *pdata++;
		wCRC = wCRCTalbeAbs[(chChar ^ wCRC) & 15] ^ (wCRC >> 4);
		wCRC = wCRCTalbeAbs[((chChar >> 4) ^ wCRC) & 15] ^ (wCRC >> 4);
	}
	return wCRC;
}

#define CALC_DATA_VERIFY(veri_method, in, out) {out = veri_method(in,out, sizeof(in[0]), sizeof(out));}

//==============================================================
//				CheckSum Functions
//==============================================================
uint8_t CheckSum1ByteIn1ByteOut(uint8_t* data, int len)
{
	uint8_t checksum = 0;
	if (NULL == data) {
		THROW_MSG("---> Call function CheckSum1ByteIn1ByteOut() err! <---\n");
		return 0;
	}
	for (int i = 0; i < len; i++) {
		checksum += data[i];
	}
	checksum = ~checksum;

	return checksum;
}

uint16_t CheckSum1ByteIn2ByteOut(uint8_t* data, int len)
{
	uint16_t checksum = 0;
	if (NULL == data) {
		THROW_MSG("---> Call function CheckSum1ByteIn2ByteOut() err! <---\n");
		return 0;
	}

	for (int i = 0; i < len; i++) {
		checksum += data[i];
	}
	checksum = ~checksum;

	return checksum;
}

uint32_t CheckSum1ByteIn4ByteOut(uint8_t* data, int len)
{
	uint32_t checksum = 0;
	if (NULL == data) {
		THROW_MSG("---> Call function CheckSum1ByteIn4ByteOut() err! <---\n");
		return 0;
	}

	for (int i = 0; i < len; i++) {
		checksum += data[i];
	}
	checksum = ~checksum;

	return checksum;
}
