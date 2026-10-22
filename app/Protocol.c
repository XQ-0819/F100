#include "protocol.h"
#include "common.h"
#include "types.h"
#include "menuinc.h"
#include "bsp.h"
#include "def.h"
#include "Parameters.h"
#include "mytask.h"
#include "userconfig.h"
#include "usart.h"
#include "recode.h"
#include <string.h>
#include "encrypt.h"
#include <math.h>
#include "trial_ver.h"

Packet_Head  Packet_Buffer;
SYSTEM_REPORT report;
unsigned short Report_Current_Data_Len ;
unsigned char Current_Data_Buf[1024];


OS_EVENT *data_send_sem;//定义时间保护
BYTE localRxBuf[BUF_SIZE_2K];
static BYTE localTxBuf[BUF_SIZE_2K];

BYTE USARTCOMM;

void Packet_Init(void)
{

    Packet_Buffer.Head1 = 0xA5;
    Packet_Buffer.Head2 = 0x5A;


    Packet_Buffer.Send_Seq = 0;
    Packet_Buffer.Tail1 = 0xA5;
    Packet_Buffer.Tail2 = 0x0D;

    if (basic_inf.comm_info.ux.port == 0) {
        USARTCOMM = UART6_COMM;
    } else {
#if USE_GD == 1
        USARTCOMM = UART3_SENSOR;
#else
        if (get_hardware_ver() >= HARD_V2) {
            USARTCOMM = UART1_CON;
        } else {
            USARTCOMM = UART3_SENSOR;
        }
#endif
    }

    UartInit( USARTCOMM, basic_inf.comm_info.baud, basic_inf.comm_info.ux.dbits,
              basic_inf.comm_info.ux.parity, basic_inf.comm_info.ux.stopb);
}

//继电器上报类型转换 ，原来的类型定义好了不好修改
u8 JDQ_Update_Swtich(u8 channel)
{
    int new = 0;

    if (channel == ALLJDQ_NUM_MAX - 1) {//最后一个继电器为报警
        return basic_inf.ala_en * JDQ_STA_ALA;
    }

    new = Get_Fan_Mul(channel, STA_OFF);

    if (new < 0) {
        new =  dev_inf.JDQ[channel].enable * JDQ_STA_FAN;
    }

    return new;
}

static u8 update_id = 0;//电表上传的序号
void Report_Data_Init(unsigned char update_sta)//update == sta_off 不更新消耗数据表号
{
    u8 i = 0, j = 0, id = 0;
    time_t sys_time = GetSysTime();
    struct tm *t;
    unsigned short ala_temp;

    t = localtime((const time_t *)&sys_time);
    report.systime.year = Hex2Bcd(t->tm_year - 100);
    report.systime.month = Hex2Bcd(t->tm_mon + 1);
    report.systime.day = Hex2Bcd(t->tm_mday);
    report.systime.hour = Hex2Bcd(t->tm_hour);
    report.systime.min = Hex2Bcd( t->tm_min);

    report.device_type = SYSTEM_TYPE;
    report.Software_VER = SOFTWARE_VER;
    report.work_mode = ctrl_inf.var.mode;

    for (i = 0; i < PRO_DATA_MAX; i++) {
        if (update_sta != STA_ON) //被动采集时，1分钟以下序号不变
            break;

        update_id = (update_id + 1) % PRO_DATA_MAX;//采集序号

        if (update_id < ENERGY_MAX && energy_enable[update_id] == 1) {
            break;
        }

        if (ext_inf.din.pro_data[update_id].water_sta != STA_OFF) {
            break;
        }

        if (ext_inf.din.pro_data[update_id].food_sta != STA_OFF) {
            break;
        }
    }

    report.bak_x.id = update_id;
    report.bak_x.num = 6;


    if (run_inf.clock_in_state == STA_ON) {
        run_inf.clock_in_report = STA_ON;
        report.login_type = CLOCK_IN_SUCCESS;
    } else {
        run_inf.clock_in_report = STA_OFF;
        report.login_type = sys_sta.login_type;
    }

	if(basic_inf.type >= ROOM_DUCK_SEED){
	    report.type = basic_inf.type + 3;
	}else{
	    report.type = basic_inf.type;
	}

    report.room = basic_inf.room;
    report.temp = cal_inf.temp;
    report.set_temp = cal_inf.target_temp;
    report.hum = cal_inf.SEN[SENSOR_TYPE_HUM];

    if (cal_inf.target_hum != SENSOR_ER_VALUE) {
        report.target_hum = cal_inf.target_hum;
    } else {
        report.target_hum = ctrl_inf.sencon[SENCON_HUM].target;
    }

    report.grow_day = basic_inf.grow_day;
    report.out_temp = sta_in.sensor_adj.TEMP[TEMP_OUT_SID];
    if (basic_inf.heat_en[HEAT_AREA_A] == STA_ON) {
        report.tempA = cal_inf.tempArea[HEAT_AREA_A];
    } else {
        report.tempA = TEMP_ER_VALUE;
    }

    if (basic_inf.heat_en[HEAT_AREA_B] == STA_ON) {
        report.tempB = cal_inf.tempArea[HEAT_AREA_B];
    } else {
        report.tempB = TEMP_ER_VALUE;
    }

    if (basic_inf.heat_en[HEAT_AREA_C] == STA_ON) {
        report.tempC = cal_inf.tempArea[HEAT_AREA_C];
    } else {
        report.tempC = TEMP_ER_VALUE;
    }
    report.heat_temp = cal_inf.heat_temp;
    report.target_temp = cal_inf.target_temp;
    report.fan_temp = cal_inf.fan_temp;
    report.max_temp = cal_inf.real_ala_Htemp;
    report.min_temp = cal_inf.real_ala_Ltemp;
    report.max_hum = ctrl_inf.sencon[SENCON_HUM].alaH;
    report.min_hum = ctrl_inf.sencon[SENCON_HUM].alaL;
    report.cur_CO2 = cal_inf.SEN[SENSOR_TYPE_CO2];
    report.target_CO2 = ctrl_inf.sencon[SENCON_CO2].target;
    report.max_CO2 = ctrl_inf.sencon[SENCON_CO2].alaH;
    report.min_CO2 = ctrl_inf.sencon[SENCON_CO2].alaL;
    report.cur_NH3 = cal_inf.SEN[SENSOR_TYPE_NH3];
    report.target_NH3 = ctrl_inf.sencon[SENCON_NH3].target;
    report.max_NH3 = ctrl_inf.sencon[SENCON_NH3].alaH;
    report.min_NH3 = ctrl_inf.sencon[SENCON_NH3].alaL;
    report.cur_H2S = cal_inf.SEN[SENSOR_TYPE_H2S];
    report.target_H2S = SENSOR_ER_VALUE;
    report.max_H2S = SENSOR_ER_VALUE;
    report.min_H2S = SENSOR_ER_VALUE;
    report.cur_VAC = cal_inf.SEN[SENSOR_TYPE_VAC];
    report.target_VAC = cal_inf.target_vac;
    report.max_VAC = ctrl_inf.vac.alaH;
    report.min_VAC = ctrl_inf.vac.alaL;


    if (dev_inf.env_dev[MB_DEV_LIGHT].enable == STA_ON) {
        report.cur_light = sta_in.light;
    } else {
        report.cur_light = (unsigned short)LIGHT_ER_VALUE;
    }

    report.target_light = (unsigned short)LIGHT_ER_VALUE;
    report.max_light = (unsigned short)LIGHT_ER_VALUE;
    report.min_light = (unsigned short)LIGHT_ER_VALUE;

    report.min_air = cal_inf.min_air;
    report.max_lv = cal_inf.max_lv;
    report.min_lv = cal_inf.lv_l;
    report.lv = cal_inf.lv;
    report.lv_air = cal_inf.lv_air[cal_inf.lv];
    report.dead_total = basic_inf.total - basic_inf.survival;
    report.survival_bak = 0;
    report.survival = basic_inf.survival;

    report.scrap = (unsigned int)DATA_ER_VALUE;
    report.run_time = index_inf.run_time;

	if (soft_inf.update_pro == PRO_HL) {
	    if (ext_inf.din.pro_data[update_id].food_sta == FL_DIS) {
			report.residue_food = (unsigned int)DATA_ER_VALUE;    	
	    } else {
			report.residue_food = sta_in.food[update_id];
	    }
	}else{
		report.residue_food = 0;
	}
	
    report.auto_control_state = sys_sta.auto_mode;
    report.group_num = basic_inf.group;

    report.air_mode = 0;
    report.air_alarm = 0;


	if (soft_inf.update_pro == PRO_HL) {
	    if ( ext_inf.din.pro_data[update_id].food_sta == STA_OFF) {
	        report.food = (unsigned int)DATA_ER_VALUE;
	    } else {
	        report.food = index_inf.food[update_id][PRO_DATA_ALL] - index_inf.food[update_id][PRO_DATA_ONE_AGO];
	    }


	    if (update_id > ENERGY_MAX ||  energy_enable[update_id] == 0) {
	        for (j = 0; j < 3; j++) {
	            report.voltage[j] = 0;
	            report.current[j] = 0;
	        }

	        report.power = (unsigned int)DATA_ER_VALUE;
	    } else {
	        for (j = 0; j < 3; j++) {
	            report.voltage[j] = sta_in.ele[update_id][MB_VA + j];
	            report.current[j] = sta_in.ele[update_id][MB_IA + j];
	        }

	        if (ext_inf.din.pro_data[0].energy_sta != PL_DIS) {
				report.power = sta_in.ele[update_id][MB_ENERGY] - index_inf.energy[update_id][PRO_DATA_ONE_AGO];
	        } else {
	            report.power = (unsigned int)DATA_ER_VALUE;
	        }
	    }

	    if (ext_inf.din.pro_data[update_id].water_sta == STA_OFF) {
	        report.water = (unsigned int)DATA_ER_VALUE;
	    } else  if (ext_inf.din.pro_data[0].water_sta == WL_CJT_188 || ext_inf.din.pro_data[0].water_sta == WL_MB_OWN) {
	        if (sta_in.water[update_id] != (unsigned int)DATA_ER_VALUE) {
	            report.water = sta_in.water[update_id] - index_inf.water[update_id][PRO_DATA_ONE_AGO];
	        } else {
	            report.water = 0;
	        }
	    } else {
	        report.water = index_inf.water[update_id][PRO_DATA_ALL] - index_inf.water[update_id][PRO_DATA_ONE_AGO];
	    }		
	}else{
	    if ( ext_inf.din.pro_data[update_id].food_sta == STA_OFF) {
	        report.food = (unsigned int)DATA_ER_VALUE;
	    } else  if (ext_inf.din.pro_data[0].food_sta == FL_MB_OWN || ext_inf.din.pro_data[0].food_sta == FL_MB_XK3102) {
	        report.food = sta_in.food[update_id];
	    } else {
	        report.food = index_inf.food[update_id][PRO_DATA_ALL];
	    }


	    if (update_id > ENERGY_MAX ||  energy_enable[update_id] == 0) {
	        for (j = 0; j < 3; j++) {
	            report.voltage[j] = 0;
	            report.current[j] = 0;
	        }

	        report.power = (unsigned int)DATA_ER_VALUE;
	    } else {
	        for (j = 0; j < 3; j++) {
	            report.voltage[j] = sta_in.ele[update_id][MB_VA + j];
	            report.current[j] = sta_in.ele[update_id][MB_IA + j];
	        }

	        if (ext_inf.din.pro_data[0].energy_sta != PL_DIS) {
	            report.power = sta_in.ele[update_id][MB_ENERGY];
	        } else {
	            report.power = (unsigned int)DATA_ER_VALUE;
	        }
	    }

	    if (ext_inf.din.pro_data[update_id].water_sta == STA_OFF) {
	        report.water = (unsigned int)DATA_ER_VALUE;
	    } else  if (ext_inf.din.pro_data[0].water_sta == WL_CJT_188 || ext_inf.din.pro_data[0].water_sta == WL_MB_OWN) {
	        report.water = sta_in.water[update_id];
	    } else {
	        report.water = index_inf.water[update_id][PRO_DATA_ALL];
	    }
	}
    report.scrap = index_inf.egg[0][PRO_DATA_ALL];

    //
    for (i = 0; i < MAX_REPORT_TEMP; i++) {
        if (i < TEMP_NUM_MAX) {
            if (i == TEMP_OUT_SID) {
                report.TEMP[i].type = dev_inf.sensor_adj.TEMP_EN[i] * TEMP_OUT;//out temp
            } else {
                report.TEMP[i].type = dev_inf.sensor_adj.TEMP_EN[i];
            }

            report.TEMP[i].value = sta_in.sensor_adj.TEMP[i];
        } else {
            report.TEMP[i].type = 0;
            report.TEMP[i].value = TEMP_ER_VALUE;
        }
    }

    report.TEMP[MAX_REPORT_TEMP - 5].type = TEMP_TAR_HAND;
    report.TEMP[MAX_REPORT_TEMP - 5].value = cal_inf.tar_hand_temp;

    report.TEMP[MAX_REPORT_TEMP - 4].type = ctrl_inf.hum.enable * TEMP_TAR_HUM;
    report.TEMP[MAX_REPORT_TEMP - 4].value = cal_inf.tar_hum_temp;

    report.TEMP[MAX_REPORT_TEMP - 3].type = TEMP_LV_HAND;
    report.TEMP[MAX_REPORT_TEMP - 3].value = ctrl_inf.lv_more.lv_temp_adj;

    report.TEMP[MAX_REPORT_TEMP - 2].type = ctrl_inf.exminair.enable * TEMP_LV_OUT;
    report.TEMP[MAX_REPORT_TEMP - 2].value = cal_inf.lv_ex_offset;

    report.TEMP[MAX_REPORT_TEMP - 1].type = basic_inf.humtemp_en * TEMP_HUMAN;
    report.TEMP[MAX_REPORT_TEMP - 1].value = cal_inf.humtemp;

    for (i = 0; i < MAX_REPORT_HUM; i++) {
        report.HUM[i].type = 0;
        report.HUM[i].value = (unsigned char)SENSOR_ER_VALUE;
    }

    for (i = 0; i < MAX_REPORT_CO2; i++) {
        report.CO2[i].type = 0;
        report.CO2[i].value = SENSOR_ER_VALUE;
    }

    for (i = 0; i < MAX_REPORT_NH3; i++) {
        report.NH3[i].type = 0;
        report.NH3[i].value = SENSOR_ER_VALUE;
    }

    for (i = 0; i < MAX_REPORT_H2S; i++) {
        report.H2S[i].type = 0;
        report.H2S[i].value = SENSOR_ER_VALUE;
    }

    for (i = 0; i < MAX_REPORT_VAC; i++) {
        report.VAC[i].type = 0;
        report.VAC[i].value = VAC_ER_VALUE;
    }

    for (i = 0; i < MAX_REPORT_LIGHT; i++) {
        report.LIGHT[i].type = 0;
        report.LIGHT[i].value = SENSOR_ER_VALUE;
    }

    for (id = 0, i = 0; i < SENSOR_NUM_MAX; i++) {
        if (dev_inf.sensor_adj.SENSOR_EN[i] == SENSOR_TYPE_HUM) {
            report.HUM[id].type = STA_ON;
            report.HUM[id].value = sta_in.sensor_adj.SENSOR[i];
            id++;
			continue;
        }
		
        if (dev_inf.sensor_adj.SENSOR_EN[i] == SENSOR_TYPE_OUTHUM) {
            report.HUM[id].type = 2;
            report.HUM[id].value = sta_in.sensor_adj.SENSOR[i];
            id++;
			continue;
        }		

    }

    for (id = 0, i = 0; i < SENSOR_NUM_MAX; i++) {
        if (dev_inf.sensor_adj.SENSOR_EN[i] == SENSOR_TYPE_CO2) {
            report.CO2[id].type = STA_ON;
            report.CO2[id].value = sta_in.sensor_adj.SENSOR[i];
            id++;
        }
    }

//    for (id = 0, i = 0; i < SENSOR_NUM_MAX; i++) {
//        if (dev_inf.sensor_adj.SENSOR_EN[i] == SENSOR_TYPE_SPEED) {
//            report.CO2[6 + id].type = STA_ON;
//            report.CO2[6 + id].value = sta_in.sensor_adj.SENSOR[i];
//            id++;
//        }
//    }

    for (id = 0, i = 0; i < SENSOR_NUM_MAX; i++) {
        if (dev_inf.sensor_adj.SENSOR_EN[i] == SENSOR_TYPE_NH3) {
            report.NH3[id].type = STA_ON;
            report.NH3[id].value = sta_in.sensor_adj.SENSOR[i];
            id++;
        }
    }

    for (id = 0, i = 0; i < SENSOR_NUM_MAX; i++) {
        if (dev_inf.sensor_adj.SENSOR_EN[i] == SENSOR_TYPE_H2S) {
            report.H2S[id].type = STA_ON;
            report.H2S[id].value = sta_in.sensor_adj.SENSOR[i];
            id++;
        }
    }

    for (id = 0, i = 0; i < SENSOR_NUM_MAX; i++) {
        if (id < 2 && dev_inf.sensor_adj.SENSOR_EN[i] == SENSOR_TYPE_SPEED) {
            report.CO2[6 + id].type = STA_ON;
            report.CO2[6 + id].value = sta_in.sensor_adj.SENSOR[i];
            id++;
        }
    }

    if (id == 0) { //没有启用风速传感器才用计算值
        if (cal_inf.SEN_EN[SENSOR_TYPE_SPEED] != STA_ON && basic_inf.humtemp_en != STA_ON) {
            report.CO2[6].type = STA_OFF;//复用
            report.CO2[6].value = SENSOR_ER_VALUE;
        } else {
            report.CO2[6].type = STA_ON;//复用
            report.CO2[6].value = cal_inf.wind_speed * SPEED_RATE;
        }
    }

    report.H2S[2].type = dev_inf.env_dev[MB_DEV_PM2_5].enable;//复用
    report.H2S[2].value = sta_in.pm2_5;
    report.H2S[3].type = dev_inf.env_dev[MB_DEV_PM2_5].enable;//复用
    report.H2S[3].value = sta_in.pm10;

    for (id = 0, i = 0; i < SENSOR_NUM_MAX; i++) {
        if (dev_inf.sensor_adj.SENSOR_EN[i] == SENSOR_TYPE_VAC) {
            report.VAC[id].type = STA_ON;
            report.VAC[id].value = sta_in.sensor_adj.SENSOR[i];
            id++;
        }
    }

    report.LIGHT[0].type = dev_inf.env_dev[MB_DEV_LIGHT].enable;
    report.LIGHT[0].value = sta_in.light;

    for (i = 0; i < MAX_REPORT_PWM; i++) {
        if (i < PWM_NUM_MAX) {
            if (dev_inf.SWIN.detail[i].enable == STA_ON) {
                report.PWM[i].type  = dev_inf.SWIN.detail[i].enable * PWM_STA_SWIN;
            } else if (i == IM_PWM3_PHEAT && basic_inf.pheat_en == STA_ON) {
                report.PWM[i].type  = basic_inf.pheat_en * PWM_STA_HEAT;
            } else if (i < APWM_NUM_MAX && basic_inf.area_pwm[i].enable == STA_ON) {
                report.PWM[i].type  = basic_inf.area_pwm[i].enable * PWM_STA_AFAN;
            } else {
                report.PWM[i].type  =  dev_inf.PWM[i].enable * PWM_STA_FAN;
            }

            report.PWM[i].value = sta_out.ALLPWM[i];
            report.PWM[i].time  = sta_out.PWM_TIME[i] / MIN_TO_SEC;
        } else if (i == PWM_NUM_MAX) {
            if (dev_inf.SWIN.detail[i].enable == STA_ON) {
                report.PWM[i].type  = dev_inf.SWIN.detail[i].enable * PWM_STA_SWIN;
            } else {
                report.PWM[i].type  = basic_inf.tempc_pwm_en.enable * PWM_STA_TEMPC;
            }

            report.PWM[i].value = sta_out.ALLPWM[i];
            report.PWM[i].time  = sta_out.PWM_TIME[i] / MIN_TO_SEC;
        } else if (i == PWM_NUM_MAX + 1) {
            if (dev_inf.SWIN.detail[i].enable == STA_ON) {
                report.PWM[i].type  = dev_inf.SWIN.detail[i].enable * PWM_STA_SWIN;
            } else {
                report.PWM[i].type  = dev_inf.light.enable * PWM_STA_LIGHT;
            }

            report.PWM[i].value = sta_out.ALLPWM[i];
            report.PWM[i].time  = sta_out.PWM_TIME[i] / MIN_TO_SEC;
        } else {
            report.PWM[i].type  = 0;
            report.PWM[i].value = 0;
            report.PWM[i].time  = 0;
        }
    }

    for (i = 0; i < MAX_REPORT_JDQ; i++) {
        if (i < ALLJDQ_NUM_MAX) {
            report.JDQ[i].type = JDQ_Update_Swtich(i);

            if ((dev_inf.WIN.detail[0].enable == STA_ON && i == JDQ_WIN1_DOWN)
                    || (dev_inf.WIN.detail[1].enable == STA_ON && i == JDQ_WIN2_DOWN)
                    || (dev_inf.WIN.detail[2].enable == STA_ON && i == JDQ_WIN3_DOWN)
                    || (dev_inf.WIN.detail[3].enable == STA_ON && i == JDQ_WIN4_DOWN)) {
                report.JDQ[i].value = 0;
                report.JDQ[i].time = sta_out.JDQ_TIME[i];//按秒计算用来观察动作情况
                continue;
            } else if ((dev_inf.WIN.detail[0].enable == STA_ON && i == JDQ_WIN1_UP)
                       || (dev_inf.WIN.detail[1].enable == STA_ON && i == JDQ_WIN2_UP)) {
                report.JDQ[i].value = sta_out.WIN[(i - JDQ_WIN1_UP) / 2];
            } else if ((dev_inf.WIN.detail[2].enable == STA_ON && i == JDQ_WIN3_UP)
                       || (dev_inf.WIN.detail[3].enable == STA_ON && i == JDQ_WIN4_UP)) {
                report.JDQ[i].value = sta_out.WIN[2 + (i - JDQ_WIN3_UP) / 2];
            } else {
                report.JDQ[i].value = sta_out.ALLJDQ[i];
            }

            report.JDQ[i].time = sta_out.JDQ_TIME[i] / MIN_TO_SEC;
        } else {
            report.JDQ[i].type = 0;
            report.JDQ[i].value = 0;
            report.JDQ[i].time = 0;
        }
    }

    report.ALAM_sys1 = 0;
    report.ALAM_sys1 = ALA_Get_State(ALA_LNH3) | (report.ALAM_sys1 << 1);
    report.ALAM_sys1 = ALA_Get_State(ALA_HNH3) | (report.ALAM_sys1 << 1);
    report.ALAM_sys1 = ALA_Get_State(ALA_LCO2) | (report.ALAM_sys1 << 1);
    report.ALAM_sys1 = ALA_Get_State(ALA_HCO2) | (report.ALAM_sys1 << 1);
    report.ALAM_sys1 = ALA_Get_State(ALA_LHUM) | (report.ALAM_sys1 << 1);
    report.ALAM_sys1 = ALA_Get_State(ALA_HHUM) | (report.ALAM_sys1 << 1);
    report.ALAM_sys1 = ALA_Get_State(ALA_LTEMP) | (report.ALAM_sys1 << 1);
    report.ALAM_sys1 = ALA_Get_State(ALA_HTEMP) | (report.ALAM_sys1 << 1);
    report.ALAM_sys2 = 0;
    report.ALAM_sys2 = ALA_Get_State(ALA_LVAC) | (report.ALAM_sys2 << 1);
    report.ALAM_sys2 = ALA_Get_State(ALA_HVAC) | (report.ALAM_sys2 << 1);

    report.ALAM_temp = (ALA_State() >> ALA_ETEMP1) & 0x00FF;
    ala_temp = (ALA_State() >> ALA_ATEMP1) & 0x00FF;
    report.ALAM_temp = (ala_temp << 8) + report.ALAM_temp;

    report.ALAM_hum = ALA_Get_State(ALA_EHUM);

    if (ALA_Get_State(ALA_AHTEMP) == ALA_STA_ON) {
        setbit(report.ALAM_hum, 4);
    } else {
        clrbit(report.ALAM_hum, 4);
    }

    if (ALA_Get_State(ALA_ALTEMP) == ALA_STA_ON) {
        setbit(report.ALAM_hum, 5);
    } else {
        clrbit(report.ALAM_hum, 5);
    }

    if (ALA_Get_State(ALA_SWHTEMP) == ALA_STA_ON) {
        setbit(report.ALAM_hum, 6);
    } else {
        clrbit(report.ALAM_hum, 6);
    }

    if (ALA_Get_State(ALA_SWLTEMP) == ALA_STA_ON) {
        setbit(report.ALAM_hum, 7);
    } else {
        clrbit(report.ALAM_hum, 7);
    }

    report.ALAM_CO2 = ALA_Get_State(ALA_ECO2);

    if (ALA_Get_State(ALA_ESPEED) == ALA_STA_ON) {
        setbit(report.ALAM_CO2, 6);
    } else {
        clrbit(report.ALAM_CO2, 6);
    }

    report.ALAM_NH3 = ALA_Get_State(ALA_ENH3);
    report.ALAM_VAC = ALA_Get_State(ALA_EVAC);//+电表

    if (ALA_Get_State(ALA_EENERG) == ALA_STA_ON) {
        setbit(report.ALAM_VAC, 4);
    } else {
        clrbit(report.ALAM_VAC, 4);
    }

    if (ALA_Get_State(ALA_EWATER) == ALA_STA_ON) {
        setbit(report.ALAM_VAC, 5);
    } else {
        clrbit(report.ALAM_VAC, 5);
    }

    if (ALA_Get_State(ALA_EFOOD) == ALA_STA_ON) {
        setbit(report.ALAM_VAC, 6);
    } else {
        clrbit(report.ALAM_VAC, 6);
    }

    report.ALAM_H2S = ALA_Get_State(ALA_EH2S);//+pm2.5

    if (ALA_Get_State(ALA_EPM2_5) == ALA_STA_ON) {
        setbit(report.ALAM_H2S, 2);
    } else {
        clrbit(report.ALAM_H2S, 2);
    }

    if (ALA_Get_State(ALA_HPM2_5) == ALA_STA_ON) {
        setbit(report.ALAM_H2S, 4);
    } else {
        clrbit(report.ALAM_H2S, 4);
    }

    if (ALA_Get_State(ALA_HPM10) == ALA_STA_ON) {
        setbit(report.ALAM_H2S, 6);
    } else {
        clrbit(report.ALAM_H2S, 6);
    }


    report.ALAM_LIGHT = ALA_Get_State(ALA_ELIGHT);

    if (ALA_Get_State(ALA_AFAN) == ALA_STA_ON) {
        setbit(report.ALAM_LIGHT, 5);
    } else {
        clrbit(report.ALAM_LIGHT, 5);
    }

    if (ALA_Get_State(ALA_AFAN_BL) == ALA_STA_ON) {
        setbit(report.ALAM_LIGHT, 6);
    } else {
        clrbit(report.ALAM_LIGHT, 6);
    }

    report.ALAM_OTHER = 0;

    if (ALA_Get_State(ALA_LV) == ALA_STA_ON) {
        setbit(report.ALAM_OTHER, 0);
    } else {
        clrbit(report.ALAM_OTHER, 0);
    }

    if (ALA_Get_State(ALA_WINPOS) == ALA_STA_ON) {
        setbit(report.ALAM_OTHER, 1);
    } else {
        clrbit(report.ALAM_OTHER, 1);
    }

    if (ALA_Get_State(ALA_LEAKAGEW) == ALA_STA_ON) {
        setbit(report.ALAM_OTHER, 2);
    } else {
        clrbit(report.ALAM_OTHER, 2);
    }

//    if (ALA_Get_State(ALA_HWPRESS) == ALA_STA_ON) {
//        setbit(report.ALAM_OTHER, 3);
//    } else {
//        clrbit(report.ALAM_OTHER, 3);
//    }

//    if (ALA_Get_State(ALA_LWPRESS) == ALA_STA_ON) {
//        setbit(report.ALAM_OTHER, 4);
//    } else {
//        clrbit(report.ALAM_OTHER, 4);
//    }

    if (ALA_Get_State(ALA_LACKW) == ALA_STA_ON) {
        setbit(report.ALAM_OTHER, 5);
    } else {
        clrbit(report.ALAM_OTHER, 5);
    }



    report.ALAM_OUT = sta_in.din.alarm;

    if (ALA_Get_State(ALA_POWER) == ALA_STA_ON) {
        report.ALAM_BK = 1;
    } else {
        report.ALAM_BK = 0;
    }

    memcpy(Current_Data_Buf, &report.systime.year, Report_Current_Data_Len);

}


unsigned char Data_Send_Buf[Report_Max_Buffer_Size];//发送处理程序使用
unsigned short Data_Send_Num;//发送个数

//     Packet_Buffer.Result= 0x00;
//  调用该需要先确定结果
void Report_Data_Buffer(unsigned char * Enter_Send_Buffer, unsigned short Packet_Len, unsigned char Report_Commond)
{

    if (Report_Commond == Report_Current_Data_V2) {
        Packet_Buffer.Length = 1;
    } else {
        Packet_Buffer.Length = Packet_Len;
    }

    Packet_Buffer.Commend = Report_Commond;
    Packet_Buffer.Equipmend_ID = basic_inf.id;

    memcpy(&Data_Send_Buf[SRART_BUF_LEN] , Enter_Send_Buffer, Packet_Len);
    memcpy(Data_Send_Buf, &Packet_Buffer.Head1, SRART_BUF_LEN);


    memcpy(&Data_Send_Buf[SRART_BUF_LEN + Packet_Len], &Packet_Buffer.Result, END_BUF_LEN);
    Packet_Buffer.Crc = crc16_s(Data_Send_Buf, SRART_BUF_LEN + 1 + Packet_Len);
    memcpy(&Data_Send_Buf[SRART_BUF_LEN + Packet_Len], &Packet_Buffer.Result, END_BUF_LEN);
    Data_Send_Num = SRART_BUF_LEN + END_BUF_LEN + Packet_Len;
}


extern unsigned char send_Qbox[10];//发生的消息队列指针
TSCOPE	lv_scope_temp[LV_SCOPE_KIND];
unsigned char set_result = 0;
unsigned short set_seq = 0 ;
static int update_len = 0;
void Read_Set_Cmd(unsigned char * Ethernet_Recv_Buf, unsigned short Ethernet_Recv_len)
{
    unsigned char os_err = 0;
    unsigned short Crc_Data, len;
    unsigned char Command_Num, id, i, j;
    Packet_Head Respon_Buffer;
    PUP_FLIE pUpdate_File = NULL;
    TDATE_TIME	dt;
    time_t	t1, t2;
    PPWD	ppwd_temp = NULL;
    int     ret;

    if ((Ethernet_Recv_Buf[0] == 0xA5) && (Ethernet_Recv_Buf[1] == 0X5A)
            && (Ethernet_Recv_Buf[Ethernet_Recv_len - 2] == 0XA5)
            && (Ethernet_Recv_Buf[Ethernet_Recv_len - 1] == 0X0D)) {

        memcpy(&Respon_Buffer.Head1, &Ethernet_Recv_Buf[0], SRART_BUF_LEN + 1);

        if (basic_inf.id != (Ethernet_Recv_Buf[3] + (Ethernet_Recv_Buf[4] << 8))) {
            return ;
        }

        Crc_Data = crc16_s(Ethernet_Recv_Buf, Ethernet_Recv_len - 4);

        if (Crc_Data == (Ethernet_Recv_Buf[Ethernet_Recv_len - 3] << 8) + Ethernet_Recv_Buf[Ethernet_Recv_len - 4] ) {

            Command_Num = Ethernet_Recv_Buf[2];
            sys_sta.net_sta = STA_ON;
            run_inf.NetSta_Time = (3 * basic_inf.update_time + 1) * 60 * RUN_FREQ; // 3次

            if (Command_Num == Report_Current_Data_V2) {
                if (run_inf.clock_in_report == STA_ON) {
                    run_inf.clock_in_report = STA_OFF;
                    run_inf.clock_in_state = STA_OFF;
                }

                sys_sta.net_ack = STA_ON;
                return;
            } else if (Command_Num == Report_Creg_Data_V2) {
                if (Respon_Buffer.Result == 0) {
                    sys_sta.net_creg = STA_ON;
                }

                return;
            }

            if (sys_sta.trial_end == STA_ON && Command_Num != CLEAR_DATA_INFO) {
                return;
            }

            OSSemPend(data_send_sem, 0, &os_err);

            switch (Command_Num) {
            case Report_Current_Data_V2:
                sys_sta.net_ack = STA_ON;
                break;

            case Report_Creg_Data_V2:
                if (Respon_Buffer.Result == 0) {
                    sys_sta.net_creg = STA_ON;
                }

                break;

            case Read_Current_Data_V2:
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;

                if (run_inf.report_time <= 0) {
                    run_inf.report_time = MIN_TO_SEC * RUN_FREQ;
	                if (run_inf.clock_in_report == STA_ON) {
	                    run_inf.clock_in_report = STA_OFF;
	                    run_inf.clock_in_state = STA_OFF;
	                }
					
                    Report_Data_Init(STA_ON);
                } else {
                    Report_Data_Init(STA_OFF);
                }

                Report_Data_Buffer(Current_Data_Buf, Report_Current_Data_Len, Read_Current_Data_V2);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                // OSSemPost(data_send_sem);

                break;

            case HEART_BEAT_DATA:
            case SET_TIME_INFO:
                len = 7;
                Packet_Buffer.Result = Failed_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    id = 0;
                    dt.year = Ethernet_Recv_Buf[SRART_BUF_LEN + id++];
                    dt.mon = Ethernet_Recv_Buf[SRART_BUF_LEN + id++];
                    dt.day = Ethernet_Recv_Buf[SRART_BUF_LEN + id++];
                    dt.hour = Ethernet_Recv_Buf[SRART_BUF_LEN + id++];
                    dt.min = Ethernet_Recv_Buf[SRART_BUF_LEN + id++];
                    dt.sec = Ethernet_Recv_Buf[SRART_BUF_LEN + id++];

                    do {
                        if (Bcd2Hex(dt.year) < 0
                                || Bcd2Hex(dt.mon) < 0 || Bcd2Hex(dt.mon) > 12
                                || Bcd2Hex(dt.day) < 0 || Bcd2Hex(dt.day) > 30
                                || Bcd2Hex(dt.hour) < 0 || Bcd2Hex(dt.hour) > 23
                                || Bcd2Hex(dt.min) < 0 || Bcd2Hex(dt.min) > 59
                                || Bcd2Hex(dt.sec) < 0 || Bcd2Hex(dt.sec) > 59) {
                            break;
                        }

                        if (Command_Num == HEART_BEAT_DATA) { //心跳判断时间差
                            t1 = DT2SysTm( &dt );

                            if ( t1 == (time_t)(-1) ) {
                                break;
                            }

                            t2 = GetSysTime( );

                            if ( fabs(difftime(t1, t2)) < DIFF_SECOND ) {
                                break;
                            }
                        }

                        if (PCF8563_SetDT( &dt ) < 0) {
                            break;
                        }

                        Reset_Sync_Time();

                        Packet_Buffer.Result = Success_Result;
                        Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    } while (0);

                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                if (Command_Num == HEART_BEAT_DATA) break; //心跳不需要回复

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );

                break;

//            case SET_TEMP_V2_INFO:
//                len = sizeof(ctrl_inf.temp_adj) + sizeof(ctrl_inf.temp);

//                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
//                    Packet_Buffer.Result = Success_Result;
//                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
//                    memcpy((INT8U *)&ctrl_inf.temp_adj, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);
//                    //ctrl_inf.temp_en = STA_ON;
//                    ctrl_inf.temp[0].enable = STA_ON;

//                    for (i = 0; i < TEMP_CURVE_NUM; i++) {
//                        if (ctrl_inf.temp[i].heat_temp > ctrl_inf.temp[i].target_temp) {
//                            ctrl_inf.temp[i].heat_temp = ctrl_inf.temp[i].target_temp;
//                        }

//                        if (ctrl_inf.temp[i].vertical_temp < ctrl_inf.temp[i].target_temp) {
//                            ctrl_inf.temp[i].vertical_temp = ctrl_inf.temp[i].target_temp;
//                        }
//                    }

//                    if (Config_Save(STEMP_ID) < 0) {
//                        Packet_Buffer.Result = Failed_Result;
//                    }
//                } else {
//                    Packet_Buffer.Result = Failed_Result;
//                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
//                }

//                Report_Data_Buffer(NULL, 0, Command_Num);
//                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
//                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
//                break;


//            case READ_TEMP_V2_INFO:
//                len = sizeof(ctrl_inf.temp_adj) + sizeof(ctrl_inf.temp);
//                Packet_Buffer.Result = Success_Result;
//                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
//                memcpy(Current_Data_Buf, (INT8U *)&ctrl_inf.temp_adj, len);
//                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
//                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
//                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
//                break;

            case SET_TEMP_V2_1_INFO:
                len = sizeof(ctrl_inf.temp_adj) + sizeof(ctrl_inf.temp) + sizeof(ctrl_inf.extemp);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ctrl_inf.temp_adj, &Ethernet_Recv_Buf[SRART_BUF_LEN], sizeof(ctrl_inf.temp_adj) + sizeof(ctrl_inf.temp));
                    memcpy((INT8U *)&ctrl_inf.extemp, &Ethernet_Recv_Buf[SRART_BUF_LEN + sizeof(ctrl_inf.temp_adj) + sizeof(ctrl_inf.temp)], sizeof(ctrl_inf.extemp));
                    ctrl_inf.temp[0].enable = STA_ON;

                    for (i = 0; i < TEMP_CURVE_NUM; i++) {
                        if (ctrl_inf.temp[i].heat_temp > ctrl_inf.temp[i].target_temp) {
                            ctrl_inf.temp[i].heat_temp = ctrl_inf.temp[i].target_temp;
                        }

                        if (ctrl_inf.temp[i].vertical_temp < ctrl_inf.temp[i].target_temp) {
                            ctrl_inf.temp[i].vertical_temp = ctrl_inf.temp[i].target_temp;
                        }
                    }

                    if (Config_Save(STEMP_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case READ_TEMP_V2_1_INFO:
                len = sizeof(ctrl_inf.temp_adj) + sizeof(ctrl_inf.temp) + sizeof(ctrl_inf.extemp);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ctrl_inf.temp_adj, sizeof(ctrl_inf.temp_adj) + sizeof(ctrl_inf.temp));
                memcpy(&Current_Data_Buf[sizeof(ctrl_inf.temp_adj) + sizeof(ctrl_inf.temp)], (INT8U *)&ctrl_inf.extemp, sizeof(ctrl_inf.extemp));
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_MINAIR_V2_INFO:
                len = sizeof(ctrl_inf.minair_en) + sizeof(ctrl_inf.minair);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ctrl_inf.minair_en, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SMINAIRC_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_MINAIR_V2_INFO:
                len = sizeof(ctrl_inf.minair_en) + sizeof(ctrl_inf.minair);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ctrl_inf.minair_en, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_EXMINAIR_V2_INFO:
                len = sizeof(ctrl_inf.exminair);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ctrl_inf.exminair, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SMINAIRC_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case READ_EXMINAIR_V2_INFO:
                len = sizeof(ctrl_inf.exminair);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ctrl_inf.exminair, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_HUM_V2_INFO:
                len = sizeof(ctrl_inf.hum);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ctrl_inf.hum, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SHUM_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_HUM_V2_INFO:
                len = sizeof(ctrl_inf.hum);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ctrl_inf.hum, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


//            case SET_LV_V2_INFO:
//                len = sizeof(ctrl_inf.lv) / CTRL_LV_NUM;
//                id = Ethernet_Recv_Buf[SRART_BUF_LEN];

//                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] - 1 == len && id >= 1 && id <= 30) {
//                    Packet_Buffer.Result = Success_Result;
//                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
//                    memcpy((INT8U *)&ctrl_inf.lv[id - 1], &Ethernet_Recv_Buf[SRART_BUF_LEN + 1], len);

//                    if (id - 1 == 0) {
//                        ctrl_inf.lv[0].enable = STA_ON;
//                        ctrl_inf.lv[0].offset = 0;
//                        ctrl_inf.lv[0].re_diff = 0;
//                    }

//                    if (Config_Save(SLVPWM_ID) < 0) {
//                        Packet_Buffer.Result = Failed_Result;
//                    }
//                } else {
//                    Packet_Buffer.Result = Failed_Result;
//                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
//                }

//                Report_Data_Buffer(NULL, 0, Command_Num);
//                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
//                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
//                break;


//            case READ_LV_V2_INFO:
//                len = sizeof(ctrl_inf.lv) / CTRL_LV_NUM;
//                id = Ethernet_Recv_Buf[SRART_BUF_LEN];

//                if (id >= 1 && id <= 30) {
//                    Packet_Buffer.Result = Success_Result;
//                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
//                    Current_Data_Buf[0] = id;
//                    memcpy(&Current_Data_Buf[1], (INT8U *)&ctrl_inf.lv[id - 1], len);
//                    Report_Data_Buffer(Current_Data_Buf, len + 1, Command_Num);
//                } else {
//                    Packet_Buffer.Result = Failed_Result;
//                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
//                    Report_Data_Buffer(NULL, 0, Command_Num);
//                }

//                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
//                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
//                break;

            case SET_LV_V2_1_INFO:
                len = sizeof(TLV) + sizeof(EXLV);
                id = Ethernet_Recv_Buf[SRART_BUF_LEN];

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] - 1 == len && id >= 1 && id <= 30) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ctrl_inf.lv[id - 1], &Ethernet_Recv_Buf[SRART_BUF_LEN + 1], sizeof(TLV));
                    memcpy((INT8U *)&ctrl_inf.exlv[id - 1], &Ethernet_Recv_Buf[SRART_BUF_LEN + 1 + sizeof(TLV)], sizeof(EXLV));

                    if (id - 1 == 0) {
                        ctrl_inf.lv[0].enable = STA_ON;
                        ctrl_inf.lv[0].offset = 0;
                    }

                    if (ctrl_inf.lv[id - 1].PWM[0].on_time == 0) {
                        ctrl_inf.lv[id - 1].PWM[0].on_time = 1;
                    }

                    if (Config_Save(SLVPWM_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_LV_V2_1_INFO:
                len = sizeof(TLV) + sizeof(EXLV);
                id = Ethernet_Recv_Buf[SRART_BUF_LEN];

                if (id >= 1 && id <= 30) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    Current_Data_Buf[0] = id;
                    memcpy(&Current_Data_Buf[1], (INT8U *)&ctrl_inf.lv[id - 1], sizeof(TLV));
                    memcpy(&Current_Data_Buf[1 + sizeof(TLV)], (INT8U *)&ctrl_inf.exlv[id - 1], sizeof(EXLV));
                    Report_Data_Buffer(Current_Data_Buf, len + 1, Command_Num);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    Report_Data_Buffer(NULL, 0, Command_Num);
                }

                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_LVMORE_V2_INFO:
                len = sizeof(ctrl_inf.lv_more);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ctrl_inf.lv_more, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SLVMORE_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_LVMORE_V2_INFO:
                len = sizeof(ctrl_inf.lv_more);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ctrl_inf.lv_more, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case SET_DAYTEMP_V2_INFO:
                len = sizeof(ext_inf.day_temp);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ext_inf.day_temp, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SDAYTEMP_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( UART6_COMM, localTxBuf, Data_Send_Num );
                break;


            case READ_DAYTEMP_V2_INFO:
                len = sizeof(ext_inf.day_temp);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ext_inf.day_temp, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( UART6_COMM, localTxBuf, Data_Send_Num );
                break;


            case READ_LVAIR_V2_INFO:
                len = sizeof(cal_inf.lv_air);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&cal_inf.lv_air, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_SENSOR_V2_INFO:
                len = sizeof(dev_inf.sensor_adj);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&dev_inf.sensor_adj, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SSENSOR_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
					
					AIN_WIN_CHK(0);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_SENSOR_V2_INFO:
                len = sizeof(dev_inf.sensor_adj);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.sensor_adj, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_TEMP_DEF_V2_INFO:
                len = 60;

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&dev_inf.TEMP_Define, &Ethernet_Recv_Buf[SRART_BUF_LEN], sizeof(dev_inf.TEMP_Define));

                    if (Config_Save(STEMP_DEF_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_TEMP_DEF_V2_INFO:
                len = 60;
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memset(Current_Data_Buf, 0, len);
                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.TEMP_Define, sizeof(dev_inf.TEMP_Define));
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_PWM_V2_INFO:
                len = sizeof(dev_inf.PWM);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&dev_inf.PWM[0], &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SPWM_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    PWM_SWIN_CHK(0);
                    PWM3_PHEAT_CHK(0);
                    PWM_APWM_CHK(0);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_PWM_V2_INFO:
                len = sizeof(dev_inf.PWM);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.PWM[0], len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_JDQ_V2_INFO:
                len = sizeof(dev_inf.JDQ);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&dev_inf.JDQ[0], &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SJDQ_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    JDQ_MulFan_CHK();
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_JDQ_V2_INFO:
                len = sizeof(dev_inf.JDQ);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.JDQ[0], len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_WATER_V2_INFO:
                len = sizeof(dev_inf.wet_en) + sizeof(dev_inf.wet);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&dev_inf.wet_en, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SWET_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    JDQ_MulOther_CHK(JDQ_WET);
                    JDQ_MulOther_CHK(JDQ_WET2);
                    Wet_Spary_Mul(IM_WET_SPARY);

                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_WATER_V2_INFO:
                len = sizeof(dev_inf.wet_en) + sizeof(dev_inf.wet);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.wet_en, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_SPRAY_V2_INFO:
                len = sizeof(dev_inf.spray_en) + sizeof(dev_inf.spray) + 1;// 多一个工作模式

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;

                    if (Ethernet_Recv_Buf[SRART_BUF_LEN] == 0) {
                        dev_inf.spray_en = SPRAY_MODE_OFF;
                    } else {
                        if (Ethernet_Recv_Buf[SRART_BUF_LEN + 1] == 0) {
                            dev_inf.spray_en = SPRAY_MODE_HUM;
                        } else {
                            dev_inf.spray_en = SPRAY_MODE_TEMP;
                        }
                    }

                    memcpy((INT8U *)&dev_inf.spray[0].enable, &Ethernet_Recv_Buf[SRART_BUF_LEN + 2], sizeof(dev_inf.spray));

                    if (Config_Save(SSPRAY_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    JDQ_MulOther_CHK(JDQ_SPRAY);
                    Wet_Spary_Mul(IM_SPARY_WET);

                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_SPRAY_V2_INFO:
                len = sizeof(dev_inf.spray_en) + sizeof(dev_inf.spray) + 1;
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;

                if (dev_inf.spray_en == SPRAY_MODE_OFF) {
                    Current_Data_Buf[0] = 0;
                    Current_Data_Buf[1] = 0;
                } else if (dev_inf.spray_en == SPRAY_MODE_HUM) {
                    Current_Data_Buf[0] = 1;
                    Current_Data_Buf[1] = 0;
                } else {
                    Current_Data_Buf[0] = 1;
                    Current_Data_Buf[1] = 1;
                }

                memcpy((INT8U *)&Current_Data_Buf[2], (INT8U *)&dev_inf.spray[0].enable, sizeof(dev_inf.spray));
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_LIGHT_V2_INFO:
                len = sizeof(dev_inf.light);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&dev_inf.light, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SLIGHT_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    Light_SWIN_CHK(0);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_LIGHT_V2_INFO:
                len = sizeof(dev_inf.light);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.light, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_HEAT_V2_INFO:
                len = sizeof(basic_inf.heat_en) + sizeof(basic_inf.heat);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)(len)) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&basic_inf.heat_en, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SHEAT_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    JDQ_MulOther_CHK(JDQ_HEAT1);
                    JDQ_MulOther_CHK(JDQ_HEAT2);
                    JDQ_MulOther_CHK(JDQ_HEAT3);

                    Heat1_Timer3_Mul(IM_HEAT1_TIMER3);
                    Heat2_Timer4_Mul(IM_HEAT2_TIMER4);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_HEAT_V2_INFO:
                len = sizeof(basic_inf.heat_en) + sizeof(basic_inf.heat);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&basic_inf.heat_en, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_PHEAT_V2_INFO:
                len = sizeof(basic_inf.pheat_en) + sizeof(basic_inf.pheat);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)(len)) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&basic_inf.pheat_en, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SHEAT_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    PWM3_PHEAT_CHK(1);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_PHEAT_V2_INFO:
                len = sizeof(basic_inf.pheat_en) + sizeof(basic_inf.pheat);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&basic_inf.pheat_en, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_TEMPC_V2_INFO:
                len = sizeof(basic_inf.tempc_en) + sizeof(basic_inf.tempc);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)(len)) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&basic_inf.tempc_en, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(STEMPC_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    Tempc_Timer2_Mul(IM_TEMPC_TIMER2);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_TEMPC_V2_INFO:
                len = sizeof(basic_inf.tempc_en) + sizeof(basic_inf.tempc);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&basic_inf.tempc_en, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_TEMPC_PWM_V2_INFO:
                len = sizeof(basic_inf.tempc_pwm_en) + sizeof(basic_inf.tempc_pwm);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)(len)) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&basic_inf.tempc_pwm_en, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(STEMPC_PWM_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    Tempc_SWIN_CHK(0);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_TEMPC_PWM_V2_INFO:
                len = sizeof(basic_inf.tempc_pwm_en) + sizeof(basic_inf.tempc_pwm);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&basic_inf.tempc_pwm_en, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_APWM_V2_INFO:
                len = sizeof(basic_inf.area_pwm);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)(len)) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&basic_inf.area_pwm[0].enable, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SAPWM_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    PWM_APWM_CHK(1);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_APWM_V2_INFO:
                len = sizeof(basic_inf.area_pwm);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&basic_inf.area_pwm[0].enable, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_FAN_CUR_V2_INFO:
                len = sizeof(dev_inf.fan_chk);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)(len)) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&dev_inf.fan_chk.enable, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SFAN_CHK_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_FAN_CUR_V2_INFO:
                len = sizeof(dev_inf.fan_chk);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                dev_inf.fan_chk.energy_sta = ext_inf.din.pro_data[0].energy_sta;
                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.fan_chk.enable, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case READ_OPEN_RECODE_INFO:
                id = 0;
                memset(Current_Data_Buf, 0, sizeof(Current_Data_Buf));
                memcpy(Current_Data_Buf, (INT8U *)&index_inf.start_time, sizeof(index_inf.start_time));
                id += sizeof(index_inf.start_time);

                if (index_inf.start_time / DATA_RECORD_MAX < 1) {
                    i = index_inf.start_time;
                } else {
                    i = DATA_RECORD_MAX;
                    ret = index_inf.start_time % DATA_RECORD_MAX;
                }

                for (; i > 0; i --) {
                    if (index_inf.start_time / DATA_RECORD_MAX < 1) {
                        ret = i;
                    } else {
                        if (--ret < 0) {
                            ret = DATA_RECORD_MAX - 1;
                        }
                    }

                    memcpy(&Current_Data_Buf[id], (INT8U *) &index_inf.start_t[ret], 4);
                    id += 4;
                }

                len = 426;
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
//                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.fan_chk.enable, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case CLEAR_DATA_INFO:
                len = sizeof(sys_sta.clear);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)(len)) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&sys_sta.clear, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (sys_sta.clear.start_time == STA_ON) {
                        index_inf.start_time = 0;
                    }

                    if (sys_sta.clear.restart_time == STA_ON) {
                        index_inf.restart_time = 0;
                    }

                    if (sys_sta.clear.run_time == STA_ON) {
                        index_inf.run_time = 0;
                    }

                    if (sys_sta.clear.reset_en == STA_ON) {
                        sys_sta.delay_rst = 10;
                    }

                    if (sys_sta.clear.new_group_en == STA_ON) {
                        basic_inf.group++;

                        if (basic_inf.group >= MAX_GROUP_NUM) {
                            basic_inf.group = 1;
                        }

                        basic_inf.survival = 0;
                        basic_inf.total = 0;
                        Group_Def();
                    }

                    if (sys_sta.clear.reset_group_en == STA_ON) {
                        basic_inf.group = 1;
                    }

                    if (sys_sta.clear.work_mode == 1) {
                        ctrl_inf.var.mode = STA_OFF;
                    } else if (sys_sta.clear.work_mode == 2) {
                        ctrl_inf.var.mode = STA_ON;
                    }

                    if (sys_sta.clear.clear_water == STA_ON) {
                        memset(index_inf.water, 0, sizeof(index_inf.water));
                    }

                    if (sys_sta.clear.clear_food == STA_ON) {
                        memset(index_inf.food, 0, sizeof(index_inf.food));
                    }

                    if (sys_sta.clear.trial_en == STA_ON) {
                        Trial_Clear(sys_sta.clear.trial_time);
                    }

                    if (Config_Save(SINDEX_ID) < 0 || Config_Save(SBASIC_ID) < 0
                            || Config_Save(SVAR_ID) < 0 ) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_WIN_V2_INFO:
                len = sizeof(dev_inf.SWIN) + sizeof(dev_inf.WIN);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&dev_inf.SWIN, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SSWIN_ID) < 0 || Config_Save(SWIN_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }


                    PWM_SWIN_CHK(1);
                    PWM3_PHEAT_CHK(0);
                    PWM_APWM_CHK(0);
                    Light_SWIN_CHK(1);
                    Tempc_SWIN_CHK(1);
					AIN_WIN_CHK(1);

                    JDQ_MulOther_CHK(JDQ_WIN1_UP);
                    JDQ_MulOther_CHK(JDQ_WIN1_DOWN);
                    JDQ_MulOther_CHK(JDQ_WIN2_UP);
                    JDQ_MulOther_CHK(JDQ_WIN2_DOWN);
                    JDQ_MulOther_CHK(JDQ_WIN3_UP);
                    JDQ_MulOther_CHK(JDQ_WIN3_DOWN);
                    JDQ_MulOther_CHK(JDQ_WIN4_UP);
                    JDQ_MulOther_CHK(JDQ_WIN4_DOWN);

                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_WIN_V2_INFO:
                len = sizeof(dev_inf.SWIN) + sizeof(dev_inf.WIN);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.SWIN, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_SG_V2_INFO:
                len = sizeof(ext_inf.din);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ext_inf.din, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    pluse_water_chk_alarm();
                    alarm_chk_pluse_water();

                    if (Config_Save(SDIN_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_SG_V2_INFO:
                len = sizeof(ext_inf.din);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ext_inf.din, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_CIRCLE_V2_INFO:
                len = sizeof(dev_inf.circle);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&dev_inf.circle, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SCIRCLE_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    JDQ_MulOther_CHK(JDQ_CIR_FAN);
                    Circle_Timer1_Mul(IM_CIRCLE_TIMER1);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_CIRCLE_V2_INFO:
                len = sizeof(dev_inf.circle);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.circle, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case SET_TIMER_V2_INFO:
                len = sizeof(ext_inf.timer);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ext_inf.timer, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(STIMER_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                    JDQ_MulOther_CHK(JDQ_TIMER1);
                    JDQ_MulOther_CHK(JDQ_TIMER2);
                    JDQ_MulOther_CHK(JDQ_TIMER3);
                    JDQ_MulOther_CHK(JDQ_TIMER4);
                    Circle_Timer1_Mul(IM_TIMER1_CIRCLE);
                    Tempc_Timer2_Mul(IM_TIMER2_TEMPC);
                    Heat1_Timer3_Mul(IM_TIMER3_HEAT1);
                    Heat2_Timer4_Mul(IM_TIMER4_HEAT2);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_TIMER_V2_INFO:
                len = sizeof(ext_inf.timer);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ext_inf.timer, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_BASIC_INFO:
                len = OFFSETOF(TBASIC, id) - sizeof(basic_inf.wMagic) + 8;

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&basic_inf.group, &Ethernet_Recv_Buf[SRART_BUF_LEN], (len - 8));
                    mycpy((INT8U *)&basic_inf.survival, &Ethernet_Recv_Buf[SRART_BUF_LEN + (len - 8)], 8);

                    if (basic_inf.dayupdate_time > (MAX_TIME_DAY - 29)) {
                        basic_inf.dayupdate_time = 510;
                    }

                    if (basic_inf.dataupdate_time > (MAX_TIME_DAY - 29)) {
                        basic_inf.dataupdate_time = 510;
                    }

                    if (Config_Save(SBASIC_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case READ_BASIC_INFO:
                len = OFFSETOF(TBASIC, id) - sizeof(basic_inf.wMagic) + 8;
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&basic_inf.group, (len - 8));
                mycpy(&Current_Data_Buf[(len - 8)], (INT8U *)&basic_inf.survival, 8);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_ASLV_V2_INFO:
                id = Ethernet_Recv_Buf[SRART_BUF_LEN];
                len = sizeof(TSCOPE) + 1;

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)(len) && id < LV_SCOPE_KIND) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&lv_scope_temp[id], &Ethernet_Recv_Buf[SRART_BUF_LEN + 1], len - 1);

                    for (j = 0; j < LV_SCOPE_NUM; j++) {
                        if (lv_scope_temp[id].curve[j].min_lv != 0)lv_scope_temp[id].curve[j].min_lv--;

                        if (lv_scope_temp[id].curve[j].max_lv != 0)lv_scope_temp[id].curve[j].max_lv--;

                        if (lv_scope_temp[id].curve[j].cold_lv != 0)lv_scope_temp[id].curve[j].cold_lv--;

                        if (lv_scope_temp[id].curve[j].warm_lv != 0)lv_scope_temp[id].curve[j].warm_lv--;
                    }

                    memcpy((INT8U *)&ctrl_inf.lv_scope[id], (INT8U *)&lv_scope_temp[id], len - 1);

                    if (Config_Save(SSCOPE_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_ASLV_V2_INFO:
                id = Ethernet_Recv_Buf[SRART_BUF_LEN];
                len = sizeof(TSCOPE);

                if (id < LV_SCOPE_KIND) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&lv_scope_temp[id], (INT8U *)&ctrl_inf.lv_scope[id], len);

                    for (j = 0; j < LV_SCOPE_NUM; j++) {
                        lv_scope_temp[id].curve[j].min_lv++;
                        lv_scope_temp[id].curve[j].max_lv++;
                        lv_scope_temp[id].curve[j].cold_lv++;
                        lv_scope_temp[id].curve[j].warm_lv++;
                    }

                    Current_Data_Buf[0] = id;
                    memcpy(&Current_Data_Buf[1], (INT8U *)&lv_scope_temp[id], len);
                    Report_Data_Buffer(Current_Data_Buf, len + 1, Command_Num);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    Report_Data_Buffer(NULL, 0, Command_Num);
                }

                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_OTSEN_V2_INFO:
                id = Ethernet_Recv_Buf[SRART_BUF_LEN];
                len = sizeof(TSENCON) + 1;

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == (u8)len && id < SENCON_NUM) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ctrl_inf.sencon[id], &Ethernet_Recv_Buf[SRART_BUF_LEN + 1], len - 1);

                    if (Config_Save(SSENCON_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_OTSEN_V2_INFO:
                id = Ethernet_Recv_Buf[SRART_BUF_LEN];
                len = sizeof(TSENCON);

                if (id < SENCON_NUM) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    Current_Data_Buf[0] = id;
                    memcpy(&Current_Data_Buf[1], (INT8U *)&ctrl_inf.sencon[id], len);
                    Report_Data_Buffer(Current_Data_Buf, len + 1, Command_Num);
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    Report_Data_Buffer(NULL, 0, Command_Num);
                }

                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_VAC_V2_INFO:
                len = sizeof(ctrl_inf.vac);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ctrl_inf.vac, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (ctrl_inf.vac.alaL > 100 || ctrl_inf.vac.alaL < 0)	ctrl_inf.vac.alaL = 0;

                    if (ctrl_inf.vac.alaH > 100 || ctrl_inf.vac.alaH < 0)	ctrl_inf.vac.alaH = 35;

                    if (Config_Save(SVAC_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_VAC_V2_INFO:
                len = sizeof(ctrl_inf.vac);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ctrl_inf.vac, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case READ_WIN_DEGREE_INFO:
                len = sizeof(ext_inf.lv_save);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ext_inf.lv_save, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_VAR_V2_INFO:
                len = sizeof(ctrl_inf.var);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&ctrl_inf.var, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SVAR_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_VAR_V2_INFO:
                len = sizeof(ctrl_inf.var);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&ctrl_inf.var, len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_ENV_V2_INFO:
                len = sizeof(dev_inf.env_dev) + sizeof(dev_inf.limit);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    memcpy((INT8U *)&dev_inf.env_dev[0], &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (Config_Save(SENV_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }

                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_ENV_V2_INFO:
                len = sizeof(dev_inf.env_dev) + sizeof(dev_inf.limit);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                memcpy(Current_Data_Buf, (INT8U *)&dev_inf.env_dev[0], len);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_PWD_V2_INFO:
                len = sizeof(TPWD);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    ppwd_temp = (PPWD)(&Ethernet_Recv_Buf[SRART_BUF_LEN]);

                    if (ppwd_temp->pwd1_type == STA_ON) {
                        memcpy(basic_inf.supadmin_pwd, ppwd_temp->pwd1, PWD_LEN);
                        basic_inf.supadmin_pwd[PWD_LEN] = 0;
                    }

                    if (ppwd_temp->pwd2_type == STA_ON) {
                        memcpy(basic_inf.admin_pwd, ppwd_temp->pwd2, PWD_LEN);
                        basic_inf.admin_pwd[PWD_LEN] = 0;
                    }

                    if (Config_Save(SBASIC_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;


            case READ_PWD_V2_INFO:
                len = sizeof(TPWD);
                Packet_Buffer.Result = Success_Result;
                Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                id = 0;
                Current_Data_Buf[id++] = 0;
                memcpy(&Current_Data_Buf[id], basic_inf.supadmin_pwd, PWD_LEN);
                id += PWD_LEN;
                Current_Data_Buf[id++] = 0;
                memcpy(&Current_Data_Buf[id], basic_inf.admin_pwd, PWD_LEN);
                Report_Data_Buffer(Current_Data_Buf, len, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_UPDATE_ST:
                len = sizeof(update_st) - sizeof(update_st.wMagic);

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;

                    memcpy(&update_st.sta, &Ethernet_Recv_Buf[SRART_BUF_LEN], len);

                    if (update_st.type > UP_VER_CUR || update_st.len >= UP_MAX_SIZE
                            || (update_st.sta == UP_END && update_len != update_st.len) ) {
                        update_st.sta = UP_END;
                        Packet_Buffer.Result = Failed_Result;
                        Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                        Report_Data_Buffer(NULL, 0, Command_Num);
                        memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                        SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                        break;
                    }

                    if (update_st.sta == UP_END) {
                        if (update_st.type == UP_VER_BAK) {
                            SPI_Flash_Write((INT8U *)&update_st.len, soft_bak_addr , sizeof(update_st.len));
                        } else {
                            SPI_Flash_Write((INT8U *)&update_st.len, soft_cur_addr , sizeof(update_st.len));
                        }

                        if (Config_Save(SSOFT_INIT_ID) < 0) {
                            Packet_Buffer.Result = Failed_Result;
                        } else {
                            sys_sta.delay_rst = 10;
                        }
                    } else {
                        update_len = update_st.len;
                    }

                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_UPDATE_FILE:
                //长度不判断
                pUpdate_File = (PUP_FLIE)(&Ethernet_Recv_Buf[SRART_BUF_LEN]);

                if (pUpdate_File->len > UP_MAX_SIZE || update_st.sta != UP_START) {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    Report_Data_Buffer(NULL, 0, Command_Num);
                    memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                    SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                    break;
                }

                if (Update_SaveData(update_st.type, Respon_Buffer.Send_Seq, pUpdate_File->len, pUpdate_File->buf) > 0) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            case SET_UPDATE_CHG:

                len = 1;//程序类型

                if (Ethernet_Recv_Buf[SRART_BUF_LEN - 1] == len && Ethernet_Recv_Buf[SRART_BUF_LEN] <= UP_VER_CUR) {
                    Packet_Buffer.Result = Success_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                    update_st.type = Ethernet_Recv_Buf[SRART_BUF_LEN];
                    update_st.sta = UP_END;

                    if (update_st.type == UP_VER_BAK) {
                        SPI_Flash_Read((INT8U *)&update_st.len, soft_bak_addr , sizeof(update_st.len));
                    } else {
                        SPI_Flash_Read((INT8U *)&update_st.len, soft_cur_addr , sizeof(update_st.len));
                    }

                    if (Config_Save(SSOFT_INIT_ID) < 0) {
                        Packet_Buffer.Result = Failed_Result;
                    } else {
                        sys_sta.delay_rst = 10;
                    }
                } else {
                    Packet_Buffer.Result = Failed_Result;
                    Packet_Buffer.Send_Seq = Respon_Buffer.Send_Seq;
                }

                Report_Data_Buffer(NULL, 0, Command_Num);
                memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
                break;

            default:
                break;
            }

            OSSemPost(data_send_sem);
        }
    }

}


void Comm_Rec_Task(void * pdata)
{
    u8 os_err = 0;
    u16 bytes = 0;

    data_send_sem = OSSemCreate(1);
    //Report_Current_Data_Len = sizeof(report);

    for (;;) {


        os_err = WaitUartIdle( USARTCOMM, 0 );

        if (os_err == OS_ERR_NONE) { //没有超时
            bytes = GetUartData( USARTCOMM, localRxBuf, sizeof(localRxBuf) );

            if (bytes > 2) {
                Read_Set_Cmd(localRxBuf, bytes);
            }
        }

        OSTimeDlyHMSM(0, 0, 0, 10);
    }
}

void Creg_Data_Init(void)
{
    memcpy(Current_Data_Buf, Get_McuID(), Get_McuID_Len());
}


void Update_Task(void * pdtat)
{
    u8 os_err;
    static int heart_time = 0;
	u8 send_buf[2];
    Report_Current_Data_Len = sizeof(report);

    for (;;) {
//定时发送
        task_time[Update_id] = 0;

        if (basic_inf.update_mode == UPDATA_SLAVE) {
            OSTimeDlyHMSM(0, 1, 0, 0);
            continue;
        }

        if (soft_inf.creg_en == STA_ON && sys_sta.net_creg == STA_OFF) {
            OSSemPend(data_send_sem, 0, &os_err);
            Packet_Buffer.Result = Success_Result;
            Packet_Buffer.Send_Seq = 0x01;
            Creg_Data_Init();
            Report_Data_Buffer(Current_Data_Buf, Get_McuID_Len(), Report_Creg_Data_V2);
            memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
            SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
            OSSemPost(data_send_sem);
            OSTimeDlyHMSM(0, 1, 0, 0);
            continue;
        }

        if (sys_sta.on_sec >= (sys_sta.net_cycletime + basic_inf.update_time * MIN_TO_SEC)
                && sys_sta.on_sec >= 2 * MIN_TO_SEC) {
            sys_sta.net_cycletime = sys_sta.on_sec;
            sys_sta.net_ack = STA_OFF;
            sys_sta.net_times = 0;
            OSSemPend(data_send_sem, 0, &os_err);
            Packet_Buffer.Result = Success_Result;
            Packet_Buffer.Send_Seq++;
            Report_Data_Init(STA_ON);
            Report_Data_Buffer(Current_Data_Buf, Report_Current_Data_Len, Report_Current_Data_V2);
            memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);

            while (1) {//增加回应判断
                if (sys_sta.net_ack == STA_ON
                        || sys_sta.net_times > SEND_TIMES) { //重发次数
                    break;
                }

                if (sys_sta.on_sec < (sys_sta.net_sendtime + SEND_TIME_OUT / 1000)) { //延时等待
                    OSTimeDlyHMSM(0, 0, 0, 200);
                    continue;
                }


                sys_sta.net_times++;
                sys_sta.net_sendtime = sys_sta.on_sec;
                SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
            }

            OSSemPost(data_send_sem);
        }

        heart_time++;

        if (heart_time >= 1 * HEART_TIME) {
            heart_time = 0;

            OSTimeDlyHMSM(0, 0, 1, 0);
            OSSemPend(data_send_sem, 0, &os_err);
            Packet_Buffer.Result = Success_Result;
            Packet_Buffer.Send_Seq++;
			send_buf[0] = ctrl_inf.var.mode;
			send_buf[1] = HEART_TIME;
            Report_Data_Buffer(send_buf, 2, HEART_BEAT_DATA);
            memcpy(localTxBuf, Data_Send_Buf, Data_Send_Num);
            SendUartData( USARTCOMM, localTxBuf, Data_Send_Num );
            OSSemPost(data_send_sem);
        }

		

        OSTimeDlyHMSM(0, 0, 1, 0);
    }

}

