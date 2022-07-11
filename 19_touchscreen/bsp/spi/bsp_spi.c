#include "bsp_spi.h"

/* spi初始化 */
void spi_init(ECSPI_Type *base)
{
    base->CONREG = 0;               /* 清0 */
    base->CONREG |= (1 << 0) | (1 << 3) | (1 << 4) | (7 << 20);

    base->CONFIGREG = 0;
    base->PERIODREG = 0x2000;

    /* SPI时钟，ICM20608的SPI最高是8MHz，将SPI CLK = 6MHz */
    base->CONREG &= ~(0xF << 12) | (0xF << 8);              /* 先将bit15~12和bit11~8清0 */
    base->CONREG |= (9 << 12);                              /* 前级10分频，后级不设置  60 / 10 = 6MHz */
}

/* SPI发送/接收函数 */
unsigned char spich0_readwrite_byte(ECSPI_Type *base, unsigned char txdata)
{
    uint32_t spirxdata = 0;
    uint32_t spitxdata = txdata;

    /* 这里选择通道0 */
    base->CONREG &= ~(3 << 18);
    base->CONREG |= (0 << 18);

    /* 数据发送 */
    while((base->STATREG & (1 << 0)) == 0);         /* 等待为空 */
    base->TXDATA = spitxdata;

    /* 数据接收 */
    while((base->STATREG & (1 << 3)) == 0);
    spirxdata = base->RXDATA;

    return spirxdata;
}