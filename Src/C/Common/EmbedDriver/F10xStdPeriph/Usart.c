#include "F10xStdPeriph.h"
#include "BSP.h"
#include <stdio.h>
/**
@code
# The value of rcc_apb_group depend on the variable 'rcc_mask', the relation is like below:
    ## If the periph clock is belong the APB1, then the value of rcc_apb_group should be 1,
         the periph clock is belong the APB2, then the value of rcc_apb_group should be 2

@endcode
*/
void UartConfig(UsartClass* self)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    if(self->rcc_apb_group == 1){
        RCC_APB1PeriphClockCmd(self->rcc_mask, ENABLE);
	}
    if(self->rcc_apb_group == 2){
        RCC_APB2PeriphClockCmd(self->rcc_mask, ENABLE);
	}

    if( self->remap != GPIO_NO_REMAP){
        GPIO_PinRemapConfig(self->remap, ENABLE);
	}

    GpioConfig(&self->tx_pin);
    GpioConfig(&self->rx_pin);

	USART_DeInit(self->usart);
	USART_InitStructure.USART_BaudRate = self->baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	//=================================================
	//		Check whether use share mem or not.
	//=================================================
	if(self->trans_buf_size == 0){
		self->is_trans_share_mem = true;
	}
	else{
		self->is_trans_share_mem = false;
		self->trans_buf = (uint8_t*)malloc( sizeof(uint8_t)* (self->trans_buf_size));


		if(NULL == self->trans_buf){
			THROW_MSG("Dma buffer create failed...\n\r");
		}
	}

	//=================================================
	//	Create USART recv buffer
	//=================================================
	if(self->recv_buf_size == 0){
		THROW_MSG("Can't create USART object without recv buffer!\n\r");
	}

	self->recv_buf = (uint8_t*)malloc( sizeof(uint8_t)* (self->recv_buf_size));
	if(NULL == self->recv_buf){
		if(self->is_trans_share_mem == false){
			free(self->trans_buf);
		}
		THROW_MSG("Dma buffer create failed...\n\r");
	}

	//=================================================
	//	Config RX DMA
	//=================================================
	if(self->rxdma != NULL){
		self->rxdma->buffer_size = self->recv_buf_size;
		self->rxdma->trans_buf = self->recv_buf;
		DmaConfig(self->rxdma);
		USART_DMACmd(self->usart,USART_DMAReq_Rx,ENABLE);
	}

	//=================================================
	//	Config TX DMA
	//=================================================
	if(self->txdma != NULL){
		self->txdma->buffer_size = self->trans_buf_size;

		if(self->is_trans_share_mem){
			self->txdma->trans_buf = self->trans_buf;
		}

		DmaConfig(self->txdma);
		USART_DMACmd(self->usart,USART_DMAReq_Tx,ENABLE);
	}

	USART_Init(self->usart, &USART_InitStructure);
	USART_Cmd(self->usart, ENABLE);
}

void UsartPutchar(USART_TypeDef* usart, uint16_t ch)
{
	if(usart == NULL)
		return;

	USART_SendData(usart, (uint16_t) ch);
	while (USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET);
}

void UsartTransShareMemEnable(void* self_, bool new_state)
{
	UsartClass* self = (UsartClass*) self_;
	self->is_trans_share_mem = new_state;
}

void UsartTransBufCreate(void* self_, int size)
{
	UsartClass* self = (UsartClass*) self_;

	if(self->trans_buf != NULL){
		THROW_MSG("UsartClass: Trans buffer already exsit!\n\r");
		return; //We could do realloc here, but realloc may cause memmory leak in MCU,
		//so we just return..
	}

	self->trans_buf_size = size;
	self->trans_buf = (uint8_t*)malloc( sizeof(uint8_t)* (self->trans_buf_size));

	if(NULL == self->trans_buf){
		THROW_MSG("UsartClass: Trans buffer create failed!\n\r");
	}
}

bool UsartBlockSend(void* self_, uint8_t* data, int len)
{
	if(self_ == NULL){
		return false;
	}

	if(data == NULL){
		return false;
	}

	UsartClass* self = (UsartClass*) self_;

	if(self->txdma != NULL) {
		if(!self->is_trans_share_mem) {
			if(self->trans_buf == NULL) {
				return false;
			}

			memcpy(self->trans_buf, data, len);
			return DmaMemToPeriphStart(self->txdma, self->trans_buf, len);
		}
		else {
			return DmaMemToPeriphStart(self->txdma, data, len);
		}
	}
	else {
		for(int i = 0; i < len; i ++) {
			UsartPutchar(self->usart, (uint16_t)(data[i]));
		}
		return true;
	}
}


void UartNewReadLoop(void* self_)
{
	UsartClass* self = (UsartClass*) self_;

	self->recv_len = self->recv_buf_size - DMA_GET_CNT(self->rxdma->dma_ch);
}

bool UartGetOneByte(void* self_, uint8_t* data)
{
	UsartClass* self = (UsartClass*) self_;

	if(self->read_byte_ptr == self->recv_len){
		return false;
	}

	*data = self->recv_buf[self->read_byte_ptr];

	self->read_byte_ptr ++;

	if(self->read_byte_ptr >= self->recv_buf_size)
		self->read_byte_ptr = 0;
    return true;
}

bool UsartSelfTest(UsartClass* self)
{
	uint8_t data[4] = {0x55,0xAA,0x66,0x99};
	uint8_t buffer[4] = {0,0,0,0};

	if(UsartBlockSend(self, data, 4) == false){
		return false;
	}

	for(volatile int i = 0; i < 7200000; i ++){ }

	UartNewReadLoop(self);

	int p = 0;
	bool res = false;
	uint8_t data_byte = 0;

	do{
		res = UartGetOneByte(self, &data_byte);
		if(res){
			if(p < 4){
				buffer[p] = data_byte;
			}
			else{
				return false;
			}
			p ++;
		}
	}while(res);

	if(p != 4)
		return false;

	if(memcmp(data, buffer, 4) == 0){
		return true;
	}
	else{
		return false;
	}

}


int32_t UartSend(void* self_, uint8_t* data, int len)
{
	if(self_ == NULL){
		return false;
	}

	if(data == NULL){
		return false;
	}

	UsartClass* self = (UsartClass*) self_;

	if(self->txdma != NULL) {
		if(!self->is_trans_share_mem) {
			if(self->trans_buf == NULL) {
				return -1;
			}

			memcpy(self->trans_buf, data, len);
			return DmaMemToPeriphStart(self->txdma, self->trans_buf, len);
		}
		else {
			return DmaMemToPeriphStart(self->txdma, data, len);
		}
	}
	else {
		for(int i = 0; i < len; i ++) {
			UsartPutchar(self->usart, (uint16_t)(data[i]));
		}
		return 0;
	}
}

int32_t UartCheckNewData(UsartClass * self)
{
	int32_t rx_num = self->recv_buf_size - DMA_GET_CNT(self->rxdma->dma_ch);

	rx_num -= self->read_byte_ptr;

	if (rx_num < 0){
		rx_num += self->recv_buf_size;
	}

	return rx_num;
}



int32_t UartRecv(void* self_, uint8_t ** data, int32_t * length)
{
	UsartClass * self = (UsartClass *) self_;
	int32_t rx_num = UartCheckNewData((UsartClass *) self);

	if (rx_num == 0) {
		*data = NULL;
		*length = 0;
		return -1;
	}

	int32_t rx_buf_remain = self->recv_buf_size - self->read_byte_ptr;

	rx_num = (rx_buf_remain >= rx_num)?(rx_num):(rx_buf_remain);
	rx_num = (rx_num > *length)?(*length):(rx_num);

	*data = &(self->recv_buf[self->read_byte_ptr]);

	self->read_byte_ptr = (self->read_byte_ptr+rx_num)%self->recv_buf_size;

	*length = rx_num;

	return 0;
}

int32_t UartForceReset(void * self_)
{
	UsartClass * self = (UsartClass *) self_;
	if(self->txdma != NULL){
		DmaStart(self->txdma, self->trans_buf, self->trans_buf_size);
	}
	if(self->rxdma != NULL){
		DmaStart(self->rxdma, self->recv_buf, self->recv_buf_size);
	}
	
	return 0;
}

int UartIsDmaBuffAlwaysEmpty(UsartClass* self)
{	
	if(0 == (self->recv_buf_size - DMA_GET_CNT(self->rxdma->dma_ch))){
		self->no_recv_cnt ++;
		if(self->no_recv_cnt >= 10){
			return 1;
		}
	}
	else {
		self->no_recv_cnt = 0;
	}
	
	return 0;
}

int fputc(int ch, FILE *f);
int fputc(int ch, FILE *f)
{

USART_SendData(gTerminalUsart.usart, (uint8_t) ch);

while (USART_GetFlagStatus(gTerminalUsart.usart, USART_FLAG_TC) == RESET);

return ch;
}
