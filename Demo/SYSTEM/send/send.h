#ifndef  __SEND_H
#define	 __SEND_H



#include "stm32f10x.h"



/********************************** �û���Ҫ���õĲ���**********************************/
//Ҫ���ӵ��ȵ�����ƣ���WIFI����
#define      macUser_ESP8266_ApSsid           "1234" 
//#define      macUser_ESP8266_ApSsid           "Xiaomi_V4S48" 
//Ҫ���ӵ��ȵ����Կ
#define      macUser_ESP8266_ApPwd            "12345678" 

//Ҫ���ӵķ������� IP�������Ե�IP 
#define      macUser_ESP8266_TcpServer_IP     "192.168.0.102" 
//#define      macUser_ESP8266_TcpServer_IP     "10.0.2.12" 
//Ҫ���ӵķ������Ķ˿�
#define      macUser_ESP8266_TcpServer_Port    "80"  

/********************************** �ⲿȫ�ֱ��� ***************************************/
extern volatile uint8_t ucTcpClosedFlag;
/********************************** ���Ժ������� ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );

void packagepost(char *ip,char *port,char *page);

#endif
