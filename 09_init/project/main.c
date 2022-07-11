#include "main.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "exti.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	unsigned char led_state = OFF;

	int_init();			/* 中断初始化 */
	imx6u_clk_init();	/* 初始化时钟 */
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();			/* 初始化led 			*/
	beep_init();		/* 初始化beep	 		*/
	key_init();			/* 初始化按键 */
	exti_init();		/* 初始化中断 */

	while(1)			
	{	
		led_state = !led_state;
		led_switch(LED0, led_state);
		delay(500);
	}

	return 0;
}
