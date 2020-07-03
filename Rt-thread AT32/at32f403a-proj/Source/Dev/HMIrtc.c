#include "HMIrtc.h"
#include "rtc.h"

//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31

Calendar_T calendar_t;	// �����ṹ��

/* �·�x�������ݱ� */											 
uint8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 
/* ƽ����·����ڱ� */
const uint8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
/* �������� */
char *wday[] ={"������", "����һ", "���ڶ�", "������", "������", "������", "������"}; //{ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/********************************************************************************************************
** ����: RTC_NVIC_Config,  RTC�ж����ã���RTC_Init��������
**------------------------------------------------------------------------------------------------------
** ����: ��
** ����: ��											  
********************************************************************************************************/
static void RTC_NVIC_Config(void)
{
	NVIC_InitType NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;				//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 30;	//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 30;			//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);							
}

/********************************************************************************************************
** ����: RTC_Init,  RTC��ʼ��
**------------------------------------------------------------------------------------------------------
** ����: ��
** ����: 0���ɹ�����������ʧ�ܣ�											  
********************************************************************************************************/
uint8 RTC_Init(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR | RCC_APB1PERIPH_BKP, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCtrl(ENABLE); 

	/* Check Backup data registers is correct*/
	if (BKP_ReadBackupReg(BKP_DT1) != 0x5051)
	{
		/* Reset Backup Domain */
		BKP_Reset();
		
		/* Enable the LSI OSC */
		RCC_LSEConfig(RCC_LSE_ENABLE);
		/* Wait till LSI is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_LSESTBL) == RESET);
		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSelection_LSE);
		
		/* Enable RTC Clock */
		RCC_RTCCLKCmd(ENABLE);
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		
		/* Set RTC prescaler: set RTC period to 1sec */
		RTC_SetDIV(32767);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		
		/* Set the RTC time */
		RTC_Set(2020, 6, 30, 23, 59, 55);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		
		/* Writes data to Backup Register */
		BKP_WriteBackupReg(BKP_DT1, 0x5051);
	}
	else
	{
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		/* Clear RTC pending flag */
		RTC_ClearFlag(RTC_FLAG_PACE);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}
	RTC_INTConfig(RTC_INT_PACE,ENABLE);    //ʹ�����ж�
	RTC_NVIC_Config();                     //�ж�����
	return 0; 									 // ��ʼ���ɹ�
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(RTC_Init,Rtc Init!);
/********************************************************************************************************
** ����: IsLeapYear,  �ж����ꡢƽ��
**------------------------------------------------------------------------------------------------------
** ����: year
** ����: 0��ƽ�꣩��1�����꣩		
** ע��: �����ж�����:1.�ܱ�4���������ܱ�100������2.�ܱ�4���������ܱ�400����
********************************************************************************************************/
uint8 IsLeapYear(uint16 year)
{
	if( year%4==0 )
	{
		if( year%100==0 )
		{
			if( year%400==0 )
			{
				return 1;//����
			}
			else
			{
				return 0;//ƽ��
			}
		}
		else
		{
			return 1;	 //����
		}
	}
	else
	{
		return 0;        //ƽ��
	}
}

/********************************************************************************************************
** ����: RTC_Get,  �õ���ǰ��ʱ��
**------------------------------------------------------------------------------------------------------
** ����: ��
** ����: 0���ɹ���������������											  
********************************************************************************************************/
uint8 RTC_Get(void)
{
	static uint16 daycnt = 0;
	uint32 rtc_count = 0;    		// ���ڽ���RTC������ֵ
	uint32 day_num = 0;		 		// 1970��1��1��������˶�����
	uint16 year = 0;				// ����
	
    rtc_count = RTC_GetCounter();	// ��ȡRTC��������ֵ
 	day_num = rtc_count/86400;      // �õ�����(��������Ӧ��)
	if( daycnt!=day_num )			// ����һ����
	{	  
		daycnt = day_num;
		year = 1970;				// ��1970�꿪ʼ
		while( day_num >= 365 )		// ����������1��ͼ����
		{				 
			if( IsLeapYear(year) )  // 1�����꣬0��ƽ��
			{
				if( day_num >= 366 )
				{
					day_num -= 366; //��������ȥ����366��֮��������1
				}
				else 
				{
					year++;
					break;
				}  
			}
			else day_num -= 365;    // ��������ȥƽ��365��֮��������1
			year++;  
		}   
		calendar_t.w_year = year;	// �õ����
		year = 0;
		while(day_num>=28)			// ������һ���¾ͼ��·�
		{
			if( IsLeapYear(calendar_t.w_year)&&year==1 )//�����ǲ�������2�·�
			{
				if( day_num >= 29 )
					day_num -= 29;	//��ȥ��ݵ�����֮�����������ȥ����2��29��֮���·ݼ�1
				else 
					break; 
			}
			else 
			{
				if( day_num >= mon_table[year] )
					day_num -= mon_table[year];//ƽ��
				else break;
			}
			year++;  
		}
		calendar_t.w_month = year+1;	//�õ��·�
		calendar_t.w_date  = day_num+1; //�õ����� 
	}
	day_num=rtc_count%86400;     		//һ����֮�ڵ�������   	   
	calendar_t.hour=day_num/3600;     	//Сʱ
	calendar_t.min=(day_num%3600)/60; 	//����	
	calendar_t.sec=(day_num%3600)%60; 	//����
	calendar_t.week=RTC_GetWeek(calendar_t.w_year,calendar_t.w_month,calendar_t.w_date);//��ȡ����   
	return 0;
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(RTC_Get,Get Rtc Time!);
/********************************************************************************************************
** ����: RTC_GetWeek,  ������������ڼ�
**------------------------------------------------------------------------------------------------------
** ����: uint16 year,uint8 month,uint8 day
** ����: ���ں�											  
********************************************************************************************************/
uint8 RTC_GetWeek(uint16 year,uint8 month,uint8 day)
{
	uint16 week;
	uint8 yearH,yearL;
	
	yearH = year/100;	
	yearL = year%100; 
	// ���Ϊ21����,�������100  
	if ( yearH>19 )
		yearL += 100;
	// ����������ֻ��1900��֮���  
	week = yearL+yearL/4;
	week = week%7; 
	week = week+day+table_week[month-1];
	if( yearL%4==0&&month<3 )
		week--;
	return (week%7);
}

/********************************************************************************************************
** ����: RTC_Set,  RTC��������ʱ�亯������1970��1��1��Ϊ��׼���������ʱ��ת��Ϊ���ӣ�
**------------------------------------------------------------------------------------------------------
** ����: uint16 year,uint8 mon,uint8 day,uint8 hour,uint8 min,uint8 sec
** ����: 0���ɹ�����1��ʧ�ܣ�											  
********************************************************************************************************/
uint8 RTC_Set(uint16 year,uint8 mon,uint8 day,uint8 hour,uint8 min,uint8 sec)
{
	uint16 t;
	uint32 seccount=0;
	
	/* ��1970������������������������ */
	if( year<1970||year>2099 )	
		return 1;	 
	for(t=1970;t<year;t++)	//��������ݵ��������
	{
		if(IsLeapYear(t))
		{
			seccount += 31622400;    //�����������
		}
		else 
		{
			seccount += 31536000;  //ƽ���������
		}
	}
	
	/* ��ĳ��1�������µ������������������·� */
	mon -= 1; //mon_table���������
	for( t=0; t<mon; t++)	   //��ǰ���·ݵ����������
	{
		seccount += (u32)mon_table[t]*86400;
		if(IsLeapYear(year)&&t==1)
		{
			seccount += 86400;//����2�·�����һ���������	
		}			
	}
	
	/* ��ĳ��1������������������������ú��� */
	seccount += (u32)(day-1)*86400;
	
	/* ��ĳ��0ʱ����ǰСʱ������������������Сʱ */
	seccount += (u32)hour*3600;
	
	/* ��ĳСʱ0������ǰ���ӵ����������������÷��� */
    seccount += (u32)min*60;	 
	
	/* ��ĳ����0������ǰ����������������������� */
	seccount += sec;
	
	/* ʹ��PWR��BKP����ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR | RCC_APB1PERIPH_BKP, ENABLE);	 
	PWR_BackupAccessCtrl(ENABLE);// ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(seccount);	// ����RTC��������ֵ
	RTC_WaitForLastTask();	    // �ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	
	return 0;	    
}
void RTC_SET(void)
{
  RTC_Set(2020,6,30,12,0,0);
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(RTC_SET,Set Rtc Time!);

/********************************************************************************************************
** ����: RTC_IRQHandler,  RTC�жϴ�����
**------------------------------------------------------------------------------------------------------
** ����: ��
** ����: ��								  
********************************************************************************************************/
void RTC_IRQHandler(void)
{		 			  								 
		/*  RTC�жϴ��� */
	if (RTC_GetINTStatus(RTC_INT_PACE) != RESET)//�����ж�
	{	
		RTC_Get();		 //����ʱ��  
		//------------------------------------------------------------------------------------------
		//������ʾ
//		printf("RTC Time:%.4d-%.2d-%.2d %.2d:%.2d:%.2d\t%s\r\n",
//		calendar_t.w_year,calendar_t.w_month,calendar_t.w_date,
//		calendar_t.hour,calendar_t.min,calendar_t.sec,wday[calendar_t.week]);
		//------------------------------------------------------------------------------------------
		//��������ʾ
		DisplayRTCTime(calendar_t);
		
		RTC_ClearINTPendingBit(RTC_INT_PACE);// �����ж�
		RTC_WaitForLastTask();	  	    			// �ȴ����һ�ζ�RTC�Ĵ�����д�������
	}				 	   	 
}

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
