/******************************************************************************

  文 件 名   : W25Q16.c
  版 本 号   : 初稿
  作    者   : 邹稳
  生成日期   : 2013年5月20日星期一
  最近修改   :
  功能描述   : W25Q16 FLASH 驱动
  函数列表   :
              Flash_SPIx_ReadWriteByte
              SPI_Flash_Erase_Chip
              SPI_Flash_Erase_Sector
              SPI_Flash_Init
              SPI_Flash_PowerDown
              SPI_Flash_Read
              SPI_Flash_ReadID
              SPI_Flash_ReadSR
              SPI_Flash_Wait_Busy
              SPI_Flash_WAKEUP
              SPI_Flash_Write
              SPI_FLASH_Write_Disable
              SPI_FLASH_Write_Enable
              SPI_Flash_Write_NoCheck
              SPI_Flash_Write_Page
              SPI_FLASH_Write_SR
  修改历史   :
  1.日    期   : 2013年5月20日星期一
    作    者   : 邹稳
    修改内容   : 创建文件

  2.日    期   : 2016年11月22日
    作    者   : 邹稳
    修改内容   : 修改支持W25Q256


  3.日    期   : 2016年11月23日
    作    者   : 邹稳
    修改内容   : 修改支持F4

		//3位地址线模式下超过16M的数据又重新从0开始输出
  4.日    期   : 2020-03-30
    作    者   : 邹稳
    修改内容   : 增加支持W25Q128
******************************************************************************/


/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/


/*****************************************************************************/
#include "W25QXX.h"
#include "common.h"
#include <string.h>

OS_EVENT    *w25qxx_rw_sem;

static int flash_type = 0;

u8 Flash_SPIx_ReadWriteByte(u8 TxData)
{
    u8 retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) { //检查指定的SPI标志位设置与否:发送缓存空标志位
        retry++;

        if (retry > 200)return 0;
    }

    SPI_I2S_SendData(SPI3, TxData); //通过外设SPIx发送一个数据
    retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位

    {
        retry++;

        if (retry > 200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI3); //返回通过SPIx最近接收的数据
}
void SPI_Flash_DMA_Mode(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
//    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE);
//    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);
    SPI_Cmd(SPI3, DISABLE);    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_RxOnly;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI3, &SPI_InitStructure);    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(SPI3, ENABLE);
}

void SPI_Flash_SPI_Mode(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);
    SPI_Cmd(SPI3, DISABLE);    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI3, &SPI_InitStructure);    /*!< Enable the sFLASH_SPI  */
    SPI_Cmd(SPI3, ENABLE);
}


#ifdef USE_F4
//初始化SPI FLASH的IO口
void SPI_Flash_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE );

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*!< Disable the sFLASH_SPI  */
    SPI_Cmd(SPI3, DISABLE);    /*!< SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI3, &SPI_InitStructure);    /*!< Enable the sFLASH_SPI  */

    SPI_Cmd(SPI3, ENABLE);


    /*!< Configure sFLASH Card CS pin in output pushpull mode ********************/

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_FLASH_CS_H;                            //取消片选


    flash_type = SPI_Flash_ReadID();

//    SPI_FLASH_DMA_RxConfig();
    if (flash_type == W25Q256) {
        delay_ms(100);
        SPI_FLASH_Write_SR3(0x02);//进入4线模式
        SPI_Flash_Enter_4Byte();

        if (SPI_Flash_ReadSR3() & 0x01 != 0x01) {
            delay_ms(100);
        }
    }

    w25qxx_rw_sem = OSSemCreate(1);


}

#else

//初始化SPI FLASH的IO口
void SPI_Flash_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3, ENABLE );


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //SPI HOLD
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_0);		//hold置高电平

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  //SPI CS
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//(上拉)
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//(浮空);//如果使用GPIO_Mode_IN_FLOATING需要加外部上拉电阻，否则会产生干扰中断
    GPIO_Init(GPIOB, &GPIO_InitStructure);
//DF_WP
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_7);


    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//定义波特率预分频的值:波特率预分频值为2
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
    SPI_Init(SPI3, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPI3, ENABLE); //使能SPI外设
    SPI_FLASH_CS_H;                            //取消片选
    flash_type = SPI_Flash_ReadID();

    if (flash_type == W25Q256) {
        SPI_FLASH_DMA_RxConfig();

        delay_ms(100);
        SPI_FLASH_Write_SR3(0x02);//进入4线模式
        SPI_Flash_Enter_4Byte();

        if (SPI_Flash_ReadSR3() & 0x01 != 0x01) {
            delay_ms(100);
        }
    }

    w25qxx_rw_sem = OSSemCreate(1);

}

#endif


//读取SPI_FLASH的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 SPI_Flash_ReadSR(void)
{
    u8 byte = 0;
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令
    byte = Flash_SPIx_ReadWriteByte(0Xff);           //读取一个字节
    SPI_FLASH_CS_H;                            //取消片选
    return byte;
}
//写SPI_FLASH状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void SPI_FLASH_Write_SR(u8 sr)
{
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令
    Flash_SPIx_ReadWriteByte(sr);               //写入一个字节
    SPI_FLASH_CS_H;                            //取消片选
}
//读取SPI_FLASH的状态寄存器
//HOLD  DRV1   DRV0   R   R   WPS   ADP   ADS
u8 SPI_Flash_ReadSR3(void)
{
    u8 byte = 0;
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(W25X_ReadStatusReg_3);    //发送读取状态寄存器命令
    byte = Flash_SPIx_ReadWriteByte(0Xff);           //读取一个字节
    SPI_FLASH_CS_H;                            //取消片选
    return byte;
}
//写SPI_FLASH状态寄存器
void SPI_FLASH_Write_SR3(u8 sr)
{
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(W25X_WriteStatusReg_3);   //发送写取状态寄存器命令
    Flash_SPIx_ReadWriteByte(sr);               //写入一个字节
    SPI_FLASH_CS_H;                            //取消片选
}
//SPI_FLASH写使能
//将WEL置位
void SPI_FLASH_Write_Enable(void)
{
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(W25X_WriteEnable);      //发送写使能
    SPI_FLASH_CS_H;                            //取消片选
}
//SPI_FLASH写禁止
//将WEL清零
void SPI_FLASH_Write_Disable(void)
{
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(W25X_WriteDisable);     //发送写禁止指令
    SPI_FLASH_CS_H;                            //取消片选
}
//读取芯片ID
//返回值如下:
//0XEF13,表示芯片型号为W25Q80
//0XEF14,表示芯片型号为W25Q16
//0XEF15,表示芯片型号为W25Q32
//0XEF16,表示芯片型号为W25Q64
//0XEF17,表示芯片型号为W25Q128
u16 SPI_Flash_ReadID(void)
{
    u16 Temp = 0;
    SPI_FLASH_CS_L;
    Flash_SPIx_ReadWriteByte(0x90);//发送读取ID命令
    Flash_SPIx_ReadWriteByte(0x00);
    Flash_SPIx_ReadWriteByte(0x00);
    Flash_SPIx_ReadWriteByte(0x00);
    Temp |= Flash_SPIx_ReadWriteByte(0xFF) << 8;
    Temp |= Flash_SPIx_ReadWriteByte(0xFF);
    SPI_FLASH_CS_H;
    return Temp;
}
//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void SPI_Flash_Read(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
    u16 i;
    u8 os_err;

#ifdef OS_CRITICAL_METHOD	 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
    OS_CPU_SR cpu_sr = 0;
    OS_ENTER_CRITICAL();		//停止调度
#endif
    OSSemPend(w25qxx_rw_sem, 0, &os_err); //等待其他操作完成

    SPI_FLASH_CS_L;                            //使能器件

    if (flash_type == W25Q256) {
        Flash_SPIx_ReadWriteByte(W25X_ReadData);         //发送读取命令
        Flash_SPIx_ReadWriteByte((u8)((ReadAddr) >> 24)); //发送32bit地址
        Flash_SPIx_ReadWriteByte((u8)((ReadAddr) >> 16));
        Flash_SPIx_ReadWriteByte((u8)((ReadAddr) >> 8));
        Flash_SPIx_ReadWriteByte((u8)ReadAddr);

    } else {
        Flash_SPIx_ReadWriteByte(W25X_ReadData);         //发送读取命令
        Flash_SPIx_ReadWriteByte((u8)((ReadAddr) >> 16)); //发送24bit地址
        Flash_SPIx_ReadWriteByte((u8)((ReadAddr) >> 8));
        Flash_SPIx_ReadWriteByte((u8)ReadAddr);
    }

    for (i = 0; i < NumByteToRead; i++) {
        pBuffer[i] = Flash_SPIx_ReadWriteByte(0XFF); //循环读数
    }

    SPI_FLASH_CS_H;                            //取消片选
    OSSemPost(w25qxx_rw_sem); //数据发送完成
#ifdef OS_CRITICAL_METHOD	 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
    OS_EXIT_CRITICAL();//准许调度
#endif
}
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void SPI_Flash_Write_Page(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 i;
    SPI_FLASH_Write_Enable();                  //SET WEL
    SPI_FLASH_CS_L;                            //使能器件

    if (flash_type == W25Q256) {
        Flash_SPIx_ReadWriteByte(W25X_PageProgram);         //发送读取命令
        Flash_SPIx_ReadWriteByte((u8)((WriteAddr) >> 24)); //发送32bit地址
        Flash_SPIx_ReadWriteByte((u8)((WriteAddr) >> 16));
        Flash_SPIx_ReadWriteByte((u8)((WriteAddr) >> 8));
        Flash_SPIx_ReadWriteByte((u8)WriteAddr);

    } else {
        Flash_SPIx_ReadWriteByte(W25X_PageProgram);      //发送写页命令
        Flash_SPIx_ReadWriteByte((u8)((WriteAddr) >> 16)); //发送24bit地址
        Flash_SPIx_ReadWriteByte((u8)((WriteAddr) >> 8));
        Flash_SPIx_ReadWriteByte((u8)WriteAddr);
    }

    for (i = 0; i < NumByteToWrite; i++)Flash_SPIx_ReadWriteByte(pBuffer[i]); //循环写数

    SPI_FLASH_CS_H;                            //取消片选
    SPI_Flash_Wait_Busy();					   //等待写入结束
}
//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void SPI_Flash_Write_NoCheck(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u16 pageremain;
    pageremain = 256 - WriteAddr % 256; //单页剩余的字节数

    if (NumByteToWrite <= pageremain)pageremain = NumByteToWrite; //不大于256个字节

    while (1) {
        SPI_Flash_Write_Page(pBuffer, WriteAddr, pageremain);

        if (NumByteToWrite == pageremain)break; //写入结束了
        else { //NumByteToWrite>pageremain
            pBuffer += pageremain;
            WriteAddr += pageremain;

            NumByteToWrite -= pageremain;			 //减去已经写入了的字节数

            if (NumByteToWrite > 256)pageremain = 256; //一次可以写入256个字节
            else pageremain = NumByteToWrite; 	 //不够256个字节了
        }
    };
}
//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//添加 判断写入数据和原来的一致就不需要写了。写完之后再读出来检查一次写入的数据
u8 SPI_FLASH_BUF[4096];
u8 SPI_Flash_Write(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    u8 os_err;

#ifdef OS_CRITICAL_METHOD	 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
    OS_CPU_SR cpu_sr = 0;
    OS_ENTER_CRITICAL();		//停止调度
#endif
    secpos = WriteAddr / 4096; //扇区地址 0~511 for w25x16
    secoff = WriteAddr % 4096; //在扇区内的偏移
    secremain = 4096 - secoff; //扇区剩余空间大小

    if (NumByteToWrite <= secremain)secremain = NumByteToWrite; //不大于4096个字节

    OSSemPend(w25qxx_rw_sem, 0, &os_err); //等待其他操作完成


    while (1) {
        SPI_Flash_Read(SPI_FLASH_BUF, secpos * 4096, 4096); //读出整个扇区的内容

//------------------------  add 如果要写的数据和读出来的数据一致就不再写入了---------------------
        for (i = 0; i < secremain; i++) { //校验数据
            if (SPI_FLASH_BUF[secoff + i] != pBuffer[i])break; //需要写入
        }

        if (i < secremain) { //需要写入
            for (i = 0; i < secremain; i++) { //校验数据
                if (SPI_FLASH_BUF[secoff + i] != 0XFF)break; //需要擦除
            }

            if (i < secremain) { //需要擦除
                SPI_Flash_Erase_Sector(secpos);//擦除这个扇区

                for (i = 0; i < secremain; i++) { //复制
                    SPI_FLASH_BUF[i + secoff] = pBuffer[i];
                }

                SPI_Flash_Write_NoCheck(SPI_FLASH_BUF, secpos * 4096, 4096); //写入整个扇区

            } else SPI_Flash_Write_NoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.

//-----------------------------校验---------------------------------
            SPI_Flash_Read(SPI_FLASH_BUF, secpos * 4096, 4096); //读出整个扇区的内容

            for (i = 0; i < secremain; i++) { //校验数据
                if (SPI_FLASH_BUF[secoff + i] != pBuffer[i])break; //不一致
            }

            if (i < secremain) { //写入失败
#ifdef OS_CRITICAL_METHOD	 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
                OS_EXIT_CRITICAL();			//准许调度
#endif
                OSSemPost(w25qxx_rw_sem); //数据发送完成

                return 0;
            }

//-------------------------------------------------------------------------
        }

        if (NumByteToWrite == secremain)break; //写入结束了
        else { //写入未结束
            secpos++;//扇区地址增1
            secoff = 0; //偏移位置为0

            pBuffer += secremain; //指针偏移
            WriteAddr += secremain; //写地址偏移
            NumByteToWrite -= secremain;				//字节数递减

            if (NumByteToWrite > 4096)secremain = 4096;	//下一个扇区还是写不完
            else secremain = NumByteToWrite;			//下一个扇区可以写完了
        }
    }

    OSSemPost(w25qxx_rw_sem); //数据发送完成

#ifdef OS_CRITICAL_METHOD	 	//如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
    OS_EXIT_CRITICAL();			//准许调度
#endif
    return 1;

}
//擦除整个芯片
//整片擦除时间:
//W25X16:25s
//W25X32:40s
//W25X64:40s
//等待时间超长...
void SPI_Flash_Erase_Chip(void)
{
    SPI_FLASH_Write_Enable();                  //SET WEL
    SPI_Flash_Wait_Busy();
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令
    SPI_FLASH_CS_H;                            //取消片选
    SPI_Flash_Wait_Busy();   				   //等待芯片擦除结束
}
//擦除一个扇区
//Dst_Addr:扇区地址 0~511 for w25x16
//擦除一个山区的最少时间:150ms
void SPI_Flash_Erase_Sector(u32 Dst_Addr)
{
    Dst_Addr *= 4096;
    SPI_FLASH_Write_Enable();                  //SET WEL
    SPI_Flash_Wait_Busy();
    SPI_FLASH_CS_L;                            //使能器件

    if (flash_type == W25Q256) {
        Flash_SPIx_ReadWriteByte(W25X_SectorErase);         //发送读取命令
        Flash_SPIx_ReadWriteByte((u8)((Dst_Addr) >> 24)); //发送32bit地址
        Flash_SPIx_ReadWriteByte((u8)((Dst_Addr) >> 16));
        Flash_SPIx_ReadWriteByte((u8)((Dst_Addr) >> 8));
        Flash_SPIx_ReadWriteByte((u8)Dst_Addr);

    } else {
        Flash_SPIx_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令
        Flash_SPIx_ReadWriteByte((u8)((Dst_Addr) >> 16)); //发送24bit地址
        Flash_SPIx_ReadWriteByte((u8)((Dst_Addr) >> 8));
        Flash_SPIx_ReadWriteByte((u8)Dst_Addr);
    }

    SPI_FLASH_CS_H;                            //取消片选
    SPI_Flash_Wait_Busy();   				   //等待擦除完成
}
//等待空闲
void SPI_Flash_Wait_Busy(void)
{
    while ((SPI_Flash_ReadSR() & 0x01) == 0x01); // 等待BUSY位清空
}
//进入掉电模式
void SPI_Flash_PowerDown(void)
{
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(W25X_PowerDown);        //发送掉电命令
    SPI_FLASH_CS_H;                            //取消片选
    delay_us(3);                               //等待TPD
}
//唤醒
void SPI_Flash_WAKEUP(void)
{
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB
    SPI_FLASH_CS_H;                            //取消片选
    delay_us(3);                               //等待TRES1
}

void SPI_Flash_Enter_4Byte(void)
{
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(0xB7);   //  send W25X_PowerDown command 0xAB
    SPI_FLASH_CS_H;                            //取消片选
    delay_us(3);                               //等待TRES1
}

void SPI_Flash_Exit_4Byte(void)
{
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(0xE9);   //  send W25X_PowerDown command 0xAB
    SPI_FLASH_CS_H;                            //取消片选
    delay_us(3);                               //等待TRES1
}

u8 SPI_Flash_ReadAdd(void)
{
    u8 byte = 0;
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(0xC8);    //发送读取状态寄存器命令
    byte = Flash_SPIx_ReadWriteByte(0Xff);           //读取一个字节
    SPI_FLASH_CS_H;                            //取消片选
    return byte;
}

void SPI_FLASH_WriteAdd(u8 sr)
{
    SPI_FLASH_CS_L;                            //使能器件
    Flash_SPIx_ReadWriteByte(0xC5);   //发送写取状态寄存器命令
    Flash_SPIx_ReadWriteByte(sr);               //写入一个字节
    SPI_FLASH_CS_H;                            //取消片选
}

//#define ADDR_FLASH_RESERVED		(32*1024*1024)
//#define SIZE_TEST_BUF 			(789)
//u8 TestBuf[SIZE_TEST_BUF];
//int W25Qxx_Test(void)
//{
//    int addr = 0, i = 0;

//    for ( addr = 0; addr < ADDR_FLASH_RESERVED; ) {
//		IWDG_ReloadCounter();
//        for ( i = 0; i < SIZE_TEST_BUF; i++ ) {
//            TestBuf[i] = (u8) (addr + i);
//        }

//        SPI_Flash_Write(TestBuf, addr,  SIZE_TEST_BUF );
//        addr += SIZE_TEST_BUF;
//    }

//    for ( addr = 0; addr < ADDR_FLASH_RESERVED; ) {
//		IWDG_ReloadCounter();
//        SPI_Flash_Read(TestBuf, addr,  SIZE_TEST_BUF );

//        for ( i = 0; i < SIZE_TEST_BUF; i++ ) {
//            if ( TestBuf[i] != (u8)(addr + i) ) {
//                return -1;
//            }
//        }

//        addr += SIZE_TEST_BUF;
//    }


//    return 1;
//}














