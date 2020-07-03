#include "config.h"
#include "WeatherInfo.h"
#include "CJSON.h"


/* ��֪������api.seniverse.com��--�������ݵ���Դ */
#define WEATHER_IP 		"116.62.81.138"
#define WEATHER_PORT 	"80"



/* ����һЩȫ�ֱ��� */
//------------------------------------------------------------------------------------------
//����Ҫ����������,������Ӧ��ʹ��
WeatherType weather_data;

int GetTest(void)
{	
	
  ESP8266_Link_Server(enumTCP, (char*)WEATHER_IP, (char*)WEATHER_PORT,Single_ID_0);//TCP
	ESP8266_UnvarnishSend ();//����ģʽΪ��͸��	

	macESP8266_Usart ( "GET https://test");

	ESP8266_ExitUnvarnishSend ( );//�˳�͸��
  ESP8266_Close_Link (  );//�ر�TCP��UDP����  HZJ																		
  
	return 0;
}

/********************************************************************************************************
** ����: cJSON_NowWeatherParse, ��������ʵ�����ݰ�now.json
**------------------------------------------------------------------------------------------------------
** ����: JSON:����ʵ�����ݰ�  WeatherType:�洢�����õ������õ�����
** ˵��: ������Դ����֪������api.seniverse.com��
** ����: 0:�����ɹ� ����������ʧ��		
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
 
	json = cJSON_Parse(JSON);//����JSON���ݰ�
	if(json == NULL)		  //���JSON���ݰ��Ƿ�����﷨�ϵĴ��󣬷���NULL��ʾ���ݰ���Ч
	{
		printf("Error before: [%s]\r\n",cJSON_GetErrorPtr()); //��ӡ���ݰ��﷨�����λ��
		return 0;
	}
	else
	{
		if((arrayItem = cJSON_GetObjectItem(json,"results")) != NULL)               //ƥ���ַ���"results",��ȡ��������
		{
			int size = cJSON_GetArraySize(arrayItem);     //��ȡ�����ж������
			
			if((object = cJSON_GetArrayItem(arrayItem,0)) != NULL)//��ȡ����������
			{
        printf("--------------------------------Now Weather Data Begin------------------------------\r\n");
				/* ƥ���Ӷ���1 */
				if((subobject = cJSON_GetObjectItem(object,"location")) != NULL)
				{
	
					printf("---------------------------------subobject1-------------------------------\r\n");
					if((item = cJSON_GetObjectItem(subobject,"id")) != NULL)   
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
						
					}
					if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) //����--��Ҫ�õ�������
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);   //��ӡ����
						utf8str = item->valuestring;
						SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
						memcpy(result->name,gbkstr, strlen(gbkstr));   //��������
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
				/* ƥ���Ӷ���2 */
				if((subobject = cJSON_GetObjectItem(object,"now")) != NULL)
				{
					printf("---------------------------------subobject2-------------------------------\r\n");
						if((item = cJSON_GetArrayItem(subobject,0)) != NULL)  						//��ȡ����������
						{
							memset(utf8str, '\0', 64);
	            memset(gbkstr,  '\0', 64);
							if((item = cJSON_GetObjectItem(subobject,"text")) != NULL)    //����Ԥ������--��Ҫ�õ�������
							{
									printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
									utf8str = item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->text,gbkstr, strlen(gbkstr));   //��������

							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((item = cJSON_GetObjectItem(subobject,"code")) != NULL)//����Ԥ������--��Ҫ�õ�������
							{
									printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
									utf8str = item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->code,gbkstr, strlen(gbkstr));   //��������

							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((item = cJSON_GetObjectItem(subobject,"temperature")) != NULL)      //����--��Ҫ�õ�������
							{
									printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
									utf8str = item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->temperature,gbkstr, strlen(gbkstr));   //��������
							}
						}
					}
				}
				/* ƥ���Ӷ���3 */
				if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
				{
					printf("---------------------------------subobject3-------------------------------\r\n");
					printf("cJSON_GetObjectItem: %s:%s\r\n",subobject->string,subobject->valuestring);
				}
				printf("--------------------------------Now Weather Data End--------------------------------\r\n");
			} 
		
		}
	cJSON_Delete(json); //�ͷ�cJSON_Parse()����������ڴ�ռ�
	/* �ͷ��ڴ�� */
	myfree(SRAMIN,utf8str);
	myfree(SRAMIN,gbkstr);
	return 1;
}
/********************************************************************************************************
** ����: GET_NowWeather, GET ����ʵ�����ݰ�now.json
**------------------------------------------------------------------------------------------------------
** ����: void
** ˵��: ������Դ����֪������api.seniverse.com��
** ����: 0:��ȡ�ɹ� ��������ȡʧ��
********************************************************************************************************/
int GetNowWeather(void)
{
	bool sign;  
	
  ESP8266_Link_Server(enumTCP, (char*)WEATHER_IP, (char*)WEATHER_PORT,Single_ID_0);//TCP
	ESP8266_UnvarnishSend ();//����ģʽΪ��͸��	

	macESP8266_Usart ( "GET https://api.seniverse.com/v3/weather/now.json?key=SiM5_bac6DvBWsUzi&location=%s&language=zh-Hans&unit=c\r\n\r\n",g_city);//Ĭ��Ϊ��������,��Ҫ����\r\n\r\n,����language��uintΪ��ѡ����

	strEsp8266_Fram_Record .InfBit .FramLength = 0;  //���¿�ʼ�����µ����ݰ�

	rt_thread_mdelay(1000);   //�ȴ���������
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';  //����λ
	printf("Receive now data:%s\r\n",strEsp8266_Fram_Record .Data_RX_BUF);  //�ض���  ����rt_kprintf����

	sign=cJSON_NowWeatherParse((char*)strEsp8266_Fram_Record .Data_RX_BUF, &weather_data);	 //����ʵ����������

	ESP8266_ExitUnvarnishSend ( );//�˳�͸��
  ESP8266_Close_Link (  );//�ر�TCP��UDP����																		

	return sign;
}

MSH_CMD_EXPORT(GetNowWeather,Get NowWeather Infomation);     //���������msh�����б�

/********************************************************************************************************
** ����: cJSON_DailyWeatherParse, ���������������ݰ�daily.json
**------------------------------------------------------------------------------------------------------
** ����: JSON:�����������ݰ�  WeatherType:�洢�����õ������õ�����
** ˵��: ������Դ����֪������api.seniverse.com��
** ����: 0:�����ɹ� ����������ʧ��		
********************************************************************************************************/
//2000 46F4    0x20002360  2009 7518
static int cJSON_DailyWeatherParse(char *JSON, WeatherType *result)
{
	unsigned char i;
	cJSON *json,*arrayItem,*object,*subobject,*sunobject,*item,*child_Item;
	char *utf8str,*gbkstr;
	int gbkstr_len;
	
//	rt_mp_t mp_weather;
	
 /* ��̬�����ڴ�ض��� */
//	mp_weather=rt_mp_create("mp_weather",    //�ڴ����
//                               2,					 //�ڴ������
//                               64);				 //�ڴ������
	/* �����ڴ�� */
//	utf8str = (char*)rt_mp_alloc(mp_weather, RT_WAITING_FOREVER);  
//	gbkstr  = (char*)rt_mp_alloc(mp_weather, RT_WAITING_FOREVER);
	
	utf8str = (char*)mymalloc(SRAMIN, 64*sizeof(char));
	gbkstr = (char*)mymalloc(SRAMIN, 64*sizeof(char));
	
	memset(utf8str, '\0', 64);
	memset(gbkstr,  '\0', 64);
 
	json = cJSON_Parse(JSON);//����JSON���ݰ�
	if(json == NULL)		  //���JSON���ݰ��Ƿ�����﷨�ϵĴ��󣬷���NULL��ʾ���ݰ���Ч
	{
		printf("Error before: [%s]\r\n",cJSON_GetErrorPtr()); //��ӡ���ݰ��﷨�����λ��
		return 0;
	}
	else
	{
		if((arrayItem = cJSON_GetObjectItem(json,"results")) != NULL)               //ƥ���ַ���"results",��ȡ��������
		{
			int size = cJSON_GetArraySize(arrayItem);     //��ȡ�����ж������
			
			if((object = cJSON_GetArrayItem(arrayItem,0)) != NULL)//��ȡ����������
			{
        printf("--------------------------------Daily Weather Data Begin------------------------------\r\n");
				/* ƥ���Ӷ���1 */
				if((subobject = cJSON_GetObjectItem(object,"location")) != NULL)
				{
					
					printf("---------------------------------subobject1-------------------------------\r\n");
					if((item = cJSON_GetObjectItem(subobject,"id")) != NULL)   
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);
						
					}
					if((item = cJSON_GetObjectItem(subobject,"name")) != NULL) //����--��Ҫ�õ�������
					{
						printf("cJSON_GetObjectItem: %s:%s\r\n",item->string,item->valuestring);   //��ӡ����
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
				/* ƥ���Ӷ���2 */
				if((subobject = cJSON_GetObjectItem(object,"daily")) != NULL)
				{
					printf("---------------------------------subobject2-------------------------------\r\n");
					for(i=0;i<3;i++)      //������������
					{
						if((sunobject = cJSON_GetArrayItem(subobject,i)) != NULL)  						//��ȡ����������
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
							if((child_Item = cJSON_GetObjectItem(sunobject,"date")) != NULL)//����--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->date[i],gbkstr, strlen(gbkstr));   //��������

							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((child_Item = cJSON_GetObjectItem(sunobject,"text_day")) != NULL)//����Ԥ������--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->text_day[i],gbkstr, strlen(gbkstr));   //��������

							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((child_Item = cJSON_GetObjectItem(sunobject,"code_day")) != NULL)//����Ԥ������--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->code_day[i],gbkstr, strlen(gbkstr));   //��������

							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((child_Item = cJSON_GetObjectItem(sunobject,"text_night")) != NULL)//����Ԥ������--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->text_night[i],gbkstr, strlen(gbkstr));   //��������

							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((child_Item = cJSON_GetObjectItem(sunobject,"code_night")) != NULL)//����Ԥ������--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->code_night[i],gbkstr, strlen(gbkstr));   //��������

							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((child_Item = cJSON_GetObjectItem(sunobject,"high")) != NULL) //�������--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->high[i],gbkstr, strlen(gbkstr));   //��������
							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((child_Item = cJSON_GetObjectItem(sunobject,"low")) != NULL) //�������--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->low[i],gbkstr, strlen(gbkstr));   //��������
							}
							if((child_Item = cJSON_GetObjectItem(sunobject,"rainfall")) != NULL) //������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
							}
							if((child_Item = cJSON_GetObjectItem(sunobject,"precip")) != NULL) //����
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((child_Item = cJSON_GetObjectItem(sunobject,"wind_direction")) != NULL) //����--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->wind_direction[i],gbkstr, strlen(gbkstr));   //��������
							}
							if((child_Item = cJSON_GetObjectItem(sunobject,"wind_direction_degree")) != NULL) //����̶�
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((child_Item = cJSON_GetObjectItem(sunobject,"wind_speed")) != NULL) //����--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->wind_speed[i],gbkstr, strlen(gbkstr));   //��������
							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((child_Item = cJSON_GetObjectItem(sunobject,"wind_scale")) != NULL) //�����ȼ�--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->wind_scale[i],gbkstr, strlen(gbkstr));   //��������
							}
							 memset(utf8str, '\0', 64);  //���
	             memset(gbkstr, '\0', 64);	 //���
							if((child_Item = cJSON_GetObjectItem(sunobject,"humidity")) != NULL) //ʪ��--��Ҫ�õ�������
							{
									printf("     cJSON_GetObjectItem: %s:%s\r\n",child_Item->string,child_Item->valuestring);
									utf8str = child_Item->valuestring;
									SwitchToGbk((const unsigned char*)utf8str, strlen((const char*)utf8str), (unsigned char*)gbkstr, &gbkstr_len);
									memcpy(result->humidity[i],gbkstr, strlen(gbkstr));   //��������
							}
						}
					}
				}
				/* ƥ���Ӷ���3 */
				if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
				{
					printf("---------------------------------subobject3-------------------------------\r\n");
					printf("cJSON_GetObjectItem: %s:%s\r\n",subobject->string,subobject->valuestring);
				}
				printf("--------------------------------Daily Weather Data End--------------------------------\r\n");
			} 
		
		}
	};
	cJSON_Delete(json); //�ͷ�cJSON_Parse()����������ڴ�ռ�
	/* �ͷ��ڴ�� */
//	rt_mp_free(utf8str);
//	rt_mp_free(gbkstr);
	
	myfree(SRAMIN,utf8str);
	myfree(SRAMIN,gbkstr);
	/*ɾ���ڴ��*/
//	rt_mp_delete(mp_weather);
	return 1;

}	


/********************************************************************************************************
** ����: GetDailyWeather, GET �����������ݰ�daily.json
**------------------------------------------------------------------------------------------------------
** ����: void
** ˵��: ������Դ����֪������api.seniverse.com��
** ����: 0:��ȡ�ɹ� ��������ȡʧ��	
** ����: 2017.12.8 by Hezhijie and Lizhengnian 
********************************************************************************************************/
int GetDailyWeather(void)
{
	bool sign;  
	
  ESP8266_Link_Server(enumTCP, (char*)WEATHER_IP, (char*)WEATHER_PORT,Single_ID_0);//TCP
	ESP8266_UnvarnishSend ();//����ģʽΪ��͸��	

	macESP8266_Usart ( "GET https://api.seniverse.com/v3/weather/daily.json?key=SiM5_bac6DvBWsUzi&location=%s&language=zh-Hans&unit=c\r\n\r\n",g_city);//Ĭ��Ϊ��������,��Ҫ����\r\n\r\n,����language��uintΪ��ѡ����

	strEsp8266_Fram_Record .InfBit .FramLength = 0;  //���¿�ʼ�����µ����ݰ�

	rt_thread_mdelay(1000);   //�ȴ���������
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';  //����λ
	printf("Receive Daily data:%s\r\n",strEsp8266_Fram_Record .Data_RX_BUF);  //�ض���  ����rt_kprintf����
	

	sign=cJSON_DailyWeatherParse((char*)strEsp8266_Fram_Record .Data_RX_BUF, &weather_data);	 //����δ��������������

	ESP8266_ExitUnvarnishSend ( );//�˳�͸��
  ESP8266_Close_Link (  );//�ر�TCP��UDP����  HZJ
																					
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,sizeof(strEsp8266_Fram_Record.Data_RX_BUF));  //�������
	return sign;
}

MSH_CMD_EXPORT(GetDailyWeather,Get DailyWeather Infomation);     //���������msh�����б�

/********************************************************************************************************
** ����: cJSON_LifeSuggestionParse, ��������ָ���������
**------------------------------------------------------------------------------------------------------
** ����: JSON:����ָ������������ݰ� WeatherType:�洢�����õ������õ�����
** ˵��: ������Դ����֪������api.seniverse.com��
** ����: 0:�����ɹ� ����������ʧ��
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
	json = cJSON_Parse(JSON); //����JSON���ݰ�
	if(json == NULL)		  //���JSON���ݰ��Ƿ�����﷨�ϵĴ��󣬷���NULL��ʾ���ݰ���Ч
	{
		printf("Error before: [%s]\n",cJSON_GetErrorPtr()); //��ӡ���ݰ��﷨�����λ��
		return 0;
	}
	else
	{
		if((arrayItem = cJSON_GetObjectItem(json,"results")) != NULL); //ƥ���ַ���"results",��ȡ��������
		{
			int size = cJSON_GetArraySize(arrayItem);     //��ȡ�����ж������
		  printf("--------------------------------Life Suggestion Data Begin------------------------------\r\n");
			if((object = cJSON_GetObjectItem(arrayItem,0)) != NULL)//��ȡ����������
			{
				/* ƥ���Ӷ���1 */
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
				/* ƥ���Ӷ���2 */
				if((subobject = cJSON_GetObjectItem(object,"suggestion")) != NULL)
				{
					printf("---------------------------------subobject2-------------------------------\r\n");
				 if((childobject = cJSON_GetObjectItem(subobject,"car_washing"))!= NULL)  //ϴ�� --��Ҫ�õ�������
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
				 if((childobject = cJSON_GetObjectItem(subobject,"flu"))!= NULL)         //���� --��Ҫ�õ�������
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
				if((childobject = cJSON_GetObjectItem(subobject,"dressing"))!= NULL)         //���� --��Ҫ�õ�������
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
				if((childobject = cJSON_GetObjectItem(subobject,"sport"))!= NULL)         //�˶�--��Ҫ�õ�������
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
				if((childobject = cJSON_GetObjectItem(subobject,"travel"))!= NULL)         //���� --��Ҫ�õ�������
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
				if((childobject = cJSON_GetObjectItem(subobject,"uv"))!= NULL)         //������ --��Ҫ�õ�������
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
				/* ƥ���Ӷ���3 */
				if((subobject = cJSON_GetObjectItem(object,"last_update")) != NULL)
				{
					printf("---------------------------------subobject3-------------------------------\r\n");
					printf("cJSON_GetObjectItem: %s:%s\r\n",subobject->string,subobject->valuestring);
				}
				printf("--------------------------------Life Suggestion Data End--------------------------------\r\n"); 
			}
		}
	}
			/* �ͷ��ڴ�� */
	myfree(SRAMIN,utf8str);
	myfree(SRAMIN,gbkstr);
	
	return 1;
}

/********************************************************************************************************
** ����: GETLifeSuggestion, GET ����ָ���������suggestion.json
**------------------------------------------------------------------------------------------------------
** ����: void
** ˵��: ������Դ����֪������api.seniverse.com��
** ����: 0:��ȡ�ɹ� ��������ȡʧ��
********************************************************************************************************/
int GetLifeSuggestion(void)
{
	bool sign;  
	
  ESP8266_Link_Server(enumTCP, (char*)WEATHER_IP, (char*)WEATHER_PORT,Single_ID_0);//TCP
	ESP8266_UnvarnishSend ();//����ģʽΪ��͸��	

	macESP8266_Usart ( "GET https://api.seniverse.com/v3/life/suggestion.json?key=SiM5_bac6DvBWsUzi&location=%s&language=zh-Hans&unit=c\r\n\r\n",g_city);//Ĭ��Ϊ��������,��Ҫ����\r\n\r\n,����language��uintΪ��ѡ����

	strEsp8266_Fram_Record .InfBit .FramLength = 0;  //���¿�ʼ�����µ����ݰ�

	rt_thread_mdelay(1000);   //�ȴ���������
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';  //����λ
	printf("Receive now data:%s\r\n",strEsp8266_Fram_Record .Data_RX_BUF);  //�ض���  ����rt_kprintf����

	sign=cJSON_LifeSuggestionParse((char*)strEsp8266_Fram_Record .Data_RX_BUF, &weather_data);	 //��������ָ���������

	ESP8266_ExitUnvarnishSend ( );//�˳�͸��
  ESP8266_Close_Link (  );//�ر�TCP��UDP����
	 																					
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,sizeof(strEsp8266_Fram_Record.Data_RX_BUF));  //�������
	
	return sign;
}

MSH_CMD_EXPORT(GetLifeSuggestion,Get Life Suggestion);     //���������msh�����б�

/***********************************************************************end***********************************************************************/
