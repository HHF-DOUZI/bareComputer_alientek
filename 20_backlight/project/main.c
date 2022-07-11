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
#include "imc20608.h"
// #include "ft5426.h"
#include "gt911.h"
#include "bsp_backlight.h"
#include "stdio.h"

/*
 * @description	: 使能I.MX6U的硬件NEON和FPU
 * @param 		: 无
 * @return 		: 无
 */
 void imx6ul_hardfpu_enable(void)
{
	uint32_t cpacr;
	uint32_t fpexc;

	/* 使能NEON和FPU */
	cpacr = __get_CPACR();
	cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk))
		   |  (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);
	__set_CPACR(cpacr);

	fpexc = __get_FPEXC();
	fpexc |= 0x40000000UL;	
	__set_FPEXC(fpexc);
}

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned char duty;
	unsigned char key = 0;
	unsigned char i = 0;
	unsigned char state = OFF;

	imx6ul_hardfpu_enable();	/* 开启硬件浮点运算及NEON */
	
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
	// ft5426_init();		/* FT5426初始化 */
	// gt9147_init();
	backlight_init();	/* 初始化背光*/		

    tftlcd_dev.forecolor = LCD_RED;
    tftlcd_dev.backcolor = LCD_WHITE;
	tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"hhf touch TEST");    /* 显示字符串 */
	lcd_show_string(50, 40, 200, 16, 16, (char*)"ATOM@ALIENTEK");  
	lcd_show_string(50, 60, 200, 16, 16, (char*)"2022/7/6");  
	tftlcd_dev.forecolor = LCD_RED;

	duty = 10;
	pwm1_setduty(duty);
	while(1)					
	{
		key = key_getValue();
		if(key == KEY0_VALUE) {
			duty += 10;
			if(duty > 100)
				duty = 0;
			printf("PWM1 Duty = %d\r\n", duty);
			pwm1_setduty(duty);
		}

		delay_ms(10);
		i++;
	
		if(i == 50)
		{	
			i = 0;
			state = !state;
			led_switch(LED0,state); 
		}
	}
	return 0;
}