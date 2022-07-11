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
#include "bsp_ap3216c.h"
#include "stdio.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned short ir, ps, als;
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
	ap3216c_init();		/* AP3216C初始化 */

    tftlcd_dev.forecolor = LCD_RED;
    tftlcd_dev.backcolor = LCD_WHITE;
	tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"hhf RTC TEST");    /* 显示字符串 */
	lcd_show_string(50, 40, 200, 16, 16, (char*)"ATOM@ALIENTEK");  
	lcd_show_string(50, 60, 200, 16, 16, (char*)"2022/7/6");  
	tftlcd_dev.forecolor = LCD_BLUE;

	while(1)					
	{	
		ap3216c_readdata(&ir, &ps, &als);
		printf("ir = %d ps = %d als = %d\r\n", ir, ps, als);

		state = !state;
		led_switch(LED0,state);
		delay_ms(1000);	/* 延时一秒 */
	}
	return 0;
}