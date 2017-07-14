#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <stdint.h>

typedef struct RtData_{
	void * data;
	int32_t time_stamp;
} RtData;

typedef struct RtDataSocket_ {
	RtData * rt_data;
	int32_t update_time;
} RtDataSocket;

#define RtDataPlugin(data, rt_socket) (								\
	(rt_socket).rt_data = &(data), 									\
	(rt_socket).update_time = (data).time_stamp						\
)	

#define NewRtDataCount(rt_socket) (												\
	(int32_t)((rt_socket).rt_data->time_stamp - (rt_socket).update_time)		\
)

#define GetRtData(rt_socket, type) (								\
	(NewRtDataCount(rt_socket) > 0)? 								\
	(((rt_socket).update_time = (rt_socket).rt_data->time_stamp),	\
	(type *)((rt_socket).rt_data->data)):							\
	(NULL) 														\
)

#define PeekRtData() (												\
	(type *)((rt_socket).rt_data->data)								\
)


#include "../Lang/Lang.h"
#include <math.h>
#include "FastMath.h"
#include "Filter.h"
#include "PidRegulator.h"
#include "Ringbuffer.h"
#include "Limit.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "SigGen.h"
#include "Adrc.h"
#include "Macros.h"
#include "DataVerify.h"
#include "AttitudeEstimator.h"
#include "ByteOrderTransform.h"
#include "TrackingDiff.h"




#ifdef C_PLATFORM_STM32
#include "FOC/Foc.h"
#include <arm_math.h>
#endif //C_PLATFORM_STM32






#endif //_ALGORITHM_H_
