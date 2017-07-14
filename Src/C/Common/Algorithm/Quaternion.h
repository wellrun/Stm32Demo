#ifndef QUATERNION_H_
#define QUATERNION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Algorithm.h"
	
typedef struct QuaternionF32Class
{
	float q0;
	float q1;
	float q2;
	float q3;
} QuaternionF32Class;

typedef struct EulerAngleF32Class
{
	float pitch;
	float roll;
	float yaw;
} EulerAngleF32Class;

typedef struct
{
	int32_t pitch;
	int32_t roll;
	int32_t yaw;
} EulerAngleQ31Class;




void QuatToEuler(QuaternionF32Class* quad, EulerAngleF32Class* euler);
void QuatFromEuler(QuaternionF32Class* quat, EulerAngleF32Class* euler);
void QuatFromVector3D(QuaternionF32Class* quat, Dim3DataF32* vect);
void QuatMulti(QuaternionF32Class* q, QuaternionF32Class* p, QuaternionF32Class* result);
void QuatInv(QuaternionF32Class* q, QuaternionF32Class* result);
float QuatGetL2Norm(QuaternionF32Class* quat);
void QuatNormalize(QuaternionF32Class* quat, float threshold_div1);
void QuatToZAxisVector3D(QuaternionF32Class* quat, Dim3DataF32* vect);
void QuatAdd(QuaternionF32Class* a, QuaternionF32Class* b, QuaternionF32Class* result);
void QuatReset(QuaternionF32Class* quat);
void QuatDiffEquUpdate(QuaternionF32Class* q, Dim3DataF32* inc);
void QuatScale(QuaternionF32Class* q, QuaternionF32Class* result, float scale);
void QuatToYAxisVector3D(QuaternionF32Class* quat, Dim3DataF32* vect);
void QuatToEulerExtendRange(QuaternionF32Class* quat, EulerAngleF32Class* euler);
void GravityVectorToEuler(Dim3DataF32 * vector, EulerAngleF32Class * euler);
void GravityVectorF32ToEulerQ31(Dim3DataF32 * vector, EulerAngleQ31Class * euler);
#ifdef __cplusplus
}
#endif

#endif // QUATERNION_H_
