//=============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//=============================================================================
/// \file    sdp800.h (V1.0)
/// \author  RFU
/// \date    24-Jan-2016
/// \brief   Sensor Layer: Definitions of commands and functions for sensor
///                        access.
//=============================================================================

#ifndef SDP800_H
#define SDP800_H

#include "stm32f4xx.h"
#include "i2c_hal.h"


/*******D6F¾²Ñ¹²ÎÊý*******************************************************************************************/
#define D6FPH_ADDRESS   0x6C
#define SENS_CTRL       0xD040
#define COMP_DATA1_H    0xD051
#define TMP_H           0xD061
#define BUFFER_0        0x07
#define CTRL_REG        0x0B
#define START_ADDRESS   0x00

#define SENS_CTRL_MS        2 
#define SENS_CTRL_DV_PWR    1
#define SENS_CTRL_VAL       (0x01 << SENS_CTRL_MS | 0x01 << SENS_CTRL_DV_PWR)

#define SERIAL_CTRL_D_BYTE_CNT3     5
#define SERIAL_CTRL_REQ             3
#define SERIAL_CTRL_R_WZ            2
#define SERIAL_CTRL_VAL   (0x01 << SERIAL_CTRL_R_WZ | 0x01 << SERIAL_CTRL_REQ | 0x01 << SERIAL_CTRL_D_BYTE_CNT3)
/****************************************************************************************************************/


//-----------------------------------------------------------------------------
/// \brief Enumeration to configure the temperature compensation for
///        measurement.
typedef enum {
  SDP800_TEMPCOMP_MASS_FLOW,
  SDP800_TEMPCOMP_DIFFERNTIAL_PRESSURE
} Sdp800TempComp;

//-----------------------------------------------------------------------------
/// \brief Enumeration to configure the averaging for measurement.
typedef enum {
  SDP800_AVERAGING_NONE,
  SDP800_AVERAGING_TILL_READ
} Sdp800Averaging;

//-----------------------------------------------------------------------------
/// \brief  Initializes the I2C bus for communication with the sensor.
/// \param  i2cAddress  Sensors I2C address.
void Sdp800_Init(uint8_t i2cAddress);

//-----------------------------------------------------------------------------
/// \brief  Starts the continous mesurement with the specified settings.
/// \param  tempComp  Temperature compensation: Mass flow or diff. pressure.
/// \param  averaging Averaging: None or average till read.
/// \return ERROR_NONE              = No error.
///         ERROR_ACK               = No acknowledgment from sensor.
///         ERROR_INVALID_PARAMETER = At least one of the specified parameter
///                                   are invalid.
Error Sdp800_StartContinousMeasurement(Sdp800TempComp  tempComp,
                                       Sdp800Averaging averaging);

//-----------------------------------------------------------------------------
/// \brief  Stops the continous mesurement.
/// \return ERROR_NONE              = No error.
///         ERROR_ACK               = No acknowledgment from sensor.
Error Sdp800_StopContinousMeasurement(void);

//-----------------------------------------------------------------------------
/// \brief  Reads the measurment result from the continous measurment.
/// \param  diffPressure Pointer to return the measured diverential pressur.
///         temperature  Pointer to return the measured temperature.
/// \return ERROR_NONE              = No error.
///         ERROR_ACK               = No acknowledgment from sensor.
///         ERROR_CHECKSUM          = Checksum does not match.
Error Sdp800_ReadMeasurementResults(float* diffPressure, float* temperature);

//-----------------------------------------------------------------------------
/// \brief  Calls the soft reset mechanism that forces the sensor into a
///         well-defined state without removing the power supply.
/// \return ERROR_NONE              = No error.
///         ERROR_ACK               = No acknowledgment from sensor.
Error Sdp800_SoftReset(void);
Error Sdp800_Reset(void);
void sdp800_proc(void);
int get_sdp800_value(void);
#endif /* SDP800_H */
