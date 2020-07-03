/****************************************Copyright (c)**************************************************
**																福建师范大学物理与能源学院
**------------------------------------------------------------------------------------------------------
** 文件: system.c 
** 版本: v1.0
**------------------------------------------------------------------------------------------------------
** 描述:
**      系统的初始化
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "system.h"

#define WDT_DEVICE_NAME    "wdt"    /* 看门狗设备名称 */

static rt_device_t wdg_dev;         /* 看门狗设备句柄 */

static void idle_hook(void)
{
    /* 在空闲线程的回调函数里喂狗 */
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
//    rt_kprintf("feed the dog!\n ");
}

/******************************************************************************************************
 * 看门狗设备初始化
 *******************************************************************************************************/
void WDT_Init(void)
{
	rt_err_t ret = RT_EOK;
	rt_uint32_t timeout = 1;
	
	wdg_dev = rt_device_find(WDT_DEVICE_NAME);
	if (!wdg_dev)
	{
			rt_kprintf("find %s failed!\n", WDT_DEVICE_NAME);
	}
	/* 初始化设备 */
	ret = rt_device_init(wdg_dev);
	if (ret != RT_EOK)
	{
			rt_kprintf("initialize %s failed!\n", WDT_DEVICE_NAME);
	}
	/* 设置看门狗溢出时间 */
	ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);   //溢出时间  25s
	if (ret != RT_EOK)
	{
			rt_kprintf("set %s timeout failed!\n", WDT_DEVICE_NAME);
	}
	/* 启动看门狗 */
	ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
	if (ret != RT_EOK)
	{
			rt_kprintf("start %s failed!\n", WDT_DEVICE_NAME);
	}
	/* 设置空闲线程回调函数 */
	rt_thread_idle_sethook(idle_hook);
}

void LED_Init(void)
{
      /* set LED2 pin mode to output */
    rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
    /* set LED3 pin mode to output */
    rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);
    /* set LED4 pin mode to output */
    rt_pin_mode(LED4_PIN, PIN_MODE_OUTPUT);
}
/*******************************************************************************************************
** 函数: SysInit，上电初始化
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: 无
********************************************************************************************************/
void SysInit(void)
{
	SysTickInit();				        	 // 系统滴答定时器初始化函数
  ESP8266_Init();               	 // esp8266模块引脚，串口2初始化，波特率115200;
  RTC_Init();                      // RTC初始化
	WDT_Init();                      // 看门狗设备初始化
	TaskInit();    									 // 线程任务初始化
}

static void ResetSystem(void)
{
  SysInit();   //初始化
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(ResetSystem, Reset System);

/********************************************************************************************************
**                            End Of File
********************************************************************************************************/

