#ifndef ATTITUDE_ESTIMATOR_H
#define ATTITUDE_ESTIMATOR_H

#include "Algorithm.h"

#ifdef __cplusplus
extern "C" {
#endif
	


typedef struct AttitudeEstimator_ AttitudeEstimator;

typedef int32_t (*ImuDataSrcConfigFunc)(AttitudeEstimator * self, int32_t ** imu_data);
typedef int32_t (*AttitudeEstimateFunc)(AttitudeEstimator * self);



typedef struct AttitudeEstim_
{
	QuaternionF32Class quat;
	EulerAngleF32Class euler; 
} AttitudeEstim;

typedef struct AttitudeEstimator_ 
{
	//Common data and function
	RtDataSocket  rt_data_src[3];
	float ** data_src;
	Dim3DataF32 * imu_data[3];
	Dim3DataF32 acc;
	Dim3DataF32 gyro;
	Dim3DataF32 mag;
	Dim3DataF32 ground_orient_estim;
	EulerAngleF32Class euler;
	EulerAngleQ31Class euler_q31;
	ImuDataSrcConfigFunc imu_data_src_config;
	Lpf1stQ31 * acc_lpf[3];
	RtData rt_euler;
	RtData rt_euler_q31;
	uint16_t acc_absent_cnt;
	int8_t acc_error_flag;
	
	//Algorithm specifical data and function
	QuaternionF32Class quat;
	AttitudeEstimateFunc attitude_estimate;
	Dim3DataF32 gyro_bais;
} AttitudeEstimator;


int32_t ImuDataSrcConfig(AttitudeEstimator * self, float ** imu_data);
int32_t AttitudeEstimateConfig(AttitudeEstimator * self, RtData * imu_data);
int32_t QuatCompFilterAttitudeEstimate(AttitudeEstimator * self);
int32_t AttitudeEstimateOutputEulerF32(AttitudeEstimator * self);
int32_t AttitudeEstimateOutputEulerQ31(AttitudeEstimator * self);

#ifdef __cplusplus
}
#endif
	

#endif // ATTITUDE_ESTIMATOR_H

