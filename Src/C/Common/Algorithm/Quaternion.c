#include "Algorithm.h"

/*Euler angle rotate order: Yaw -> Pitch -> Roll
**************************
*       z                *
*       |  (/) roll      *
*       |  /             *
*       | /\             *
*       |/__|________ y  *
*      / \  |pitch       *
*     /   \/             *
*    /yaw  \             *
*   x                    *
**************************
*/
void QuatToEuler(QuaternionF32Class* quat, EulerAngleF32Class* euler)
{
	float yaw, pitch, roll;
	float q0,q1,q2,q3;
	q0 = quat->q0;
	q1 = quat->q1;
	q2 = quat->q2;
	q3 = quat->q3;
	
	roll = (atan2(2.0*(q0*q1 + q2*q3),
				  1 - 2.0*(q1*q1 + q2*q2)));
 
	pitch = asin(2.0*(q0*q2 - q3*q1));
	
	yaw = atan2(2.0*(q0*q3 + q1*q2),
				 1 - 2.0*(q2*q2 + q3*q3));

	euler->pitch = pitch;
	euler->roll = roll;
	euler->yaw = yaw;
}

#ifndef PI
#define PI					3.14159265358979f
#endif
//this function is untested
void QuatToEulerExtendRange(QuaternionF32Class* quat, EulerAngleF32Class* euler)
{
	float yaw, pitch, roll;
	float q0 = quat->q0;
	float q1 = quat->q1;
	float q2 = quat->q2;
	float q3 = quat->q3;
	float q1q1 = q1 * q1;
	float q2q2 = q2 * q2;
	float q3q3 = q3 * q3;
	
	float dir_cos_matrix_11 = 1.0f - 2.0f *(q2q2 + q3q3);
	float dir_cos_matrix_12 = 2.0f * (q0*q3 + q1*q2);
	float dir_cos_matrix_13 = 2.0f * (q3*q1 - q0*q2);
	float dir_cos_matrix_23 = 2.0f * (q0*q1 + q2*q3);
	float dir_cos_matrix_33 = 1.0f - 2.0f * (q1q1 + q2q2);
	float pitch_sin_square = dir_cos_matrix_13 * dir_cos_matrix_13;
	float pitch_cos_square = dir_cos_matrix_23 * dir_cos_matrix_23 + dir_cos_matrix_33 * dir_cos_matrix_33;
	
	if(dir_cos_matrix_33 >= 0) {
		roll = FastAtan2F32(dir_cos_matrix_23, dir_cos_matrix_33);
	}
	else {
		roll = FastAtan2F32(-dir_cos_matrix_23, -dir_cos_matrix_33);		
	}
	pitch = FastAtan2F32(-dir_cos_matrix_13, dir_cos_matrix_33);
	yaw = FastAtan2F32(dir_cos_matrix_12, dir_cos_matrix_11);

	euler->pitch = pitch;
	euler->roll = roll;
	euler->yaw = yaw;
}

void GravityVectorToEuler(Dim3DataF32 * vector, EulerAngleF32Class * euler)
{
	float pitch, roll;


	float dir_cos_matrix_13 = vector->x;
	float dir_cos_matrix_23 = vector->y;
	float dir_cos_matrix_33 = vector->z;

	
	if(dir_cos_matrix_33 >= 0) {
		roll = FastAtan2F32(dir_cos_matrix_23, dir_cos_matrix_33);
	}
	else {
		roll = FastAtan2F32(-dir_cos_matrix_23, -dir_cos_matrix_33);		
	}
	pitch = FastAtan2F32(-dir_cos_matrix_13, dir_cos_matrix_33);

	euler->pitch = pitch;
	euler->roll = roll;
}

void GravityVectorF32ToEulerQ31(Dim3DataF32 * vector, EulerAngleQ31Class * euler)
{
	int32_t pitch, roll;


	float dir_cos_matrix_13 = vector->x;
	float dir_cos_matrix_23 = vector->y;
	float dir_cos_matrix_33 = vector->z;

	
	if(dir_cos_matrix_33 >= 0) {
		roll = FastAtan2F32OutputQ31(dir_cos_matrix_23, dir_cos_matrix_33);
	}
	else {
		roll = FastAtan2F32OutputQ31(-dir_cos_matrix_23, -dir_cos_matrix_33);		
	}
	pitch = FastAtan2F32OutputQ31(-dir_cos_matrix_13, dir_cos_matrix_33);

	euler->pitch = pitch;
	euler->roll = roll;
}

//void MagnetVectorToEuler(Dim3DataF32 * vector, EulerAngleF32Class * euler)
//{
//	
//}

void QuatFromEuler(QuaternionF32Class* quat, EulerAngleF32Class* euler)
{   
	float cr2 = cos(euler->roll*0.5);
    float cp2 = cos(euler->pitch*0.5);
    float cy2 = cos(euler->yaw*0.5);
    float sr2 = sin(euler->roll*0.5);
    float sp2 = sin(euler->pitch*0.5);
    float sy2 = sin(euler->yaw*0.5);

    quat->q0 = cr2*cp2*cy2 + sr2*sp2*sy2;
    quat->q1 = sr2*cp2*cy2 - cr2*sp2*sy2;
    quat->q2 = cr2*sp2*cy2 + sr2*cp2*sy2;
    quat->q3 = cr2*cp2*sy2 - sr2*sp2*cy2;
}

void QuatFromVector3D(QuaternionF32Class* quat, Dim3DataF32* vect)
{
	EulerAngleF32Class tmp_euler;
	Vector3DToEuler(vect, &tmp_euler);
	QuatFromEuler(quat, &tmp_euler);
}

/**
  * @breif  This method get the gravity orientaton from attitude quaternion.
  *
  * @detail The rotation matrix presented by euler angle, in the order of yaw-pitch-roll:
  * C_R^b = 
  * [ cos(r)*cos(y) + sin(p)*sin(r)*sin(y),   cos(y)*sin(p)*sin(r) - cos(r)*sin(y), -cos(p)*sin(r)]
  * [                        cos(p)*sin(y),                          cos(p)*cos(y),         sin(p)]
  * [ cos(y)*sin(r) - cos(r)*sin(p)*sin(y), - sin(r)*sin(y) - cos(r)*cos(y)*sin(p),  cos(p)*cos(r)]
  *
  *	
  * Gravity orientaton will not affect by the first rotation in yaw. We can project the gravity vector to
  * three axis of the sensor, the projection correspond to the last column of C_R^b. Corresponding to the 
  * rotation matrix presented by quaternion:(C_b^R = C_R^b')
  *
  * C_b^R(3,1) = -sin(r)cos(p) = 2.0f * (q1*q3 - q0*q2)
  * C_b^R(3,2) = sin(p) = 2.0f * (q0*q1 + q2*q3)
  * C_b^R(3,3) = cos(r)cos(p) = q3*q3 - q2*q2 - q1*q1 + q0*q0
  *
  */
void QuatToZAxisVector3D(QuaternionF32Class* quat, Dim3DataF32* vect)
{
	float q0 = quat->q0;
	float q1 = quat->q1;
	float q2 = quat->q2;
	float q3 = quat->q3;					 
	vect->x = 2.0f * (q1*q3 - q0*q2);		 
    vect->y = 2.0f * (q0*q1 + q2*q3);		 
    vect->z = q3*q3 - q2*q2 - q1*q1 + q0*q0; 
}

/**
  * @breif  This method get the earth magnetic orientaton from attitude quaternion.
  *
  * @detail The rotation matrix presented by euler angle, in the order of roll-pitch-yaw:
  * C_R^b = 
  * [ cos(r)*cos(y) - sin(p)*sin(r)*sin(y), -cos(p)*sin(y), - cos(y)*sin(r) - cos(r)*sin(p)*sin(y)]
  * [ cos(r)*sin(y) + cos(y)*sin(p)*sin(r),  cos(p)*cos(y),   cos(r)*cos(y)*sin(p) - sin(r)*sin(y)]
  * [                        cos(p)*sin(r),        -sin(p),                          cos(p)*cos(r)]
  *
  *	
  * Earth magnetic orientaton will not affect by the first rotation in roll. We can project the magnetic vector to
  * three axis of the sensor, the projection correspond to the second column of C_R^b. Corresponding to the 
  * rotation matrix presented by quaternion:(C_b^R = C_R^b')
  *
  * C_b^R(2,1) = -cos(p)*sin(y) = 2.0f * (q1*q2 + q0*q3)
  * C_b^R(2,2) = cos(p)*cos(y) = q2*q2 - q3*q3 + q0*q0 - q1*q1
  * C_b^R(2,3) = -sin(p) = 2.0f * (q2*q3 - q0*q1)
  *
  */
void QuatToYAxisVector3D(QuaternionF32Class* quat, Dim3DataF32* vect)
{
	float q0 = quat->q0;
	float q1 = quat->q1;
	float q2 = quat->q2;
	float q3 = quat->q3;					
	vect->x = 2.0f * (q1*q2 + q0*q3);		
    vect->y = q2*q2 - q3*q3 + q0*q0 - q1*q1;
    vect->z = 2.0f * (q2*q3 - q0*q1);		
}

void QuatAdd(QuaternionF32Class* a, QuaternionF32Class* b, QuaternionF32Class* result)
{
	result->q0 = a->q0 + b->q0;
	result->q1 = a->q1 + b->q1;
	result->q2 = a->q2 + b->q2;
	result->q3 = a->q3 + b->q3;
}

void QuatDiffEquUpdate(QuaternionF32Class* q, Dim3DataF32* inc)
{
	q->q0 += (-q->q1*inc->x - q->q2*inc->y - q->q3*inc->z);
    q->q1 += ( q->q0*inc->x + q->q2*inc->z - q->q3*inc->y);
    q->q2 += ( q->q0*inc->y - q->q1*inc->z + q->q3*inc->x);
    q->q3 += ( q->q0*inc->z + q->q1*inc->y - q->q2*inc->x);
}

//Q (x) P = M'(Q)P
void QuatMulti(QuaternionF32Class* q, QuaternionF32Class* p, QuaternionF32Class* result)
{
	float q0 = q->q0;
	float q1 = q->q1;
	float q2 = q->q2;
	float q3 = q->q3;
	float p0 = p->q0;
	float p1 = p->q1;
	float p2 = p->q2;
	float p3 = p->q3;
	
	result->q0 = p0*q0 - p1*q1 - p2*q2 - p3*q3;
	result->q1 = p0*q1 + p1*q0 + p2*q3 - p3*q2;
	result->q2 = p0*q2 - p1*q3 + p2*q0 + p3*q1;
	result->q3 = p0*q3 + p1*q2 - p2*q1 + p3*q0;
}

void QuatInv(QuaternionF32Class* q, QuaternionF32Class* result)
{
	result->q0 = q->q0;
	result->q1 = - q->q1;
	result->q2 = - q->q2;
	result->q3 = - q->q3;
}


void QuatScale(QuaternionF32Class* q, QuaternionF32Class* result, float scale)
{
	result->q0 = q->q0 * scale;
	result->q1 = q->q1 * scale;
	result->q2 = q->q2 * scale;
	result->q3 = q->q3 * scale;
}

float QuatGetL2Norm(QuaternionF32Class* quat)
{
	return 1.0f / FastInvSqrtF32(
					SQUARE(quat->q0) + 
					SQUARE(quat->q1) + 
					SQUARE(quat->q2) + 
					SQUARE(quat->q3));
}

void QuatNormalize(QuaternionF32Class* quat, float threshold_div1)
{
	float norm_div1 = FastInvSqrtF32(
					SQUARE(quat->q0) + 
					SQUARE(quat->q1) + 
					SQUARE(quat->q2) + 
					SQUARE(quat->q3));
	
	if(norm_div1 < threshold_div1) {
		quat->q0 *= norm_div1;
		quat->q1 *= norm_div1;
		quat->q2 *= norm_div1;
		quat->q3 *= norm_div1;
	}
}

void QuatReset(QuaternionF32Class* quat)
{
	quat->q0 = 1.0f;
	quat->q1 = 0.0f;
	quat->q2 = 0.0f;
	quat->q3 = 0.0f;
}

