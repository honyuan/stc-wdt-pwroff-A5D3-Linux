/* Define to prevent recursive inclusion */
#ifndef __GPIO_H
#define __GPIO_H

#include "stc15w2xx.h"
#include "intrins.h"

/*
	GPIO配置文件,初始化
*/
/*default：
 PWREN 	= 0,电阻下拉;
 LCDON 	= 0,电阻下拉;
 PWRSIG = 0,电阻下拉;
*/
/*输出信号*/  
#define  PWREN 		P54
#define  LCDON 		P55
#define  PWRSIG 	P31

/*default：
 BTN = 1,电阻上拉;
 WDT = 1,高电平禁能看门狗,低电平使能;																	      
 PWROFF = 0,电阻下拉,高电平请求关机;
*/
/*输入信号*/  
#define  PWROFF 	P30  
#define  WDT 		P32  
#define  BTN 		P33 

void GPIO_init(void);
#endif /* __GPIO_H */