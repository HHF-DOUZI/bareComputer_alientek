#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_key.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	int i = 0;
	unsigned char led_state = OFF;
	unsigned char beep_state = OFF;
	int keyValue = 0;

	clk_enable();		/* 使能所有的时钟 			*/
	led_init();			/* 初始化led 			*/
	beep_init();		/* 初始化beep	 		*/
	key_init();			/* 初始化按键 */

	while(1)			
	{	
		/* 按键控制蜂鸣器 */
		keyValue = key_getValue();
		if(keyValue)	/* 如果为证，表示有效得按键值 */
		{
			switch (keyValue)
			{
			case KEY0_VALUE:
				beep_state = !beep_state;
				beep_switch(beep_state);
				break;
			}
		}
		
		i++;
		if(i == 50)			/* 50*10=500ms */
		{
			i = 0;
			led_state = !led_state;
			led_switch(LED0, led_state);
		}
		delay(10);
	}

	return 0;
}
