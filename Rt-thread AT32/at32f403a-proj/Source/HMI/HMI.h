#ifndef __HMI_H__
#define __HMI_H__

#include "WeatherInfo.h"
#include "HMIrtc.h"

   /* WIFIͼ������� */
#define WIFI_Icon_X		315
#define WIFI_Icon_Y		2

  /* Loadͼ������ */
#define Load_Icon_X		168
#define Load_Icon_Y		96

  /* ��ǰ����ͼ������� */
#define Now_Icon_X		340
#define Now_Icon_Y		44

 /* �����������ͼ�������D */
#define Today_day_Icon_X		46
#define Today_day_Icon_Y		43

 /* ����ҹ������ͼ������� */
#define Today_night_Icon_X		46
#define Today_night_Icon_Y		155

/* �����������ͼ������� */
#define Tomorrow_day_Icon_X		176
#define Tomorrow_day_Icon_Y		43

/* ����ҹ������ͼ������� */
#define Tomorrow_night_Icon_X		176
#define Tomorrow_night_Icon_Y		155

/* �����������ͼ������� */
#define AfterTomor_day_Icon_X	 306
#define AfterTomor_day_Icon_Y	 43

/* ����ҹ������ͼ������꼰ͼƬID */
#define AfterTomor_night_Icon_X	306
#define AfterTomor_night_Icon_Y	155

/* ��������ҳ���ʶ */
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

/* �������� */
void Task_SendHMI_WeaData(WeatherType weather);
void DisplayWeatherIcon(WeatherType weather);
void DisplayWifiIcon(unsigned char sign);
void HMI_RePrecess(void);

#endif


