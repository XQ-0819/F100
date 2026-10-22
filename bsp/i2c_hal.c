//=============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//=============================================================================
/// \file    i2c_hal.c (V1.0)
/// \author  RFU
/// \date    24-Jan-2016
/// \brief   I2C hardware abstraction layer
//=============================================================================
#include "i2c_hal.h"
#include "common.h"


/* ¶Ë¿ÚÉùÃ÷ */

#if USE_GD == 1

#define SDP810_I2C_Sda_GPIO GPIOC
#define SDP810_I2C_Sda_ClockCmd RCC_AHB1Periph_GPIOC
#define SDP810_I2C_Sda_PIN GPIO_Pin_4
#define SDP810_I2C_Scl_GPIO GPIOC
#define SDP810_I2C_Scl_ClockCmd RCC_AHB1Periph_GPIOC
#define SDP810_I2C_Scl_PIN GPIO_Pin_5

#else
#define SDP810_I2C_Sda_GPIO GPIOH
#define SDP810_I2C_Sda_ClockCmd RCC_AHB1Periph_GPIOH
#define SDP810_I2C_Sda_PIN GPIO_Pin_8
#define SDP810_I2C_Scl_GPIO GPIOH
#define SDP810_I2C_Scl_ClockCmd RCC_AHB1Periph_GPIOH
#define SDP810_I2C_Scl_PIN GPIO_Pin_7
#endif

#define SDP810_I2C_Scl_SET GPIO_SetBits(SDP810_I2C_Scl_GPIO,SDP810_I2C_Scl_PIN)
#define SDP810_I2C_Scl_CLR GPIO_ResetBits(SDP810_I2C_Scl_GPIO,SDP810_I2C_Scl_PIN)
#define SDP810_I2C_Scl_READ GPIO_ReadInputDataBit(SDP810_I2C_Scl_GPIO,SDP810_I2C_Scl_PIN)
#define SDP810_I2C_Sda_SET GPIO_SetBits(SDP810_I2C_Sda_GPIO,SDP810_I2C_Sda_PIN)
#define SDP810_I2C_Sda_CLR GPIO_ResetBits(SDP810_I2C_Sda_GPIO,SDP810_I2C_Sda_PIN)
#define SDP810_I2C_Sda_READ GPIO_ReadInputDataBit(SDP810_I2C_Sda_GPIO,SDP810_I2C_Sda_PIN)

// I2C IO-Pins                         /* -- adapt the defines for your uC -- */
// SDA on port B, bit 14
#define SDA_LOW()	SDP810_I2C_Sda_CLR // set SDA to low 
#define SDA_OPEN() SDP810_I2C_Sda_SET // set SDA to open-drain 
#define SDA_READ	SDP810_I2C_Sda_READ	// read SDA

// SCL on port B, bit 13	/* -- adapt the defines for your uC -- */
#define SCL_LOW()	SDP810_I2C_Scl_CLR // set SCL to low
#define SCL_OPEN() SDP810_I2C_Scl_SET // set SCL to open-drain 
#define SCL_READ	SDP810_I2C_Scl_READ	// read SCL



//-----------------------------------------------------------------------------
void I2c_Init(void)                      /* -- adapt the init for your uC -- */
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(SDP810_I2C_Sda_ClockCmd | SDP810_I2C_Scl_ClockCmd, ENABLE);

    GPIO_InitStructure.GPIO_Pin = SDP810_I2C_Sda_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SDP810_I2C_Sda_GPIO, &GPIO_InitStructure);

    SDA_OPEN();

    GPIO_InitStructure.GPIO_Pin = SDP810_I2C_Scl_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SDP810_I2C_Scl_GPIO, &GPIO_InitStructure);

    SCL_OPEN();
}

//-----------------------------------------------------------------------------
void I2c_StartCondition(void)
{
    SDA_OPEN();
    delay_us(1);
    SCL_OPEN();
    delay_us(1);
    SDA_LOW();
    delay_us(10);  // hold time start condition (t_HD;STA)
    SCL_LOW();
    delay_us(10);
}

//-----------------------------------------------------------------------------
void I2c_StopCondition(void)
{
    SCL_LOW();
    delay_us(1);
    SDA_LOW();
    delay_us(1);
    SCL_OPEN();
    delay_us(10);  // set-up time stop condition (t_SU;STO)
    SDA_OPEN();
    delay_us(10);
}

//-----------------------------------------------------------------------------
Error I2c_WriteByte( uint8_t txByte)
{
    uint8_t mask;
    Error error = ERROR_NONE;

    for (mask = 0x80; mask > 0; mask >>= 1) { // shift bit for masking (8 times)
        if ((mask & txByte) == 0) SDA_LOW();   // masking txByte & write to SDA-Line
        else                     SDA_OPEN();

        delay_us(1);                  // data set-up time (t_SU;DAT)
        SCL_OPEN();                            // generate clock pulse on SCL
        delay_us(5);                  // SCL high time (t_HIGH)
        SCL_LOW();
        delay_us(1);                  // data hold time(t_HD;DAT)
    }

    SDA_OPEN();                              // release SDA-line
    SCL_OPEN();                              // clk #9 for ack
    delay_us(1);                    // data set-up time (t_SU;DAT)

    if (SDA_READ) error = ERROR_ACK;         // check ack from i2c slave

    SCL_LOW();
    delay_us(20);                   // wait to see byte package on scope
    return error;                             // return error code
}

//-----------------------------------------------------------------------------
uint8_t I2c_ReadByte(etI2cAck ack)
{
    uint8_t mask, ucErrTime = 0;
    uint8_t rxByte = ERROR_NONE;
    SDA_OPEN();                              // release SDA-line

    for (mask = 0x80; mask > 0; mask >>= 1) { // shift bit for masking (8 times)
        SCL_OPEN();                            // generate clock pulse on SCL
        delay_us(1);                  // data set-up time (t_SU;DAT)

        while (SCL_READ == 0) {
            ucErrTime++;

            if (ucErrTime > 250) {
                I2c_StopCondition();
                ucErrTime = 0;
                return 1;
            }               // wait while hold master
        }

        delay_us(3);                  // SCL high time (t_HIGH)

        if (SDA_READ) rxByte = rxByte | mask;  // read bit1001000

        SCL_LOW();
        delay_us(1);                  // data hold time(t_HD;DAT)
    }

    if (ack == ACK) SDA_LOW();               // send acknowledge if necessary
    else           SDA_OPEN();

    delay_us(1);                    // data set-up time (t_SU;DAT)
    SCL_OPEN();                              // clk #9 for ack
    delay_us(5);                    // SCL high time (t_HIGH)
    SCL_LOW();
    SDA_OPEN();                              // release SDA-line
    delay_us(20);                   // wait to see byte package on scope
    return rxByte;                           // return error code
}
