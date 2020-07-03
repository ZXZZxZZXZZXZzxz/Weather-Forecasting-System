#include "config.h"
#include "WifiSet.h"

/****************************************************************
* ������ WifiSettings   ����WIFI
* ������pSSID Ҫ���ӵ��ȵ�����ƣ���WIFI���� 
        pPassWord  Ҫ���ӵ��ȵ����Կ
****************************************************************/
bool WifiSettings( char * pSSID, char * pPassWord )
{
	macESP8266_CH_ENABLE();//����ESP8266
	
	ESP8266_AT_Test();//ATָ�����
	
	ESP8266_Net_Mode_Choose ( STA );//ģʽѡ��

  while ( ! ESP8266_JoinAP ( pSSID, pPassWord ) );	//����WIFI
	
	ESP8266_Enable_MultipleId ( DISABLE );//�Ƿ������	
	return 1;
}

