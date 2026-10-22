#include "recode.h"
#include "Parameters.h"
#include "pcf8563.h"
#include "bsp.h"
#include "datasave.h"
#include "jdq.h"
#include <string.h>
#include "menuinc.h"
#include "def.h"
#include <stdlib.h>




static uint64_t ala_state = 0;
int ALA_Set_State(u8 type, u8 state)
{
    int res 	= 0;

    if (type >= ALA_MAX || state > ALA_STA_ON) {
        res = -1;
        return res;
    }

    if (ctrl_inf.var.mode == STA_OFF) {
        res = -2;
        return res;
    }

    if (getbit(soft_inf.ala_en, type) == 1) {
        clrbit(ala_state, type);
        return 2;
    }

    if (state == ALA_STA_ON) {
        setbit(ala_state, type);
    } else {
        clrbit(ala_state, type);
    }

    res = 1;
    return res;
}

int ALA_Set_State_Time(u8 type, u8 state)
{
    if (state == ALA_STA_ON) {
        if (ala_inf[type].sec == 0) {
            ala_inf[type].sec = 1;
            return -1;
        }

        if (ala_inf[type].sec < ctrl_inf.var.alarm_delay) {
            return -2;
        }

        ALA_Set_State(type, state);
    } else {
        ala_inf[type].sec = 0;
        ALA_Set_State(type, state);
    }

    return 1;
}

int ALA_Get_State(u8 type)
{
    int res 	= 0;

    if (type >= ALA_MAX ) {
        res = -1;
        return res;
    }

    res = getbit(ala_state, type);
    return res;
}

uint64_t ALA_State(void)
{
    return ala_state;
}

void ALA_Save(void)
{
    int res = 0;

    if (sys_sta.on_sec < 2 * MIN_TO_SEC) {
        return;
    }

    if (recode_cur.ala.state == ala_state) {
        return;
    }

    recode_cur.ala.state = ala_state;
    recode_cur.ala.grow_day = basic_inf.grow_day;
    recode_cur.ala.alarm_time = GetSysTime();
    res = Write_Recode_Data((u8 *)&recode_cur.ala, RECODE_ALA, index_inf.ala_id);

    if (res < 1) {
        return;
    }

    index_inf.ala_id ++;

    if (index_inf.ala_id >= ALA_RECORD_MAX) {
        index_inf.ala_id = 0;
        index_inf.ala_state = 1;
    }

    Config_Save(SINDEX_ID);
}



TSALA ala_inf[ALA_TYPE_MAX];
#define TEMP_DEADZONE	(5)
#define HUM_DEADZONE	(2)
#define CO2_DEADZONE	(10)
#define NH3_DEADZONE	(2)
#define VAC_DEADZONE	(5)
#define PM_DEADZONE		(3)

void Ala_More_Tar(int ala_cur, int ala_tar, u8 id, int deadzone)
{
//    if (ala_cur > ala_tar + deadzone ||
//            ala_cur < ala_tar - deadzone) {
//        if (ala_inf[id].time < ALA_TIME) {
//            return;
//        }

//        ala_inf[id].time = 0;

//        if (ala_cur > ala_tar) {
//            if (ala_inf[id].sec < ctrl_inf.var.alarm_delay) {
//                return ;
//            }

//            ALA_Set_State(id, ALA_STA_ON);
//        } else {
//            ala_inf[id].sec = 0;
//            ALA_Set_State(id, ALA_STA_OFF);
//        }

//        return;
//    }

//    ala_inf[id].time = 0;
    u16 alarm_delay = 0;

    if (id == ALA_HVAC || id == ALA_LVAC) {
        alarm_delay = ctrl_inf.vac.ala_delay_s;
    } else {
        alarm_delay = ctrl_inf.var.alarm_delay;
    }

    if (ala_cur > ala_tar) {
        if (ala_inf[id].sec < alarm_delay) {
            return ;
        }

        ALA_Set_State(id, ALA_STA_ON);
    } else {
        ala_inf[id].sec = 0;
        ALA_Set_State(id, ALA_STA_OFF);
    }

}


void Ala_Less_Tar(int ala_cur, int ala_tar, u8 id, int deadzone)
{
//    if (ala_cur > ala_tar + deadzone ||
//            ala_cur < ala_tar - deadzone) {
//        if (ala_inf[id].time < ALA_TIME) {
//            return;
//        }

//        ala_inf[id].time = 0;

//        if (ala_cur < ala_tar) {
//            if (ala_inf[id].sec < ctrl_inf.var.alarm_delay) {
//                return ;
//            }

//            ALA_Set_State(id, ALA_STA_ON);
//        } else {
//            ala_inf[id].sec = 0;
//            ALA_Set_State(id, ALA_STA_OFF);
//        }

//        return;
//    }

//    ala_inf[id].time = 0;
    u16 alarm_delay = 0;

    if (id == ALA_HVAC || id == ALA_LVAC) {
        alarm_delay = ctrl_inf.vac.ala_delay_s;
    } else {
        alarm_delay = ctrl_inf.var.alarm_delay;
    }

    if (ala_cur < ala_tar) {
        if (ala_inf[id].sec < alarm_delay) {
            return ;
        }

        ALA_Set_State(id, ALA_STA_ON);
    } else {
        ala_inf[id].sec = 0;
        ALA_Set_State(id, ALA_STA_OFF);
    }

}

void Update_ALATest(void)
{
    time_t sys_time;
    int now_time;

    if (basic_inf.ala_test.enable != STA_ON) {
        cal_inf.ala_test = STA_OFF;
        return;
    }

    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;

    if (now_time >= (basic_inf.ala_test.time * MIN_TO_SEC) && now_time <= (basic_inf.ala_test.time * MIN_TO_SEC) + basic_inf.ala_test.hold_time) {
        cal_inf.ala_test = STA_ON;
    } else {
        cal_inf.ala_test = STA_OFF;
    }
}
int temp_diff_ala(u8 i)
{
    signed short 	temp;

    if (dev_inf.sensor_adj.TEMP_EN[i] != STA_ON || (i == TEMP_OUT_SID )) {	//启用不是室外
        return -1;
    }

    if ( cal_inf.VaiTemp_num >= 3 && ctrl_inf.var.temp_diff != 0
            && (abs(sta_in.sensor_adj.TEMP[i] - cal_inf.BaseTemp) > ctrl_inf.var.temp_diff)) {//需要报警
        return 2;
    }

    if (ctrl_inf.var.temp_diff == 0 || cal_inf.VaiTemp_num < 3) { //数量不够
        temp = cal_inf.temp;
    } else {
        temp = cal_inf.BaseTemp;
    }

    if (ctrl_inf.var.temp_diff_ala != 0
            && (abs(sta_in.sensor_adj.TEMP[i] - temp) > ctrl_inf.var.temp_diff_ala)) {//需要报警
        return 1;
    }

    return -3;
}

static int lack_water_delay = 0;
int GetLackWaterSta(void)
{
    int i = 0;

    if (basic_inf.grow_day < 1) {//从第1日龄开始
        return -1;
    }

    for (i = 0; i < TR_TYPE_NUM; i++) {
        if (ext_inf.timer[i].enable != STA_ON) {
            continue;
        }

        if (ext_inf.timer[i].type != TIMER_TYPE_WATER && ext_inf.timer[i].type != TIMER_TYPE_LIGHT) {//灯光或水线没开的时候不计算
            continue;
        }

        if (sta_out.ALLJDQ[JDQ_TIMER1 + i] != STA_ON) {
            return -1;
        }
    }

    if (dev_inf.light.enable == STA_ON) {
        if (sta_out.ALLPWM[PWM_LIGHT] == 0) {
            return -1;
        } else {
            return 1;
        }
    }

    return 1;
}

void CheckWaterflow(void)
{
    int i = 0;
    unsigned int water_record = 0;

    if (run_inf.water_flow_time < MIN_TO_SEC) {//一分钟记录一次
        return;
    }

    run_inf.water_flow_time = 0;

    if (ext_inf.din.pro_data[0].water_sta == WL_DIS) {//水表没启用
        ALA_Set_State(ALA_LEAKAGEW, ALA_STA_OFF);
        ALA_Set_State(ALA_LACKW, ALA_STA_OFF);
        return;
    }

//计算当前水量数据
    for (water_record = 0, i = 0; i < PRO_DATA_MAX; i++) {
        if (ext_inf.din.pro_data[i].water_sta == WL_DIS) {
            continue;
        }

        if (ext_inf.din.pro_data[0].water_sta == WL_CJT_188 || ext_inf.din.pro_data[0].water_sta == WL_MB_OWN) {
            if (sta_in.water[i] != (unsigned int)DATA_ER_VALUE) {
                water_record += sta_in.water[i];
            }
        } else {
            water_record += index_inf.water[i][PRO_DATA_ALL];
        }
    }

    if (water_record == 0) {
        ALA_Set_State(ALA_LEAKAGEW, ALA_STA_OFF);
        ALA_Set_State(ALA_LACKW, ALA_STA_OFF);
        return;
    }

    if (ext_inf.din.leakage_delay_min > WATER_ALA_DELAY) {
        ext_inf.din.leakage_delay_min = WATER_ALA_DELAY;
    }

    if (ext_inf.din.lack_delay_min > WATER_ALA_DELAY) {
        ext_inf.din.lack_delay_min = WATER_ALA_DELAY;
    }

    if (ext_inf.din.leakage_enable != STA_ON || ext_inf.din.leakage_delay_min == 0
            || ext_inf.din.leakage_max_flow == 0 || run_inf.water_record[ext_inf.din.leakage_delay_min - 1] == 0) {
        ALA_Set_State(ALA_LEAKAGEW, ALA_STA_OFF);
    } else {
        if (water_record - run_inf.water_record[ext_inf.din.leakage_delay_min - 1] > (ext_inf.din.leakage_max_flow * ext_inf.din.leakage_delay_min)) {
            ALA_Set_State(ALA_LEAKAGEW, ALA_STA_ON);
        } else {
            ALA_Set_State(ALA_LEAKAGEW, ALA_STA_OFF);
        }

    }

    if (GetLackWaterSta() < 0) {
        lack_water_delay = 0;
    } else {
        lack_water_delay++;
    }

    if (ext_inf.din.lack_enable != STA_ON || ext_inf.din.lack_delay_min == 0
            || ext_inf.din.lack_min_flow == 0 || run_inf.water_record[ext_inf.din.lack_delay_min - 1] == 0
            || lack_water_delay < ext_inf.din.lack_delay_min) {//时间还没到
        ALA_Set_State(ALA_LACKW, ALA_STA_OFF);
    } else {
        if (water_record - run_inf.water_record[ext_inf.din.lack_delay_min - 1] < (ext_inf.din.lack_min_flow * ext_inf.din.lack_delay_min)) {
            ALA_Set_State(ALA_LACKW, ALA_STA_ON);
        } else {
            ALA_Set_State(ALA_LACKW, ALA_STA_OFF);
        }
    }


//先进先出队列保存
    for (i = WATER_ALA_DELAY - 1; i > 0; i--) {
        run_inf.water_record[i] = run_inf.water_record[i - 1];
    }

    run_inf.water_record[0] = water_record;
}




extern int get_win_adjust(void);
void Update_ALA(void)
{
    u8 i = 0;
    int min_temp, max_temp;
    //报警测试
    Update_ALATest();

//	空舍不报警
    if (ctrl_inf.var.mode == STA_OFF) {
        ala_state = 0;
        return ;
    }

//    for (i = 0; i < ALA_TYPE_MAX; i++) {
//        if (ala_inf[i].time < ALA_TIME) {
//            ala_inf[i].time++;
//        }
//    }


    //TEMP
    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if (basic_inf.tempc_en.enable == STA_ON && (i + 1) == basic_inf.tempc_en.temp_id ) { //单独温控只判断故障
            if (dev_inf.sensor_adj.TEMP_EN[i] == STA_ON) {
                if (sta_in.sensor_real.TEMP[i] >= TEMP_ER_VALUE) { //传感器错误
                    ALA_Set_State_Time((ALA_ETEMP1 + i), ALA_STA_ON);
                    ALA_Set_State_Time(ALA_SWHTEMP, ALA_STA_OFF);
                    ALA_Set_State_Time(ALA_SWLTEMP, ALA_STA_OFF);
                    ALA_Set_State_Time((ALA_ATEMP1 + i), ALA_STA_OFF);//去掉错误报警
                    continue;
                } else {
                    ALA_Set_State_Time((ALA_ETEMP1 + i), ALA_STA_OFF);
                }


                if (basic_inf.tempc_en.max_ala_temp != 0 && (sta_in.sensor_adj.TEMP[i] > basic_inf.tempc_en.max_ala_temp)) { //高温报警
                    ALA_Set_State_Time(ALA_SWHTEMP, ALA_STA_ON);
                } else {
                    ALA_Set_State_Time(ALA_SWHTEMP, ALA_STA_OFF);
                }


                if (basic_inf.tempc_en.min_ala_temp != 0 && (sta_in.sensor_adj.TEMP[i] < basic_inf.tempc_en.min_ala_temp)) { //高温报警
                    ALA_Set_State_Time(ALA_SWLTEMP, ALA_STA_ON);
                } else {
                    ALA_Set_State_Time(ALA_SWLTEMP, ALA_STA_OFF);
                }

            } else {
                ALA_Set_State_Time((ALA_ETEMP1 + i), ALA_STA_OFF);
                ALA_Set_State_Time(ALA_SWHTEMP, ALA_STA_OFF);
                ALA_Set_State_Time(ALA_SWLTEMP, ALA_STA_OFF);
            }

            ALA_Set_State_Time((ALA_ATEMP1 + i), ALA_STA_OFF);//去掉错误报警

            continue;
        }

        if (basic_inf.tempc_pwm_en.enable == STA_ON && (i + 1) == basic_inf.tempc_pwm_en.temp_id ) { //单独温控只判断故障
            if (dev_inf.sensor_adj.TEMP_EN[i] == STA_ON) {
                if (sta_in.sensor_real.TEMP[i] >= TEMP_ER_VALUE) { //传感器错误
                    ALA_Set_State_Time((ALA_ETEMP1 + i), ALA_STA_ON);
                    ALA_Set_State_Time(ALA_SWHTEMP, ALA_STA_OFF);
                    ALA_Set_State_Time(ALA_SWLTEMP, ALA_STA_OFF);
                    ALA_Set_State_Time((ALA_ATEMP1 + i), ALA_STA_OFF);//去掉错误报警
                    continue;
                } else {
                    ALA_Set_State_Time((ALA_ETEMP1 + i), ALA_STA_OFF);
                }


                if (basic_inf.tempc_pwm_en.max_ala_temp != 0 && (sta_in.sensor_adj.TEMP[i] > basic_inf.tempc_pwm_en.max_ala_temp)) { //高温报警
                    ALA_Set_State_Time(ALA_SWHTEMP, ALA_STA_ON);
                } else {
                    ALA_Set_State_Time(ALA_SWHTEMP, ALA_STA_OFF);
                }


                if (basic_inf.tempc_pwm_en.min_ala_temp != 0 && (sta_in.sensor_adj.TEMP[i] < basic_inf.tempc_pwm_en.min_ala_temp)) { //高温报警
                    ALA_Set_State_Time(ALA_SWLTEMP, ALA_STA_ON);
                } else {
                    ALA_Set_State_Time(ALA_SWLTEMP, ALA_STA_OFF);
                }

            } else {
                ALA_Set_State_Time((ALA_ETEMP1 + i), ALA_STA_OFF);
                ALA_Set_State_Time(ALA_SWHTEMP, ALA_STA_OFF);
                ALA_Set_State_Time(ALA_SWLTEMP, ALA_STA_OFF);
            }

            ALA_Set_State_Time((ALA_ATEMP1 + i), ALA_STA_OFF);//去掉错误报警

            continue;
        }

        //温度故障告警
        if (i == TEMP_OUT_SID) {
            max_temp = TEMP_OUTMAX_ERR;
            min_temp = TEMP_OUTMIN_ERR;
        } else {
            max_temp = TEMP_MAX_ERR;
            min_temp = TEMP_MIN_ERR;
        }

        if (dev_inf.sensor_adj.TEMP_EN[i] == STA_ON && (sta_in.sensor_real.TEMP[i] > max_temp || sta_in.sensor_real.TEMP[i] < min_temp)) { //传感器错误
            ALA_Set_State_Time((ALA_ETEMP1 + i), ALA_STA_ON);
            ALA_Set_State_Time((ALA_ATEMP1 + i), ALA_STA_OFF);
        }  else {
            ALA_Set_State_Time((ALA_ETEMP1 + i), ALA_STA_OFF);

            if (temp_diff_ala(i) > 0) { //需要报警
                ALA_Set_State_Time((ALA_ATEMP1 + i), ALA_STA_ON);
            } else {
                ALA_Set_State_Time((ALA_ATEMP1 + i), ALA_STA_OFF);
            }
        }
    }

    if (cal_inf.temp != TEMP_ER_VALUE) {
        Ala_More_Tar (cal_inf.temp , cal_inf.real_ala_Htemp, ALA_HTEMP, TEMP_DEADZONE);
        Ala_Less_Tar (cal_inf.temp , cal_inf.real_ala_Ltemp, ALA_LTEMP, TEMP_DEADZONE);
    } else {
        ALA_Set_State_Time(ALA_HTEMP, ALA_STA_OFF);
        ALA_Set_State_Time(ALA_LTEMP, ALA_STA_OFF);
    }

//体感高低
    if (cal_inf.humtemp != TEMP_ER_VALUE) {
        if (ctrl_inf.extemp.hum_Htemp != 0 && cal_inf.humtemp > cal_inf.target_cal_temp + ctrl_inf.extemp.hum_Htemp) {
            ALA_Set_State_Time(ALA_AHTEMP, ALA_STA_ON);
        } else {
            ALA_Set_State_Time(ALA_AHTEMP, ALA_STA_OFF);
        }

        if (ctrl_inf.extemp.hum_Ltemp != 0 && cal_inf.humtemp < cal_inf.target_cal_temp - ctrl_inf.extemp.hum_Ltemp) {
            ALA_Set_State_Time(ALA_ALTEMP, ALA_STA_ON);
        } else {
            ALA_Set_State_Time(ALA_ALTEMP, ALA_STA_OFF);
        }
    } else {
        ALA_Set_State_Time(ALA_AHTEMP, ALA_STA_OFF);
        ALA_Set_State_Time(ALA_ALTEMP, ALA_STA_OFF);
    }

//HUM
    if ((cal_inf.SEN_EN[SENSOR_TYPE_HUM] == STA_ON && cal_inf.SEN_ER[SENSOR_TYPE_HUM] == STA_ON)
            || (cal_inf.SEN_EN[SENSOR_TYPE_OUTHUM] == STA_ON && cal_inf.SEN_ER[SENSOR_TYPE_OUTHUM] == STA_ON)) {
        ALA_Set_State_Time(ALA_EHUM, ALA_STA_ON);
    } else {
        ALA_Set_State_Time(ALA_EHUM, ALA_STA_OFF);
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_HUM] == STA_ON && cal_inf.SEN[SENSOR_TYPE_HUM] != (short)SENSOR_ER_VALUE) {
        Ala_More_Tar (cal_inf.SEN[SENSOR_TYPE_HUM] , ctrl_inf.sencon[SENCON_HUM].alaH, ALA_HHUM, HUM_DEADZONE);
        Ala_Less_Tar (cal_inf.SEN[SENSOR_TYPE_HUM] , ctrl_inf.sencon[SENCON_HUM].alaL, ALA_LHUM, HUM_DEADZONE);
    } else {
        ALA_Set_State_Time(ALA_HHUM, ALA_STA_OFF);
        ALA_Set_State_Time(ALA_LHUM, ALA_STA_OFF);
    }


//CO2
    if (cal_inf.SEN_EN[SENSOR_TYPE_CO2] == STA_ON && cal_inf.SEN_ER[SENSOR_TYPE_CO2] == STA_ON) {
        ALA_Set_State_Time(ALA_ECO2, ALA_STA_ON);
    } else {
        ALA_Set_State_Time(ALA_ECO2, ALA_STA_OFF);
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_CO2] == STA_ON && cal_inf.SEN[SENSOR_TYPE_CO2] != (short)SENSOR_ER_VALUE) {
        Ala_More_Tar (cal_inf.SEN[SENSOR_TYPE_CO2] , ctrl_inf.sencon[SENCON_CO2].alaH, ALA_HCO2, CO2_DEADZONE);
    } else {
        ALA_Set_State_Time(ALA_HCO2, ALA_STA_OFF);
    }

//NH3
    if (cal_inf.SEN_EN[SENSOR_TYPE_NH3] == STA_ON && cal_inf.SEN_ER[SENSOR_TYPE_NH3] == STA_ON) {
        ALA_Set_State_Time(ALA_ENH3, ALA_STA_ON);
    } else {
        ALA_Set_State_Time(ALA_ENH3, ALA_STA_OFF);
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_NH3] == STA_ON && cal_inf.SEN[SENSOR_TYPE_NH3] != (short)SENSOR_ER_VALUE) {
        Ala_More_Tar (cal_inf.SEN[SENSOR_TYPE_NH3] , ctrl_inf.sencon[SENCON_NH3].alaH, ALA_HNH3, NH3_DEADZONE);
    } else {
        ALA_Set_State_Time(ALA_HNH3, ALA_STA_OFF);
    }

//H2S
    if (cal_inf.SEN_EN[SENSOR_TYPE_H2S] == STA_ON && cal_inf.SEN_ER[SENSOR_TYPE_H2S] == STA_ON) {
        ALA_Set_State_Time(ALA_EH2S, ALA_STA_ON);
    } else {
        ALA_Set_State_Time(ALA_EH2S, ALA_STA_OFF);
    }

//SPEED
    if (cal_inf.SEN_EN[SENSOR_TYPE_SPEED] == STA_ON && cal_inf.SEN_ER[SENSOR_TYPE_SPEED] == STA_ON) {
        ALA_Set_State_Time(ALA_ESPEED, ALA_STA_ON);
    } else {
        ALA_Set_State_Time(ALA_ESPEED, ALA_STA_OFF);
    }

//VAC
    if (cal_inf.SEN_EN[SENSOR_TYPE_VAC] == STA_ON && cal_inf.SEN_ER[SENSOR_TYPE_VAC] == STA_ON) {
        ALA_Set_State_Time(ALA_EVAC, ALA_STA_ON);
    } else {
        ALA_Set_State_Time(ALA_EVAC, ALA_STA_OFF);
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_VAC] == STA_ON && cal_inf.SEN[SENSOR_TYPE_VAC] != (short)VAC_ER_VALUE) {
        if (get_win_adjust() != STA_ON && run_inf.lv_win_delay == 0) {//校准时不报警
            Ala_More_Tar (abs(cal_inf.SEN[SENSOR_TYPE_VAC]) , abs(ctrl_inf.vac.alaH) , ALA_HVAC, VAC_DEADZONE);

            if (cal_inf.total_fan_num != 0 && cal_inf.lv >= ctrl_inf.vac.min_alalv) {
                Ala_Less_Tar (abs(cal_inf.SEN[SENSOR_TYPE_VAC]) , abs(ctrl_inf.vac.alaL), ALA_LVAC, VAC_DEADZONE);
            } else {
                ALA_Set_State_Time(ALA_LVAC, ALA_STA_OFF);
            }
        } else {
            ala_inf[ALA_HVAC].sec = 0;
            ala_inf[ALA_LVAC].sec = 0;
        }
    } else {
        ALA_Set_State_Time(ALA_HVAC, ALA_STA_OFF);
        ALA_Set_State_Time(ALA_LVAC, ALA_STA_OFF);
    }

//pm2.5 pm10
    if (dev_inf.env_dev[MB_DEV_PM2_5].enable == STA_ON && ALA_Get_State(ALA_EPM2_5) == ALA_STA_OFF) {
        Ala_More_Tar (sta_in.pm2_5, dev_inf.limit.pm2_5_H, ALA_HPM2_5, PM_DEADZONE);
        Ala_More_Tar (sta_in.pm10, dev_inf.limit.pm10_H, ALA_HPM10, PM_DEADZONE);
    } else {
        ALA_Set_State_Time(ALA_HPM2_5, ALA_STA_OFF);
        ALA_Set_State_Time(ALA_HPM10, ALA_STA_OFF);
    }


//外部报警
    for (i = 0; i < ALARM_NUM_MAX; i++) {
        if (getbit(ext_inf.din.alarm, 7) == 1 && getbit(sta_in.din.alarm, i) == 1
                && getbit(ext_inf.din.alarm, i) == 1) {
            ALA_Set_State_Time(ALA_ELE + i, ALA_STA_ON);
        } else {
            ALA_Set_State_Time(ALA_ELE + i, ALA_STA_OFF);
        }
    }

    if (sys_sta.ac_power_off == STA_ON) { //断电报警不延时
        ALA_Set_State(ALA_POWER, ALA_STA_ON);
    } else {
        ALA_Set_State(ALA_POWER, ALA_STA_OFF);
    }

	
    CheckWaterflow();


//级别设置错误报警

    for (i = 1; i < CTRL_LV_NUM; i++) {
        if ((ctrl_inf.lv[i].enable != STA_ON)) {
            ALA_Set_State(ALA_LV, ALA_STA_OFF);
            break;
        }

        if (ctrl_inf.lv[i].offset  < ctrl_inf.lv[i - 1].offset) {
            ALA_Set_State(ALA_LV, ALA_STA_ON);
            break;
        }

        if (cal_inf.lv_air[i] < cal_inf.lv_air[i - 1]) {
            ALA_Set_State(ALA_LV, ALA_STA_ON);
            break;
        }		
    }

    if (i >= CTRL_LV_NUM) { //所有级别都开没有报警
        ALA_Set_State(ALA_LV, ALA_STA_OFF);
    }

    ALA_Save();
}


float Get_Avg_Value(int avg, int value, u16 num)
{
    float res;

    if (num == 0) {
        res = -1;
        return res;
    }

    res = (float)(avg * (num - 1) * 1.0f / num + value * 1.0f / num);
    return res;
}
//计算平均数次数
typedef struct {
    u16 	in_temp;			//室内温度
    u16 	out_temp;			//室外温度
} TAVG_NUM;
TAVG_NUM avg_num;
void Update_Cal_Data(void)
{
    if (cal_inf.temp != TEMP_ER_VALUE) {
        if (recode_cur.data.min_temp == TEMP_ER_VALUE || recode_cur.data.min_temp > cal_inf.temp) {
            recode_cur.data.min_temp = cal_inf.temp;
        }

        if (recode_cur.data.avg_temp == TEMP_ER_VALUE ) {
            recode_cur.data.avg_temp = 0;
        }

        avg_num.in_temp++;
        recode_cur.data.avg_temp = (short) Get_Avg_Value(recode_cur.data.avg_temp, cal_inf.temp, avg_num.in_temp);

        if (recode_cur.data.max_temp == TEMP_ER_VALUE || recode_cur.data.max_temp < cal_inf.temp) {
            recode_cur.data.max_temp = cal_inf.temp;
        }
    }

    if (sta_in.sensor_adj.TEMP[TEMP_OUT_SID] != TEMP_ER_VALUE) {
        if (recode_cur.data.out_temp == TEMP_ER_VALUE) {
            recode_cur.data.out_temp = 0;
        }

        avg_num.out_temp++;
        recode_cur.data.out_temp = (short) Get_Avg_Value(recode_cur.data.out_temp, sta_in.sensor_adj.TEMP[TEMP_OUT_SID], avg_num.out_temp);
    }

}
void Cal_Temp_Def(void)
{
    recode_cur.data.min_temp = TEMP_ER_VALUE;
    recode_cur.data.avg_temp = TEMP_ER_VALUE;
    recode_cur.data.max_temp = TEMP_ER_VALUE;
    recode_cur.data.out_temp = TEMP_ER_VALUE;

    memset(&avg_num.in_temp, 0, sizeof(avg_num));
}
void Data_Save(void)
{
    int res;
    res = Write_Recode_Data((u8 *)&recode_cur.data, RECODE_DATA, index_inf.data_id);

    if (res < 1) {
        return;
    }

    index_inf.data_id ++;

    if (index_inf.data_id >= DATA_RECORD_MAX) {
        index_inf.data_id = 0;
        index_inf.data_state = 1;
    }

    Config_Save(SINDEX_ID);
}

void Data_Pro_Save(void)
{
    int res;
    res = Write_Recode_Data((u8 *)&use_data_cur.grow_day, RECODE_USE_DATA, index_inf.data_pro_id);

    if (res < 1) {
        return;
    }

    index_inf.data_pro_id ++;

    if (index_inf.data_pro_id >= DATA_PRO_RECORD_MAX) {
        index_inf.data_pro_id = 0;
        index_inf.data_pro_state = 1;
    }

    Config_Save(SINDEX_ID);
}

void Update_Pro_Data(void)
{
    int i = 0, j = 0;
    time_t sys_time = GetSysTime();
    int cal_tatol = 0;

    for (i = 0 ; i < PRO_DATA_MAX ; i++) {
        for (j = PRO_SAVE_NUM - 1; j > 0; j--) {
            if (j == PRO_DATA_ONE_AGO) {
                if (ext_inf.din.pro_data[0].water_sta == WL_CJT_188 || ext_inf.din.pro_data[0].water_sta == WL_MB_OWN) {
                    index_inf.water[i][PRO_DATA_ONE_AGO] = sta_in.water[i];
                } else {
                    index_inf.water[i][PRO_DATA_ONE_AGO] = index_inf.water[i][PRO_DATA_ALL];
                }

                index_inf.energy[i][PRO_DATA_ONE_AGO] = sta_in.ele[i][MB_ENERGY];
                index_inf.food[i][j] = index_inf.food[i][j - 1];
                continue;
            }

            index_inf.water[i][j] = index_inf.water[i][j - 1];
            index_inf.food[i][j] = index_inf.food[i][j - 1];
            index_inf.energy[i][j] = index_inf.energy[i][j - 1];

        }
    }

    for (j = PRO_SAVE_NUM - 1; j > 0; j--) {
        index_inf.egg[0][j] = index_inf.egg[0][j - 1];
    }

    for (j = PRO_SAVE_NUM - 1; j > 0; j--) {
        if (j == PRO_DATA_ONE_AGO) {
            index_inf.animal[j] = basic_inf.survival;
            continue;
        }

        index_inf.animal[j] = index_inf.animal[j - 1];
    }



    use_data_cur.grow_day = basic_inf.grow_day;
    use_data_cur.time = sys_time;
    use_data_cur.survival = basic_inf.survival;

    for (i = 0 ; i < PRO_DATA_MAX ; i++) {
        cal_tatol = index_inf.water[i][PRO_DATA_ONE_AGO] - index_inf.water[i][PRO_DATA_TWO_AGO];

        if (cal_tatol >= 0) {
            use_data_cur.water[i] = cal_tatol;
        } else {
            use_data_cur.water[i] = 0;
        }

        cal_tatol = index_inf.energy[i][PRO_DATA_ONE_AGO] - index_inf.energy[i][PRO_DATA_TWO_AGO];

        if (cal_tatol >= 0) {
            use_data_cur.energy[i] = cal_tatol;
        } else {
            use_data_cur.energy[i] = 0;
        }

        cal_tatol = index_inf.food[i][PRO_DATA_ONE_AGO] - index_inf.food[i][PRO_DATA_TWO_AGO];

        if (cal_tatol >= 0) {
            use_data_cur.food[i] = cal_tatol;
        } else {
            use_data_cur.food[i] = 0;
        }
    }

    Data_Pro_Save();
}

void Updata_Data(void)
{
    int i = 0;
    time_t sys_time = GetSysTime();

    for (i = 0; i < ALLPWM_NUM_MAX; i++) {
        sta_out.PWM_TIME[i] = 0;
    }

    for (i = 0; i < ALLJDQ_NUM_MAX; i++) {
        sta_out.JDQ_TIME[i] = 0;
    }

    recode_cur.data.time = sys_time;
    recode_cur.data.grow_day = basic_inf.grow_day;
    recode_cur.data.dead_total = (basic_inf.total - basic_inf.survival);
    recode_cur.data.survival = basic_inf.survival;

    if (recode_cur.data.min_temp == TEMP_ER_VALUE) {
        recode_cur.data.min_temp = cal_inf.temp;
    }

    if (recode_cur.data.avg_temp == TEMP_ER_VALUE) {
        recode_cur.data.avg_temp = cal_inf.temp;
    }

    if (recode_cur.data.max_temp == TEMP_ER_VALUE) {
        recode_cur.data.max_temp = cal_inf.temp;
    }

    if (recode_cur.data.out_temp == TEMP_ER_VALUE) {
        recode_cur.data.out_temp = sta_in.sensor_adj.TEMP[TEMP_OUT_SID];
    }

    Data_Save();
    Cal_Temp_Def();
}










/*****************************************************************************/









