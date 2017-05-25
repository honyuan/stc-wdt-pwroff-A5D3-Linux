/* Define to prevent recursive inclusion */
#ifndef __GPIO_H
#define __GPIO_H

#include "stc15w2xx.h"
#include "intrins.h"

/*
	GPIO�����ļ�,��ʼ��
*/
/*default��
 PWREN 	= 0,��������;
 LCDON 	= 0,��������;
 PWRSIG = 0,��������;
*/
/*����ź�*/  
#define  PWREN 		P54
#define  LCDON 		P55
#define  PWRSIG 	P31

/*default��
 BTN = 1,��������;
 WDT = 1,�ߵ�ƽ���ܿ��Ź�,�͵�ƽʹ��;																	      
 PWROFF = 0,��������,�ߵ�ƽ����ػ�;
*/
/*�����ź�*/  
#define  PWROFF 	P30  
#define  WDT 		P32  
#define  BTN 		P33 

void GPIO_init(void);
#endif /* __GPIO_H */