#include"dht11.h"
#include"delay.h"
void dht11_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
}
u8 dht11_get(u16 *temp,u16 *hum)
{
	 u8 i=0;
	 u32 dat=0;
	 u8 crc=0;
	 
	 DHT_EN_OUT();
	 DHT_DQ_OUT=0;
	 delay_ms(20);
	 DHT_DQ_OUT=1;
	 delay_us(30);
	 DHT_EN_IN();
	 while(DHT_DQ_IN)
	 {
		 i++;
		 delay_us(5);
		 if(i>200)return 1;
	 }
	 delay_us(20);
	  while(!DHT_DQ_IN)
	 {
		 i++;
		 delay_us(5);
		 if(i>200)return 1;
	 }
	 for(i=0;i<40;i++)
	 {
		 while(DHT_DQ_IN);
		 while(!DHT_DQ_IN);
		 delay_us(40);
		 if(i<32)
		 {
			 dat<<=1;
		 if(DHT_DQ_IN)dat|=1;
		 }
		 else
		 {
			 crc<<=1;
			 if(DHT_DQ_IN)crc|=1;
		 }
		 
	 }
	 DHT_EN_OUT();
	 DHT_DQ_OUT=1;
	 i=(dat>>24)+((dat>>16)&0xff)+((dat>>8)&0xff)+((dat>>0)&0xff);
	 if(crc==i)
	 {
	 *hum=dat>>24;
	 *temp=(dat&0x0000ff00)>>8; 
	 }
	 else return 1;
	 return 0;
}




