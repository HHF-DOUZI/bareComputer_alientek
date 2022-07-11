#ifndef __KEYFILTER_H
#define __KEYFILTER_H
#include "imx6ul.h"

void keyfilter_init(void);
void filtertimer_init(unsigned int value);
void filtertimer_stop(void);
void filtertimer_start(unsigned int value);
void keyfiltertimer_irqHandler(unsigned int gicciar,void *param);
void gpio1_16_31_irqHandler(unsigned int gicciar, void *parem);

#endif