#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H
#include "imx6ul.h"


/* 函数声明 */
void delay_init();
void gpt1_irqHandler(unsigned int gicciar, void *parem);
void delay_us(unsigned int usDalay);
void delay_ms(unsigned int msDalay);
void delay_short(volatile unsigned int n);
void delay(volatile unsigned int n);

#endif

