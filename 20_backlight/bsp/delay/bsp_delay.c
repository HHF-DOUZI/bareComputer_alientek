#include "bsp_delay.h"
#include "bsp_int.h"
#include "bsp_led.h"

/* 延时初始化函数 */
void delay_init()
{
	GPT1->CR = 0;
	GPT1->CR = 1 << 15;
	while ((GPT1->CR >> 15) & 0x01);

	/*
		设置GPT1时钟源ipk_clk = 66MHz,
		restart 模式
		默认为计数寄存器从0开始
	*/
	GPT1->CR |= (1 << 1) | (1 << 6);

	/* 分频设置 */
	GPT1->PR = 65;					/* 66分频，频率= 66000 0000/66 = 1M */
	/*
	 * 	1M的频率，计1个数就是1us，那么0xffffffff=4294967295us = 71.5min
	*/
	GPT1->OCR[0] = 0xffffffff;

#if 0
	/* 配置输出比较通道1 */
	GPT1->OCR[0] = 1000000/2;		/* 设置中断周期为500ms */

	/* 打开GPT1输出比较寄存器通道1的中断 */
	GPT1->IR = 1 << 0;

	/* 设置GIC */
	GIC_EnableIRQ(GPT1_IRQn);
	system_register_irqHandler(GPT1_IRQn, gpt1_irqHandler, NULL);
#endif
	/* 打开GPT1使能，bit0 置1 */
	GPT1->CR |= 1 << 0;
}

#if 0
/* GPT1中断服务函数 */
void gpt1_irqHandler(unsigned int gicciar, void *parem)
{
	static unsigned char state = 0;

	if(GPT1->SR & (1 << 0))
	{
		state = !state;
		led_switch(LED0, state);
	}

	GPT1->SR |= 1 << 0;						/* 清除中断标志位 */

}
#endif

/* us延时 */
void delay_us(unsigned int usDalay)
{
	unsigned long oldCnt, newCnt;
	unsigned long tcntValue = 0;

	oldCnt = GPT1->CNT;
	while(1)
	{
		newCnt = GPT1->CNT;
		if( newCnt != oldCnt )
		{
			if(newCnt > oldCnt)
				tcntValue += newCnt - oldCnt;
			else
			{
				tcntValue += 0xffffffff - oldCnt + newCnt;
			}
			oldCnt = newCnt;
			if(tcntValue >= usDalay)
				break;
		}
	}
}

/* 毫秒延时 */
void delay_ms(unsigned int msDalay)
{
	int i = 0;
	for(i = 0; i < msDalay; i++)
		delay_us(1000);
}

/*
 * @description	: 短时间延时函数
 * @param - n	: 要延时循环次数(空操作循环次数，模式延时)
 * @return 		: 无
 */
void delay_short(volatile unsigned int n)
{
	while(n--){}
}

/*
 * @description	: 延时函数,在396Mhz的主频下
 * 			  	  延时时间大约为1ms
 * @param - n	: 要延时的ms数
 * @return 		: 无
 */
void delay(volatile unsigned int n)
{
	while(n--)
	{
		delay_short(0x7ff);
	}
}


