/*
********************************************************************************************************
*                                          All Rights Reserved
* Version   : V1.00
* Data      : 2012-08-18/21:49
* By        : yang
* For       : ARMv7M Cortex-M4
* Toolchain : RealView Development Suite
*             RealView Microcontroller Development Kit (MDK)
*             ARM Developer Suite (ADS)
*             Keil uVision
*********************************************************************************************************
*/
#include "ADS7843.h"
#include "app.h"
#include "ucos_ii.h"
#include "common.h"
#include "usart.h"
#include <stdio.h>
#include <stdio.h>
#include "parameters.h"

#define SPI_MODE  0
//#define GPIO_MODE 0

u16 TPReadX(void);
u16 TPReadY(void);

//lcd x轴点阵数
#define x_point_num 800

//lcd y轴点阵数
#define y_point_num 480


TOUCH_T g_tTP;

#if USE_GD == 1
#ifdef SPI_MODE
/* ADS7853 SPI Interface pins  */
#define ADS7843_SPI                           SPI4
#define ADS7843_SPI_CLK                       RCC_APB2Periph_SPI4
#define ADS7843_SPI_CLK_INIT                  RCC_APB2PeriphClockCmd
#endif
/*SCK-pin*/
#define ADS7843_SPI_SCK_PIN                   GPIO_Pin_2
#define ADS7843_SPI_SCK_GPIO_PORT             GPIOE
#define ADS7843_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOE
#ifdef SPI_MODE
#define ADS7843_SPI_SCK_SOURCE                GPIO_PinSource2
#define ADS7843_SPI_SCK_AF                    GPIO_AF_SPI4
#endif


/*MISO-pin*/
#define ADS7843_SPI_MISO_PIN                  GPIO_Pin_5
#define ADS7843_SPI_MISO_GPIO_PORT            GPIOE
#define ADS7843_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOE
#ifdef SPI_MODE
#define ADS7843_SPI_MISO_SOURCE               GPIO_PinSource5
#define ADS7843_SPI_MISO_AF                   GPIO_AF_SPI4
#endif


/*MOSI-pin*/
#define ADS7843_SPI_MOSI_PIN                  GPIO_Pin_6
#define ADS7843_SPI_MOSI_GPIO_PORT            GPIOE
#define ADS7843_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOE
#ifdef SPI_MODE
#define ADS7843_SPI_MOSI_SOURCE               GPIO_PinSource6
#define ADS7843_SPI_MOSI_AF                   GPIO_AF_SPI4
#endif

#define ADS7843_SPI_CS_PIN                        GPIO_Pin_2
#define ADS7843_SPI_CS_GPIO_PORT                  GPIOD
#define ADS7843_SPI_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOD

#define ADS7843_IRQ_PIN                        GPIO_Pin_12
#define ADS7843_IRQ_GPIO_PORT                  GPIOC
#define ADS7843_IRQ_GPIO_CLK                   RCC_AHB1Periph_GPIOC
#else	
#ifdef SPI_MODE
/* ADS7853 SPI Interface pins  */
#define ADS7843_SPI                           SPI2
#define ADS7843_SPI_CLK                       RCC_APB1Periph_SPI2
#define ADS7843_SPI_CLK_INIT                  RCC_APB1PeriphClockCmd
#endif
/*SCK-pin*/
#define ADS7843_SPI_SCK_PIN                   GPIO_Pin_13
#define ADS7843_SPI_SCK_GPIO_PORT             GPIOB
#define ADS7843_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#ifdef SPI_MODE
#define ADS7843_SPI_SCK_SOURCE                GPIO_PinSource13
#define ADS7843_SPI_SCK_AF                    GPIO_AF_SPI2
#endif


/*MISO-pin*/
#define ADS7843_SPI_MISO_PIN                  GPIO_Pin_2
#define ADS7843_SPI_MISO_GPIO_PORT            GPIOI
#define ADS7843_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOI
#ifdef SPI_MODE
#define ADS7843_SPI_MISO_SOURCE               GPIO_PinSource2
#define ADS7843_SPI_MISO_AF                   GPIO_AF_SPI2
#endif


/*MOSI-pin*/
#define ADS7843_SPI_MOSI_PIN                  GPIO_Pin_3
#define ADS7843_SPI_MOSI_GPIO_PORT            GPIOI
#define ADS7843_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOI
#ifdef SPI_MODE
#define ADS7843_SPI_MOSI_SOURCE               GPIO_PinSource3
#define ADS7843_SPI_MOSI_AF                   GPIO_AF_SPI2
#endif

#define ADS7843_SPI_EN_PIN                        GPIO_Pin_4
#define ADS7843_SPI_EN_GPIO_PORT                  GPIOE
#define ADS7843_SPI_EN_GPIO_CLK                   RCC_AHB1Periph_GPIOE

#define ADS7843_SPI_CS_PIN                        GPIO_Pin_12
#define ADS7843_SPI_CS_GPIO_PORT                  GPIOB
#define ADS7843_SPI_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOB

#define ADS7843_IRQ_PIN                        GPIO_Pin_11
#define ADS7843_IRQ_GPIO_PORT                  GPIOD
#define ADS7843_IRQ_GPIO_CLK                   RCC_AHB1Periph_GPIOD
#endif

#define ADS7843_IRQ_LOW()       !(GPIO_ReadInputDataBit(ADS7843_IRQ_GPIO_PORT,ADS7843_IRQ_PIN))
#define ADS7843_IRQ_HIGH()      (GPIO_ReadInputDataBit(ADS7843_IRQ_GPIO_PORT,ADS7843_IRQ_PIN))

/* Select sFLASH: Chip Select pin low */
#define ADS7843_CS_LOW()       GPIO_ResetBits(ADS7843_SPI_CS_GPIO_PORT, ADS7843_SPI_CS_PIN)
/* Deselect sFLASH: Chip Select pin high */
#define ADS7843_CS_HIGH()      GPIO_SetBits(ADS7843_SPI_CS_GPIO_PORT, ADS7843_SPI_CS_PIN)

#ifdef GPIO_MODE
#define ADS7843_CLK_LOW()       GPIO_ResetBits(ADS7843_SPI_SCK_GPIO_PORT, ADS7843_SPI_SCK_PIN)
#define ADS7843_CLK_HIGH()      GPIO_SetBits(ADS7843_SPI_SCK_GPIO_PORT, ADS7843_SPI_SCK_PIN)

#define ADS7843_MISO_LOW()       !(GPIO_ReadInputDataBit(ADS7843_SPI_MISO_GPIO_PORT,ADS7843_SPI_MISO_PIN))
#define ADS7843_MISO_HIGH()      (GPIO_ReadInputDataBit(ADS7843_SPI_MISO_GPIO_PORT,ADS7843_SPI_MISO_PIN))

#define ADS7843_MOSI_LOW()       GPIO_ResetBits(ADS7843_SPI_MOSI_GPIO_PORT, ADS7843_SPI_MOSI_PIN)
#define ADS7843_MOSI_HIGH()      GPIO_SetBits(ADS7843_SPI_MOSI_GPIO_PORT, ADS7843_SPI_MOSI_PIN)
#endif

#ifdef SPI_MODE
/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void SPI_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*!< Enable the SPI clock */
    ADS7843_SPI_CLK_INIT(ADS7843_SPI_CLK, ENABLE);

    /*!< Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(ADS7843_SPI_SCK_GPIO_CLK | ADS7843_SPI_MISO_GPIO_CLK |
                           ADS7843_SPI_MOSI_GPIO_CLK | ADS7843_SPI_CS_GPIO_CLK, ENABLE);

    /*!< SPI pins configuration *************************************************/

    /*!< Connect SPI pins to AF5 */
    GPIO_PinAFConfig(ADS7843_SPI_SCK_GPIO_PORT, ADS7843_SPI_SCK_SOURCE, ADS7843_SPI_SCK_AF);
    GPIO_PinAFConfig(ADS7843_SPI_MISO_GPIO_PORT, ADS7843_SPI_MISO_SOURCE, ADS7843_SPI_MISO_AF);
    GPIO_PinAFConfig(ADS7843_SPI_MOSI_GPIO_PORT, ADS7843_SPI_MOSI_SOURCE, ADS7843_SPI_MOSI_AF);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    /*!< SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = ADS7843_SPI_SCK_PIN;
    GPIO_Init(ADS7843_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /*!< SPI MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  ADS7843_SPI_MOSI_PIN;
    GPIO_Init(ADS7843_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /*!< SPI MISO pin configuration */
    GPIO_InitStructure.GPIO_Pin =  ADS7843_SPI_MISO_PIN;
    GPIO_Init(ADS7843_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

}
#endif

static void delay_TFT_1us(u16 time)
{
    u16 i = time * 10;

    while (i != 0) {
        i--;
    }
}
//SPI 的配置
void SPI_TouchConfig(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

#ifdef SPI_MODE
    SPI_InitTypeDef   SPI_InitStructure;
#endif

#ifdef GPIO_MODE
    /*!< Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(ADS7843_SPI_SCK_GPIO_CLK | ADS7843_SPI_MISO_GPIO_CLK |
                           ADS7843_SPI_MOSI_GPIO_CLK | ADS7843_SPI_CS_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    /*!< SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = ADS7843_SPI_SCK_PIN;
    GPIO_Init(ADS7843_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /*!< SPI MOSI pin configuration */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin =  ADS7843_SPI_MOSI_PIN;
    GPIO_Init(ADS7843_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /*!< SPI MISO pin configuration */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin =  ADS7843_SPI_MISO_PIN;
    GPIO_Init(ADS7843_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

#endif

#ifdef SPI_MODE

    SPI_LowLevel_Init();
    /*!< Disable the sFLASH_SPI  */
    SPI_Cmd(ADS7843_SPI, DISABLE);
    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;

    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(ADS7843_SPI, &SPI_InitStructure);

    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(ADS7843_SPI, ENABLE);

#endif

#if USE_GD == 1

#else	
    RCC_AHB1PeriphClockCmd(ADS7843_SPI_EN_GPIO_CLK, ENABLE);
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = ADS7843_SPI_EN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ADS7843_SPI_EN_GPIO_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(ADS7843_SPI_EN_GPIO_PORT, ADS7843_SPI_EN_PIN);
#endif


    /*!< Configure sFLASH Card CS pin in output pushpull mode ********************/
    GPIO_InitStructure.GPIO_Pin = ADS7843_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(ADS7843_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

    RCC_AHB1PeriphClockCmd(ADS7843_IRQ_GPIO_CLK, ENABLE);
    /*!< Configure ADS7843 IRQ ********************/
    GPIO_InitStructure.GPIO_Pin = ADS7843_IRQ_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(ADS7843_IRQ_GPIO_PORT, &GPIO_InitStructure);

    /*!< Deselect the ADS7843: Chip Select high */
    ADS7843_CS_HIGH();
#ifdef GPIO_MODE
    ADS7843_CLK_LOW();
#endif
    delay_TFT_1us(1000);
    ADS7843_CS_LOW();
    TPReadX();
    TPReadY();
    ADS7843_CS_HIGH();
}

unsigned char SPI_WriteByte(unsigned char data_in)
{
    unsigned char Data_Out = 0;

#ifdef GPIO_MODE
    unsigned char count = 0;

    for (count = 0; count < 8; count++) {
        ADS7843_CLK_LOW();

        if (data_in & 0x80) {
            ADS7843_MOSI_HIGH();
        } else {
            ADS7843_MOSI_LOW();
        }

        data_in <<= 1;
        delay_TFT_1us(10);

        ADS7843_CLK_HIGH();

        Data_Out = Data_Out << 1;

        if ((ADS7843_MISO_HIGH()) & 0x01) {
            Data_Out++;
        } else {
            Data_Out = Data_Out;
        }

        delay_TFT_1us(10);
    }

    ADS7843_CLK_LOW();
#endif

#ifdef SPI_MODE

    /*!< Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(ADS7843_SPI, SPI_I2S_FLAG_TXE) == RESET);

    /*!< Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(ADS7843_SPI, data_in);

    /*!< Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(ADS7843_SPI, SPI_I2S_FLAG_RXNE) == RESET);

    /*!< Return the byte read from the SPI bus */
    Data_Out = SPI_I2S_ReceiveData(ADS7843_SPI);

#endif
    return (Data_Out);
}

//从触摸屏上读取信息
u16 TPReadX(void)
{
    u16 z = 0;
    ADS7843_CS_LOW();
    delay_TFT_1us(30);
    SPI_WriteByte(0x90);//XY外部链接颠倒了
    delay_TFT_1us(30);
    z = SPI_WriteByte(0x00);
    z <<= 8;
    z += SPI_WriteByte(0x00);
    delay_TFT_1us(30);
    ADS7843_CS_HIGH();
    z = z >> 3;
    return(z);
}

u16 TPReadY(void)
{
    u16 y = 0;
    ADS7843_CS_LOW();
    delay_TFT_1us(30);
    SPI_WriteByte(0xd0);
    delay_TFT_1us(30);
    y = SPI_WriteByte(0x00);
    y <<= 8;
    y += SPI_WriteByte(0x00);
    delay_TFT_1us(30);
    ADS7843_CS_HIGH();
    y = y >> 3;
    return(y);
}
/*
* pressed return 0,else return 1
*/
u8 ADS7843_IRQ(void)
{
    if ((ADS7843_IRQ_HIGH()) & 0x01) {
        return 1;
    } else {
        return 0;
    }
}
/* 读取次数 */
#define ADS7843_READ_TIMES    5
/* 丢弃值  */
#define ADS7843_LOST_VAL      1
uint16_t TOUCH_DataFilter(uint8_t _ucCh)
{
    uint16_t i, j;
    uint16_t buf[ADS7843_READ_TIMES];
    uint16_t usSum;
    uint16_t usTemp;

    /* 读取READ_TIMES次数据*/
    for (i = 0; i < ADS7843_READ_TIMES; i++) {

        if (_ucCh == 1) {
            buf[i] = TPReadX();
        } else {
            buf[i] = TPReadY();
        }

		OSTimeDly(1);
    }

    /* 升序排列 */
    for (i = 0; i < ADS7843_READ_TIMES - 1; i++) {
        for (j = i + 1; j < ADS7843_READ_TIMES; j++) {
            if (buf[i] > buf[j]) {
                usTemp = buf[i];
                buf[i] = buf[j];
                buf[j] = usTemp;
            }
        }
    }

    usSum = 0;

    /*求和 */
    for (i = ADS7843_LOST_VAL; i < ADS7843_READ_TIMES - ADS7843_LOST_VAL; i++) {
        usSum += buf[i];
    }

    /*求平均 */
    usTemp = usSum / (ADS7843_READ_TIMES - 2 * ADS7843_LOST_VAL);

    return usTemp;
}
int32_t TOUCH_Abs(int32_t x)
{
    if (x >= 0) {
        return x;
    } else {
        return -x;
    }
}
/*
*********************************************************************************************************
*	函 数 名: TOUCH_ReadAdcXY
*	功能说明: 连续2次读取触摸屏IC,且这两次的偏差不能超过
*             ADC_ERR_RANGE,满足条件,则认为读数正确,否则读数错误.
*             该函数能大大提高准确度
*	形    参：x,y:读取到的坐标值
*	返 回 值: 0,失败;1,成功
*********************************************************************************************************
*/
/* 误差范围 */
uint8_t ADC_ERR_RANGE = 10;
int16_t usAdcNowX;
int16_t usAdcNowY;
uint8_t TOUCH_ReadAdcXY(int16_t *_usX, int16_t *_usY)
{
    uint16_t iX1, iY1;
    uint16_t iX2, iY2;
    uint16_t iX, iY;

    iX1 = TOUCH_DataFilter(ADC_CH_X);
    iX2 = TOUCH_DataFilter(ADC_CH_X);
    iY1 = TOUCH_DataFilter(ADC_CH_Y);
    iY2 = TOUCH_DataFilter(ADC_CH_Y);

    iX = TOUCH_Abs(iX1 - iX2);
    iY = TOUCH_Abs(iY1 - iY2);
    usAdcNowX = iX1;
    usAdcNowY = iY1;

    /* 前后两次采样在+-ERR_RANGE内 */
    if ((iX <= ADC_ERR_RANGE) && (iY <= ADC_ERR_RANGE)) {
        *_usX = (iX1 + iX2) / 2;
        *_usY = (iY1 + iY2) / 2;

        return 1;
    } else {
        return 0;
    }
}


/*
*********************************************************************************************************
*	函 数 名: TOUCH_Scan
*	功能说明: 触摸板事件检测程序。该函数被周期性调用，每ms调用1次. 见 bsp_Timer.c
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

u16 terminal_turnoff_time = TERMINAL_TURNOFF_TIME_DEF;
u16 send_time = 0;

void TOUCH_SCAN(void)
{
    uint8_t s_invalid_count = 0;

    if (ADS7843_IRQ_HIGH() == 0) {
        if (basic_inf.ss_time == 0) {
            terminal_turnoff_time = TERMINAL_TURNOFF_TIME_DEF;
        } else {
            terminal_turnoff_time = basic_inf.ss_time;
        }

        if (g_tTP.Enable == 1) { //只读取第一次的数据
            return;
        }
		
		OSTimeDly(30);
        while (!TOUCH_ReadAdcXY(&g_tTP.usAdcNowX, &g_tTP.usAdcNowY) && s_invalid_count < 7) {
            s_invalid_count++;
        }

        if (s_invalid_count >= 7) {
            g_tTP.usAdcNowX = -1;
            g_tTP.usAdcNowY = -1;
            g_tTP.Enable = 0;
        } else {
            g_tTP.Enable = 1;
        }

    } else {
        send_time++;

        if (send_time >= 500) {
            send_time = 0;
            TOUCH_ReadAdcXY(&g_tTP.usAdcNowX, &g_tTP.usAdcNowY);
        }

        g_tTP.usAdcNowX = -1;
        g_tTP.usAdcNowY = -1;
        g_tTP.Enable = 0;
    }

}

u8 get_touch_irq(void)
{
    return ADS7843_IRQ_HIGH();
}

u8 Get_LCD_State(void)
{
    if (terminal_turnoff_time > 0)
        return 1;

    return 0;
}

void LCD_State_SW(void)
{
    if (terminal_turnoff_time > 0)
        terminal_turnoff_time--;
}

