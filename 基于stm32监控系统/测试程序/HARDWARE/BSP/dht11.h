#ifndef _dht11_h
#define _dht11_h
#include"sys.h"
#define DHT_EN_IN() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=0X00008000;}
#define DHT_EN_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=0X00003000;}
#define DHT_DQ_OUT PAout(11)
#define DHT_DQ_IN  PAin(11)
u8 dht11_get(u16 *temp,u16 *hum);
void dht11_init(void);
#endif


