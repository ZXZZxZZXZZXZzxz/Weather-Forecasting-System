#include "config.h"
#include "Task.h"

/*************************************************************************************
*                                     变量定义
**************************************************************************************/
static rt_thread_t dynamic_thread=RT_NULL;  //动态线程控制块指针
/* 设备 */
 rt_device_t serial3;     									/* 定义设备  串口3 */

 /* 信号量 */
 rt_sem_t usart3_rx_sem;   									/* 用于接收串口3消息的信号量 */
 rt_sem_t espwifi_getweath; 								/* 访问天气信号量 */
 rt_sem_t HMIRePre_sem;     								/* 串口屏处理信号量 */
 rt_sem_t GetWea_sem;     								/* 串口屏刷新天气数据信号量 */
 
 /* 软件定时器 */
 rt_timer_t GetNTPSoftTimer;
 
 /************************************************************************************
 *                               软件定时器回调函数
 ************************************************************************************/
 void GetNTPSoftTimer_CallBack(void *paramter);  //NTP回调函数
/*************************************************************************************
*                                线程入口函数声明
**************************************************************************************/
void serial3_thread_entry(void *parameter);      //串口3 入口函数
void GetWeather_thread_entry(void *parameter);   //访问天气入口函数
void HMI_thread_entry(void *parameter);          //更新串口屏入口函数
void NTP_thread_entry(void *parameter);          //更新RTC入口函数
void HMIRePre_thread_entry(void *parameter);     //串口屏处理入口函数

TaskStruct TaskThreads[]={
		{"serial3",     serial3_thread_entry,   RT_NULL, 200, 11, 10},            //串口3设备线程
		{"GetWeather",  GetWeather_thread_entry,RT_NULL, 800, 11, 10},            //访问天气线程
		{"UpdateHMI",   HMI_thread_entry,       RT_NULL, 5000,11, 10},						//串口屏线程  
		{"HMIRePrecess",HMIRePre_thread_entry,  RT_NULL, 6000,11, 10},						//串口屏处理线程
    {"",RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL}
/*****************************************************************************/
//用户添加线程参数
//例如：{线程名字，线程入口函数，线程入口函数参数，线程栈大小，线程的优先级，线程时间片}
};

/***************************************************************************************
*                               任务初始化
***************************************************************************************/
void TaskInit(void)
{

  uint8_t TaskThreadIndex=0;
   /* usart3设备操作 */
   serial3_init();    

/*****************************信号量********************************/
	/* espwifi_getweath */
	espwifi_getweath=rt_sem_create("espwifi_getweath", 0, RT_IPC_FLAG_FIFO);
	if(espwifi_getweath!=RT_NULL)
	{
		rt_kprintf("\nCreate espwifi_getweath Successful !\n\n");
	}
	/* HMIRePre_sem */
	HMIRePre_sem=rt_sem_create("HMIRePre_sem", 0, RT_IPC_FLAG_FIFO);
		if(HMIRePre_sem!=RT_NULL)
	{
		rt_kprintf("\nCreate HMIRePre_sem Successful !\n\n");
	}
	/* GetWea_sem */
	GetWea_sem=rt_sem_create("GetWea_sem", 0, RT_IPC_FLAG_FIFO);
		if(GetWea_sem!=RT_NULL)
	{
		rt_kprintf("\nCreate GetWea_sem Successful !\n\n");
	}

/*******************************软件定时器*******************************/
	GetNTPSoftTimer=rt_timer_create("GetNTPSoftTimer",   /* 软件定时器名称 */
												 GetNTPSoftTimer_CallBack,     /* 软件定时器回调函数 */
												 0,                    				 /* 定时器超时函数入口参数 */
											   6*60*60*1000,                 /* 软件定时器的超时时间（周期）   6小时*/
												 RT_TIMER_FLAG_PERIODIC);      /* 软件定时器模式   周期模式*/  
		if(GetNTPSoftTimer!=RT_NULL)
   {
			rt_timer_start(GetNTPSoftTimer);  //启动软件定时器
			rt_kprintf("GetNTPSoftTimer Start Successful!\n\r");
   }
/*********************************************************************************/
   while(1)
    {
      if(strcmp(TaskThreads[TaskThreadIndex].name,"")!=0)
        {
          dynamic_thread=rt_thread_create(TaskThreads[TaskThreadIndex].name,        /*线程名字*/
															 TaskThreads[TaskThreadIndex].entry,  								/*线程入口函数*/
															 TaskThreads[TaskThreadIndex].parameter,              /*线程入口函数参数*/
															 TaskThreads[TaskThreadIndex].stack_size,             /*线程栈大小 */  
															 TaskThreads[TaskThreadIndex].priority,               /*线程的优先级*/
															 TaskThreads[TaskThreadIndex].tick);      						/*线程时间片*/
          if(dynamic_thread!=RT_NULL)
            {
               rt_thread_startup(dynamic_thread);
            }
           TaskThreadIndex++;
        }
        else break;
    }
}

 /***************************************************************************************************************************************************
 *                                                                          软件定时器回调函数
 *******************************************************************************************************************************************************/

/******************************************NTP获取************************************************/
void GetNTPSoftTimer_CallBack(void *paramter)
{
  AccessNTP();
}

/*******************************************************************************************************************************************************
*                                                                             线程入口函数
******************************************************************************************************************************************************/

/**********************uart3设备**********************************/
void serial3_thread_entry(void *parameter)
{
    char ch;
    while (1)
    {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(serial3, -1, &ch, 1) != 1)
        {
					/* 阻塞等待接收信号量，等到信号量后再次读取数据 */
					rt_sem_take(usart3_rx_sem, RT_WAITING_FOREVER);
        }					
				if(RECPos3<RECBUF_MAX-1)
				{
				  s_sUsartBuf3[RECPos3++]=ch;
				  if(s_sUsartBuf3[RECPos3-1]==0x0A)     //结束位对应为0x0A
					{
						s_sUsartBuf3[RECPos3]='\0';
						rt_sem_release(HMIRePre_sem);      //释放串口屏处理信号量
					}
				}
    }
}

/*******************************************访问天气*****************************************************/
void GetWeather_thread_entry(void *parameter)
{
	bool Now_sign,Daily_sign,Life_sign;
	static rt_err_t rwRet=RT_EOK;
	while(1)
	{
    rwRet=rt_sem_take(GetWea_sem,RT_WAITING_FOREVER);   //等待获取信号量
		if(rwRet==RT_EOK)
		{
			 GetTest();                     //测试
			 Now_sign=GetNowWeather();      //访问实况天气数据
			 Daily_sign=GetDailyWeather();  //访问未来三天天气数据
			 Life_sign=GetLifeSuggestion(); //访问生活建议数据	 
			 AccessNTP();   								//获取时间
		}
			if(Daily_sign&Now_sign&Life_sign)  
			{
				/* 接收到天气数据并解析成功后，向更新串口屏线程发送信号量 */
				rt_sem_release(espwifi_getweath);
				rt_thread_delay(10*60*1000);     //十分钟访问一次
			}
			
	}			
}
/*******************************************更新串口屏***************************************************/
void HMI_thread_entry(void *parameter)
{
	static rt_err_t rwRet;
  while(1)
	{
		rwRet=rt_sem_take(espwifi_getweath, RT_WAITING_FOREVER);//等待获取信号量
		if(rwRet==RT_EOK)  
		{
			DisplayWeatherIcon(weather_data);     //串口屏更新天气图标
			Task_SendHMI_WeaData(weather_data);   //串口屏更新Desk页面数据
			if(strcmp(g_page,"page_Desk")==0)
			{
				DisplayWifiIcon(WIFI_connect_Sign);//串口屏更新WIFI图标
				rt_kprintf("Update HMI Succseeful !\r\n");
			}
		}
	}
}

/*******************************************串口屏处理****************************************************/
void HMIRePre_thread_entry(void *parameter)
{
	static rt_err_t rwRet;
  while(1)
	{
	  rwRet=rt_sem_take(HMIRePre_sem, RT_WAITING_FOREVER);  //等待获取信号量
		if(rwRet==RT_EOK)  
		{
	    HMI_RePrecess();       //处理函数
		}
	}
}
/***************************************************************************************
*                                end of file
****************************************************************************************/
