#include "main.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "exti.h"
#include "bsp_epit.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	int_init();			/* 中断初始化 */
	imx6u_clk_init();	/* 初始化时钟 */
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();			/* 初始化led 			*/
	beep_init();		/* 初始化beep	 		*/
	key_init();			/* 初始化按键 */
	exti_init();		/* 初始化中断 */
	epit_init(0, 66000000/2, 0);		/* 初始化EPIT定时器 */

	while(1)			
	{	
		delay(500);
	}

	return 0;
}
