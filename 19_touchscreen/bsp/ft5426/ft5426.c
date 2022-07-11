#include "ft5426.h"
#include "bsp_gpio.h"
#include "bsp_int.h"
#include "bsp_delay.h"
#include "bsp_i2c.h"
#include "stdio.h"

struct ft5426_dev_sturct ft5426_dev;

/* 初始化FT5426 */
void ft5426_init(void)
{
    unsigned char reg_value[2];
    unsigned char ret = 0;

    /* IO初始化 */
    IOMUXC_SetPinMux(IOMUXC_UART5_TX_DATA_I2C2_SCL, 1);             /* 复用I2C2_SCL */
    IOMUXC_SetPinMux(IOMUXC_UART5_RX_DATA_I2C2_SDA, 1);

    IOMUXC_SetPinConfig(IOMUXC_UART5_TX_DATA_I2C2_SCL, 0x70b0);     /* 设置电气属性 SW_PAD */
    IOMUXC_SetPinConfig(IOMUXC_UART5_RX_DATA_I2C2_SDA, 0x70b0); 

    /* 初始化INT引脚，使能中断 */
    gpio_pin_config_t cinit_config;
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO09_GPIO1_IO09, 0);	            /* 复用GPIO1_IO09 */	
	
	
	/* 2、、配置GPIO1_IO03的IO属性	
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 00 默认下拉
	 *bit [13]: 0 kepper功能
	 *bit [12]: 1 pull/keeper使能
	 *bit [11]: 0 关闭开路输出
	 *bit [7:6]: 10 速度100Mhz
	 *bit [5:3]: 110 R0/6驱动能力
	 *bit [0]: 0 低转换率
	 */
	IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO09_GPIO1_IO09,0XF080);
    cinit_config.direction = KGPIO_DigtalInput;
	cinit_config.interruptMode = kGPIO_IntRisingOrFallingEdge;
    /* GPIO初始化 */
    gpio_init(GPIO1, 9, &cinit_config);	

    GIC_EnableIRQ(GPIO1_Combined_0_15_IRQn);
    system_register_irqHandler(GPIO1_Combined_0_15_IRQn, (system_irq_handler_t)gpio1_io09_irqHandler, NULL);
    gpio_enable(GPIO1, 9);

    /* 初始化复位IO */
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09,0);               /* 复用GPIO5_IO09 */
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER9_GPIO5_IO09,0X10B0);       /* 设置电气属性 */
    gpio_pin_config_t ctrst_config;
    ctrst_config.direction = KGPIO_DigtalOutput;
    ctrst_config.outputLogic = 1;
    gpio_init(GPIO5, 9, &ctrst_config);

    gpio_pinwrite(GPIO5, 9, 0);             /* 复位FT5426 */
    delay_ms(50);
    gpio_pinwrite(GPIO5, 9, 1);             /* 停止复位 */
    delay_ms(50);
    
    /* I2C2接口初始化 */
    i2c_init(I2C2);

    /* FT5426初始化 */
    ft5426_read_len(FT5426_ADDR, FT5426_IDGLIB_VERSION, 2, reg_value);
    printf("touch frimware version: %#x\r\n", ((unsigned short)reg_value[0] << 8) | reg_value[1]);

    ft5426_write_byte(FT5426_ADDR, FT5426_DEVICE_MODE, 0);          /* 设置FT5426工作在正常模式 */ 
    ft5426_write_byte(FT5426_ADDR, FT5426_IDG_MODE, 1);             /* 中断模式 */
    ret = ft5426_read_byte(FT5426_ADDR, FT5426_IDG_MODE);
    printf("0x80 = %#x\r\n", ret);

    ft5426_dev.initflag = FT5426_INIT_FINISHED;

}

/* 中断处理函数 */
void gpio1_io09_irqHandler(unsigned int gicciar, void *parem)
{
    /* 判断中断标志位 */
    if(ft5426_dev.initflag == FT5426_INIT_FINISHED)
    {
        ft5426_read_tpcoord();
    }

    /* 清除中断标志位 */
    gpio_clearIntFlags(GPIO1, 9);
}

/* ft5426写一个字节数据 */
unsigned char ft5426_write_byte(unsigned char addr, unsigned char reg, unsigned char data)
{
    unsigned char writedata = data;
    unsigned char status    = 0;
    struct i2c_transfer transfer;

    transfer.slaveAddress   = addr;
    transfer.direction      = kI2C_Write;
    transfer.subaddress     = reg;
    transfer.subaddressSize = 1;
    transfer.data           = &writedata;
    transfer.dataSize       = 1;

    if(i2c_master_transfer(I2C2, &transfer))
        status = 1;
    return status;
}

/* ft5426读一个字节数据 */
unsigned char ft5426_read_byte(unsigned char addr, unsigned char reg)
{
    unsigned char val = 0;
    struct i2c_transfer transfer;

    transfer.slaveAddress   = addr;
    transfer.direction      = kI2C_Read;
    transfer.subaddress     = reg;
    transfer.subaddressSize = 1;
    transfer.data           = &val;
    transfer.dataSize       = 1;
    i2c_master_transfer(I2C2, &transfer);

    return val;
}

/* ft5426读一串字节数据 */
void ft5426_read_len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char *buf)
{
    struct i2c_transfer transfer;

    transfer.slaveAddress   = addr;
    transfer.direction      = kI2C_Read;
    transfer.subaddress     = reg;
    transfer.subaddressSize = 1;
    transfer.data           = buf;
    transfer.dataSize       = 1;
    i2c_master_transfer(I2C2, &transfer);
}

/* 读取触摸坐标信息 */
void ft5426_read_tpcoord(void)
{
    unsigned char i = 0;
    unsigned char type = 0;
    unsigned char poinbuf[FT5426_XYCOORDREG_NUM];

    ft5426_dev.point_num = ft5426_read_byte(FT5426_ADDR, FT5426_TD_STATUS);
    
    for( i = 0; i < ft5426_dev.point_num; i++)
    {
        unsigned char *buf = &poinbuf[i * 6];

        ft5426_dev.x[i] = ((buf[0] << 8) | buf[1]) & 0xfff;
        ft5426_dev.y[i] = ((buf[2] << 8) | buf[3]) & 0xfff;

        type = buf[0] >> 6;
        if(type == FT5426_TOUCH_EVENT_DOWN)
        {

        }
        else if(type == FT5426_TOUCH_EVENT_UP){

        }
        else if(type == FT5426_TOUCH_EVENT_ON){

        }
        else if(type == FT5426_TOUCH_EVENT_RESERVED){

        }
    }
}