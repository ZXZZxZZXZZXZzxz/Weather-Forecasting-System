#ifndef __HMI_H__
#define __HMI_H__

#include "WeatherInfo.h"
#include "HMIrtc.h"

   /* WIFI图标的坐标 */
#define WIFI_Icon_X		315
#define WIFI_Icon_Y		2

  /* Load图标坐标 */
#define Load_Icon_X		168
#define Load_Icon_Y		96

  /* 当前天气图标的坐标 */
#define Now_Icon_X		340
#define Now_Icon_Y		44

 /* 今天白天天气图标的坐标D */
#define Today_day_Icon_X		46
#define Today_day_Icon_Y		43

 /* 今天夜里天气图标的坐标 */
#define Today_night_Icon_X		46
#define Today_night_Icon_Y		155

/* 明天白天天气图标的坐标 */
#define Tomorrow_day_Icon_X		176
#define Tomorrow_day_Icon_Y		43

/* 明天夜里天气图标的坐标 */
#define Tomorrow_night_Icon_X		176
#define Tomorrow_night_Icon_Y		155

/* 后天白天天气图标的坐标 */
#define AfterTomor_day_Icon_X	 306
#define AfterTomor_day_Icon_Y	 43

/* 后天夜里天气图标的坐标及图片ID */
#define AfterTomor_night_Icon_X	306
#define AfterTomor_night_Icon_Y	155

/* 串口屏的页面标识 */
extern char g_page[32];
extern char g_update[32];
extern char g_place[32];
extern char g_place_utf8[32];
extern char g_city[32];
extern int utf8_len;
extern char g_wifi_ssid[32];
extern char g_wifi_password[32];

extern char g_time_hour[8];
extern char g_time_min[8];
extern char g_time_sec[8];

/* 函数声明 */
void Task_SendHMI_WeaData(WeatherType weather);
void DisplayWeatherIcon(WeatherType weather);
void DisplayWifiIcon(unsigned char sign);
void HMI_RePrecess(void);

#endif


