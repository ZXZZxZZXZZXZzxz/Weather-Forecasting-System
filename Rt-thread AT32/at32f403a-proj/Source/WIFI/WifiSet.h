#ifndef __WIFI_SET_H__
#define __WIFI_SET_H__

/********************************** 用户需要设置的参数**********************************/
//要连接的热点的名称，即WIFI名称
//#define      macUser_ESP8266_ApSsid           "hc" 

//要连接的热点的密钥
//#define      macUser_ESP8266_ApPwd            "12345678" 



bool WifiSettings( char * pSSID, char * pPassWord );


#endif



