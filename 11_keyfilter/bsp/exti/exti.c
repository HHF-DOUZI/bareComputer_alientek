#include "exti.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_delay.h"
#include "bsp_beep.h"

/* 初始化外部中断，也就是GPIO1-IO18 */
void exti_init(void)
{
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);             /* 复用GPIO1-IO18 */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);     /* 设置电气属性 SW_PAD */

    /* GPIO初始化 */
    key_config.direction = KGPIO_DigtalInput;
    key_config.interruptMode = kGPIO_IntFallingEdge;
    gpio_init(GPIO1, 18, &key_config);

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_register_irqHandler(GPIO1_Combined_16_31_IRQn, (system_irq_handler_t)gpio1_io18_irqHandler, NULL);
    gpio_enable(GPIO1, 18);
}

/* 中断处理函数 */
void gpio1_io18_irqHandler(unsigned int gicciar, void *parem)
{
    static unsigned char state = 0;

    delay(10);                      /* 在实际的开发中，禁止在中断服务函数中调用延时函数！！ */
    if(gpio_pinread(GPIO1, 18) == 0)
    {
        state = !state;
        beep_switch(state);
    }

    /* 清除中断标志位 */
    gpio_clearIntFlags(GPIO1, 18);
}