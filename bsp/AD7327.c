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
#include "AD7327.h"
#include "common.h"
#include "ucos_ii.h"
#include "bsp.h"
#include "Parameters.h"
#include "app.h"


#define SPI_MODE  0
//#define GPIO_MODE 0




//Registers
#define Write 0x8000
#define ControlRegister 0x0000
#define RangeRegister1 0x2000
#define RangeRegister2 0x4000
#define SequenceRegister 0x6000

//Control Register
#define CH0 0x0000
#define CH1 0x0400
#define CH2 0x0800
#define CH3 0x0C00
#define CH4 0x1000
#define CH5 0x1400
#define CH6 0x1800
#define CH7 0x1C00

#define Mode0 0x0000
#define Mode1 0x0100
#define Mode2 0x0200
#define Mode3 0x0300

#define Normal 0x0000
#define AutoStandby 0x0040
#define AutoShutdown 0x0080
#define FullShutdown 0x00C0

#define TwoCompliment 0x0000
#define StraightBinary 0x0020

#define Ref_Dis 0x0000
#define Ref_En 0x0010

#define SequencerOff 0x00
#define SequencerPrev 0x0004
#define Sequencer 0x0008

//Range Register1
#define IN10vCH0 0x0000
#define IN5vCH0 0x0800
#define IN2p5vCH0 0x1000
#define IN0_10vCH0 0x1800

#define IN10vCH1 0x0000
#define IN5vCH1 0x0200
#define IN2p5vCH1 0x0400
#define IN0_10vCH1 0x0600

#define IN10vCH2 0x0000
#define IN5vCH2 0x0080
#define IN2p5vCH2 0x0100
#define IN0_10vCH2 0x0180

#define IN10vCH3 0x0000
#define IN5vCH3 0x0020
#define IN2p5vCH3 0x0040
#define IN0_10vCH3 0x0060

//Range Register2
#define IN10vCH4 0x0000
#define IN5vCH4 0x0800
#define IN2p5vCH4 0x1000
#define IN0_10vCH4 0x1800

#define IN10vCH5 0x0000
#define IN5vCH5 0x0200
#define IN2p5vCH5 0x0400
#define IN0_10vCH5 0x0600

#define IN10vCH6 0x0000
#define IN5vCH6 0x0080
#define IN2p5vCH6 0x0100
#define IN0_10vCH6 0x0180

#define IN10vCH7 0x0000
#define IN5vCH7 0x0020
#define IN2p5vCH7 0x0040
#define IN0_10vCH7 0x0060

//Sequencer
#define VIN0 0x1000
#define VIN1 0x0800
#define VIN2 0x0400
#define VIN3 0x0200
#define VIN4 0x0100
#define VIN5 0x0080
#define VIN6 0x0040
#define VIN7 0x0020

//上电的寄存器设置
//电压输入范围可以单独设置
//通道0`3的电压输入范围
////初始化寄存器的时候设为最大-10~+10
#define write_range1_register   0xa000  //输入范围-10~+10 

#define write_range1_register_10_10   0xa000  //输入范围-10~+10
#define write_range1_register_5       0xaaa0  //输入范围+-5
#define write_range1_register_2_5     0xb550  //输入范围+-2.5
#define write_range1_register_0_10    0xbfe0  //输入范围0~10



//通道4~7的电压输入范围
////初始化寄存器的时候设为最大-10~+10
#define wirte_range2_register   0xc000  //输入范围-10~+10


#define wirte_range2_register_10_10   0xc000  //输入范围-10~+10
#define wirte_range2_register_5       0xcaa0  //输入范围+-5
#define wirte_range2_register_2_5     0xd550  //输入范围+2.5
#define wirte_range2_register_0_10    0xdfe0  //输入范围0~10


#define write_sequence_register 0xFFE0   //通道全部选通

//#define write_sequence_register 0xffe0   //通道全部选通//0xE800

//控制寄存器的设置包括输入模式是单端还是差分，输入地址等
//初始化寄存器的时候设为单端模式
#define write_control_register  0x8030   //单端模式默认第一通道

//选择单端输入通道
#define write_control_register_1  0x8030
#define write_control_register_2  0x8430
#define write_control_register_3  0x8830
#define write_control_register_4  0x8c30
#define write_control_register_5  0x9030
#define write_control_register_6  0x9430
#define write_control_register_7  0x9830
#define write_control_register_8  0x9c30


#define set_single_ended_mode          0x8030   //单端模式  
#define set_fully_differential_mode    0x9030   //全差分模式
#define set_4pseudo_differential_mode  0x8830  //4伪差分
#define set_7pseudo_differential_mode  0x9830  //7伪差分

//读取单端输入
#define read_single_ended_1  0x8030
#define read_single_ended_2  0x8430
#define read_single_ended_3  0x8830
#define read_single_ended_4  0x8c30
#define read_single_ended_5  0x9030
#define read_single_ended_6  0x9430
#define read_single_ended_7  0x9830
#define read_single_ended_8  0x9c30

#define read_adc128s_1  0x0000
#define read_adc128s_2  0x0800
#define read_adc128s_3  0x1000
#define read_adc128s_4  0x1800
#define read_adc128s_5  0x2000
#define read_adc128s_6  0x2800
#define read_adc128s_7  0x3000
#define read_adc128s_8  0x3800

//读取差分
#define read_fully_differential_1  0x8230
#define read_fully_differential_2  0x8630

#define read_fully_differential_3  0x8A30
#define read_fully_differential_4  0x8E30

#define read_fully_differential_5  0x9230
#define read_fully_differential_6  0x9630

#define read_fully_differential_7  0x9A30
#define read_fully_differential_8  0x9E30

//设置通道的电压输入范围
#define set_channel_1_voltage_10_10   0xa000
#define set_channel_1_voltage_5       0xa800
#define set_channel_1_voltage_2_5     0xb000
#define set_channel_1_voltage_10      0xb800

#define set_channel_2_voltage_10_10   0xa000
#define set_channel_2_voltage_5       0xa200
#define set_channel_2_voltage_2_5     0xa400
#define set_channel_2_voltage_10      0xa600

#define set_channel_3_voltage_10_10   0xa000
#define set_channel_3_voltage_5       0xa080
#define set_channel_3_voltage_2_5     0xa100
#define set_channel_3_voltage_10      0xa180

#define set_channel_4_voltage_10_10   0xa000
#define set_channel_4_voltage_5       0xa020
#define set_channel_4_voltage_2_5     0xa040
#define set_channel_4_voltage_10      0xa060

#define set_channel_5_voltage_10_10   0xc000
#define set_channel_5_voltage_5       0xc800
#define set_channel_5_voltage_2_5     0xd000
#define set_channel_5_voltage_10      0xd800

#define set_channel_6_voltage_10_10   0xc000
#define set_channel_6_voltage_5       0xc200
#define set_channel_6_voltage_2_5     0xc400
#define set_channel_6_voltage_10      0xc600

#define set_channel_7_voltage_10_10   0xc000
#define set_channel_7_voltage_5       0xc080
#define set_channel_7_voltage_2_5     0xc100
#define set_channel_7_voltage_10      0xc180

#define set_channel_8_voltage_10_10   0xc000
#define set_channel_8_voltage_5       0xc020
#define set_channel_8_voltage_2_5     0xc040
#define set_channel_8_voltage_10      0xc060

#if USE_GD == 1

#ifdef SPI_MODE
/* ADS7853 SPI Interface pins  */
#define AD7327_SPI                           SPI5
#define AD7327_SPI_CLK                       RCC_APB2Periph_SPI5
#define AD7327_SPI_CLK_INIT                  RCC_APB2PeriphClockCmd
#endif
/*SCK-pin*/
#define AD7327_SPI_SCK_PIN                   GPIO_Pin_7
#define AD7327_SPI_SCK_GPIO_PORT             GPIOF
#define AD7327_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOF
#ifdef SPI_MODE
#define AD7327_SPI_SCK_SOURCE                GPIO_PinSource7
#define AD7327_SPI_SCK_AF                    GPIO_AF_SPI5
#endif


/*MISO-pin*/
#define AD7327_SPI_MISO_PIN                  GPIO_Pin_8
#define AD7327_SPI_MISO_GPIO_PORT            GPIOF
#define AD7327_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOF
#ifdef SPI_MODE
#define AD7327_SPI_MISO_SOURCE               GPIO_PinSource8
#define AD7327_SPI_MISO_AF                   GPIO_AF_SPI5
#endif


/*MOSI-pin*/
#define AD7327_SPI_MOSI_PIN                  GPIO_Pin_9
#define AD7327_SPI_MOSI_GPIO_PORT            GPIOF
#define AD7327_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOF
#ifdef SPI_MODE
#define AD7327_SPI_MOSI_SOURCE               GPIO_PinSource9
#define AD7327_SPI_MOSI_AF                   GPIO_AF_SPI5
#endif

#define AD7327_SPI_CS_PIN                        GPIO_Pin_4
#define AD7327_SPI_CS_GPIO_PORT                  GPIOD
#define AD7327_SPI_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOD


#else

#ifdef SPI_MODE
/* ADS7853 SPI Interface pins  */
#define AD7327_SPI                           SPI4
#define AD7327_SPI_CLK                       RCC_APB2Periph_SPI4
#define AD7327_SPI_CLK_INIT                  RCC_APB2PeriphClockCmd
#endif
/*SCK-pin*/
#define AD7327_SPI_SCK_PIN                   GPIO_Pin_2
#define AD7327_SPI_SCK_GPIO_PORT             GPIOE
#define AD7327_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOE
#ifdef SPI_MODE
#define AD7327_SPI_SCK_SOURCE                GPIO_PinSource2
#define AD7327_SPI_SCK_AF                    GPIO_AF_SPI4
#endif


/*MISO-pin*/
#define AD7327_SPI_MISO_PIN                  GPIO_Pin_5
#define AD7327_SPI_MISO_GPIO_PORT            GPIOE
#define AD7327_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOE
#ifdef SPI_MODE
#define AD7327_SPI_MISO_SOURCE               GPIO_PinSource5
#define AD7327_SPI_MISO_AF                   GPIO_AF_SPI4
#endif


/*MOSI-pin*/
#define AD7327_SPI_MOSI_PIN                  GPIO_Pin_6
#define AD7327_SPI_MOSI_GPIO_PORT            GPIOE
#define AD7327_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOE
#ifdef SPI_MODE
#define AD7327_SPI_MOSI_SOURCE               GPIO_PinSource6
#define AD7327_SPI_MOSI_AF                   GPIO_AF_SPI4
#endif

#define AD7327_SPI_CS_PIN                        GPIO_Pin_9
#define AD7327_SPI_CS_GPIO_PORT                  GPIOH
#define AD7327_SPI_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOH

#endif

/* Select sFLASH: Chip Select pin low */
#define AD7327_CS_LOW()       GPIO_ResetBits(AD7327_SPI_CS_GPIO_PORT, AD7327_SPI_CS_PIN)
/* Deselect sFLASH: Chip Select pin high */
#define AD7327_CS_HIGH()      GPIO_SetBits(AD7327_SPI_CS_GPIO_PORT, AD7327_SPI_CS_PIN)

#ifdef GPIO_MODE
#define AD7327_CLK_LOW()       GPIO_ResetBits(AD7327_SPI_SCK_GPIO_PORT, AD7327_SPI_SCK_PIN)
#define AD7327_CLK_HIGH()      GPIO_SetBits(AD7327_SPI_SCK_GPIO_PORT, AD7327_SPI_SCK_PIN)

#define AD7327_MISO_LOW()       !(GPIO_ReadInputDataBit(AD7327_SPI_MISO_GPIO_PORT,AD7327_SPI_MISO_PIN))
#define AD7327_MISO_HIGH()      (GPIO_ReadInputDataBit(AD7327_SPI_MISO_GPIO_PORT,AD7327_SPI_MISO_PIN))

#define AD7327_MOSI_LOW()       GPIO_ResetBits(AD7327_SPI_MOSI_GPIO_PORT, AD7327_SPI_MOSI_PIN)
#define AD7327_MOSI_HIGH()      GPIO_SetBits(AD7327_SPI_MOSI_GPIO_PORT, AD7327_SPI_MOSI_PIN)
#endif

#ifdef SPI_MODE
/**
  * @brief  Initializes the peripherals used by the SPI FLASH driver.
  * @param  None
  * @retval None
  */
void SPI_AD7327_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*!< Enable the SPI clock */
    AD7327_SPI_CLK_INIT(AD7327_SPI_CLK, ENABLE);

    /*!< Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(AD7327_SPI_SCK_GPIO_CLK | AD7327_SPI_MISO_GPIO_CLK |
                           AD7327_SPI_MOSI_GPIO_CLK | AD7327_SPI_CS_GPIO_CLK, ENABLE);

    /*!< SPI pins configuration *************************************************/

    /*!< Connect SPI pins to AF5 */
    GPIO_PinAFConfig(AD7327_SPI_SCK_GPIO_PORT, AD7327_SPI_SCK_SOURCE, AD7327_SPI_SCK_AF);
    GPIO_PinAFConfig(AD7327_SPI_MISO_GPIO_PORT, AD7327_SPI_MISO_SOURCE, AD7327_SPI_MISO_AF);
    GPIO_PinAFConfig(AD7327_SPI_MOSI_GPIO_PORT, AD7327_SPI_MOSI_SOURCE, AD7327_SPI_MOSI_AF);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    /*!< SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = AD7327_SPI_SCK_PIN;
    GPIO_Init(AD7327_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /*!< SPI MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  AD7327_SPI_MOSI_PIN;
    GPIO_Init(AD7327_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /*!< SPI MISO pin configuration */
    GPIO_InitStructure.GPIO_Pin =  AD7327_SPI_MISO_PIN;
    GPIO_Init(AD7327_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

}
#endif

static void dalay_AD7327_1us(u16 time)
{
    u16 i = time * 2;

    while (i != 0) {
        i--;
    }
}
//SPI 的配置
void SPI_AD7327Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

#ifdef SPI_MODE
    SPI_InitTypeDef   SPI_InitStructure;
#endif

#ifdef GPIO_MODE
    /*!< Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(AD7327_SPI_SCK_GPIO_CLK | AD7327_SPI_MISO_GPIO_CLK |
                           AD7327_SPI_MOSI_GPIO_CLK | AD7327_SPI_CS_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    /*!< SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = AD7327_SPI_SCK_PIN;
    GPIO_Init(AD7327_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

    /*!< SPI MOSI pin configuration */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin =  AD7327_SPI_MOSI_PIN;
    GPIO_Init(AD7327_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /*!< SPI MISO pin configuration */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin =  AD7327_SPI_MISO_PIN;
    GPIO_Init(AD7327_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

#endif

#ifdef SPI_MODE

    SPI_AD7327_LowLevel_Init();
    /*!< Disable the sFLASH_SPI  */
    SPI_Cmd(AD7327_SPI, DISABLE);
    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;

    if (get_hardware_ver() >= HARD_V2) {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    } else {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    }

    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;

    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(AD7327_SPI, &SPI_InitStructure);

    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(AD7327_SPI, ENABLE);

#endif

    /*!< Configure sFLASH Card CS pin in output pushpull mode ********************/
    GPIO_InitStructure.GPIO_Pin = AD7327_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(AD7327_SPI_CS_GPIO_PORT, &GPIO_InitStructure);


    /*!< Deselect the AD7327: Chip Select high */
    AD7327_CS_HIGH();
#ifdef GPIO_MODE
    AD7327_CLK_LOW();
#endif

}


static s16 AD7327_SPI_FLASH_Write_ReadByte(u16 data_in)
{
    s16 Data_Out = 0;

#ifdef GPIO_MODE
    unsigned char count = 0;

    for (count = 0; count < 16; count++) {
        AD7327_CLK_LOW();

        if (data_in & 0x80) {
            AD7327_MOSI_HIGH();
        } else {
            AD7327_MOSI_LOW();
        }

        data_in <<= 1;
        dalay_AD7327_1us(10);

        AD7327_CLK_HIGH();

        Data_Out = Data_Out << 1;

        if ((AD7327_MISO_HIGH()) & 0x01) {
            Data_Out++;
        } else {
            Data_Out = Data_Out;
        }

        dalay_AD7327_1us(10);
    }

    AD7327_CLK_LOW();
#endif

#ifdef SPI_MODE

    /*!< Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(AD7327_SPI, SPI_I2S_FLAG_TXE) == RESET);

    /*!< Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(AD7327_SPI, data_in);

    /*!< Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(AD7327_SPI, SPI_I2S_FLAG_RXNE) == RESET);

    /*!< Return the byte read from the SPI bus */
    Data_Out = SPI_I2S_ReceiveData(AD7327_SPI);

#endif
    return (Data_Out);
}
#if USE_GD == 1

#define CD4051_A0_PIN                   GPIO_Pin_11
#define CD4051_A0_GPIO_PORT             GPIOD
#define CD4051_A0_GPIO_CLK              RCC_AHB1Periph_GPIOD

#define CD4051_A1_PIN                   GPIO_Pin_12
#define CD4051_A1_GPIO_PORT             GPIOB
#define CD4051_A1_GPIO_CLK              RCC_AHB1Periph_GPIOB

#define CD4051_A2_PIN                   GPIO_Pin_13
#define CD4051_A2_GPIO_PORT             GPIOB
#define CD4051_A2_GPIO_CLK              RCC_AHB1Periph_GPIOB

#else

#define CD4051_OE_PIN                   GPIO_Pin_8
#define CD4051_OE_GPIO_PORT             GPIOI
#define CD4051_OE_GPIO_CLK              RCC_AHB1Periph_GPIOI

#define CD4051_A0_PIN                   GPIO_Pin_13
#define CD4051_A0_GPIO_PORT             GPIOC
#define CD4051_A0_GPIO_CLK              RCC_AHB1Periph_GPIOC

#define CD4051_A1_PIN                   GPIO_Pin_6
#define CD4051_A1_GPIO_PORT             GPIOF
#define CD4051_A1_GPIO_CLK              RCC_AHB1Periph_GPIOF

#define CD4051_A2_PIN                   GPIO_Pin_7
#define CD4051_A2_GPIO_PORT             GPIOF
#define CD4051_A2_GPIO_CLK              RCC_AHB1Periph_GPIOF

#endif

#define CD4051_A0_ON  GPIO_SetBits(CD4051_A0_GPIO_PORT, CD4051_A0_PIN)
#define CD4051_A0_OFF  GPIO_ResetBits(CD4051_A0_GPIO_PORT, CD4051_A0_PIN)
#define CD4051_A1_ON  GPIO_SetBits(CD4051_A1_GPIO_PORT, CD4051_A1_PIN)
#define CD4051_A1_OFF  GPIO_ResetBits(CD4051_A1_GPIO_PORT, CD4051_A1_PIN)
#define CD4051_A2_ON  GPIO_SetBits(CD4051_A2_GPIO_PORT, CD4051_A2_PIN)
#define CD4051_A2_OFF  GPIO_ResetBits(CD4051_A2_GPIO_PORT, CD4051_A2_PIN)

void CD4501_Mode(u8 mode)
{
    if (mode & 0x01) {
        CD4051_A0_OFF;
    } else {
        CD4051_A0_ON;
    }

    mode >>= 1;

    if (mode & 0x01) {
        CD4051_A1_OFF;
    } else {
        CD4051_A1_ON;
    }

    mode >>= 1;

    if (mode & 0x01) {
        CD4051_A2_OFF;
    } else {
        CD4051_A2_ON;
    }

}


void CD4051_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
#if USE_GD == 1

#else
    RCC_AHB1PeriphClockCmd(CD4051_OE_GPIO_CLK, ENABLE);
    //EN
    GPIO_InitStructure.GPIO_Pin = CD4051_OE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CD4051_OE_GPIO_PORT , &GPIO_InitStructure);
    GPIO_SetBits(CD4051_OE_GPIO_PORT, CD4051_OE_PIN);
	
#endif

    RCC_AHB1PeriphClockCmd(CD4051_A0_GPIO_CLK, ENABLE);
    //A0
    GPIO_InitStructure.GPIO_Pin = CD4051_A0_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CD4051_A0_GPIO_PORT , &GPIO_InitStructure);


    RCC_AHB1PeriphClockCmd(CD4051_A1_GPIO_CLK, ENABLE);
    //A1
    GPIO_InitStructure.GPIO_Pin = CD4051_A1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CD4051_A1_GPIO_PORT , &GPIO_InitStructure);


    RCC_AHB1PeriphClockCmd(CD4051_A2_GPIO_CLK, ENABLE);
    //A2
    GPIO_InitStructure.GPIO_Pin = CD4051_A2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CD4051_A2_GPIO_PORT , &GPIO_InitStructure);

    CD4501_Mode(0);

}


uint16_t WriteToControlRegister (uint16_t Address, uint16_t Mode, uint16_t PowerManagement, uint16_t Coding, uint16_t RefSel, uint16_t Sequence)
{
    uint16_t DataToWrite = 0x00;
    uint16_t ReturnData = 0x00;

    DataToWrite = Write;
    DataToWrite = DataToWrite + ControlRegister;
    DataToWrite = DataToWrite + Address + Mode + PowerManagement + Coding + RefSel + Sequence;

    ReturnData = AD7327_SPI_FLASH_Write_ReadByte(DataToWrite);
    return ReturnData;
}

uint16_t WriteToRangeRegister1 (uint16_t RangeV0, uint16_t RangeV1, uint16_t RangeV2, uint16_t RangeV3)
{
    uint16_t DataToWrite = 0x00;
    uint16_t ReturnData = 0x00;

    DataToWrite = Write;
    DataToWrite = DataToWrite | RangeRegister1;
    DataToWrite = DataToWrite | RangeV0 | RangeV1 | RangeV2 | RangeV3;

    ReturnData = AD7327_SPI_FLASH_Write_ReadByte(DataToWrite);
    return ReturnData;
}

uint16_t WriteToRangeRegister2 (uint16_t RangeV4, uint16_t RangeV5, uint16_t RangeV6, uint16_t RangeV7)
{
    uint16_t DataToWrite = 0x00;
    uint16_t ReturnData = 0x00;

    DataToWrite = Write;
    DataToWrite = DataToWrite + RangeRegister2;
    DataToWrite = DataToWrite + RangeV4 + RangeV5 + RangeV6 + RangeV7;

    ReturnData = AD7327_SPI_FLASH_Write_ReadByte(DataToWrite);
    return ReturnData;
}

uint16_t WriteToSequenceRegister (uint16_t InputPin)
{
    uint16_t DataToWrite = 0x00;
    uint16_t ReturnData = 0x00;

    DataToWrite = Write;
    DataToWrite = DataToWrite + SequenceRegister;
    DataToWrite = DataToWrite + InputPin;

    ReturnData = AD7327_SPI_FLASH_Write_ReadByte(DataToWrite);
    return ReturnData;
}

uint16_t ReadSequence (void)
{
    uint16_t DataToWrite = 0x00;
    uint16_t ReturnData = 0x00;

// 	SpiCfg(pADI_SPI0,SPICON_MOD_TX1RX1,SPICON_MASEN_EN,SPICON_CON_EN|
// 					SPICON_RXOF_EN|SPICON_ZEN_EN|SPICON_TIM_TXWR|SPICON_CPHA_SAMPLELEADING|
// 					SPICON_CPOL_HIGH|SPICON_ENABLE_EN);

    DataToWrite = 0x0000;

    ReturnData = AD7327_SPI_FLASH_Write_ReadByte(DataToWrite);
    return ReturnData;
}



void AD7327_Write_Register(u16 register_data)
{
    dalay_AD7327_1us(10);
    AD7327_CS_LOW();
    dalay_AD7327_1us(10);
    AD7327_SPI_FLASH_Write_ReadByte(register_data);
    AD7327_CS_HIGH() ;
    dalay_AD7327_1us(10);
}

u16 AD7327_Read_Register(void)
{
    u16 ad_data;
    AD7327_CS_LOW();
    dalay_AD7327_1us(10);
    ad_data = AD7327_SPI_FLASH_Write_ReadByte(0x0000);
    AD7327_CS_HIGH();
    dalay_AD7327_1us(10);
    ad_data = ad_data & 0x0fff; //只需要12位,没有负数，不要符号位
    return ad_data;
}

u16 AD7327_Read_Register_0_10(void)
{
    u16 ad_data;
    AD7327_CS_LOW();
    dalay_AD7327_1us(10);
    ad_data = AD7327_SPI_FLASH_Write_ReadByte(0x0000);
    AD7327_CS_HIGH();
    dalay_AD7327_1us(10);
    ad_data = ad_data & 0x1fff; //需要13位
    return ad_data;
}

void AD7327_Init(void)
{
    CD4051_Init();
    SPI_AD7327Config();

    if (get_hardware_ver() >= HARD_V2) {
        return;
    }

    AD7327_CS_LOW();
    delay_us(100);
    WriteToRangeRegister1 (IN5vCH0, IN5vCH1, IN5vCH2, IN5vCH3);
    AD7327_CS_HIGH();
    delay_ms(100);//不知道为什么必须设置两遍才生效


    AD7327_CS_LOW();
    dalay_AD7327_1us(100);
    WriteToRangeRegister1 (IN5vCH0, IN5vCH1, IN5vCH2, IN5vCH3);
    AD7327_CS_HIGH();
    dalay_AD7327_1us(100);

    AD7327_CS_LOW();
    dalay_AD7327_1us(100);
    WriteToRangeRegister2 (IN5vCH4, IN5vCH5, IN5vCH6, IN0_10vCH7);
    AD7327_CS_HIGH();
    dalay_AD7327_1us(100);

    AD7327_CS_LOW();
    dalay_AD7327_1us(100);
    WriteToSequenceRegister (VIN7);//指定通道
    dalay_AD7327_1us(100);

    AD7327_CS_LOW();
    dalay_AD7327_1us(100);
    WriteToControlRegister(CH3, Mode0, Normal, StraightBinary, Ref_En, SequencerOff);
    AD7327_CS_HIGH();
    dalay_AD7327_1us(100);

    AD7327_Write_Register(read_single_ended_1);
    AD7327_Read_Register();
    AD7327_Read_Register();//读取第一次可能为0的
}

#define AD_TIME		(5)
#define AD_NUM		(15)
const u16 read_single_ended[8] = {read_single_ended_1, read_single_ended_2, read_single_ended_3, read_single_ended_4, \
                                  read_single_ended_5, read_single_ended_6, read_single_ended_7, read_single_ended_8
                                 };

const u16 read_adc128s[8] = {read_adc128s_1, read_adc128s_2, read_adc128s_3, read_adc128s_4, \
                             read_adc128s_5, read_adc128s_6, read_adc128s_7, read_adc128s_8
                            };

u16 ad7327_value[AD_NUM][AD_TIME] = {0};
static u8 read_num = 0;
static u8 read_flag = 0;//第一次采集

//定时调用
void AD7327_Proc(void)
{
    u8 i = 0, j = 0, id = 0;

    for (i = 0 ; i < 7; i++) {
        if (get_hardware_ver() < HARD_V2) {
            AD7327_Write_Register(read_single_ended[i]);
        } else {
            AD7327_Write_Register(read_adc128s[i]);
        }

        ad7327_value[id++][read_num] = AD7327_Read_Register();
        OSTimeDly(5);
    }

//    AD7327_Write_Register(read_single_ended_8);

//    for (i = 0; i < 6; i++) {
//        CD4501_Mode(i);
//        OSTimeDly(10);
//        ad7327_value[id++][read_num] = AD7327_Read_Register_0_10();
//    }

    read_num++;

    if (read_num >= AD_TIME) {
        read_num = 0;
    }

    if (read_flag == 0) {//初始化全部相等
        read_flag = 1;
        read_num = 0;

        for (i = 0; i < AD_NUM; i++) {
            for (j = 1; j < AD_TIME; j++) {
                ad7327_value[i][j] = ad7327_value[i][0];
            }
        }
    }
}

static u8 read_win_num = 0;
//这个函数要放在AD7327_Proc前面
void AD7327_WIN_Proc(void)
{
    u8 i = 0, id = 7;

    if (get_hardware_ver() < HARD_V2) {
        AD7327_Write_Register(read_single_ended_8);
    }

    for (i = 0; i < 8; i++) {
        if (i < SENSOR_NUM_MAX && get_sensor_sta(i) < 0) {
            id++;
            continue;
        }

        if (get_hardware_ver() >= HARD_V2) {
            AD7327_Write_Register(read_adc128s_8);
        }

        CD4501_Mode(i);
        OSTimeDly(10);
        ad7327_value[id++][read_win_num] = AD7327_Read_Register_0_10();
    }

    read_win_num++;

    if (read_win_num >= AD_TIME) {
        read_win_num = 0;
    }
}

static u16 filter_buffer[AD_TIME];
u16 Aver_Filter_u16(u16 *buffer, INT8U num)
{
    INT8U i, j;
    u32 temp_int;

    if (num > AD_TIME)
        return 0;

    for (i = 0; i < num; i++) {
        filter_buffer[i] = buffer[i];
    }

    //中位值平均滤波法
    //排序
    for (j = 0; j < num - 1; j++) {
        for (i = 0; i < num - j - 1; i++) {
            if ( filter_buffer[i] > filter_buffer[i + 1] ) {
                temp_int = filter_buffer[i];
                filter_buffer[i] = filter_buffer[i + 1];
                filter_buffer[i + 1] = temp_int;
            }
        }
    }

    //去掉最大和最小值后求均值
    temp_int = 0;

    for (i = 1; i < num - 1; i++) {
        temp_int += filter_buffer[i];
    }

    return (temp_int / (num - 2));
}

int Get_AD_Value(u8 sid)
{
    u16 value = 0;

    if (sid > AD_NUM)
        return -1;

    value = Aver_Filter_u16(ad7327_value[sid], AD_TIME);
    return value;
}

