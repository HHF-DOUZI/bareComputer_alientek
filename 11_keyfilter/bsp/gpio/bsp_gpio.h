#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "MCIMX6Y2.h"
#include "cc.h"

/* 描述中断触发类型 */
typedef enum _gpio_interrupt_mode
{
    kGPIO_Nointmode = 0U,                   /* 无中断功能 */
    kGPIO_IntLowLevel = 1U,                 /* 低电平触发 */
    kGPIO_IntHighLevel = 2U,                /* 高电平触发 */
    kGPIO_IntRisingEdge = 3U,               /* 上升沿触发 */
    kGPIO_IntFallingEdge = 4U,              /* 下降沿触发 */
    kGPIO_IntRisingOrFallingEdge = 5U,      /* 上升沿或者下降沿触发 */
}gpio_interrupt_mode_t;

/* 枚举类型和GPIO结构体 */
typedef enum _gpio_pin_direction
{
    KGPIO_DigtalInput = 0U,
    KGPIO_DigtalOutput = 1U, 
}gpio_pin_direction_t;

typedef struct _gpio_pin_config
{
    gpio_pin_direction_t direction;
    uint8_t outputLogic;
    gpio_interrupt_mode_t interruptMode;       /* 中断方式 */
}gpio_pin_config_t;

void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
void gpio_pinwrite(GPIO_Type *base, int pin, int value);
int gpio_pinread(GPIO_Type *base, int pin);
void gpio_enable(GPIO_Type *base, unsigned int pin);
void gpio_disenable(GPIO_Type *base, unsigned int pin);
void gpio_clearIntFlags(GPIO_Type *base, unsigned int pin);
void gpio_intconfig(GPIO_Type *base, unsigned int pin, gpio_interrupt_mode_t ping_int_Mode);

#endif
