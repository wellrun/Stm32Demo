#include "Algorithm.h"

void Vector3DToEuler(Dim3DataF32* self, EulerAngleF32Class* euler)
{
	euler->pitch = -asin(self->x);
	euler->roll = atan2(self->y, self->z);
	euler->yaw = 0;//atan2(b.x, a.x);
}

//a x b
inline void Vector3DCrossProduct(Dim3DataF32* a, Dim3DataF32* b, Dim3DataF32* result)
{
	result->x = (a->y * b->z  -  a->z * b->y);
    result->y = (a->z * b->x  -  a->x * b->z);
    result->z = (a->x * b->y  -  a->y * b->x);
}

inline float Vector3DDotProduct(Dim3DataF32* a, Dim3DataF32* b)
{
	return (a->x * b->x + a->y * b->y + a->z * b->z);
}

inline int32_t Vector3DDotProductQ31(Dim3DataQ31 * a, Dim3DataQ31 * b)
{
	return (a->x * b->x + a->y * b->y + a->z * b->z);
}

inline void Vector3DAdd(Dim3DataF32* a, Dim3DataF32* b, Dim3DataF32* result)
{
	result->x = a->x + b->x; 
	result->y = a->y + b->y; 
	result->z = a->z + b->z; 
}

inline void Vector3DSub(Dim3DataF32* a, Dim3DataF32* b, Dim3DataF32* result)
{
	result->x = a->x * b->x; 
	result->y = a->y * b->y; 
	result->z = a->z * b->z; 
}

inline void Vector3DScale(Dim3DataF32* a, Dim3DataF32* result, float scale)
{
	result->x = a->x * scale;
	result->y = a->y * scale;
	result->z = a->z * scale;
}

inline float* Vector3DMaxItem(Dim3DataF32* a)
{
	Dim3DataF32 tmp;
	
	Vector3DAbs(a, &tmp);
	
	if(tmp.x >= tmp.y && tmp.x >= tmp.z)	return &(a->x);
	if(tmp.y >= tmp.x && tmp.y >= tmp.z)	return &(a->y);
	if(tmp.z >= tmp.x && tmp.z >= tmp.y)	return &(a->z);
	else return &(a->x);
}

inline void Vector3DAbs(Dim3DataF32* a, Dim3DataF32* result)
{
	result->x = fabs(a->x);
	result->y = fabs(a->y);
	result->z = fabs(a->z);
}

inline void Vector3DCopy(Dim3DataF32* a, Dim3DataF32* b)
{
	b->x = a->x;
	b->y = a->y;
	b->z = a->z;
}

inline void Vector3DLimit(Dim3DataF32* a, float limit)
{
	RangeLimit(a->x, -limit, limit);
    RangeLimit(a->y, -limit, limit);
    RangeLimit(a->z, -limit, limit);
}

inline float Vector3DGetL1Dist(Dim3DataF32* a, Dim3DataF32* b)
{
	return (fabs(a->x - b->x) + 
			fabs(a->y - b->y) + 
			fabs(a->z - b->z));
}

inline float Vector3DGetL2Dist(Dim3DataF32* a, Dim3DataF32* b)
{
	return 1.0f / FastInvSqrtF32(
		SQUARE(a->x - b->x) + 
		SQUARE(a->y - b->y) + 
		SQUARE(a->z - b->z));
}

inline float Vector3DGetL2Norm(Dim3DataF32* a)
{
	return 1.0f / FastInvSqrtF32(
		SQUARE(a->x) + 
		SQUARE(a->y) + 
		SQUARE(a->z));
}

inline float Vector3DNormalize(Dim3DataF32* a, float threshold_div1)
{
	float norm_div1 = FastInvSqrtF32(
		SQUARE(a->x) + 
		SQUARE(a->y) + 
		SQUARE(a->z));

	if(norm_div1 < threshold_div1 || threshold_div1 == 0.0f) {
		a->x = a->x * norm_div1;
		a->y = a->y * norm_div1;
		a->z = a->z * norm_div1;
	}
	return norm_div1;
}


void Vector3dCoplanarity3(Dim3DataF32* a, Dim3DataF32* b, Dim3DataF32* c, Dim3DataF32* err)
{
	Dim3DataF32 cp1, cp2;
	
	Vector3DCrossProduct(a, b, &cp1);
	Vector3DCrossProduct(c, b, &cp2);
	
	Vector3DCrossProduct(&cp1, &cp2, err);
}

void Vector3DAxisSwap(Dim3DataF32* a, Dim3DataF32* result, V3DSwapType swap, Dim3DataF32* dir)
{
	Dim3DataF32 swp_tmp;
	swp_tmp.x = a->x;
	swp_tmp.y = a->y;
	swp_tmp.z = a->z;
	
	switch(swap) {
		case(V3DSwapNone):{
			result->x = swp_tmp.x;
			result->y = swp_tmp.y;
			result->z = swp_tmp.z;
		}break;
		
		case(V3DSwapXZY):{
			result->x = swp_tmp.x;
			result->z = swp_tmp.y;
			result->y = swp_tmp.z;			
		}break;
		
		case(V3DSwapZYX):{
			result->z = swp_tmp.x;
			result->y = swp_tmp.y;
			result->x = swp_tmp.z;			
		}break;
		
		case(V3DSwapYXZ):{
			result->y = swp_tmp.x;
			result->x = swp_tmp.y;
			result->z = swp_tmp.z;			
		}break;
		
		case(V3DSwapYZX):{
			result->y = swp_tmp.x;
			result->z = swp_tmp.y;
			result->x = swp_tmp.z;			
		}break;
		
		default:
			break;
	}
	result->x *= dir->x;
	result->y *= dir->y;
	result->z *= dir->z;
}
