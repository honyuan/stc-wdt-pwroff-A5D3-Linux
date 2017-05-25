#include "delayms.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"


uchar tm0_flag = 0; 
uchar tm0_count = 0;
uchar wdt_flag = 0;


/*****************************************/
/*�жϷ������б�*/
/*****************************************/
/*****�ⲿ�ж�0,WDT******/
void extint0() interrupt 0  using 0			
{
	    EX0 =0;
	   	wdt_flag = 1;
		EX0 =1;
}

/*****timer0��ʱ��0�ж�******/
/***10����@6.000MHz,�ж���һ��***/
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


/*���ڽ����ж�,���ڷ��Ͷ˿ڱ���Ҫ����������*/
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
/*�жϷ�������ʼ��*/
/*************/
void NVIC_init(void)
{
	 EX0=1;    			//�����ⲿ�ж� 0
	 IT0=1;        	 	//1Ϊ�½��ش���
//	 EX1=1;    			//�����ⲿ�ж� 1
//	 IT1=1;         	//1Ϊ�½��ش���
//INT_CLKO |= 0x40;	//���ⲿ�ж�4,ʹ���½����ж�
	 EA=1;   			//�������ж�
}
/********************************************
�������ƣ�void clrWatchdog( void )
����������ι���Ź���ÿ9sιһ�Ρ�
����˵�����̶�9sιһ�Σ�����û��Ҫ������
����˵������
************************************************/
void ClearWDT( void )
{
    WDT_CONTR = 0x37;    
}

/*CPU ���Ŷ���*/
/*-----------------------------
	PWREN-1<--|+++++|--8<-BTN
	3V3  -2 --|	    |--7<-WDT
	LCDON-3<--|	    |--6>-PWRSIG
	GND-  4 --|+++++|--5<-PWROFF
	
 WDT = 0,����ι��,�͵�ƽʹ��;																	      
 PWROFF = 0,�͵�ƽ����ػ�;

------------------------------*/
/*���������*/

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
	Delay1500ms();  //��ʱ1.5s
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
			WDT = 1;�ߵ�ƽ��ֹ���Ź�����
			��ֹTimer0��ʱ
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
			WDT = 0;�͵�ƽʹ�ܿ��Ź�����
			��ʼTimer0��ʱ
		***/		
		if(!WDT)
		{
			ClearWDT();
			if(tm0_count >=14)
				{
					/*û��ι������λ��Դ*/
					UART_String("dog is dead! \t\r\n");
					LCDON = 0;
					PWREN = 0;
					Delay1500ms();  //��ʱ1.5s
					PWREN = 1;
					Delay1500ms();  //��ʱ1.5s
					LCDON = 1;
					UART_String("dog is relive! \t\r\n");
					tm0_flag = 0;
					tm0_count = 0;
				}
					
		}

		/*reboot,PWROFF=0 reboot���� */
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
							Delay1500ms();  //��ʱ1.5s
							PWREN = 1;
							Delay1500ms();  //��ʱ1.5s
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
			
