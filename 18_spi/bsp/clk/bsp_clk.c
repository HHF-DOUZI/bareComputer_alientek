#include "bsp_clk.h"
/*
 * @description	: 使能I.MX6U所有外设时钟
 * @param 		: 无
 * @return 		: 无
 */
void clk_enable(void)
{
	CCM->CCGR0 = 0XFFFFFFFF;
	CCM->CCGR1 = 0XFFFFFFFF;
	CCM->CCGR2 = 0XFFFFFFFF;
	CCM->CCGR3 = 0XFFFFFFFF;
	CCM->CCGR4 = 0XFFFFFFFF;
	CCM->CCGR5 = 0XFFFFFFFF;
	CCM->CCGR6 = 0XFFFFFFFF;
}

/* 初始化时钟 */
void imx6u_clk_init()
{
	unsigned int reg = 0;
	/*  初始化6U的主频为528MHz  */
	if( ( (CCM->CCSR >> 2 ) & 0X1 ) == 0 )		/* 当迁侍中使用pll1_mian_clk也就是PLL_sw_clk =0 */
	{
		CCM->CCSR &= ~(1 << 8); 		/* 设置step_clk = osc_clk = 24Mhz */
		CCM->CCSR |= (1 << 2);			/* PLL1_SW_CLK = STEP_CLK = 24MHz */
	}

	/* 设置PLL1 = 1056 MHz*/
	CCM_ANALOG->PLL_ARM = (1 << 13) | ((88 << 0) & 0X7F);
	CCM->CACRR = 1;					/* （bit2-0） 001 设置2分频 */
	CCM->CCSR &= ~(1 << 2);			/* 设置pll1_sw_clk = pll1_main_clk = 1056MHz */

	/* 设置PLL2的4路PFD */
	reg = CCM_ANALOG->PFD_528;
	reg &= ~(0x3F3F3F3F);		/* 先清0 */
	reg |= (32 << 24);				/* PLL2_PDF3 = 297MHz */
	reg |= (24 << 16);				/* PLL2_PDF2 = 396MHz */
	reg |= (16 << 8);				/* PLL2_PDF1 = 594MHz */
	reg |= (8 << 0);				/* PLL2_PDF3 = 352MHz */
	CCM_ANALOG->PFD_528 = reg;

	/* 设置PLL3的4路PFD */
	reg = 0;
	reg = CCM_ANALOG->PFD_480;
	reg &= ~(0x3F3F3F3F);		/* 先清0 */
	reg |= (19 << 24);				/* PLL3_PDF3 = 454.7MHz */
	reg |= (17 << 16);				/* PLL3_PDF2 = 508.2MHz */
	reg |= (16 << 8);				/* PLL3_PDF1 = 540MHz */
	reg |= (12 << 0);				/* PLL3_PDF3 = 720MHz */
	CCM_ANALOG->PFD_480 = reg;

	/* 设置AHB_CLK_ROOT = 132MHz */
	CCM->CBCMR &= ~(3 << 18);			/* bit18-19 清0 */
	CCM->CBCMR |= (1 << 18);			/* 设置pre_preiph clock = PLL2_PFD2 = 396 MHz */
	CCM->CBCDR &= ~(1 << 25);
	while (CCM->CDHIPR & (1 << 5));		/* 等待握手信号 */

#if 0
	CCM->CBCDR &= ~(7 << 10);			/* bit 12 - 10 清0 */
	CCM->CBCDR |= (2 << 10);			/* 3分频 */
	while(CCM->CDHIPR & (1 << 1));		/* 等待握手信号 */
#endif

	/* IPG_CLK_ROOT = 66MHz */
	CCM->CBCDR &= ~(3 << 8);
	CCM->CBCDR |= (1 << 8);				/* IPG_CLK_ROOT = AHB_CLK_ROOT/2 = 132/2 = 66 MHz */

	/* PER_CLK_ROOT = 66MHz */
	CCM->CSCMR1 &= ~(1 << 6);			/* PERCLK_CLK_ROOT时钟源为IPG_CLK= 66MHz */
	CCM->CSCMR1 &= ~(0X3F << 0);		/* 1分频，PERCLK_CLK_ROOT = 66 MHz */

	/* 设置UART根时钟为80MHz */
	CCM->CSCDR1 &= ~(1 << 6);			/* UART_CLK_ROOT= 80 MHz */
	CCM->CSCDR1 &= ~0x3F;				/* 1分频 */

	/* 设置ECSPI根时钟为PLL3_60MHz */
	CCM->CSCDR2 &= ~(1 << 18);			/* ECSPI时钟源为PLL3_60MHz */
	CCM->CSCDR2 &= ~(0x3F << 19);		/* 1分频 */

}