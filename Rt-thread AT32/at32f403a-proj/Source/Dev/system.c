/****************************************Copyright (c)**************************************************
**																����ʦ����ѧ��������ԴѧԺ
**------------------------------------------------------------------------------------------------------
** �ļ�: system.c 
** �汾: v1.0
**------------------------------------------------------------------------------------------------------
** ����:
**      ϵͳ�ĳ�ʼ��
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "system.h"

#define WDT_DEVICE_NAME    "wdt"    /* ���Ź��豸���� */

static rt_device_t wdg_dev;         /* ���Ź��豸��� */

static void idle_hook(void)
{
    /* �ڿ����̵߳Ļص�������ι�� */
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
//    rt_kprintf("feed the dog!\n ");
}

/******************************************************************************************************
 * ���Ź��豸��ʼ��
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
	/* ��ʼ���豸 */
	ret = rt_device_init(wdg_dev);
	if (ret != RT_EOK)
	{
			rt_kprintf("initialize %s failed!\n", WDT_DEVICE_NAME);
	}
	/* ���ÿ��Ź����ʱ�� */
	ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout);   //���ʱ��  25s
	if (ret != RT_EOK)
	{
			rt_kprintf("set %s timeout failed!\n", WDT_DEVICE_NAME);
	}
	/* �������Ź� */
	ret = rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL);
	if (ret != RT_EOK)
	{
			rt_kprintf("start %s failed!\n", WDT_DEVICE_NAME);
	}
	/* ���ÿ����̻߳ص����� */
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
** ����: SysInit���ϵ��ʼ��
**------------------------------------------------------------------------------------------------------
** ����: void
** ����: ��
********************************************************************************************************/
void SysInit(void)
{
	SysTickInit();				        	 // ϵͳ�δ�ʱ����ʼ������
  ESP8266_Init();               	 // esp8266ģ�����ţ�����2��ʼ����������115200;
  RTC_Init();                      // RTC��ʼ��
	WDT_Init();                      // ���Ź��豸��ʼ��
	TaskInit();    									 // �߳������ʼ��
}

static void ResetSystem(void)
{
  SysInit();   //��ʼ��
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(ResetSystem, Reset System);

/********************************************************************************************************
**                            End Of File
********************************************************************************************************/

