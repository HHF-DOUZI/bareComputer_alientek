#ifndef __BSP_BACKLIGHT_H
#define __BSP_BACKLIGHT_H
#include "imx6ul.h"

/* 背光信息 */
struct backlight_Dev_struc 
{
    unsigned char pwm_duty;             /* 0~100，占空比设置 */
};

extern struct backlight_Dev_struc backlight_Dev;

/* 初始化PWM，backlight */
void backlight_init(void);
void pwm1_setperiod_value(unsigned int value);
/* 设置占空比 duty = 100 */
void pwm1_setduty(unsigned char duty);
void pwm1_irqhandler(void);

#endif