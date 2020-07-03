#include "config.h"
#include "WeatherInfo.h"
#include "CJSON.h"


/* 心知天气（api.seniverse.com）--天气数据的来源 */
#define WEATHER_IP 		"116.62.81.138"
#define WEATHER_PORT 	"80"



/* 定义一些全局变量 */
//------------------------------------------------------------------------------------------
//所需要的天气数据,供其他应用使用
WeatherType weather_data;

int GetTest(void)
{	
	
  ESP8266_Link_Server(enumTCP, (char*)WEATHER_IP, (char*)WEATHER_PORT,Single_ID_0);//TCP
	ESP8266_UnvarnishSend ();//传输模式为：透传	

	macESP8266_Usart ( "GET https://test");

	ESP8266_ExitUnvarnishSend ( );//退出透传
  ESP8266_Close_Link (  );//关闭TCP或UDP连接  HZJ																		
  
	return 0;
}

/********************************************************************************************************
** 函数: cJSON_NowWeatherParse, 解析天气实况数据包now.json
**------------------------------------------------------------------------------------------------------
** 参数: JSON:天气实况数据包  WeatherType:存储解析得到的有用的数据
** 说明: 数据来源：心知天气（api.seniverse.com）
** 返回: 0:解析成功 其他：解析失败		
********************************************************************************************************/
static int cJSON_NowWeatherParse(char *JSON, WeatherType *result)
{
	cJSON *json,*arrayItem,*object,*subobject,*item;
	char *utf8str,*gbkstr;
	int gbkstr_len;
	
	utf8str = (char*)mymalloc(SRAMIN, 64*sizeof(char));
	gbkstr = (char*)mymalloc(SRAMIN, 64*sizeof(char));
	
	memset(utf8str, '\0', 64);
	memset(gbkstr,  '\0', 64);
 
	json = cJSON_Parse(JSON);//解析JSON数据包
	if(json == NULL)		  //检测JSON数据包是否存在语法上的错误，返回NULL表示数据包无效
	{
		printf("Error before: [%s]\r\n",cJSON_GetErrorPtr()); //打印数据包语法错误的位置
		return 0;
	}
	else
	{
		if((arrayItem = cJSON_GetObjectItem(json,"results")) != NULL)               //匹配字符串"results",获取数组内容
		{
			int size = cJSON_GetArraySize(arrayItem);     //获取数组中对象个数
			
			if((object = cJSON_GetArrayItem(arrayItem,0)) != NULL)//获取父对象内容
			{
        printf("--------------------------------Now Weather Data Begin------------------------------\r\n");
				/* 匹配子对象1 */
				if((subobject = cJSON_GetObjectItem(object,"location")) != NULL)
				{
	
					printf("---------------------------------subobject1-------------------------------\r\n");
					if((item = cJSON_GetObjectItem(subobject,"id")) != NULL)   
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
						
					}
					if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) //地名--需要用到的数据
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);   //打印数据
						utf8str = item->valuestring;
						SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
						memcpy(result->name,gbkstr, strlen(gbkstr));   //保存数据
					}
						
					if((item = cJSON_GetObjectItem(subobject,"country")) != NULL)
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
					if((item = cJSON_GetObjectItem(subobject,"path")) != NULL)  
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
					if((item = cJSON_GetObjectItem(subobject,"timezone")) != NULL)
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
					if((item = cJSON_GetObjectItem(subobject,"timezone_offset")) != NULL)
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
				}
				/* 匹配子对象2 */
				if((subobject = cJSON_GetObjectItem(object,"now")) != NULL)
				{
					printf("---------------------------------subobject2-------------------------------\r\n");
						if((item = cJSON_GetArrayItem(subobject,0)) != NULL)  						//获取父对象内容
						{
							memset(utf8str, '\0', 64);
	            memset(gbkstr,  '\0', 64);
							if((item = cJSON_GetObjectItem(subobject,"text")) != NULL)    //天气预报文字--需要用到的数据
							{
									printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
									utf8str = item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->text,gbkstr, strlen(gbkstr));   //保存数据

							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((item = cJSON_GetObjectItem(subobject,"code")) != NULL)//天气预报代码--需要用到的数据
							{
									printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
									utf8str = item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->code,gbkstr, strlen(gbkstr));   //保存数据

							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((item = cJSON_GetObjectItem(subobject,"temperature")) != NULL)      //气温--需要用到的数据
							{
									printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
									utf8str = item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->temperature,gbkstr, strlen(gbkstr));   //保存数据
							}
						}
					}
				}
				/* 匹配子对象3 */
				if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
				{
					printf("---------------------------------subobject3-------------------------------\r\n");
					printf("cJSON_GetObjectItem: %s:%s\r\n",subobject->string,subobject->valuestring);
				}
				printf("--------------------------------Now Weather Data End--------------------------------\r\n");
			} 
		
		}
	cJSON_Delete(json); //释放cJSON_Parse()分配出来的内存空间
	/* 释放内存块 */
	myfree(SRAMIN,utf8str);
	myfree(SRAMIN,gbkstr);
	return 1;
}
/********************************************************************************************************
** 函数: GET_NowWeather, GET 天气实况数据包now.json
**------------------------------------------------------------------------------------------------------
** 参数: void
** 说明: 数据来源：心知天气（api.seniverse.com）
** 返回: 0:获取成功 其他：获取失败
********************************************************************************************************/
int GetNowWeather(void)
{
	bool sign;  
	
  ESP8266_Link_Server(enumTCP, (char*)WEATHER_IP, (char*)WEATHER_PORT,Single_ID_0);//TCP
	ESP8266_UnvarnishSend ();//传输模式为：透传	

	macESP8266_Usart ( "GET https://api.seniverse.com/v3/weather/now.json?key=SiM5_bac6DvBWsUzi&location=%s&language=zh-Hans&unit=c\r\n\r\n",g_city);//默认为北京天气,不要忘记\r\n\r\n,参数language、uint为可选参数

	strEsp8266_Fram_Record .InfBit .FramLength = 0;  //重新开始接收新的数据包

	rt_thread_mdelay(1000);   //等待返回数据
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';  //结束位
	printf("Receive now data:%s\r\n",strEsp8266_Fram_Record .Data_RX_BUF);  //重定向  关联rt_kprintf函数

	sign=cJSON_NowWeatherParse((char*)strEsp8266_Fram_Record .Data_RX_BUF, &weather_data);	 //解析实况天气数据

	ESP8266_ExitUnvarnishSend ( );//退出透传
  ESP8266_Close_Link (  );//关闭TCP或UDP连接																		

	return sign;
}

MSH_CMD_EXPORT(GetNowWeather,Get NowWeather Infomation);     //添加命令至msh命令列表

/********************************************************************************************************
** 函数: cJSON_DailyWeatherParse, 解析三天天气数据包daily.json
**------------------------------------------------------------------------------------------------------
** 参数: JSON:三天天气数据包  WeatherType:存储解析得到的有用的数据
** 说明: 数据来源：心知天气（api.seniverse.com）
** 返回: 0:解析成功 其他：解析失败		
********************************************************************************************************/
//2000 46F4    0x20002360  2009 7518
static int cJSON_DailyWeatherParse(char *JSON, WeatherType *result)
{
	unsigned char i;
	cJSON *json,*arrayItem,*object,*subobject,*sunobject,*item,*child_Item;
	char *utf8str,*gbkstr;
	int gbkstr_len;
	
//	rt_mp_t mp_weather;
	
 /* 动态创建内存池对象 */
//	mp_weather=rt_mp_create("mp_weather",    //内存池名
//                               2,					 //内存块数量
//                               64);				 //内存块容量
	/* 申请内存块 */
//	utf8str = (char*)rt_mp_alloc(mp_weather, RT_WAITING_FOREVER);  
//	gbkstr  = (char*)rt_mp_alloc(mp_weather, RT_WAITING_FOREVER);
	
	utf8str = (char*)mymalloc(SRAMIN, 64*sizeof(char));
	gbkstr = (char*)mymalloc(SRAMIN, 64*sizeof(char));
	
	memset(utf8str, '\0', 64);
	memset(gbkstr,  '\0', 64);
 
	json = cJSON_Parse(JSON);//解析JSON数据包
	if(json == NULL)		  //检测JSON数据包是否存在语法上的错误，返回NULL表示数据包无效
	{
		printf("Error before: [%s]\r\n",cJSON_GetErrorPtr()); //打印数据包语法错误的位置
		return 0;
	}
	else
	{
		if((arrayItem = cJSON_GetObjectItem(json,"results")) != NULL)               //匹配字符串"results",获取数组内容
		{
			int size = cJSON_GetArraySize(arrayItem);     //获取数组中对象个数
			
			if((object = cJSON_GetArrayItem(arrayItem,0)) != NULL)//获取父对象内容
			{
        printf("--------------------------------Daily Weather Data Begin------------------------------\r\n");
				/* 匹配子对象1 */
				if((subobject = cJSON_GetObjectItem(object,"location")) != NULL)
				{
					
					printf("---------------------------------subobject1-------------------------------\r\n");
					if((item = cJSON_GetObjectItem(subobject,"id")) != NULL)   
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
						
					}
					if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) //地名--需要用到的数据
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);   //打印数据
					}
						
					if((item = cJSON_GetObjectItem(subobject,"country")) != NULL)
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
					if((item = cJSON_GetObjectItem(subobject,"path")) != NULL)  
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
					if((item = cJSON_GetObjectItem(subobject,"timezone")) != NULL)
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
					if((item = cJSON_GetObjectItem(subobject,"timezone_offset")) != NULL)
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
				}
				/* 匹配子对象2 */
				if((subobject = cJSON_GetObjectItem(object,"daily")) != NULL)
				{
					printf("---------------------------------subobject2-------------------------------\r\n");
					for(i=0;i<3;i++)      //三天天气数据
					{
						if((sunobject = cJSON_GetArrayItem(subobject,i)) != NULL)  						//获取父对象内容
						{
							switch(i)
							{
								case 0:
									printf("today:\r\n");
								break;
								case 1:
									printf("tomorrow:\r\n");
								break;
								case 2:
									printf("the day after tomorrow:\r\n");
								break;
								
							}
							if((child_Item = cJSON_GetObjectItem(sunobject,"date")) != NULL)//日期--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->date[i],gbkstr, strlen(gbkstr));   //保存数据

							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((child_Item = cJSON_GetObjectItem(sunobject,"text_day")) != NULL)//天气预报文字--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->text_day[i],gbkstr, strlen(gbkstr));   //保存数据

							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((child_Item = cJSON_GetObjectItem(sunobject,"code_day")) != NULL)//天气预报代码--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->code_day[i],gbkstr, strlen(gbkstr));   //保存数据

							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((child_Item = cJSON_GetObjectItem(sunobject,"text_night")) != NULL)//天气预报文字--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->text_night[i],gbkstr, strlen(gbkstr));   //保存数据

							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((child_Item = cJSON_GetObjectItem(sunobject,"code_night")) != NULL)//天气预报代码--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->code_night[i],gbkstr, strlen(gbkstr));   //保存数据

							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((child_Item = cJSON_GetObjectItem(sunobject,"high")) != NULL) //最高气温--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->high[i],gbkstr, strlen(gbkstr));   //保存数据
							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((child_Item = cJSON_GetObjectItem(sunobject,"low")) != NULL) //最低气温--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->low[i],gbkstr, strlen(gbkstr));   //保存数据
							}
							if((child_Item = cJSON_GetObjectItem(sunobject,"rainfall")) != NULL) //降雨量
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
							}
							if((child_Item = cJSON_GetObjectItem(sunobject,"precip")) != NULL) //降雨
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((child_Item = cJSON_GetObjectItem(sunobject,"wind_direction")) != NULL) //风向--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->wind_direction[i],gbkstr, strlen(gbkstr));   //保存数据
							}
							if((child_Item = cJSON_GetObjectItem(sunobject,"wind_direction_degree")) != NULL) //风向程度
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((child_Item = cJSON_GetObjectItem(sunobject,"wind_speed")) != NULL) //风速--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->wind_speed[i],gbkstr, strlen(gbkstr));   //保存数据
							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((child_Item = cJSON_GetObjectItem(sunobject,"wind_scale")) != NULL) //风力等级--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->wind_scale[i],gbkstr, strlen(gbkstr));   //保存数据
							}
							 memset(utf8str, '\0', 64);  //清空
	             memset(gbkstr, '\0', 64);	 //清空
							if((child_Item = cJSON_GetObjectItem(sunobject,"humidity")) != NULL) //湿度--需要用到的数据
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->humidity[i],gbkstr, strlen(gbkstr));   //保存数据
							}
						}
					}
				}
				/* 匹配子对象3 */
				if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
				{
					printf("---------------------------------subobject3-------------------------------\r\n");
					printf("cJSON_GetObjectItem: %s:%s\r\n",subobject->string,subobject->valuestring);
				}
				printf("--------------------------------Daily Weather Data End--------------------------------\r\n");
			} 
		
		}
	};
	cJSON_Delete(json); //释放cJSON_Parse()分配出来的内存空间
	/* 释放内存块 */
//	rt_mp_free(utf8str);
//	rt_mp_free(gbkstr);
	
	myfree(SRAMIN,utf8str);
	myfree(SRAMIN,gbkstr);
	/*删除内存池*/
//	rt_mp_delete(mp_weather);
	return 1;

}	


/********************************************************************************************************
** 函数: GetDailyWeather, GET 三天天气数据包daily.json
**------------------------------------------------------------------------------------------------------
** 参数: void
** 说明: 数据来源：心知天气（api.seniverse.com）
** 返回: 0:获取成功 其他：获取失败	
** 作者: 2017.12.8 by Hezhijie and Lizhengnian 
********************************************************************************************************/
int GetDailyWeather(void)
{
	bool sign;  
	
  ESP8266_Link_Server(enumTCP, (char*)WEATHER_IP, (char*)WEATHER_PORT,Single_ID_0);//TCP
	ESP8266_UnvarnishSend ();//传输模式为：透传	

	macESP8266_Usart ( "GET https://api.seniverse.com/v3/weather/daily.json?key=SiM5_bac6DvBWsUzi&location=%s&language=zh-Hans&unit=c\r\n\r\n",g_city);//默认为北京天气,不要忘记\r\n\r\n,参数language、uint为可选参数

	strEsp8266_Fram_Record .InfBit .FramLength = 0;  //重新开始接收新的数据包

	rt_thread_mdelay(1000);   //等待返回数据
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';  //结束位
	printf("Receive Daily data:%s\r\n",strEsp8266_Fram_Record .Data_RX_BUF);  //重定向  关联rt_kprintf函数
	

	sign=cJSON_DailyWeatherParse((char*)strEsp8266_Fram_Record .Data_RX_BUF, &weather_data);	 //解析未来三天天气数据

	ESP8266_ExitUnvarnishSend ( );//退出透传
  ESP8266_Close_Link (  );//关闭TCP或UDP连接  HZJ
																					
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,sizeof(strEsp8266_Fram_Record.Data_RX_BUF));  //清空数组
	return sign;
}

MSH_CMD_EXPORT(GetDailyWeather,Get DailyWeather Infomation);     //添加命令至msh命令列表

/********************************************************************************************************
** 函数: cJSON_LifeSuggestionParse, 解析生活指数及生活建议
**------------------------------------------------------------------------------------------------------
** 参数: JSON:生活指数及生活建议数据包 WeatherType:存储解析得到的有用的数据
** 说明: 数据来源：心知天气（api.seniverse.com）
** 返回: 0:解析成功 其他：解析失败
********************************************************************************************************/
static int cJSON_LifeSuggestionParse(char *JSON, WeatherType *result)
{
	cJSON *json,*arrayItem,*object,*subobject,*childobject,*item;
	char *utf8str,*gbkstr;
	int gbkstr_len;
	
	utf8str = (char*)mymalloc(SRAMIN, 200*sizeof(char));
	gbkstr = (char*)mymalloc(SRAMIN, 200*sizeof(char));
	
	memset(utf8str, '\0', 64);
	memset(gbkstr,  '\0', 64);
	
	printf("Receive LifeSuggestion data:%s\n",JSON);
	json = cJSON_Parse(JSON); //解析JSON数据包
	if(json == NULL)		  //检测JSON数据包是否存在语法上的错误，返回NULL表示数据包无效
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr()); //打印数据包语法错误的位置
		return 0;
	}
	else
	{
		if((arrayItem = cJSON_GetObjectItem(json,"results")) != NULL); //匹配字符串"results",获取数组内容
		{
			int size = cJSON_GetArraySize(arrayItem);     //获取数组中对象个数
		  printf("--------------------------------Life Suggestion Data Begin------------------------------\r\n");
			if((object = cJSON_GetObjectItem(arrayItem,0)) != NULL)//获取父对象内容
			{
				/* 匹配子对象1 */
				if((subobject = cJSON_GetObjectItem(object,"location")) != NULL)
				{		
					printf("---------------------------------subobject1-------------------------------\r\n");
					if((item = cJSON_GetObjectItem(subobject,"id")) != NULL)   
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
					if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) 
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);  
					}				
					if((item = cJSON_GetObjectItem(subobject,"country")) != NULL)
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
					if((item = cJSON_GetObjectItem(subobject,"path")) != NULL)  
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
					if((item = cJSON_GetObjectItem(subobject,"timezone")) != NULL)
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
					if((item = cJSON_GetObjectItem(subobject,"timezone_offset")) != NULL)
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
					}
				}
				/* 匹配子对象2 */
				if((subobject = cJSON_GetObjectItem(object,"suggestion")) != NULL)
				{
					printf("---------------------------------subobject2-------------------------------\r\n");
				 if((childobject = cJSON_GetObjectItem(subobject,"car_washing"))!= NULL)  //洗车 --需要用到的数据
					{
						if((item = cJSON_GetObjectItem(childobject,"brief"))!=NULL)
						{
						  printf("cJSON_GetObjectItem: %s:%s\r\n",childobject->string,item->valuestring);
							utf8str = item->valuestring;
							SwitchToGbk((const unsigned char*)utf8str, strlen(utf8str), (unsigned char*)gbkstr, &gbkstr_len);
							memcpy(result->life_suggestion[0], gbkstr, strlen(gbkstr));
						}
					}
					memset(utf8str, '\0', 64);
	        memset(gbkstr,  '\0', 64);
				 if((childobject = cJSON_GetObjectItem(subobject,"flu"))!= NULL)         //流感 --需要用到的数据
					{
						if((item = cJSON_GetObjectItem(childobject,"brief"))!=NULL)
						{
						  printf("cJSON_GetObjectItem: %s:%s\r\n",childobject->string,item->valuestring);
							utf8str = item->valuestring;
							SwitchToGbk((const unsigned char*)utf8str, strlen(utf8str), (unsigned char*)gbkstr, &gbkstr_len);
							memcpy(result->life_suggestion[1], gbkstr, strlen(gbkstr));
						}
					}
					memset(utf8str, '\0', 64);
	        memset(gbkstr,  '\0', 64);
				if((childobject = cJSON_GetObjectItem(subobject,"dressing"))!= NULL)         //穿搭 --需要用到的数据
					{
						if((item = cJSON_GetObjectItem(childobject,"brief"))!=NULL)
						{
						  printf("cJSON_GetObjectItem: %s:%s\r\n",childobject->string,item->valuestring);
							utf8str = item->valuestring;
							SwitchToGbk((const unsigned char*)utf8str, strlen(utf8str), (unsigned char*)gbkstr, &gbkstr_len);
							memcpy(result->life_suggestion[2], gbkstr, strlen(gbkstr));
						}
					}
					memset(utf8str, '\0', 64);
	        memset(gbkstr,  '\0', 64);
				if((childobject = cJSON_GetObjectItem(subobject,"sport"))!= NULL)         //运动--需要用到的数据
					{
						if((item = cJSON_GetObjectItem(childobject,"brief"))!=NULL)
						{
						  printf("cJSON_GetObjectItem: %s:%s\r\n",childobject->string,item->valuestring);
							utf8str = item->valuestring;
							SwitchToGbk((const unsigned char*)utf8str, strlen(utf8str), (unsigned char*)gbkstr, &gbkstr_len);
							memcpy(result->life_suggestion[3], gbkstr, strlen(gbkstr));
						}
					}
					memset(utf8str, '\0', 64);
	        memset(gbkstr,  '\0', 64);
				if((childobject = cJSON_GetObjectItem(subobject,"travel"))!= NULL)         //旅行 --需要用到的数据
					{
						if((item = cJSON_GetObjectItem(childobject,"brief"))!=NULL)
						{
						  printf("cJSON_GetObjectItem: %s:%s\r\n",childobject->string,item->valuestring);
							utf8str = item->valuestring;
							SwitchToGbk((const unsigned char*)utf8str, strlen(utf8str), (unsigned char*)gbkstr, &gbkstr_len);
							memcpy(result->life_suggestion[4], gbkstr, strlen(gbkstr));
						}
					}
					memset(utf8str, '\0', 64);
	        memset(gbkstr,  '\0', 64);
				if((childobject = cJSON_GetObjectItem(subobject,"uv"))!= NULL)         //紫外线 --需要用到的数据
					{
						if((item = cJSON_GetObjectItem(childobject,"brief"))!=NULL)
						{
						  printf("cJSON_GetObjectItem: %s:%s\r\n",childobject->string,item->valuestring);
							utf8str = item->valuestring;
							SwitchToGbk((const unsigned char*)utf8str, strlen(utf8str), (unsigned char*)gbkstr, &gbkstr_len);
							memcpy(result->life_suggestion[5], gbkstr, strlen(gbkstr));
						}
					}
				}
				/* 匹配子对象3 */
				if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
				{
					printf("---------------------------------subobject3-------------------------------\r\n");
					printf("cJSON_GetObjectItem: %s:%s\r\n",subobject->string,subobject->valuestring);
				}
				printf("--------------------------------Life Suggestion Data End--------------------------------\r\n"); 
			}
		}
	}
			/* 释放内存块 */
	myfree(SRAMIN,utf8str);
	myfree(SRAMIN,gbkstr);
	
	return 1;
}

/********************************************************************************************************
** 函数: GETLifeSuggestion, GET 生活指数及生活建议suggestion.json
**------------------------------------------------------------------------------------------------------
** 参数: void
** 说明: 数据来源：心知天气（api.seniverse.com）
** 返回: 0:获取成功 其他：获取失败
********************************************************************************************************/
int GetLifeSuggestion(void)
{
	bool sign;  
	
  ESP8266_Link_Server(enumTCP, (char*)WEATHER_IP, (char*)WEATHER_PORT,Single_ID_0);//TCP
	ESP8266_UnvarnishSend ();//传输模式为：透传	

	macESP8266_Usart ( "GET https://api.seniverse.com/v3/life/suggestion.json?key=SiM5_bac6DvBWsUzi&location=%s&language=zh-Hans&unit=c\r\n\r\n",g_city);//默认为北京天气,不要忘记\r\n\r\n,参数language、uint为可选参数

	strEsp8266_Fram_Record .InfBit .FramLength = 0;  //重新开始接收新的数据包

	rt_thread_mdelay(1000);   //等待返回数据
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';  //结束位
	printf("Receive now data:%s\r\n",strEsp8266_Fram_Record .Data_RX_BUF);  //重定向  关联rt_kprintf函数

	sign=cJSON_LifeSuggestionParse((char*)strEsp8266_Fram_Record .Data_RX_BUF, &weather_data);	 //解析生活指数及生活建议

	ESP8266_ExitUnvarnishSend ( );//退出透传
  ESP8266_Close_Link (  );//关闭TCP或UDP连接
	 																					
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,sizeof(strEsp8266_Fram_Record.Data_RX_BUF));  //清空数组
	
	return sign;
}

MSH_CMD_EXPORT(GetLifeSuggestion,Get Life Suggestion);     //添加命令至msh命令列表

/***********************************************************************end***********************************************************************/
