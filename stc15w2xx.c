#include "delayms.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"


uchar tm0_flag = 0; 
uchar tm0_count = 0;
uchar wdt_flag = 0;


/*****************************************/
/*中断服务函数列表*/
/*****************************************/
/*****外部中断0,WDT******/
void extint0() interrupt 0  using 0			
{
	    EX0 =0;
	   	wdt_flag = 1;
		EX0 =1;
}

/*****timer0定时器0中断******/
/***10毫秒@6.000MHz,中断来一次***/
void timer0int() interrupt 1 using 2	
{
	tm0_flag++; 
	if(	tm0_flag >=250)
	{
		tm0_flag = 0; 
		tm0_count++;
		if(tm0_count >=20)
		{ 
			tm0_count = 0;
		}
	}
}


/*串口接收中断,串口发送端口必须要接上拉电阻*/
void UART_Interrupt_Receive( void ) interrupt 4
{
    if(RI)
    {
        RI = 0;
    }
    else
    {
        TI = 0;
    }
}
/**************/
/*中断服务函数初始化*/
/*************/
void NVIC_init(void)
{
	 EX0=1;    			//开启外部中断 0
	 IT0=1;        	 	//1为下降沿触发
//	 EX1=1;    			//开启外部中断 1
//	 IT1=1;         	//1为下降沿触发
//INT_CLKO |= 0x40;	//打开外部中断4,使能下降沿中断
	 EA=1;   			//开启总中断
}
/********************************************
函数名称：void clrWatchdog( void )
功能描述：喂看门狗，每9s喂一次。
参数说明：固定9s喂一次，所以没有要参数。
返回说明：无
************************************************/
void ClearWDT( void )
{
    WDT_CONTR = 0x37;    
}

/*CPU 引脚定义*/
/*-----------------------------
	PWREN-1<--|+++++|--8<-BTN
	3V3  -2 --|	    |--7<-WDT
	LCDON-3<--|	    |--6>-PWRSIG
	GND-  4 --|+++++|--5<-PWROFF
	
 WDT = 0,边沿喂狗,低电平使能;																	      
 PWROFF = 0,低电平请求关机;

------------------------------*/
/*主函数入口*/

void main(void)
{
	UartInit();	
	UART_String("hello! \t\r\n");
	GPIO_init();
	Delay50ms();
	if(!WDT)
	{
	  Timer0Init();	
	}
	PWREN  = 1;
	Delay1500ms();  //延时1.5s
	LCDON  = 1;
	tm0_flag = 0;
	tm0_count = 0;
	NVIC_init();
	ClearWDT();
	UART_String("start! \t\r\n");
    while(1)
	{	
		ClearWDT();
		/***
			WDT = 1;高电平禁止看门狗功能
			禁止Timer0计时
		***/
		if(wdt_flag)
		{
			tm0_flag = 0;
			tm0_count = 0;
			wdt_flag = 0;
			UART_String("dog is full! \t\r\n");	
			ClearWDT();
		}
	
		/***
			WDT = 0;低电平使能看门狗功能
			开始Timer0计时
		***/		
		if(!WDT)
		{
			ClearWDT();
			if(tm0_count >=14)
				{
					/*没有喂狗，则复位电源*/
					UART_String("dog is dead! \t\r\n");
					LCDON = 0;
					PWREN = 0;
					Delay1500ms();  //延时1.5s
					PWREN = 1;
					Delay1500ms();  //延时1.5s
					LCDON = 1;
					UART_String("dog is relive! \t\r\n");
					tm0_flag = 0;
					tm0_count = 0;
				}
					
		}

		/*reboot,PWROFF=0 reboot触发 */
		while(!PWROFF)
		{
				ClearWDT();
				UART_String("pwroff check 1 \t\r\n");
				Delay50ms();
				if(!PWROFF)
				{
					UART_String("pwroff check 2 \t\r\n");
					Delay50ms();
					if(!PWROFF)
						{
							ClearWDT();
							UART_String("pwroff is on \t\r\n");
							LCDON = 0;
							PWREN = 0;
							Delay1500ms();  //延时1.5s
							PWREN = 1;
							Delay1500ms();  //延时1.5s
							LCDON = 1;
							UART_String("reboot is done \t\r\n");
							tm0_flag = 0;
							tm0_count = 0;
						}
				}
				ClearWDT();
		}
	}
}
			
