#include "bsp_gpio.h"

/* gpio 初始化 */
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
    if(config->direction == KGPIO_DigtalInput) /* 如果是输入 */
    {
        base->GDIR &= ~(1 << pin);
    }
    else
    {
        base->GDIR |= (1 << pin);
        /* 设置默认输出电平 */
        gpio_pinwrite(base, pin, config->outputLogic);
    }
}

/* 控制GPIO高低电平 */
void gpio_pinwrite(GPIO_Type *base, int pin, int value)
{
    if(value == 0)  /* 写 0 */
    {
        base->DR &= ~(1 << pin);
    }
    else            /* 写1  */
    {
        base->DR |= (1 << pin);
    }
}

/* 读取指令IO电平 */
int gpio_pinread(GPIO_Type *base, int pin)
{
    return (((base->DR) >> pin) & 0x1);
}