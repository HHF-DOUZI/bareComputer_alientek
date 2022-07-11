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
#include "stdio.h"

/*
 * @description	: main函数
 * @param 		: 无
 * @return 		: 无
 */
int main(void)
{
	int c = 100, a;
	int_init();			/* 中断初始化 */
	imx6u_clk_init();	/* 初始化时钟 */
	uart_init();		/* 初始化串口 */
	clk_enable();		/* 使能所有的时钟 			*/
	led_init();			/* 初始化led 			*/
	beep_init();		/* 初始化beep	 		*/
	key_init();			/* 初始化按键 */
	delay_init();		/* 高精度延时GPT1定时器 */

	printf("%d的十六进制nonono：%#X\r\n", c, c);
	while(1)			
	{	
		// printf("hello，请输入两个整数，使用空格隔开:");
		// scanf("%d %d", &a, &b);
		// printf("\r\n %d+%d=%d \r\n", a, b, a+b);
		puts("请输入 1 个字符:");
		a=getc();
		putc(a); /* 回显功能 */
		puts("\r\n");

		/* 显示输入的字符 */
		puts("您输入的字符为:");
		putc(a);
		puts("\r\n\r\n");

	}

	return 0;
}
