#include "gpio.h"
#include "delayms.h"
#include "uart.h"

void GPIO_init(void)
{
	P3M0 = 0x02;
	P3M1 = 0x00;
	P5M0 = 0x30;
	P5M1 = 0x00;	 //P5.4 P5.5推挽输出 
	PWREN  = 0;
	LCDON  = 0;
	PWRSIG = 1;		//他不为高,串口不打印数据
	if(PCON&0x10) //取出PCON.POF这位值,0001 0000
	{
		UART_String("cold reset\n\t\r");
		PCON &= 0xef; 
	}
	else 
	{
		//热启动
		Delay1500ms();  //延时1.5s
		UART_String("hot reset!\n\t\r");
	}
}

