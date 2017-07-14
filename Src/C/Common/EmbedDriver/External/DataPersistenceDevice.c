

#include "DataPersistenceDevice.h"

/**
 * @brief init data persisttence device, allocate memmory for the data buffer, 
			called by a marco
 * @param self: Object Pointer 
 * @param data_size: size of the whole data block
 * @return return 0 if success
 * @author xinyu.han
 */		
int32_t DataPersistenceInit_(DataPersistenceDevice * self, uint32_t data_size)
{
	//allocate memmory for data buffer
	self->data_buffer = malloc(data_size + sizeof(DataPersistenceHeadFormat));
	//set the data size (the size of the buffer)
	self->data_size = data_size + sizeof(DataPersistenceHeadFormat);
	
	return 0;
}

/**
 * @brief load data from data persisttence device, called by a marco
 * @param self: Object Pointer 
 * @param addr_offset: addr offset of the src data block
 * @param data_size: size of the data block
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t DataPersistenceLoadItemFromDevice_(
	DataPersistenceDevice * self,
	uint32_t addr_offset,
	uint32_t data_size )
{
	return self->read(
		self, 
		addr_offset, 
		(uint8_t *)self->data_buffer + addr_offset, 
		data_size
	);
}

/**
 * @brief load all data from data persisttence device, called by a marco
 * @param self: Object Pointer 
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t DataPersistenceLoadAll_(DataPersistenceDevice * self)
{
	return self->load_all(self);
}

/**
 * @brief save all data to data persisttence device, called by a marco
 * @param self: Object Pointer 
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t DataPersistenceSaveAll_(DataPersistenceDevice * self, uint32_t data_id)
{
	((DataPersistenceHeadFormat *)self->data_buffer)->write_flag = data_id;
	((DataPersistenceHeadFormat *)self->data_buffer)->length = self->data_size - sizeof(DataPersistenceHeadFormat);
	return self->save_all(self);
}


int32_t DataPersistenceClear_(DataPersistenceDevice * self) 
{
	memset(self->data_buffer, 0, self->data_size);
}




