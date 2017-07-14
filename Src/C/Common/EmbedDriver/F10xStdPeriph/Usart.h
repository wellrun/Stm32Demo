#ifndef CONFIG_UART_H_
#define CONFIG_UART_H_

#include "F10xStdPeriph.h"
	struct DmaClass;
	
	typedef struct UsartClass_
	{
		USART_TypeDef* usart;
		uint32_t baud;
		GpioClass tx_pin;
		GpioClass rx_pin;
		GPIOMode_TypeDef tx_mode;
		uint32_t rcc_mask;
		uint8_t rcc_apb_group;
		uint32_t remap;
		
		uint8_t* trans_buf;
		uint16_t trans_buf_size;
		uint8_t* recv_buf;
		uint16_t recv_buf_size;
		
		uint16_t recv_len;
		uint16_t read_byte_ptr;
		uint8_t  no_recv_cnt;
		struct DmaClass* rxdma;
		struct DmaClass* txdma;
		bool is_trans_share_mem;
	} UsartClass;

	void UartConfig(UsartClass* self);
	void UsartPutchar(USART_TypeDef* usart, uint16_t ch);
	void UsartTransShareMemEnable(void* self_, bool new_state);
	void UsartTransBufCreate(void* self_, int size);
	bool UsartBlockSend(void* self_, uint8_t* data, int len);
	void UartNewReadLoop(void* self_);
	bool UartGetOneByte(void* self_, uint8_t* data);
	bool UsartSelfTest(UsartClass* self);
	
	int32_t UartSend(void* self_, uint8_t* data, int len);
	int32_t UartRecv(void* self_, uint8_t** data, int32_t * length);
	int32_t UartForceReset(void * self_);
	int UartIsDmaBuffAlwaysEmpty(UsartClass* self);
#endif
