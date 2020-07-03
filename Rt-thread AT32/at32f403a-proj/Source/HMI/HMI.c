/***********************************************************************************************************************
**
**
**
************************************************************************************************************************/
#include "config.h"
#include "HMI.h"



//串口屏的页面标识
char g_page[32] = {0};
char g_update[32]={0};  //刷新指令
//定义一些全局变量，用于接收串口屏下发的数据
char g_place[32] = {0};
char g_place_utf8[32] = {0};
char g_city[32] = "beijing";
int  utf8_len = 0;
char g_wifi_ssid[32] = "";
char g_wifi_password[32] = "";

//存储时间数据
char g_time_hour[8]={0};
char g_time_min[8]={0};
char g_time_sec[8]={0};
//求字符型数组中元素的个数
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
** 函数: Task_SendHMI, 更新天气数据至液晶触摸串口屏的Desk页面
**------------------------------------------------------------------------------------------------------
** 参数: weather：获取到的天气数据
** 返回: 无		
** 说明: 举例说明：“Desk.DeskPlaceTxt.txt”中Desk为页面名称，DeskPlaceTxt为文本控件名称，txt为控件的txt属性。
**		其中页面名称与控件名称是可更改的，属性名称是不可更改的。
********************************************************************************************************/
void Task_SendHMI_WeaData(WeatherType weather)  //串口屏更新
{
	 char HMI_Buffer[100]={0};             //数据缓存区
	 int Lenth=0;                         //长度
	 char endstr[3]={0xFF,0xFF,0xFF};     //结束符
	 
	 if(strcmp(g_page,"page_Desk")==0)    //字符串比较  相等返回0  桌面
		{	
			 sprintf(HMI_Buffer,"Desk.DeskPlaceTxt.txt=\"%s\"", weather.name);                   //地名
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);   																													 //延时100ms
			
			 sprintf(HMI_Buffer, "Desk.DeskTem.txt=\"%s℃\"", weather.temperature);                //温度
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);                                     //发送数据
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
			
			 rt_thread_mdelay(50);
			 
			 sprintf(HMI_Buffer, "Desk.DeskWeathText.txt=\"%s\"", weather.text);               	 //天气文字
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);                                     //发送数据
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
			
			 rt_thread_mdelay(50);
			
			 sprintf(HMI_Buffer, "Desk.DeskHumiTxt.txt=\"%s%%\"", weather.humidity[0]);          //湿度
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);                                     //发送数据
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
			
			 rt_thread_mdelay(50);                                                              //延时100ms
			 
			 sprintf(HMI_Buffer, "Desk.WindDirTxt.txt=\"%s风\"", weather.wind_direction[0]);     //风向
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));      																 //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
			
			 rt_thread_mdelay(50);   																														//延时100ms
				
			 sprintf(HMI_Buffer, "Desk.WindScaleTxt.txt=\"%s级\"", weather.wind_scale[0]);				//风力
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));      																	//求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																		//发送数据
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);   //延时100ms
			 
			 sprintf(HMI_Buffer, "Desk.WindSpeedTxt.txt=\"%skm/h\"", weather.wind_speed[0]);      //风速
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                       //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);                                      //发送数据
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
			 memset(HMI_Buffer, 0, sizeof(HMI_Buffer));

			 rt_thread_mdelay(50);   																														//延时100ms
				 
			 sprintf(HMI_Buffer, "Desk.LifeSuggestion.txt=\"%s洗车\r\n%s运动\r\n%s旅行\r\n紫外线%s\"", weather.life_suggestion[0],weather.life_suggestion[3],weather.life_suggestion[4],weather.life_suggestion[5]);         //生活建议
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                       //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);                                      //发送数据
			 rt_device_write(serial3, 0, endstr, sizeof(endstr));
		   memset(HMI_Buffer, 0, sizeof(HMI_Buffer));
	 }
	 if(strcmp(g_page,"page_DailyWeather")==0)    //近三日天气数据页面
	  {
			 /* 今天 白天 */
		   sprintf(HMI_Buffer,"DailyWeather.TodayTextday.txt=\"%s\"", weather.text_day[0]);            //当天白天天气
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* 今天 气温范围 */
			 sprintf(HMI_Buffer,"DailyWeather.TodayTemp.txt=\"%s℃/%s℃\"",weather.low[0], weather.high[0]);            //当天白天气温
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			/* 今天 夜晚 */
			 sprintf(HMI_Buffer,"DailyWeather.TodayTextnig.txt=\"%s\"", weather.text_night[0]);            //当天夜晚天气
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* 今天 风向 风力 */
			 sprintf(HMI_Buffer,"DailyWeather.TodayWind.txt=\"%s风 %s级\"", weather.wind_direction[0],weather.wind_scale[0]);  //当天风向 风力 
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 
			 /* 明天 白天 */
		   sprintf(HMI_Buffer,"DailyWeather.TorTextday.txt=\"%s\"", weather.text_day[1]);            //明天白天天气
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* 明天 气温范围 */
			 sprintf(HMI_Buffer,"DailyWeather.TorTemp.txt=\"%s℃/%s℃\"",weather.low[1], weather.high[1]);            //明天白天气温
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			/* 明天 夜晚 */
			 sprintf(HMI_Buffer,"DailyWeather.TorTextnig.txt=\"%s\"", weather.text_night[1]);            //明天夜晚天气
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* 明天 风向 风力 */
			 sprintf(HMI_Buffer,"DailyWeather.TorWind.txt=\"%s风 %s级\"", weather.wind_direction[1],weather.wind_scale[1]);  //明天风向 风力 
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 
			 /* 后天 白天 */
		   sprintf(HMI_Buffer,"DailyWeather.AfTorTextday.txt=\"%s\"", weather.text_day[2]);            //后天白天天气
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* 后天 气温范围 */
			 sprintf(HMI_Buffer,"DailyWeather.AfterTorTemp.txt=\"%s℃/%s℃\"", weather.low[1], weather.high[1]);            //后天白天气温
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			/* 后天 夜晚 */
			 sprintf(HMI_Buffer,"DailyWeather.AfTorTextnig.txt=\"%s\"", weather.text_night[2]);            //后天夜晚天气
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
			 
			 rt_thread_mdelay(50);
			 /* 后天 风向 风力 */
			 sprintf(HMI_Buffer,"DailyWeather.AftorWind.txt=\"%s风 %s级\"", weather.wind_direction[2],weather.wind_scale[2]);  //后天风向 风力 
			 Lenth=ForLenth(HMI_Buffer,sizeof(HMI_Buffer));                                      //求数组元素个数
			 rt_device_write(serial3, 0, HMI_Buffer, Lenth);  																	 //发送数据
			 rt_device_write(serial3, 0, endstr, 3);
			 memset(HMI_Buffer,'\0',sizeof(HMI_Buffer));
	  }
	 memset(s_sUsartBuf3, 0, sizeof(s_sUsartBuf3));                                       //串口屏会返回数据，清掉串口3接收的数据
	 RECPos3=0;
}
static void Update_HMI(void)
{
   Task_SendHMI_WeaData(weather_data);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(Update_HMI,Send Data To The HMI);  

/*  搜索天气图标 */
static char SearchWeaIconID(char *weather_code)
{
		char Icon_ID;												 //图标ID
  /* 受限于串口屏的内部flash资源，这里采用比较笨的方法设置天气图标的ID */
		if(atoi(weather_code)==0||atoi(weather_code)==2)
			Icon_ID=11;       //白天晴
		if(atoi(weather_code)==1||atoi(weather_code)==3)
			Icon_ID=12;       //夜晚晴
		else if(atoi(weather_code)==5||atoi(weather_code)==7||atoi(weather_code)==4)
			Icon_ID=13;       //白天多云
		else if(atoi(weather_code)==6||atoi(weather_code)==8)
			Icon_ID=14;       //夜晚多云
		else if(atoi(weather_code)==9)
			Icon_ID=15;       //阴
		else if(atoi(weather_code)>=10&&atoi(weather_code)<=11)
			Icon_ID=16;       //白天阵雨
		else if(atoi(weather_code)==13)
			Icon_ID=17;				// 小雨
		else if(atoi(weather_code)==14)
			Icon_ID=18;				//中雨
		else if(atoi(weather_code)>=15&&atoi(weather_code)<=18)
			Icon_ID=19;				//大雨
		else if(atoi(weather_code)==21)
			Icon_ID=20;       //白天阵雪
		else if(atoi(weather_code)==22)
			Icon_ID=21;       //小雪
		else if(atoi(weather_code)==23)
			Icon_ID=22;       //中雪
		else if(atoi(weather_code)>=24&&atoi(weather_code)<=25)
			Icon_ID=23;       //大雪
		else if(atoi(weather_code)==26)
			Icon_ID=24;       //浮尘
		else if(atoi(weather_code)==27)
			Icon_ID=25;       //扬沙
		else if(atoi(weather_code)==30)
			Icon_ID=26;       //雾
		else if(atoi(weather_code)==31)
			Icon_ID=27;       //霾
		else if(atoi(weather_code)==34)
			Icon_ID=28;       //飓风
		else if(atoi(weather_code)==19)
			Icon_ID=29;       //冰雹
		return Icon_ID;
}
/********************************************************************************************************
** 函数: DisplayWeatherIcon, 更新天气图标至串口屏
**------------------------------------------------------------------------------------------------------
** 参数: weather：获取到的天气数据
** 返回: 无		
** 说明: 刷图指令：pic x,y,picid  (x:起始点x坐标，y:起始点y坐标，picid:图片ID)
********************************************************************************************************/

void DisplayWeatherIcon(WeatherType weather)
{
	char icon_buf[32] = {0};						 //数据缓存区
	int Lenth=0;                         //长度
	char endstr[3]={0xFF,0xFF,0xFF};     //结束符
	char Icon_ID;												 //图标ID
	memset(icon_buf, '\0', sizeof(icon_buf));
	
	if(strcmp(g_page,"page_Desk")==0)    //字符串比较  相等返回0  桌面
	{
	  Icon_ID=SearchWeaIconID(weather.code);   //搜索图标ID
		sprintf(icon_buf, "pic %d,%d,%d", Now_Icon_X, Now_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //求数组元素个数
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //发送数据
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
	}
	if(strcmp(g_page,"page_DailyWeather")==0)    //近三日天气数据页面
	{
		/* 今天 白天 */
		Icon_ID=SearchWeaIconID(weather.code_day[0]);   //搜索图标ID
		sprintf(icon_buf, "pic %d,%d,%d", Today_day_Icon_X, Today_day_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //求数组元素个数
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //发送数据
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
		/* 今天 夜晚 */
		Icon_ID=SearchWeaIconID(weather.code_night[0]);   //搜索图标ID
		if(Icon_ID==13) Icon_ID+=1;			//夜晚多云
		else if(Icon_ID==16) Icon_ID=32;//夜晚阵雨
		else if(Icon_ID==29) Icon_ID=31;//夜晚阵雪
		sprintf(icon_buf, "pic %d,%d,%d", Today_night_Icon_X, Today_night_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //求数组元素个数
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //发送数据
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
		
		/* 明天 白天 */
		Icon_ID=SearchWeaIconID(weather.code_day[1]);   //搜索图标ID
		sprintf(icon_buf, "pic %d,%d,%d", Tomorrow_day_Icon_X, Tomorrow_day_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //求数组元素个数
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //发送数据
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
		/* 明天 夜晚 */
		Icon_ID=SearchWeaIconID(weather.code_night[1]);   //搜索图标ID
	  if(Icon_ID==13) Icon_ID+=1;     //夜晚多云
		else if(Icon_ID==16) Icon_ID=32;//夜晚阵雨
		else if(Icon_ID==29) Icon_ID=31;//夜晚阵雪
		sprintf(icon_buf, "pic %d,%d,%d", Tomorrow_night_Icon_X, Tomorrow_night_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //求数组元素个数
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //发送数据
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
		
		/* 后天 白天 */
		Icon_ID=SearchWeaIconID(weather.code_day[2]);   //搜索图标ID
		sprintf(icon_buf, "pic %d,%d,%d", AfterTomor_day_Icon_X, AfterTomor_day_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //求数组元素个数
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //发送数据
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
		/* 后天 夜晚 */
		Icon_ID=SearchWeaIconID(weather.code_night[2]);   //搜索图标ID
		if(Icon_ID==13) Icon_ID+=1;			//夜晚多云
		else if(Icon_ID==16) Icon_ID=32;//夜晚阵雨
		else if(Icon_ID==29) Icon_ID=31;//夜晚阵雪
		sprintf(icon_buf, "pic %d,%d,%d", AfterTomor_night_Icon_X, AfterTomor_night_Icon_Y, Icon_ID);
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //求数组元素个数
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //发送数据
	  rt_device_write(serial3, 0, endstr, 3);
	  memset(icon_buf,'\0',sizeof(icon_buf));
	}
}
static void DisplayNowWeatherTcon(void)
{
  DisplayWeatherIcon(weather_data);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(DisplayNowWeatherTcon,Display Weather Icon); 

/********************************************************************************************************
** 函数: DisplayWifiIcon, 更新WIFI图标至串口屏
**------------------------------------------------------------------------------------------------------
** 参数: weather：获取到的天气数据
** 返回: 无		
** 说明: 刷图指令：pic x,y,picid  (x:起始点x坐标，y:起始点y坐标，picid:图片ID)
********************************************************************************************************/
void DisplayWifiIcon(unsigned char sign)
{
		char icon_buf[32] = {0};						 //数据缓存区
		int Lenth=0;                         //长度
		char endstr[3]={0xFF,0xFF,0xFF};     //结束符
		char Icon_ID;
	if(sign)     //已连接
    Icon_ID=10;
	else                      //断开
		Icon_ID=9;
		if(strcmp(g_page,"page_Desk")==0)    //字符串比较  相等返回0  桌面
	{
		sprintf(icon_buf, "pic %d,%d,%d", WIFI_Icon_X, WIFI_Icon_Y, Icon_ID);                  
		Lenth=ForLenth(icon_buf,sizeof(icon_buf));                                      //求数组元素个数
	  rt_device_write(serial3, 0, icon_buf, Lenth);  																	 //发送数据
	  rt_device_write(serial3, 0, endstr, 3);
	}
}
static void DISPLAYWIFITCON(void)
{
  DisplayWifiIcon(WIFI_connect_Sign);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(DISPLAYWIFITCON,Display WIFI Icon);

/********************************************************************************************************
** 函数: DisplayRTCTime, 更新时间至串口屏
**------------------------------------------------------------------------------------------------------
** 参数: weather：获取到的天气数据
** 返回: 无		
********************************************************************************************************/
void DisplayRTCTime(Calendar_T calendar)
{
	char time_buf[36]={0};
	int Lenth=0;                         //长度
  char endstr[3]={0xFF,0xFF,0xFF};     //结束符
		
	sprintf(time_buf, "Desk.DeskDateTxt.txt=\"%.4d-%.2d-%.2d\"",calendar.w_year ,calendar.w_month ,calendar.w_date );   //日期
	Lenth=ForLenth(time_buf,sizeof(time_buf));                                      //求数组元素个数
	rt_device_write(serial3, 0, time_buf, Lenth);                                     //发送数据
	rt_device_write(serial3, 0, endstr, 3);
	memset(time_buf, 0, sizeof(time_buf));

	rt_thread_mdelay(50);
	
	sprintf(time_buf, "Desk.DeskTimeTxt.txt=\"%.2d:%.2d:%.2d\"", calendar.hour,calendar.min,calendar.sec);             //时间           
	Lenth=ForLenth(time_buf,sizeof(time_buf));                                      //求数组元素个数
	rt_device_write(serial3, 0, time_buf, Lenth);  																	 //发送数据
	rt_device_write(serial3, 0, endstr, 3);
	
  memset(time_buf,0,sizeof(time_buf));
	
	sprintf(time_buf, "Desk.DeskWeek.txt=\"%s\"", wday[calendar.week]);                                            //星期
	Lenth=ForLenth(time_buf,sizeof(time_buf));                                      //求数组元素个数
	rt_device_write(serial3, 0, time_buf, Lenth);  																	 //发送数据
	rt_device_write(serial3, 0, endstr, 3);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(DisplayRTCTime,Display Rtc Time);

/*************************************************
*函数：  DisplayLoadIcon      加载图标
*参数：  sign     1 加载中      0加载完毕
**************************************************/
static void DisplayLoadIcon(unsigned char sign)
{
	char Load_buf[36]={0};
	int Lenth=0;                         //长度
  char endstr[3]={0xFF,0xFF,0xFF};     //结束符
  if(sign)
	{
	  sprintf(Load_buf, "pic %d,%d,%d",Load_Icon_X,Load_Icon_Y,43);                        
	  Lenth=ForLenth(Load_buf,sizeof(Load_buf));                                      //求数组元素个数
	  rt_device_write(serial3, 0, Load_buf, Lenth);  																	 //发送数据
	  rt_device_write(serial3, 0, endstr, 3);
	}
	else
	{
	  sprintf(Load_buf, "pic %d,%d,%d",Load_Icon_X,Load_Icon_Y,44);                      
	  Lenth=ForLenth(Load_buf,sizeof(Load_buf));                                      //求数组元素个数
	  rt_device_write(serial3, 0, Load_buf, Lenth);  																	 //发送数据
	  rt_device_write(serial3, 0, endstr, 3);
	}

}
/*************************************************************************************
**串口屏下发的数据处理函数
**
**
**************************************************************************************/
void HMI_RePrecess(void)
{
	char *p = NULL;
	/* 页面标识 */
	if((p=strstr((char*)s_sUsartBuf3,"page:"))!=NULL)
	{
		if(sscanf(p,"page:%s",g_page)==1)
		{
			printf("page:%s\n",g_page);
			
			DisplayWeatherIcon(weather_data);      //天气图标
			Task_SendHMI_WeaData(weather_data);    //天气数据
			
			if(strcmp(g_page,"page_Desk")==0)
			{
				DisplayWifiIcon(WIFI_connect_Sign);  //WIFI图标
			}
		}
	}
	/* 刷新 重新获取天气数据*/
	if((p=strstr((char*)s_sUsartBuf3,"update:"))!=NULL)
	{
		if(sscanf(p,"update:%s",g_update)==1)
		{
			printf("update:%s\n",g_update);
			
			 DisplayLoadIcon(1);     //显示加载图标
			
			 GetTest();            //测试
			 GetNowWeather();      //访问实况天气数据
			 GetDailyWeather();  //访问未来三天天气数据
			 GetLifeSuggestion(); //访问生活建议数据
			 AccessNTP();   //获取时间
			
			 DisplayLoadIcon(0);     //关闭加载图标
		}				
	}
	/* wifi名称 */
	 if((p=strstr((char*)s_sUsartBuf3,"wifi_ssid:"))!=NULL)
	{
		if(sscanf(p,"wifi_ssid:%s",g_wifi_ssid)==1)
		{
			printf("wifi_ssid:%s\n",g_wifi_ssid);
		}
	}
	/* wifi密码 */
  if((p=strstr((char*)s_sUsartBuf3,"wifi_password:"))!=NULL)
	{
		if(sscanf(p,"wifi_password:%s",g_wifi_password)==1)
		{
			printf("wifi_password:%s\n",g_wifi_password);
			WIFI_connect_Sign = ESP8266_JoinAP(g_wifi_ssid, g_wifi_password);   //连接WIFI
			printf("g_connect_sta = %d\n",WIFI_connect_Sign);
			DisplayWifiIcon(WIFI_connect_Sign);
			if(WIFI_connect_Sign)
			{
				printf("WIFI Connect Successful!\n");
				
				rt_sem_release(GetWea_sem);    //WIFI已连接 获取数据 释放信号量   GetWeather_thread_entry线程入口函数（获取天气数据）等待接收
			}
			else 
				printf("WIFI Connect Failed!\n");
		}
	}
	/* 解析地名 */
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
			
			 memset(&weather_data,0,sizeof(weather_data)); //清空
			
			 DisplayLoadIcon(1);     //显示加载图标
			
			 GetTest();            //测试
			 GetNowWeather();      //访问实况天气数据
			 GetDailyWeather();  //访问未来三天天气数据
			 GetLifeSuggestion(); //访问生活建议数据
			 AccessNTP();   //获取时间
			
			 DisplayLoadIcon(0);     //关闭加载图标
		}
	}
	/* 设置时间 */
	if((p=strstr((char*)s_sUsartBuf3,"Time_hour:"))!=NULL)   //时
	{
		if(sscanf(p,"Time_hour:%s",g_time_hour)==1)
		{
			printf("Time_hour:%s\n",g_time_hour);
			RTC_Get();
			RTC_Set(calendar_t.w_year ,calendar_t.w_month ,calendar_t.w_date ,atoi(g_time_hour),calendar_t.min ,calendar_t.sec );
		}
	}
	else if((p=strstr((char*)s_sUsartBuf3,"Time_min:"))!=NULL) //分
	{
	  if(sscanf(p,"Time_min:%s",g_time_min)==1)
		{
			printf("Time_min:%s\n",g_time_min);
			RTC_Get();
			RTC_Set(calendar_t.w_year ,calendar_t.w_month ,calendar_t.w_date ,calendar_t.hour, atoi(g_time_min),calendar_t.sec );
		}
	}
	else if((p=strstr((char*)s_sUsartBuf3,"Time_sec:"))!=NULL)  //秒
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

