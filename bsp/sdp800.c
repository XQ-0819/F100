//=============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//=============================================================================
/// \file    sdp800.c (V1.0)
/// \author  RFU
/// \date    24-Jan-2016
/// \brief   Sensor Layer: Implementation of functions for sensor access.
//=============================================================================

#include "sdp800.h"
#include "stdbool.h"
#include "common.h"
#include "bsp.h"
#include "ucos_ii.h"


#define SDP810_MAX 100
#define SDP810_MIN -100

#define VAC_TYPE_SDP800	(0)
#define VAC_TYPE_D6F	(1)

static uint8_t vac_type = 0;

/*D6F函数*/
Error Pre_ExecuteCommand(void);
Error D6F_SoftReset(void);
Error D6F_Reset(void);
Error Read_Pressure(float* diffPressure);

// Sensor Commands
typedef enum {
    /// Undefined dummy command.
    COMMAND_UNDEFINED                       = 0x0000,
    /// Start continous measurement                     \n
    /// Temperature compensation: Mass flow             \n
    /// Averaging: Average till read
    COMMAND_START_MEASURMENT_MF_AVERAGE     = 0x3603,
    /// Start continous measurement                     \n
    /// Temperature compensation: Mass flow             \n
    /// Averaging: None - Update rate 1ms
    COMMAND_START_MEASURMENT_MF_NONE        = 0x3608,
    /// Start continous measurement                     \n
    /// Temperature compensation: Differential pressure \n
    /// Averaging: Average till read
    COMMAND_START_MEASURMENT_DP_AVERAGE     = 0x3615,
    /// Start continous measurement                     \n
    /// Temperature compensation: Differential pressure \n
    /// Averaging: None - Update rate 1ms
    COMMAND_START_MEASURMENT_DP_NONE        = 0x361E,
    // Stop continuous measurement.
    COMMAND_STOP_CONTINOUS_MEASUREMENT      = 0x3FF9
} Command;

static Error ExecuteCommand(Command cmd);
static Error ReadMeasurementRawResults(int16_t* diffPressureTicks,
                                       int16_t* temperatureTicks,
                                       uint16_t* scaleFactor);
static Error ReadWordWithCrcCheck(uint16_t *value, bool ack);
static Error CheckCrc(const uint8_t data[], uint8_t size, uint8_t checksum);

static const float scaleFactorTemperature = 200;

static uint8_t _i2cAddress;

//-----------------------------------------------------------------------------
void Sdp800_Init(uint8_t i2cAddress)
{
#if USE_GD == 1

#else		
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif

    _i2cAddress = i2cAddress;
    I2c_Init();
    Sdp800_Reset();
}

//-----------------------------------------------------------------------------
Error Sdp800_StartContinousMeasurement(Sdp800TempComp  tempComp,
                                       Sdp800Averaging averaging)
{
    Error error;
    Command command = COMMAND_UNDEFINED;

    // determine command code
    switch (tempComp) {
    case SDP800_TEMPCOMP_MASS_FLOW:
        switch (averaging) {
        case SDP800_AVERAGING_TILL_READ:
            command = COMMAND_START_MEASURMENT_MF_AVERAGE;
            break;

        case SDP800_AVERAGING_NONE:
            command = COMMAND_START_MEASURMENT_MF_NONE;
            break;
        }

        break;

    case SDP800_TEMPCOMP_DIFFERNTIAL_PRESSURE:
        switch (averaging) {
        case SDP800_AVERAGING_TILL_READ:
            command = COMMAND_START_MEASURMENT_DP_AVERAGE;
            break;

        case SDP800_AVERAGING_NONE:
            command = COMMAND_START_MEASURMENT_DP_NONE;
            break;
        }

        break;
    }

    if (COMMAND_UNDEFINED != command) {
        error = ExecuteCommand(command);
        // wait 10 ms for startup
        delay_us(10000);
    } else {
        error = ERROR_IVALID_PARAMETER;
    }

    return error;
}

//-----------------------------------------------------------------------------
Error Sdp800_StopContinousMeasurement(void)
{
    return ExecuteCommand(COMMAND_STOP_CONTINOUS_MEASUREMENT);
}

//-----------------------------------------------------------------------------
Error Sdp800_ReadMeasurementResults(float* diffPressure, float* temperature)
{
    Error error;
    int16_t  diffPressureTicks;
    int16_t  temperatureTicks;
    uint16_t scaleFactorDiffPressure;

    error = ReadMeasurementRawResults(&diffPressureTicks, &temperatureTicks,
                                      &scaleFactorDiffPressure);

    if (ERROR_NONE == error) {
        *diffPressure = (float)diffPressureTicks / (float)scaleFactorDiffPressure;
        *temperature  = (float)temperatureTicks / scaleFactorTemperature;
    }

    return error;
}

// add hardreset
//-----------------------------------------------------------------------------
Error Sdp800_SoftReset(void)
{
    Error error;

    // write a start condition
    I2c_StartCondition();

    // write the upper 8 bits of reset
    error = I2c_WriteByte(0x00);

    // write the lower 8 bits of reset
    if (ERROR_NONE == error) {
        error = I2c_WriteByte(0x06);
    }

    I2c_StopCondition();

    // wait 20 ms
    OSTimeDly(20);

    return error;
}

//-----------------------------------------------------------------------------
static Error ReadMeasurementRawResults(int16_t*  diffPressureTicks,
                                       int16_t*  temperatureTicks,
                                       uint16_t* scaleFactor)
{
    Error error;

    // write a start condition
    I2c_StartCondition();

    // write the sensor I2C address with the read flag
    error = I2c_WriteByte(_i2cAddress << 1 | 0x01);

    // reas differnetial pressure ticks
    if (ERROR_NONE == error) {
        error = ReadWordWithCrcCheck((uint16_t*)diffPressureTicks, true);
    }

    // read temperature pressure ticks
    if (ERROR_NONE == error) {
        error = ReadWordWithCrcCheck((uint16_t*)temperatureTicks, true);
    }

    // read scale factor
    if (ERROR_NONE == error) {
        error = ReadWordWithCrcCheck(scaleFactor, false);
    }

    I2c_StopCondition();

    return error;
}

//-----------------------------------------------------------------------------
Error ExecuteCommand(Command cmd)
{
    Error error;

    // write a start condition
    I2c_StartCondition();

    // write the sensor I2C address with the write flag
    error = I2c_WriteByte(_i2cAddress << 1 | 0x00);

    // write the upper 8 bits of the command
    if (ERROR_NONE == error) {
        error = I2c_WriteByte(cmd >> 8);
    }

    // write the lower 8 bits of the command to the sensor
    if (ERROR_NONE == error) {
        error = I2c_WriteByte(cmd & 0xFF);
    }

    I2c_StopCondition();

    return error;
}

//-----------------------------------------------------------------------------
static Error ReadWordWithCrcCheck(uint16_t *value, bool ack)
{
    Error error;
    uint8_t bytes[2];
    uint8_t checksum;

    // read two data bytes and one checksum byte
    bytes[0] = I2c_ReadByte(ACK);
    bytes[1] = I2c_ReadByte(ACK);
    checksum = I2c_ReadByte(ack ? ACK : NACK);

    // verify checksum
    error = CheckCrc(bytes, 2, checksum);

    // combine the two bytes to a 16-bit value
    *value = (bytes[0] << 8) | bytes[1];

    return error;
}

//-----------------------------------------------------------------------------
static Error CheckCrc(const uint8_t data[], uint8_t size, uint8_t checksum)
{
    uint8_t crc = 0xFF, i = 0, bit = 0;

    // calculates 8-Bit checksum with given polynomial 0x31 (x^8 + x^5 + x^4 + 1)
    for (i = 0; i < size; i++) {
        crc ^= (data[i]);

        for ( bit = 8; bit > 0; --bit) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else           crc = (crc << 1);
        }
    }

    // verify checksum
    return (crc == checksum) ? ERROR_NONE : ERROR_CHECKSUM;
}

#define AD_TIME 5
static unsigned char read_num = 0;
short sdp800_value[AD_TIME] = {0};
Error Sdp800_Reset(void)
{
    Error error;
    int i = 0;
	
#if USE_GD == 1

#else		
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
    OSTimeDly(1000);
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
    OSTimeDly(500);
#endif

    for (i = 0; i < AD_TIME; i++) {
        sdp800_value[i] = VAC_ER_VALUE;
    }

    if (vac_type == VAC_TYPE_SDP800) {
        // reset the sensor
        Sdp800_SoftReset();

        // start continous measurement
        error = Sdp800_StartContinousMeasurement(SDP800_TEMPCOMP_MASS_FLOW,
                SDP800_AVERAGING_TILL_READ);
    } else {
        // reset the sensor
        D6F_SoftReset();

        // start continous measurement
        Pre_ExecuteCommand();
    }

    return error;
}

void sdp800_proc(void)
{
    Error error;
    float diffPressure;
    float temperature;

    if (vac_type == VAC_TYPE_SDP800) {
        // read measurement results
        error = Sdp800_ReadMeasurementResults(&diffPressure, &temperature);
    } else {
        D6F_Reset();
        error = Read_Pressure(&diffPressure);
    }

    if (error != ERROR_NONE) {
        sdp800_value[read_num] = VAC_ER_VALUE;
    } else {
        sdp800_value[read_num] = diffPressure;
    }

    read_num++;

    if (read_num >= AD_TIME) {
        read_num = 0;
    }

}

short filter_ftbuffer[AD_TIME];
short Aver_Filter_short( short *buffer, unsigned char num )
{
    INT8U i, j;
    short temp_int , data_num = 0;

    for (i = 0; i < num; i++) {
        filter_ftbuffer[i] = buffer[i];
    }

    //中位值平均滤波法
    //排序
    for (j = 0; j < num - 1; j++) {
        for (i = 0; i < num - j - 1; i++) {
            if ( filter_ftbuffer[i] > filter_ftbuffer[i + 1] ) {
                temp_int = filter_ftbuffer[i];
                filter_ftbuffer[i] = filter_ftbuffer[i + 1];
                filter_ftbuffer[i + 1] = temp_int;
            }
        }
    }

    //去掉最大和最小值后求均值
    temp_int = 0;

    for (i = 1; i < num - 1; i++) {
        if (filter_ftbuffer[i] == VAC_ER_VALUE) {
            continue;
        }

        data_num ++;
        temp_int += filter_ftbuffer[i];
    }

    if (data_num == 0)
        return VAC_ER_VALUE;

    return (temp_int / (data_num));
}

static int error_time = 0;

int get_sdp800_value(void)
{
    int value = 0;

    value = Aver_Filter_short(sdp800_value, AD_TIME);

    if (value == VAC_ER_VALUE) {
        if (error_time > 6) {//6次没成功就先不重启了
            error_time++;

            if (error_time > 180) {//about 1min
                error_time = 0;
            }

            return value;
        }

        if (vac_type == VAC_TYPE_SDP800) {
            vac_type = VAC_TYPE_D6F;
            Sdp800_Init(0x6C);//E6F
        } else {
            vac_type = VAC_TYPE_SDP800;
            Sdp800_Init(0x25); // sdp800
        }

        Sdp800_Reset();
        error_time++;

        return value;
    }

    error_time = 0;

    if (value >= SDP810_MAX) {
        value = SDP810_MAX;
    } else if (value <= SDP810_MIN) {
        value = SDP810_MIN;
    }

    return value;
}


/***********************************************************************************************************************
***********************************************************************************************************************
************************************以下是针对E6F静压传感器的驱动程序*************************************************
***********************************************************************************************************************
************************************************************************************************************************/

/*****************************************************************************
 函 数 名  : Pre_ExecuteCommand
 功能描述  :
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2021年7月1日
    作    者   : 董玉华
    修改内容   :

*****************************************************************************/
Error Pre_ExecuteCommand(void)
{
    Error error;

    // write a start condition
    I2c_StartCondition();

    // write the sensor I2C address with the write flag
    error = I2c_WriteByte(_i2cAddress << 1 | 0x00);

    error = I2c_WriteByte(START_ADDRESS);

    // write the upper 8 bits of the command
    if (ERROR_NONE == error) {
        error = I2c_WriteByte(SENS_CTRL >> 8);
    }

    // write the lower 8 bits of the command to the sensor
    if (ERROR_NONE == error) {
        error = I2c_WriteByte(SENS_CTRL & 0xFF);
    }

    error = I2c_WriteByte(0x18);
    error = I2c_WriteByte(SENS_CTRL_VAL);

    I2c_StopCondition();

    return error;
}


/*****************************************************************************
 函 数 名  : D6F_SoftReset
 功能描述  : 加载NVM ，上电200us之后必须进行的初始化
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2021年7月1日
    作    者   : 董玉华
    修改内容   :

*****************************************************************************/
Error D6F_SoftReset(void)
{
    Error error;

    // write a start condition
    I2c_StartCondition();

    error = I2c_WriteByte(_i2cAddress << 1 | 0x00);

    // write the upper 8 bits of reset
    error = I2c_WriteByte(CTRL_REG);
    // write the upper 8 bits of reset
    error = I2c_WriteByte(0x00);

    I2c_StopCondition();

    return error;
}


/*****************************************************************************
 函 数 名  : D6F_Reset
 功能描述  :
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2021年7月1日
    作    者   : 董玉华
    修改内容   :

*****************************************************************************/
Error D6F_Reset(void)
{
    Error error;

    // reset the sensor
    D6F_SoftReset();

    // start continous measurement
    Pre_ExecuteCommand();
    // wait 10 ms
    OSTimeDly(40);
    return error;
}


/*****************************************************************************
 函 数 名  : Read_Pressure
 功能描述  :
 输入参数  :
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2021年7月1日
    作    者   : 董玉华
    修改内容   :

*****************************************************************************/
Error Read_Pressure(float* diffPressure)
{
    Error error;
    uint8_t pressbytes[2];
    unsigned short get_value = 0;

    // write a start condition
    I2c_StartCondition();

    // write the sensor I2C address with the write flag
    error = I2c_WriteByte(_i2cAddress << 1 | 0x00);

    error = I2c_WriteByte(START_ADDRESS);

    // write the upper 8 bits of the command
    if (ERROR_NONE == error) {
        error = I2c_WriteByte(COMP_DATA1_H >> 8);
    }

    // write the lower 8 bits of the command to the sensor
    if (ERROR_NONE == error) {
        error = I2c_WriteByte(COMP_DATA1_H & 0xFF);
    }

    error = I2c_WriteByte(SERIAL_CTRL_VAL);
    I2c_StopCondition();

    I2c_StartCondition();
    // write the sensor I2C address with the write flag
    error = I2c_WriteByte(_i2cAddress << 1 | 0x00);
    error = I2c_WriteByte(BUFFER_0);

    I2c_StartCondition();
    // read the sensor I2C address with the read flag
    error = I2c_WriteByte(_i2cAddress << 1 | 0x01);

    pressbytes[0] = I2c_ReadByte(ACK);
    pressbytes[1] = I2c_ReadByte(ACK);
    get_value = (unsigned short)(pressbytes[0] << 8) + pressbytes[1];
    *diffPressure = (((int)((get_value - 1024) * 1000) / 60000) - 500);

    I2c_StopCondition();
    return error;
}


