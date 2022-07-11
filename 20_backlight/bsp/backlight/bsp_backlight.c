#include "bsp_backlight.h"
#include "bsp_int.h"

struct backlight_Dev_struc backlight_Dev;

/* 初始化PWM，backlight */
void backlight_init(void)
{
    unsigned char i = 0;

    /* IO初始化 */
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO08_PWM1_OUT, 0);
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO08_PWM1_OUT, 0xB090);

    /* PWM初始化 */
    PWM1->PWMCR = 0;            /* 清0 */

    /* pwm时钟源 = 66 MHz，设置66分频，因此PWM时钟频率为1MHz（所以 bit 4 为 65 ) */
    PWM1->PWMCR |= (1 << 16) | (65 << 4) | (1 << 26);

    /* PWM频率为1KHz */
    pwm1_setperiod_value(1000);

    /* 设置占空比 */
    backlight_Dev.pwm_duty = 50;
    for (i = 0; i < 4; i++)
    {
        pwm1_setduty(backlight_Dev.pwm_duty);
    }

    /* 使能FIFO中断 */
    PWM1->PWMIR = 1 << 0;            /* 使能PWM1的FIFO中断 */
    GIC_EnableIRQ(PWM1_IRQn);			/* 使能GIC中对应的中断 */
	system_register_irqHandler(PWM1_IRQn, (system_irq_handler_t)pwm1_irqhandler, NULL);	/* 注册中断服务函数 */

    PWM1->PWMSAR = 0xFF;

    /* 打开PWM */
    PWM1->PWMCR |= 1 << 0;
}

/* 中断函数 */
void pwm1_irqhandler(void)
{ 
	if(PWM1->PWMSR & (1 << 3))              /* FIFO中断位 */
	{
        pwm1_setduty(backlight_Dev.pwm_duty);
        PWM1->PWMSR |= 1 << 3;              /* 清除中断标志位 */  
	}
}

/* 设置PR寄存器 */
void pwm1_setperiod_value(unsigned int value)
{
    unsigned int regvalue = 0;

    if(value < 2)
        regvalue = 2;
    else 
        regvalue = value - 2;
    PWM1->PWMPR = (regvalue & 0xFFFF);
}

/* 设置占空比 duty = 100 */
void pwm1_setduty(unsigned char duty)
{
    unsigned short period;
    unsigned short sample;

    backlight_Dev.pwm_duty = duty;
    period = PWM1->PWMPR + 2;
    sample = (unsigned short)(period * backlight_Dev.pwm_duty / 100.0F);

    PWM1->PWMSAR = (sample & 0xFFFF);
}