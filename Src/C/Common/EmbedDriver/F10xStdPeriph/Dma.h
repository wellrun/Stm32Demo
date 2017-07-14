#ifndef CONFIG_DMA_H_
#define CONFIG_DMA_H_

#include "F10xStdPeriph.h"

#ifdef __cplusplus
extern "C" {
#endif
	
	#define DMA_IT_STATE_BIT_GIF_SHIFT	(0u)
	#define DMA_IT_STATE_BIT_GIF_MASK	(1u)
	#define DMA_IT_STATE_BIT_TCIF_SHIFT	(1u)
	#define DMA_IT_STATE_BIT_TCIF_MASK	(2u)
	#define DMA_IT_STATE_BIT_HTIF_SHIFT	(2u)
	#define DMA_IT_STATE_BIT_HTIF_MASK	(4u)
	#define DMA_IT_STATE_BIT_TEIF_SHIFT	(3u)
	#define DMA_IT_STATE_BIT_TEIF_MASK	(8u)

	#define DMA_IT_GET_STATE(self, it) (((self)->dma->ISR & (((self)->dma_channel_low_bit) << (DMA_IT_STATE_BIT_##it##_SHIFT))) != 0)
	#define DMA_IT_CLEAR_STATE(self, it) ((self)->dma->IFCR = (((self)->dma_channel_low_bit) << (DMA_IT_STATE_BIT_##it##_SHIFT)) )
	
	#define DMA_ENABLE(self)	((self)->dma_ch->CCR |= ((uint32_t)0x01))
	#define DMA_DISABLE(self)	((self)->dma_ch->CCR &= ~((uint32_t)0x01))
	
	#define DMA_CNT(self)		((self)->dma_ch->CNDTR)
	#define DMA_MEM_ADDR(self)		((self)->dma_ch->CMAR)
	#define DMA_PHE_ADDR(self)		((self)->dma_ch->CPAR)
	
	typedef struct DmaClass{
		uint32_t rcc_mask;
		DMA_Channel_TypeDef* dma_ch;
		DMA_TypeDef* dma;
		uint32_t dir;
		uint32_t it_reg;
		uint32_t periph_addr;
		uint16_t buffer_size;
		uint32_t datasize;
		uint32_t dma_mode;
		uint32_t dma_priority;
		uint32_t dma_channel_mask;
		uint32_t dma_channel_low_bit;
		uint8_t dma_channel_index;
		bool is_tx_share_mem;
		bool is_first_trans;
		
		/*Don't need init*/
		uint8_t* trans_buf;	
	} DmaClass;
	
	#define NEW_DMA(dma, ch, 	 	 \
		dir, periph_addr,            \
		dma_mode, dma_priority,      \
		is_tx_share_mem)             \
	{									\
	}

	#define DMA_GET_CNT(dma_ch) dma_ch->CNDTR
	
	void DmaConfig(DmaClass* self);
	bool DmaMemToPeriphStart(DmaClass* self, uint8_t* data, uint16_t len);
	bool DmaStart(DmaClass* self, uint8_t* data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
