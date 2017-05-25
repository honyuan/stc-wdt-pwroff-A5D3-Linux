#include "gpio.h"
#include "delayms.h"
#include "uart.h"

void GPIO_init(void)
{
	P3M0 = 0x02;
	P3M1 = 0x00;
	P5M0 = 0x30;
	P5M1 = 0x00;	 //P5.4 P5.5������� 
	PWREN  = 0;
	LCDON  = 0;
	PWRSIG = 1;		//����Ϊ��,���ڲ���ӡ����
	if(PCON&0x10) //ȡ��PCON.POF��λֵ,0001 0000
	{
		UART_String("cold reset\n\t\r");
		PCON &= 0xef; 
	}
	else 
	{
		//������
		Delay1500ms();  //��ʱ1.5s
		UART_String("hot reset!\n\t\r");
	}
}

