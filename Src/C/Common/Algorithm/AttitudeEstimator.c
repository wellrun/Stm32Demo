#include "AttitudeEstimator.h"

#define COMP_FILTER_KP 0.5f
#define COMP_FILTER_KI 0.05f

#ifndef COMP_FILTER_KP 
#define COMP_FILTER_KP 0.0f
#endif
#ifndef COMP_FILTER_KI
#define COMP_FILTER_KI 0.0f
#endif

//#define IMU_SAMP_TIME_500HZ
#define IMU_SAMP_TIME_1000HZ
//1000Hz
#ifdef IMU_SAMP_TIME_1000HZ
	#define COMP_FILTER_SAMP_TIME_HALF 0.0005f
	#define COMP_FILTER_SAMP_TIME 0.001f
#endif

//500Hz
#ifdef IMU_SAMP_TIME_500HZ
	#define COMP_FILTER_SAMP_TIME_HALF 0.001f
	#define COMP_FILTER_SAMP_TIME 0.002f
#endif

#define AE_ACC_UNIT_SCALE	(0.000244140625f)
#define AE_GYRO_UNIT_SCALE	(0.001065264436f)
#define AE_MAG_UNIT_SCALE	(1.0f)

int32_t ImuDataSrcConfig(AttitudeEstimator * self, float ** imu_data)
{
	self->data_src = imu_data;
	self->quat.q0 = 1;
	return 0;
}

int32_t AttitudeEstimateConfig(AttitudeEstimator * self, RtData * imu_data)
{
	RtDataPlugin(imu_data[0], self->rt_data_src[0]);
	RtDataPlugin(imu_data[1], self->rt_data_src[1]);
	RtDataPlugin(imu_data[2], self->rt_data_src[2]);
	
	self->rt_euler.data = (void *)&self->euler;
	self->rt_euler.time_stamp = 0;
	
	self->rt_euler_q31.data = (void *)&self->euler_q31;
	self->rt_euler_q31.time_stamp = 0;
	
	self->acc_lpf[0] = NewLpf1stQ31(1927, 11);
	self->acc_lpf[1] = NewLpf1stQ31(1927, 11);
	self->acc_lpf[2] = NewLpf1stQ31(1927, 11);
	
	self->quat.q0 = 1;
	return 0;
}

int8_t gInitQuatCalFlag = 0;

int32_t QuatCompFilterAttitudeEstimate(AttitudeEstimator * self)
{
	char is_mag_available = 0;
	char is_acc_available = 0;
	Dim3DataF32 * acc 	= &self->acc;
	Dim3DataF32 * gyro 	= &self->gyro;
	Dim3DataF32 * mag 	= &self->mag;
	int * rt_acc = GetRtData(self->rt_data_src[0], int);
	int * rt_gyro = GetRtData(self->rt_data_src[1], int);
	int * rt_mag = GetRtData(self->rt_data_src[2], int);
	

	if ( rt_gyro != NULL ) {
		self->gyro.x = (float)rt_gyro[0];
		self->gyro.y = (float)rt_gyro[1];
		self->gyro.z = (float)rt_gyro[2];
		Vector3DScale(gyro, gyro, AE_GYRO_UNIT_SCALE);
		if( rt_acc != NULL ) {
			self->acc.x = (float)Lpf1stQ31Update(self->acc_lpf[0], rt_acc[0]);
			self->acc.y = (float)Lpf1stQ31Update(self->acc_lpf[1], rt_acc[1]);
			self->acc.z = (float)Lpf1stQ31Update(self->acc_lpf[2], rt_acc[2]);
			Vector3DScale(acc, acc, AE_ACC_UNIT_SCALE);
			is_acc_available = 1;
		}
		if ( rt_mag != NULL ) {
			self->mag.x = (float)rt_mag[0];
			self->mag.y = (float)rt_mag[1];
			self->mag.z = (float)rt_mag[2];
			Vector3DScale(mag, mag, AE_MAG_UNIT_SCALE);
			is_mag_available = 1;
		}
	}
	else {
		//THROW_MSG("CompFilter: Filter has not enough input data!\n\r");
		return -1;
	}
	
	Dim3DataF32 mag_orient_estim = {0.0};
	Dim3DataF32 grav_estim_err = {0.0};
	Dim3DataF32 gyro_i_term = {0.0};
	Dim3DataF32 gyro_p_term = {0.0};
	Dim3DataF32 gyro_corretion = {0.0};
	Dim3DataF32 mag_estim_err = {0.0};
	
	float gain_factor = 1.0;
	float acc_norm = 1.0;
	float norm_err = 0.0;
	if(is_acc_available) {
		acc_norm = Vector3DNormalize(acc, 2.0f);
		norm_err = acc_norm - 1.0;
		norm_err = ABS(norm_err);
		if(norm_err < 0.03) {
			gain_factor = 1.0;
		}
		else if(norm_err < 0.1){
			gain_factor = 1.0 - 14.2857 * (norm_err - 0.03);
		}
		else {
			gain_factor = 0.0;
		}
		if(gInitQuatCalFlag == 0) {
			QuatFromVector3D(&self->quat, acc);
			gInitQuatCalFlag = 1;
		}
		Vector3DCrossProduct(acc, &self->ground_orient_estim, &grav_estim_err);
		self->acc_absent_cnt = 0;
		self->acc_error_flag = 0;
	}
	else if(!self->acc_error_flag){
		self->acc_absent_cnt++;
		if(self->acc_absent_cnt > 2000) {
			self->acc_error_flag = 1;
		}
	}
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	if(is_mag_available) {
		//Get the real magnetic oreintaion(with white noise) and estimated magnetic 
		//oreintaion in the form of 3-vector in 3D orthogonal crod-system..
		Dim3DataF32 temp = {0.0};
		QuatToYAxisVector3D(&self->quat, &mag_orient_estim);
		Vector3DCrossProduct(&self->ground_orient_estim, mag, &temp);
		Vector3DCrossProduct(&temp, &mag_orient_estim, &mag_estim_err);
		Vector3DAdd(&grav_estim_err, &mag_estim_err, &grav_estim_err);
	}
	
	Vector3DCopy(gyro, &gyro_corretion);
	
	if(is_acc_available || is_mag_available) {
		//Calculate P term and I term in the filter's PI regulator.
		//Let I term be the gyroscope's bais estimation.
		Vector3DScale(&grav_estim_err, &gyro_p_term, gain_factor * COMP_FILTER_KP);
		Vector3DScale(&grav_estim_err, &gyro_i_term, gain_factor * COMP_FILTER_KI * COMP_FILTER_SAMP_TIME);
		Vector3DAdd(&self->gyro_bais, &gyro_i_term, &self->gyro_bais);
		Vector3DAdd(&gyro_corretion, &gyro_p_term, &gyro_corretion);
		Vector3DAdd(&gyro_corretion, &self->gyro_bais, &gyro_corretion);
	}
	//Update attitude by gyroscope data (1st-order Picard approximation)
	Vector3DScale(&gyro_corretion, &gyro_corretion, COMP_FILTER_SAMP_TIME_HALF);
	QuatDiffEquUpdate(&self->quat, &gyro_corretion);
	QuatNormalize(&self->quat, 1.0f);
	QuatToZAxisVector3D(&self->quat, &self->ground_orient_estim);
	return 0;
}

int32_t AttitudeEstimateOutputEulerF32(AttitudeEstimator * self)
{
	GravityVectorToEuler(&self->ground_orient_estim, &self->euler);
	self->rt_euler.time_stamp++;
}

int32_t AttitudeEstimateOutputEulerQ31(AttitudeEstimator * self)
{
	GravityVectorF32ToEulerQ31(&self->ground_orient_estim, &self->euler_q31);
	self->rt_euler_q31.time_stamp++;
}
	
