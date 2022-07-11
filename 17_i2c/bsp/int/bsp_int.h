#ifndef __BSPINT_H
#define __BSPINT_H
#include "imx6ul.h"

/* 定义中断处理函数--函数--函数--指向函数地址 */
typedef void (*system_irq_handler_t)(unsigned int gicciar, void *parem);

/* 中断处理函数结构体 */
typedef struct _sys_irq_handle
{
    system_irq_handler_t    irqHandler;             /* 中断处理函数 */
    void *userParam;
}sys_irq_handle_t;

void int_init(void);
void system_register_irqHandler(IRQn_Type irq, system_irq_handler_t handler, void *userParam);
void default_irqHandler(unsigned int gicciar, void *userParem);

#endif