#ifndef __HMIRTC_H
#define __HMIRTC_H

#include "config.h"

/* �����ṹ�� */
typedef struct 
{
	uint16 w_year;
	uint8  w_month;
	uint8  w_date;
	uint8  hour;
	uint8  min;
	uint8  sec;
	uint8  week;
}Calendar_T;

extern Calendar_T calendar_t;//����һ���������ṹ�塱Ϊ�ⲿ����
extern char *wday[];		 //����һ���������ڵ�ָ���������

/* ��ʼ��RTC,����0���ɹ���������1��ʧ�ܣ� */
uint8 RTC_Init(void);

/* ƽ�������ж� */
uint8 IsLeapYear(uint16 year);

/* ���ʱ�� */
uint8 RTC_Get(void);
 
/* ������� */
uint8 RTC_GetWeek(uint16 year,uint8 month,uint8 day);

/* ����ʱ�� */
uint8 RTC_Set(uint16 year,uint8 mon,uint8 day,uint8 hour,uint8 min,uint8 sec);	

/* ��ӡʱ���������� */
void DisplayTime(Calendar_T time);
void DisplayRTCTime(Calendar_T calendar);
#endif

