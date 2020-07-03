#include "Usart.h"
#include "config.h"

unsigned char 	s_sUsartBuf3 [RECBUF_MAX]={0};				// 串口3处理缓存Buff3
unsigned char RECPos3=0;															//存储当前串口3接收数据存放的位置

unsigned char RecCh;																	//储存接收的一个字节数据

/********************************************************usart3设备开始****************************************************************/
/* 初始化 */
 void serial3_init(void)
{
//	static char msg_pool[256];
	/* 查找系统中的串口3设备 */
	serial3 = rt_device_find(SAMPLE_UART3_NAME);
	if (!serial3)
	{
			rt_kprintf("find %s failed!\n\n", SAMPLE_UART3_NAME);
	}
//    /* 初始化串口3接收数据消息队列 */
//    rt_mq_init(&usart3_rx_queue, 
//							"usart3_rx_queue",						 /* 名字 */
//               msg_pool,                 /* 存放消息的缓冲区 */
//               sizeof(struct rx_msg),    		 /* 一条消息的最大长度 */
//               sizeof(s_sUsartBuf3),         /* 存放消息的缓冲区大小 */
//               RT_IPC_FLAG_FIFO);        		 /* 如果有多个线程等待，按照先来先得到的方法分配消息 */
  
	usart3_rx_sem=rt_sem_create("usart3_rx_sem", 0, RT_IPC_FLAG_FIFO);
	if(usart3_rx_sem!=RT_NULL)
	{
		rt_kprintf("\nCreate usart3_rx_sem Successful !\n\n");
		/* 以中断接收及轮询发送方式打开串口设备 */
		rt_device_open(serial3, RT_DEVICE_FLAG_INT_RX);
		/* 设置接收回调函数 */
		rt_device_set_rx_indicate(serial3, usart3_input);
	}
}

/* 接收数据回调函数 */
rt_err_t usart3_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(usart3_rx_sem);

    return RT_EOK;
}
/*
 * 程序清单：这是一个 串口 设备使用例程
 * 例程导出了 uart_sample 命令到控制终端
 * 命令调用格式：uart3_sample 
 * 程序功能：通过串口输出字符串"hello RT-Thread!"
*/
static int usart3_sample(void)
{
    char str[] = "hello RT-Thread!\r\n";

    /* 打印字符串 */
	  rt_kprintf("%s\r\n",str);   //
	  rt_kprintf("usart3 recv data:%s\r\n",s_sUsartBuf3);

	  memset(s_sUsartBuf3,0,sizeof(s_sUsartBuf3));    //清空
	  RECPos3=0;
    return 0;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(usart3_sample, uart3 device sample);
/********************************************************usart3设备结束****************************************************************/

/*  发送单个字节数据  */
void UsartSendByte(USART_Type* USARTx,uint8 ch)
{
	
	USART_SendData(USARTx, (uint8)ch);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TRAC)== RESET);
}

//添加下面两个函数 HZJ
/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART2_printf()调用
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
 * 函数名：USART_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART3_printf( USART3, "\r\n this is a demo \r\n" );
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

	while ( * Data != 0 )     // 判断是否到达字符串结束符
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //换行符
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
				case 's':										  //字符串
				s = va_arg(ap, const char *);
				
				for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TDE) == RESET );
				}
				
				Data++;
				
				break;

				case 'd':			
					//十进制
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
