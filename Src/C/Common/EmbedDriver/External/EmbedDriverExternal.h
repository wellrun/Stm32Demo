#ifndef _EMBED_DRIVER_EXTERNAL_
#define _EMBED_DRIVER_EXTERNAL_


#include "../../Lang/Lang.h"

#define PERIPH_ID_SPI	(0)
#define PERIPH_ID_I2C	(1)
#define PERIPH_ID_UART	(2)

typedef struct IODeviceInterface_ IODeviceInterface;

typedef int32_t (*DeviceSend)(void * self, const uint8_t * data, int32_t size);
typedef int32_t (*DeviceRecv)(void * self, uint8_t * data, int32_t size);
typedef void (*DeviceReadSelect)(void * self, uint32_t info);
typedef void (*DeviceWriteSelect)(void * self, uint32_t info);
typedef void (*DeviceUnselect)(void * self, uint32_t info);





typedef struct RegisterDeviceInterface_ RegisterDeviceInterface; 

typedef int32_t (*DeviceMemoryWrite)(void * self, int32_t device_info, uint32_t mem_addr, const uint8_t * data, int32_t size);
typedef int32_t (*DeviceMemoryRead)(void * self, int32_t device_info, uint32_t mem_addr, uint8_t * data, int32_t size);
typedef int32_t (*DeviceReset)(void * self);
typedef int32_t (*DeviceStart)(void * self);
typedef int32_t (*DeviceStop)(void * self);
typedef int32_t (*ClassInit)(void * self);

typedef struct RegisterDeviceInterface_ 
{
	DeviceMemoryWrite device_memory_write;
	DeviceMemoryRead device_memory_read;
	DeviceReset device_reset;
	DeviceStart device_start;
	DeviceStop device_stop;
	uint32_t periph_id;
} RegisterDeviceInterface;

typedef struct IODeviceInterface_
{
	DeviceSend device_send;
	DeviceRecv device_recv;
	DeviceReset device_reset;
	DeviceStart device_start;
	DeviceStop device_stop;
	DeviceReadSelect device_read_select;
	DeviceWriteSelect device_write_select;
	DeviceUnselect device_unselect;
	uint32_t periph_id;
} IODeviceInterface;




#define DeviceInit(dev) (dev)->Init((dev))

#include "DataPersistenceDevice.h"

#include "Tle5012.h"
#include "TLC59283.h"
#include "Mpu9250.h"
#include "Ultrasonic.h"
#include "AT24C02.h"
#include "Infrared.h"
//#include "FDC2212.h"
#include "LIS3MDL.h"

#endif //_EMBED_DRIVER_EXTERNAL_
