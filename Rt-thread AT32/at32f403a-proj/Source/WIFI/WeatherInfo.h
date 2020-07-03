#ifndef __WEATHER_INFO__
#define __WEATHER_INFO__

/* �������ݽṹ�� */
typedef struct
{
	/* ʵ���������� */
	char name[32];							//����
	char text[32];							//����Ԥ������
	char code[32];				 			//����Ԥ������
	char temperature[32];   	  //����
	
	/* ���졢���졢������������ */
	char date[3][32];			      //����
	char text_day[3][64];	      //����������������
	char code_day[3][32];		    //���������������
	char text_night[3][64];     //���������������
	char code_night[3][64]; 	  //��������������
	char high[3][32];			      //�����
	char low[3][32];			      //�����
	char rainfall[3][32];       //������
	char wind_direction[3][64]; //����
	char wind_speed[3][32];  	  //���٣���λkm/h����unit=cʱ��
	char wind_scale[3][32];  	  //�����ȼ�
	char humidity[3][32];       //ʪ��

	/*  0 ϴ�� 
	    1 ����
			2 ����
			3 �˶�
			4 ����
			5 ������*/
	char life_suggestion[6][64];	//�����
}WeatherType;

extern WeatherType weather_data;

int GetTest(void);    //����
int GetNowWeather(void);     //����ʵ����������
int GetDailyWeather(void);   //����������������
int GetLifeSuggestion(void); //�������������

/* SwitchToGbk������utf8togbk.c��    ת���������ָ�ʽΪGBK */
extern int SwitchToGbk(const unsigned char* pszBufIn, int nBufInLen, unsigned char* pszBufOut, int* pnBufOutLen);
#endif
