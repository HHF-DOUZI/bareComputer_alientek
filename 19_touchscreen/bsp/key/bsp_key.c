#include "bsp_key.h"
#include "bsp_delay.h"
#include "bsp_gpio.h"

/* 初始化按键 */
void key_init(void)
{
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);             /* 复用GPIO1-IO18 */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xf080);     /* 设置电气属性 SW_PAD */

    key_config.direction = KGPIO_DigtalInput;
    /* GPIO初始化 */
    gpio_init(GPIO1, 18, &key_config);
}

int read_key(void)
{
    int ret = 0;
    ret = gpio_pinread(GPIO1, 18);
    return ret;
}

int key_getValue(void)
{
    int ret = 0;
    static unsigned char release = 1;       /* 为1表示按键释放 */      

    if((release == 1) && (gpio_pinread(GPIO1, 18) == 0))
    {
        delay(10);          /* 延时消抖 */
        release = 0;        /* 标记按键按下 */
        if(gpio_pinread(GPIO1, 18) == 0)
        {
            ret = KEY0_VALUE;
        }
    }
    else if(gpio_pinread(GPIO1, 18) == 1)       /* 未按下 */
    {
        ret = KEY_NONE;
        release = 1;        /* 标记按键释放 */
    }
    return ret; 
}