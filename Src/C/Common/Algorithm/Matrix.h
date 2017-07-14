#ifndef MATRIX_H_
#define MATRIX_H_
#include "Algorithm.h"
#ifdef __cplusplus
extern "C" {
#endif
	
typedef struct Dim3DataQ31{
	int32_t x;
	int32_t y;
	int32_t z;
} Dim3DataQ31;

typedef struct Dim3DataF32{
	float x;
	float y;
	float z;
} Dim3DataF32;
	
	struct EulerAngleF32Class;
	typedef enum V3DSwapType_ {V3DSwapNone = 0, V3DSwapXZY, V3DSwapZYX, V3DSwapYXZ, V3DSwapYZX} V3DSwapType;
	
	void Vector3DToEuler(Dim3DataF32* self, struct EulerAngleF32Class* euler);
	void Vector3DCrossProduct(struct Dim3DataF32* a, struct Dim3DataF32* b, struct Dim3DataF32* result);
	float Vector3DDotProduct(Dim3DataF32* a, Dim3DataF32* b);
	void Vector3DAdd(Dim3DataF32* a, Dim3DataF32* b, Dim3DataF32* result);
	void Vector3DSub(Dim3DataF32* a, Dim3DataF32* b, Dim3DataF32* result);	
	void Vector3DScale(Dim3DataF32* a, Dim3DataF32* result, float scale);
	float Vector3DGetL2Norm(Dim3DataF32* a);
	float Vector3DNormalize(Dim3DataF32* a, float threshold_div1);
	void Vector3DCopy(Dim3DataF32* a, Dim3DataF32* b);
	void Vector3DLimit(Dim3DataF32* a, float limit);
	void Vector3dCoplanarity3(Dim3DataF32* a, Dim3DataF32* b, Dim3DataF32* c, Dim3DataF32* err);
	void Vector3DAxisSwap(Dim3DataF32* a, Dim3DataF32* result, V3DSwapType swap, Dim3DataF32* dir);
	float* Vector3DMaxItem(Dim3DataF32* a);
	void Vector3DAbs(Dim3DataF32* a, Dim3DataF32* result);
	
	int32_t Vector3DDotProductQ31(Dim3DataQ31 * a, Dim3DataQ31 * b);
	
#ifdef __cplusplus
}
#endif

#endif // MATRIX_H_
