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
#include "bsp_lcdapi.h"
#include "stdio.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	static unsigned char state = 0;
	
	int_init();			/* 中断初始化 */
	imx6u_clk_init();	/* 初始化时钟 */
	uart_init();		/* 初始化串口 */
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();			/* 初始化led 			*/
	beep_init();		/* 初始化beep	 		*/
	key_init();			/* 初始化按键 */
	delay_init();		/* 高精度延时GPT1定时器 */
	lcd_init();			/* LCD初始化 */

	lcd_clear(LCD_WHITE);
	lcd_drawPoint(0, 0, LCD_RED);
	lcd_readPoint(0, 0);

	lcd_show_string(10, 10, 24, 24, 24, (char *)"HUANGhongfu");

	while(1)			
	{	
		state = !state;
		led_switch(LED0, state);
		delay_ms(1000);
	}

	return 0;
}