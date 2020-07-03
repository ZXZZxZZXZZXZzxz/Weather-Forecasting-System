#include "config.h"
#include "NtpExample.h"
#include "time.h"


#define NTP_TIMESTAMP_DELTA 2208988800ull //number of seconds between 1900 and 1970，1900-1970的时间差
#define SEC_TIME_ZONE         + (8*60*60)  //Beijing,GMT+8， 时区差



/* 时间服务器--用于同步网络时间 */
#define TIME_SERVERIP	"ntp.rt-thread.org"//NTP服务器  
#define TIME_PORTNUM	"123"//NTP服务器端口号，固定为123

/************************************************************
*                     检测数组是否含有元素
*************************************************************/
unsigned char ArrayLenth=0;   //数组元素位置
static bool JudgeIsExsiEle(char * str,int lenth)
{
	if(lenth>0)
	{
    if(str[ArrayLenth]==0)
		{
			ArrayLenth++;
			return JudgeIsExsiEle(str,lenth-1);  //递归查询
		}
		else
		{
		  return true;     //数组含有元素
		}
	}
	else
    return false;      //不含有元素
}

/************************************************************
*                     获取时间
*************************************************************/
int GetNtp(void)
{
	uint8_t PosInNtpPacket;
	uint8_t buf[48];//存储NTP服务器返回的数据
	uint32_t local_timestamp; 
  ntp_packet packet ;
  struct tm * Net_time; 
  uint8_t NTP_Data[48]={0}; //48字节的报文
	
	/*******************************/
	//NTP查询报文赋初值
  NTP_Data[0]=0xa3; 
	//10100011, 0xa3,100 版本4
  //00011011, 0x1b,011 版本3
  //00010011, 0x13,010 版本2
  //00001011, 0x0b,001 版本1
	for(PosInNtpPacket=1;PosInNtpPacket<48;PosInNtpPacket++) NTP_Data[PosInNtpPacket]=0;//剩余的47字节为0
  /*******************************/
	if(!ESP8266_Link_Server(enumUDP, (char*)TIME_SERVERIP, (char*)TIME_PORTNUM,Single_ID_0))  //UDP连接
		return 0;  //失败
	
  rt_thread_mdelay(300);  //等待连接NTP服务器
	
	ESP8266_UnvarnishSend ();//传输模式为：透传	

	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;  																					//重新开始接收新的数据包
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,sizeof(strEsp8266_Fram_Record.Data_RX_BUF));  //清空数组
	
	for(PosInNtpPacket=0;PosInNtpPacket<48;PosInNtpPacket++) //发送NTP查询包
		UsartSendByte(macESP8266_USARTx,NTP_Data[PosInNtpPacket]);

	rt_thread_mdelay(5000);
	
	if(!JudgeIsExsiEle(strEsp8266_Fram_Record.Data_RX_BUF,sizeof(strEsp8266_Fram_Record.Data_RX_BUF)))   //检测数组
		return 0;
	
	rt_memcpy(buf,strEsp8266_Fram_Record.Data_RX_BUF,48);
	
	/*******************************/
	//接收包原始数据打印
	printf("/*******************************/\r\n");
	printf("Receive NTP Packet (Hex):");
	for(PosInNtpPacket=0;PosInNtpPacket<48;PosInNtpPacket++)
		if(PosInNtpPacket%16==0)
		{
			printf("\r\n");
			printf("%02X ",buf[PosInNtpPacket]);
		}
		else
			printf("%02X ",buf[PosInNtpPacket]);

	printf("\r\n/*******************************/\r\n");
	/*******************************/
	packet.txTm_s = buf[40]<<24 | buf[40+1]<<16|buf[40+2]<<8 |buf[40+3];//时间差offset=（（T2-T1）+（T3-T4））/2。而是用T3作为对时基准时间。  
  local_timestamp = packet.txTm_s - NTP_TIMESTAMP_DELTA;//减去1970和1900的差值
  local_timestamp +=SEC_TIME_ZONE; //加上北京的时间差，GMT+8
  Net_time = localtime(&local_timestamp); //秒数转换位标准时间

	RTC_Set((Net_time->tm_year)+1900, (Net_time->tm_mon)+1, Net_time->tm_mday, Net_time->tm_hour,Net_time->tm_min,Net_time->tm_sec);
	printf("NTP Time:%04d-%02d-%02d %02d:%02d:%02d\r\n",(Net_time->tm_year)+1900, (Net_time->tm_mon)+1, Net_time->tm_mday, Net_time->tm_hour,Net_time->tm_min,Net_time->tm_sec); //打印出时间
  
	ESP8266_ExitUnvarnishSend ( );//退出透传HZJ
  ESP8266_Close_Link (  );//关闭TCP或UDP连接HZJ
		
	strEsp8266_Fram_Record .InfBit .FramLength = 0;  																					//重新开始接收新的数据包
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,sizeof(strEsp8266_Fram_Record.Data_RX_BUF));  //清空数组
	memset(buf,0,sizeof(buf));  //清空
		
	return 1;   //成功
}

uint8 AccessNTP(void)
{
  uint8 sign;
	sign=GetNtp();   //获取时间
	
	if(sign==0)    //防止丢包
	{
		rt_kprintf("NTP Access failed!\r\n");  
	  return AccessNTP();   //重新获取获取  递归
	}
	else
	{
		rt_kprintf("NTP Access Successful!\r\n");  
		return 1;         //访问成功
	}

}
MSH_CMD_EXPORT(AccessNTP, Get NTP Time);
