/****************************************Copyright (c)**************************************************
**   
**------------------------------------------------------------------------------------------------------
** 文件: config.h 
** 版本: v1.0
**------------------------------------------------------------------------------------------------------
** 描述:
**      用于存放开发工程的全局变量
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef __CONFIG__H__
#define __CONFIG__H__

//----------------------------------------------------------------
//无符号类型宏定义
#define uchar 	unsigned char													 
//#define uint 	unsigned int

#define uint8 	unsigned char 
#define uint16 	unsigned short int
#define uint32 	unsigned int
	
//包含全局类型定义

typedef enum
{
  MSG_NULL = 0,
  /***********************************/
  //添加用户消息常量，例如：MSG_XXX
  MSG_KEY1_PRESS,
  MSG_KEY2_PRESS,
  

  /*************************************/
  MSG_NUM
	
}MSG_TYPE;

//----------------------------------------------------------------
// MAIN_GLOBAL宏	main.c 定义#define  MAIN_GLOBAL
#ifdef 	 MAIN_CONFIG										// 头文件被多个C文件调用,避免变量冲突的问题 
	#define  EXT											// 主文件中定义的变量
#else
	#define  EXT	extern									// 全局文件变量的声明
#endif
	
	//----------------------------------------------------------------
//RTOS相关的头文件包含
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "Task.h"     //自定义文件，TAsk.c中存取线程、定时器、TPC等相关代码
	
//----------------------------------------------------------------
//包含常用头文件  
#include "board.h"     // 如果要用AT的固件库，必须包含这个头文件
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "system.h"
	
//----------------------------------------------------------------
//包含外设驱动头文件  
#include "drv_gpio.h"
#include "Usart.h"
#include "SysTick.h"
#include "BspEsp8266.h"
#include "WifiSet.h"
#include "HMI.h"
#include "HMIrtc.h"
#include "time.h"
#include "WeatherInfo.h"
#include "CJSON.h"
#include "malloc.h"
#include "GbkToUtf_8.h"
#include "NtpExample.h"

//---------------------------------------------------------------
/* 串口3接收消息结构*/
struct rx_msg
{
		rt_device_t dev;
		rt_size_t size;
};
	


/* 用于接收消息的信号量 */
EXT rt_sem_t usart3_rx_sem;            //串口3接收数据信号量
EXT rt_sem_t espwifi_getweath;         //访问天气信号量
EXT rt_sem_t HMIRePre_sem;     				 //串口屏信号量
EXT rt_sem_t GetWea_sem;            //WIFI已连接 串口屏刷新天气数据信号量
/* 设备 */
EXT rt_device_t serial3;
//---------------------------------------------------------------


#endif

/********************************************************************************************************
**                            End Of File
********************************************************************************************************/

