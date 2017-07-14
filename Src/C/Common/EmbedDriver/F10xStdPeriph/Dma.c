#include "F10xStdPeriph.h"


void DmaConfig(DmaClass* self)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(self->rcc_mask,ENABLE);
	uint32_t temp = 0;
	if(self->dma_ch >= DMA1_Channel1 && self->dma_ch <= DMA1_Channel7) {
		self->dma = DMA1;
		self->dma_channel_index = ((uint32_t)self->dma_ch - (uint32_t)DMA1_Channel1) / (0x14) ;
	}
	else if(self->dma_ch >= DMA2_Channel1 && self->dma_ch <= DMA2_Channel5) {
		self->dma = DMA2;
		self->dma_channel_index = ((uint32_t)self->dma_ch - (uint32_t)DMA2_Channel1) / (0x14);
	}
	
	temp = ((1u) << (self->dma_channel_index << 2));
	
	self->dma_channel_low_bit = temp;
	self->dma_channel_mask = (temp << 3) | (temp << 2) | (temp << 1) | (temp);
	
	DMA_DeInit(self->dma_ch);
	DMA_InitStructure.DMA_PeripheralBaseAddr = self->periph_addr;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)self->trans_buf;   
	DMA_InitStructure.DMA_DIR = self->dir;
	DMA_InitStructure.DMA_BufferSize =  self->buffer_size;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	DMA_InitStructure.DMA_PeripheralDataSize = self->datasize;	
	DMA_InitStructure.DMA_MemoryDataSize = self->datasize;
	DMA_InitStructure.DMA_Mode = self->dma_mode;		
	DMA_InitStructure.DMA_Priority = self->dma_priority;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(self->dma_ch, &DMA_InitStructure);
		
	self->is_first_trans = true;
	
	DMA_Cmd(self->dma_ch, ENABLE);
}

bool DmaMemToPeriphStart(DmaClass* self, uint8_t* data, uint16_t len)
{
	if(!self->is_first_trans){
		if(!DMA_IT_GET_STATE(self, TCIF)){   //If the transmission of last frame is not finished..
			return false;
		}
		DMA_IT_CLEAR_STATE(self, TCIF);
	}
	else{
		self->is_first_trans = false;
	}

	DMA_DISABLE(self);						//Disable DMA
	DMA_MEM_ADDR(self) = (uint32_t)data;
	DMA_CNT(self) = len;
	DMA_ENABLE(self);						//Renable DMA
	
	return true;
}

bool DmaStart(DmaClass* self, uint8_t* data, uint16_t len)
{
	if(!self->is_first_trans){
		if(!DMA_IT_GET_STATE(self, TCIF)){   //If the transmission of last frame is not finished..
			return false;
		}
		DMA_IT_CLEAR_STATE(self, TCIF);
	}

	DMA_DISABLE(self);						//Disable DMA
	DMA_MEM_ADDR(self) = (uint32_t)data;
	DMA_CNT(self) = len;
	DMA_ENABLE(self);						//Renable DMA
	
	return true;
}
