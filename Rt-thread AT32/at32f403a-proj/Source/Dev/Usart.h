#ifndef __USART_H__
#define __USART_H__
#include "config.h"

//串口设备名注册
#define SAMPLE_UART3_NAME       "uart3"

// 共有定义 
#define  RECBUF_MAX  	256															// 最大接收数据缓冲区

//接收数组声明
extern unsigned char 	s_sUsartBuf1 [256];				// 串口处理缓存Buff
extern unsigned char 	s_sUsartBuf3 [256];			// 串口处理缓存Buff

extern unsigned char RECPos1;     			//接收数组长度
extern unsigned char RECPos3;     			//接收数组长度

extern unsigned char RecCh;						//储存接收的一个字节数据

/* 函数声明 */
void serial3_init(void);     //串口3设备初始化
rt_err_t usart3_input(rt_device_t dev, rt_size_t size);   //中断回调函数

void UsartSendByte(USART_Type* USARTx,uint8 ch);
void USART_printf ( USART_Type * USARTx, char * Data, ... );

#define BSP_IF_USING_ESP8266_USART2_IRQHandler    1

#endif

