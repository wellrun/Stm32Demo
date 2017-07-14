#include "AT24C02.h"

extern void CpuDelayUs(unsigned long t);

static int32_t At24c02PageWrite(At24c02 * self, uint32_t mem_addr, const uint8_t * data, int32_t size);
static int32_t At24c02PageRead(At24c02 * self, uint32_t mem_addr, uint8_t * data, int32_t size);



/**
 * @brief page read function,data can be read continuously  
 * @param self: Object Pointer 
 * @param mem_addr: addr of the src data block
 * @param data: data buffer used to save the data read out
 * @param size: size of the data block
 * @return return 0 if success
 * @author xinyu.han
 */
static int32_t At24c02PageRead(At24c02 * self, uint32_t mem_addr, uint8_t * data, int32_t size)
{
	At24c02 * self_ = (At24c02 *)self;
	return self_->device_interface->device_memory_read( 
		self_->device_interface, 
		self_->device_info, 
		self_->base_addr + mem_addr, 
		data,
		size 
	);	
}


/**
 * @brief page write function,data can be write continuously  
 * @param self: Object Pointer 
 * @param mem_addr: addr of the src data block
 * @param data: data buffer used to save the data write in
 * @param size: size of the data block
 * @return return 0 if success
 * @author xinyu.han
 */
static int32_t At24c02PageWrite(At24c02 * self, uint32_t mem_addr, const uint8_t * data, int32_t size)
{
	At24c02 * self_ = (At24c02 *)self;
	return self_->device_interface->device_memory_write( 	
		self_->device_interface, 							
		self_->device_info, 								
		self_->base_addr + mem_addr, 						
		data, 												
		size 												
	);
	
}

/**
 * @brief random read function, random reading can be performance  
 * @param self: Object Pointer 
 * @param mem_addr: addr of the src data block
 * @param data: data buffer used to save the data read out
 * @param size: size of the data block
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t At24c02Read(void * self, uint32_t mem_addr, uint8_t * data, int32_t size)
{
	At24c02 * self_ = (At24c02 *) self;
	if (data == NULL || self_ == NULL || mem_addr >= self_->data_size)
		return -1;
	uint8_t * current_data_ptr = data;
	int32_t current_mem_addr = mem_addr;
	int32_t page_size = self_->page_size;
	int32_t once_read_size = 0;
	int32_t remain_size = size;
	
	while(remain_size > 0) {
		
		once_read_size = page_size - current_mem_addr % page_size;
		
		if(remain_size < once_read_size) {
			once_read_size = remain_size;
		}
		
		if (At24c02PageRead(self, current_mem_addr, current_data_ptr, once_read_size) != 0 ) {
			return -1;
		}
		CpuDelayUs(10000);
		current_mem_addr += once_read_size;
		current_data_ptr += once_read_size;
	
		remain_size -= once_read_size;
		
	}
	
	return 0;
}



/**
 * @brief random write function, random writing can be performance  
 * @param self: Object Pointer 
 * @param mem_addr: addr of the src data block
 * @param data: data buffer used to save the data write in
 * @param size: size of the data block
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t At24c02Write(void * self, uint32_t mem_addr, const uint8_t * data, int32_t size)
{
	At24c02 * self_ = (At24c02 *) self;
	if (data == NULL || self_ == NULL || mem_addr >= self_->data_size)
		return -1;
	const uint8_t * current_data_ptr = data;
	int32_t current_mem_addr = mem_addr;
	int32_t page_size = self_->page_size;
	int32_t once_write_size = 0;
	int32_t remain_size = size;
	
	while(remain_size > 0) {
		
		once_write_size = page_size - current_mem_addr % page_size;
		
		if(remain_size < once_write_size) {
			once_write_size = remain_size;
		}
		
		if (At24c02PageWrite(self, current_mem_addr, current_data_ptr, once_write_size) != 0 ) {
			
			return -1;
		}
		
		CpuDelayUs(10000);
		
		current_mem_addr += once_write_size;
		current_data_ptr += once_write_size;
	
		remain_size -= once_write_size;
		
	}
	
	return 0;
}


/**
 * @brief load all data from device
 * @param self: Object Pointer 
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t At24c02SaveAll(void * self)
{	
	return At24c02Write(self, 0, (uint8_t *)((At24c02 *)self)->data_buffer, ((At24c02 *)self)->data_size);
}


/**
 * @brief save all data to data persisttence device, called by a marco
 * @param self: Object Pointer 
 * @return return 0 if success
 * @author xinyu.han
 */
int32_t At24c02LoadAll(void * self)
{
	return At24c02Read(self, 0, (uint8_t *)((At24c02 *)self)->data_buffer, ((At24c02 *)self)->data_size);
}


int32_t At24c02Init(void * self)
{
	At24c02 * self_ = (At24c02 *)self;
	self_->read 	= At24c02Read;
	self_->write 	= At24c02Write;
	self_->save_all = At24c02SaveAll;
	self_->load_all = At24c02LoadAll;
	return 0;
}











