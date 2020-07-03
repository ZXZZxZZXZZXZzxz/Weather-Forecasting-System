#include "config.h"
#include "Task.h"

/*************************************************************************************
*                                     ��������
**************************************************************************************/
static rt_thread_t dynamic_thread=RT_NULL;  //��̬�߳̿��ƿ�ָ��
/* �豸 */
 rt_device_t serial3;     									/* �����豸  ����3 */

 /* �ź��� */
 rt_sem_t usart3_rx_sem;   									/* ���ڽ��մ���3��Ϣ���ź��� */
 rt_sem_t espwifi_getweath; 								/* ���������ź��� */
 rt_sem_t HMIRePre_sem;     								/* �����������ź��� */
 rt_sem_t GetWea_sem;     								/* ������ˢ�����������ź��� */
 
 /* �����ʱ�� */
 rt_timer_t GetNTPSoftTimer;
 
 /************************************************************************************
 *                               �����ʱ���ص�����
 ************************************************************************************/
 void GetNTPSoftTimer_CallBack(void *paramter);  //NTP�ص�����
/*************************************************************************************
*                                �߳���ں�������
**************************************************************************************/
void serial3_thread_entry(void *parameter);      //����3 ��ں���
void GetWeather_thread_entry(void *parameter);   //����������ں���
void HMI_thread_entry(void *parameter);          //���´�������ں���
void NTP_thread_entry(void *parameter);          //����RTC��ں���
void HMIRePre_thread_entry(void *parameter);     //������������ں���

TaskStruct TaskThreads[]={
		{"serial3",     serial3_thread_entry,   RT_NULL, 200, 11, 10},            //����3�豸�߳�
		{"GetWeather",  GetWeather_thread_entry,RT_NULL, 800, 11, 10},            //���������߳�
		{"UpdateHMI",   HMI_thread_entry,       RT_NULL, 5000,11, 10},						//�������߳�  
		{"HMIRePrecess",HMIRePre_thread_entry,  RT_NULL, 6000,11, 10},						//�����������߳�
    {"",RT_NULL,RT_NULL,RT_NULL,RT_NULL,RT_NULL}
/*****************************************************************************/
//�û�����̲߳���
//���磺{�߳����֣��߳���ں������߳���ں����������߳�ջ��С���̵߳����ȼ����߳�ʱ��Ƭ}
};

/***************************************************************************************
*                               �����ʼ��
***************************************************************************************/
void TaskInit(void)
{

  uint8_t TaskThreadIndex=0;
   /* usart3�豸���� */
   serial3_init();    

/*****************************�ź���********************************/
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

/*******************************�����ʱ��*******************************/
	GetNTPSoftTimer=rt_timer_create("GetNTPSoftTimer",   /* �����ʱ������ */
												 GetNTPSoftTimer_CallBack,     /* �����ʱ���ص����� */
												 0,                    				 /* ��ʱ����ʱ������ڲ��� */
											   6*60*60*1000,                 /* �����ʱ���ĳ�ʱʱ�䣨���ڣ�   6Сʱ*/
												 RT_TIMER_FLAG_PERIODIC);      /* �����ʱ��ģʽ   ����ģʽ*/  
		if(GetNTPSoftTimer!=RT_NULL)
   {
			rt_timer_start(GetNTPSoftTimer);  //���������ʱ��
			rt_kprintf("GetNTPSoftTimer Start Successful!\n\r");
   }
/*********************************************************************************/
   while(1)
    {
      if(strcmp(TaskThreads[TaskThreadIndex].name,"")!=0)
        {
          dynamic_thread=rt_thread_create(TaskThreads[TaskThreadIndex].name,        /*�߳�����*/
															 TaskThreads[TaskThreadIndex].entry,  								/*�߳���ں���*/
															 TaskThreads[TaskThreadIndex].parameter,              /*�߳���ں�������*/
															 TaskThreads[TaskThreadIndex].stack_size,             /*�߳�ջ��С */  
															 TaskThreads[TaskThreadIndex].priority,               /*�̵߳����ȼ�*/
															 TaskThreads[TaskThreadIndex].tick);      						/*�߳�ʱ��Ƭ*/
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
 *                                                                          �����ʱ���ص�����
 *******************************************************************************************************************************************************/

/******************************************NTP��ȡ************************************************/
void GetNTPSoftTimer_CallBack(void *paramter)
{
  AccessNTP();
}

/*******************************************************************************************************************************************************
*                                                                             �߳���ں���
******************************************************************************************************************************************************/

/**********************uart3�豸**********************************/
void serial3_thread_entry(void *parameter)
{
    char ch;
    while (1)
    {
        /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(serial3, -1, &ch, 1) != 1)
        {
					/* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
					rt_sem_take(usart3_rx_sem, RT_WAITING_FOREVER);
        }					
				if(RECPos3<RECBUF_MAX-1)
				{
				  s_sUsartBuf3[RECPos3++]=ch;
				  if(s_sUsartBuf3[RECPos3-1]==0x0A)     //����λ��ӦΪ0x0A
					{
						s_sUsartBuf3[RECPos3]='\0';
						rt_sem_release(HMIRePre_sem);      //�ͷŴ����������ź���
					}
				}
    }
}

/*******************************************��������*****************************************************/
void GetWeather_thread_entry(void *parameter)
{
	bool Now_sign,Daily_sign,Life_sign;
	static rt_err_t rwRet=RT_EOK;
	while(1)
	{
    rwRet=rt_sem_take(GetWea_sem,RT_WAITING_FOREVER);   //�ȴ���ȡ�ź���
		if(rwRet==RT_EOK)
		{
			 GetTest();                     //����
			 Now_sign=GetNowWeather();      //����ʵ����������
			 Daily_sign=GetDailyWeather();  //����δ��������������
			 Life_sign=GetLifeSuggestion(); //�������������	 
			 AccessNTP();   								//��ȡʱ��
		}
			if(Daily_sign&Now_sign&Life_sign)  
			{
				/* ���յ��������ݲ������ɹ�������´������̷߳����ź��� */
				rt_sem_release(espwifi_getweath);
				rt_thread_delay(10*60*1000);     //ʮ���ӷ���һ��
			}
			
	}			
}
/*******************************************���´�����***************************************************/
void HMI_thread_entry(void *parameter)
{
	static rt_err_t rwRet;
  while(1)
	{
		rwRet=rt_sem_take(espwifi_getweath, RT_WAITING_FOREVER);//�ȴ���ȡ�ź���
		if(rwRet==RT_EOK)  
		{
			DisplayWeatherIcon(weather_data);     //��������������ͼ��
			Task_SendHMI_WeaData(weather_data);   //����������Deskҳ������
			if(strcmp(g_page,"page_Desk")==0)
			{
				DisplayWifiIcon(WIFI_connect_Sign);//����������WIFIͼ��
				rt_kprintf("Update HMI Succseeful !\r\n");
			}
		}
	}
}

/*******************************************����������****************************************************/
void HMIRePre_thread_entry(void *parameter)
{
	static rt_err_t rwRet;
  while(1)
	{
	  rwRet=rt_sem_take(HMIRePre_sem, RT_WAITING_FOREVER);  //�ȴ���ȡ�ź���
		if(rwRet==RT_EOK)  
		{
	    HMI_RePrecess();       //������
		}
	}
}
/***************************************************************************************
*                                end of file
****************************************************************************************/
