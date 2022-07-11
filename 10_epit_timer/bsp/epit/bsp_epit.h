#ifndef __BSPEPIT_H
#define __BSPEPIT_H
#include "imx6ul.h"

void epit_init(unsigned int frac, unsigned int Lvalue, unsigned int CMPRvalue);
void epit1_irqHandler(unsigned int gicciar,void *param);
#endif