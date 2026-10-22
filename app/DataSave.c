#include "DataSave.h"
#include "eeprom.h"
#include <string.h>
#include "Parameters.h"
#include "tslib_calibration.h"
#include "bsp.h"
#include "menuinc.h"
#include "pcf8563.h"
#include "ff.h"
#include "recode.h"
#include "w25qxx.h"
#include "protocol.h"
#include "jdq.h"
#include "def.h"
#include "usart.h"
#include "Common.h"
#include "modbus.h"
#include "trial_ver.h"

/*****************************************************************************/
#define TEST_WNUM		(1024)//<1024
#define EEPROM_MAX		(32*1024)
__align(4) INT8U eeprom_Wbuf[1024] = {0};
__align(4) INT8U eeprom_Rbuf[1024] = {0};
INT8U EEPEOM_ALL_TEST(void)
{
    INT8U 	ret = true;
    INT32U 	addr = 0, i = 0;
    INT16U 	ee_test = 0;

    EEPROM_Read(EE_EETEST_ADDR, (INT8U *)&ee_test, sizeof(ee_test));

    if (ee_test == KC_MAGIC) {
        return ret;
    }

    for (addr = 0; addr < EEPROM_MAX;) {
        for (i = 0; i < TEST_WNUM; i++) {
            eeprom_Wbuf[i] = (u8)(addr + i);
        }

        if (addr + TEST_WNUM <= EEPROM_MAX) {
            EEPROM_Write(addr, eeprom_Wbuf,  TEST_WNUM);
            addr += TEST_WNUM;

        } else {
            EEPROM_Write( addr, eeprom_Wbuf, (EEPROM_MAX - addr));
            break;
        }
    }

    for (addr = 0; addr < EEPROM_MAX;) {
        for (i = 0; i < TEST_WNUM; i++) {
            eeprom_Wbuf[i] = (u8)(addr + i);
        }

        if (addr + TEST_WNUM <= EEPROM_MAX) {
            EEPROM_Read( addr, eeprom_Rbuf, TEST_WNUM);

            if (memcmp(eeprom_Wbuf, eeprom_Rbuf, TEST_WNUM) != 0) {
                ret = false;
                break;
            }

            addr += TEST_WNUM;
        } else {
            EEPROM_Read(addr, eeprom_Rbuf,  (EEPROM_MAX - addr));

            if (memcmp(eeprom_Wbuf, eeprom_Rbuf, (EEPROM_MAX - addr)) != 0) {
                ret = false;
                break;
            }

            break;
        }
    }

    if (ret == true) {
        ee_test = KC_MAGIC;
        EEPROM_Write( EE_EETEST_ADDR, (INT8U *)&ee_test, sizeof(ee_test) );
    }

    //重新写入软件参数
    basic_inf.type = ROOM_CHICKEN_MEAT;
    EEPROM_Write( EE_SOFT_ADDR, (INT8U*)&soft_inf.wMagic, sizeof(soft_inf));
    EEPROM_Write( EE_CAL_ADDR, (INT8U*)&cal.a[0], sizeof(cal.a) );
    SetInf_Def();

    return ret;
}

/*****************************************************************************
 函 数 名  : Save_DEVMem
 功能描述  : 存储DEV
 输入参数  : int offset
             INT8U * member
             int len
 输出参数  : 无
 返 回 值  : >0正常
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2019年3月26日
    作    者   : 邹稳
    修改内容   : 新生成函数

*****************************************************************************/
int Save_DEVMem( int offset, INT8U * member, int len )
{
    int	ires, dwAddr;

    if ( offset < 0 ||
            member == NULL || len < 1 ) {
        return -1;
    }

    dwAddr = EE_DEV_ADDR + offset;
    ires = EEPROM_Write( dwAddr, member, len );

    return ires;
}

int Save_EXTMem( int offset, INT8U * member, int len )
{
    int	ires, dwAddr;

    if ( offset < 0 ||
            member == NULL || len < 1 ) {
        return -1;
    }

    dwAddr = EE_EXT_ADDR + offset;
    ires = EEPROM_Write( dwAddr, member, len );

    return ires;
}

/*****************************************************************************
 函 数 名  : Save_CTRLMem
 功能描述  : 存储CTRL
 输入参数  : int offset
             INT8U * member
             int len
 输出参数  : 无
 返 回 值  : >0正常
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2019年3月26日
    作    者   : 邹稳
    修改内容   : 新生成函数

*****************************************************************************/
int Save_CTRLMem( int offset, INT8U * member, int len )
{
    int	ires, dwAddr;

    if ( offset < 0 ||
            member == NULL || len < 1 ) {
        return -1;
    }

    dwAddr = EE_CTRL_ADDR + offset;
    ires = EEPROM_Write( dwAddr, member, len );

    return ires;
}

/*****************************************************************************
 函 数 名  : Save_BASICMem
 功能描述  : 存储BASIC
 输入参数  : int offset
             INT8U * member
             int len
 输出参数  : 无
 返 回 值  : >0正常
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2019年3月26日
    作    者   : 邹稳
    修改内容   : 新生成函数

*****************************************************************************/
int Save_BASICMem( int offset, INT8U * member, int len )
{
    int	ires, dwAddr;

    if ( offset < 0 ||
            member == NULL || len < 1 ) {
        return -1;
    }

    dwAddr = EE_BASIC_ADDR + offset;
    ires = EEPROM_Write( dwAddr, member, len );

    return ires;
}
int Save_INDEXMem( int offset, INT8U * member, int len )
{
    int	ires, dwAddr;

    if ( offset < 0 ||
            member == NULL || len < 1 ) {
        return -1;
    }

    dwAddr = EE_INDEX_ADDR + offset;
    ires = EEPROM_Write( dwAddr, member, len );

    return ires;
}

/*****************************************************************************
 函 数 名  : Update_SaveData
 功能描述  : 升级数据保存
 输入参数  : u8 type      0-备份文件 1-升级文件
             int seq      当前升级序列号
             int len      升级数据包长度
             INT8U * buf  升级文件
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2020-03-12
    作    者   : 邹稳
    修改内容   : 新生成函数

*****************************************************************************/
int Update_SaveData( u8 type, int seq, int len, INT8U * buf)
{
    int addr = 0;
    int res = 0;

    if (buf == NULL || len == 0 || type > UP_VER_CUR) {
        return -1;
    }

    if (type == UP_VER_BAK) {
        addr = soft_bak_addr + sizeof(update_st.len);
    } else {
        addr = soft_cur_addr + sizeof(update_st.len);
    }

    addr += seq * UP_DATA_SIZE;
    res = SPI_Flash_Write(buf, addr, len);

    return res;
}
/*****************************************************************************
 函 数 名  : EEPROM_SaveData
 功能描述  : 数据保存
 输入参数  : INT8U sid
 输出参数  : 无
 返 回 值  : >0正常
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2019年3月26日
    作    者   : 邹稳
    修改内容   : 新生成函数

*****************************************************************************/
int	EEPROM_SaveData(INT8U sid)
{
    int		ires, offset, size;

    switch ( sid ) {
    case SSENSOR_ID: {
        offset = OFFSETOF(TDEV, sensor_adj);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.sensor_adj, sizeof(dev_inf.sensor_adj) );
    }
    break;

    case SDEV_SHOW_ID: {
        offset = OFFSETOF(TDEV, dev_equ_show);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.dev_equ_show, sizeof(dev_inf.dev_equ_show) + sizeof(dev_inf.ctrl_equ_show));
    }
    break;

    case STEMP_DEF_ID: {
        offset = OFFSETOF(TDEV, TEMP_Define);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.TEMP_Define, sizeof(dev_inf.TEMP_Define) );
    }
    break;

    case SSWIN_ID: {
        offset = OFFSETOF(TDEV, SWIN);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.SWIN, sizeof(dev_inf.SWIN) );
    }
    break;

    case SWIN_ID: {
        offset = OFFSETOF(TDEV, WIN);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.WIN, sizeof(dev_inf.WIN) );
    }
    break;

    case SDIN_ID: {
        offset = OFFSETOF(TEXT, din);
        ires = Save_EXTMem( offset, (INT8U *)&ext_inf.din, sizeof(ext_inf.din) );
    }
    break;

    case SFAN_CHK_ID: {
        offset = OFFSETOF(TDEV, fan_chk);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.fan_chk, sizeof(dev_inf.fan_chk) );
    }
    break;

    case SPWM_ID: {
        offset = OFFSETOF(TDEV, PWM);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.PWM[0], sizeof(dev_inf.PWM) );
    }
    break;

    case SJDQ_ID: {
        offset = OFFSETOF(TDEV, JDQ);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.JDQ[0], sizeof(dev_inf.JDQ) );
    }
    break;

    case SHEAT_ID: {
        offset = OFFSETOF(TBASIC, heat_en);
        ires = Save_BASICMem( offset, (INT8U *)&basic_inf.heat_en, (sizeof(basic_inf.heat_en) + sizeof(basic_inf.heat)
                              + sizeof(basic_inf.pheat_en) + sizeof(basic_inf.pheat)) );
    }
    break;

    case STEMPC_ID: {
        offset = OFFSETOF(TBASIC, tempc_en);
        ires = Save_BASICMem( offset, (INT8U *)&basic_inf.tempc_en, (sizeof(basic_inf.tempc_en) + sizeof(basic_inf.tempc)) );
    }
    break;

    case STEMPC_PWM_ID: {
        offset = OFFSETOF(TBASIC, tempc_pwm_en);
        ires = Save_BASICMem( offset, (INT8U *)&basic_inf.tempc_pwm_en, (sizeof(basic_inf.tempc_pwm_en) + sizeof(basic_inf.tempc_pwm)) );
    }
    break;

    case SAPWM_ID: {
        offset = OFFSETOF(TBASIC, area_pwm);
        ires = Save_BASICMem( offset, (INT8U *)&basic_inf.area_pwm, sizeof(basic_inf.area_pwm) );
    }
    break;

    case SSPRAY_ID: {
        offset = OFFSETOF(TDEV, spray_en);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.spray_en, (sizeof(dev_inf.spray_en) + sizeof(dev_inf.spray)) );
    }
    break;

    case SWET_ID: {
        offset = OFFSETOF(TDEV, wet_en);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.wet_en, (sizeof(dev_inf.wet_en) + sizeof(dev_inf.wet)) );
    }
    break;

    case SLIGHT_ID: {
        offset = OFFSETOF(TDEV, light);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.light, sizeof(dev_inf.light) );
    }
    break;

    case SCIRCLE_ID: {
        offset = OFFSETOF(TDEV, circle);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.circle, sizeof(dev_inf.circle) );
    }
    break;

    case STIMER_ID: {
        offset = OFFSETOF(TEXT, timer);
        ires = Save_EXTMem( offset, (INT8U *)&ext_inf.timer, sizeof(ext_inf.timer) );
    }
    break;

    case SLV_SAVE_ID: {
        offset = OFFSETOF(TEXT, lv_save);
        ires = Save_EXTMem( offset, (INT8U *)&ext_inf.lv_save, sizeof(ext_inf.lv_save) );
    }
    break;

    case SDAYTEMP_ID: {
        offset = OFFSETOF(TEXT, day_temp);
        ires = Save_EXTMem( offset, (INT8U *)&ext_inf.day_temp, sizeof(ext_inf.day_temp) );
    }
    break;
	
    case SENV_ID: {
        offset = OFFSETOF(TDEV, env_dev);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.env_dev[0], (sizeof(dev_inf.env_dev) + sizeof(dev_inf.limit)));
    }
    break;

    case SSENSOR_RG_ID: {
        offset = OFFSETOF(TDEV, sensor_rg);
        ires = Save_DEVMem( offset, (INT8U *)&dev_inf.sensor_rg, sizeof(dev_inf.sensor_rg) );
    }
    break;

    case STEMP_ID: {
        offset = OFFSETOF(TCTRL, temp_adj);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.temp_adj, (sizeof(ctrl_inf.temp_adj) + sizeof(ctrl_inf.temp)) );

        offset = OFFSETOF(TCTRL, extemp);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.extemp, sizeof(ctrl_inf.extemp));
    }
    break;

    case SMINAIRC_ID: {
        offset = OFFSETOF(TCTRL, minair_en);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.minair_en, (sizeof(ctrl_inf.minair_en) + sizeof(ctrl_inf.minair)) );

        offset = OFFSETOF(TCTRL, exminair);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.exminair, sizeof(ctrl_inf.exminair) );

    }
    break;

    case SHUM_ID: {
        offset = OFFSETOF(TCTRL, hum);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.hum, sizeof(ctrl_inf.hum));
    }
    break;

    case SLVPWM_ID:
    case SLVWIN_ID:
    case SLVJDQ_ID: {
        //分两次写
        offset = 0;
        size = sizeof(TLV) * (CTRL_LV_NUM / 2) + sizeof(ctrl_inf.wMagic);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.wMagic, size );

        if (ires < 0)break;

        offset += size;
        size = sizeof(ctrl_inf.lv) - sizeof(TLV) * (CTRL_LV_NUM / 2) - sizeof(ctrl_inf.wMagic);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.lv[CTRL_LV_NUM / 2], size );

        offset = OFFSETOF(TCTRL, exlv);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.exlv[0], sizeof(ctrl_inf.exlv));

    }
    break;

    case SSCOPE_ID: {
        offset = OFFSETOF(TCTRL, lv_scope);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.lv_scope[0], (sizeof(ctrl_inf.lv_scope) + sizeof(ctrl_inf.lv_scope)) );
    }
    break;

    case SSENCON_ID: {
        offset = OFFSETOF(TCTRL, sencon);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.sencon[0], sizeof(ctrl_inf.sencon) );
    }
    break;

    case SVAC_ID: {
        offset = OFFSETOF(TCTRL, vac);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.vac, sizeof(ctrl_inf.vac) );
    }
    break;

    case SVAR_ID: {
        offset = OFFSETOF(TCTRL, var);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.var, sizeof(ctrl_inf.var) );
    }
    break;

    case SLVMORE_ID: {
        offset = OFFSETOF(TCTRL, lv_more);
        ires = Save_CTRLMem( offset, (INT8U *)&ctrl_inf.lv_more, sizeof(ctrl_inf.lv_more) );
    }
    break;

    case SBASIC_ID: {
        size = OFFSETOF(TBASIC, x);
        offset = 0;
        ires = Save_BASICMem( offset, (INT8U *)&basic_inf.wMagic, size );
    }
    break;

    case SINDEX_ID: {
        offset = 0;
        ires = Save_INDEXMem( offset, (INT8U *)&index_inf.wMagic, sizeof(index_inf));
    }
    break;

    case SCLOCK_ID: {
        ires = EEPROM_Write(EE_CLOCK_ADDR, (INT8U *)&clock_inf.wMagic, sizeof(clock_inf));
    }
    break;

    case SSOFT_ID: {
        ires = EEPROM_Write(EE_SOFT_ADDR, (INT8U *)&soft_inf.wMagic, sizeof(soft_inf));
    }

    break;

    case SSOFT_INIT_ID: {
        ires = SPI_Flash_Write((INT8U *)&update_st.wMagic, update_init_addr, sizeof(update_st));
    }

    break;

    default:
        ires = -1;
        break;
    }

    return ires;
}

int	lv_save_data(INT8U lv)
{
    int		ires, offset;

    if (lv >= CTRL_LV_NUM) {
        return -1;
    }

    offset = OFFSETOF(TEXT, lv_save);
    offset += lv * sizeof(TLV_SAVE);
    ires = Save_EXTMem( offset, (INT8U *)&ext_inf.lv_save[lv], sizeof(TLV_SAVE) );
    return ires;
}


INT8U his_data_Wbuf[4096] = {0};
// FIXME:	数据存储可以全部一次性存储在flash中，不需要在eeprom缓存。
int his_data_save(void)
{
    int		ires;

    if (sys_sta.on_sec < (sys_sta.his_cycletime + MIN_TO_SEC)) {
        return -1;
    }

    sys_sta.his_cycletime = sys_sta.on_sec;

    memset((INT8U *)&his_data.cur_time, 0, sizeof(THDATA));
    his_data.cur_time = GetSysTime();
    his_data.avg_temp = cal_inf.temp;
    his_data.heat_temp = cal_inf.heat_temp;
    his_data.target_temp = cal_inf.target_temp;
    his_data.fan_temp = cal_inf.fan_temp;
    his_data.avg_hum = cal_inf.SEN_EN[SENSOR_TYPE_HUM];
    his_data.ala = ALA_State();

    // crc
    his_data.crc = crc16_s((INT8U *)&his_data.cur_time, (sizeof(THDATA) - sizeof(his_data.crc)));

    //save
    if (index_inf.his_buf_id >= HIS_BUF_NUM) {//判断限制
        index_inf.his_buf_id = 0;
    }

    ires = EEPROM_Write(EE_HIS_ADDR + index_inf.his_buf_id * sizeof(THDATA), (INT8U *)&his_data.cur_time, sizeof(THDATA));

    if (ires <= 0) {
        return ires;
    }

    index_inf.his_buf_id++;

    if (index_inf.his_buf_id >= HIS_BUF_NUM) {//判断限制
        index_inf.his_buf_id = 0;

        //转存flash
        if (index_inf.his_id >= HIS_NUM) {//判断限制
            index_inf.his_id = 0;
        }

        EEPROM_Read(EE_HIS_ADDR, his_data_Wbuf, sizeof(his_data_Wbuf));
        ires = SPI_Flash_Write(his_data_Wbuf, (his_data_addr + index_inf.his_id * 4096), sizeof(his_data_Wbuf));

        if (ires <= 0) {
            return ires;
        }

        index_inf.his_id++;

        if (index_inf.his_id >= HIS_NUM) {//判断限制
            index_inf.his_id = 0;
            index_inf.his_state = 1;
        }

        Config_Save(SINDEX_ID);
    }

//	Config_Save(SINDEX_ID); 10分钟自动保存一次序列号 断电最多可能丢10分钟数据
    return 3;
}

void set_lv_pwm(unsigned char lv, unsigned char num, unsigned char mode,
                unsigned char degree, unsigned short on, unsigned short off, unsigned short cycle )
{
    ctrl_inf.lv[lv].PWM[num].mode = mode;
    ctrl_inf.lv[lv].PWM[num].degree = degree;
    ctrl_inf.lv[lv].PWM[num].on_time = on;
    ctrl_inf.lv[lv].PWM[num].off_time = off;
    ctrl_inf.lv[lv].PWM[num].cycle = cycle;
}

void set_lv_win(unsigned char lv, unsigned char num, unsigned char mode, unsigned char degree)
{
    if (num < WINDOW_NUM_MAX - 2) {
        ctrl_inf.lv[lv].WIN[num].degree = degree;
    } else {
        ctrl_inf.exlv[lv].WIN[num - 2].degree = degree;
    }
}


void set_lv_swin(unsigned char lv, unsigned char num, unsigned char mode, unsigned char degree)
{
    ctrl_inf.lv[lv].SWIN[num].degree = degree;
}

void set_lv_jdq(unsigned char lv, unsigned char num, unsigned char mode, unsigned short on, unsigned short off, unsigned short cycle )
{
    ctrl_inf.lv[lv].JDQ[num].mode = mode;
    ctrl_inf.lv[lv].JDQ[num].on_time = on;
    ctrl_inf.lv[lv].JDQ[num].off_time = off;
    ctrl_inf.lv[lv].JDQ[num].cycle = cycle;
}

void set_temp(unsigned char num, unsigned char en, unsigned short day,
              signed short heat, signed short target, signed short vertical, signed short min, signed short max)
{
    ctrl_inf.temp[num].enable = en;
    ctrl_inf.temp[num].grow_day = day;
    ctrl_inf.temp[num].heat_temp = heat;
    ctrl_inf.temp[num].target_temp = target;
    ctrl_inf.temp[num].vertical_temp = vertical;
    ctrl_inf.temp[num].min_temp = min;
    ctrl_inf.temp[num].max_temp = max;
}
void set_min_air(unsigned char num, unsigned char en, unsigned short day,
                 signed short weight, signed short breath)
{
    ctrl_inf.minair[num].enable = en;
    ctrl_inf.minair[num].grow_day = day;
    ctrl_inf.minair[num].weight = weight;
    ctrl_inf.minair[num].breath = breath;
}

void ctrl_inf_temp_init(void)
{
    unsigned char i = 0;
    //ctrl_inf.temp_en = true;

    ctrl_inf.temp_adj = 0;

    set_temp(0, true, 1, 335, 335, 360, 325, 350);
    set_temp(1, true, 7, 300, 300, 320, 290, 340);
    set_temp(2, true, 14, 280, 280, 295, 265, 330);
    set_temp(3, true, 21, 260, 260, 270, 240, 310);
    set_temp(4, true, 28, 240, 240, 250, 220, 290);
    set_temp(5, true, 35, 220, 220, 230, 195, 270);
    set_temp(6, true, 42, 190, 190, 200, 160, 250);

    for (i = 7 ; i < TEMP_CURVE_NUM ; i++) {
        set_temp(i, false, 60, 190, 190, 200, 160, 260);
    }

    ctrl_inf.temp[0].enable = true;

    ctrl_inf.extemp.start_day = 1;
    ctrl_inf.extemp.stop_day = 1;

    ctrl_inf.extemp.hum_Ltemp = 50;
    ctrl_inf.extemp.hum_Htemp = 80;

    ctrl_inf.extemp.ala_diff_en = STA_OFF;
    ctrl_inf.extemp.day_temp_en = STA_ON;
    ctrl_inf.extemp.chg_hours = 1;
    ctrl_inf.extemp.chg_times = 4;
    ctrl_inf.extemp.minwind_temp_sta = MIN_WIND_OFF;
    ctrl_inf.extemp.temp_simple = STA_ON;

    if (basic_inf.type != ROOM_CHICKEN_MEAT && basic_inf.type != ROOM_CHICKEN_BROOD
            && basic_inf.type != ROOM_DUCK_MEAT) {
        set_temp(0, true, 1, 180, 180, 190, 150, 260);

        for (i = 1 ; i < TEMP_CURVE_NUM ; i++) {
            set_temp(i, false, 1, 180, 180, 190, 150, 260);
        }
    }

}
void ctrl_inf_air_init(void)
{
    unsigned char i = 0, id = 0;
    ctrl_inf.minair_en = false;

    set_min_air(0, false, 1, 4, 100);
    set_min_air(1, false, 7, 18, 100);
    set_min_air(2, false, 14, 50, 100);
    set_min_air(3, false, 21, 97, 100);
    set_min_air(4, false, 28, 152, 100);
    set_min_air(5, false, 35, 213, 100);
    set_min_air(6, false, 42, 274, 100);

    for (i = 7 ; i < MINAIR_NUM ; i++) {
        set_min_air(i, false, 165, 1100, 22);
    }

    memset((INT8U *)&ctrl_inf.exminair.enable, 0, sizeof(ctrl_inf.exminair));
    ctrl_inf.exminair.x[id++].out_temp = -300;
    ctrl_inf.exminair.x[id++].out_temp = -100;
    ctrl_inf.exminair.x[id++].out_temp = 0;
    ctrl_inf.exminair.x[id++].out_temp = 100;
    ctrl_inf.exminair.x[id++].out_temp = 200;
    ctrl_inf.exminair.x[id++].out_temp = 300;

    for (i = 0; i < EXMINAIR_NUM; i++) {
        ctrl_inf.exminair.x[i].breath = 10;
    }

// add  hum
    memset((INT8U *)&ctrl_inf.hum, 0, sizeof(ctrl_inf.hum));
    ctrl_inf.hum.enable = STA_OFF;
    ctrl_inf.hum.max_temp = 30;
    ctrl_inf.hum.start_hum = 30;
    ctrl_inf.hum.stop_hum = 80;
    ctrl_inf.hum.min_adj_hum = 5;
    ctrl_inf.hum.start_time = 0;
    ctrl_inf.hum.end_time = 0;


    for (i = 0; i < HUM_CURVE_NUM; i++) {
        ctrl_inf.hum.x[i].enable = STA_OFF;
        ctrl_inf.hum.x[i].grow_day = 42;
        ctrl_inf.hum.x[i].target_hum = 45;
        ctrl_inf.hum.x[i].scale = 0;
    }

    id = 0;
    ctrl_inf.hum.x[id].grow_day = 1;
    ctrl_inf.hum.x[id].target_hum = 65;
    ctrl_inf.hum.x[id++].scale = 10;
    ctrl_inf.hum.x[id].grow_day = 7;
    ctrl_inf.hum.x[id].target_hum = 65;
    ctrl_inf.hum.x[id++].scale = 10;
    ctrl_inf.hum.x[id].grow_day = 14;
    ctrl_inf.hum.x[id].target_hum = 60;
    ctrl_inf.hum.x[id++].scale = 8;
    ctrl_inf.hum.x[id].grow_day = 21;
    ctrl_inf.hum.x[id].target_hum = 55;
    ctrl_inf.hum.x[id++].scale = 6;
    ctrl_inf.hum.x[id].grow_day = 28;
    ctrl_inf.hum.x[id].target_hum = 55;
    ctrl_inf.hum.x[id++].scale = 6;
    ctrl_inf.hum.x[id].grow_day = 35;
    ctrl_inf.hum.x[id].target_hum = 50;
    ctrl_inf.hum.x[id++].scale = 3;
    ctrl_inf.hum.x[id].grow_day = 42;
    ctrl_inf.hum.x[id].target_hum = 45;
    ctrl_inf.hum.x[id++].scale = 0;

}
void ctrl_inf_lv_init(void)
{
    unsigned char i = 0;
    unsigned char j = 0;

    for (i = 0; i < CTRL_LV_NUM; i ++) {
        ctrl_inf.lv[i].enable = false;
        ctrl_inf.lv[i].offset = 50;
        ctrl_inf.lv[i].re_diff = 1;

        for (j = 0; j < SWINDOW_NUM_MAX; j++) {
            set_lv_swin(i, j++, MODE_OFF, 0);
        }

        for (j = 0; j < WINDOW_NUM_MAX; j++) {
            set_lv_win(i, j++, MODE_OFF, 0);
        }

        for (j = 0; j < PWM_NUM_MAX; j++) {
            set_lv_pwm(i, j, MODE_OFF, 0, 150, 150, 300);
        }

        for (j = 0; j < JDQ_NUM_MAX; j++) {
            set_lv_jdq(i, j, MODE_OFF, 100, 100, 200);
        }
    }

    i = 0;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 0;
    ctrl_inf.lv[i].re_diff = 0;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 25);
    set_lv_pwm(i, 0, MODE_OFF, 0, 60, 240, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON_OFF, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 5;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 25);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON_OFF, 150, 150, 300);


    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 10;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 25);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 15;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 60);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 20;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 100);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 25;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_ON, 40);
    set_lv_swin(i, SWIN5_ID, MODE_OFF, 0);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 30;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_ON, 70);
    set_lv_swin(i, SWIN5_ID, MODE_OFF, 0);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 35;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_ON, 100);
    set_lv_swin(i, SWIN5_ID, MODE_OFF, 0);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
}

void ctrl_inf_lv_brood_init(void)
{
    unsigned char i = 0;
    unsigned char j = 0;

    for (i = 0; i < CTRL_LV_NUM; i ++) {
        ctrl_inf.lv[i].enable = false;
        ctrl_inf.lv[i].offset = 50;
        ctrl_inf.lv[i].re_diff = 1;

        for (j = 0; j < SWINDOW_NUM_MAX; j++) {
            set_lv_swin(i, j++, MODE_OFF, 0);
        }

        for (j = 0; j < WINDOW_NUM_MAX; j++) {
            set_lv_win(i, j++, MODE_OFF, 0);
        }

        for (j = 0; j < PWM_NUM_MAX; j++) {
            set_lv_pwm(i, j, MODE_OFF, 0, 150, 150, 300);
        }

        for (j = 0; j < JDQ_NUM_MAX; j++) {
            set_lv_jdq(i, j, MODE_OFF, 100, 100, 200);
        }
    }

    i = 0;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 0;
    ctrl_inf.lv[i].re_diff = 0;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 25);
    set_lv_pwm(i, 0, MODE_OFF, 0, 40, 260, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON_OFF, 150, 150, 300);



    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 3;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 25);
    set_lv_pwm(i, 0, MODE_OFF, 0, 90, 210, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON_OFF, 150, 150, 300);


    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 6;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 25);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON_OFF, 150, 150, 300);


    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 9;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 25);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 12;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 60);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 15;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_OFF, 0);
    set_lv_swin(i, SWIN5_ID, MODE_ON, 100);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 20;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_ON, 40);
    set_lv_swin(i, SWIN5_ID, MODE_OFF, 0);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 25;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_ON, 70);
    set_lv_swin(i, SWIN5_ID, MODE_OFF, 0);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);

    i++;
    j = 0;
    ctrl_inf.lv[i].enable = true;
    ctrl_inf.lv[i].offset = 30;
    ctrl_inf.lv[i].re_diff = 1;

    set_lv_win(i, WIN4_ID, MODE_ON, 100);
    set_lv_swin(i, SWIN5_ID, MODE_OFF, 0);
    set_lv_pwm(i, 0, MODE_OFF, 0, 150, 150, 300);
    j = 0;
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
    set_lv_jdq(i, j++, MODE_ON, 150, 150, 300);
}

void ctrl_inf_lv_assist_init(void)
{
    unsigned char i = 0, j = 0;

    for (i = 0; i < LV_SCOPE_KIND; i++) {
        ctrl_inf.lv_scope[i].enable = STA_OFF;

        for (j = 0; j < LV_SCOPE_NUM; j++) {
            ctrl_inf.lv_scope[i].curve[j].enable = STA_OFF;

            if (i == AS_TYPE_DAY) {
                ctrl_inf.lv_scope[i].curve[j].day_time = 99;
            } else if (i == AS_TYPE_TIME) {
                ctrl_inf.lv_scope[i].curve[j].day_time = MAX_TIME_DAY;
            } else {
                ctrl_inf.lv_scope[i].curve[j].day_time = 400;
            }

            ctrl_inf.lv_scope[i].curve[j].min_lv = 0;
            ctrl_inf.lv_scope[i].curve[j].max_lv = 10;
            ctrl_inf.lv_scope[i].curve[j].cold_lv = 3;
            ctrl_inf.lv_scope[i].curve[j].warm_lv = 10;
            ctrl_inf.lv_scope[i].curve[j].temp_type = 0;
            ctrl_inf.lv_scope[i].curve[j].temp_diff = 0;
            ctrl_inf.lv_scope[i].curve[j].temp_cold = 100;
            ctrl_inf.lv_scope[i].curve[j].temp_warm = 300;
        }
    }

    ctrl_inf.lv_scope[AS_TYPE_DAY].curve[0].day_time = 1;
    ctrl_inf.lv_scope[AS_TYPE_DAY].curve[1].day_time = 7;
    ctrl_inf.lv_scope[AS_TYPE_DAY].curve[2].day_time = 14;
    ctrl_inf.lv_scope[AS_TYPE_DAY].curve[3].day_time = 21;

    ctrl_inf.lv_scope[AS_TYPE_TIME].curve[0].day_time = 0;
    ctrl_inf.lv_scope[AS_TYPE_TIME].curve[1].day_time = 8 * 60;
    ctrl_inf.lv_scope[AS_TYPE_TIME].curve[2].day_time = 18 * 60;

    ctrl_inf.lv_scope[AS_TYPE_OUT].curve[0].day_time = 0;
    ctrl_inf.lv_scope[AS_TYPE_OUT].curve[1].day_time = 100;
    ctrl_inf.lv_scope[AS_TYPE_OUT].curve[2].day_time = 300;
    ctrl_inf.lv_scope[AS_TYPE_OUT].curve[3].day_time = 400;


    for (i = 0; i < SENCON_NUM; i++) {
        ctrl_inf.sencon[i].enable = STA_OFF;
        ctrl_inf.sencon[i].delay_min = 30;
        ctrl_inf.sencon[i].continue_sec = 180;
        ctrl_inf.sencon[i].heat_flag = 0;
        ctrl_inf.sencon[i].temp_drop = 3;
    }

    ctrl_inf.sencon[SENCON_HUM].target = 70;
    ctrl_inf.sencon[SENCON_HUM].alaL = 20;
    ctrl_inf.sencon[SENCON_HUM].alaH = 90;
    ctrl_inf.sencon[SENCON_HUM].dead_zone = 2;

    ctrl_inf.sencon[SENCON_CO2].target = 3000;
    ctrl_inf.sencon[SENCON_CO2].alaL = 1;
    ctrl_inf.sencon[SENCON_CO2].alaH = 4000;
    ctrl_inf.sencon[SENCON_CO2].dead_zone = 100;

    ctrl_inf.sencon[SENCON_NH3].target = 10;
    ctrl_inf.sencon[SENCON_NH3].alaL = 1;
    ctrl_inf.sencon[SENCON_NH3].alaH = 20;
    ctrl_inf.sencon[SENCON_NH3].dead_zone = 2;


    ctrl_inf.vac.type = STA_OFF;
    ctrl_inf.vac.targetL = 25;
    ctrl_inf.vac.targetH = 20;
    ctrl_inf.vac.alaL = 10;
    ctrl_inf.vac.alaH = 35;
    ctrl_inf.vac.dead_zone = 3;
    ctrl_inf.vac.delay_s = 10;
    ctrl_inf.vac.make_s = 6;

    ctrl_inf.vac.low_tempdiff = -250;
    ctrl_inf.vac.high_tempdiff = -100;
    ctrl_inf.vac.min_ctrllv = 0;
    ctrl_inf.vac.max_ctrllv = 29;
    ctrl_inf.vac.min_alalv = 0;
    ctrl_inf.vac.adj_move_s = 0;
    ctrl_inf.vac.adj_delay_s = 0;
    ctrl_inf.vac.ala_delay_s = 30;

    ctrl_inf.var.mode = STA_ON;
    ctrl_inf.var.lv_delay = 60;
    ctrl_inf.var.fan_delay = 3;
//    ctrl_inf.var.fan_lvdelay = 0;
    ctrl_inf.var.alarm_delay = 20;
    ctrl_inf.var.temp_diff = 50;
    ctrl_inf.var.temp_diff_ala = 30;
	ctrl_inf.var.shield_ala_time = 30;
    ctrl_inf.var.win_link = 0;
    ctrl_inf.var.food_min = 30;
    ctrl_inf.var.win_link_lv = 2;
    ctrl_inf.var.win_pretime = 0;
    ctrl_inf.var.pwm_fan_defrost_time = 0;

    for (i = 0; i < 10; i++) {
        ctrl_inf.var.swin_defrost_en[i] = 0;
    }

    ctrl_inf.var.swin_hold_time = 0;
    ctrl_inf.var.swin_defrost_degree = 0;

    ctrl_inf.lv_more.lv_fan_cycle_en = STA_OFF;
    ctrl_inf.lv_more.lv_fan_cycle = 300;
    ctrl_inf.lv_more.lv_re_diff_en = STA_ON;
    ctrl_inf.lv_more.lv_re_diff = 1;
    ctrl_inf.lv_more.lv_temp_adj = 0;
    ctrl_inf.lv_more.fan_auto_sort = STA_OFF;

    for (i = 0; i < DAYTEMP_CURVE_NUM; i++) {
        ext_inf.day_temp[i].enable = STA_OFF;
        ext_inf.day_temp[i].grow_day = 1;
        ext_inf.day_temp[i].start_time = 1200;		
        ext_inf.day_temp[i].end_time = 360;	
        ext_inf.day_temp[i].fix_temp = 20; 	
        ext_inf.day_temp[i].fix_times = 4;   	
    }
}


void lv_save_default(void)
{
    int i = 0, j = 0;

    for (i = 0; i < CTRL_LV_NUM; i++) {
        for (j = 0; j < SWINDOW_NUM_MAX; j++) {
            ext_inf.lv_save[i].SWIN[j] = 0;
        }

        j = 0;
        ext_inf.lv_save[i].WIN[j] = 0;
        j++;
        ext_inf.lv_save[i].WIN[j] = 0;
        j++;
        ext_inf.lv_save[i].WIN[j] = 0;
        j++;
        ext_inf.lv_save[i].WIN[j] = 0;
        j++;
    }

}

#define SDEV_DF_ID			(0)
#define SCTRL_DF_ID			(1)
#define SBASIC_DF_ID		(2)
#define SINDEX_DF_ID		(3)
#define SGROUP_DF_ID		(4)
#define SEXT_DF_ID			(5)

void SensorRG_Default(void)
{
    u8 i = 0;

    for (i = 0 ; i < SENSOR_TYPE_NUM; i++) {
        dev_inf.sensor_rg[i].enable = STA_OFF;
        dev_inf.sensor_rg[i].min_value = 0;
        dev_inf.sensor_rg[i].max_value = 100;
    }

    dev_inf.sensor_rg[SENSOR_TYPE_TEMP].min_value = -400;
    dev_inf.sensor_rg[SENSOR_TYPE_TEMP].max_value = 800;
    dev_inf.sensor_rg[SENSOR_TYPE_CO2].enable = STA_ON;

    if (basic_inf.type < ROOM_TYPE_CHICKEN_ID ) {
        dev_inf.sensor_rg[SENSOR_TYPE_CO2].max_value = 5000;
    } else {
        dev_inf.sensor_rg[SENSOR_TYPE_CO2].max_value = 10000;
    }

    dev_inf.sensor_rg[SENSOR_TYPE_VAC].min_value = -100;
    dev_inf.sensor_rg[SENSOR_TYPE_SPEED].max_value = 500;

    EEPROM_Write( EE_DEV_ADDR, (INT8U*)&dev_inf, sizeof(dev_inf));
}

//ID类的信息只初始化一次，不能还原
int Param_Default(INT8U sid)
{
    time_t sys_time;
    unsigned char i = 0, j = 0;
    int		 size;

    switch (sid) {
    case SDEV_DF_ID:
        dev_inf.wMagic = KC_MAGIC;
        memset((unsigned char* )&dev_inf.sensor_adj, 0, sizeof(dev_inf.sensor_adj));
        dev_inf.sensor_adj.TEMP_EN[0] = 1;
        dev_inf.sensor_adj.TEMP_EN[2] = 1;
        dev_inf.sensor_adj.TEMP_EN[4] = 1;

        dev_inf.sensor_adj.SENSOR_EN[0] = SENSOR_TYPE_TEMP;
        dev_inf.sensor_adj.SENSOR_EN[1] = SENSOR_TYPE_HUM;
        dev_inf.sensor_adj.SENSOR_EN[2] = SENSOR_TYPE_NH3;
        dev_inf.sensor_adj.SENSOR_EN[3] = SENSOR_TYPE_CO2;
        dev_inf.sensor_adj.SENSOR_EN[4] = SENSOR_TYPE_VAC;
        dev_inf.sensor_adj.SENSOR_EN[5] = SENSOR_TYPE_H2S;

        memset((unsigned char* )&dev_inf.TEMP_Define, 0, sizeof(dev_inf.TEMP_Define));
        dev_inf.TEMP_Define[0][0] = STA_ON;
        dev_inf.TEMP_Define[0][1] = STA_ON;

        dev_inf.TEMP_Define[1][2] = STA_ON;
        dev_inf.TEMP_Define[1][3] = STA_ON;

        dev_inf.TEMP_Define[2][4] = STA_ON;
        dev_inf.TEMP_Define[2][5] = STA_ON;

        memset((unsigned char* )&dev_inf.dev_equ_show, 0, sizeof(dev_inf.dev_equ_show));
        memset((unsigned char* )&dev_inf.ctrl_equ_show, 0, sizeof(dev_inf.ctrl_equ_show));

        dev_inf.WIN.adjust_open_time = 22 * 60;
        dev_inf.WIN.adjust_close_time = 23 * 60;
        dev_inf.WIN.cal_time = 99;
        dev_inf.WIN.stop_fan_en = 0;
        dev_inf.WIN.adjust_en = STA_ON;
        dev_inf.WIN.min_degree = 2;
        dev_inf.WIN.close_degree = 0;

        for (i = 0 ; i < WINDOW_NUM_MAX ; i++) {
            dev_inf.WIN.detail[i].enable = false;
            dev_inf.WIN.detail[i].mode = WIN_MODE_TIME;
            dev_inf.WIN.detail[i].open_time = 60;
            dev_inf.WIN.detail[i].close_time = 60;
            dev_inf.WIN.detail[i].close_v = 100;
            dev_inf.WIN.detail[i].open_v = 400;
            dev_inf.WIN.detail[i].ctrl_mode = WIN_LV_CTRL;
            dev_inf.WIN.detail[i].prior_ctrl = STA_OFF;
        }

        dev_inf.WIN.detail[3].enable = STA_ON;

        dev_inf.fan_chk.enable = STA_OFF;
        dev_inf.fan_chk.percent = 10;
        dev_inf.fan_chk.offset_cur = 0;
        dev_inf.fan_chk.phase_cur = 0;
        dev_inf.fan_chk.energy_sta = STA_OFF;
        dev_inf.fan_chk.energy_addr[0] = 0x31;
        dev_inf.fan_chk.energy_ct = 1;
        dev_inf.fan_chk.error_cur = 0;

        for (i = 0 ; i < PWM_NUM_MAX ; i++) {
            dev_inf.PWM[i].enable = false;
            dev_inf.PWM[i].type = FAN_36I;
            dev_inf.PWM[i].max_air = 106;
            dev_inf.PWM[i].fan_num = 1;
            dev_inf.PWM[i].min_vol = 0;
        }

        for (i = 0 ; i < JDQ_NUM_MAX ; i++) {
            if (i < 8) {
                dev_inf.JDQ[i].enable = true;
            } else {
                dev_inf.JDQ[i].enable = false;
            }

            dev_inf.JDQ[i].type = FAN_50I;
            dev_inf.JDQ[i].max_air = 380;
            dev_inf.JDQ[i].fan_num = 1;
            dev_inf.JDQ[i].current = 30;
        }

        for (j = 0; j < HEAT_NUM_MAX; j++) {
            basic_inf.heat_en[j] = STA_ON;

            for (i = 0 ; i < HEAT_NUM ; i++) {
                basic_inf.heat[j][i].enable = false;
                basic_inf.heat[j][i].start_time = 0;
                basic_inf.heat[j][i].stop_time = 1439;
                basic_inf.heat[j][i].start_offtemp = -2;
                basic_inf.heat[j][i].stop_offtemp = 2;
                basic_inf.heat[j][i].start_sec = 60;
                basic_inf.heat[j][i].stop_sec = 0;
            }
        }

        basic_inf.pheat_en = STA_OFF;

        for (i = 0 ; i < HEAT_NUM ; i++) {
            basic_inf.pheat[i].enable = false;
            basic_inf.pheat[i].start_time = 0;
            basic_inf.pheat[i].stop_time = 1439;
            basic_inf.pheat[i].start_offtemp = -2;
            basic_inf.pheat[i].stop_offtemp = 2;
            basic_inf.pheat[i].start_freq = 60;
        }

        basic_inf.tempc_en.enable = STA_OFF;
        basic_inf.tempc_en.mode = TEMPC_MODE_HIGH;
        basic_inf.tempc_en.temp_id = TEMPC_TEMPAVG;
        basic_inf.tempc_en.protect_temp = 0;
        basic_inf.tempc_en.start_temp = 400;
        basic_inf.tempc_en.min_ala_temp = 0;
        basic_inf.tempc_en.max_ala_temp = 0;


        for (i = 0 ; i < TEMPC_NUM; i++) {
            basic_inf.tempc[i].enable = STA_OFF;
            basic_inf.tempc[i].start_time = 0;
            basic_inf.tempc[i].stop_time = 1439;
            basic_inf.tempc[i].start_offtemp = 280;
            basic_inf.tempc[i].stop_offtemp = 250;
            basic_inf.tempc[i].start_sec = 60;
            basic_inf.tempc[i].stop_sec = 60;
        }

        basic_inf.tempc_pwm_en.enable = STA_OFF;
        basic_inf.tempc_pwm_en.mode = TEMPC_MODE_HIGH;
        basic_inf.tempc_pwm_en.temp_id = TEMPC_TEMPAVG;
        basic_inf.tempc_pwm_en.protect_temp = 0;
        basic_inf.tempc_pwm_en.start_temp = 400;
        basic_inf.tempc_pwm_en.min_ala_temp = 0;
        basic_inf.tempc_pwm_en.max_ala_temp = 0;


        for (i = 0 ; i < TEMPC_NUM; i++) {
            basic_inf.tempc_pwm[i].enable = STA_OFF;
            basic_inf.tempc_pwm[i].start_time = 0;
            basic_inf.tempc_pwm[i].stop_time = 1439;
            basic_inf.tempc_pwm[i].start_offtemp = 280;
            basic_inf.tempc_pwm[i].stop_offtemp = 250;
            basic_inf.tempc_pwm[i].start_freq = 60;
        }

        basic_inf.heat[0][0].enable = STA_ON;
        basic_inf.heat[1][0].enable = STA_ON;
        basic_inf.heat[2][0].enable = STA_ON;

        for (i = 0 ; i < APWM_NUM_MAX; i++) {
            basic_inf.area_pwm[i].enable = STA_OFF;
            basic_inf.area_pwm[i].target_temp = 220;
            basic_inf.area_pwm[i].start_sec = 100;
            basic_inf.area_pwm[i].stop_sec = 0;
            basic_inf.area_pwm[i].min_vol = 4;

            for (j = 0; j < FREQ_NUM; j++) {
                basic_inf.area_pwm[i].freq[j].enable = STA_OFF;
                basic_inf.area_pwm[i].freq[j].offset = 0;
                basic_inf.area_pwm[i].freq[j].freq = 0;
            }
        }

        for (i = 0 ; i < SWINDOW_NUM_MAX ; i++) {
            dev_inf.SWIN.detail[i].enable = false;
            dev_inf.SWIN.detail[i].open_time = 60;
            dev_inf.SWIN.detail[i].close_time = 60;
            dev_inf.SWIN.detail[i].ctrl_mode = WIN_LV_CTRL;
            dev_inf.SWIN.detail[i].prior_ctrl = STA_OFF;
        }

        dev_inf.SWIN.detail[4].enable = STA_ON;

        for (j = 0; j < WET_NUM_MAX; j++) {
            dev_inf.wet_en[j] = 0;

            for (i = 0 ; i < WET_NUM ; i++) {
                dev_inf.wet[j][i].enable = false;
                dev_inf.wet[j][i].start_time = 0;
                dev_inf.wet[j][i].stop_time = 1439;
                dev_inf.wet[j][i].start_offtemp = 50;
                dev_inf.wet[j][i].stop_offtemp = 40;
                dev_inf.wet[j][i].max_hum = 80;
                dev_inf.wet[j][i].start_sec = 30;
                dev_inf.wet[j][i].stop_sec = 600;
            }
        }

        dev_inf.wet_en[0] = STA_ON;
        dev_inf.wet[0][0].enable = true;
        dev_inf.spray_en = SPRAY_MODE_OFF;

        for (i = 0 ; i < SPRAY_NUM ; i++) {
            dev_inf.spray[i].enable = true;
            dev_inf.spray[i].start_time = 0;
            dev_inf.spray[i].stop_time = 1439;
            dev_inf.spray[i].start_hum = 40;
            dev_inf.spray[i].stop_hum = 60;
            dev_inf.spray[i].min_offtemp = 20;
            dev_inf.spray[i].start_sec = 30;
            dev_inf.spray[i].stop_sec = 60;
        }

        dev_inf.light.enable = STA_OFF;
        dev_inf.light.delay_ontime = 30;
        dev_inf.light.delay_offtime = 30;

        for (i = 0 ; i < LIGHT_NUM ; i++) {
            dev_inf.light.curve[i].enable = STA_OFF;
            dev_inf.light.curve[i].grow_day = 20;
            dev_inf.light.curve[i].start_time = 0;
            dev_inf.light.curve[i].stop_time = 1439;
            dev_inf.light.curve[i].value = 50;
        }

        dev_inf.circle.enable = STA_OFF;
        dev_inf.circle.offset = 0;
        dev_inf.circle.start_time = 0;
        dev_inf.circle.stop_time = 1439;
        dev_inf.circle.start_lv = 3;
        dev_inf.circle.stop_lv = 10;

        for (i = 0 ; i < TEMP_NUM_MAX ; i++) {
            dev_inf.circle.temp_sta[i] = STA_OFF;
        }

        for (j = 0; j < TR_TYPE_NUM; j++) {
            ext_inf.timer[j].enable = STA_OFF;
            ext_inf.timer[j].type = TIMER_TYPE_NONE;
            ext_inf.timer[j].back_time = 6;
            ext_inf.timer[j].mode = TIMER_MODE_CYCLE;
            ext_inf.timer[j].start_min = 10;
            ext_inf.timer[j].stop_min = 10;

            for (i = 0 ; i < TIMER_NUM ; i++) {
                ext_inf.timer[j].num_en[i] = STA_OFF;
                ext_inf.timer[j].start_time[i] = 0;
                ext_inf.timer[j].stop_time[i] = 1439;
                ext_inf.timer[j].delay_min[i] = 10;
            }
        }

        for (i = 0 ; i < MB_DEV_MAX; i++) {
            dev_inf.env_dev[i].enable = STA_OFF;
            dev_inf.env_dev[i].addr = 1;
        }

        dev_inf.limit.pm2_5_H = 75;
        dev_inf.limit.pm10_H = 100;

        EEPROM_Write( EE_DEV_ADDR, (INT8U*)&dev_inf, sizeof(dev_inf));
        break;

    case SEXT_DF_ID:
        ext_inf.wMagic = KC_MAGIC;

        memset((unsigned char* )&ext_inf.din, 0, sizeof(ext_inf.din));

        for (i = 0 ; i < PRO_DATA_MAX ; i++) {
            ext_inf.din.pro_data[i].energy_sta = PL_DIS;
            ext_inf.din.pro_data[i].energy_addr[0] = 0x31;
            ext_inf.din.pro_data[i].energy_ct = 1;

            ext_inf.din.pro_data[i].water_unit = 10;
            ext_inf.din.pro_data[i].water_addr[0] = 0x32;
            ext_inf.din.pro_data[i].food_unit = 10;
            ext_inf.din.pro_data[i].food_addr = 1;
        }

        for (i = 0 ; i < MB_DATA_MAX ; i++) {
            ext_inf.din.mb_data[i].ux.port = 0;
            ext_inf.din.mb_data[i].ux.dbits = UDATA_8B;
            ext_inf.din.mb_data[i].ux.stopb = STOP_BIT1;
            ext_inf.din.mb_data[i].ux.parity = PARITY_NONE;

            ext_inf.din.mb_data[i].baud = 3;
            ext_inf.din.mb_data[i].reg_addr = 1;
            ext_inf.din.mb_data[i].dx.len = 4;
            ext_inf.din.mb_data[i].dx.point = 0;
            ext_inf.din.mb_data[i].dx.mem = 1;
            ext_inf.din.mb_data[i].dx.encode = 0;
        }

        ext_inf.din.leakage_enable = STA_OFF;		//是否启用 1-使用
        ext_inf.din.leakage_max_flow = 100;			//最大流量
        ext_inf.din.leakage_delay_min = 5;			//报警延时
        ext_inf.din.lack_enable = STA_OFF;			//是否启用 1-使用
        ext_inf.din.lack_min_flow = 10;				//最小流量
        ext_inf.din.lack_delay_min = 10;			//报警延时

        EEPROM_Write( EE_EXT_ADDR, (INT8U*)&ext_inf, sizeof(ext_inf));
        break;

    case SCTRL_DF_ID:
        //ctrl_inf 结构体
        ctrl_inf.wMagic = KC_MAGIC;
        ctrl_inf_temp_init();
        ctrl_inf_air_init();

        if (basic_inf.type != ROOM_CHICKEN_MEAT && basic_inf.type != ROOM_CHICKEN_BROOD
                && basic_inf.type != ROOM_DUCK_MEAT) {
            ctrl_inf_lv_init();
        }	else {
            ctrl_inf_lv_brood_init();
        }

        ctrl_inf_lv_assist_init();

        lv_save_default();
        Config_Save(SLV_SAVE_ID);
        Config_Save(SLVPWM_ID);
        size = (sizeof(ctrl_inf.lv) + sizeof(ctrl_inf.wMagic));
        EEPROM_Write( (EE_CTRL_ADDR + size), (INT8U*)&ctrl_inf.temp_adj, (sizeof(ctrl_inf) - size) );
        break;

    case SBASIC_DF_ID:
        //basic_inf 结构体
        basic_inf.wMagic = KC_MAGIC;
        basic_inf.grow_day = 0;
        sys_sta.update_hour = MAX_1BYTE;
        sys_time = GetSysTime();
        basic_inf.group_time = sys_time - (sys_time % DAY_TO_SEC) + DAY_TO_SEC;
        basic_inf.data_time = sys_time - (sys_time % DAY_TO_SEC) + DAY_TO_SEC;
        basic_inf.update_mode = UPDATA_MASTER;
        basic_inf.update_time = 1;
        basic_inf.dayupdate_time = 510;
        basic_inf.dataupdate_time = 510;
        strcpy(basic_inf.supadmin_pwd, "123456");
        strcpy(basic_inf.admin_pwd, "123123");
        basic_inf.survival = 0;
        basic_inf.total = 0;
        basic_inf.ala_en = STA_ON;
        basic_inf.ala_test.enable = STA_OFF;
        basic_inf.ala_test.time = 510;
        basic_inf.ala_test.hold_time = 10;
        basic_inf.ala_open_mode = STA_OFF;
        basic_inf.humtemp_en = STA_OFF;
        basic_inf.wind_area = 0;
        basic_inf.ss_time = 300;
        basic_inf.home_style = HOME_HOUSE;
        basic_inf.touch_beep = STA_ON;
        basic_inf.cur_season = DATA_FACTORY;


        basic_inf.comm_info.ux.port = 1;
        basic_inf.comm_info.ux.dbits = UDATA_8B;
        basic_inf.comm_info.ux.stopb = STOP_BIT1;
        basic_inf.comm_info.ux.parity = PARITY_NONE;
        basic_inf.comm_info.baud = 7;


        EEPROM_Write( EE_BASIC_ADDR, (INT8U*)&basic_inf, sizeof(basic_inf));
        break;

    case SINDEX_DF_ID:
        index_inf.wMagic = KC_MAGIC;
        index_inf.ala_id = 0;
        index_inf.ala_state = 0;
        index_inf.data_id = 0;
        index_inf.data_state = 0;
        index_inf.his_buf_id = 0;
        index_inf.his_id = 0;
        index_inf.his_state = 0;

        index_inf.data_pro_id = 0;
        index_inf.data_pro_state = 0;

        for (i = 0; i <= PRO_DATA_MAX; i++) {
            memset(&index_inf.animal, 0, sizeof(index_inf.animal));
            memset(&index_inf.water[0], 0, sizeof(index_inf.water));
            memset(&index_inf.food[0], 0, sizeof(index_inf.food));
            memset(&index_inf.energy[0][0], 0, sizeof(index_inf.energy));
            memset(&index_inf.addfood[0], 0, sizeof(index_inf.addfood));
            memset(&index_inf.egg[0], 0, sizeof(index_inf.egg));
        }

        index_inf.start_time = 0;
        index_inf.run_time = 0;
        index_inf.restart_time = 0;
        EEPROM_Write( EE_INDEX_ADDR, (INT8U*)&index_inf, sizeof(index_inf));
        break;

    case SGROUP_DF_ID:
        index_inf.ala_id = 0;
        index_inf.ala_state = 0;
        index_inf.data_id = 0;
        index_inf.data_state = 0;
        index_inf.his_buf_id = 0;
        index_inf.his_id = 0;
        index_inf.his_state = 0;

        index_inf.data_pro_id = 0;
        index_inf.data_pro_state = 0;

        for (i = 0; i <= PRO_DATA_MAX; i++) {
            memset(&index_inf.animal, 0, sizeof(index_inf.animal));
            memset(&index_inf.water[0], 0, sizeof(index_inf.water));
            memset(&index_inf.food[0], 0, sizeof(index_inf.food));
            memset(&index_inf.energy[0][0], 0, sizeof(index_inf.energy));
            memset(&index_inf.addfood[0], 0, sizeof(index_inf.addfood));
            memset(&index_inf.egg[0], 0, sizeof(index_inf.egg));
        }

        EEPROM_Write( EE_INDEX_ADDR, (INT8U*)&index_inf, sizeof(index_inf));
        break;

    default:
        break;
    }

    return 1;
}

//在编译的时候如果condition为真就会出错
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
void Param_Init(void)
{
    unsigned char ret;

    //检查结构体大小
    BUILD_BUG_ON(sizeof(TDEV) > 1024);
    BUILD_BUG_ON(sizeof(TBASIC) > 960);
    BUILD_BUG_ON(sizeof(TCTRL) > 7168);
    BUILD_BUG_ON(sizeof(TEXT) > 1024);
    BUILD_BUG_ON(sizeof(TINDEX) > 2048);

    if (EE_MAX_ADDR >= EE_CLOCK_ADDR) {
        while (1);
    }
	
	if (SDRAM_GBK32_ADDR + FONT32_SIZE  >= SDRAM_BANK_ADDR + 7 * 1024 * 1024) {
        while (1);
    }


    memset( (unsigned char* )&sta_in, 0, sizeof(sta_in) );
    memset( (unsigned char* )&sta_out, 0, sizeof(sta_out) );
    memset( (unsigned char* )&cal_inf, 0, sizeof(cal_inf) );
    memset( (unsigned char* )&sys_sta, 0, sizeof(sys_sta) );
    memset( (unsigned char* )&run_inf, 0, sizeof(run_inf) );
    memset( (unsigned char* )&recode_cur, 0, sizeof(recode_cur) );
    memset( (unsigned char* )&use_data_cur, 0, sizeof(use_data_cur) );
    memset( (unsigned char* )&ala_inf, 0, sizeof(ala_inf) );


    //读取存储的前一个报警
    if (index_inf.ala_id != 0) {
        Read_Recode_Data((U8*)&recode_cur.ala, RECODE_ALA, (index_inf.ala_id - 1));
    } else if (index_inf.ala_state == 1) {
        Read_Recode_Data((U8*)&recode_cur.ala, RECODE_ALA, (ALA_RECORD_MAX - 1));
    }

    Cal_Temp_Def();
    run_inf.WIN_First[0] = STA_ON;//上电自动校准一次
    run_inf.WIN_First[1] = STA_ON;//上电自动校准一次
    run_inf.WIN_First[2] = STA_ON;//上电自动校准一次
    run_inf.WIN_First[3] = STA_ON;//上电自动校准一次

    sta_out.WIN[0] = WIN_DEF_VA;//上电的默认值错误为了开始就调整一次
    sta_out.WIN[1] = WIN_DEF_VA;//上电的默认值错误为了开始就调整一次
    sta_out.WIN[2] = WIN_DEF_VA;//上电的默认值错误为了开始就调整一次
    sta_out.WIN[3] = WIN_DEF_VA;//上电的默认值错误为了开始就调整一次
    cal_inf.old_lv = LV_DEF_VA; //第一次必须重新计算
    run_inf.old_adj_hum = MAX_1BYTE;//第一次必须重新计算

    sys_sta.lcd_state = 1;
    sys_sta.auto_mode = STA_ON;

    if (strcmp(basic_inf.supadmin_pwd, PWD_NO_KEY) != 0) {
        sys_sta.login_type = LOGIN_GUEST;
    } else {
        sys_sta.login_type = LOGIN_SUPADMIN;
    }

    sys_sta.net_sta = STA_OFF;
    sys_sta.net_creg = STA_OFF;


    index_inf.start_time++;

    ret = index_inf.start_time % DATA_RECORD_MAX;
    index_inf.start_t[ret] = GetSysTime();

    Config_Save(SINDEX_ID);

// 检查升级程序
    SPI_Flash_Read((INT8U *)&update_st.wMagic, update_init_addr, sizeof(update_st));

    if (update_st.wMagic != KC_MAGIC) {
        update_st.wMagic = KC_MAGIC;
        update_st.sta = UP_END;
        update_st.type = UP_VER_RUN;
        update_st.len = 0;
    }

    update_st.type = UP_VER_RUN;
    Config_Save(SSOFT_INIT_ID);

    sys_sta.update_hour = MAX_1BYTE;
    basic_inf.home_style = HOME_HOUSE;

    JDQ_Control(JDQ_ALA, STA_OFF);//关闭报警

    Packet_Init();
}

void basic_inf_check(void)
{
    //basic_inf 结构体
    if (basic_inf.wMagic != KC_MAGIC)  {
        Param_Default(SBASIC_DF_ID);
    }

    if (soft_inf.room_mode == ROOM_MODE_SOW) {
        if (basic_inf.type > ROOM_TYPE_CHICKEN_ID) {
            basic_inf.type = ROOM_TYPE_FATTEN;
        }
    } else {
        if (basic_inf.type < ROOM_TYPE_CHICKEN_ID || basic_inf.type >= ROOM_TYPE_NUM) {
            basic_inf.type = ROOM_CHICKEN_MEAT;
        }
    }

    if (soft_inf.update_pro > PRO_HL) {
		soft_inf.update_pro = PRO_HL;
    }
    soft_inf.update_pro = PRO_KC;
    
    if (basic_inf.room > ROOM_MAX) {
        basic_inf.room = 1;
    }

    if (basic_inf.id > 65535) {
        basic_inf.id = 1;
    }

    if (basic_inf.group > MAX_GROUP_NUM) {
        basic_inf.group = 1;
    }

    if (basic_inf.update_mode > UPDATA_SLAVE) {
        basic_inf.update_mode = UPDATA_MASTER;
    }

    if (basic_inf.update_time > 255) {
        basic_inf.update_time = 1;
    }

    if (basic_inf.dayupdate_time > (MAX_TIME_DAY - 29)) {
        basic_inf.dayupdate_time = 510;
    }

    if (basic_inf.dataupdate_time > (MAX_TIME_DAY - 29)) {
        basic_inf.dataupdate_time = 510;
    }

    if (basic_inf.ala_test.enable > STA_ON) {
        basic_inf.ala_test.enable = STA_OFF;
    }

    if (basic_inf.ala_test.time > MAX_TIME_DAY) {
        basic_inf.ala_test.time = 510;
    }

    if (basic_inf.ala_test.hold_time > TIME_3BIT) {
        basic_inf.ala_test.hold_time = 10;
    }

    if (basic_inf.ala_open_mode > STA_ON) {
        basic_inf.ala_open_mode = STA_OFF;
    }

    if (basic_inf.humtemp_en > STA_ON) {
        basic_inf.humtemp_en = STA_OFF;
    }

    if (basic_inf.wind_area > TIME_4BIT) {
        basic_inf.wind_area = 0;
    }

    if (basic_inf.ss_time > TIME_3BIT) {
        basic_inf.ss_time = 300;
    }

    if (basic_inf.home_style > HOME_CIRCLE) {
        basic_inf.home_style = HOME_HOUSE;
    }

    if (basic_inf.touch_beep > STA_ON) {
        basic_inf.touch_beep = STA_ON;
    }

    if (Language_Bit_original > LANGUAGE_NUM - 1) {
        Language_Bit_original = LANGUAGE_GBK;
    }

    if (basic_inf.cur_season > DATA_WINTER) {
        basic_inf.cur_season = DATA_FACTORY;
    }

    if (basic_inf.comm_info.ux.port > 1)basic_inf.comm_info.ux.port = 1;

    if (basic_inf.comm_info.ux.dbits > UDATA_8B)basic_inf.comm_info.ux.dbits = UDATA_8B;

    if (basic_inf.comm_info.ux.stopb > STOP_BIT2)basic_inf.comm_info.ux.stopb = STOP_BIT1;

    if (basic_inf.comm_info.ux.parity > PARITY_EVEN)basic_inf.comm_info.ux.parity = PARITY_NONE;

    if (basic_inf.comm_info.baud > 7)basic_inf.comm_info.baud = 7;

}

extern short sensor_max[SENSOR_TYPE_NUM];
extern short sensor_min[SENSOR_TYPE_NUM];
void dev_inf_check(void)
{
    unsigned char i = 0, j = 0;

    if (dev_inf.wMagic != KC_MAGIC) {
        Param_Default(SDEV_DF_ID);
        SensorRG_Default();
    }

    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if ((dev_inf.sensor_adj.TEMP[i] < TEMP_ADJMIN) || (dev_inf.sensor_adj.TEMP[i] > TEMP_ADJMAX)) dev_inf.sensor_adj.TEMP[i] = 0;

        if (dev_inf.sensor_adj.TEMP_EN[i] > true)  dev_inf.sensor_adj.TEMP_EN[i] = 0;
    }

    for (i = 0; i < SENSOR_NUM_MAX; i++) {
        if (dev_inf.sensor_adj.SENSOR_EN[i] >= SENSOR_TYPE_NUM)  dev_inf.sensor_adj.SENSOR_EN[i] = SENSOR_TYPE_DIS;

        if (dev_inf.sensor_adj.SENSOR_EN[i] == SENSOR_TYPE_DIS) {
            dev_inf.sensor_adj.SENSOR[i] = 0;
            continue;
        }

        if ((dev_inf.sensor_adj.SENSOR[i] < sensor_min[dev_inf.sensor_adj.SENSOR_EN[i]]) || (dev_inf.sensor_adj.SENSOR[i] > sensor_max[dev_inf.sensor_adj.SENSOR_EN[i]])) dev_inf.sensor_adj.SENSOR[i] = 0;

    }

    for (i = 0; i < DEV_ITEM_NUM; i++) {
        if (dev_inf.dev_equ_show[i] > STA_ON)  dev_inf.dev_equ_show[i] = STA_OFF;
    }

    for (i = 0; i < CTRL_ITEM_NUM; i++) {
        if (dev_inf.ctrl_equ_show[i] > STA_ON)  dev_inf.ctrl_equ_show[i] = STA_OFF;
    }

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        for (j = 0; j < TEMP_NUM_MAX; j++) {
            if (dev_inf.TEMP_Define[i][j] > STA_ON) dev_inf.TEMP_Define[i][j] = STA_OFF;
        }
    }

    if (dev_inf.WIN.adjust_open_time >= DAY_TO_MIN) dev_inf.WIN.adjust_open_time = 22 * 60;

    if (dev_inf.WIN.adjust_close_time >= DAY_TO_MIN) dev_inf.WIN.adjust_close_time = 23 * 60;

    if (dev_inf.WIN.stop_fan_en > true) dev_inf.WIN.stop_fan_en = 0;

    if (dev_inf.WIN.adjust_en > STA_ON) dev_inf.WIN.adjust_en = STA_OFF;

    if (dev_inf.WIN.min_degree <= 0 || dev_inf.WIN.min_degree > 10) dev_inf.WIN.min_degree = 2;

    if (dev_inf.WIN.close_degree > 50) dev_inf.WIN.close_degree = 0;

    for (i = 0 ; i < WINDOW_NUM_MAX ; i++) {
        if (dev_inf.WIN.detail[i].enable > true)  dev_inf.WIN.detail[i].enable = false;

        if (dev_inf.WIN.detail[i].close_v > 1000) dev_inf.WIN.detail[i].close_v = 100;

        if (dev_inf.WIN.detail[i].open_v > 1000) dev_inf.WIN.detail[i].open_v = 400;

        if (dev_inf.WIN.detail[i].mode > WIN_MODE_POS) dev_inf.WIN.detail[i].mode = WIN_MODE_TIME;

//        if (dev_inf.WIN.detail[i].open_time < 1) dev_inf.WIN.detail[i].open_time = 60;

//        if (dev_inf.WIN.detail[i].close_time < 1) dev_inf.WIN.detail[i].close_time = 60;

        if (dev_inf.WIN.detail[i].ctrl_mode > WIN_ONLY_VAC_CTRL) dev_inf.WIN.detail[i].ctrl_mode = WIN_LV_CTRL;

        if (dev_inf.WIN.detail[i].prior_ctrl > STA_ON) dev_inf.WIN.detail[i].prior_ctrl = STA_OFF;
    }

    if (dev_inf.fan_chk.enable > STA_ON)  	dev_inf.fan_chk.enable = STA_OFF;

    if (dev_inf.fan_chk.percent > MAX_PERCENT)  	dev_inf.fan_chk.percent = 10;

    if (dev_inf.fan_chk.energy_sta > PL_NUM)  	dev_inf.fan_chk.energy_sta = PL_DIS;

    if (dev_inf.fan_chk.energy_ct <= 0)  	dev_inf.fan_chk.energy_ct = 1;

    if (dev_inf.fan_chk.error_cur > TIME_2BIT)  	dev_inf.fan_chk.error_cur = 0;

    for (i = 0 ; i < PWM_NUM_MAX ; i++) {
        if (dev_inf.PWM[i].enable > true)  		dev_inf.PWM[i].enable = false;

        if (dev_inf.PWM[i].type > FAN_SIZE_NUM) dev_inf.PWM[i].type = FAN_18I;

        if (dev_inf.PWM[i].max_air > AIR_MAX) 	dev_inf.PWM[i].max_air = 10;

        if (dev_inf.PWM[i].fan_num > TIME_1BIT || dev_inf.PWM[i].fan_num == 0) 	dev_inf.PWM[i].fan_num = 1;

        if (dev_inf.PWM[i].min_vol > VOL_MAX) 	dev_inf.PWM[i].min_vol = 0;
    }

    for (i = 0 ; i < JDQ_NUM_MAX ; i++) {
        if (dev_inf.JDQ[i].enable > true) dev_inf.JDQ[i].enable = false;

        if (dev_inf.JDQ[i].type > FAN_SIZE_NUM) dev_inf.JDQ[i].type = FAN_18I;

        if (dev_inf.JDQ[i].max_air > AIR_MAX) dev_inf.JDQ[i].max_air = 10;

        if (dev_inf.JDQ[i].fan_num > TIME_1BIT || dev_inf.JDQ[i].fan_num == 0) dev_inf.JDQ[i].fan_num = 1;
    }

    for (j = 0; j < HEAT_NUM_MAX; j++) {

        if (basic_inf.heat_en[j] > true) basic_inf.heat_en[j] = false;

        for (i = 0 ; i < HEAT_NUM ; i++) {
            if (basic_inf.heat[j][i].enable > true) basic_inf.heat[j][i].enable = false;

            if (basic_inf.heat[j][i].start_time > MAX_TIME_DAY) basic_inf.heat[j][i].start_time = 0;

            if (basic_inf.heat[j][i].stop_time > MAX_TIME_DAY) basic_inf.heat[j][i].stop_time = 1439;

//            if (basic_inf.heat[j][i].start_offtemp > TEMP_OFFSET_MAX) basic_inf.heat[j][i].start_offtemp = 20;

//            if (basic_inf.heat[j][i].stop_offtemp > TEMP_OFFSET_MAX) basic_inf.heat[j][i].stop_offtemp = 20;

            if (basic_inf.heat[j][i].start_sec > TIME_3BIT) basic_inf.heat[j][i].start_sec = 100;

            if (basic_inf.heat[j][i].stop_sec > TIME_3BIT) basic_inf.heat[j][i].stop_sec = 100;
        }
    }

    if (basic_inf.pheat_en > true) basic_inf.pheat_en = false;

    for (i = 0 ; i < HEAT_NUM ; i++) {
        if (basic_inf.pheat[i].enable > true) basic_inf.pheat[i].enable = false;

        if (basic_inf.pheat[i].start_time > MAX_TIME_DAY) basic_inf.pheat[i].start_time = 0;

        if (basic_inf.pheat[i].stop_time > MAX_TIME_DAY) basic_inf.pheat[i].stop_time = 1439;

        if (basic_inf.pheat[i].start_freq > MAX_DEGREE) basic_inf.pheat[i].start_freq = 60;
    }

    if (basic_inf.tempc_en.enable > true) basic_inf.tempc_en.enable = 0;

    if (basic_inf.tempc_en.mode > TEMPC_MODE_ECIDF) basic_inf.tempc_en.mode = TEMPC_MODE_HIGH;

    if (basic_inf.tempc_en.temp_id > TEMPC_TEMP6) basic_inf.tempc_en.temp_id = TEMPC_TEMPAVG;

    if (basic_inf.tempc_en.protect_temp > TEMP_MAX) basic_inf.tempc_en.protect_temp = 0;

    if (basic_inf.tempc_en.start_temp > TEMP_MAX) basic_inf.tempc_en.start_temp = 400;

    if (basic_inf.tempc_en.min_ala_temp > TEMP_MAX) basic_inf.tempc_en.min_ala_temp = 0;

    if (basic_inf.tempc_en.max_ala_temp > TEMP_MAX) basic_inf.tempc_en.max_ala_temp = 0;


    for (i = 0 ; i < TEMPC_NUM; i++) {
        if (basic_inf.tempc[i].enable > true) basic_inf.tempc[i].enable = false;

        if (basic_inf.tempc[i].start_time > MAX_TIME_DAY) basic_inf.tempc[i].start_time = 0;

        if (basic_inf.tempc[i].stop_time > MAX_TIME_DAY) basic_inf.tempc[i].stop_time = 1439;

        if (basic_inf.tempc[i].start_offtemp > TEMP_MAX || basic_inf.tempc[i].start_offtemp < TEMP_MIN) basic_inf.tempc[i].start_offtemp = 280;

        if (basic_inf.tempc[i].stop_offtemp > TEMP_MAX || basic_inf.tempc[i].stop_offtemp < TEMP_MIN) basic_inf.tempc[i].stop_offtemp = 250;

        if (basic_inf.tempc[i].start_sec > TIME_3BIT) basic_inf.tempc[i].start_sec = 100;

        if (basic_inf.tempc[i].stop_sec > TIME_3BIT) basic_inf.tempc[i].stop_sec = 100;
    }

    if (basic_inf.tempc_pwm_en.enable > true) basic_inf.tempc_pwm_en.enable = 0;

    if (basic_inf.tempc_pwm_en.mode > TEMPC_MODE_ECIDF) basic_inf.tempc_pwm_en.mode = TEMPC_MODE_HIGH;

    if (basic_inf.tempc_pwm_en.temp_id > TEMPC_TEMP6) basic_inf.tempc_pwm_en.temp_id = TEMPC_TEMPAVG;

    if (basic_inf.tempc_pwm_en.protect_temp > TEMP_MAX) basic_inf.tempc_pwm_en.protect_temp = 0;

    if (basic_inf.tempc_pwm_en.start_temp > TEMP_MAX) basic_inf.tempc_pwm_en.start_temp = 400;

    if (basic_inf.tempc_pwm_en.min_ala_temp > TEMP_MAX) basic_inf.tempc_pwm_en.min_ala_temp = 0;

    if (basic_inf.tempc_pwm_en.max_ala_temp > TEMP_MAX) basic_inf.tempc_pwm_en.max_ala_temp = 400;

    for (i = 0 ; i < TEMPC_NUM; i++) {
        if (basic_inf.tempc_pwm[i].enable > true) basic_inf.tempc_pwm[i].enable = false;

        if (basic_inf.tempc_pwm[i].start_time > MAX_TIME_DAY) basic_inf.tempc_pwm[i].start_time = 0;

        if (basic_inf.tempc_pwm[i].stop_time > MAX_TIME_DAY) basic_inf.tempc_pwm[i].stop_time = 1439;

        if (basic_inf.tempc_pwm[i].start_offtemp > TEMP_MAX || basic_inf.tempc_pwm[i].start_offtemp < TEMP_MIN) basic_inf.tempc_pwm[i].start_offtemp = 280;

        if (basic_inf.tempc_pwm[i].stop_offtemp > TEMP_MAX || basic_inf.tempc_pwm[i].stop_offtemp < TEMP_MIN) basic_inf.tempc_pwm[i].stop_offtemp = 250;

        if (basic_inf.tempc_pwm[i].start_freq > MAX_PERCENT) basic_inf.tempc_pwm[i].start_freq = 100;

    }

    for (i = 0 ; i < APWM_NUM_MAX; i++) {
        if (basic_inf.area_pwm[i].enable > true) basic_inf.area_pwm[i].enable = false;

        if ((basic_inf.area_pwm[i].target_temp > TEMP_MAX)) basic_inf.area_pwm[i].target_temp = 220;

        if (basic_inf.area_pwm[i].start_sec > TIME_3BIT) basic_inf.area_pwm[i].start_sec = 100;

        if (basic_inf.area_pwm[i].stop_sec > TIME_3BIT) basic_inf.area_pwm[i].stop_sec = 100;

        if (basic_inf.area_pwm[i].min_vol > VOL_MAX) basic_inf.area_pwm[i].min_vol = 4;

        for (j = 0; j < FREQ_NUM; j++) {
            if (basic_inf.area_pwm[i].freq[j].enable > true) basic_inf.area_pwm[i].freq[j].enable = false;

            if ((basic_inf.area_pwm[i].freq[j].offset > TEMP_OFFSET_MAX) || (basic_inf.area_pwm[i].freq[j].offset < TEMP_OFFSET_MIN)) basic_inf.area_pwm[i].freq[j].offset = 0;

            if (basic_inf.area_pwm[i].freq[j].freq > MAX_PERCENT) basic_inf.area_pwm[i].freq[j].freq = MAX_PERCENT;
        }
    }

    for (i = 0 ; i < SWINDOW_NUM_MAX ; i++) {
        if (dev_inf.SWIN.detail[i].enable > true) dev_inf.SWIN.detail[i].enable = 0;

        if (dev_inf.SWIN.detail[i].open_time < 1) dev_inf.SWIN.detail[i].open_time = 60;

        if (dev_inf.SWIN.detail[i].close_time < 1) dev_inf.SWIN.detail[i].close_time = 60;

        if (dev_inf.SWIN.detail[i].ctrl_mode > WIN_VAC_CTRL) dev_inf.SWIN.detail[i].ctrl_mode = WIN_LV_CTRL;

        if (dev_inf.SWIN.detail[i].prior_ctrl > STA_ON) dev_inf.SWIN.detail[i].prior_ctrl = STA_OFF;
    }


    for (j = 0; j < WET_NUM_MAX; j++) {

        if (dev_inf.wet_en[j] > true) dev_inf.wet_en[j] = false;

        for (i = 0 ; i < WET_NUM ; i++) {
            if (dev_inf.wet[j][i].enable > true) dev_inf.wet[j][i].enable = false;

            if (dev_inf.wet[j][i].start_time > MAX_TIME_DAY) dev_inf.wet[j][i].start_time = 100;

            if (dev_inf.wet[j][i].stop_time > MAX_TIME_DAY) dev_inf.wet[j][i].stop_time = 100;

            if (dev_inf.wet[j][i].start_offtemp > TEMP_OFFSET_MAX) dev_inf.wet[j][i].start_offtemp = 20;

            if (dev_inf.wet[j][i].stop_offtemp > TEMP_OFFSET_MAX) dev_inf.wet[j][i].stop_offtemp = 20;

            if (dev_inf.wet[j][i].max_hum > HUM_MAX) dev_inf.wet[j][i].max_hum = 80;

            if (dev_inf.wet[j][i].start_sec > TIME_4BIT) dev_inf.wet[j][i].start_sec = 100;

            if (dev_inf.wet[j][i].stop_sec > TIME_4BIT) dev_inf.wet[j][i].stop_sec = 100;
        }
    }

    if (dev_inf.spray_en > SPRAY_MODE_TEMP) dev_inf.spray_en = SPRAY_MODE_OFF;

    for (i = 0 ; i < SPRAY_NUM ; i++) {
        if (dev_inf.spray[i].enable > true) dev_inf.spray[i].enable = true;

        if (dev_inf.spray[i].start_time > MAX_TIME_DAY) dev_inf.spray[i].start_time = 100;

        if (dev_inf.spray[i].stop_time > MAX_TIME_DAY) dev_inf.spray[i].stop_time = 100;

        if (dev_inf.spray[i].start_hum > HUM_MAX) dev_inf.spray[i].start_hum = 20;

        if (dev_inf.spray[i].stop_hum > HUM_MAX) dev_inf.spray[i].stop_hum = 80;

        if (dev_inf.spray[i].min_offtemp < TEMP_ADJMIN || dev_inf.spray[i].min_offtemp > TEMP_ADJMAX) dev_inf.spray[i].min_offtemp = 20;

        if (dev_inf.spray[i].start_sec > TIME_4BIT) dev_inf.spray[i].start_sec = 100;

        if (dev_inf.spray[i].stop_sec > TIME_4BIT) dev_inf.spray[i].stop_sec = 100;
    }

    if (dev_inf.light.enable > STA_ON) dev_inf.light.enable = STA_OFF;

    if (dev_inf.light.delay_ontime > TIME_4BIT || dev_inf.light.delay_ontime == 0) dev_inf.light.delay_ontime = 30;

    if (dev_inf.light.delay_offtime > TIME_4BIT || dev_inf.light.delay_offtime == 0) dev_inf.light.delay_offtime = 30;

    for (i = 0 ; i < LIGHT_NUM ; i++) {
        if (dev_inf.light.curve[i].enable > STA_ON) dev_inf.light.curve[i].enable = STA_OFF;

        if (dev_inf.light.curve[i].grow_day > DAY_MAX ) dev_inf.light.curve[i].grow_day = 2 * i;

        if (dev_inf.light.curve[i].start_time > MAX_TIME_DAY) dev_inf.light.curve[i].start_time = 0;

        if (dev_inf.light.curve[i].stop_time > MAX_TIME_DAY) dev_inf.light.curve[i].stop_time = 1439;

        if (dev_inf.light.curve[i].value > MAX_PERCENT) dev_inf.light.curve[i].value = 50;

    }


    if (dev_inf.circle.enable > STA_ON) dev_inf.circle.enable = STA_OFF;

    if (dev_inf.circle.offset > TEMP_OFFSET_MAX) dev_inf.circle.offset = 20;

    if (dev_inf.circle.start_time > MAX_TIME_DAY) dev_inf.circle.start_time = 100;

    if (dev_inf.circle.stop_time > MAX_TIME_DAY) dev_inf.circle.stop_time = 100;

    if (dev_inf.circle.start_lv > CTRL_LV_NUM)dev_inf.circle.start_lv = 3;

    if (dev_inf.circle.stop_lv > CTRL_LV_NUM)dev_inf.circle.stop_lv = 3;

    for (i = 0 ; i < TEMP_NUM_MAX ; i++) {
        if (dev_inf.circle.temp_sta[i] > STA_ON) dev_inf.circle.temp_sta[i] = STA_OFF;
    }

    for (j = 0; j < TR_TYPE_NUM; j++) {
        if (ext_inf.timer[j].enable > STA_ON) ext_inf.timer[j].enable = STA_OFF;

        if (ext_inf.timer[j].type > TIMER_TYPE_FEED) ext_inf.timer[j].type = TIMER_TYPE_NONE;

        if (ext_inf.timer[j].back_time > TIME_2BIT) ext_inf.timer[j].back_time = 6;

        if (ext_inf.timer[j].mode > TIMER_MODE_ON_DELAY) ext_inf.timer[j].mode = TIMER_MODE_CYCLE;

        if (ext_inf.timer[j].start_min > TIME_3BIT) ext_inf.timer[j].start_min = 100;

        if (ext_inf.timer[j].stop_min > TIME_3BIT) ext_inf.timer[j].stop_min = 100;

        for (i = 0 ; i < TIMER_NUM ; i++) {
            if (ext_inf.timer[j].num_en[i] > STA_ON) ext_inf.timer[j].num_en[i] = STA_OFF;

            if (ext_inf.timer[j].start_time[i] > MAX_TIME_DAY) ext_inf.timer[j].start_time[i] = 100;

            if (ext_inf.timer[j].stop_time[i] > MAX_TIME_DAY) ext_inf.timer[j].stop_time[i] = 100;

            if (ext_inf.timer[j].delay_min[i] > TIME_3BIT) ext_inf.timer[j].delay_min[i] = 100;
        }
    }

    for (i = 0 ; i < MB_DEV_MAX; i++) {
        if (dev_inf.env_dev[i].enable > STA_ON) dev_inf.env_dev[i].enable = STA_OFF;
    }

    if (dev_inf.limit.pm2_5_H > TIME_3BIT) dev_inf.limit.pm2_5_H = 75;

    if (dev_inf.limit.pm10_H > TIME_3BIT) dev_inf.limit.pm10_H = 100;

    for (i = 0 ; i < SENSOR_TYPE_NUM; i++) {
        if (dev_inf.sensor_rg[i].enable > STA_ON) dev_inf.sensor_rg[i].enable = STA_OFF;
    }
}

int room_type_egg_en(void)
{
    if (basic_inf.type == ROOM_CHICKEN_SEED || basic_inf.type == ROOM_CHICKEN_EGG || basic_inf.type == ROOM_DUCK_EGG) {
        return 1;
    }

    return -1;
}

void ext_inf_check(void)
{
    unsigned char i = 0, j = 0;

    if (ext_inf.wMagic != KC_MAGIC) {
        Param_Default(SEXT_DF_ID);
    }

    for (i = 0 ; i < PRO_DATA_MAX ; i++) {
        if (ext_inf.din.pro_data[i].energy_sta > PL_NUM) {
            ext_inf.din.pro_data[i].energy_sta = PL_DIS;
        }

        if (ext_inf.din.pro_data[i].energy_ct <= 0)  	ext_inf.din.pro_data[i].energy_ct = 1;

        if (ext_inf.din.pro_data[i].water_sta > WL_NUM)  	ext_inf.din.pro_data[i].water_sta = WL_DIS;

        if (ext_inf.din.pro_data[i].water_unit > 9999)  	ext_inf.din.pro_data[i].water_unit = 10;

        if (ext_inf.din.pro_data[i].food_sta > FL_NUM)  	ext_inf.din.pro_data[i].food_sta = FL_DIS;

        if (ext_inf.din.pro_data[i].food_unit > 9999)  	ext_inf.din.pro_data[i].food_unit = 10;

        if (ext_inf.din.egg_en[i] > STA_ON) {
            ext_inf.din.egg_en[i] = STA_OFF;
        }

        if (room_type_egg_en() < 0) {
            ext_inf.din.egg_en[i] = STA_OFF;
        }
    }

    for (i = 0 ; i < MB_DATA_MAX ; i++) {
        if (ext_inf.din.mb_data[i].ux.dbits > UDATA_8B)ext_inf.din.mb_data[i].ux.dbits = UDATA_8B;

        if (ext_inf.din.mb_data[i].ux.stopb > STOP_BIT2)ext_inf.din.mb_data[i].ux.stopb = STOP_BIT1;

        if (ext_inf.din.mb_data[i].ux.parity > PARITY_EVEN)ext_inf.din.mb_data[i].ux.parity = PARITY_NONE;

        if (ext_inf.din.mb_data[i].baud > 7)ext_inf.din.mb_data[i].baud = 3;

        if (ext_inf.din.mb_data[i].dx.len > 9)ext_inf.din.mb_data[i].dx.len = 4;

        if (ext_inf.din.mb_data[i].dx.point > 9)ext_inf.din.mb_data[i].dx.point = 0;

        if (ext_inf.din.mb_data[i].dx.mem > 2)ext_inf.din.mb_data[i].dx.mem = 1;

        if (ext_inf.din.mb_data[i].dx.encode > 3)ext_inf.din.mb_data[i].dx.encode = 0;
    }
	
    if (ext_inf.din.leakage_enable > STA_ON)  	ext_inf.din.leakage_enable = STA_OFF;

    if (ext_inf.din.leakage_max_flow > MAX_1BYTE)  	ext_inf.din.leakage_max_flow = 100;

    if (ext_inf.din.leakage_delay_min == 0 || ext_inf.din.leakage_delay_min > WATER_ALA_DELAY)  	ext_inf.din.leakage_max_flow = 5;

    if (ext_inf.din.lack_enable > STA_ON)  	ext_inf.din.lack_enable = STA_OFF;

    if (ext_inf.din.lack_min_flow > MAX_1BYTE)  	ext_inf.din.lack_min_flow = 10;

    if (ext_inf.din.lack_delay_min == 0 || ext_inf.din.lack_delay_min > WATER_ALA_DELAY)  	ext_inf.din.lack_delay_min = 10;



    for (i = 0; i < CTRL_LV_NUM; i++) {
        for (j = 0; j < SWINDOW_NUM_MAX; j++) {
            if (ext_inf.lv_save[i].SWIN[j] >= MAX_PERCENT) {
                ext_inf.lv_save[i].SWIN[j] = 0;
            }
        }

        j = 0;

        if (ext_inf.lv_save[i].WIN[j] >= MAX_PERCENT) {
            ext_inf.lv_save[i].WIN[j] = 0;
        }

        j++;

        if (ext_inf.lv_save[i].WIN[j] >= MAX_PERCENT) {
            ext_inf.lv_save[i].WIN[j] = 0;
        }

        j++;

        if (ext_inf.lv_save[i].WIN[j] >= MAX_PERCENT) {
            ext_inf.lv_save[i].WIN[j] = 0;
        }

        j++;

        if (ext_inf.lv_save[i].WIN[j] >= MAX_PERCENT) {
            ext_inf.lv_save[i].WIN[j] = 0;
        }

        j++;
    }

}

void ctrl_inf_check(void)
{
    unsigned char i = 0 , j = 0;

    if (ctrl_inf.wMagic != KC_MAGIC) {
        Param_Default(SCTRL_DF_ID);
    }

    if (ctrl_inf.temp_adj > TEMP_ADJMAX || ctrl_inf.temp_adj < TEMP_ADJMIN) {
        ctrl_inf.temp_adj = 0;
    }

    for (i = 0 ; i < TEMP_CURVE_NUM ; i++) {
        if (ctrl_inf.temp[i].enable > true) ctrl_inf.temp[i].enable = false;

        if (ctrl_inf.temp[i].grow_day > DAY_MAX ) ctrl_inf.temp[i].grow_day = 2 * i;

        if ((ctrl_inf.temp[i].heat_temp > TEMP_MAX) || (ctrl_inf.temp[i].heat_temp < TEMP_MIN)) ctrl_inf.temp[i].heat_temp = 200;

        if ((ctrl_inf.temp[i].target_temp > TEMP_MAX) || (ctrl_inf.temp[i].target_temp < TEMP_MIN)) ctrl_inf.temp[i].target_temp = 250;

        if ((ctrl_inf.temp[i].vertical_temp > TEMP_MAX) || (ctrl_inf.temp[i].vertical_temp < TEMP_MIN))  ctrl_inf.temp[i].vertical_temp = 280;

        if ((ctrl_inf.temp[i].min_temp > TEMP_MAX) || (ctrl_inf.temp[i].min_temp < TEMP_MIN))  ctrl_inf.temp[i].min_temp = 180;

        if ((ctrl_inf.temp[i].max_temp > TEMP_MAX) || (ctrl_inf.temp[i].max_temp < TEMP_MIN))  ctrl_inf.temp[i].max_temp = 300;

        //set_temp(i,true,i*2,20,25,28,18,35);
    }

    if (ctrl_inf.extemp.ala_diff_en > STA_ON) {
        ctrl_inf.extemp.ala_diff_en = STA_OFF;
    }

    if (ctrl_inf.extemp.day_temp_en > STA_ON) {
        ctrl_inf.extemp.day_temp_en = STA_ON;
    }

    if (ctrl_inf.extemp.chg_hours > 12 || ctrl_inf.extemp.chg_hours == 0) {
        ctrl_inf.extemp.chg_hours = 1;
    }

    if (ctrl_inf.extemp.chg_times > 24 || ctrl_inf.extemp.chg_times == 0) {
        ctrl_inf.extemp.chg_times = 4;
    }

    if (ctrl_inf.extemp.minwind_temp_sta > MIN_WIND_AREA) {
        ctrl_inf.extemp.minwind_temp_sta = MIN_WIND_OFF;
    }

    if (ctrl_inf.extemp.temp_simple > STA_ON) {
        ctrl_inf.extemp.temp_simple = STA_ON;
    }

    if (ctrl_inf.minair_en > true) ctrl_inf.minair_en = false;

    for (i = 0 ; i < MINAIR_NUM ; i++) {
        if (ctrl_inf.minair[i].enable > true) ctrl_inf.minair[i].enable = false;

        if (ctrl_inf.minair[i].grow_day > DAY_MAX) ctrl_inf.minair[i].grow_day = i * 2;

        if (ctrl_inf.minair[i].weight > 65535) ctrl_inf.minair[i].weight = i * 20;

        if (ctrl_inf.minair[i].breath > 9999) ctrl_inf.minair[i].breath = 20;

        if (ctrl_inf.minair[i].min_air > AIR_MAX) ctrl_inf.minair[i].min_air = 20;

        //set_min_air(i,true,i*2,i*20,20,20);
    }


    if (ctrl_inf.exminair.enable > true) ctrl_inf.exminair.enable = false;

    for (i = 0 ; i < EXMINAIR_NUM ; i++) {
        if (ctrl_inf.exminair.x[i].enable > true) ctrl_inf.exminair.x[i].enable = false;

        if (ctrl_inf.exminair.x[i].out_temp > TEMP_MAX || ctrl_inf.exminair.x[i].out_temp < TEMP_MIN) ctrl_inf.exminair.x[i].out_temp = 0;

        if (ctrl_inf.exminair.x[i].lv_temp > 99 || ctrl_inf.exminair.x[i].lv_temp < -99) ctrl_inf.exminair.x[i].lv_temp = 0;

        if (ctrl_inf.exminair.x[i].breath > 99 || ctrl_inf.exminair.x[i].breath == 0) ctrl_inf.exminair.x[i].breath = 10;

    }

    if (ctrl_inf.hum.enable > true) ctrl_inf.hum.enable = false;

    if (ctrl_inf.hum.max_temp > TIME_2BIT) ctrl_inf.hum.max_temp = 30;

    if (ctrl_inf.hum.start_hum > HUM_MAX) ctrl_inf.hum.start_hum = 30;

    if (ctrl_inf.hum.stop_hum > HUM_MAX) ctrl_inf.hum.stop_hum = 80;

    if (ctrl_inf.hum.min_adj_hum > HUM_MAX) ctrl_inf.hum.min_adj_hum = 5;

    if (ctrl_inf.hum.start_time > MAX_TIME_DAY) ctrl_inf.hum.start_time = 0;

    if (ctrl_inf.hum.end_time > MAX_TIME_DAY) ctrl_inf.hum.end_time = 0;
	
    for (i = 0; i < HUM_CURVE_NUM; i++) {
        if (ctrl_inf.hum.x[i].enable > true) ctrl_inf.hum.x[i].enable = false;

        if (ctrl_inf.hum.x[i].grow_day > TIME_3BIT) ctrl_inf.hum.x[i].grow_day = 42;

        if (ctrl_inf.hum.x[i].target_hum > HUM_MAX) ctrl_inf.hum.x[i].target_hum = 45;

        if (ctrl_inf.hum.x[i].scale > MAX_PERCENT) ctrl_inf.hum.x[i].scale = 10;
    }

    for (i = 0; i < CTRL_LV_NUM; i ++) {
        if (ctrl_inf.lv[i].enable > true) ctrl_inf.lv[i].enable = false;

        if ((ctrl_inf.lv[i].offset > TEMP_OFFSET_MAX)) ctrl_inf.lv[i].offset = 0;

        if ((ctrl_inf.lv[i].re_diff > TEMP_OFFSET_MAX) || (ctrl_inf.lv[i].re_diff < 0)) ctrl_inf.lv[i].re_diff = 0;

        for (j = 0; j < PWM_NUM_MAX; j++) {
            if (ctrl_inf.lv[i].PWM[j].mode > MODE_NUM) ctrl_inf.lv[i].PWM[j].mode = MODE_ON;

            if (ctrl_inf.lv[i].PWM[j].degree > MAX_DEGREE) ctrl_inf.lv[i].PWM[j].degree = 100;

            if (ctrl_inf.lv[i].PWM[j].on_time > TIME_4BIT || ctrl_inf.lv[i].PWM[j].on_time == 0) ctrl_inf.lv[i].PWM[j].on_time = 100;

            if (ctrl_inf.lv[i].PWM[j].off_time > TIME_4BIT) ctrl_inf.lv[i].PWM[j].off_time = 100;

            if (ctrl_inf.lv[i].PWM[j].cycle > TIME_4BIT) ctrl_inf.lv[i].PWM[j].cycle = 200;

            //set_lv_pwm(i,l++,MODE_ON,100,100,100,200);
        }

        for (j = WIN1_ID; j <= WIN2_ID; j++) {
            if (ctrl_inf.lv[i].WIN[j].degree > MAX_DEGREE) ctrl_inf.lv[i].WIN[j].degree = 0;
        }

        for (j = WIN3_ID; j <= WIN4_ID; j++) {
            if (ctrl_inf.exlv[i].WIN[j - WIN3_ID].degree > MAX_DEGREE) ctrl_inf.exlv[i].WIN[j - WIN3_ID].degree = 0;
        }

        //set_lv_win(i,j++,MODE_ON,0);
        //set_lv_win(i,j++,MODE_ON,0);
        for (j = 0; j < SWINDOW_NUM_MAX; j++) {
            if (ctrl_inf.lv[i].SWIN[j].degree > MAX_DEGREE) ctrl_inf.lv[i].SWIN[j].degree = 0;
        }

        //set_lv_swin(i,j++,MODE_ON,0);

        for (j = 0; j < JDQ_NUM_MAX; j++) {
            if (ctrl_inf.lv[i].JDQ[j].mode > MODE_NUM) ctrl_inf.lv[i].JDQ[j].mode = MODE_ON;

            if (ctrl_inf.lv[i].JDQ[j].on_time > TIME_4BIT || ctrl_inf.lv[i].JDQ[j].on_time == 0) ctrl_inf.lv[i].JDQ[j].on_time = 100;

            if (ctrl_inf.lv[i].JDQ[j].off_time > TIME_4BIT) ctrl_inf.lv[i].JDQ[j].off_time = 100;

            if (ctrl_inf.lv[i].JDQ[j].cycle > TIME_4BIT) ctrl_inf.lv[i].JDQ[j].cycle = 200;

            //set_lv_jdq(i,j++,MODE_ON,100,100,200);
        }

        if (ctrl_inf.exlv[i].WIN[0].degree > MAX_DEGREE) ctrl_inf.exlv[i].WIN[0].degree = 0;

        if (ctrl_inf.exlv[i].WIN[1].degree > MAX_DEGREE) ctrl_inf.exlv[i].WIN[1].degree = 0;
    }

    for (i = 0; i < LV_SCOPE_KIND; i++) {
        if (ctrl_inf.lv_scope[i].enable > STA_ON) ctrl_inf.lv_scope[i].enable = STA_OFF;

        for (j = 0; j < LV_SCOPE_NUM; j++) {
            if (ctrl_inf.lv_scope[i].curve[j].enable > STA_ON) ctrl_inf.lv_scope[i].curve[j].enable = STA_OFF;

            if (i == AS_TYPE_DAY) {
                if (ctrl_inf.lv_scope[i].curve[j].day_time > DAY_MAX ) ctrl_inf.lv_scope[i].curve[j].day_time = 1;
            } else if (i == AS_TYPE_TIME) {
                if (ctrl_inf.lv_scope[i].curve[j].day_time > MAX_TIME_DAY ) ctrl_inf.lv_scope[i].curve[j].day_time = 0;
            } else {
                if (ctrl_inf.lv_scope[i].curve[j].day_time > TEMP_MAX
                        || ctrl_inf.lv_scope[i].curve[j].day_time < TEMP_MIN) ctrl_inf.lv_scope[i].curve[j].day_time = 100;
            }

            if (ctrl_inf.lv_scope[i].curve[j].min_lv > CTRL_LV_NUM)ctrl_inf.lv_scope[i].curve[j].min_lv = 0;

            if (ctrl_inf.lv_scope[i].curve[j].max_lv > CTRL_LV_NUM)ctrl_inf.lv_scope[i].curve[j].max_lv = 10;

            if (ctrl_inf.lv_scope[i].curve[j].cold_lv > CTRL_LV_NUM)ctrl_inf.lv_scope[i].curve[j].cold_lv = 3;

            if (ctrl_inf.lv_scope[i].curve[j].warm_lv > CTRL_LV_NUM)ctrl_inf.lv_scope[i].curve[j].warm_lv = 10;

            if (ctrl_inf.lv_scope[i].curve[j].temp_type > 1)ctrl_inf.lv_scope[i].curve[j].temp_type = 0;

            if ((ctrl_inf.lv_scope[i].curve[j].temp_diff > TEMP_OFFSET_MAX) || (ctrl_inf.lv_scope[i].curve[j].temp_diff < 0))
                ctrl_inf.lv_scope[i].curve[j].temp_diff = 0;

            if ((ctrl_inf.lv_scope[i].curve[j].temp_cold > TEMP_MAX) || (ctrl_inf.lv_scope[i].curve[j].temp_cold < TEMP_MIN))
                ctrl_inf.lv_scope[i].curve[j].temp_cold = 100;

            if ((ctrl_inf.lv_scope[i].curve[j].temp_warm > TEMP_MAX) || (ctrl_inf.lv_scope[i].curve[j].temp_warm < TEMP_MIN))
                ctrl_inf.lv_scope[i].curve[j].temp_warm = 300;

        }
    }

    for (i = 0; i < SENCON_NUM; i++) {
        if (ctrl_inf.sencon[i].enable > STA_ON) ctrl_inf.sencon[i].enable = STA_OFF;

        if (ctrl_inf.sencon[i].delay_min > TIME_3BIT) ctrl_inf.sencon[i].delay_min = 30;

        if (ctrl_inf.sencon[i].continue_sec > TIME_3BIT) ctrl_inf.sencon[i].continue_sec = 180;

        if (ctrl_inf.sencon[i].heat_flag > STA_ON) ctrl_inf.sencon[i].heat_flag = STA_OFF;

        if (ctrl_inf.sencon[i].temp_drop > TEMP_ADJMAX) ctrl_inf.sencon[i].temp_drop = 30;

    }

    if (ctrl_inf.sencon[SENCON_HUM].target > HUM_MAX) ctrl_inf.sencon[SENCON_HUM].target = 60;

    if (ctrl_inf.sencon[SENCON_HUM].alaL > HUM_MAX) ctrl_inf.sencon[SENCON_HUM].alaL = 20;

    if (ctrl_inf.sencon[SENCON_HUM].alaH > HUM_MAX) ctrl_inf.sencon[SENCON_HUM].alaH = 80;

    if (ctrl_inf.sencon[SENCON_HUM].dead_zone > HUM_MAX) ctrl_inf.sencon[SENCON_HUM].dead_zone = 5;

    if (ctrl_inf.sencon[SENCON_CO2].target > CO2_MAX) ctrl_inf.sencon[SENCON_CO2].target = 600;

    if (ctrl_inf.sencon[SENCON_CO2].alaL > CO2_MAX) ctrl_inf.sencon[SENCON_CO2].alaL = 1;

    if (ctrl_inf.sencon[SENCON_CO2].alaH > CO2_MAX) ctrl_inf.sencon[SENCON_CO2].alaH = 2000;

    if (ctrl_inf.sencon[SENCON_CO2].dead_zone > CO2_MAX) ctrl_inf.sencon[SENCON_CO2].dead_zone = 100;

    if (ctrl_inf.sencon[SENCON_NH3].target > NH3_MAX) ctrl_inf.sencon[SENCON_NH3].target = 5;

    if (ctrl_inf.sencon[SENCON_NH3].alaL > NH3_MAX) ctrl_inf.sencon[SENCON_NH3].alaL = 1;

    if (ctrl_inf.sencon[SENCON_NH3].alaH > NH3_MAX) ctrl_inf.sencon[SENCON_NH3].alaH = 20;

    if (ctrl_inf.sencon[SENCON_NH3].dead_zone > NH3_MAX) ctrl_inf.sencon[SENCON_NH3].dead_zone = 2;

    if (ctrl_inf.vac.type > STA_ON) ctrl_inf.vac.type = STA_OFF;

    if (ctrl_inf.vac.targetL > 100 || ctrl_inf.vac.targetL < -100)	ctrl_inf.vac.targetL = 25;

    if (ctrl_inf.vac.targetH > 100 || ctrl_inf.vac.targetH < -100)	ctrl_inf.vac.targetH = 20;

    if (ctrl_inf.vac.alaL > 100 || ctrl_inf.vac.alaL < 0)	ctrl_inf.vac.alaL = 10;

    if (ctrl_inf.vac.alaH > 100 || ctrl_inf.vac.alaH < 0)	ctrl_inf.vac.alaH = 35;

    if (ctrl_inf.vac.dead_zone > 100 || ctrl_inf.vac.dead_zone < 2)  ctrl_inf.vac.dead_zone = 3;

    if (ctrl_inf.vac.delay_s > TIME_2BIT) ctrl_inf.vac.delay_s = 10;

    if (ctrl_inf.vac.make_s > TIME_2BIT) ctrl_inf.vac.make_s = 6;

    if (ctrl_inf.vac.low_tempdiff > 0) ctrl_inf.vac.low_tempdiff = -250;

    if (ctrl_inf.vac.high_tempdiff > 0) ctrl_inf.vac.high_tempdiff = -100;

    if (ctrl_inf.vac.min_ctrllv > CTRL_LV_NUM) ctrl_inf.vac.min_ctrllv = 0;

    if (ctrl_inf.vac.max_ctrllv > CTRL_LV_NUM) ctrl_inf.vac.max_ctrllv = 29;

    if (ctrl_inf.vac.min_alalv > CTRL_LV_NUM) ctrl_inf.vac.min_alalv = 0;

    if (ctrl_inf.vac.adj_move_s > TIME_1BIT) ctrl_inf.vac.adj_move_s = 0;

    if (ctrl_inf.vac.adj_delay_s > TIME_1BIT) ctrl_inf.vac.adj_delay_s = 0;

    if (ctrl_inf.vac.ala_delay_s > MAX_1BYTE) ctrl_inf.vac.ala_delay_s = 30;

    if (ctrl_inf.var.mode > STA_ON) ctrl_inf.var.mode = STA_OFF;

    if (ctrl_inf.var.lv_delay > TIME_3BIT) ctrl_inf.var.lv_delay = 60;

    if (ctrl_inf.var.fan_delay > TIME_1BIT) ctrl_inf.var.fan_delay = 3;

//    if (ctrl_inf.var.fan_lvdelay > TIME_2BIT) ctrl_inf.var.fan_lvdelay = 0;

    if (ctrl_inf.var.alarm_delay > TIME_2BIT) ctrl_inf.var.alarm_delay = 20;

    if (ctrl_inf.var.temp_diff > TIME_2BIT) ctrl_inf.var.temp_diff = 50;

    if (ctrl_inf.var.temp_diff_ala > TIME_2BIT) ctrl_inf.var.temp_diff_ala = 30;
	
    if (ctrl_inf.var.shield_ala_time > TIME_2BIT) ctrl_inf.var.shield_ala_time = 30;

    if (ctrl_inf.var.win_link > STA_ON) ctrl_inf.var.win_link = STA_OFF;

    if (ctrl_inf.var.win_link_lv > CTRL_LV_NUM) ctrl_inf.var.win_link_lv = 2;

    if (ctrl_inf.var.win_pretime > TIME_2BIT) ctrl_inf.var.win_pretime = 0;

    if (ctrl_inf.var.pwm_fan_defrost_time > TIME_2BIT) 	ctrl_inf.var.pwm_fan_defrost_time = 0;

    for (i = 0; i < 10; i++) {
        if (ctrl_inf.var.swin_defrost_en[i] > STA_ON) 	ctrl_inf.var.swin_defrost_en[i] = STA_OFF;
    }

    if (ctrl_inf.var.swin_hold_time > MAX_1BYTE) 	ctrl_inf.var.swin_hold_time = 0;

    if (ctrl_inf.var.swin_defrost_degree > 100 || ctrl_inf.var.swin_defrost_degree < -100) 	ctrl_inf.var.swin_defrost_degree = 0;

    if (ctrl_inf.lv_more.lv_fan_cycle_en > STA_ON) ctrl_inf.lv_more.lv_fan_cycle_en = STA_OFF;

    if (ctrl_inf.lv_more.lv_fan_cycle > TIME_4BIT) ctrl_inf.lv_more.lv_fan_cycle = 300;

    if (ctrl_inf.lv_more.lv_re_diff_en > STA_ON) ctrl_inf.lv_more.lv_re_diff_en = STA_ON;

    if (ctrl_inf.lv_more.lv_re_diff > TIME_2BIT) ctrl_inf.lv_more.lv_re_diff = 1;

    if (abs(ctrl_inf.lv_more.lv_temp_adj) > TIME_2BIT) ctrl_inf.lv_more.lv_temp_adj = 0;

    if (ctrl_inf.lv_more.fan_auto_sort > STA_ON) ctrl_inf.lv_more.fan_auto_sort = STA_OFF;

    for (i = 0; i < DAYTEMP_CURVE_NUM; i++) {
        if (ext_inf.day_temp[i].enable > STA_ON) {
            ext_inf.day_temp[i].enable = STA_OFF;
        }

        if (ext_inf.day_temp[i].grow_day > DAY_MAX || ext_inf.day_temp[i].grow_day < DAY_MIN) {
            ext_inf.day_temp[i].grow_day = 1;
        }


        if (ext_inf.day_temp[i].start_time > MAX_TIME_DAY) ext_inf.day_temp[i].start_time = 1200;

        if (ext_inf.day_temp[i].end_time > MAX_TIME_DAY) ext_inf.day_temp[i].end_time = 360;


        if (ext_inf.day_temp[i].fix_temp > DAYTEMP_FIX_TEMP_MAX|| ext_inf.day_temp[i].fix_temp < DAYTEMP_FIX_TEMP_MIN) {
            ext_inf.day_temp[i].fix_temp = 20;
        }
		
        if (ext_inf.day_temp[i].fix_times > DAYTEMP_FIX_TIMES_MAX|| ext_inf.day_temp[i].fix_times < DAYTEMP_FIX_TIMES_MIN) {
            ext_inf.day_temp[i].fix_times = 4;
        }
    }
}

void index_inf_check(void)
{
    if (index_inf.wMagic != KC_MAGIC) {
        Param_Default(SINDEX_DF_ID);
        return ;
    }

    if (index_inf.ala_id >= ALA_RECORD_MAX || index_inf.ala_state > 1) {
        index_inf.ala_id = 0;
        index_inf.ala_state = 0;
    }

    if (index_inf.data_id >= DATA_RECORD_MAX || index_inf.data_state > 1) {
        index_inf.data_id = 0;
        index_inf.data_state = 0;
    }

    if (index_inf.data_pro_id >= DATA_PRO_RECORD_MAX || index_inf.data_pro_state > 1) {
        index_inf.data_pro_id = 0;
        index_inf.data_pro_state = 0;
    }

    if (index_inf.his_buf_id >= HIS_BUF_NUM) {
        index_inf.his_buf_id = 0;
    }

    if (index_inf.his_id >= DATA_RECORD_MAX || index_inf.his_state > 1) {
        index_inf.his_id = 0;
        index_inf.his_state = 0;
    }
}

void Param_Check(void)
{
    basic_inf_check();
    dev_inf_check();
    ctrl_inf_check();
    index_inf_check();
    ext_inf_check();

    if (cal.a[6] != 0x00010000) {
        cal.a[0] = 0x03459A88;
        cal.a[1] = 0xFFFFC8C9;
        cal.a[2] = 0x00000036;
        cal.a[3] = 0xFFA3A664;
        cal.a[4] = 0x00000034;
        cal.a[5] = 0x00002919;
        cal.a[6] = 0x00010000; //如果没读出来也不会死机
    }

}

int	Load_Param(void)
{

    int		ires, size;

    ires = EEPROM_Read( EE_DEV_ADDR, (INT8U*)&dev_inf, sizeof(dev_inf));

    if (ires < 1 || dev_inf.wMagic != KC_MAGIC) { /* 默认配置 */
        Param_Default(SDEV_DF_ID);
    }

    ires = EEPROM_Read( EE_EXT_ADDR, (INT8U*)&ext_inf, sizeof(ext_inf));

    if (ires < 1 || ext_inf.wMagic != KC_MAGIC) { /* 默认配置 */
        Param_Default(SEXT_DF_ID);
    }

//数据量太多分两次读取
    size = (sizeof(ctrl_inf.lv) + sizeof(ctrl_inf.wMagic));
    ires = EEPROM_Read( EE_CTRL_ADDR, (INT8U*)&ctrl_inf, size);
    ires = EEPROM_Read( (EE_CTRL_ADDR + size), (INT8U*)&ctrl_inf.temp_adj, (sizeof(ctrl_inf) - size) );

    if (ires < 1 || ctrl_inf.wMagic != KC_MAGIC) { /* 默认配置 */
        Param_Default(SCTRL_DF_ID);
    }

    ires = EEPROM_Read( EE_BASIC_ADDR, (INT8U*)&basic_inf, sizeof(basic_inf));

    if (ires < 1 || basic_inf.wMagic != KC_MAGIC) { /* 默认配置 */
        basic_inf.type = ROOM_CHICKEN_MEAT;
        basic_inf.room = 1;
        basic_inf.id = 1;
        basic_inf.group = 1;
        Language_Bit_original = LANGUAGE_GBK;
        Param_Default(SBASIC_DF_ID);
        SensorRG_Default();
    }

    ires = EEPROM_Read( EE_INDEX_ADDR, (INT8U*)&index_inf, sizeof(index_inf));

    if (ires < 1 || index_inf.wMagic != KC_MAGIC) { /* 默认配置 */
        Param_Default(SINDEX_DF_ID);
    }

    EEPROM_Read( EE_CAL_ADDR, (INT8U*)&cal.a[0], sizeof(cal.a));


    ires = EEPROM_Read( EE_SOFT_ADDR, (INT8U*)&soft_inf.wMagic, sizeof(soft_inf));

    if (ires < 1 || soft_inf.wMagic != KC_MAGIC) { /* 默认配置 */
        soft_inf.wMagic = KC_MAGIC;
        soft_inf.creg_en = STA_ON;
        soft_inf.ala_en = 0;
        soft_inf.update_pro = PRO_HL;	
        EEPROM_Write( EE_SOFT_ADDR, (INT8U*)&soft_inf.wMagic, sizeof(soft_inf));
    }

    ires = EEPROM_Read( EE_CLOCK_ADDR, (INT8U*)&clock_inf.wMagic, sizeof(clock_inf));

    if (ires < 1 || clock_inf.wMagic != KC_MAGIC) { /* 默认配置 */
        memset((unsigned char* )&clock_inf.wMagic, 0, sizeof(clock_inf));
        clock_inf.wMagic = KC_MAGIC;
        EEPROM_Write( EE_CLOCK_ADDR, (INT8U*)&clock_inf.wMagic, sizeof(clock_inf));
    }
	
    if (clock_inf.clock_in_id >= CLOCK_RECORD_MAX) {
		clock_inf.clock_in_id = 0;	
    }
	
    Trial_Ver_Init();
    Param_Check();
    IM_CHK();
    return 1;
}

int Read_Recode_Data(u8 *buf, u8 tpye, u16 id)
{
    u32 addr = 0;
    u16 size = 0;
    int		ires;

    if (buf == NULL) {
        return -1;
    }

    switch (tpye) {
    case RECODE_ALA:
        if (id > ALA_RECORD_MAX) {
            return -2;
        }

        size = sizeof(TALA);
        addr = EE_ALA_ADDR + id * size;
        break;

    case RECODE_DATA:
        if (id > DATA_RECORD_MAX) {
            return -3;
        }

        size = sizeof(TDATA);
        addr = EE_DATA_ADDR + id * size;
        break;

    case RECODE_USE_DATA:
        if (id > DATA_PRO_RECORD_MAX) {
            return -3;
        }

        size = sizeof(TUSE_DATA);
        addr = EE_USE_ADDR + id * size;
        break;

    default:
        return -4;
    }

    ires = EEPROM_Read( addr, buf, size );
    return ires;
}

int Write_Recode_Data(u8 *buf, u8 tpye, u16 id)
{
    u32 addr = 0;
    u16 size = 0;
    int		ires;

    if (buf == NULL) {
        return -1;
    }

    switch (tpye) {
    case RECODE_ALA:
        if (id > ALA_RECORD_MAX) {
            return -2;
        }

        size = sizeof(TALA);
        addr = EE_ALA_ADDR + id * size;
        break;

    case RECODE_DATA:
        if (id > DATA_RECORD_MAX) {
            return -2;
        }

        size = sizeof(TDATA);
        addr = EE_DATA_ADDR + id * size;
        break;

    case RECODE_USE_DATA:
        if (id > DATA_PRO_RECORD_MAX) {
            return -2;
        }

        size = sizeof(TUSE_DATA);
        addr = EE_USE_ADDR + id * size;
        break;

    default:
        return -1;
    }

    ires = EEPROM_Write( addr, buf, size );
    return ires;
}

void Group_Def(void)
{
    Param_Default(SGROUP_DF_ID);
    memset( (unsigned char* )&recode_cur, 0, sizeof(recode_cur) );
    memset( (unsigned char* )&use_data_cur, 0, sizeof(use_data_cur) );

}

void Recode_Def(void)
{
    Param_Default(SINDEX_DF_ID);
    memset( (unsigned char* )&recode_cur, 0, sizeof(recode_cur) );
    memset( (unsigned char* )&use_data_cur, 0, sizeof(use_data_cur) );
}

void SetInf_Def(void)
{
    Param_Default(SDEV_DF_ID);
    Param_Default(SCTRL_DF_ID);
    Param_Default(SBASIC_DF_ID);
    Param_Default(SEXT_DF_ID);
    Recode_Def();

    soft_inf.ala_en = 0;//报警屏蔽
    EEPROM_Write( EE_SOFT_ADDR, (INT8U*)&soft_inf.wMagic, sizeof(soft_inf));

    memset((unsigned char* )&clock_inf.wMagic, 0, sizeof(clock_inf));
    clock_inf.wMagic = KC_MAGIC;
    EEPROM_Write( EE_CLOCK_ADDR, (INT8U*)&clock_inf.wMagic, sizeof(clock_inf));
}

void clock_in_data_save(void)
{
	if(sys_sta.net_sta == STA_ON){
		run_inf.clock_in_state = STA_ON;
	}
	
    if (clock_inf.clock_in_id >= CLOCK_RECORD_MAX) {
		clock_inf.clock_in_id = 0;	
    }
	
    clock_inf.clock_in_t[clock_inf.clock_in_id] = GetSysTime();
    clock_inf.clock_in_time ++;
    clock_inf.clock_in_id ++;

    if (clock_inf.clock_in_id >= CLOCK_RECORD_MAX) {
        clock_inf.clock_in_id = 0;
        clock_inf.clock_in_state = 1;
    }

    Config_Save(SCLOCK_ID);
}


int fs_writeinfor( unsigned char *path, u8 *buf, u32 seek, u16 num)
{
    FRESULT res;               /* FatFs 函数公共结果代码 */
    FIL  fdata;                   /* 文件对象 */
    UINT bw;             /* 文件读写字节字节计数 */
    u8 i = 0;

    /* OPEN FILE */
    if (num > 1024) {
        return 0;
    }


    res = f_open(&fdata, (const char *)path, FA_CREATE_ALWAYS | FA_WRITE);	 //强制类型转换

    if (res != FR_OK ) {
        res = f_open(&fdata, (const char *)path, FA_CREATE_ALWAYS | FA_WRITE);	 //强制类型转换

        if (res != FR_OK ) {
            return -1;
        }
    }

    for (i = 0; i < 3; i++) {
        if (seek != 0) {
            f_lseek(&fdata, seek);
        }

        res = f_write(&fdata, buf, num, & bw); //写文件
        f_close(&fdata);
        f_open(&fdata, (const char *)path, FA_OPEN_EXISTING | FA_READ); //读一次比较是否相等

        if (seek != 0) {
            f_lseek(&fdata, seek);
        }

        res = f_read(&fdata, eeprom_Rbuf, num, & bw); //读文件

        if (memcmp(buf, eeprom_Rbuf, num) == 0) {
            f_close(&fdata);
            return 1 ;
        } else {
            f_close(&fdata);
            f_open(&fdata, (const char *)path, FA_CREATE_ALWAYS | FA_WRITE);	 //强制类型转换
        }

    }

    f_close(&fdata);
    return -2 ;
}

int fs_readinfor(unsigned char *path, u8 *buf, u16 read_num) //一次读512字节
{
    FRESULT res;               /* FatFs 函数公共结果代码 */
    FIL  fdata;                   /* 文件对象 */
    UINT bw;             /* 文件读写字节字节计数 */
    u16 num = 0;

    if (read_num > 1024) {
        return -1;
    }

    res = f_open(&fdata, (const char *)path, FA_OPEN_EXISTING | FA_READ); //读两次

    if (res != FR_OK ) {
        res = f_open(&fdata, (const char *)path, FA_OPEN_EXISTING | FA_READ);

        if (res != FR_OK ) {
            res = f_open(&fdata, (const char *)path, FA_CREATE_ALWAYS | FA_WRITE);	 //强制类型转换

            if (res != FR_OK ) {
                return -2;
            }
        }
    }

    if (read_num  == 0) {
        do {
            res = f_read(&fdata, buf, 1024, & bw); //读文件

            if (res != FR_OK ) {
                return -3;
            }

            num += bw;
        } while (bw == 1024);
    } else {
        res = f_read(&fdata, buf, read_num, & bw); //读文件

        if (res != FR_OK ) {
            return -4;
        }

        num += bw;
    }

    f_close(&fdata);
    return num ;
}

THDATA	flash_cur_data[HIS_BUF_NUM];
int flash_his_out(void)
{
    int 	ret = 1, addr = 0, i = 0, id = 0, j = 0, file_id = 0;
    u16     size = 1024;
    u8		file_name[48];
    u16	    start_id = 0, total_num = 0;
    time_t  sys_time = GetSysTime();
    struct tm *t;
    t = localtime((const time_t *)&sys_time);

    //判断数据
    if (index_inf.his_state == 0 && index_inf.his_id == 0) {
        return ret;
    }

    //当前已有数据条数
    if (index_inf.his_state == 1) {
        total_num = HIS_NUM;
    } else {
        total_num = index_inf.his_id;
    }

    if (index_inf.his_id > 0) {
        start_id = index_inf.his_id - 1;
    } else {
        start_id = HIS_NUM - 1;
    }

    for (i = 0; i < total_num; id++) {
        // TODO:只导出第一个文件
        if (id >= 1) {
            return 1;
        }

        sprintf((char *)file_name, "%d_%02d%02d%02d%02d_%02d.xls", basic_inf.id, (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min, (id + 1));
        addr = 0;
        size = sprintf((char *)eeprom_Wbuf, "时间戳\t平均温度\t加热温度\t目标温度\t纵向温度\t平均湿度\t报警代码\r\n");
        ret = fs_writeinfor(file_name, eeprom_Wbuf, addr, size);

        if (ret < 0) {
            return ret;
        }

        addr += size;
        file_id = 0;

        while (1) {
            if (i >= total_num ) {
                break;
            }

            OSTimeDly(100);
            SPI_Flash_Read((INT8U *)&flash_cur_data[0].cur_time, (his_data_addr + start_id * 4096), sizeof(flash_cur_data));

            if (start_id == 0) {
                start_id = HIS_NUM - 1;
            } else {
                start_id --;
            }


            for (j = HIS_BUF_NUM; j > 0; j--) {
                //crc
                if (flash_cur_data[j - 1].crc != crc16_s((INT8U *)&flash_cur_data[j - 1].cur_time, (sizeof(THDATA) - sizeof(flash_cur_data[j - 1].crc)))) {
                    continue;
                }

                size = sprintf((char *)eeprom_Wbuf, "%d\t%f\t%f\t%f\t%f\t%d\t%lld\r\n", flash_cur_data[j - 1].cur_time, \
                               (float)(flash_cur_data[j - 1].avg_temp / (float)TEMP_RATE), (float)(flash_cur_data[j - 1].heat_temp * 1.0f / (float)TEMP_RATE), \
                               (float)(flash_cur_data[j - 1].target_temp / (float)TEMP_RATE), (float)(flash_cur_data[j - 1].fan_temp * 1.0f / (float)TEMP_RATE), \
                               flash_cur_data[j - 1].avg_hum, flash_cur_data[j - 1].ala);
                ret = fs_writeinfor(file_name, eeprom_Wbuf, addr, size);

                if (ret < 0) {
                    return ret;
                }

                addr += size;
            }

            i++;
            file_id++;

            if (i >= total_num || file_id >= HIS_FILE_NUM) {
                file_id = 0;
                break;
            }
        }
    }

    return ret;
}

int eeprom_his_out(void)
{
    int 	ret = 1, addr = 0, i = 0;
    u16     size = 1024;
    u8		file_name[48];
    int		display_num = 0, start_id = 0;
    THDATA	cur_data;
    time_t  sys_time = GetSysTime();
    struct tm *t;
    t = localtime((const time_t *)&sys_time);

    //判断数据
    if (index_inf.his_buf_id == 0) {
        return ret;
    }

    //打印缓存数据
    sprintf((char *)file_name, "%d_%02d%02d%02d%02d_00.xls", basic_inf.id, (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min);
    addr = 0;
    size = sprintf((char *)eeprom_Wbuf, "时间戳\t平均温度\t加热温度\t目标温度\t纵向温度\t平均湿度\t报警代码\r\n");
    ret = fs_writeinfor(file_name, eeprom_Wbuf, addr, size);

    if (ret < 0) {
        return ret;
    }

    addr += size;

    if (index_inf.his_buf_id > 0) {
        display_num = index_inf.his_buf_id;
        start_id = index_inf.his_buf_id - 1;
    }

    for (i = 0; i < display_num; i++) {
        if (EEPROM_Read(EE_HIS_ADDR + start_id * sizeof(THDATA), (INT8U *)&cur_data.cur_time, sizeof(THDATA)) < 1) {
            continue;
        }

        if (start_id != 0) {
            start_id --;
        }

        //crc
        if (cur_data.crc != crc16_s((INT8U *)&cur_data.cur_time, (sizeof(THDATA) - sizeof(cur_data.crc)))) {
            continue;
        }

        size = sprintf((char *)eeprom_Wbuf, "%d\t%f\t%f\t%f\t%f\t%d\t%lld\r\n", cur_data.cur_time, \
                       (float)(cur_data.avg_temp / (float)TEMP_RATE), (float)(cur_data.heat_temp * 1.0f / (float)TEMP_RATE), \
                       (float)(cur_data.target_temp / (float)TEMP_RATE), (float)(cur_data.fan_temp * 1.0f / (float)TEMP_RATE), \
                       cur_data.avg_hum, cur_data.ala);
        ret = fs_writeinfor(file_name, eeprom_Wbuf, addr, size);

        if (ret < 0) {
            return ret;
        }

        addr += size;
    }

    OSTimeDly(10);
    return ret;
}

int His_USB_OUT(void)
{
    eeprom_his_out();
    flash_his_out();

    return 1;
}

int Data_USB_OUT(void)
{
    int 	ret = 1, addr = 0;
    u16     size = 1024;
    u8		file_name[48];
    u16		display_num = 0, start_id = 0, i = 0;
    TDATA	cur_data;

    time_t  sys_time = GetSysTime();
    struct tm *t;
    t = localtime((const time_t *)&sys_time);
    sprintf((char *)file_name, "%d_%02d%02d%02d%02d_data.xls", basic_inf.id, (t->tm_mon + 1), t->tm_mday, t->tm_hour, t->tm_min);
    addr = 0;
    size = sprintf((char *)eeprom_Wbuf, "日龄\t最小温度\t平均温度\t最大温度\t室外温度\t死亡总量\t动物存量\r\n");
    ret = fs_writeinfor(file_name, eeprom_Wbuf, addr, size);

    if (ret < 0) {
        return ret;
    }

    addr += size;

    //当前已有数据条数
    if (index_inf.data_state == 1) {
        display_num = DATA_RECORD_MAX;
    } else {
        display_num = index_inf.data_id;
    }

    if (display_num == 0) {
        return ret;
    }

    if (index_inf.data_id > 0) {
        start_id = index_inf.data_id - 1;
    } else {
        start_id = DATA_RECORD_MAX - 1;
    }

    for (i = 0; i < display_num; i++) {
        if (Read_Recode_Data((U8*)&cur_data, RECODE_DATA, start_id) < 1) {
            continue;
        }

        size = sprintf((char *)eeprom_Wbuf, "%d\t%f\t%f\t%f\t%f\t%d\t%d\r\n", cur_data.grow_day, \
                       (float)(cur_data.min_temp / (float)TEMP_RATE), (float)(cur_data.avg_temp * 1.0f / (float)TEMP_RATE), \
                       (float)(cur_data.max_temp / (float)TEMP_RATE), (float)(cur_data.out_temp * 1.0f / (float)TEMP_RATE), \
                       cur_data.dead_total, cur_data.survival);
        ret = fs_writeinfor(file_name, eeprom_Wbuf, addr, size);

        if (ret < 0) {
            return ret;
        }

        addr += size;

        if (start_id == 0) {
            start_id = DATA_RECORD_MAX - 1;
        } else {
            start_id --;
        }
    }

    return ret;
}


int SetInf_USB_OUT(void)
{
    int 	ret = 1, addr = 0;
    u16     size = 1024;
    int		offset;
    unsigned short crc16 = 0xFFFF;

    offset = OFFSETOF(TBASIC, x);
    addr = EE_BASIC_ADDR + offset;

    while (1) {
        EEPROM_Read(addr, eeprom_Wbuf, size);
        ret = fs_writeinfor("setinf.bin", eeprom_Wbuf, (addr - (EE_BASIC_ADDR + offset)), size);
        crc16 = GetCRC16(eeprom_Wbuf, size, crc16);

        if (ret < 0) {
            return ret;
        }

        addr += size;

        if ((addr + 1024) >= EE_INDEX_ADDR) {
            size = (EE_INDEX_ADDR - addr);
        } else {
            size = 1024;
        }

        if ( size == 0 || addr > EE_INDEX_ADDR) {
            break;
        }
    }

    eeprom_Wbuf[0] = (INT8U)(crc16 >> 8);
    eeprom_Wbuf[1] = (INT8U)crc16;
    fs_writeinfor("setinf.bin", eeprom_Wbuf, (addr - (EE_BASIC_ADDR + offset)), sizeof(crc16));
    return ret;
}

int SetInf_USB_IN_Check(void)
{
    FRESULT res;
    FIL  	fdata;
    UINT 	bw;
    int 	ret = 1, addr = EE_BASIC_ADDR;
    u16     size = 0;
    int		offset;
    unsigned short crc16 = 0xFFFF;
    offset = OFFSETOF(TBASIC, x);
    addr += offset;
    res = f_open(&fdata, "setinf.bin", FA_OPEN_EXISTING | FA_READ);

    if (res != FR_OK ) {
        res = f_open(&fdata, "setinf.bin", FA_OPEN_EXISTING | FA_READ);

        if (res != FR_OK ) {
            ret = -1;
            f_close(&fdata);
            return ret;
        }
    }

    size = 1024;

    while (1) {
        res = f_read(&fdata, eeprom_Rbuf, size, & bw);

        if (res != FR_OK ) {
            ret = -2;
            f_close(&fdata);
            return ret;
        }

        crc16 = GetCRC16(eeprom_Rbuf, size, crc16);
        addr += bw;

        if ((addr + 1024) >= EE_INDEX_ADDR) {
            size = (EE_INDEX_ADDR - addr);
        } else {
            size = 1024;
        }

        if (bw < 1024 || size == 0 || addr > EE_INDEX_ADDR) {
            break;
        }
    }

    f_read(&fdata, eeprom_Rbuf, sizeof(crc16), & bw);
    f_close(&fdata);

    if (crc16 != (eeprom_Rbuf[0] * 0x100 + eeprom_Rbuf[1])) {
        return -1;
    }

    return ret;
}

int SetInf_USB_IN(void)
{
    FRESULT res;
    FIL  	fdata;
    UINT 	bw;
    int 	ret = 1, addr = EE_BASIC_ADDR;
    u16     size = 0;
    int		offset;

    if (SetInf_USB_IN_Check() < 0) {
        return -1;
    }

    offset = OFFSETOF(TBASIC, x);
    addr += offset;
    res = f_open(&fdata, "setinf.bin", FA_OPEN_EXISTING | FA_READ);

    if (res != FR_OK ) {
        res = f_open(&fdata, "setinf.bin", FA_OPEN_EXISTING | FA_READ);

        if (res != FR_OK ) {
            ret = -1;
            f_close(&fdata);
            return ret;
        }
    }

    size = 1024;

    while (1) {
        res = f_read(&fdata, eeprom_Rbuf, size, & bw);

        if (res != FR_OK ) {
            ret = -2;
            f_close(&fdata);
            return ret;
        }

        EEPROM_Write(addr, eeprom_Rbuf, bw);
        addr += bw;

        if ((addr + 1024) >= EE_INDEX_ADDR) {
            size = (EE_INDEX_ADDR - addr);
        } else {
            size = 1024;
        }

        if (bw < 1024 || size == 0 || addr > EE_INDEX_ADDR) {
            break;
        }
    }

    f_close(&fdata);
    return ret;
}


#define PARA_SIZE_MAX	(20*1024)

int Read_SeasonPara_Time(unsigned char num)//包含CRC校验
{
    int flash_addr = 0, size = 0, eeprom_addr = 0;
    unsigned short crc16 = 0xFFFF;

    if (num > DATA_WINTER) {
        return -1;
    }

    flash_addr = SEASON_ADDR  + num * PARA_SIZE_MAX;
    SPI_Flash_Read( (INT8U*)&season_inf.wMagic, flash_addr,  sizeof(season_inf));

    if (season_inf.wMagic != KC_MAGIC || season_inf.save_time == 0 || season_inf.save_time == 0xFFFFFFFF)
        return -2;

    flash_addr = SEASON_ADDR  + num * PARA_SIZE_MAX + sizeof(season_inf);
    eeprom_addr = EE_BASIC_ADDR + OFFSETOF(TBASIC, x);
    size = 1024;

    while (1) {
        SPI_Flash_Read(eeprom_Rbuf, flash_addr,  size);
        crc16 = GetCRC16(eeprom_Rbuf, size, crc16);
        flash_addr += size;
        eeprom_addr += size;

        if ((eeprom_addr + 1024) >= EE_INDEX_ADDR) {
            size = (EE_INDEX_ADDR - eeprom_addr);
        } else {
            size = 1024;
        }

        if (size == 0 || eeprom_addr > EE_INDEX_ADDR) {
            break;
        }
    }

    if (season_inf.crc16 != crc16) {
        return -3;
    }

    return 1;
}

int Read_SeasonPara(unsigned char num)
{
    int flash_addr = 0, size = 0, eeprom_addr = 0;

    if (num > DATA_WINTER) {
        return -1;
    }

    if (Read_SeasonPara_Time(num) < 0) {
        return -2;
    }

    flash_addr = SEASON_ADDR  + num * PARA_SIZE_MAX + sizeof(season_inf);
    eeprom_addr = EE_BASIC_ADDR + OFFSETOF(TBASIC, x);
    size = 1024;

    while (1) {
        SPI_Flash_Read(eeprom_Rbuf, flash_addr,  size);
        EEPROM_Write(eeprom_addr, eeprom_Rbuf, size);
        flash_addr += size;
        eeprom_addr += size;

        if ((eeprom_addr + 1024) >= EE_INDEX_ADDR) {
            size = (EE_INDEX_ADDR - eeprom_addr);
        } else {
            size = 1024;
        }

        if (size == 0 || eeprom_addr > EE_INDEX_ADDR) {
            break;
        }
    }

    return 1;
}

int Write_SeasonPara(unsigned char num)
{
    int flash_addr = 0, size = 0, eeprom_addr = 0;
    unsigned short crc16 = 0xFFFF;

    if (num > DATA_WINTER) {
        return -1;
    }

    flash_addr = SEASON_ADDR  + num * PARA_SIZE_MAX  + sizeof(season_inf);
    eeprom_addr = EE_BASIC_ADDR + OFFSETOF(TBASIC, x);
    size = 1024;

    while (1) {
        EEPROM_Read(eeprom_addr, eeprom_Wbuf, size);
        SPI_Flash_Write( eeprom_Wbuf, flash_addr,  size);
        flash_addr += size;
        eeprom_addr += size;
        crc16 = GetCRC16(eeprom_Wbuf, size, crc16);

        if ((eeprom_addr + 1024) >= EE_INDEX_ADDR) {
            size = (EE_INDEX_ADDR - eeprom_addr);
        } else {
            size = 1024;
        }

        if ( size == 0 || eeprom_addr > EE_INDEX_ADDR) {
            break;
        }
    }

    flash_addr = SEASON_ADDR  + num * PARA_SIZE_MAX;
    season_inf.wMagic = KC_MAGIC;
    season_inf.save_time = GetSysTime();
    season_inf.crc16 = crc16;
    SPI_Flash_Write( (INT8U*)&season_inf.wMagic, flash_addr,  sizeof(season_inf));
    return 1;
}



int timer_report_id(u8 id)
{
    if (id >= TR_TYPE_NUM)
        return 0;

    if (ext_inf.timer[id].enable != STA_ON)
        return 0;


    if (ext_inf.timer[id].type == TIMER_TYPE_LIGHT)
        return JDQ_STA_LIGHT;

    if (ext_inf.timer[id].type == TIMER_TYPE_WATER)
        return JDQ_STA_WATER;

    if (ext_inf.timer[id].type == TIMER_TYPE_FEED)
        return JDQ_STA_FEED;

    return JDQ_STA_TIMER;
}

//其他复用返回>0,
//set_off = 1,如果启用就设置为关闭
/*****************************************************************************
 函 数 名  : Get_Fan_Mul
 功能描述  : 检查继电器复用情况
 输入参数  : u8 id  继电器序号
             u8 set_off  1-如果启用就设置对应的类型为关闭
 输出参数  : <0-没有复用，>0-有复用情况，且值为对应协议的ID
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2020-03-16
    作    者   : 邹稳
    修改内容   : 新生成函数

*****************************************************************************/
int Get_Fan_Mul(u8 id, u8 set_off)
{
    int res = -1;

    if (id < JDQ_NUM_FIXED) {
        return res;
    }

    switch (id) {
    case JDQ_WET:
        if (dev_inf.wet_en[0] == STA_ON) {
            if (set_off == STA_ON) {
                dev_inf.wet_en[0] = STA_OFF;
                Config_Save(SWET_ID);
            }

            res = JDQ_STA_WET;
        }

        break;

    case JDQ_SPRAY:
        if (dev_inf.wet_en[1] == STA_ON) {
            if (set_off == STA_ON) {
                dev_inf.wet_en[1] = STA_OFF;
                Config_Save(SWET_ID);
            }

            res = JDQ_STA_WET;
        }

        if (dev_inf.spray_en != SPRAY_MODE_OFF) {
            if (set_off == STA_ON) {
                dev_inf.spray_en = SPRAY_MODE_OFF;
                Config_Save(SSPRAY_ID);
            }

            res = JDQ_STA_SPRAY;
        }

        break;

    case JDQ_CIR_FAN://JDQ_TIMER1
        if (dev_inf.circle.enable == STA_ON) {
            if (set_off == STA_ON) {
                dev_inf.circle.enable = STA_OFF;
                Config_Save(SCIRCLE_ID);
            }

            res = JDQ_STA_CIRCLE;
        }

        if (ext_inf.timer[TR_TYPE_TIMER1].enable == STA_ON) {
            if (set_off == STA_ON) {
                ext_inf.timer[TR_TYPE_TIMER1].enable = STA_OFF;
                Config_Save(STIMER_ID);
            }

            res = timer_report_id(TR_TYPE_TIMER1);
        }

        break;

    case JDQ_TIMER2:
        if (basic_inf.tempc_en.enable == STA_ON) {
            if (set_off == STA_ON) {
                basic_inf.tempc_en.enable = STA_OFF;
                Config_Save(STEMPC_ID);
            }

            res = JDQ_STA_TEMPC;
        }

        if (ext_inf.timer[TR_TYPE_TIMER2].enable == STA_ON) {
            if (set_off == STA_ON) {
                ext_inf.timer[TR_TYPE_TIMER2].enable = STA_OFF;
                Config_Save(STIMER_ID);
            }

            res = timer_report_id(TR_TYPE_TIMER2);
        }

        break;

    case JDQ_HEAT1://JDQ_TIMER3
    case JDQ_HEAT2://JDQ_TIMER4
        if (basic_inf.heat_en[id - JDQ_HEAT1] == STA_ON) {
            if (set_off == STA_ON) {
                basic_inf.heat_en[id - JDQ_HEAT1] = STA_OFF;
                Config_Save(SHEAT_ID);
            }

            res = JDQ_STA_HEAT;
        }

        if (ext_inf.timer[TR_TYPE_TIMER3 + id - JDQ_HEAT1].enable == STA_ON) {
            if (set_off == STA_ON) {
                ext_inf.timer[TR_TYPE_TIMER3 + id - JDQ_HEAT1].enable = STA_OFF;
                Config_Save(STIMER_ID);
            }

            res = timer_report_id(TR_TYPE_TIMER3 + id - JDQ_HEAT1);
        }

        break;

    case JDQ_HEAT3:
        if (basic_inf.heat_en[id - JDQ_HEAT1] == STA_ON) {
            if (set_off == STA_ON) {
                basic_inf.heat_en[id - JDQ_HEAT1] = STA_OFF;
                Config_Save(SHEAT_ID);
            }

            res = JDQ_STA_HEAT;
        }

        break;

    case JDQ_WIN1_UP:
    case JDQ_WIN1_DOWN:
        if (dev_inf.WIN.detail[WIN1_ID].enable == STA_ON) {
            if (set_off == STA_ON) {
                dev_inf.WIN.detail[WIN1_ID].enable = STA_OFF;
                Config_Save(SWIN_ID);
            }

            res = JDQ_STA_WIN_F100 + id - JDQ_WIN1_UP;
        }

        break;

    case JDQ_WIN2_UP:
    case JDQ_WIN2_DOWN:
        if (dev_inf.WIN.detail[WIN2_ID].enable == STA_ON) {
            if (set_off == STA_ON) {
                dev_inf.WIN.detail[WIN2_ID].enable = STA_OFF;
                Config_Save(SWIN_ID);
            }

            res = JDQ_STA_WIN_F100 + id - JDQ_WIN2_UP;
        }

        break;

    case JDQ_WIN3_UP:
    case JDQ_WIN3_DOWN:
        if (dev_inf.WIN.detail[WIN3_ID].enable == STA_ON) {
            if (set_off == STA_ON) {
                dev_inf.WIN.detail[WIN3_ID].enable = STA_OFF;
                Config_Save(SWIN_ID);
            }

            res = JDQ_STA_WIN_F100 + id - JDQ_WIN3_UP;
        }

        break;

    case JDQ_WIN4_UP:
    case JDQ_WIN4_DOWN:
        if (dev_inf.WIN.detail[WIN4_ID].enable == STA_ON) {
            if (set_off == STA_ON) {
                dev_inf.WIN.detail[WIN4_ID].enable = STA_OFF;
                Config_Save(SWIN_ID);
            }

            res = JDQ_STA_WIN_F100 + id - JDQ_WIN4_UP;
        }

        break;
    }

    return res;
}

//上电检测继电器的复用情况
void JDQ_Mul_CHK(void)
{
    u8 i = 0, j = 0;

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        if (Get_Fan_Mul(i, STA_OFF) > 0 && dev_inf.JDQ[i].enable == STA_ON) { //继电器风机复用
            dev_inf.JDQ[i].enable = STA_OFF;

            //级别禁止
            for (j = 0; j < CTRL_LV_NUM; j++) {
                ctrl_inf.lv[j].JDQ[i].mode = MODE_OFF;
            }
        }
    }

    Config_Save(SLVJDQ_ID);
    Config_Save(SJDQ_ID);
}

//设置时检测继电器的复用情况(风机以外为准) 水泵和喷淋的复用单独设置
void JDQ_MulOther_CHK(u8 sid)
{
    u8 j = 0;

    if (Get_Fan_Mul(sid, STA_OFF) > 0 && dev_inf.JDQ[sid].enable == STA_ON) { //继电器风机复用
        dev_inf.JDQ[sid].enable = STA_OFF;

        //级别禁止
        for (j = 0; j < CTRL_LV_NUM; j++) {
            ctrl_inf.lv[j].JDQ[sid].mode = MODE_OFF;
        }

        Config_Save(SLVJDQ_ID);
        Config_Save(SJDQ_ID);
    }
}

void Wet_Spary_Mul(u8 sid)
{
    if (dev_inf.wet_en[1] == STA_ON && dev_inf.spray_en != SPRAY_MODE_OFF) {
        if (sid == IM_WET_SPARY) {
            dev_inf.spray_en = SPRAY_MODE_OFF;
            Config_Save(SSPRAY_ID);
        } else if (sid == IM_SPARY_WET) {
            dev_inf.wet_en[1] = STA_OFF;
            Config_Save(SWET_ID);
        }
    }
}

void Circle_Timer1_Mul(u8 sid)
{
    if (dev_inf.circle.enable == STA_ON && ext_inf.timer[TR_TYPE_TIMER1].enable == STA_ON) {
        if (sid == IM_CIRCLE_TIMER1) {
            ext_inf.timer[TR_TYPE_TIMER1].enable = STA_OFF;
            Config_Save(STIMER_ID);
        } else if (sid == IM_TIMER1_CIRCLE) {
            dev_inf.circle.enable = STA_OFF;
            Config_Save(SCIRCLE_ID);
        }
    }
}

void Tempc_Timer2_Mul(u8 sid)
{
    if (basic_inf.tempc_en.enable == STA_ON && ext_inf.timer[TR_TYPE_TIMER1].enable == STA_ON) {
        if (sid == IM_TEMPC_TIMER2) {
            ext_inf.timer[TR_TYPE_TIMER2].enable = STA_OFF;
            Config_Save(STIMER_ID);
        } else if (sid == IM_TIMER2_TEMPC) {
            basic_inf.tempc_en.enable = STA_OFF;
            Config_Save(STEMPC_ID);
        }
    }
}

void Heat1_Timer3_Mul(u8 sid)
{
    if (basic_inf.heat_en[0] == STA_ON && ext_inf.timer[TR_TYPE_TIMER3].enable == STA_ON) {
        if (sid == IM_HEAT1_TIMER3) {
            ext_inf.timer[TR_TYPE_TIMER3].enable = STA_OFF;
            Config_Save(STIMER_ID);
        } else if (sid == IM_TIMER3_HEAT1) {
            basic_inf.heat_en[0] = STA_OFF;
            Config_Save(SHEAT_ID);
        }
    }
}

void Heat2_Timer4_Mul(u8 sid)
{
    if (basic_inf.heat_en[1] == STA_ON && ext_inf.timer[TR_TYPE_TIMER4].enable == STA_ON) {
        if (sid == IM_HEAT2_TIMER4) {
            ext_inf.timer[TR_TYPE_TIMER4].enable = STA_OFF;
            Config_Save(STIMER_ID);
        } else if (sid == IM_TIMER4_HEAT2) {
            basic_inf.heat_en[1] = STA_OFF;
            Config_Save(SHEAT_ID);
        }
    }
}

//设置时检测继电器的复用情况(风机为准)
void JDQ_MulFan_CHK(void)
{
    u8 i = 0;

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        if (Get_Fan_Mul(i, STA_OFF) > 0 && dev_inf.JDQ[i].enable == STA_ON) { //继电器风机复用
            Get_Fan_Mul(i, STA_ON);
        }
    }
}


//id--0 保留前面
void PWM_SWIN_CHK(u8 id)
{
    u8 i = 0, j = 0;

    if (id > 1)return;

    for (i = 0; i < PWM_NUM_MAX; i++) {//先排除小窗和风机共用
        if (dev_inf.PWM[i].enable == STA_ON && dev_inf.SWIN.detail[i].enable == STA_ON) {
            if (id == 0) {
                dev_inf.SWIN.detail[i].enable = STA_OFF;

                //级别禁止
                for (j = 0; j < CTRL_LV_NUM; j++) {
                    ctrl_inf.lv[j].SWIN[i].degree = 0;
                }

                Config_Save(SLVWIN_ID);
                Config_Save(SSWIN_ID);
            } else {
                dev_inf.PWM[i].enable = STA_OFF;

                //级别禁止
                for (j = 0; j < CTRL_LV_NUM; j++) {
                    ctrl_inf.lv[j].PWM[i].mode = MODE_OFF;
                }

                Config_Save(SLVPWM_ID);
                Config_Save(SPWM_ID);
            }
        }
    }


}

//id--0 保留前面
void PWM3_PHEAT_CHK(u8 id)
{
    u8 j = 0;

    if (id > 1)return;

    //与变频加热共用 保留模拟量
    if (basic_inf.pheat_en == STA_ON && dev_inf.PWM[IM_PWM3_PHEAT].enable == STA_ON
            || basic_inf.pheat_en == STA_ON && dev_inf.SWIN.detail[IM_PWM3_PHEAT].enable == STA_ON) {
        if (id == 0) {
            basic_inf.pheat_en = STA_OFF;
            Config_Save(SHEAT_ID);
        } else {
            dev_inf.PWM[IM_PWM3_PHEAT].enable = STA_OFF;
            dev_inf.SWIN.detail[IM_PWM3_PHEAT].enable = STA_OFF;

            //级别禁止
            for (j = 0; j < CTRL_LV_NUM; j++) {
                ctrl_inf.lv[j].PWM[IM_PWM3_PHEAT].mode = MODE_OFF;
                ctrl_inf.lv[j].SWIN[IM_PWM3_PHEAT].degree = 0;
            }

            Config_Save(SLVPWM_ID);
            Config_Save(SPWM_ID);
            Config_Save(SSWIN_ID);
        }
    }
}

//id--0 保留前面
void PWM_APWM_CHK(u8 id)
{
    u8 i = 0, j = 0;

    if (id > 1)return;

    for (i = 0; i < APWM_NUM_MAX; i++) {
        //与变频加热共用 保留模拟量
        if (basic_inf.area_pwm[i].enable == STA_ON && dev_inf.PWM[i].enable == STA_ON
                || basic_inf.area_pwm[i].enable == STA_ON && dev_inf.SWIN.detail[i].enable == STA_ON) {
            if (id == 0) {
                basic_inf.area_pwm[i].enable = STA_OFF;
                Config_Save(SAPWM_ID);
            } else {
                dev_inf.PWM[i].enable = STA_OFF;
                dev_inf.SWIN.detail[i].enable = STA_OFF;

                //级别禁止
                for (j = 0; j < CTRL_LV_NUM; j++) {
                    ctrl_inf.lv[j].PWM[i].mode = MODE_OFF;
                    ctrl_inf.lv[j].SWIN[i].degree = 0;
                }

                Config_Save(SLVPWM_ID);
                Config_Save(SPWM_ID);
                Config_Save(SSWIN_ID);
            }
        }
    }
}

//id--0 保留前面
void Light_SWIN_CHK(u8 id)
{
    u8 j = 0;

    if (id > 1)return;

    //与小窗共用 保留光照
    if (dev_inf.light.enable == STA_ON && dev_inf.SWIN.detail[PWM_LIGHT].enable == STA_ON) {
        if (id == 0) {
            dev_inf.SWIN.detail[PWM_LIGHT].enable = STA_OFF;

            //级别禁止
            for (j = 0; j < CTRL_LV_NUM; j++) {
                ctrl_inf.lv[j].SWIN[PWM_LIGHT].degree = 0;
            }

            Config_Save(SLVPWM_ID);
            Config_Save(SSWIN_ID);
        } else {
            dev_inf.light.enable = STA_OFF;
            Config_Save(SLIGHT_ID);
        }
    }
}

void Tempc_SWIN_CHK(u8 id)
{
    u8 j = 0;

    if (id > 1)return;

    //与小窗共用 保留温控开关
    if (basic_inf.tempc_pwm_en.enable == STA_ON && dev_inf.SWIN.detail[PWM_TEMPC].enable == STA_ON) {
        if (id == 0) {
            dev_inf.SWIN.detail[PWM_TEMPC].enable = STA_OFF;

            //级别禁止
            for (j = 0; j < CTRL_LV_NUM; j++) {
                ctrl_inf.lv[j].SWIN[PWM_TEMPC].degree = 0;
            }

            Config_Save(SLVPWM_ID);
            Config_Save(SSWIN_ID);
        } else {
            basic_inf.tempc_pwm_en.enable = STA_OFF;
            Config_Save(STEMPC_PWM_ID);
        }
    }
}

void AIN_WIN_CHK(u8 id)
{
	if(dev_inf.sensor_adj.SENSOR_EN[4] != STA_OFF && dev_inf.WIN.detail[0].mode == WIN_MODE_POS){
        if (id == 0) {
            dev_inf.WIN.detail[0].mode = WIN_MODE_TIME;
            Config_Save(SWIN_ID);
        } else {
            dev_inf.sensor_adj.SENSOR_EN[4] = STA_OFF;
            Config_Save(SSENSOR_ID);
        }	
	}

	if(dev_inf.sensor_adj.SENSOR_EN[5] != STA_OFF && dev_inf.WIN.detail[1].mode == WIN_MODE_POS){
        if (id == 0) {
            dev_inf.WIN.detail[1].mode = WIN_MODE_TIME;
            Config_Save(SWIN_ID);
        } else {
            dev_inf.sensor_adj.SENSOR_EN[5] = STA_OFF;
            Config_Save(SSENSOR_ID);
        }	
	}	
}

void IM_CHK(void)
{
    PWM_SWIN_CHK(0);
    PWM3_PHEAT_CHK(0);
    PWM_APWM_CHK(0);
    Light_SWIN_CHK(0);
    Tempc_SWIN_CHK(0);
    JDQ_Mul_CHK();
    pluse_water_chk_alarm();
	AIN_WIN_CHK(0);
}



