#ifndef __USART_H__
#define __USART_H__
#include "config.h"

//�����豸��ע��
#define SAMPLE_UART3_NAME       "uart3"

// ���ж��� 
#define  RECBUF_MAX  	256															// ���������ݻ�����

//������������
extern unsigned char 	s_sUsartBuf1 [256];				// ���ڴ�����Buff
extern unsigned char 	s_sUsartBuf3 [256];			// ���ڴ�����Buff

extern unsigned char RECPos1;     			//�������鳤��
extern unsigned char RECPos3;     			//�������鳤��

extern unsigned char RecCh;						//������յ�һ���ֽ�����

/* �������� */
void serial3_init(void);     //����3�豸��ʼ��
rt_err_t usart3_input(rt_device_t dev, rt_size_t size);   //�жϻص�����

void UsartSendByte(USART_Type* USARTx,uint8 ch);
void USART_printf ( USART_Type * USARTx, char * Data, ... );

#define BSP_IF_USING_ESP8266_USART2_IRQHandler    1

#endif

