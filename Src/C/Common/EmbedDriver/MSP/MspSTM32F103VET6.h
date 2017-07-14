#ifndef _MSP_H_
#define _MSP_H_

#include "../F10xStdPeriph/F10xStdPeriph.h"

#define TEST_BOARD_20160801
#ifdef  TEST_BOARD_20160801

//=============================
//		DMA
//=============================
#define UART1_TX_DMA {                            \
	.rcc_mask = RCC_AHBPeriph_DMA1,               \
	.dma_ch = DMA1_Channel4,                      \
	.dir = DMA_DIR_PeripheralDST,                 \
	.it_reg  = DMA1_IT_TC4,						  \
	.periph_addr = (uint32_t)&(USART1->DR),       \
	.datasize = DMA_MemoryDataSize_Byte,          \
	.dma_mode = DMA_Mode_Normal,                  \
	.dma_priority = DMA_Priority_High,            \
}

#define UART1_RX_DMA {                            \
	.rcc_mask = RCC_AHBPeriph_DMA1,               \
	.dma_ch = DMA1_Channel5,                      \
	.dir = DMA_DIR_PeripheralSRC,                 \
	.periph_addr = (uint32_t)&(USART1->DR),       \
	.datasize = DMA_MemoryDataSize_Byte,          \
	.dma_mode = DMA_Mode_Circular,                \
	.dma_priority = DMA_Priority_High,            \
}

#define UART2_TX_DMA {                            \
	.rcc_mask = RCC_AHBPeriph_DMA1,               \
	.dma_ch = DMA1_Channel7,                      \
	.dir = DMA_DIR_PeripheralDST,                 \
	.it_reg  = DMA1_IT_TC7,						  \
	.periph_addr = (uint32_t)&(USART2->DR),       \
	.datasize = DMA_MemoryDataSize_Byte,          \
	.dma_mode = DMA_Mode_Normal,                  \
	.dma_priority = DMA_Priority_High,            \
}

#define UART2_RX_DMA {                            \
	.rcc_mask = RCC_AHBPeriph_DMA1,               \
	.dma_ch = DMA1_Channel6,                      \
	.dir = DMA_DIR_PeripheralSRC,                 \
	.periph_addr = (uint32_t)&(USART2->DR),       \
	.datasize = DMA_MemoryDataSize_Byte,          \
	.dma_mode = DMA_Mode_Circular,                \
	.dma_priority = DMA_Priority_High,            \
}

#define UART4_TX_DMA {								\
	.rcc_mask = RCC_AHBPeriph_DMA2,               	\
	.dma_ch = DMA2_Channel5,                      	\
	.dir = DMA_DIR_PeripheralDST,                 	\
	.it_reg  = DMA2_IT_TC5,						  \
	.periph_addr = (uint32_t)&(UART4->DR),       	\
	.datasize = DMA_MemoryDataSize_Byte,          	\
	.dma_mode = DMA_Mode_Normal,                  	\
	.dma_priority = DMA_Priority_Low,            	\
}

#define UART4_RX_DMA {								\
	.rcc_mask = RCC_AHBPeriph_DMA2,               	\
	.dma_ch = DMA2_Channel3,                      	\
	.dir = DMA_DIR_PeripheralSRC,                 	\
	.it_reg  = DMA2_IT_TC3,						  \
	.periph_addr = (uint32_t)&(UART4->DR),       	\
	.datasize = DMA_MemoryDataSize_Byte,          	\
	.dma_mode = DMA_Mode_Circular,                	\
	.dma_priority = DMA_Priority_Low,            	\
}

#define UART3_TX_DMA {								\
	.rcc_mask = RCC_AHBPeriph_DMA1,               	\
	.dma_ch = DMA2_Channel2,                      	\
	.dir = DMA_DIR_PeripheralDST,                 	\
	.it_reg  = DMA1_IT_TC2,						    \
	.periph_addr = (uint32_t)&(USART3->DR),       	\
	.datasize = DMA_MemoryDataSize_Byte,          	\
	.dma_mode = DMA_Mode_Normal,                  	\
	.dma_priority = DMA_Priority_Low,            	\
}

#define UART3_RX_DMA {								\
	.rcc_mask = RCC_AHBPeriph_DMA1,               	\
	.dma_ch = DMA1_Channel3,                      	\
	.dir = DMA_DIR_PeripheralSRC,                 	\
	.it_reg  = DMA1_IT_TC3,						    \
	.periph_addr = (uint32_t)&(USART3->DR),       	\
	.datasize = DMA_MemoryDataSize_Byte,          	\
	.dma_mode = DMA_Mode_Circular,                	\
	.dma_priority = DMA_Priority_Low,            	\
}

extern DmaClass mUart1RxDma;
extern DmaClass mUart1TxDma;
extern DmaClass mUart2RxDma;
extern DmaClass mUart2TxDma;
extern DmaClass mUart4RxDma;
extern DmaClass mUart4TxDma;
extern DmaClass mUart3RxDma;
extern DmaClass mUart3TxDma;
//=============================
//		UART
//=============================
#define USART1_DEFAULT_DMA_CFG {                        \
	.usart = USART1,                                    \
	.baud = 921600,                                     \
	.tx_pin = NEW_GPIO(A, 9, GPIO_Mode_AF_PP),          \
	.rx_pin = NEW_GPIO(A, 10, GPIO_Mode_IN_FLOATING),   \
	.tx_mode = GPIO_Mode_AF_PP,                         \
	.rcc_mask = RCC_APB2Periph_USART1,                  \
	.rcc_apb_group = 2,                                 \
	.remap = GPIO_NO_REMAP,                             \
	.rxdma = &mUart1RxDma,								\
	.txdma = &mUart1TxDma,								\
};

#define USART1_DMA_CFG {                        \
	.usart = USART1,                                    \
	.baud = 19200,                                     \
	.tx_pin = NEW_GPIO(A, 9, GPIO_Mode_AF_OD),          \
	.rx_pin = NEW_GPIO(A, 10, GPIO_Mode_IN_FLOATING),   \
	.tx_mode = GPIO_Mode_AF_PP,                         \
	.rcc_mask = RCC_APB2Periph_USART1,                  \
	.rcc_apb_group = 2,                                 \
	.remap = GPIO_NO_REMAP,                             \
	.rxdma = &mUart1RxDma,								\
	.txdma = &mUart1TxDma,								\
};

#define USART1_REMAP_DMA_CFG {                          \
	.usart = USART1,                                    \
	.baud = 115200,                                     \
	.tx_pin = NEW_GPIO(B, 6, GPIO_Mode_AF_PP),          \
	.rx_pin = NEW_GPIO(B, 7, GPIO_Mode_IN_FLOATING),    \
	.tx_mode = GPIO_Mode_AF_PP,                         \
	.rcc_mask = RCC_APB2Periph_USART1,                  \
	.rcc_apb_group = 2,                                 \
	.remap = GPIO_Remap_USART1,                         \
	.rxdma = &mUart1RxDma,								\
	.txdma = &mUart1TxDma,								\
};

#define USART2_DEFAULT_DMA_CFG {                        \
	.usart = USART2,                                    \
	.baud = 921600,                                     \
	.tx_pin = NEW_GPIO(A, 2, GPIO_Mode_AF_PP),          \
	.rx_pin = NEW_GPIO(A, 3, GPIO_Mode_IN_FLOATING),   \
	.tx_mode = GPIO_Mode_AF_PP,                         \
	.rcc_mask = RCC_APB1Periph_USART2,                  \
	.rcc_apb_group = 1,                                 \
	.remap = GPIO_NO_REMAP,                             \
	.rxdma = &mUart2RxDma,								\
	.txdma = &mUart2TxDma,								\
};

#define USART2_DMA_CFG {                        \
	.usart = USART2,                                    \
	.baud = 115200,                                     \
	.tx_pin = NEW_GPIO(A, 2, GPIO_Mode_AF_PP),          \
	.rx_pin = NEW_GPIO(A, 3, GPIO_Mode_IN_FLOATING),   \
	.tx_mode = GPIO_Mode_AF_PP,                         \
	.rcc_mask = RCC_APB1Periph_USART2,                  \
	.rcc_apb_group = 1,                                 \
	.remap = GPIO_NO_REMAP,                             \
	.rxdma = &mUart2RxDma,								\
	.txdma = &mUart2TxDma,								\
};

#define UART4_DMA_CFG {									\
	.usart = UART4,                                    \
	.baud = 19200,                                     \
	.tx_pin = NEW_GPIO(C, 10, GPIO_Mode_AF_OD),          \
	.rx_pin = NEW_GPIO(C, 11, GPIO_Mode_IN_FLOATING),   \
	.tx_mode = GPIO_Mode_AF_OD,                         \
	.rcc_mask = RCC_APB1Periph_UART4,                  \
	.rcc_apb_group = 1,                                 \
	.remap = GPIO_NO_REMAP,                             \
	.rxdma = &mUart4RxDma,								\
	.txdma = &mUart4TxDma,								\
};

#define USART3_REMAP_DMA_CFG {                          \
	.usart = USART3,                                    \
	.baud = 115200,                                     \
	.tx_pin = NEW_GPIO(D, 8, GPIO_Mode_AF_PP),          \
	.rx_pin = NEW_GPIO(D, 9, GPIO_Mode_IN_FLOATING),    \
	.tx_mode = GPIO_Mode_AF_PP,                         \
	.rcc_mask = RCC_APB1Periph_USART3,                  \
	.rcc_apb_group = 1,                                 \
	.remap = GPIO_FullRemap_USART3,                     \
	.rxdma = &mUart3RxDma,								\
	.txdma = &mUart3TxDma,								\
};

//=============================
//		ADC
//=============================


//=============================
//		GPIO
//=============================


#endif  //TEST_BOARD_20160801

#endif //_MSP_H_



