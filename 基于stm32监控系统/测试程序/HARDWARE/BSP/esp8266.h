#ifndef _esp8266_h_
#define _esp8266_h_
#include <stdbool.h>
#include"sys.h"
#define USART2_RX_LEN 200

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#define   macUser_ESP8266_BulitApSsid         "ESP8266"      //要建立的热点的名称
#define   macUser_ESP8266_BulitApEcn           OPEN               //要建立的热点的加密方式
#define   macUser_ESP8266_BulitApPwd           "12345678"         //要建立的热点的密钥

#define   macUser_ESP8266_TcpServer_IP         "192.168.4.1"      //服务器开启的IP地址
#define   macUser_ESP8266_TcpServer_Port       "8080"             //服务器开启的端口   

#define   macUser_ESP8266_TcpServer_OverTime   "1800"             //服务器超时时间（单位：秒）
typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;

typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
	
typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;

extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	char  Data_RX_BUF [ USART2_RX_LEN ];
	
  union {
    __IO u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} strEsp8266_Fram_Record;
typedef struct{
	u8 on_off;
	u16 temp;
	u16 tmax;
	u16 mq5val;
	u16 mq5max;
	u16 mq7val;
	u16 mq7max;

}SensorDef;
extern SensorDef sensorend1;
void uart2_init(u32 bound);

bool ESP8266_Cmd ( char * cmd, char * reply1, char * reply2, u32 waittime );
void ESP8266_Server(void);
uint8_t ESP8266_Inquire_StaIp ( char * STAIp, uint8_t ucArrayLength );
extern u8 paraFan[4];
extern u8 smartconfig_en;
#endif




