#include "config.h"
#include "NtpExample.h"
#include "time.h"


#define NTP_TIMESTAMP_DELTA 2208988800ull //number of seconds between 1900 and 1970��1900-1970��ʱ���
#define SEC_TIME_ZONE         + (8*60*60)  //Beijing,GMT+8�� ʱ����



/* ʱ�������--����ͬ������ʱ�� */
#define TIME_SERVERIP	"ntp.rt-thread.org"//NTP������  
#define TIME_PORTNUM	"123"//NTP�������˿ںţ��̶�Ϊ123

/************************************************************
*                     ��������Ƿ���Ԫ��
*************************************************************/
unsigned char ArrayLenth=0;   //����Ԫ��λ��
static bool JudgeIsExsiEle(char * str,int lenth)
{
	if(lenth>0)
	{
    if(str[ArrayLenth]==0)
		{
			ArrayLenth++;
			return JudgeIsExsiEle(str,lenth-1);  //�ݹ��ѯ
		}
		else
		{
		  return true;     //���麬��Ԫ��
		}
	}
	else
    return false;      //������Ԫ��
}

/************************************************************
*                     ��ȡʱ��
*************************************************************/
int GetNtp(void)
{
	uint8_t PosInNtpPacket;
	uint8_t buf[48];//�洢NTP���������ص�����
	uint32_t local_timestamp; 
  ntp_packet packet ;
  struct tm * Net_time; 
  uint8_t NTP_Data[48]={0}; //48�ֽڵı���
	
	/*******************************/
	//NTP��ѯ���ĸ���ֵ
  NTP_Data[0]=0xa3; 
	//10100011, 0xa3,100 �汾4
  //00011011, 0x1b,011 �汾3
  //00010011, 0x13,010 �汾2
  //00001011, 0x0b,001 �汾1
	for(PosInNtpPacket=1;PosInNtpPacket<48;PosInNtpPacket++) NTP_Data[PosInNtpPacket]=0;//ʣ���47�ֽ�Ϊ0
  /*******************************/
	if(!ESP8266_Link_Server(enumUDP, (char*)TIME_SERVERIP, (char*)TIME_PORTNUM,Single_ID_0))  //UDP����
		return 0;  //ʧ��
	
  rt_thread_mdelay(300);  //�ȴ�����NTP������
	
	ESP8266_UnvarnishSend ();//����ģʽΪ��͸��	

	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;  																					//���¿�ʼ�����µ����ݰ�
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,sizeof(strEsp8266_Fram_Record.Data_RX_BUF));  //�������
	
	for(PosInNtpPacket=0;PosInNtpPacket<48;PosInNtpPacket++) //����NTP��ѯ��
		UsartSendByte(macESP8266_USARTx,NTP_Data[PosInNtpPacket]);

	rt_thread_mdelay(5000);
	
	if(!JudgeIsExsiEle(strEsp8266_Fram_Record.Data_RX_BUF,sizeof(strEsp8266_Fram_Record.Data_RX_BUF)))   //�������
		return 0;
	
	rt_memcpy(buf,strEsp8266_Fram_Record.Data_RX_BUF,48);
	
	/*******************************/
	//���հ�ԭʼ���ݴ�ӡ
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
	packet.txTm_s = buf[40]<<24 | buf[40+1]<<16|buf[40+2]<<8 |buf[40+3];//ʱ���offset=����T2-T1��+��T3-T4����/2��������T3��Ϊ��ʱ��׼ʱ�䡣  
  local_timestamp = packet.txTm_s - NTP_TIMESTAMP_DELTA;//��ȥ1970��1900�Ĳ�ֵ
  local_timestamp +=SEC_TIME_ZONE; //���ϱ�����ʱ��GMT+8
  Net_time = localtime(&local_timestamp); //����ת��λ��׼ʱ��

	RTC_Set((Net_time->tm_year)+1900, (Net_time->tm_mon)+1, Net_time->tm_mday, Net_time->tm_hour,Net_time->tm_min,Net_time->tm_sec);
	printf("NTP Time:%04d-%02d-%02d %02d:%02d:%02d\r\n",(Net_time->tm_year)+1900, (Net_time->tm_mon)+1, Net_time->tm_mday, Net_time->tm_hour,Net_time->tm_min,Net_time->tm_sec); //��ӡ��ʱ��
  
	ESP8266_ExitUnvarnishSend ( );//�˳�͸��HZJ
  ESP8266_Close_Link (  );//�ر�TCP��UDP����HZJ
		
	strEsp8266_Fram_Record .InfBit .FramLength = 0;  																					//���¿�ʼ�����µ����ݰ�
	memset(strEsp8266_Fram_Record.Data_RX_BUF,0,sizeof(strEsp8266_Fram_Record.Data_RX_BUF));  //�������
	memset(buf,0,sizeof(buf));  //���
		
	return 1;   //�ɹ�
}

uint8 AccessNTP(void)
{
  uint8 sign;
	sign=GetNtp();   //��ȡʱ��
	
	if(sign==0)    //��ֹ����
	{
		rt_kprintf("NTP Access failed!\r\n");  
	  return AccessNTP();   //���»�ȡ��ȡ  �ݹ�
	}
	else
	{
		rt_kprintf("NTP Access Successful!\r\n");  
		return 1;         //���ʳɹ�
	}

}
MSH_CMD_EXPORT(AccessNTP, Get NTP Time);
