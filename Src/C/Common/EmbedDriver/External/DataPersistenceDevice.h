#ifndef DATA_PERSISTENCE_DEVICE_H_
#define DATA_PERSISTENCE_DEVICE_H_

#include "EmbedDriverExternal.h"

typedef struct DataPersistenceHeadFormat_ {
	uint32_t write_flag;
	uint32_t length;
} DataPersistenceHeadFormat;


typedef int32_t (*DataPersistenDeviceRead)(void * self, uint32_t mem_addr, uint8_t * data, int32_t size);
typedef int32_t (*DataPersistenDeviceWrite)(void * self, uint32_t mem_addr, const uint8_t * data, int32_t size);
typedef int32_t (*DataPersistenDeviceSaveAll)(void * self);
typedef int32_t (*DataPersistenDeviceLoadAll)(void * self);



typedef struct DataPersistenceDevice_
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
} DataPersistenceDevice;


/**
 * @brief marco to init a data persistence device, allocate memmory for data 
 *        buffer
 * @param self: Object Pointer 
 * @param data_format: a user defined data stuct (better in pack(1)) type name
 * @return return 0 if success
 * @author xinyu.han
 */
#define DataPersistenceInit(self, data_format) ( 								\
	(self)->data_size = sizeof(data_format) + sizeof(DataPersistenceHeadFormat), 	\
	(self)->data_buffer = malloc((self)->data_size), 								\
	(self)->init((self))																	\
)

/**
 * @brief marco to get a data item in the user defined data struct from the data
 *        buffer 
 * @param self: Object Pointer 
 * @param data_format: a user defined data stuct (better in pack(1)) type name
 * @param item: data item name in the user defined struct 
 * @return return data item, can be used as left value or rigth value
 * @author xinyu.han
 */
#define DataPersistenceGetItem(self, data_format, item) 									\
	(((data_format *)((uint32_t)(self)->data_buffer + sizeof(DataPersistenceHeadFormat)))->item)


/**
 * @brief marco to get a data item in the user defined data struct from the data
 *	      persistence device, the data will be reload from the device to buffer
 * @param self: Object Pointer 
 * @param data_format: a user defined data stuct (better in pack(1)) type name
 * @param item: data item name in the user defined struct 
 * @return return data item, can be used as left value or rigth value
 * @author xinyu.han
 */
#define DataPersistenceGetItemFromDevice(self, data_format, item) ( 						\
	DataPersistenceLoadItemFromDevice_(										\
		(self), 															\
		(uint32_t)&(GetItem((self), data_format, item)) - 					\
		(uint32_t)(self)->data_buffer, 										\
		(sizeof(GetItem((self), data_format, item)))						\
	)																		\
	, DataPersistenceGetItem((self))														\
)	

/**
 * @brief marco to load all data from data persistence device to the buffer
 * @param self: Object Pointer 
 * @return return 0 if success
 * @author xinyu.han
 */
#define DataPersistenceLoadAll(self) DataPersistenceLoadAll_((DataPersistenceDevice *)(self))
#define DataPersistenceSaveAll(self, data_id) DataPersistenceSaveAll_((DataPersistenceDevice *)(self), (data_id))
//#define DataPersistenceLoadAll(self) (self)->load_all((self)) //DataPersistenceLoadAll_((DataPersistenceDevice *)(self))
//#define DataPersistenceSaveAll(self) (self)->save_all((self)) //DataPersistenceSaveAll_((DataPersistenceDevice *)(self))
		
		
#define IsDataPersitenceHasData(self, data_id) 		(((DataPersistenceHeadFormat *)(self)->data_buffer)->write_flag == (data_id))
#define IsDataPersitenceHasNewData(self) 	(((DataPersistenceHeadFormat *)(self)->data_buffer)->length < ((self)->data_size - sizeof(DataPersistenceHeadFormat)))
#define DataPersistenceClear(self)			DataPersistenceClear_((DataPersistenceDevice *)(self))
int32_t DataPersistenceInit_(DataPersistenceDevice * self, uint32_t data_size);

int32_t DataPersistenceLoadItemFromDevice_(
	DataPersistenceDevice * self,
	uint32_t addr_offset,
	uint32_t data_size
);
		
int32_t DataPersistenceLoadAll_(DataPersistenceDevice * self);
int32_t DataPersistenceSaveAll_(DataPersistenceDevice * self, uint32_t data_id);

int32_t DataPersistenceClear_(DataPersistenceDevice * self) ;
#endif /*DATA_PERSISTENCE_DEVICE_H_*/

