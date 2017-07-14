#include "Ultrasonic.h"
#include "EmbedDriver/F10xStdPeriph/F10xStdPeriph.h"

#define ULTRASONIC_OFFSET	23
#define ULTRASONIC_LIMIT_L  226		//(249 - ULTRASONIC_OFFSET)

u8 m_US2data[4] = {0};
u8 m_US_short_command[5]={0x00,0x55,0x11,0x01,0x00};

uint8_t send_data[5]={0x00,0x55,0x11,0x01,0x00};

void UltrSonicSerialRegister(UltrSonic * self, UsartClass* usart_)
{
	self->usart = usart_;
}

int32_t UltrSonicLoadDistance(UltrSonic * self)
{
	if(self == NULL){ return 0; }
	if(self->usart == NULL){ return 0; }
	
	uint8_t * recv_data = NULL;
	int32_t length = 4;
	
	UartRecv(self->usart, &recv_data, &length);
	UartSend(self->usart, send_data, 5);
	
	UltrSonicDataAnalysis(self, recv_data, length);
	
	return self->distance;
}

/**
* Raw Data: 1-div
*/
void UltrSonicDataAnalysis(UltrSonic * self, uint8_t * data, int32_t length)
{
	if( length == 4) {
		for (int32_t i = 0; i < 4; i++) {
			self->data_buf[i] = data[i];
		}
		
		if (self->data_buf[2] == 0x01 && \
			0xFF == (uint8_t)(self->data_buf[0] + self->data_buf[1] + self->data_buf[2]+ self->data_buf[3]) ) {
			self->distance = (uint16_t)( ( (self->data_buf[0]<<8)+self->data_buf[1] ) * 171.5 / 1000 ) - ULTRASONIC_OFFSET;
			//Range Limit: 0.249m - 1.5m
			if(self->distance > 1500) {
				self->distance = 1500;
			}
			else if(self->distance < ULTRASONIC_LIMIT_L){
				self->distance = ULTRASONIC_LIMIT_L;
			}
		}
		else {
			UartForceReset(self->usart);
		}
	}
	else if( length != 0) {
		UartForceReset(self->usart);
	}
}




