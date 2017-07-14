#ifndef FAST_MATH_H
#define FAST_MATH_H

#include "Algorithm.h"

#define USE_FAST_SIN_COS
#define USE_FAST_TAN_COT
#define USE_FAST_ASIN_ACOS
#define USE_FAST_ATAN

#ifdef USE_FAST_SIN_COS
int32_t FastSinQ31(int32_t theta);

float FastSinF32(float theta);

int32_t FastCosQ31(int32_t theta);

float FastCosF32(float theta);

/////////////////////////////////////////////////////////

int32_t FastSinF32OutputQ31(float theta);

float FastSinQ31OutputF32(int32_t theta);

int32_t FastCosF32OutputQ31(float theta);

float FastCosQ31OutputF32(int32_t theta);

#endif /*USE_FAST_SIN_COS*/

#ifdef USE_FAST_TAN_COT

int32_t FastTanQ31(int32_t theta);

float FastTanF32(float theta);

int32_t FastCotQ31(int32_t theta);

float FastCotF32(float theta);

#endif /*USE_FAST_TAN_COT*/

#ifdef USE_FAST_ATAN

int32_t FastAtanQ31(int32_t k);

float FastAtanF32(float k);

int32_t FastAtan2Q31(int32_t sin, int32_t cos);

float FastAtan2F32(float sin, float cos);

///////////////////////////////////////////////////////

int32_t FastAtanF32OutputQ31(float k);

float FastAtanQ31OutputF32(int32_t k);

int32_t FastAtan2F32OutputQ31(float sin, float cos);

float FastAtan2Q31OutputF32(int32_t sin, int32_t cos);

#endif /*USE_FAST_ATAN*/


#ifdef USE_FAST_ASIN_ACOS

int32_t FastAsinQ31(int32_t k);

float FastAsinF32(float k);

int32_t FastAcosQ31(int32_t k);

float FastAcosF32(float k);

///////////////////////////////////////////////////////

int32_t FastAsinF32OutputQ31(float k);

float FastAsinQ31OutputF32(int32_t k);

int32_t FastAcosF32OutputQ31(float k);

float FastAcosQ31OutputF32(int32_t k);



#endif /*USE_FAST_ASIN_ACOS*/





int32_t FastIsqrtQ31(int32_t x);
float FastInvSqrtF32(float x);
float FastSqrtF32(float x);
uint16_t Byte_Contact(uint8_t h, uint8_t l);
void Fill_u16_To_u8(uint16_t x, uint8_t* h, uint8_t* l);




#define SQUARE(x) ((x)*(x))
#define SIGN(x) ((x) >= 0 ? ((x)>0?1:0) : -1)
#ifndef ABS
#define ABS(x) (((x) >= 0) ? (x) : -(x))
#endif
#endif
