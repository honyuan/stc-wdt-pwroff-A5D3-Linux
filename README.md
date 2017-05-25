# stc-wdt-pwroff-A5D3-Linux
STC is a watch dog of  A5D3 Linux

PWREN-1<--|+++++|--8<-BTN
3V3  -2 --|	    |--7<-WDT
LCDON-3<--|	    |--6>-PWRSIG
GND-  4 --|+++++|--5<-PWROFF
	
WDT = 0,边沿喂狗,低电平使能;																	      
PWROFF = 0,低电平请求关机;
