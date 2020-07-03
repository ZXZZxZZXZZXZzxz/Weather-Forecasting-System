#ifndef __WEATHER_INFO__
#define __WEATHER_INFO__

/* 天气数据结构体 */
typedef struct
{
	/* 实况天气数据 */
	char name[32];							//地名
	char text[32];							//天气预报文字
	char code[32];				 			//天气预报代码
	char temperature[32];   	  //气温
	
	/* 今天、明天、后天天气数据 */
	char date[3][32];			      //日期
	char text_day[3][64];	      //白天天气现象文字
	char code_day[3][32];		    //白天天气现象代码
	char text_night[3][64];     //晚间天气现象文字
	char code_night[3][64]; 	  //晚间天气现象代码
	char high[3][32];			      //最高温
	char low[3][32];			      //最低温
	char rainfall[3][32];       //降雨量
	char wind_direction[3][64]; //风向
	char wind_speed[3][32];  	  //风速，单位km/h（当unit=c时）
	char wind_scale[3][32];  	  //风力等级
	char humidity[3][32];       //湿度

	/*  0 洗车 
	    1 流感
			2 穿搭
			3 运动
			4 旅行
			5 紫外线*/
	char life_suggestion[6][64];	//生活建议
}WeatherType;

extern WeatherType weather_data;

int GetTest(void);    //测试
int GetNowWeather(void);     //访问实况天气数据
int GetDailyWeather(void);   //访问三天天气数据
int GetLifeSuggestion(void); //访问生活建议数据

/* SwitchToGbk函数在utf8togbk.c中    转换中文文字格式为GBK */
extern int SwitchToGbk(const unsigned char* pszBufIn, int nBufInLen, unsigned char* pszBufOut, int* pnBufOutLen);
#endif
