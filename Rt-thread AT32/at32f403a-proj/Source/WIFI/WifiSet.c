#include "config.h"
#include "WifiSet.h"

/****************************************************************
* 函数： WifiSettings   设置WIFI
* 参数：pSSID 要连接的热点的名称，即WIFI名称 
        pPassWord  要连接的热点的密钥
****************************************************************/
bool WifiSettings( char * pSSID, char * pPassWord )
{
	macESP8266_CH_ENABLE();//启用ESP8266
	
	ESP8266_AT_Test();//AT指令测试
	
	ESP8266_Net_Mode_Choose ( STA );//模式选择

  while ( ! ESP8266_JoinAP ( pSSID, pPassWord ) );	//连接WIFI
	
	ESP8266_Enable_MultipleId ( DISABLE );//是否多连接	
	return 1;
}

