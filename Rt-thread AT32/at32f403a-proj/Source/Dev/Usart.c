#include "Usart.h"
#include "config.h"

unsigned char 	s_sUsartBuf3 [RECBUF_MAX]={0};				// ����3������Buff3
unsigned char RECPos3=0;															//�洢��ǰ����3�������ݴ�ŵ�λ��

unsigned char RecCh;																	//������յ�һ���ֽ�����

/********************************************************usart3�豸��ʼ****************************************************************/
/* ��ʼ�� */
 void serial3_init(void)
{
//	static char msg_pool[256];
	/* ����ϵͳ�еĴ���3�豸 */
	serial3 = rt_device_find(SAMPLE_UART3_NAME);
	if (!serial3)
	{
			rt_kprintf("find %s failed!\n\n", SAMPLE_UART3_NAME);
	}
//    /* ��ʼ������3����������Ϣ���� */
//    rt_mq_init(&usart3_rx_queue, 
//							"usart3_rx_queue",						 /* ���� */
//               msg_pool,                 /* �����Ϣ�Ļ����� */
//               sizeof(struct rx_msg),    		 /* һ����Ϣ����󳤶� */
//               sizeof(s_sUsartBuf3),         /* �����Ϣ�Ļ�������С */
//               RT_IPC_FLAG_FIFO);        		 /* ����ж���̵߳ȴ������������ȵõ��ķ���������Ϣ */
  
	usart3_rx_sem=rt_sem_create("usart3_rx_sem", 0, RT_IPC_FLAG_FIFO);
	if(usart3_rx_sem!=RT_NULL)
	{
		rt_kprintf("\nCreate usart3_rx_sem Successful !\n\n");
		/* ���жϽ��ռ���ѯ���ͷ�ʽ�򿪴����豸 */
		rt_device_open(serial3, RT_DEVICE_FLAG_INT_RX);
		/* ���ý��ջص����� */
		rt_device_set_rx_indicate(serial3, usart3_input);
	}
}

/* �������ݻص����� */
rt_err_t usart3_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(usart3_rx_sem);

    return RT_EOK;
}
/*
 * �����嵥������һ�� ���� �豸ʹ������
 * ���̵����� uart_sample ��������ն�
 * ������ø�ʽ��uart3_sample 
 * �����ܣ�ͨ����������ַ���"hello RT-Thread!"
*/
static int usart3_sample(void)
{
    char str[] = "hello RT-Thread!\r\n";

    /* ��ӡ�ַ��� */
	  rt_kprintf("%s\r\n",str);   //
	  rt_kprintf("usart3 recv data:%s\r\n",s_sUsartBuf3);

	  memset(s_sUsartBuf3,0,sizeof(s_sUsartBuf3));    //���
	  RECPos3=0;
    return 0;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(usart3_sample, uart3 device sample);
/********************************************************usart3�豸����****************************************************************/

/*  ���͵����ֽ�����  */
void UsartSendByte(USART_Type* USARTx,uint8 ch)
{
	
	USART_SendData(USARTx, (uint8)ch);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TRAC)== RESET);
}

//��������������� HZJ
/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART2_printf()����
 */
static char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;
		
	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;

} /* NCL_Itoa */

/*
 * ��������USART_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ����
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART3_printf( USART3, "\r\n this is a demo \r\n" );
 *            		 USART3_printf( USART3, "\r\n %d \r\n", i );
 *            		 USART3_printf( USART3, "\r\n %s \r\n", j );
 */
void USART_printf ( USART_Type * USARTx, char * Data, ... ) 
{
	const char *s;
	int d;   
	char buf[16];

	
	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //���з�
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;

				default:
				Data ++;
				break;
			}			 
		}
		
		else if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
				s = va_arg(ap, const char *);
				
				for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TDE) == RESET );
				}
				
				Data++;
				
				break;

				case 'd':			
					//ʮ����
				d = va_arg(ap, int);
				
				itoa(d, buf, 10);
				
				for (s = buf; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TDE) == RESET );
				}
				
				Data++;
				
				break;
				
				default:
				Data++;
				
				break;
				
			}		 
		}
		
		else USART_SendData(USARTx, *Data++);
		
		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TDE ) == RESET );
		
	}
}
/******************************************************end of file*****************************************/
