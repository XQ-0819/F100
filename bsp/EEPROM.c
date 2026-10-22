/******************************************************************************

  文 件 名   : EEPROM.c
  版 本 号   : 初稿
  作    者   : 邹稳
  生成日期   : 2014年9月20日星期六
  最近修改   :
  功能描述   : EEPROM驱动文件，IIC接口，支持24c01-256驱动 for STM32F4
  函数列表   :
              delay_iic_us
              EEPROM_Config------API
              EEPROM_Read -------API
              EEPROM_Write	-----API
              _IIC_Check
              _IIC_End
              _IIC_M_Check
              _IIC_N_Check
              IIC_ReadBuffer
              _IIC_ReadByte
              IIC_ReadPage
              _IIC_Start
              IIC_WriteBuffer
              _IIC_WriteByte
              IIC_WritePage
  修改历史   :
  1.日    期   : 2014年9月20日星期六
    作    者   : 邹稳
    修改内容   : 创建文件，在开漏输出模式下，输出为高的情况下可以输入的值，不需要更改输入输出模式
    				注意地址与写保护管脚电平

******************************************************************************/


/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/



/*****************************************************************************/
#include "EEPROM.h"
#include "ucos_ii.h"
#include "app.h"

#define EEPEOM_WRITE_MAX 4096

#if USE_GD == 1

#define EEPROM_SCK_CLK1 RCC_AHB1Periph_GPIOG
#define EEPROM_SDA_CLK1 RCC_AHB1Periph_GPIOD

#define EEPROM_SCK_PORT1 GPIOG
#define EEPROM_SDA_PORT1 GPIOD

#define EEPROM_SCK_PIN1 GPIO_Pin_13
#define EEPROM_SDA_PIN1 GPIO_Pin_7

#else

#define EEPROM_SCK_CLK1 RCC_AHB1Periph_GPIOH
#define EEPROM_SDA_CLK1 RCC_AHB1Periph_GPIOH

#define EEPROM_SCK_PORT1 GPIOH
#define EEPROM_SDA_PORT1 GPIOH

#define EEPROM_SCK_PIN1 GPIO_Pin_5
#define EEPROM_SDA_PIN1 GPIO_Pin_4


/*2.9硬件版本*/
#define EEPROM_SCK_CLK2 RCC_AHB1Periph_GPIOG
#define EEPROM_SDA_CLK2 RCC_AHB1Periph_GPIOC

#define EEPROM_SCK_PORT2 GPIOG
#define EEPROM_SDA_PORT2 GPIOC

#define EEPROM_SCK_PIN2 GPIO_Pin_3
#define EEPROM_SDA_PIN2 GPIO_Pin_5
#endif


static u32 EEPROM_SCK_CLK = EEPROM_SCK_CLK1;
static u32 EEPROM_SDA_CLK = EEPROM_SDA_CLK1;
static GPIO_TypeDef *EEPROM_SCK_PORT = EEPROM_SCK_PORT1;
static GPIO_TypeDef *EEPROM_SDA_PORT = EEPROM_SDA_PORT1;
static u16 EEPROM_SCK_PIN = EEPROM_SCK_PIN1;
static u16 EEPROM_SDA_PIN = EEPROM_SDA_PIN1;


#define SDA_Set GPIO_SetBits(EEPROM_SDA_PORT,EEPROM_SDA_PIN)
#define SDA_Clr GPIO_ResetBits(EEPROM_SDA_PORT,EEPROM_SDA_PIN)
#define SCK_Set GPIO_SetBits(EEPROM_SCK_PORT,EEPROM_SCK_PIN)
#define SCK_Clr GPIO_ResetBits(EEPROM_SCK_PORT,EEPROM_SCK_PIN)


//--32/64 1010 A2A1A0 R/W
#define WriteAddress_EEPROM    0xA0
#define ReadAddress_EEPROM     0xA1

/*****24Cxx Seriel EEPROM*************************/
#define EEPROM 256
/********
01 -> 24C01;   02 -> 24C02;  04 -> 24C04;   08 -> 24C08;
16 -> 24C16;   32 -> 24C32;  64 -> 24C64;   128 -> 24C128;
256-> 24C256;  512 -> 24C512;
*/

#if EEPROM==1
#define PAGE_SIZE           8
#define EE_SIZE             0x0080
#elif EEPROM==2
#define PAGE_SIZE           16
#define EE_SIZE             0x0100
#elif EEPROM==4
#define PAGE_SIZE           16
#define EE_SIZE             0x0200
#elif EEPROM==8
#define PAGE_SIZE           16
#define EE_SIZE             0x0400
#elif EEPROM==16
#define PAGE_SIZE           16
#define EE_SIZE             0x0800
#elif EEPROM==32
#define PAGE_SIZE           32
#define EE_SIZE             0x1000
#elif EEPROM==64
#define PAGE_SIZE           32
#define EE_SIZE             0x2000
#elif EEPROM==128
#define PAGE_SIZE           64
#define EE_SIZE             0x4000
#elif EEPROM==256
#define PAGE_SIZE           64
#define EE_SIZE             0x8000
#endif





OS_EVENT      *eeprom_Sem;

static   void IIC_ReadBuffer(unsigned char* pBuffer, unsigned int ReadAddress, unsigned short NumbyteToWrite);
static   void IIC_WriteBuffer(unsigned char* pBuffer, unsigned int WriteAddress, unsigned short NumByteToWrite);
static   u8 *IIC_ReadPage(u8 *, u8, u16, u8);
static   u8 *IIC_WritePage(u8 *, u8, u16, u8);

static void delay_iic_us(u16 time)
{
    u16 i = 0;

    while (time--) {
        i = 4; //自己定义

        while (i--) ;
    }
}
void EEPROM_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if (get_hardware_ver() >= HARD_V4) {
        EEPROM_SCK_CLK = EEPROM_SCK_CLK2;
        EEPROM_SDA_CLK = EEPROM_SDA_CLK2;
        EEPROM_SCK_PORT = EEPROM_SCK_PORT2;
        EEPROM_SDA_PORT = EEPROM_SDA_PORT2;
        EEPROM_SCK_PIN = EEPROM_SCK_PIN2;
        EEPROM_SDA_PIN = EEPROM_SDA_PIN2;
    } else {
        EEPROM_SCK_CLK = EEPROM_SCK_CLK1;
        EEPROM_SDA_CLK = EEPROM_SDA_CLK1;
        EEPROM_SCK_PORT = EEPROM_SCK_PORT1;
        EEPROM_SDA_PORT = EEPROM_SDA_PORT1;
        EEPROM_SCK_PIN = EEPROM_SCK_PIN1;
        EEPROM_SDA_PIN = EEPROM_SDA_PIN1;
    }

    RCC_AHB1PeriphClockCmd(EEPROM_SCK_CLK | EEPROM_SDA_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = EEPROM_SCK_PIN;
    GPIO_Init(EEPROM_SCK_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = EEPROM_SDA_PIN;
    GPIO_Init(EEPROM_SDA_PORT, &GPIO_InitStructure);

    eeprom_Sem = OSSemCreate(1);

}
void _IIC_Start(void)
{
    SDA_Set;
    delay_iic_us(4);
    SCK_Set;
    delay_iic_us(4);
    SDA_Clr;
    delay_iic_us(4);
    SCK_Clr;
    delay_iic_us(4);
}
void _IIC_End(void)
{
    SCK_Clr;
    delay_iic_us(4);
    SDA_Clr;
    delay_iic_us(4);
    SCK_Set;
    delay_iic_us(4);
    SDA_Set;
    delay_iic_us(4);
    SCK_Clr;
}
/*****************************************************************************
 函 数 名  : _IIC_Check
 功能描述  : IIC的ACK等待
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年9月20日星期六
    作    者   : 邹稳
    修改内容   : 新生成函数

*****************************************************************************/
u8 _IIC_Check(void)
{
    u8 ucErrTime = 0;
    SCK_Clr;
    delay_iic_us(4);
    SDA_Set;
    delay_iic_us(4);
    SCK_Set;
    delay_iic_us(4);

    while (GPIO_ReadInputDataBit(EEPROM_SDA_PORT, EEPROM_SDA_PIN) == 1) {
        ucErrTime++;

        if (ucErrTime > 250) {
            _IIC_End();
            ucErrTime = 0;
            return 1;
        }
    }

    delay_iic_us(4);
    SCK_Clr;
    return 0;
}
/*****************************************************************************
 函 数 名  : _IIC_M_Check
 功能描述  : 发送ACK确认
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年9月20日星期六
    作    者   : 邹稳
    修改内容   : 新生成函数

*****************************************************************************/
void _IIC_M_Check(void)
{
    SCK_Clr;
    delay_iic_us(4);
    SDA_Clr;
    delay_iic_us(4);
    SCK_Set;
    delay_iic_us(4);
    SCK_Clr;
    delay_iic_us(4);
}
/*****************************************************************************
 函 数 名  : _IIC_N_Check
 功能描述  : 发送NACK停止接收数据
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年9月20日星期六
    作    者   : 邹稳
    修改内容   : 新生成函数

*****************************************************************************/
void _IIC_N_Check(void)
{
    SCK_Clr;
    delay_iic_us(4);
    SDA_Set;
    delay_iic_us(4);
    SCK_Set;
    delay_iic_us(4);
    SCK_Clr;
    delay_iic_us(4);
}
void _IIC_WriteByte(u8 data)
{
    u8 i, temp;

    for (i = 1; i <= 8; i++) {
        temp = (data & 0x80) >> 7;
        data = data << 1;

        if (temp) {
            SCK_Clr;  //Write 1
            delay_iic_us(4);
            SDA_Set;
            delay_iic_us(4);
            SCK_Set;
            delay_iic_us(4);
            SCK_Clr;
            delay_iic_us(4);
        } else {
            SCK_Clr;  //Write 0
            delay_iic_us(4);
            SDA_Clr;
            delay_iic_us(4);
            SCK_Set;
            delay_iic_us(4);
            SCK_Clr;
            delay_iic_us(4);
        }
    }
}
u8 _IIC_ReadByte(void)
{
    u8 i, j, data;
    data = 0;
    SCK_Clr;
    delay_iic_us(4);
    SDA_Set;

    for (i = 1; i <= 8; i++) {
        SCK_Set;
        delay_iic_us(4);
        j = GPIO_ReadInputDataBit(EEPROM_SDA_PORT, EEPROM_SDA_PIN);
        data = j | (data << 1);
        SCK_Clr;
        delay_iic_us(4);
    }

    SCK_Clr;
    return data;
}
#define WRITE_TIME		(3)
unsigned char TempBuffer[EEPEOM_WRITE_MAX];
unsigned char EEPROM_Write(unsigned int WriteAddress, unsigned char* pBuffer,  unsigned short NumbyteToWrite)
{
    unsigned short i = 0, time = 0;
    unsigned char err, flag;

    if ((WriteAddress + NumbyteToWrite) > EE_SIZE || NumbyteToWrite > EEPEOM_WRITE_MAX) {
        return 0;
    } else {

        OSSemPend(eeprom_Sem, 0, &err); //等待数据返回

        for (time = 0; time < WRITE_TIME; time++) {
            IIC_WriteBuffer(pBuffer, WriteAddress, NumbyteToWrite);
//        IIC_WriteBuffer(pBuffer,WriteAddress,NumbyteToWrite);
            IIC_ReadBuffer(TempBuffer, WriteAddress, NumbyteToWrite);
            flag = 1;

            for (i = 0; i < NumbyteToWrite; i++) {
                if (TempBuffer[i] != pBuffer[i]) {
                    flag = 0;
                    break;
                }

            }

            if (flag == 1) {
                break;
            }

            OSTimeDly(20);
        }

        OSSemPost(eeprom_Sem);
        return flag;

    }
}

unsigned char EEPROM_Read(unsigned int ReadAddress, unsigned char* pBuffer,  unsigned short NumbyteToWrite)
{
    unsigned char err;

    if ((ReadAddress + NumbyteToWrite) > EE_SIZE) {
        return 0;
    } else {
        OSSemPend(eeprom_Sem, 0, &err); //等待数据返回
        IIC_ReadBuffer(pBuffer, ReadAddress, NumbyteToWrite);
        OSSemPost(eeprom_Sem);

//        IIC_ReadBuffer(pBuffer,ReadAddress,NumbyteToWrite);
        return 1;
    }
}


/**
***************************************************
 *@Function     :IIC_ReadBuffer()
 *@Return_Value :Null
 *@Peremater    :pBuffer:
                 ReadAddr:
                 NumByteToWrite:
 *@Brief        :
***************************************************/
void IIC_ReadBuffer(unsigned char* pBuffer, unsigned int ReadAddress, unsigned short NumByteToWrite)
{
    u8 NumOfPage = 0, NumOfSingle = 0; //,count=0
    u16 Part = 0; //
    u8 PageAddress = 0;
    /******pageAddress is over 8bit***********/
#if EEPROM < 32
    PageAddress = (u8)(ReadAddress >> 7) & 0x0E | ReadAddress_EEPROM;
#else
    PageAddress = ReadAddress_EEPROM;
#endif
    /*******判断起始地址是否跨页片区******/
    Part = ReadAddress / PAGE_SIZE;

    if (Part != 0) {
        Part = PAGE_SIZE * (Part + 1) - ReadAddress;
    } else {
        Part = PAGE_SIZE - ReadAddress;
    }

    /******/
    if (Part >= NumByteToWrite) {
        IIC_ReadPage(pBuffer, PageAddress, ReadAddress, NumByteToWrite);
    } else {
        NumOfPage = (NumByteToWrite - Part) / PAGE_SIZE;
        NumOfSingle = (NumByteToWrite - Part) % PAGE_SIZE;
        IIC_ReadPage(pBuffer, PageAddress, ReadAddress, Part);

        pBuffer += Part;
        ReadAddress += Part;

        while (NumOfPage--) {
            IIC_ReadPage(pBuffer, PageAddress, ReadAddress, PAGE_SIZE);

            ReadAddress += PAGE_SIZE;
            pBuffer += PAGE_SIZE;
        }

        if (NumOfSingle != 0) {
            IIC_ReadPage(pBuffer, PageAddress, ReadAddress, NumOfSingle);

        }
    }
}



/**
***************************************************
 *@Function     :IIC_WriteBuffer()
 *@Return_Value :Null
 *@Peremater    :pBuffer:
                 WriteAddr:
                 NumByteToWrite:
 *@Brief        :
***************************************************/
void IIC_WriteBuffer(unsigned char* pBuffer, unsigned int WriteAddress, unsigned short NumByteToWrite)
{
    u8 NumOfPage = 0, NumOfSingle = 0; //,count=0
    u16 Part = 0; //
    u8 PageAddress = 0;
    /******pageAddress is over 8bit***********/
#if EEPROM < 32
    PageAddress = (u8)(WriteAddress >> 7) & 0x0E | WriteAddress_EEPROM;
#else
    PageAddress = WriteAddress_EEPROM;
#endif



    /*******判断起始地址是否跨页片区******/
    Part = WriteAddress / PAGE_SIZE;

    if (Part != 0) {
        Part = PAGE_SIZE * (Part + 1) - WriteAddress;
    } else {
        Part = PAGE_SIZE - WriteAddress;
    }

    /******/
    if (Part >= NumByteToWrite) {
        IIC_WritePage(pBuffer, PageAddress, WriteAddress, NumByteToWrite);
    } else {
        NumOfPage = (NumByteToWrite - Part) / PAGE_SIZE;
        NumOfSingle = (NumByteToWrite - Part) % PAGE_SIZE;
        IIC_WritePage(pBuffer, PageAddress, WriteAddress, Part);

        WriteAddress += Part;
        pBuffer += Part;

        while (NumOfPage--) {
            IIC_WritePage(pBuffer, PageAddress, WriteAddress, PAGE_SIZE);

            WriteAddress += PAGE_SIZE;
            pBuffer += PAGE_SIZE;
        }

        if (NumOfSingle != 0) {
            IIC_WritePage(pBuffer, PageAddress, WriteAddress, NumOfSingle);

        }
    }
}


/**
***************************************************
 *@Function     :IIC_WritePage(u8 *pBuffer,u8 PartAddr,u8 WriteAddr,u8 NumByteToWrite)
 *@Return_Value :Null
 *@Peremater    :pBuffer:
                 PartAddr:
                 WriteAddr:
                 NumByteToWrite:
 *@Brief        :
***************************************************/
u8* IIC_WritePage(u8 *pBuffer, u8 PartAddr, u16 WriteAddr, u8 NumByteToWrite)
{
    u16 i = 0;

    do {
        _IIC_Start();		//启动
        _IIC_WriteByte(PartAddr);  //控制字 写
    } while (_IIC_Check() == 1);	 //未确认

#if EEPROM < 32
    _IIC_WriteByte((u8)WriteAddr);
    _IIC_Check();
#else
    _IIC_WriteByte((u8)(WriteAddr >> 8));
    _IIC_Check();
    _IIC_WriteByte((u8)WriteAddr);
    _IIC_Check();
#endif

//	_IIC_Check();
    for (i = 0; i < NumByteToWrite; i++) {
        _IIC_WriteByte(pBuffer[i]);
        _IIC_Check();
    }

    _IIC_End();
    return pBuffer;
}





/**
*************************************************
 *@Function     : IIC_ReadPage(u8 *pBuffer,u8 PartAddr,u8 WriteAddr,u8 NumByteToWrite)
 *@Return_Value :Null
 *@Peremater    :
 *@Brief        :
***************************************************/
u8* IIC_ReadPage(u8 *ReadBuffer, u8 PartAddr, u16 ReadAddr, u8 NumByteToWrite)
{
    u16 i;

    do {
        _IIC_Start();		//启动
        _IIC_WriteByte(PartAddr & 0xfe); //控制字 写(把地址最后一位置0)
    } while (_IIC_Check() == 1);	 //未确认

#if EEPROM < 32
    _IIC_WriteByte((u8)ReadAddr);
    _IIC_Check();
#else
    _IIC_WriteByte((u8)(ReadAddr >> 8));
    _IIC_Check();
    _IIC_WriteByte((u8)ReadAddr);
    _IIC_Check();
#endif

    do {
        _IIC_Start();		//启动
        _IIC_WriteByte(PartAddr);  //控制字 读
    } while (_IIC_Check() == 1);

    for (i = 0; i < NumByteToWrite; i++) {
        ReadBuffer[i] = _IIC_ReadByte();

        if (i < (NumByteToWrite - 1))
            _IIC_M_Check();
    }

    _IIC_N_Check();
    _IIC_End();
    return ReadBuffer;
}

