#include "bsp_led.h"

/*
 * @description	: 初始化LED对应的GPIO
 * @param 		: 无
 * @return 		: 无
 */
void led_init(void)
{
	/* 1、初始化IO复用 */
	IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03,0);		/* 复用为GPIO1_IO03 */
	
	
	/* 2、、配置GPIO1_IO03的IO属性	
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 00 默认下拉
	 *bit [13]: 0 kepper功能
	 *bit [12]: 1 pull/keeper使能
	 *bit [11]: 0 关闭开路输出
	 *bit [7:6]: 10 速度100Mhz
	 *bit [5:3]: 110 R0/6驱动能力
	 *bit [0]: 0 低转换率
	 */
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03,0X10B0);
	
    /* GPIO初始化 */
    GPIO1->GDIR = 0x8;           /* 设置为输出 */
    GPIO1->DR |= (1<<3);         /* 关闭LED灯 */	
}

/* 打开LED灯 */
void led_on(void)
{
    GPIO1->DR &= ~(1<<3); /* bit3清零 */
}

/* 关闭LED灯 */
void led_off(void)
{
    GPIO1->DR |= (1<<3); /* bit3置1 */
}

/*
 * @description		: LED控制函数，控制LED打开还是关闭
 * @param - led		: 要控制的LED灯编号
 * @param - status	: 0，关闭LED0，1 打开LED0
 * @return 			: 无
 */
void led_switch(int led, int status)
{	
	switch(led)
	{
		case LED0:
			if(status == ON)
				GPIO1->DR &= ~(1 << 3);	/* 打开LED0 */
			else if(status == OFF)
				GPIO1->DR |= (1 << 3);	/* 关闭LED0 */
			break;
	}
}
