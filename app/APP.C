/******************************************************************************

  文 件 名   : APP.C
  版 本 号   : 初稿
  作    者   : 邹稳
  生成日期   : 2013年4月18日 星期四
  最近修改   :
  功能描述   : 与该工程有关的其他函数（与其他文件关联性不大）
  函数列表   :
              beep_port_init
              GPIO_Configuration
              input_port_init
              jdq_port_init
              TIMER4_Configuration
  修改历史   :
  1.日    期   : 2013年4月18日 星期四
    作    者   : 邹稳
    修改内容   : 创建文件

******************************************************************************/


/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
#include "app.h"
#include <string.h>
#include "common.h"
#include "datasave.h"
#include "parameters.h"
#include "encrypt.h"
#include "protocol.h"

/*****************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(ALARM_LED_GPIO_CLK|RUN_LED_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_InitStructure.GPIO_Pin = ALARM_LED_PIN;
    GPIO_Init(ALARM_LED_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = RUN_LED_PIN;
    GPIO_Init(RUN_LED_GPIO_PORT, &GPIO_InitStructure);


    ALARM_LED_ON;
    RUN_LED_ON;
    delay_ms(300);
    ALARM_LED_OFF;
    RUN_LED_OFF;


    /* Enable the GPIO_LED Clock */
    RCC_AHB1PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
    BEEP_OFF;
}

u8 beep_flag = 0;
void beep_off(void)
{
    if (beep_flag != 0) {
        beep_flag++;

        if (beep_flag >= 120) {
            beep_flag = 0;
            BEEP_OFF;
        }
    }
}

void beep_on(void)
{
    if (basic_inf.touch_beep == STA_ON) {
        beep_flag = 1;
        BEEP_ON;
    }
}


u8 hardware_ver = 0;

void Get_HardVer(void)
{
#if USE_GD == 1
	hardware_ver = HARD_V10;
#else	
    u8 hardver = 0;

    if (PDin(2) == 1) {
        hardver |= 0;
    } else {
        hardver |= 1;
    }

    hardver <<= 1;

    if (PCin(12) == 1) {
        hardver |= 0;
    } else {
        hardver |= 1;
    }

    hardver <<= 1;

    if (PCin(11) == 1) {
        hardver |= 0;
    } else {
        hardver |= 1;
    }

    hardver <<= 1;

    if (PCin(10) == 1) {
        hardver |= 0;
    } else {
        hardver |= 1;
    }

    hardware_ver = hardver;
	
#endif	
}

u8 get_hardware_ver(void)
{
    return hardware_ver;
}

void VerRead_Init(void)
{
#if USE_GD == 1

#else	
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_13;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif
    Get_HardVer();
}

void AC_Check_Proc(void)
{
    static int close_time = 0;

    if (get_hardware_ver() < HARD_V3) {
		sys_sta.ac_power_off = STA_OFF;
        return;
    }

    if (PDin(13) == 1) {
        close_time++;
    } else {
        close_time = 0;
    }

    if (close_time > 4) {
        close_time = 4;
		sys_sta.ac_power_off = STA_ON;
    }else{
		sys_sta.ac_power_off = STA_OFF;
	}

}



#define DEV_TYPE_NUM	(24)
#define DEV_MAGIC_WID	(12)
#define DEV_TYPE_WID	(14)
#define DEV_TYPE_WID2	(15)
unsigned short dev_type_code[DEV_TYPE_NUM];
// _flag 1-保存当前设备类型，0-清除当前设备类型
void Write_Dev_Type(uint8_t _flag)
{
    uint32_t write_addr, i;

    for (i = 0; i < DEV_TYPE_NUM; i++) {
        dev_type_code[i] = *(__IO uint16_t*)(ADDR_FLASH_SECTOR + 2 * i);
    }

    if (_flag == STA_ON) {
        if (dev_type_code[DEV_MAGIC_WID] == KC_MAGIC && dev_type_code[DEV_TYPE_WID] == SYSTEM_TYPE
                && dev_type_code[DEV_TYPE_WID2] == 0) {
            return ;
        }

        dev_type_code[DEV_MAGIC_WID] = KC_MAGIC;
        dev_type_code[DEV_TYPE_WID] = SYSTEM_TYPE;
        dev_type_code[DEV_TYPE_WID2] = 0;
    } else {
        dev_type_code[DEV_MAGIC_WID] = 0;
        dev_type_code[DEV_TYPE_WID] = 0;
        dev_type_code[DEV_TYPE_WID2] = 0;
    }

    FLASH_Unlock();
#if USE_F1 == 1

    if (FLASH_ErasePage(ADDR_FLASH_SECTOR) == FLASH_COMPLETE) {
        write_addr = ADDR_FLASH_SECTOR;

        for (i = 0; i < DEV_TYPE_NUM; i++) {
            FLASH_ProgramHalfWord((write_addr + 2 * i), *(dev_type_code + i));
        }
    }

#else

    if (FLASH_EraseSector(ADDR_FLASH_REAL, VoltageRange_3) == FLASH_COMPLETE) {

        write_addr = ADDR_FLASH_SECTOR;

        for (i = 0; i < DEV_TYPE_NUM; i++) {
            FLASH_ProgramHalfWord((write_addr + 2 * i), *(dev_type_code + i));
        }

    }

#endif
    FLASH_Lock();
}

void IWDG_Init(void)
{

    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {

        /* Clear reset flags */
        RCC_ClearFlag();
    }

    /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
         dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: LSI/32 */
    IWDG_SetPrescaler(IWDG_Prescaler_256);

    /* Set counter reload value to obtain 250ms IWDG TimeOut.
       Counter Reload Value = 5000ms/IWDG counter clock period
                            = 5000ms / (LSI/32)
                            = 5s / (LsiFreq/32)
                            = LsiFreq/(32 * 4)
                            = LsiFreq/128
     */
    IWDG_SetReload(1000);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}




