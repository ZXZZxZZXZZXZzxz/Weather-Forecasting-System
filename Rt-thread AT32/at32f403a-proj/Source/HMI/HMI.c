/***********************************************************************************************************************
**
**
**
************************************************************************************************************************/
#include "config.h"
#include "HMI.h"



//��������ҳ���ʶ
char g_page[32] = {0};
char g_update[32]={0};  //ˢ��ָ��
//����һЩȫ�ֱ��������ڽ��մ������·�������
char g_place[32] = {0};
char g_place_utf8[32] = {0};
char g_city[32] = "beijing";
int  utf8_len = 0;
char g_wifi_ssid[32] = "";
char g_wifi_password[32] = "";

//�洢ʱ������
char g_time_hour[8]={0};
char g_time_min[8]={0};
char g_time_sec[8]={0};
//���ַ���������Ԫ�صĸ���
static int ForLenth(char *buffer,int len)
{
	unsigned char i;
	int lenth=0;
  for(i=0;i<len;i++)
	{
	  if(buffer[i]!='\0')
			lenth++;
	}
	return lenth;
}
/********************************************************************************************************
** ����: Task_SendHMI, ��������������Һ��������������Deskҳ��
**------------------------------------------------------------------------------------------------------
** ����: weather����ȡ������������
** ����: ��		
** ˵��: ����˵������Desk.DeskPlaceTxt.txt����DeskΪҳ�����ƣ�DeskPlaceTxtΪ�ı��ؼ����ƣ�txtΪ�ؼ���txt���ԡ�
**		����ҳ��������ؼ������ǿɸ��ĵģ����������ǲ��ɸ��ĵġ�
********************************************************************************************************/
void Task_SendHMI_WeaData(WeatherType weather)  //����������
{
	 char HMI_Buffer[100]={0};             //���ݻ�����
	 int Lenth=0;                         //����
	 char endstr[3]={0xFF,0xFF,0xFF};     //������
	 
	 if(strcmp(g_page,"page_Desk")==0)    //�ַ����Ƚ�  ��ȷ���0  ����
		{	
			 sprintf(HMI_Buffer,"Desk.DeskPlaceTxt.txt=\"%s\"", weather.name);                   //����
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);   																													 //��ʱ100ms
			
			 sprintf(HMI_Buffer, "Desk.DeskTem.txt=\"%s��\"", weather.temperature);                //�¶�
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);                                     //��������
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
			
			 rt_thread_mdelay(50);
			 
			 sprintf(HMI_Buffer, "Desk.DeskWeathText.txt=\"%s\"", weather.text);               	 //��������
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);                                     //��������
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
			
			 rt_thread_mdelay(50);
			
			 sprintf(HMI_Buffer, "Desk.DeskHumiTxt.txt=\"%s%%\"", weather.humidity[0]);          //ʪ��
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);                                     //��������
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
			
			 rt_thread_mdelay(50);                                                              //��ʱ100ms
			 
			 sprintf(HMI_Buffer, "Desk.WindDirTxt.txt=\"%s��\"", weather.wind_direction[0]);     //����
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));      																 //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
			
			 rt_thread_mdelay(50);   																														//��ʱ100ms
				
			 sprintf(HMI_Buffer, "Desk.WindScaleTxt.txt=\"%s��\"", weather.wind_scale[0]);				//����
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));      																	//������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																		//��������
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);   //��ʱ100ms
			 
			 sprintf(HMI_Buffer, "Desk.WindSpeedTxt.txt=\"%skm/h\"", weather.wind_speed[0]);      //����
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                       //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);                                      //��������
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));

			 rt_thread_mdelay(50);   																														//��ʱ100ms
				 
			 sprintf(HMI_Buffer, "Desk.LifeSuggestion.txt=\"%sϴ��\r\n%s�˶�\r\n%s����\r\n������%s\"", weather.life_suggestion[0],weather.life_suggestion[3],weather.life_suggestion[4],weather.life_suggestion[5]);         //�����
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                       //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);                                      //��������
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
		   memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
	 }
	 if(strcmp(g_page,"page_DailyWeather")==0)    //��������������ҳ��
	  {
			 /* ���� ���� */
		   sprintf(HMI_Buffer,"DailyWeather.TodayTextday.txt=\"%s\"", weather.text_day[0]);            //�����������
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* ���� ���·�Χ */
			 sprintf(HMI_Buffer,"DailyWeather.TodayTemp.txt=\"%s��/%s��\"",weather.low[0], weather.high[0]);            //�����������
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			/* ���� ҹ�� */
			 sprintf(HMI_Buffer,"DailyWeather.TodayTextnig.txt=\"%s\"", weather.text_night[0]);            //����ҹ������
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* ���� ���� ���� */
			 sprintf(HMI_Buffer,"DailyWeather.TodayWind.txt=\"%s�� %s��\"", weather.wind_direction[0],weather.wind_scale[0]);  //������� ���� 
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 
			 /* ���� ���� */
		   sprintf(HMI_Buffer,"DailyWeather.TorTextday.txt=\"%s\"", weather.text_day[1]);            //�����������
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* ���� ���·�Χ */
			 sprintf(HMI_Buffer,"DailyWeather.TorTemp.txt=\"%s��/%s��\"",weather.low[1], weather.high[1]);            //�����������
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			/* ���� ҹ�� */
			 sprintf(HMI_Buffer,"DailyWeather.TorTextnig.txt=\"%s\"", weather.text_night[1]);            //����ҹ������
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* ���� ���� ���� */
			 sprintf(HMI_Buffer,"DailyWeather.TorWind.txt=\"%s�� %s��\"", weather.wind_direction[1],weather.wind_scale[1]);  //������� ���� 
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 
			 /* ���� ���� */
		   sprintf(HMI_Buffer,"DailyWeather.AfTorTextday.txt=\"%s\"", weather.text_day[2]);            //�����������
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* ���� ���·�Χ */
			 sprintf(HMI_Buffer,"DailyWeather.AfterTorTemp.txt=\"%s��/%s��\"", weather.low[1], weather.high[1]);            //�����������
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			/* ���� ҹ�� */
			 sprintf(HMI_Buffer,"DailyWeather.AfTorTextnig.txt=\"%s\"", weather.text_night[2]);            //����ҹ������
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* ���� ���� ���� */
			 sprintf(HMI_Buffer,"DailyWeather.AftorWind.txt=\"%s�� %s��\"", weather.wind_direction[2],weather.wind_scale[2]);  //������� ���� 
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //������Ԫ�ظ���
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //��������
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
	  }
	 memset(s_sUsartBuf3, 0, sizeof(s_sUsartBuf3));                                       //�������᷵�����ݣ��������3���յ�����
	 RECPos3=0;
}
static void Update_HMI(void)
{
   Task_SendHMI_WeaData(weather_data);
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(Update_HMI,Send Data To The HMI);  

/*  ��������ͼ�� */
static char SearchWeaIconID(char *weather_code)
{
		char Icon_ID;												 //ͼ��ID
  /* �����ڴ��������ڲ�flash��Դ��������ñȽϱ��ķ�����������ͼ���ID */
		if(atoi(weather_code)==0||atoi(weather_code)==2)
			Icon_ID=11;       //������
		if(atoi(weather_code)==1||atoi(weather_code)==3)
			Icon_ID=12;       //ҹ����
		else if(atoi(weather_code)==5||atoi(weather_code)==7||atoi(weather_code)==4)
			Icon_ID=13;       //�������
		else if(atoi(weather_code)==6||atoi(weather_code)==8)
			Icon_ID=14;       //ҹ�����
		else if(atoi(weather_code)==9)
			Icon_ID=15;       //��
		else if(atoi(weather_code)>=10&&atoi(weather_code)<=11)
			Icon_ID=16;       //��������
		else if(atoi(weather_code)==13)
			Icon_ID=17;				// С��
		else if(atoi(weather_code)==14)
			Icon_ID=18;				//����
		else if(atoi(weather_code)>=15&&atoi(weather_code)<=18)
			Icon_ID=19;				//����
		else if(atoi(weather_code)==21)
			Icon_ID=20;       //������ѩ
		else if(atoi(weather_code)==22)
			Icon_ID=21;       //Сѩ
		else if(atoi(weather_code)==23)
			Icon_ID=22;       //��ѩ
		else if(atoi(weather_code)>=24&&atoi(weather_code)<=25)
			Icon_ID=23;       //��ѩ
		else if(atoi(weather_code)==26)
			Icon_ID=24;       //����
		else if(atoi(weather_code)==27)
			Icon_ID=25;       //��ɳ
		else if(atoi(weather_code)==30)
			Icon_ID=26;       //��
		else if(atoi(weather_code)==31)
			Icon_ID=27;       //��
		else if(atoi(weather_code)==34)
			Icon_ID=28;       //쫷�
		else if(atoi(weather_code)==19)
			Icon_ID=29;       //����
		return Icon_ID;
}
/********************************************************************************************************
** ����: DisplayWeatherIcon, ��������ͼ����������
**------------------------------------------------------------------------------------------------------
** ����: weather����ȡ������������
** ����: ��		
** ˵��: ˢͼָ�pic x,y,picid  (x:��ʼ��x���꣬y:��ʼ��y���꣬picid:ͼƬID)
********************************************************************************************************/

void DisplayWeatherIcon(WeatherType weather)
{
	char icon_buf[32] = {0};						 //���ݻ�����
	int Lenth=0;                         //����
	char endstr[3]={0xFF,0xFF,0xFF};     //������
	char Icon_ID;												 //ͼ��ID
	memset(icon_buf, '\0', sizeof(icon_buf));
	
	if(strcmp(g_page,"page_Desk")==0)    //�ַ����Ƚ�  ��ȷ���0  ����
	{
	  Icon_ID=SearchWeaIconID(weather.code);   //����ͼ��ID
		sprintf(icon_buf, "pic %d,%d,%d", Now_Icon_X, Now_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //������Ԫ�ظ���
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //��������
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
	}
	if(strcmp(g_page,"page_DailyWeather")==0)    //��������������ҳ��
	{
		/* ���� ���� */
		Icon_ID=SearchWeaIconID(weather.code_day[0]);   //����ͼ��ID
		sprintf(icon_buf, "pic %d,%d,%d", Today_day_Icon_X, Today_day_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //������Ԫ�ظ���
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //��������
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
		/* ���� ҹ�� */
		Icon_ID=SearchWeaIconID(weather.code_night[0]);   //����ͼ��ID
		if(Icon_ID==13) Icon_ID+=1;			//ҹ�����
		else if(Icon_ID==16) Icon_ID=32;//ҹ������
		else if(Icon_ID==29) Icon_ID=31;//ҹ����ѩ
		sprintf(icon_buf, "pic %d,%d,%d", Today_night_Icon_X, Today_night_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //������Ԫ�ظ���
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //��������
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
		
		/* ���� ���� */
		Icon_ID=SearchWeaIconID(weather.code_day[1]);   //����ͼ��ID
		sprintf(icon_buf, "pic %d,%d,%d", Tomorrow_day_Icon_X, Tomorrow_day_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //������Ԫ�ظ���
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //��������
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
		/* ���� ҹ�� */
		Icon_ID=SearchWeaIconID(weather.code_night[1]);   //����ͼ��ID
	  if(Icon_ID==13) Icon_ID+=1;     //ҹ�����
		else if(Icon_ID==16) Icon_ID=32;//ҹ������
		else if(Icon_ID==29) Icon_ID=31;//ҹ����ѩ
		sprintf(icon_buf, "pic %d,%d,%d", Tomorrow_night_Icon_X, Tomorrow_night_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //������Ԫ�ظ���
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //��������
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
		
		/* ���� ���� */
		Icon_ID=SearchWeaIconID(weather.code_day[2]);   //����ͼ��ID
		sprintf(icon_buf, "pic %d,%d,%d", AfterTomor_day_Icon_X, AfterTomor_day_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //������Ԫ�ظ���
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //��������
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
		/* ���� ҹ�� */
		Icon_ID=SearchWeaIconID(weather.code_night[2]);   //����ͼ��ID
		if(Icon_ID==13) Icon_ID+=1;			//ҹ�����
		else if(Icon_ID==16) Icon_ID=32;//ҹ������
		else if(Icon_ID==29) Icon_ID=31;//ҹ����ѩ
		sprintf(icon_buf, "pic %d,%d,%d", AfterTomor_night_Icon_X, AfterTomor_night_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //������Ԫ�ظ���
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //��������
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
	}
}
static void DisplayNowWeatherTcon(void)
{
  DisplayWeatherIcon(weather_data);
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(DisplayNowWeatherTcon,Display Weather Icon); 

/********************************************************************************************************
** ����: DisplayWifiIcon, ����WIFIͼ����������
**------------------------------------------------------------------------------------------------------
** ����: weather����ȡ������������
** ����: ��		
** ˵��: ˢͼָ�pic x,y,picid  (x:��ʼ��x���꣬y:��ʼ��y���꣬picid:ͼƬID)
********************************************************************************************************/
void DisplayWifiIcon(unsigned char sign)
{
		char icon_buf[32] = {0};						 //���ݻ�����
		int Lenth=0;                         //����
		char endstr[3]={0xFF,0xFF,0xFF};     //������
		char Icon_ID;
	if(sign)     //������
    Icon_ID=10;
	else                      //�Ͽ�
		Icon_ID=9;
		if(strcmp(g_page,"page_Desk")==0)    //�ַ����Ƚ�  ��ȷ���0  ����
	{
		sprintf(icon_buf, "pic %d,%d,%d", WIFI_Icon_X, WIFI_Icon_Y, Icon_ID);                  
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //������Ԫ�ظ���
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //��������
	  rt_device_write(serial3, 0, endstr, 3);
	}
}
static void DISPLAYWIFITCON(void)
{
  DisplayWifiIcon(WIFI_connect_Sign);
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(DISPLAYWIFITCON,Display WIFI Icon);

/********************************************************************************************************
** ����: DisplayRTCTime, ����ʱ����������
**------------------------------------------------------------------------------------------------------
** ����: weather����ȡ������������
** ����: ��		
********************************************************************************************************/
void DisplayRTCTime(Calendar_T calendar)
{
	char time_buf[36]={0};
	int Lenth=0;                         //����
  char endstr[3]={0xFF,0xFF,0xFF};     //������
		
	sprintf(time_buf, "Desk.DeskDateTxt.txt=\"%.4d-%.2d-%.2d\"",calendar.w_year ,calendar.w_month ,calendar.w_date );   //����
	Lenth=ForLenth(time_buf,sizeof(time_buf));                                      //������Ԫ�ظ���
	rt_device_write(serial3, 0, time_buf, Lenth);                                     //��������
	rt_device_write(serial3, 0, endstr, 3);
	memset(time_buf, 0, sizeof(time_buf));

	rt_thread_mdelay(50);
	
	sprintf(time_buf, "Desk.DeskTimeTxt.txt=\"%.2d:%.2d:%.2d\"", calendar.hour,calendar.min,calendar.sec);             //ʱ��           
	Lenth=ForLenth(time_buf,sizeof(time_buf));                                      //������Ԫ�ظ���
	rt_device_write(serial3, 0, time_buf, Lenth);  																	 //��������
	rt_device_write(serial3, 0, endstr, 3);
	
  memset(time_buf,0,sizeof(time_buf));
	
	sprintf(time_buf, "Desk.DeskWeek.txt=\"%s\"", wday[calendar.week]);                                            //����
	Lenth=ForLenth(time_buf,sizeof(time_buf));                                      //������Ԫ�ظ���
	rt_device_write(serial3, 0, time_buf, Lenth);  																	 //��������
	rt_device_write(serial3, 0, endstr, 3);
}
/* ������ msh �����б��� */
MSH_CMD_EXPORT(DisplayRTCTime,Display Rtc Time);

/*************************************************
*������  DisplayLoadIcon      ����ͼ��
*������  sign     1 ������      0�������
**************************************************/
static void DisplayLoadIcon(unsigned char sign)
{
	char Load_buf[36]={0};
	int Lenth=0;                         //����
  char endstr[3]={0xFF,0xFF,0xFF};     //������
  if(sign)
	{
	  sprintf(Load_buf, "pic %d,%d,%d",Load_Icon_X,Load_Icon_Y,43);                        
	  Lenth=ForLenth(Load_buf,sizeof(Load_buf));                                      //������Ԫ�ظ���
	  rt_device_write(serial3, 0, Load_buf, Lenth);  																	 //��������
	  rt_device_write(serial3, 0, endstr, 3);
	}
	else
	{
	  sprintf(Load_buf, "pic %d,%d,%d",Load_Icon_X,Load_Icon_Y,44);                      
	  Lenth=ForLenth(Load_buf,sizeof(Load_buf));                                      //������Ԫ�ظ���
	  rt_device_write(serial3, 0, Load_buf, Lenth);  																	 //��������
	  rt_device_write(serial3, 0, endstr, 3);
	}

}
/*************************************************************************************
**�������·������ݴ�����
**
**
**************************************************************************************/
void HMI_RePrecess(void)
{
	char *p = NULL;
	/* ҳ���ʶ */
	if((p=strstr((char*)s_sUsartBuf3,"page:"))!=NULL)
	{
		if(sscanf(p,"page:%s",g_page)==1)
		{
			printf("page:%s\n",g_page);
			
			DisplayWeatherIcon(weather_data);      //����ͼ��
			Task_SendHMI_WeaData(weather_data);    //��������
			
			if(strcmp(g_page,"page_Desk")==0)
			{
				DisplayWifiIcon(WIFI_connect_Sign);  //WIFIͼ��
			}
		}
	}
	/* ˢ�� ���»�ȡ��������*/
	if((p=strstr((char*)s_sUsartBuf3,"update:"))!=NULL)
	{
		if(sscanf(p,"update:%s",g_update)==1)
		{
			printf("update:%s\n",g_update);
			
			 DisplayLoadIcon(1);     //��ʾ����ͼ��
			
			 GetTest();            //����
			 GetNowWeather();      //����ʵ����������
			 GetDailyWeather();  //����δ��������������
			 GetLifeSuggestion(); //�������������
			 AccessNTP();   //��ȡʱ��
			
			 DisplayLoadIcon(0);     //�رռ���ͼ��
		}				
	}
	/* wifi���� */
	 if((p=strstr((char*)s_sUsartBuf3,"wifi_ssid:"))!=NULL)
	{
		if(sscanf(p,"wifi_ssid:%s",g_wifi_ssid)==1)
		{
			printf("wifi_ssid:%s\n",g_wifi_ssid);
		}
	}
	/* wifi���� */
  if((p=strstr((char*)s_sUsartBuf3,"wifi_password:"))!=NULL)
	{
		if(sscanf(p,"wifi_password:%s",g_wifi_password)==1)
		{
			printf("wifi_password:%s\n",g_wifi_password);
			WIFI_connect_Sign = ESP8266_JoinAP(g_wifi_ssid, g_wifi_password);   //����WIFI
			printf("g_connect_sta = %d\n",WIFI_connect_Sign);
			DisplayWifiIcon(WIFI_connect_Sign);
			if(WIFI_connect_Sign)
			{
				printf("WIFI Connect Successful!\n");
				
				rt_sem_release(GetWea_sem);    //WIFI������ ��ȡ���� �ͷ��ź���   GetWeather_thread_entry�߳���ں�������ȡ�������ݣ��ȴ�����
			}
			else 
				printf("WIFI Connect Failed!\n");
		}
	}
	/* �������� */
  if((p=strstr((char*)s_sUsartBuf3,"place:"))!=NULL)
	{
		if(sscanf(p,"place:%s",g_place)==1)
		{
			 SwithToUtf_8((const unsigned char*)g_place, strlen((const char*)g_place),(unsigned char*) g_place_utf8, &utf8_len);
			 printf("place:%s\n",g_place_utf8);
			 printf("place:%X%X%X %X%X%X\n",g_place_utf8[0],g_place_utf8[1],g_place_utf8[2],
			 g_place_utf8[3],g_place_utf8[4],g_place_utf8[5]);
			 sprintf(g_city,"%%%X%%%X%%%X%%%X%%%X%%%X",g_place_utf8[0],g_place_utf8[1],
			 g_place_utf8[2],g_place_utf8[3],g_place_utf8[4],g_place_utf8[5]);		
			
			 memset(&weather_data,0,sizeof(weather_data)); //���
			
			 DisplayLoadIcon(1);     //��ʾ����ͼ��
			
			 GetTest();            //����
			 GetNowWeather();      //����ʵ����������
			 GetDailyWeather();  //����δ��������������
			 GetLifeSuggestion(); //�������������
			 AccessNTP();   //��ȡʱ��
			
			 DisplayLoadIcon(0);     //�رռ���ͼ��
		}
	}
	/* ����ʱ�� */
	if((p=strstr((char*)s_sUsartBuf3,"Time_hour:"))!=NULL)   //ʱ
	{
		if(sscanf(p,"Time_hour:%s",g_time_hour)==1)
		{
			printf("Time_hour:%s\n",g_time_hour);
			RTC_Get();
			RTC_Set(calendar_t.w_year ,calendar_t.w_month ,calendar_t.w_date ,atoi(g_time_hour),calendar_t.min ,calendar_t.sec );
		}
	}
	else if((p=strstr((char*)s_sUsartBuf3,"Time_min:"))!=NULL) //��
	{
	  if(sscanf(p,"Time_min:%s",g_time_min)==1)
		{
			printf("Time_min:%s\n",g_time_min);
			RTC_Get();
			RTC_Set(calendar_t.w_year ,calendar_t.w_month ,calendar_t.w_date ,calendar_t.hour, atoi(g_time_min),calendar_t.sec );
		}
	}
	else if((p=strstr((char*)s_sUsartBuf3,"Time_sec:"))!=NULL)  //��
	{
	  if(sscanf(p,"Time_sec:%s",g_time_sec)==1)
		{
			printf("Time_sec:%s\n",g_time_sec);
			RTC_Get();
			RTC_Set(calendar_t.w_year ,calendar_t.w_month ,calendar_t.w_date ,calendar_t.hour, calendar_t.min,atoi(g_time_sec) );
		}
	}
	memset(s_sUsartBuf3, 0, sizeof(s_sUsartBuf3));
	RECPos3=0;
	p = NULL;
}

/****************************************************************************************************************
**                                              End Of File
**
*****************************************************************************************************************/

