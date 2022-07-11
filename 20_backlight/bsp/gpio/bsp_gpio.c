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
    gpio_intconfig(base, pin, config->interruptMode);
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

/* 使能指定IO中断 */
void gpio_enable(GPIO_Type *base, unsigned int pin)
{
    base->IMR |= (1 << pin);
}

/* 禁止指定IO中断 */
void gpio_disenable(GPIO_Type *base, unsigned int pin)
{
    base->IMR &= ~(1 << pin);
}

/* 清除中断标志位 */
void gpio_clearIntFlags(GPIO_Type *base, unsigned int pin)
{
    base->ISR |= (1 << pin);
}

/* GPIO中断初始化函数 */
void gpio_intconfig(GPIO_Type *base, unsigned int pin, gpio_interrupt_mode_t ping_int_Mode)
{
    volatile uint32_t *icr;
    uint32_t icrShift;
    icrShift = pin;
    base->EDGE_SEL &= ~(1 << pin);

    if(pin < 16)            /* 低16位 */
    {
        icr = &(base->ICR1);
    }
    else
    {
        icr = &(base->ICR2);
        icrShift -= 16;
    }

    switch (ping_int_Mode)
    {
    case kGPIO_IntLowLevel:
        *icr &= ~(3 << (2 * icrShift));
        break;
    case kGPIO_IntHighLevel:
        *icr &= ~(3 << (2 * icrShift));
        *icr |= (1 << (2 * icrShift));
        break;
    case kGPIO_IntRisingEdge:
        *icr &= ~(3 << (2 * icrShift));
        *icr |= (2 << (2 * icrShift));
        break;
    case kGPIO_IntFallingEdge:
        *icr &= ~(3 << (2 * icrShift));
        *icr |= (3 << (2 * icrShift));
        break;
    case kGPIO_IntRisingOrFallingEdge:
        base->EDGE_SEL |= (1 << pin);
        break;
    
    default:
        break;
    }
}