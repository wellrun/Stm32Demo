#ifndef TRACKINGDIFF_H_
#define TRACKINGDIFF_H_
#ifdef __cplusplus
extern "C" {
#endif
	
typedef struct TrackingDiffClass_
{
	float error;
	float vel_predict;
	float pos_predict;
	
	float vel_factor;
	float pos_factor;
	float time_correction;
	
	float factor_multi;

} TrackingDiffClass;
extern TrackingDiffClass* pTouchTrackingDiff;

TrackingDiffClass* NewTrackingDiffClass(float vel_factor, float pos_factor, float time_factor);
void TrackingDiffUpdate(TrackingDiffClass* self, float target);

//extern 
#ifdef __cplusplus
}
#endif

#endif
