#include "send.h"

#include "SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "device.h"
#include "usart2.h"
 #include "readconfig.h" 
#include "iic.h"
#include "rtc.h"
#include "mod.h"
#include "string.h"
//#include "sim800.h"
/************************************************
*数据通过wifi传输到服务器
************************************************/
extern usart_st  usart_data;
extern uint64_t datanum;
extern Device_Info device;
uint16_t address;

char senddata[320];	 
static char rx_buf_str[255];  //串口数据字符串格式
static char body_data_1[160];        //需发送数据
static char body_data_2[50];        //需发送数据
char body_data_len=0;
char jsondata[500];
char postdata[380];
extern char coredata[200];
int WroteAddr=100;
uint8_t WriteBuf[212]={"qwertyuiop[]asdfghjkl;'zxcvbqwertyunm,./qwertyuiop[]asdfghjkl;'zxcvbnm,./qwertyuiop[]asdfghjkl;'zxcvbqwertyunm,./qwertyuiop[]asdfghjkl;'zxcvbnm,./qwertyuiop[]asdfghjkl;'zxcvbnm,./qwertyuiop[]asdfghjkl;'zxcvbnm,./"};

extern  struct Data_Time  timer;
extern  struct NEW_Time newtimer;
 extern dev_configration dev_config; 
volatile uint8_t ucTcpClosedFlag = 0;


 /*字节流转字符串
**参数：  source源数组
**     dest  目标字符串
**     sourceLen长度
*/
 void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)  
{  
    short i;  
    unsigned char highByte, lowByte;  
  
    for (i = 0; i < sourceLen; i++)  
    {  
        highByte = source[i] >> 4;  
        lowByte = source[i] & 0x0f ;  
  
        highByte += 0x30;  
  
        if (highByte > 0x39)  
                dest[i * 2] = highByte + 0x07;  
        else  
                dest[i * 2] = highByte;  
  
        lowByte += 0x30;  
        if (lowByte > 0x39)  
            dest[i * 2 + 1] = lowByte + 0x07;  
        else  
            dest[i * 2 + 1] = lowByte;  
    }  
    return ;  
}  






//数据内容打包
/* 函数名：packagedata()
** 描述：将需要发送的参数转成json
*/
void packagedata()
{
	 //字节流转换
     ByteToHexStr(usart_data.rx_buf,rx_buf_str,usart_data.rx_len_ed); 
	  //计算需截取的长度  
	  body_data_len=usart_data.rx_buf[2]*2;
	  //待发送数据截取  
	  strncpy(body_data_1,rx_buf_str+6,body_data_len); 
	  Time_Get();  
	 	sprintf(coredata, "data:\"%s\",\
	  \r\n time:\"%d.%02d.%02d %02d:%02d:%02d\"",body_data_1,timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec);
}

//要发送的数据进行打包
void packagepost(char *ip,char *port,char *msg)
{
   char content_page[50];
	 char content_len[300];
	 char content_host[50];
	 char content_type[] = "Content-Type:application/json-patch+json\r\n";
	 address=usart_data.tx_buf[3]+40001;
	sprintf(senddata,"{\r\n machineid:\"%s\",\r\n simid:\"%s\",\r\n%s\r\nlng:%s\r\nlat:%s\r\n}",device.machineid,device.simid,msg,device.lng,device.lat);
	  //sprintf(content_len,"Content-Length: %d\r\n",strlen(senddata));
    sprintf(postdata,"%s",senddata);
   	
}
/**
** 函数描述：向eeprom指定地址写入制定长度字符串
**
**
**
*/

void write()
{
	  int size;
		int i=0;
  	size=sizeof(coredata);
	//size=212;
		if((size+WroteAddr)<=32765)
		{
				for(i=0;i<size;i++)
			{
			  E2promWriteByte(WroteAddr+i,coredata[i]);
				delay_ms(15);
			}
			WroteAddr+=size;
			BSP_Printf("写入应急rom\r\n");
		}
		else if((size+WroteAddr)>32765)
		{
		   WroteAddr=100;
				for(i=0;i<size;i++)
				{
					E2promWriteByte(WroteAddr+i,coredata[i]);
				}
				BSP_Printf("写满了,从头写\r\n");
		}
}

void clear()
{
memset(body_data_1,0,20);
memset(postdata,0,380);
memset(senddata,0,320);
}
/*
// C prototype : void StrToHex(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - 输出缓冲区
// [IN] pbSrc - 字符串
// [IN] nLen - 16进制数的字节数(字符串的长度/2)
// return value:
// remarks : 将字符串转化为16进制数
*/
void StrToHex(u8 *pbDest, u8 *pbSrc, int nLen)
{
char h1,h2;
u8 s1,s2;
int i;

for (i=0; i<nLen; i++)
{
h1 = pbSrc[2*i];
h2 = pbSrc[2*i+1];

s1 = toupper(h1) - 0x30;
if (s1 > 9)
s1 -= 7;

s2 = toupper(h2) - 0x30;
if (s2 > 9)
s2 -= 7;

pbDest[i] = s1*16 + s2;
}
}
