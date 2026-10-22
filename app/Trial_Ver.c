#include "trial_ver.h"
#include "md5.h"
#include "eeprom.h"
#include "datasave.h"
#include "pcf8563.h"
#include "Parameters.h"
#include <string.h>
#include "md5.h"
#include <stdio.h>

CODE_T s_tCodeInf;


void Trial_Ver_Init(void)
{
    EEPROM_Read(EE_TRIAL_ADDR, (uint8_t *)&s_tCodeInf, sizeof(s_tCodeInf));

    if (s_tCodeInf.wMagic != KC_MAGIC) {
        memset((uint8_t *)&s_tCodeInf, 0, sizeof(s_tCodeInf));
        s_tCodeInf.wMagic = KC_MAGIC;
        EEPROM_Write(EE_TRIAL_ADDR, (uint8_t *)&s_tCodeInf, sizeof(s_tCodeInf));
    }
}

//试用剩余时间，<0没有启用试用，>=0 剩余的小时数
int Get_Trial_ReHour(void)
{
    if (s_tCodeInf.wDay == 0) {
        return -1;
    }

    if (s_tCodeInf.wDay * DAY_TO_HOUR <= s_tCodeInf.dUsedHour) {
        return 0;
    }

    return (s_tCodeInf.wDay * DAY_TO_HOUR - s_tCodeInf.dUsedHour);
}

//解除试用
void Trial_Clear(uint16_t _wDay)
{
    s_tCodeInf.wDay = _wDay;
    s_tCodeInf.dUsedHour = 0;
    EEPROM_Write(EE_TRIAL_ADDR, (uint8_t *)&s_tCodeInf, sizeof(s_tCodeInf));
}

static int on_sec = 0;
// 一秒调用一次
void Trial_Sec_Proc(void)
{
    if (s_tCodeInf.wDay == 0) {
        sys_sta.trial_end = STA_OFF;
        return;
    }

    on_sec++;

    if (s_tCodeInf.dUsedHour < s_tCodeInf.wDay * DAY_TO_HOUR) {
        sys_sta.trial_end = STA_OFF;
    } else {
        sys_sta.trial_end = STA_ON;
    }

    if (on_sec <= HOUR_TO_SEC) {
        return;
    }

    on_sec = 0;

    if (s_tCodeInf.dUsedHour < s_tCodeInf.wDay * DAY_TO_HOUR) {
        s_tCodeInf.dUsedHour ++;
    }
}

void Trial_Time_Save(void)
{
    EEPROM_Write(EE_TRIAL_ADDR, (uint8_t *)&s_tCodeInf, sizeof(s_tCodeInf));
}

int MD5_Cmp_My(unsigned char *input_key, unsigned char *cal_key)
{
    int i = 0, num = 0;
    unsigned char key_temp = 0;

    if (input_key == NULL || cal_key == NULL) {
        return -1;
    }

    for (i = 0; i < 16; i++) {
        key_temp = (cal_key[i] >> 4);//高4位

        if (key_temp <= 9) {
            if (input_key[num] != (key_temp + 0x30)) {
                return -2;
            }

            num ++;
        }

        if (num >= 8) {//只判断前8位
            return 1;
        }


        key_temp = (cal_key[i]& 0x0F);//低4位

        if (key_temp <= 9) {
            if (input_key[num] != (key_temp + 0x30)) {
                return -2;
            }

            num ++;
        }

        if (num >= 8) {//只判断前8位
            return 1;
        }

    }

    return 2;//不足8位
}

static unsigned char key_input_buf[24] = {0};
static unsigned char key_output_buf[16] = {0};
int Trial_Key_Check(unsigned char *input_key)
{
    time_t sys_time = GetSysTime();
    struct tm *t;

    if (input_key == NULL) {
        return -1;
    }

    t = localtime((const time_t *)&sys_time);
    sprintf((char *)key_input_buf, "%02d%02d%02d", (t->tm_year - 100), (t->tm_mon + 1), t->tm_mday);
    strcpy((char *)&key_input_buf[6], "Kc3984"); //永久解除
    md5(key_input_buf, 12, key_output_buf);

    if (MD5_Cmp_My(input_key, key_output_buf) > 0) {
        Trial_Clear(0);
        return 0;
    }

    strcpy((char *)&key_input_buf[6], "Kc0030");//解除1个月
    md5(key_input_buf, 12, key_output_buf);

    if (MD5_Cmp_My(input_key, key_output_buf) > 0) {
        Trial_Clear(30);
        return 30;
    }

    strcpy((char *)&key_input_buf[6], "Kc0060");//解除3个月
    md5(key_input_buf, 12, key_output_buf);

    if (MD5_Cmp_My(input_key, key_output_buf) > 0) {
        Trial_Clear(60);
        return 60;
    }

    strcpy((char *)&key_input_buf[6], "Kc0180");//解除6个月
    md5(key_input_buf, 12, key_output_buf);

    if (MD5_Cmp_My(input_key, key_output_buf) > 0) {
        Trial_Clear(180);
        return 180;
    }

    strcpy((char *)&key_input_buf[6], "KccVer");//解除版本限制
    md5(key_input_buf, 12, key_output_buf);

    if (MD5_Cmp_My(input_key, key_output_buf) > 0) {
        return KC_MAGIC;
    }

    return -2;
}

