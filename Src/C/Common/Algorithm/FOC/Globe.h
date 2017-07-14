#ifndef GLOBLE_H
#define GLOBLE_H

#ifndef ABS
#define ABS(a)	(((a)<0)?(-(a)):(a))
#endif

#define SGN(X) ((X) >= 0 ? ((X) > 0 ? 1:0) : -1)

#define  MAX(x,y) (((x) > (y)) ? (x) : (y)) 
#define  MIN(x,y) (((x) < (y)) ? (x) : (y)) 

#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)

#endif
