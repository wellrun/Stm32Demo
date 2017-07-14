#ifndef AT24C02_H_
#define AT24C02_H_

#include "EmbedDriverExternal.h"
#include "DataPersistenceDevice.h"
typedef struct At24c02_ At24c02;

typedef int32_t (*DataPersistenDeviceRead)(void * self, uint32_t mem_addr, uint8_t * data, int32_t size);
typedef int32_t (*DataPersistenDeviceWrite)(void * self, uint32_t mem_addr, const uint8_t * data, int32_t size);
typedef int32_t (*DataPersistenDeviceSaveAll)(void * self);
typedef int32_t (*DataPersistenDeviceLoadAll)(void * self);


typedef struct At24c02_
{
	DataPersistenDeviceRead read;
	DataPersistenDeviceWrite write;
	DataPersistenDeviceSaveAll save_all;
	DataPersistenDeviceLoadAll load_all;
	RegisterDeviceInterface * device_interface;
	ClassInit init;
	uint32_t data_size;
	uint32_t base_addr;
	void * data_buffer;
	
	uint32_t mode;
	uint16_t device_info;
	
	uint32_t page_size;
	
} At24c02;



int32_t At24c02Init(void * self);


#endif /* AT24C02_H_ */

