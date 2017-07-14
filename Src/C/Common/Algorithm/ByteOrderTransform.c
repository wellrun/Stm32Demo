//==================================================================================================================
//                                       Byte order transform 
//             converts the integer type from mcu byte order to network byte order.
//                                  The network use the big-endian.
//==================================================================================================================

#include "ByteOrderTransform.h"

//MCU use the little endian, so the high 8-bit is in the high address
//In the big-endian, the high 8 bit is in the low address
uint16_t ByteOrderTrans2Byte(uint16_t val)
{
    uint16_t temp = 0;
    
    temp = (val << 8) + (val >> 8);
    return temp;
}

uint32_t ByteOrderTrans4Byte(uint32_t val)
{
    uint32_t temp = 0;
    
    temp = (val << 24 & 0xFF000000) + (val << 8 & 0x00FF0000) + (val >> 8 & 0x0000FF00) + (val >> 24 & 0x000000FF);
    return temp;
}

