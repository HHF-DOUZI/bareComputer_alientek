#include "main.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "exti.h"
#include "bsp_epit.h"
#include "keyfilter.h"
#include "bsp_uart.h"
#include "bsp_lcd.h"
#include "bsp_rtc.h"
#include "bsp_lcdapi.h"
#include "stdio.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned char key = 0;
	int i = 3, t = 0;
	char buf[160];
	struct rtc_datetime rtcdate;
	unsigned char state = OFF;
	
	int_init();			/* 中断初始化 */
	imx6u_clk_init();	/* 初始化时钟 */
	uart_init();		/* 初始化串口 */
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();			/* 初始化led 			*/
	beep_init();		/* 初始化beep	 		*/
	key_init();			/* 初始化按键 */
	delay_init();		/* 高精度延时GPT1定时器 */
	lcd_init();			/* LCD初始化 */
	rtc_init();			/* RTC初始化 */

    tftlcd_dev.forecolor = LCD_RED;
    tftlcd_dev.backcolor = LCD_WHITE;
	tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"ZERO-IMX6UL RTC TEST");    /* 显示字符串 */
	lcd_show_string(50, 40, 200, 16, 16, (char*)"ATOM@ALIENTEK");  
	lcd_show_string(50, 60, 200, 16, 16, (char*)"2019/3/27");  
	tftlcd_dev.forecolor = LCD_BLUE;
	memset(buf, 0, sizeof(buf));
	
	while(1)
	{
		if(t==100)	//1s时间到了
		{
			t=0;
			printf("will be running %d s......\r", i);
			
			// lcd_fill(50, 90, 370, 110, tftlcd_dev.backcolor); /* 清屏 */
			sprintf(buf, "will be running %ds......", i);
			// lcd_show_string(50, 90, 300, 16, 16, buf); 
			i--;
			if(i < 0)
				break;
		}

		key = key_getValue();
		if(key == KEY0_VALUE)
		{
			rtcdate.year = 2022;
   			rtcdate.month = 7;
    		rtcdate.day = 4;
    		rtcdate.hour = 15;
    		rtcdate.minute = 02;
    		rtcdate.second = 0;
			rtc_setdatetime(&rtcdate); /* 初始化时间和日期 */
			printf("\r\n RTC Init finish\r\n");
			break;
		}
			
		delay_ms(10);
		t++;
	}
	tftlcd_dev.forecolor = LCD_RED;
	// lcd_fill(50, 90, 370, 110, tftlcd_dev.backcolor); /* 清屏 */
	// lcd_show_string(50, 90, 200, 16, 16, (char*)"Current Time:");  			/* 显示字符串 */
	tftlcd_dev.forecolor = LCD_BLUE;

	while(1)					
	{	
		rtc_getdatetime(&rtcdate);
		sprintf(buf,"%d/%d/%d %d:%d:%d",rtcdate.year, rtcdate.month, rtcdate.day, rtcdate.hour, rtcdate.minute, rtcdate.second);
		// lcd_fill(50,110, 300,130, tftlcd_dev.backcolor);
		// lcd_show_string(50, 110, 250, 16, 16,(char*)buf);  /* 显示字符串 */
		printf("%d/%d/%d %d:%d:%d\r\n",rtcdate.year, rtcdate.month, rtcdate.day, rtcdate.hour, rtcdate.minute, rtcdate.second);
		
		state = !state;
		led_switch(LED0,state);
		delay_ms(1000);	/* 延时一秒 */
	}
	return 0;
}