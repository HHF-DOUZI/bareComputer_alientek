#include "keyfilter.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_delay.h"
#include "bsp_beep.h"

/* 按键初始化 */
void keyfilter_init(void)
{
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);             /* 复用GPIO1-IO18 */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);     /* 设置电气属性 SW_PAD */

    /* GPIO初始化 */
    key_config.direction = KGPIO_DigtalInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    gpio_init(GPIO1, 18, &key_config);

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_register_irqHandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio1_16_31_irqHandler, NULL);
    gpio_enable(GPIO1, 18);

    /* 初始化定时器 */
    filtertimer_init(66000000/100);         /* 一秒66M ，这里是10毫秒 */
}

/* 初始化定时器 */
void filtertimer_init(unsigned int value)
{
    /* 配置EPIT_CR寄存器 */
    EPIT1->CR = 0;
    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 24);

    /* 加载寄存器，相当于倒计数值 */
    EPIT1->LR = value ;

    /* 比较寄存器，加载寄存器倒计数到设置指定的比较值触发中断 */
    EPIT1->CMPR = 0;

    /* 初始化中断 */
    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqHandler(EPIT1_IRQn, (system_irq_handler_t)keyfiltertimer_irqHandler, NULL);

}

/* 关闭EPIT1定时器 */
void filtertimer_stop(void)
{
    EPIT1->CR &= ~(1 << 0);
}

/* 打开EPIT1定时器 */
void filtertimer_start(unsigned int value)
{
    EPIT1->CR &= ~(1 << 0);
    EPIT1->LR = value;
    EPIT1->CR |= 1 << 0;
}

/* EPIT1定时器中断处理函数 */
void keyfiltertimer_irqHandler(unsigned int gicciar,void *param)
{
    static unsigned char state = OFF;

    if(EPIT1->SR & (1 << 0))
    {
        /* 关闭定时器 */
        filtertimer_stop();
        if (gpio_pinread(GPIO1, 18) == 0)
        {
            state = !state;
            beep_switch(state);
        }
        
    }

    /* 清除中断标志位 */
    EPIT1->SR |= (1 << 0);
}

/* 中断处理函数 */
void gpio1_16_31_irqHandler(unsigned int gicciar, void *parem)
{
    /* 开启定时器 */
    filtertimer_start(66000000/100);
    /* 清除中断标志位 */
    gpio_clearIntFlags(GPIO1, 18);
}