#include "bsp_epit.h"
#include "bsp_int.h"
#include "bsp_led.h"

/* 初始化epit */
void epit_init(unsigned int frac, unsigned int Lvalue, unsigned int CMPRvalue)
{
    if( frac > 4095)
    {
        frac = 4095;
    }

    /* 配置EPIT_CR寄存器 */
    EPIT1->CR = 0;
    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (frac << 4) | (1 << 24);

    /* 加载寄存器，相当于倒计数值 */
    EPIT1->LR = Lvalue ;

    /* 比较寄存器，加载寄存器倒计数到设置指定的比较值触发中断 */
    EPIT1->CMPR = CMPRvalue;

    /* 初始化中断 */
    GIC_EnableIRQ(EPIT1_IRQn);
    system_register_irqHandler(EPIT1_IRQn, (system_irq_handler_t)epit1_irqHandler, NULL);

    /* 打开EPIT1中断 */
    EPIT1->CR |= 1 << 0;

}

void epit1_irqHandler(unsigned int gicciar,void *param)
{
    static unsigned char state = 0;

    state = !state;
    if(EPIT1->SR & (1 << 0))
    {
        led_switch(LED0, state);
    }

    /* 清除中断标志位 */
    EPIT1->SR |= (1 << 0);
}