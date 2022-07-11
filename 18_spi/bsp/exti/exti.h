#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H
#include "imx6ul.h"

void exti_init(void);
void gpio1_io18_irqHandler(unsigned int gicciar, void *parem);

#endif