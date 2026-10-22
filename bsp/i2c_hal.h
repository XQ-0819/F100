//=============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//=============================================================================
/// \file    i2c_hal.h (V1.0)
/// \author  RFU
/// \date    24-Jan-2016
/// \brief   I2C hardware abstraction layer 
//=============================================================================

#ifndef I2C_HAL_H
#define I2C_HAL_H

#include "stm32f4xx.h"


/// Enumearation for I2C acknowledge.
typedef enum{
  ACK    = 0,
  NACK   = 1,
}etI2cAck;


/// \brief Error codes
typedef enum {
  ERROR_NONE              = 0x00, // no error
  ERROR_ACK               = 0x01, // no acknowledgment error
  ERROR_CHECKSUM          = 0x02, // checksum mismatch error
  ERROR_IVALID_PARAMETER  = 0xFF, // invalid parameter
} Error;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// \brief  Initializes the ports for I2C interface.
void I2c_Init(void);

//-----------------------------------------------------------------------------
/// \brief  Writes a start condition on I2C-Bus.
///                _____                         \n
///         SDA:        |_____                   \n
//                 _______                       \n
//          SCL:          |___
/// \remark Timing (delay) may have to be changed for different microcontroller.
void I2c_StartCondition(void);

//-----------------------------------------------------------------------------
/// \brief  Writes a stop condition on I2C-Bus.  \n
///                      _____                   \n
///         SDA:   _____|                        \n
//                     _______                   \n
//          SCL:   ___|
/// \remark Timing (delay) may have to be changed for different microcontroller.
void I2c_StopCondition(void);

//-----------------------------------------------------------------------------
/// \brief  Writes a byte to I2C-Bus and checks acknowledge.
/// \param  txByte  Transmit byte
/// \return ERROR_ACK  = no acknowledgment from sensor
//          ERROR_NONE = no error
/// \remark Timing (delay) may have to be changed for different microcontroller.
Error I2c_WriteByte(uint8_t txByte);

//-----------------------------------------------------------------------------
/// \brief  Reads a byte on I2C-Bus.
/// \param  ack  Acknowledge: ACK or NO_ACK
/// \return Received byte.
/// \remark Timing (delay) may have to be changed for different microcontroller.
uint8_t I2c_ReadByte(etI2cAck ack);

#endif
