
/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/
#include "bsp.h"
#include "userconfig.h"
#include "menuinc.h"
#include "def.h"
#include "Parameters.h"
#include "recode.h"
#include <string.h>
#include "protocol.h"
#include <stdlib.h>
#include <math.h>
#include "energy.h"
#include "app_daytemp_cal.h"



unsigned short task_time[MAX_TASK_NUM] = {0};
unsigned short task_time_max[MAX_TASK_NUM] = {
    30, 30, 30, 30, 250, 60
};

void Task_Time_Chk(void)
{
    unsigned char	i = 0;

    if (sys_sta.auto_mode == STA_OFF) {
        return;
    }

    for (i = 0; i < MAX_TASK_NUM; i++) {
        task_time[i]++;

        if (task_time[i] > task_time_max[i]) {
            index_inf.restart_time++;
            Config_Save(SINDEX_ID);//记录保存
            SystemReset();
        }
    }
}

/*****************************************************************************/
void SystemReset(void)
{
    __disable_irq();
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

void Flash_Copy_Sdram(u32 flash_read_addr, u32 sdram_write_addr, u32 size )
{
    u32 i_num = 0;

    while ((i_num + 4096) < size) {
        SPI_Flash_Read((uint8_t *) (sdram_write_addr), flash_read_addr, 4096);
        i_num += 4096;
        flash_read_addr += 4096;
        sdram_write_addr += 4096;
    }

    SPI_Flash_Read((uint8_t *) (sdram_write_addr), flash_read_addr, (size - i_num));

}
void Show_Logo(void)
{
    Flash_Copy_Sdram(WLOGO_ID * PIC_SIZE, (SDRAM_PIC_ADDR + 2 * PIC_SIZE), PICLOGO_SIZE);
    DMA2D_M2M_Config((u32)(SDRAM_PIC_ADDR + 2 * PIC_SIZE), 150, 180, 500, 115);
    GUI_SetColor(GUI_GRAY);
    GUI_SetFont(GUI_FONT_24B_ASCII);
    GUI_SetTextMode(GUI_TM_TRANS);
    sprintf(show_buf, "HW Ver:r%02d SW Ver:V%.2fr%02d", get_hardware_ver(), (float)SOFTWARE_VER / 100.0, SYS_VER2);
    GUI_DispStringHCenterAt(show_buf, 380, 450);
}

void PIC_Init(void)
{
    IWDG_ReloadCounter();
#if FONT_SDRAM == 1
    Flash_Copy_Sdram(gbk_16_addr, SDRAM_GBK16_ADDR, FONT16_SIZE);
    Flash_Copy_Sdram(gbk_24_addr, SDRAM_GBK24_ADDR, FONT24_SIZE);
    Flash_Copy_Sdram(gbk_32_addr, SDRAM_GBK32_ADDR, FONT32_SIZE);
#endif
    Flash_Copy_Sdram(WPIC_ID * PIC_SIZE, SDRAM_ICON_ADDR, ICON_SIZE);
    Flash_Copy_Sdram((init_addr + PICLOGO_SIZE), SDRAM_LANG_ADDR, MULTI_LANG_SIZE);

    Flash_Copy_Sdram(WHOME_ID * PIC_SIZE, SDRAM_PIC_ADDR + (WHOME_ID - 1) * PIC_SIZE, PIC_SIZE);

    IWDG_ReloadCounter();

    sys_sta.copy_home_pic = STA_ON;

    Flash_Copy_Sdram(WMAIN_ID * PIC_SIZE, SDRAM_PIC_ADDR + (WMAIN_ID - 1) * PIC_SIZE, PIC_SIZE);

    Flash_Copy_Sdram(WBG_ID * PIC_SIZE, SDRAM_PIC_ADDR + (WBG_ID - 1) * PIC_SIZE, PIC_SIZE);
    IWDG_ReloadCounter();

    Flash_Copy_Sdram(BACK_ID * PIC_SIZE, SDRAM_PIC_ADDR + (BACK_ID - 2) * PIC_SIZE, PIC_SIZE);

    sys_sta.copy_bg_pic = STA_ON;
}

extern const unsigned char gImage_logo[];
void BSP_Init(void)
{
    LED_Init();
    DIN_Init();
    VerRead_Init();//放前面
    JDQ_Init();
    EEPROM_Config();
    PCF8563_Init();
    PWM_Init();
    AD7327_Init();
    SPI_Flash_Init();//FLASH
    LCD_ConfigLTDC();
    GUI_Init();
    GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    Show_Logo();
    SPI_TouchConfig();//触摸屏
    InitUartEvent();
    USART_Config();


    USBH_Init(&USB_OTG_Core, USB_OTG_HS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_cb);
    f_chdrive(USB);

}

void turn_off_terminal(void)
{
    if (strcmp(basic_inf.supadmin_pwd, PWD_NO_KEY) != 0) {
        sys_sta.login_type = LOGIN_GUEST;
    } else {
        sys_sta.login_type = LOGIN_SUPADMIN;
    }

    WM_HideWindow(HBasic_Main);
    WM_ShowWindow(MMain_SS);


    sys_sta.food_adjut_state = STA_OFF;//恢复采集


    if (Current_HWIN == MSys_JDQTS
            || Current_HWIN == MSys_PWMTS
            || Current_HWIN == MSys_HardTS) {
        return;
    }

    if (Current_HWIN != HBasic_Main) { //亮屏后显示主界面
        if (HMain != 0) {
            GUI_EndDialog(HMain, 0);
            HMain = 0;
        } else {
            GUI_EndDialog(Current_HWIN, 0);
        }

        Current_HWIN = HBasic_Main;
    }

}
void turn_on_terminal(void)
{
    WM_ShowWindow(HBasic_Main);
    WM_HideWindow(MMain_SS);
}

int cal_ex_minair(void)
{
    int res = 0;
    U8 i = 0, min_lv = 0xFF;
    int value;

    if (ctrl_inf.exminair.enable != STA_ON) {
        res = -1;
        return res;
    }

    if (dev_inf.sensor_adj.TEMP_EN[TEMP_OUT_SID] == STA_OFF || sta_in.sensor_real.TEMP[TEMP_OUT_SID] == TEMP_ER_VALUE) {
        res = -2;
        return res;
    }

    value = sta_in.sensor_adj.TEMP[TEMP_OUT_SID];

//查找小值
    for (i = 0; i < EXMINAIR_NUM; i++) {
        if (ctrl_inf.exminair.x[i].enable != STA_ON) {
            continue;
        }

        if (value >= ctrl_inf.exminair.x[i].out_temp) {
            min_lv = i;
        } else {
            break;
        }
    }

    if (min_lv == 0xFF) { //没找到小的
        res = -3;
        return res;
    }

    res = min_lv;
    return res;
}



int Cal_LV_Assist(u8 type)
{
    int res = 0;
    U8 i = 0, min_lv = 0xFF;
    int now_time, value;
    time_t sys_time;

    if (type > AS_TYPE_OUT)
        return -1;

    if (ctrl_inf.lv_scope[type].enable != STA_ON) {
        res = -2;
        return res;
    }

    if ((dev_inf.sensor_adj.TEMP_EN[TEMP_OUT_SID] == STA_OFF || sta_in.sensor_real.TEMP[TEMP_OUT_SID] == TEMP_ER_VALUE)
            && type == AS_TYPE_OUT) {
        res = -3;
        return res;
    }

    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟

    if (type == AS_TYPE_TIME) {
        value = now_time;
    } else if (type == AS_TYPE_DAY) {
        value = basic_inf.grow_day;
    } else {
        value = sta_in.sensor_real.TEMP[TEMP_OUT_SID];
    }

//查找小值
    for (i = 0; i < LV_SCOPE_NUM; i++) {
        if (ctrl_inf.lv_scope[type].curve[i].enable != STA_ON) {
            continue;
        }

        if (value >= ctrl_inf.lv_scope[type].curve[i].day_time) {
            min_lv = i;
        } else {
            break;
        }
    }

    if (min_lv == 0xFF) { //没找到小的
        res = -4;
        return res;
    }

    res = min_lv;
    return res;

}

void LV_Assist_Proc(void)
{
    int min_lv = 0, max_lv = CTRL_LV_NUM - 1, res;
    int real_min_lv = 0, real_max_lv = CTRL_LV_NUM - 1;
    u8 i = 0;

    for (i = 0; i < LV_SCOPE_KIND; i++) {
        res = Cal_LV_Assist(i);

        if (res < 0)
            continue;

        max_lv = ctrl_inf.lv_scope[i].curve[res].max_lv;
        min_lv = ctrl_inf.lv_scope[i].curve[res].min_lv;

        if (real_min_lv < min_lv) {
            real_min_lv = min_lv;
        }

        if (real_max_lv > max_lv) {
            real_max_lv = max_lv;
        }
    }

    cal_inf.as_min_lv = real_min_lv;
    cal_inf.as_max_lv = real_max_lv;
}



//计算级别的通风量
//仅仅是变频风机
//实际值的10倍
unsigned int Cal_LV_PWMAir(unsigned char lv_id)
{
    unsigned char i = 0;
    unsigned int air_value[PWM_NUM_MAX] = {0};
    float on_degree = 0;
    unsigned int total_air = 0;
    unsigned char ret = 0 ;

    for (i = 0 ; i < PWM_NUM_MAX; i++) {
        if (dev_inf.PWM[i].enable == true) {
            //停止
            if (ctrl_inf.lv[lv_id].PWM[i].mode == MODE_OFF) {
                air_value[i] = 0 ;
            }
            //持续运行
            else if (ctrl_inf.lv[lv_id].PWM[i].mode == MODE_ON) {
                air_value[i] = ctrl_inf.lv[lv_id].PWM[i].degree * dev_inf.PWM[i].max_air * dev_inf.PWM[i].fan_num * AIR_RATE;
                air_value[i] = air_value[i] / ( 100.0f * FAN_RATE);
            }
            //周期运行
            else if (ctrl_inf.lv[lv_id].PWM[i].mode == MODE_ON_OFF) {
                on_degree = (float)( (ctrl_inf.lv[lv_id].PWM[0].on_time) * 1.0f / (ctrl_inf.lv[lv_id].PWM[0].on_time + ctrl_inf.lv[lv_id].PWM[0].off_time));
                air_value[i] = ctrl_inf.lv[lv_id].PWM[i].degree  * dev_inf.PWM[i].max_air * dev_inf.PWM[i].fan_num * on_degree * AIR_RATE;
                air_value[i] = air_value[i] / ( 100.0f * FAN_RATE);
            }
            //持续轮转
            else if ( (ctrl_inf.lv[lv_id].PWM[i].mode == MODE_ROTATE) && (ret == 0)) {
                ret = 1 ;
                on_degree = (float)( (ctrl_inf.lv[lv_id].PWM[0].on_time) * 1.0f / (ctrl_inf.lv[lv_id].PWM[0].on_time + ctrl_inf.lv[lv_id].PWM[0].off_time));
                air_value[i] = ctrl_inf.lv[lv_id].PWM[i].degree * dev_inf.PWM[i].max_air * dev_inf.PWM[i].fan_num * on_degree * AIR_RATE;
                air_value[i] = air_value[i] / ( 100.0f * FAN_RATE);
            } else {
                air_value[i] = 0 ;

            }
        } else {
            air_value[i] = 0 ;
        }

        total_air += air_value[i];

    }

    return total_air;
}
//计算级别的通风量
//仅仅是定速风机
//实际值的10倍
unsigned int Cal_LV_JDQAir(unsigned char lv_id)
{
    unsigned char i = 0;
    unsigned int air_value[JDQ_NUM_MAX] = {0};
    float on_degree = 0;
    unsigned int total_air = 0;
    unsigned char ret = 0 ;

    for (i = 0 ; i < JDQ_NUM_MAX; i++) {
        if (dev_inf.JDQ[i].enable == true) {
            //停止
            if (ctrl_inf.lv[lv_id].JDQ[i].mode == MODE_OFF) {
                air_value[i] = 0 ;
            }
            //持续运行
            else if (ctrl_inf.lv[lv_id].JDQ[i].mode == MODE_ON) {
                air_value[i] = dev_inf.JDQ[i].max_air * dev_inf.JDQ[i].fan_num * AIR_RATE;
                air_value[i] = air_value[i] / ( 1.0f * FAN_RATE);
            }
            //周期运行
            else if (ctrl_inf.lv[lv_id].JDQ[i].mode == MODE_ON_OFF) {
                on_degree = (float)( (ctrl_inf.lv[lv_id].PWM[0].on_time) * 1.0f / (ctrl_inf.lv[lv_id].PWM[0].on_time + ctrl_inf.lv[lv_id].PWM[0].off_time));
                air_value[i] =  dev_inf.JDQ[i].max_air * dev_inf.JDQ[i].fan_num * AIR_RATE * on_degree;
                air_value[i] = air_value[i] / ( 1.0f * FAN_RATE);
            }
            //持续轮转
            else if ( (ctrl_inf.lv[lv_id].JDQ[i].mode == MODE_ROTATE) && (ret == 0)) {
                ret = 1 ;
                on_degree = (float)( (ctrl_inf.lv[lv_id].PWM[0].on_time) * 1.0f / (ctrl_inf.lv[lv_id].PWM[0].on_time + ctrl_inf.lv[lv_id].PWM[0].off_time));
                air_value[i] = dev_inf.JDQ[i].max_air * dev_inf.JDQ[i].fan_num * AIR_RATE * on_degree;
                air_value[i] = air_value[i] / ( 1.0f * FAN_RATE);
            } else {
                air_value[i] = 0 ;

            }
        } else {
            air_value[i] = 0 ;
        }

        total_air += air_value[i];

    }

    return total_air;
}


unsigned char Find_Max_LV(void)
{
    unsigned char i = 0;
    unsigned char lv_id = CTRL_LV_NUM - 1;

    for (i = 0 ; i < CTRL_LV_NUM; i++) {
        if (ctrl_inf.lv[i].enable != true) {
            if (i == 0)
                lv_id = i;
            else
                lv_id = i - 1;

            break;
        }
    }

    return lv_id;
}
void Cal_Min_Air_LV(void)
{
    unsigned char i = 0;

    for (i = 0 ; i < CTRL_LV_NUM; i++) {
        //连续的 level
        if ((ctrl_inf.lv[i].enable == true)) {
            cal_inf.lv_air[i] = Cal_LV_PWMAir(i) + Cal_LV_JDQAir(i);
//            if (cal_inf.lv_air[i] == 0 && i != 0) {
//                ctrl_inf.lv[i].enable = STA_OFF;
//            }				
        } else {
            cal_inf.lv_air[i] = 0;
        }
    }
}


unsigned char Get_Min_Air_LV(void)
{
    unsigned char i = 0;
//    signed short temp_dep = 0;
    unsigned char lv_id = 0;

//一直计算
    Cal_Min_Air_LV();

    for (i = 0 ; i < CTRL_LV_NUM; i++) {
        //连续的 level 去掉温度偏差0的限制
        if ((ctrl_inf.lv[i].enable == true)) {

            if ((cal_inf.min_air <= cal_inf.lv_air[i])) {
                lv_id = i;
                break;
            }
        } else {
            if (i == 0)
                lv_id = i;
            else
                lv_id = i - 1;

            break;
        }
    }

    return lv_id;
}
int Find_LV_InLimits(int type)
{
    u8 i = 0;

    if (type > 0) {
        for (i = cal_inf.as_min_lv ; i <= cal_inf.as_max_lv; i++) {
            if (ctrl_inf.lv[i].enable == STA_ON)
                return i;
        }

    } else {
        for (i = cal_inf.as_max_lv ; i >= cal_inf.as_min_lv; i--) {
            if (ctrl_inf.lv[i].enable == STA_ON)
                return i;
        }
    }

    return -1;
}

//查找当前级别增加级别,没有找到返回最大级别
unsigned char Find_LV_Next(u8 old_lv, u8 add)
{
    u8 inc = 0, lv = 0;

    if (add == 0 || add >= CTRL_LV_NUM || old_lv >= CTRL_LV_NUM) {
        return old_lv;
    }

    for (lv = (old_lv + 1); lv < CTRL_LV_NUM; lv++) {
        if (ctrl_inf.lv[lv].enable == STA_ON) {
            inc++;
        }

        if (inc == add) {
            return lv;
        }
    }

    return cal_inf.max_lv;
}
unsigned char Get_Max_0LV(void)
{
    unsigned char i = 0, lv_h = 0;
    lv_h = cal_inf.max_lv;

    for (i = 0 ; i < CTRL_LV_NUM; i++) {
        //连续的 level
        if ((ctrl_inf.lv[i].enable == true) &&
                (ctrl_inf.lv[i].offset > 0)) {
            if (i == 0) {
                lv_h = 0;
            } else {
                lv_h = i - 1;
            }

            break;
        }
    }

    return lv_h;
}
unsigned char Get_Current_LV(void)
{
    unsigned char i = 0, lv_id = 0;
    signed short temp_dep = 0;
    float scale = 0;

    //小于目标温度的时候处于最小通风模式
    if (cal_inf.temp <= cal_inf.target_temp) {
        lv_id = cal_inf.lv_l;
        return lv_id;
    }

    //大于目标温度小于纵向通风的时候在最小通风模式和最大偏差0线性选择
    if ((cal_inf.temp > cal_inf.target_temp) && (cal_inf.temp <= cal_inf.fan_temp)) {
        if (cal_inf.lv_h <= cal_inf.lv_l) {
            lv_id = cal_inf.lv_l;
        } else {
            scale = (cal_inf.fan_temp - cal_inf.target_temp) * 1.0f / (cal_inf.lv_h - cal_inf.lv_l);
            lv_id = cal_inf.lv_l + (cal_inf.temp - cal_inf.target_temp) / scale;

            if (lv_id > cal_inf.lv_h) {
                lv_id = cal_inf.lv_h;
            }
        }

        return lv_id;
    }

    //大于纵向通风的时候根据温度偏差
    if (cal_inf.temp > cal_inf.fan_temp) {
        temp_dep = cal_inf.temp - cal_inf.fan_temp;

        lv_id = cal_inf.lv_h;

        for (i = cal_inf.max_lv ; i > cal_inf.lv_h; i--) {
            if ((ctrl_inf.lv[i].enable == true) &&
                    (ctrl_inf.lv[i].offset > 0) &&//这个不加偏差修正了，不会影响状态
                    ((ctrl_inf.lv[i].offset + cal_inf.lv_ex_offset + ctrl_inf.lv_more.lv_temp_adj) <= temp_dep)) {
                lv_id = i;
                break;
            }
        }

        if (lv_id < cal_inf.lv_l) {
            lv_id = cal_inf.lv_l;
        }
    }

    return lv_id;
}
//根据室外温度计算级别偏差
int cal_lv_ex_offset(void)
{
    int res = 0;

    if (cal_inf.exminair_lv < 0 || cal_inf.exminair_lv >= EXMINAIR_NUM ) {
        res = 0;
        return res;
    }

// 没有偏差不是0的级别
    if (cal_inf.max_lv <= cal_inf.lv_h) {
        res = 0;
        return res;
    }

    //判断第一个不为0的偏差的，最小只能修正成-1
    if (ctrl_inf.lv[cal_inf.lv_h + 1].offset + ctrl_inf.lv_more.lv_temp_adj + ctrl_inf.exminair.x[cal_inf.exminair_lv].lv_temp <= 0) {
        res = -(ctrl_inf.lv[cal_inf.lv_h + 1].offset + ctrl_inf.lv_more.lv_temp_adj) + 1;//最大修正偏差为0.1
    } else {
        res = ctrl_inf.exminair.x[cal_inf.exminair_lv].lv_temp;
    }

    return res;
}

//>0直接运行最小通风
int min_wind_sta(void)
{
    int i = 0;

    if (ctrl_inf.extemp.minwind_temp_sta == MIN_WIND_AVG && cal_inf.temp <= cal_inf.heat_temp) {
        return 1;
    }

    if (ctrl_inf.extemp.minwind_temp_sta == MIN_WIND_AREA) {
        for (i = 0; i < HEAT_NUM_MAX; i++) {
            if (cal_inf.tempArea[i] <= cal_inf.heat_temp) {
                return 1;
            }
        }
    }

    return -1;
}

int check_lv_temp_adj(void)
{
    int res = 0;

// 没有偏差不是0的级别
    if (cal_inf.max_lv <= cal_inf.lv_h) {
        res = 0;
        return res;
    }

    //判断第一个不为0的偏差的，最小只能修正成-1
    if (ctrl_inf.lv[cal_inf.lv_h + 1].offset + ctrl_inf.lv_more.lv_temp_adj <= 0) {
        res = -ctrl_inf.lv[cal_inf.lv_h + 1].offset + 1;//最大修正偏差为0.1
    } else {
        res = ctrl_inf.lv_more.lv_temp_adj;
    }

    return res;
}

void check_lv_re_diff(void)
{
    int i = 0;

    if (ctrl_inf.lv_more.lv_re_diff_en != STA_ON) {
        return ;
    }

    for (i = 0; i < CTRL_LV_NUM; i++) {
        ctrl_inf.lv[i].re_diff = ctrl_inf.lv_more.lv_re_diff;
    }
}

void check_lv_fan_cycle(void)
{
    int i = 0;

    if (ctrl_inf.lv_more.lv_fan_cycle_en != STA_ON) {
        return ;
    }

    for (i = 0; i < CTRL_LV_NUM; i++) {
        if (ctrl_inf.lv[i].PWM[0].on_time > ctrl_inf.lv_more.lv_fan_cycle) {
            ctrl_inf.lv[i].PWM[0].on_time = ctrl_inf.lv_more.lv_fan_cycle;
            ctrl_inf.lv[i].PWM[0].off_time = 0;
        } else {
            ctrl_inf.lv[i].PWM[0].off_time = ctrl_inf.lv_more.lv_fan_cycle - ctrl_inf.lv[i].PWM[0].on_time;
        }
    }
}

//级别计算函数
unsigned char LV_Proc(void)
{
    unsigned char i = 0;
    signed short temp_dep = 0;
    unsigned char lv_id = 0;
    int lv_temp = 0;
    static unsigned char last_lv = 0;
    static signed short last_temp_diff = 0;

    cal_inf.max_lv = Find_Max_LV();//计算最大级别
    cal_inf.lv_l = Get_Min_Air_LV();//最小通风的最低级别
    cal_inf.lv_h = Get_Max_0LV();//最大的偏差0级别
    check_lv_re_diff();
    check_lv_fan_cycle();
    ctrl_inf.lv_more.lv_temp_adj = check_lv_temp_adj();
    cal_inf.lv_ex_offset = cal_lv_ex_offset();
    lv_id = Get_Current_LV();//计算当前实际级别

//降级别时要比回差还小的时候才降
    if (lv_id < last_lv && last_temp_diff > 0
            && cal_inf.temp >= last_temp_diff
            && last_lv <= cal_inf.max_lv) {//必须是小于当前最大级别
        lv_id = last_lv;
    }


//低于加热直接运行最小通风不用延时
    if (min_wind_sta() > 0) {
        lv_id = cal_inf.lv_l;

        if (last_lv != cal_inf.lv_l) {
            run_inf.LV_Time = 0;
        }
    }

//判断级别保持时间
    if (lv_id != last_lv) {
        if (run_inf.LV_Time != 0) {
            lv_id = last_lv;
        } else {
            run_inf.LV_Time = ctrl_inf.var.lv_delay * RUN_FREQ;
        }
    }


//更新上一次的值
    last_lv = lv_id;

    if (ctrl_inf.lv[lv_id].offset > 0) {//这个不加偏差修正了，不会影响状态
        last_temp_diff = cal_inf.fan_temp + ctrl_inf.lv[lv_id].offset + cal_inf.lv_ex_offset + ctrl_inf.lv_more.lv_temp_adj - ctrl_inf.lv[lv_id].re_diff;
    } else {
        temp_dep = (cal_inf.fan_temp - cal_inf.target_temp) * 1.0f / (cal_inf.lv_h - cal_inf.lv_l) * (lv_id - cal_inf.lv_l);
        last_temp_diff = cal_inf.target_temp + temp_dep - ctrl_inf.lv[lv_id].re_diff;
    }

//级别辅助调整
    for (i = 0; i < SENCON_NUM; i++) {
        lv_id = Find_LV_Next(lv_id, run_inf.sencon_Lv[i]);
    }


//限制级别范围
    if (lv_id < cal_inf.as_min_lv) {
        lv_temp = Find_LV_InLimits(1);

        if (lv_temp >= 0) {
            lv_id = lv_temp;
        }
    } else if (lv_id > cal_inf.as_max_lv) {
        lv_temp = Find_LV_InLimits(-1);

        if (lv_temp >= 0) {
            lv_id = lv_temp;
        }
    }

    if (lv_id > cal_inf.max_lv) {
        lv_id = cal_inf.max_lv;
    }

    return lv_id;
}

//<0 未找到级别 直接关闭 (HEAT_NUM+1) 不处理
int HEAT_Proc(unsigned char id)
{
    unsigned char i = 0;
    signed short temp_dep = 0;
    int lv = 0;
    int now_time;
    time_t sys_time;

    if (basic_inf.heat_en[id] == false) {
        lv = -1 ;
        return lv;
    }

    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟
    temp_dep = cal_inf.tempArea[id] - cal_inf.heat_temp;

    for (i = 0 ; i < HEAT_NUM; i++) {
        if ((basic_inf.heat[id][i].enable == STA_OFF) ||
                (now_time < basic_inf.heat[id][i].start_time) ||
                (now_time > basic_inf.heat[id][i].stop_time))
            continue;

        if (temp_dep <= basic_inf.heat[id][i].start_offtemp) {
            lv = i;
            return lv;
        } else if (temp_dep >= basic_inf.heat[id][i].stop_offtemp) {
            //	不退出，查找下一个
        } else {
            if (cal_inf.HEAT_lv[id] < 0) { //之前是关闭的
                lv = -2;
            } else {
                lv = i;
            }

            return lv;
        }

    }

    lv = -3 ;
    return lv;
}


//<0 未找到级别 直接关闭 (HEAT_NUM+1) 不处理
int PHEAT_Proc()
{
    unsigned char i = 0;
    signed short temp_dep = 0;
    int lv = 0;
    int now_time;
    time_t sys_time;

    if (basic_inf.pheat_en == false) {
        lv = -1 ;
        return lv;
    }

    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟
    temp_dep = cal_inf.temp - cal_inf.heat_temp;

    for (i = 0 ; i < HEAT_NUM; i++) {
        if ((basic_inf.pheat[i].enable == STA_OFF) ||
                (now_time < basic_inf.pheat[i].start_time) ||
                (now_time > basic_inf.pheat[i].stop_time))
            continue;

        if (temp_dep <= basic_inf.pheat[i].start_offtemp) {
            lv = i;
            return lv;
        } else if (temp_dep >= basic_inf.pheat[i].stop_offtemp) {
            //	不退出，查找下一个
        } else {
            if (cal_inf.PHEAT_lv < 0) { //之前是关闭的
                lv = -2;
            } else {
                lv = i;
            }

            return lv;
        }
    }

    lv = -3 ;
    return lv;
}


//<0 未找到级别 直接关闭 (WET_NUM+1) 不处理
int WET_Proc(unsigned char id)
{
    int i = 0;
    signed short temp_dep = 0;
    int lv = 0;
    int now_time;
    time_t sys_time;

    if (dev_inf.wet_en[id] == false) {
        lv = -1 ;
        return lv;
    }

    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟
    temp_dep = cal_inf.temp - cal_inf.fan_temp;

    for (i = WET_NUM - 1 ; i >= 0; i--) {
        if ((dev_inf.wet[id][i].enable == STA_OFF) ||
                (now_time < dev_inf.wet[id][i].start_time ) ||
                (now_time > dev_inf.wet[id][i].stop_time))
            continue;

        if (cal_inf.SEN[SENSOR_TYPE_HUM] <= dev_inf.wet[id][i].max_hum) {
            if (temp_dep >= dev_inf.wet[id][i].start_offtemp) {
                lv = i;
                return lv;
            } else if (temp_dep <= dev_inf.wet[id][i].stop_offtemp) {
                //	不退出，查找下一个
            } else {
                if (cal_inf.WET_lv[id] < 0) { //之前是关闭的
                    lv = -2;
                } else {
                    lv = i;
                }

                return lv;
            }
        }
    }

    lv = -3 ;
    return lv;
}

int Get_Heat_Sta(void)
{
    unsigned char i = 0;

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        if (cal_inf.HEAT_lv[i] >= 0) {    //启用
            return 1;
        }
    }

    return -1;
}

//<0 未找到级别 直接关闭 (TEMPC_NUM+1) 不处理 <=-5是没有启用或者温度故障
int TEMPC_Proc(void)
{
    unsigned char i = 0;
    int lv = 0;
    int now_time;
    time_t sys_time;

    if (basic_inf.tempc_en.enable != STA_ON
            || basic_inf.tempc_en.temp_id > TEMPC_TEMP6) {
        lv = -5 ;
        return lv;
    }

    if (basic_inf.tempc_en.temp_id == TEMPC_TEMPAVG) {
        cal_inf.tempc_temp = cal_inf.temp;
    } else {
        if (dev_inf.sensor_adj.TEMP_EN[basic_inf.tempc_en.temp_id - 1] != STA_ON) {
            lv = -6 ;
            return lv;
        }

        cal_inf.tempc_temp = sta_in.sensor_adj.TEMP[basic_inf.tempc_en.temp_id - 1];
    }

    if (cal_inf.tempc_temp >= TEMP_ER_VALUE) {
        lv = -7 ;
        return lv;
    }

    if (basic_inf.tempc_en.mode == TEMPC_MODE_ECIDF && Get_Heat_Sta() < 0) {//节能模式下没有加热不需要开启
        lv = -1 ;
        return lv;
    }


    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟

    for (i = 0 ; i < TEMPC_NUM; i++) {
        if ((basic_inf.tempc[i].enable == STA_OFF) ||
                (now_time < basic_inf.tempc[i].start_time) ||
                (now_time > basic_inf.tempc[i].stop_time))
            continue;

        if (basic_inf.tempc_en.mode == TEMPC_MODE_HIGH) {
// 高温开启
            if (cal_inf.tempc_temp >= basic_inf.tempc[i].start_offtemp) {
                lv = i;
                return lv;
            } else if (cal_inf.tempc_temp <= basic_inf.tempc[i].stop_offtemp) {
                //	不退出，查找下一个
            } else {
                if (cal_inf.TEMPC_lv < 0) { //之前是关闭的
                    lv = -1;
                } else {
                    lv = i;
                }

                return lv;
            }

        } else {
// 低温开启或者节能引风机
            if (cal_inf.tempc_temp <= basic_inf.tempc[i].start_offtemp) {
                lv = i;
                return lv;
            } else if (cal_inf.tempc_temp >= basic_inf.tempc[i].stop_offtemp) {
                //	不退出，查找下一个
            } else {
                if (cal_inf.TEMPC_lv < 0) { //之前是关闭的
                    lv = -1;
                } else {
                    lv = i;
                }

                return lv;
            }

        }
    }

    lv = -1 ;
    return lv;
}

//<0 未找到级别 直接关闭 (TEMPC_NUM+1) 不处理 <=-5是没有启用或者温度故障
int TEMPC_Pwm_Proc(void)
{
    unsigned char i = 0;
    int lv = 0;
    int now_time;
    time_t sys_time;

    if (basic_inf.tempc_pwm_en.enable != STA_ON
            || basic_inf.tempc_pwm_en.temp_id > TEMPC_TEMP6) {
        lv = -5 ;
        return lv;
    }

    if (basic_inf.tempc_pwm_en.temp_id == TEMPC_TEMPAVG) {
        cal_inf.tempc_pwm_temp = cal_inf.temp;
    } else {
        if (dev_inf.sensor_adj.TEMP_EN[basic_inf.tempc_pwm_en.temp_id - 1] != STA_ON) {
            lv = -6 ;
            return lv;
        }

        cal_inf.tempc_pwm_temp = sta_in.sensor_adj.TEMP[basic_inf.tempc_pwm_en.temp_id - 1];
    }

    if (cal_inf.tempc_pwm_temp >= TEMP_ER_VALUE) {
        lv = -7 ;
        return lv;
    }

    if (basic_inf.tempc_pwm_en.mode == TEMPC_MODE_ECIDF && Get_Heat_Sta() < 0) {//节能模式下没有加热不需要开启
        lv = -1 ;
        return lv;
    }


    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟

    for (i = 0 ; i < TEMPC_NUM; i++) {
        if ((basic_inf.tempc_pwm[i].enable == STA_OFF) ||
                (now_time < basic_inf.tempc_pwm[i].start_time) ||
                (now_time > basic_inf.tempc_pwm[i].stop_time))
            continue;

        if (basic_inf.tempc_pwm_en.mode == TEMPC_MODE_HIGH) {
// 高温开启
            if (cal_inf.tempc_pwm_temp >= basic_inf.tempc_pwm[i].start_offtemp) {
                lv = i;
                return lv;
            } else if (cal_inf.tempc_pwm_temp <= basic_inf.tempc_pwm[i].stop_offtemp) {
                //	不退出，查找下一个
            } else {
                if (cal_inf.TEMPC_pwm_lv < 0) { //之前是关闭的
                    lv = -1;
                } else {
                    lv = i;
                }

                return lv;
            }

        } else {
// 低温开启或者节能引风机
            if (cal_inf.tempc_pwm_temp <= basic_inf.tempc_pwm[i].start_offtemp) {
                lv = i;
                return lv;
            } else if (cal_inf.tempc_pwm_temp >= basic_inf.tempc_pwm[i].stop_offtemp) {
                //	不退出，查找下一个
            } else {
                if (cal_inf.TEMPC_pwm_lv < 0) { //之前是关闭的
                    lv = -1;
                } else {
                    lv = i;
                }

                return lv;
            }

        }
    }

    lv = -1 ;
    return lv;
}

//<0 未找到级别 直接关闭 (SPRAY_NUM+1) 不处理
int SPRAY_Proc(void)
{
    int i = 0;
    signed short temp_dep = 0;
    int lv = 0;
    int now_time;
    time_t sys_time;

    if (dev_inf.spray_en == SPRAY_MODE_OFF) {
        lv = -1 ;
        return lv;
    }

    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟
    temp_dep =  cal_inf.temp - cal_inf.fan_temp;

    if (dev_inf.spray_en == SPRAY_MODE_HUM) { //加湿
        for (i = 0 ; i < SPRAY_NUM; i++) {
            if ((dev_inf.spray[i].enable == STA_OFF) ||
                    (now_time < dev_inf.spray[i].start_time) ||
                    (now_time > dev_inf.spray[i].stop_time))
                continue;

            if (temp_dep >= dev_inf.spray[i].min_offtemp) {
                if ((cal_inf.SEN[SENSOR_TYPE_HUM] <= dev_inf.spray[i].start_hum)) {
                    lv = i;
                    return lv;
                } else if ((cal_inf.SEN[SENSOR_TYPE_HUM] >= dev_inf.spray[i].stop_hum)) {
                    //	不退出，查找下一个
                } else {
                    if (cal_inf.SPRAY_lv < 0) { //之前是关闭的
                        lv = -2;
                    } else {
                        lv = i;
                    }

                    return lv;
                }
            }
        }
    } else {// 降温
        for (i = SPRAY_NUM - 1 ; i >= 0; i--) {
            if ((dev_inf.spray[i].enable == STA_OFF) ||
                    (now_time < dev_inf.spray[i].start_time) ||
                    (now_time > dev_inf.spray[i].stop_time))
                continue;

            if (cal_inf.SEN[SENSOR_TYPE_HUM] <= dev_inf.spray[i].min_offtemp) {
                if (temp_dep >= dev_inf.spray[i].start_hum) {
                    lv = i;
                    return lv;
                } else if (temp_dep <= dev_inf.spray[i].stop_hum) {
                    //	不退出，查找下一个
                } else {
                    if (cal_inf.SPRAY_lv < 0) { //之前是关闭的
                        lv = -2;
                    } else {
                        lv = i;
                    }

                    return lv;
                }
            }
        }
    }

    lv = -3 ;
    return lv;

}



//计算目标值
int Update_Aim_Light(void)
{
    int res = 0;
    U8 i = 0, min_lv = 0, max_lv;
    int now_time;
    time_t sys_time;

    if (dev_inf.light.enable != STA_ON) {
        res = -1;
        return res;
    }

//查找大小值
    for (max_lv = 0, min_lv = 0, i = 0; i < LIGHT_NUM; i++) {
        if (dev_inf.light.curve[i].enable != STA_ON) {
            continue;
        }

        if (basic_inf.grow_day >= dev_inf.light.curve[i].grow_day) {
            min_lv = i;
        } else {
            max_lv = i;
            break;
        }
    }

    if (min_lv == 0 && dev_inf.light.curve[0].enable != STA_ON) { //没找到小的
        if (max_lv == 0) {
            res = -2;
            return res;
        }

        min_lv = max_lv;
    }

    if (max_lv == 0) { //没有找到大的
        max_lv = min_lv;
    }

    for (i = 0; i <= min_lv; i++) {
        if (dev_inf.light.curve[i].enable != STA_ON) {
            continue;
        }

        if (dev_inf.light.curve[i].grow_day != dev_inf.light.curve[min_lv].grow_day) {
            continue;
        }

        sys_time = GetSysTime();
        now_time = sys_time % DAY_TO_SEC;
        now_time = now_time / 60; //转换成分钟

        if (dev_inf.light.curve[i].start_time < dev_inf.light.curve[i].stop_time &&
                now_time >= dev_inf.light.curve[i].start_time &&
                now_time < dev_inf.light.curve[i].stop_time) {
            res = dev_inf.light.curve[i].value;
            return res;
        }

        if (dev_inf.light.curve[i].start_time > dev_inf.light.curve[i].stop_time &&
                (now_time >= dev_inf.light.curve[i].start_time ||
                 now_time < dev_inf.light.curve[i].stop_time)) {
            res = dev_inf.light.curve[i].value;
            return res;
        }
    }

    res = -4;
    return res;
}


//ntc表最大最小值
#define TEMP_RES_MIN		(-40)
#define TEMP_RES_MAX		(150)
#define TEMP_RES_NUM		(191)

const unsigned int tempresist[TEMP_RES_NUM] = {
    301239, 282261, 264624, 248226, 232969, 218766, 205538, 193210, 181714, 170990,
    160978, 151667, 142964, 134826, 127211, 120082, 113405, 107149, 101284, 95782,
    90619, 85677, 81041, 76688, 72600, 68760, 65150, 61755, 58561, 55555,
    52724, 50058, 47546, 45177, 42943, 40834, 38838, 36952, 35172, 33489,
    31898, 30377, 28939, 27579, 26292, 25074, 23886, 22762, 21699, 20692,
    19739, 18835, 17978, 17166, 16396, 15665, 14960, 14290, 13655, 13052,
    12480, 11932, 11411, 10917, 10447, 10000, 9576, 9173, 8789, 8423,
    8075, 7737, 7415, 7109, 6816, 6538, 6275, 6024, 5785, 5556,
    5338, 5127, 4925, 4733, 4549, 4373, 4203, 4041, 3886, 3737,
    3596, 3461, 3332, 3208, 3090, 2977, 2868, 2764, 2664, 2568,
    2477, 2390, 2308, 2228, 2151, 2078, 2007, 1938, 1872, 1809,
    1748, 1690, 1635, 1582, 1531, 1482, 1432, 1384, 1339, 1295,
    1252, 1212, 1173, 1136, 1100, 1066, 1033, 1001, 970, 940,
    912, 884, 857, 831, 806, 781, 758, 737, 715, 695,
    675, 655, 636, 618, 601, 583, 567, 550, 534, 519,
    504, 491, 477, 465, 452, 440, 429, 417, 406, 396,
    386, 375, 365, 355, 346, 336, 328, 319, 311, 302,
    295, 288, 281, 274, 268, 261, 255, 249, 244, 238,
    233, 227, 222, 217, 212, 207, 202, 197, 193, 188,
    184
};//b值3950 25度 10k 1度一个坐标点


//AD值转换为电阻值
int NTCresist(u16 ad_value)
{
    int temp_register;
    float adv_data = 0.0;
    adv_data = (ad_value * 5) / 4096.0;

    if (adv_data >= 4.8) {
        return  (-1);
    }

    temp_register = 8.2 * adv_data / (5 - adv_data) * 1000; //单位是R
    return  temp_register;
}

//*************************************
// 函数名称：FineTab  二分查找算法 ->查温度表
// 函数功能：查找数据在表中对应的位置 表中数据从大到小
// 入口参数：表地址、表长度、要查找的数据
// 出口参数：无
// 返 回 值：数据在表中的位置 后一个
//*************************************
int FineTab(unsigned int data)   //表中数据从大到小
{
    u16 st, ed, m ;
    u16 i ;

    st = 0 ;
    ed = TEMP_RES_NUM - 1 ;
    i = 0  ;

    if (data >= tempresist[st]) return st ;
    else if (data <= tempresist[ed]) return (ed + 1) ;

    while (st < ed) {
        m = (st + ed) / 2 ;

        if (data == tempresist[m] ) break ;

        if (data < tempresist[m] && data > tempresist[m + 1]) break ;


        if (data > tempresist[m])  ed = m ;
        else st = m ;

        if (i++ > TEMP_RES_NUM) break ;
    }

    if (st > ed ) return 0 ;

    return (m + 1) ;
}

int AD_TO_Temp(u16 ad_value)
{
    int NTCvalue, Degree_C, Degree_0C;
    int temp_ntc = 0;
    NTCvalue = NTCresist(ad_value);

    if (NTCvalue < 0) {
        return TEMP_ER_VALUE;
    }

    Degree_C = FineTab(NTCvalue);

    if (Degree_C >= TEMP_RES_NUM) {
        return (TEMP_RES_MAX * 10);
    }

    if (Degree_C == 0) {
        return (TEMP_RES_MIN * 10);
    }

    Degree_0C = ((tempresist[Degree_C - 1] - NTCvalue) * 10) / (tempresist[Degree_C - 1] - tempresist[Degree_C]); //差分成小數
    temp_ntc = (TEMP_RES_MIN + Degree_C - 1 ) * 10 + Degree_0C;
    return temp_ntc;
}

const s16 AD4ma_value[SENSOR_TYPE_NUM] = {HUM_4MA, TEMP_4MA, HUM_4MA, CO2_4MA, NH3_4MA, H2S_4MA, VAC_4MA, SPEED_4MA, HUM_4MA};
const s16 AD20ma_value[SENSOR_TYPE_NUM] = {HUM_20MA, TEMP_20MA, HUM_20MA, CO2_20MA, NH3_20MA, H2S_20MA, VAC_20MA, SPEED_20MA, HUM_20MA};
float AD4_20Ma_TO(u8 type, u16 ad_value)
{
    float adv_data = 0.0, scale = 0.0, result = 0.0;
    s16 AD4ma = 0, AD20ma = 0;

    if (type > SENSOR_TYPE_NUM || type == SENSOR_TYPE_DIS)
        return 0;

    if (dev_inf.sensor_rg[type].enable == STA_ON) {
        AD4ma = dev_inf.sensor_rg[type].min_value;
        AD20ma = dev_inf.sensor_rg[type].max_value;
    } else {
        AD4ma = AD4ma_value[type];
        AD20ma = AD20ma_value[type];
    }

    adv_data = (ad_value * 10) / 8192.0;

    if (adv_data < 0.8 || adv_data > 5.2) { //没接传感器或者传感器出错
        if (type == SENSOR_TYPE_TEMP ) {
            return TEMP_ER_VALUE;
        } else if (type == SENSOR_TYPE_VAC) {
            return VAC_ER_VALUE;
        } else {
            return SENSOR_ER_VALUE;
        }
    } else if (adv_data <= 1) { //返回最小值
        return AD4ma;
    } else if (adv_data >= 5 ) { //返回最大值
        return AD20ma;
    }

    scale = (AD20ma - AD4ma) / 16.0;
    result = ((adv_data / 0.25 - 4) * scale + AD4ma);
    return result;
}

u16 AD0_10V_TO_PER(u16 ad_value)
{
    float adv_data = 0.0;
    u16 result = 0;
    adv_data = (ad_value * 10) / 8192.0;
    result = (u16)(adv_data * VOL_RATE);
    return result;
}

void Find_Temp_Num(void)
{
    u8 i = 0, total = 0;
    memset(cal_inf.VaiTemp_id, 0, sizeof(cal_inf.VaiTemp_id));

    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if (i == TEMP_OUT_SID ) { //室外不参与计算
            continue;
        }

        if (basic_inf.tempc_en.enable == STA_ON && (i + 1) == basic_inf.tempc_en.temp_id) {//单独温控不参与
            continue;
        }

        if (basic_inf.tempc_pwm_en.enable == STA_ON && (i + 1) == basic_inf.tempc_pwm_en.temp_id) {//单独温控不参与
            continue;
        }

        if (dev_inf.sensor_adj.TEMP_EN[i] == 0 ||//出错不累加
                sta_in.sensor_real.TEMP[i] > TEMP_MAX_ERR || sta_in.sensor_real.TEMP[i] < TEMP_MIN_ERR) {
            continue;
        }

        cal_inf.VaiTemp_id[i] = 1;
        total++;
    }

    cal_inf.VaiTemp_num = total;
}

int Get_Temp_State(void)
{
    int i = 0;

    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if (cal_inf.oldVaiTemp_id[i] == 1 && cal_inf.VaiTemp_id[i] == 0) {
            memcpy(cal_inf.oldVaiTemp_id, cal_inf.VaiTemp_id, sizeof(cal_inf.VaiTemp_id));
            return -1;
        }
    }

    memcpy(cal_inf.oldVaiTemp_id, cal_inf.VaiTemp_id, sizeof(cal_inf.VaiTemp_id));
    return 1;
}

int Get_Sen_State(void)
{
    int i = 0;

    for (i = 0; i < SENSOR_NUM_MAX; i++) {
        if (cal_inf.oldVaiSen_id[i] == 1 && cal_inf.VaiSen_id[i] == 0) {
            memcpy(cal_inf.oldVaiSen_id, cal_inf.VaiSen_id, sizeof(cal_inf.VaiSen_id));
            return -1;
        }
    }

    memcpy(cal_inf.oldVaiSen_id, cal_inf.VaiSen_id, sizeof(cal_inf.VaiSen_id));
    return 1;
}

void Find_Temp_Base(void)
{
    u8 i = 0, j = 0;
    int min, temp, avg;
    Find_Temp_Num();

    if (cal_inf.VaiTemp_num < 3) {
        cal_inf.BaseTemp = 0;
        return ;
    }

    min = TEMP_ER_VALUE;
    avg = 0;

    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if (cal_inf.VaiTemp_id[i] == 0 ) { //无效温度不计算
            continue;
        }

        for (j = i + 1; j < TEMP_NUM_MAX; j++) {
            if (cal_inf.VaiTemp_id[j] == 0 ) { //无效温度不计算
                continue;
            }

            temp = abs(sta_in.sensor_adj.TEMP[i] - sta_in.sensor_adj.TEMP[j]);

            if (min > temp) {
                avg = (sta_in.sensor_adj.TEMP[i] + sta_in.sensor_adj.TEMP[j]) / 2;
                min = temp;
            }
        }
    }

    if (min <= ctrl_inf.var.temp_diff) {
        cal_inf.BaseTemp = avg;
        return ;
    }

    //每个温度都不准，取和目标最近的作为基准
    min = TEMP_ER_VALUE;

    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if (cal_inf.VaiTemp_id[i] == 0 ) { //无效温度不计算
            continue;
        }

        temp = abs(sta_in.sensor_adj.TEMP[i] - cal_inf.target_temp);

        if (min > temp) {
            avg = sta_in.sensor_adj.TEMP[i];
            min = temp;
        }
    }

    cal_inf.BaseTemp = avg;

}

#define TEMP_TOTAL		(0)
#define TEMP_A			(1)
#define TEMP_B			(2)
#define TEMP_C			(3)
s16 Get_AVG_Temp(u8 type)
{
    s32 temp_total = 0;
    u8 i = 0, time = 0;

    if (type > TEMP_C) {
        return TEMP_ER_VALUE;
    }

    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if (cal_inf.VaiTemp_id[i] == 0 ) { //无效温度不计算
            continue;
        }

        if (type != 0 && dev_inf.TEMP_Define[type - 1][i] != STA_ON) {
            continue;
        }

        if ((cal_inf.VaiTemp_num >= 3) && ctrl_inf.var.temp_diff != 0
                && (abs(sta_in.sensor_adj.TEMP[i] - cal_inf.BaseTemp) > ctrl_inf.var.temp_diff)) {//需要报警
            continue;
        }

        temp_total += sta_in.sensor_adj.TEMP[i];
        time++;
    }

    if (time == 0) {
        return TEMP_ER_VALUE;
    }

    temp_total = temp_total / time;
    return (s16)temp_total;
}

//找出设置的温度个数
int Find_Set_Temp_Num(void)
{
    u8 i = 0, total = 0;

    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if (dev_inf.sensor_adj.TEMP_EN[i] == STA_ON) {
            total++;
        }
    }

    return total;
}

u8 AIN_ID[SENSOR_NUM_MAX] = {AIN_AI1, AIN_AI2, AIN_AI3, AIN_AI4, AIN_AI5, AIN_AI6};

int get_sensor_sta(u8 i)
{
    int id = 0;

    if (i >= SENSOR_NUM_MAX) {
        return -1;
    }

    for (id = 0; id < SENSOR_NUM_MAX; id++) {
        if (AIN_ID[id] == i + 7) {
            break;
        }
    }

	if(id == 4 && dev_inf.WIN.detail[0].mode == WIN_MODE_POS){
		return 1;
	}

	if(id == 5 && dev_inf.WIN.detail[1].mode == WIN_MODE_POS){
		return 1;
	}
	

    if (dev_inf.sensor_adj.SENSOR_EN[id] == SENSOR_TYPE_DIS) {
        return -1;
    } else {
        return 1;
    }
}

void Win_ReadIn(void)
{
    sta_in.sensor_real.WIN[0] 	= AD0_10V_TO_PER(Get_AD_Value(AIN_AI5));
    sta_in.sensor_real.WIN[1] 	= AD0_10V_TO_PER(Get_AD_Value(AIN_AI6));
    sta_in.sensor_real.WIN[2] 	= AD0_10V_TO_PER(Get_AD_Value(AIN_WIN1));
    sta_in.sensor_real.WIN[3] 	= AD0_10V_TO_PER(Get_AD_Value(AIN_WIN2));

	
    sta_in.sensor_adj.WIN[0] = sta_in.sensor_real.WIN[0];
    sta_in.sensor_adj.WIN[1] = sta_in.sensor_real.WIN[1];
    sta_in.sensor_adj.WIN[2] = sta_in.sensor_real.WIN[2];
    sta_in.sensor_adj.WIN[3] = sta_in.sensor_real.WIN[3];
}

const u16 Sensor_Min[SENSOR_TYPE_NUM] = {2, 3, 2, 100, 2, 2, 2, 10, 2};
const u16 Sensor_Time[SENSOR_TYPE_NUM] = {10, 10, 10, 10, 10, 10, 4, 6, 10};

void Sensor_ReadIn(void)
{
    u8 i = 0, j = 0, num = 0, en = 0, er = 0;
    signed short 	temp, min_temp, max_temp, error_value;
    signed short ai_temp;
    int total;

//传感器实际值
    for (i = 0; i < TEMP_NUM_MAX; i++) {
        temp = AD_TO_Temp(Get_AD_Value(i));

        if (abs(sta_in.sensor_real.TEMP[i] - temp) >= 2 || run_inf.temp[i] >= 10) {
            if (sta_in.sensor_real.TEMP[i] != temp) {
                run_inf.temp[i] = 0;
            }

            sta_in.sensor_real.TEMP[i] = temp;
        }
    }

    for (i = 0; i < SENSOR_NUM_MAX; i++) {
        ai_temp = AD4_20Ma_TO((dev_inf.sensor_adj.SENSOR_EN[i]), Get_AD_Value(AIN_ID[i]));

        if (abs(sta_in.sensor_real.SENSOR[i] - ai_temp) >= Sensor_Min[(dev_inf.sensor_adj.SENSOR_EN[i])] || run_inf.ai_temp[i] >= Sensor_Time[(dev_inf.sensor_adj.SENSOR_EN[i])]) {
            if (sta_in.sensor_real.SENSOR[i] != ai_temp) {
                run_inf.ai_temp[i] = 0;
            }

            sta_in.sensor_real.SENSOR[i] = ai_temp;
        }

    }

//传感器修正值和其他传感器的平均值
    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if ((basic_inf.tempc_en.enable == STA_ON && (i + 1) == basic_inf.tempc_en.temp_id)
                || (basic_inf.tempc_pwm_en.enable == STA_ON && (i + 1) == basic_inf.tempc_pwm_en.temp_id)) { //单独温控只判断故障
            if (sta_in.sensor_real.TEMP[i] != TEMP_ER_VALUE) {
                sta_in.sensor_adj.TEMP[i] = sta_in.sensor_real.TEMP[i] + dev_inf.sensor_adj.TEMP[i];
            } else {
                sta_in.sensor_adj.TEMP[i] = sta_in.sensor_real.TEMP[i];
            }

            continue;
        }

        if (i == TEMP_OUT_SID) {
            max_temp = TEMP_OUTMAX_ERR;
            min_temp = TEMP_OUTMIN_ERR;
        } else {
            max_temp = TEMP_MAX_ERR;
            min_temp = TEMP_MIN_ERR;
        }

        if (sta_in.sensor_real.TEMP[i] > max_temp || sta_in.sensor_real.TEMP[i] < min_temp) { //出错不校准
            sta_in.sensor_adj.TEMP[i] = sta_in.sensor_real.TEMP[i];
            continue;
        }

        sta_in.sensor_adj.TEMP[i] = sta_in.sensor_real.TEMP[i] + dev_inf.sensor_adj.TEMP[i];
    }

    memset(cal_inf.VaiSen_id, 0, sizeof(cal_inf.VaiSen_id));

    for (i = SENSOR_TYPE_TEMP; i < SENSOR_TYPE_NUM; i++) {
        if (sys_sta.sl_set.SEN_EN[i] == STA_ON) {
            cal_inf.SEN_EN[i] = STA_ON;
            cal_inf.SEN[i] = sys_sta.sl_set.SEN[i];
            continue;
        }

        if (i == SENSOR_TYPE_TEMP) {
            error_value = (short)TEMP_ER_VALUE;
        } else if (i == SENSOR_TYPE_VAC) {
            error_value = (short)VAC_ER_VALUE;
        } else {
            error_value = (short)SENSOR_ER_VALUE;
        }

        er = STA_OFF;

        for (total = 0, num = 0, en = 0, j = 0; j < SENSOR_NUM_MAX; j++) {

            if (dev_inf.sensor_adj.SENSOR_EN[j] != i)continue;

            en++;

            if (sta_in.sensor_real.SENSOR[j] == (short)error_value) {
                er = STA_ON;
                sta_in.sensor_adj.SENSOR[j] = sta_in.sensor_real.SENSOR[j];
                continue;
            }

            sta_in.sensor_adj.SENSOR[j] = sta_in.sensor_real.SENSOR[j] + dev_inf.sensor_adj.SENSOR[j];
            total += sta_in.sensor_adj.SENSOR[j];
            num ++;
            cal_inf.VaiSen_id[j] = 1;
        }

        if (en == 0) {
            cal_inf.SEN_ER[i] = STA_OFF;
            cal_inf.SEN_EN[i] = STA_OFF;
            cal_inf.SEN[i] = (short)error_value;
        } else {
            cal_inf.SEN_ER[i] = er;
            cal_inf.SEN_EN[i] = STA_ON;

            if (num == 0) {
                cal_inf.SEN[i] = (short)error_value;
            } else {
                cal_inf.SEN[i] = total / num;
            }
        }
    }

    Find_Temp_Base();

    if (ctrl_inf.var.mode == STA_ON && (Get_Temp_State() < 0 || Get_Sen_State() < 0)) {
        AD7327_Init();
        OSTimeDly(100);
        index_inf.restart_time++;
        Config_Save(SINDEX_ID);//记录保存
    }

    if (sys_sta.sl_set.temp_en == STA_ON) {
        sys_sta.temp_err = STA_OFF;
        cal_inf.temp = sys_sta.sl_set.temp;
    } else {
        temp = Get_AVG_Temp(TEMP_TOTAL);

        if (temp == TEMP_ER_VALUE) {
            temp = cal_inf.target_temp;//默认目标温度，运行最小通风级别
            sys_sta.temp_err = STA_ON;

            do {
                if (ctrl_inf.var.mode == STA_OFF) {//空舍不重启
                    continue;
                }

                // 温度全部不对而且有启用了温度
                if (Find_Set_Temp_Num() <= 0 || sys_sta.temp_success == STA_OFF) {
                    continue;
                }

                //先重启7327，再重启系统
                if (sys_sta.reset_flag == 0) {
                    //读函数也在同一个任务里面可以直接重新初始化
                    AD7327_Init();
                    OSTimeDly(100);
                    index_inf.restart_time++;
                    sys_sta.reset_flag++;
                    Config_Save(SINDEX_ID);//记录保存
                    continue;
                }

                sys_sta.reset_flag++;

                //系统启动3秒后重启
                if (sys_sta.on_sec >= 3 * MIN_TO_SEC && sys_sta.reset_flag >= 30) {
                    index_inf.restart_time++;
                    Config_Save(SINDEX_ID);//记录保存
                    SystemReset();
                }
            } while (0);
        } else {
            sys_sta.reset_flag = 0;
            sys_sta.temp_success = STA_ON;
            sys_sta.temp_err = STA_OFF;
        }

        cal_inf.temp = temp;
    }

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        temp = Get_AVG_Temp(i + 1);

        if (temp == TEMP_ER_VALUE) {
            temp = cal_inf.temp;
        }

        cal_inf.tempArea[i] = temp;
    }

}

//计算目标值
int Update_Aim_MinAir(void)
{
    int res = 0;
    U8 i = 0, min_lv = 0, max_lv;
    int day = 0;
    float scale;

    if (ctrl_inf.minair_en != STA_ON) {
        res = -1;
        return res;
    }

//查找大小值
    for (max_lv = 0, min_lv = 0, i = 0; i < MINAIR_NUM; i++) {
        if (ctrl_inf.minair[i].enable != STA_ON) {
            continue;
        }

        if (basic_inf.grow_day >= ctrl_inf.minair[i].grow_day) {
            min_lv = i;
        } else {
            max_lv = i;
            break;
        }
    }

    if (min_lv == 0 && ctrl_inf.minair[0].enable != STA_ON) { //没找到小的
        if (max_lv == 0) {
            res = -2;
            return res;
        }

        min_lv = max_lv;
    }

    if (max_lv == 0) { //没有找到大的
        max_lv = min_lv;
    }

    if (min_lv == max_lv) {
        res = ctrl_inf.minair[min_lv].min_air;
        return res;
    }

    day = ctrl_inf.minair[max_lv].grow_day - ctrl_inf.minair[min_lv].grow_day;

    if (day < 1) {
        res = -3;
        return res;
    }

    scale = ((int)(ctrl_inf.minair[max_lv].min_air - ctrl_inf.minair[min_lv].min_air) * 1.0f / day);
    res = ctrl_inf.minair[min_lv].min_air + scale * (basic_inf.grow_day -  ctrl_inf.minair[min_lv].grow_day);
    return res;
}

int cal_hum_min_adj(unsigned char cur_hum)
{
    int res = 0;

    if (ctrl_inf.hum.min_adj_hum <= 1 || run_inf.old_adj_hum == MAX_1BYTE) {//小于等于1，直接调节
        return cur_hum;
    }

    if ((cur_hum - cal_inf.target_hum) % ctrl_inf.hum.min_adj_hum == 0) {
        return cur_hum;
    }

    if (abs(cur_hum - run_inf.old_adj_hum) >= ctrl_inf.hum.min_adj_hum) {
        if (cur_hum < run_inf.old_adj_hum) {
            res = (cur_hum + ctrl_inf.hum.min_adj_hum) / ctrl_inf.hum.min_adj_hum;
        } else {
            res = (cur_hum) / ctrl_inf.hum.min_adj_hum;
        }//取整

        return res * ctrl_inf.hum.min_adj_hum;
    }


    return -1;
}

void get_hum_fix(void)
{
    int temp = 0, res = 0;
    unsigned char i = 0, min_lv = 0xFF;
    unsigned char cur_hum = 0;
    int now_time;
    time_t sys_time;

    if (ctrl_inf.hum.enable != STA_ON) {
        cal_inf.target_hum = SENSOR_ER_VALUE;
        run_inf.old_adj_hum = MAX_1BYTE;
        cal_inf.tar_hum_temp = 0;
        return ;
    }

//查找小值
    for (i = 0; i < HUM_CURVE_NUM; i++) {
        if (ctrl_inf.hum.x[i].enable != STA_ON) {
            continue;
        }

        if (basic_inf.grow_day >= ctrl_inf.hum.x[i].grow_day) {
            min_lv = i;
        } else {
            break;
        }
    }

    if (min_lv == 0xFF) { //没找到小的
        cal_inf.target_hum = SENSOR_ER_VALUE;
        run_inf.old_adj_hum = MAX_1BYTE;
        cal_inf.tar_hum_temp = 0;
        return ;
    }




    cal_inf.target_hum = ctrl_inf.hum.x[min_lv].target_hum;

    if (cal_inf.SEN[SENSOR_TYPE_HUM] == (short)SENSOR_ER_VALUE || cal_inf.SEN_EN[SENSOR_TYPE_HUM] != STA_ON) {
        run_inf.old_adj_hum = MAX_1BYTE;
        cal_inf.tar_hum_temp = 0;
        return ;
    }

///<查找时间段限制
    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟

    if (ctrl_inf.hum.start_time < ctrl_inf.hum.end_time) {
        if (now_time < ctrl_inf.hum.start_time || now_time > ctrl_inf.hum.end_time) {
            run_inf.old_adj_hum = MAX_1BYTE;
            cal_inf.tar_hum_temp = 0;
            return ;
        }
    } else if (ctrl_inf.hum.start_time > ctrl_inf.hum.end_time) {
        if (now_time < ctrl_inf.hum.start_time && now_time > ctrl_inf.hum.end_time) {
            run_inf.old_adj_hum = MAX_1BYTE;
            cal_inf.tar_hum_temp = 0;
            return ;
        }
    }


    if (cal_inf.SEN[SENSOR_TYPE_HUM] < ctrl_inf.hum.start_hum) {
        cur_hum = ctrl_inf.hum.start_hum;
    } else if (cal_inf.SEN[SENSOR_TYPE_HUM] > ctrl_inf.hum.stop_hum) {
        cur_hum = ctrl_inf.hum.stop_hum;
    } else {
        cur_hum = cal_inf.SEN[SENSOR_TYPE_HUM];
    }

    res = cal_hum_min_adj(cur_hum);

    if (res < 0) {
        return;//保持原来的
    }

    cur_hum = res;
    run_inf.old_adj_hum = cur_hum;

    temp = (cal_inf.target_hum - cur_hum) * ctrl_inf.hum.x[min_lv].scale * TEMP_RATE / HUM_SCALE;

    if (abs(temp) > ctrl_inf.hum.max_temp) {
        if (temp > 0) {
            temp = ctrl_inf.hum.max_temp;
        } else {
            temp = -ctrl_inf.hum.max_temp;
        }
    }

    cal_inf.tar_hum_temp = temp;
}

int GetDayAimTemp(int offset, int grow_day)
{
    signed short *min_temp, *max_temp;
    int res = 0;
    U8 i = 0, min_lv = 0, max_lv;
    int day = 0;
    float scale;

//    if (ctrl_inf.temp_en != STA_ON) {
//        res = -1;
//        return res;
//    }

//查找大小值
    for (max_lv = 0, min_lv = 0, i = 0; i < TEMP_CURVE_NUM; i++) {
        if (ctrl_inf.temp[i].enable != STA_ON) {
            continue;
        }

        if (grow_day >= ctrl_inf.temp[i].grow_day) {
            min_lv = i;
        } else {
            max_lv = i;
            break;
        }
    }

    if (min_lv == 0 && ctrl_inf.temp[0].enable != STA_ON) { //没找到小的
        if (max_lv == 0) {
            min_temp = (signed short*)(&ctrl_inf.temp[min_lv].enable + offset);
            res = (signed short)(*min_temp);
            return res;
        }

        min_lv = max_lv;
    }

    if (max_lv == 0) { //没有找到大的
        max_lv = min_lv;
    }

    if (min_lv == max_lv) {
        min_temp = (signed short*)(&ctrl_inf.temp[min_lv].enable + offset);
        res = (signed short)(*min_temp);
        return res;
    }

    day = ctrl_inf.temp[max_lv].grow_day - ctrl_inf.temp[min_lv].grow_day;

    if (day < 1) {
        min_temp = (signed short*)(&ctrl_inf.temp[min_lv].enable + offset);
        res = (signed short)(*min_temp);
        return res;
    }

    min_temp = (signed short*)(&ctrl_inf.temp[min_lv].enable + offset);
    max_temp = (signed short*)(&ctrl_inf.temp[max_lv].enable + offset);


    scale = (float)(((signed short)(*max_temp) - (signed short)(*min_temp)) * 1.0f / day);
    res = (signed short)(*min_temp) + scale * (grow_day -  ctrl_inf.temp[min_lv].grow_day);//按天计算的值

    return res;
}
//计算目标值 flag = 1,不算修正值，不按时间段变化
int Update_Aim_Temp(int offset, unsigned char flag)
{
    int res = 0, old_temp;
    float scale;

    res = GetDayAimTemp(offset, basic_inf.grow_day);

    if (flag == STA_ON) { //不算修正值，不按时间段变化
        return res;
    }

//按小时再次计算一遍
    if (ctrl_inf.extemp.chg_times * ctrl_inf.extemp.chg_hours > 24) {//时间
        ctrl_inf.extemp.chg_times = 24 / ctrl_inf.extemp.chg_hours;
    }

    if (sys_sta.update_hour >= ctrl_inf.extemp.chg_times || ctrl_inf.extemp.day_temp_en == STA_OFF) {
        sys_sta.update_hour = MAX_1BYTE;
        return res;
    }

    old_temp = GetDayAimTemp(offset, basic_inf.grow_day - 1);
    scale = (float)((res - old_temp) * 1.0f / ctrl_inf.extemp.chg_times);
    res = old_temp + scale * sys_sta.update_hour;

    return res;
}


//计算分区温度
int Update_APWM_Freq(int id )
{
    int min_freq, max_freq;
    int res = 0;
    U8 i = 0, min_lv = 0, max_lv;;
    int offset = 0, offset_scale = 0;
    float scale;

    if (id >= APWM_NUM_MAX) {
        res = -1;
        return res;
    }

    if (basic_inf.area_pwm[id].enable != STA_ON) {
        res = -2;
        return res;
    }


    if (basic_inf.area_pwm[id].target_temp == 0) {
        offset = cal_inf.tempArea[id] - cal_inf.target_temp;
    } else {
        offset = cal_inf.tempArea[id] - basic_inf.area_pwm[id].target_temp;
    }

//查找大小值
    for (max_lv = 0, min_lv = 0, i = 0; i < FREQ_NUM; i++) {
        if (basic_inf.area_pwm[id].freq[i].enable != STA_ON) {
            continue;
        }

        if (offset >= basic_inf.area_pwm[id].freq[i].offset) {
            min_lv = i;
        } else {
            max_lv = i;
            break;
        }
    }

    if (min_lv == 0 && basic_inf.area_pwm[id].freq[0].enable != STA_ON) { //没找到小的
        if (max_lv == 0) {
            res = -2;
            return res;
        }

        min_lv = max_lv;
    }

    if (max_lv == 0) { //没有找到大的
        max_lv = min_lv;
    }

    if (min_lv == max_lv) {
        res = basic_inf.area_pwm[id].freq[min_lv].freq;
        return res;
    }

    offset_scale = basic_inf.area_pwm[id].freq[max_lv].offset - basic_inf.area_pwm[id].freq[min_lv].offset;

    if (offset_scale == 0) {
        res = basic_inf.area_pwm[id].freq[min_lv].freq;
        return res;
    }

    min_freq = basic_inf.area_pwm[id].freq[min_lv].freq;
    max_freq = basic_inf.area_pwm[id].freq[max_lv].freq;


    scale = (float)((max_freq - min_freq) * 1.0f / offset_scale);
    res = min_freq + scale * (offset -  basic_inf.area_pwm[id].freq[min_lv].offset);//按天计算的值

    return res;
}


int Update_Circle_Sta(void)
{
    u8 i = 0, num = 0;
    s16 max_temp = TEMP_MIN, min_temp = TEMP_MAX;
    time_t sys_time;
    int now_time;

    if (dev_inf.circle.enable == STA_OFF) {
        return STA_OFF;
    }

    //级别不符合
    if (cal_inf.lv < dev_inf.circle.start_lv || cal_inf.lv > dev_inf.circle.stop_lv) {
        return STA_OFF;
    }

    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟

    //时间不符合
    if (now_time < dev_inf.circle.start_time || now_time > dev_inf.circle.stop_time) {
        return STA_OFF;
    }

    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if (dev_inf.circle.temp_sta[i] == STA_OFF) {
            continue;
        }

        num ++;

        if (sta_in.sensor_real.TEMP[i] == TEMP_ER_VALUE) {
            continue;
        }

        if (max_temp < sta_in.sensor_adj.TEMP[i]) {
            max_temp = sta_in.sensor_adj.TEMP[i];
        }

        if (min_temp > sta_in.sensor_adj.TEMP[i]) {
            min_temp = sta_in.sensor_adj.TEMP[i];
        }
    }

    if (num == 0) {//没有选择温度
        return STA_OFF;
    }

    if (num == 1) { //只有一个温度，与平均值比较
        if (abs(max_temp - cal_inf.temp) >= abs(dev_inf.circle.offset)) {
            return STA_ON;

        } else {
            return STA_OFF;
        }
    }

    if (abs(max_temp - min_temp) >= abs(dev_inf.circle.offset)) {
        return STA_ON;

    } else {
        return STA_OFF;

    }
}

int Update_Timer_Sta(u8 id)
{
    u8 i = 0;
    time_t sys_time;
    int now_time;

    if (ext_inf.timer[id].mode == TIMER_MODE_CYCLE || ext_inf.timer[id].enable != STA_ON) {
        return STA_OFF;
    }

    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟

    if (ext_inf.timer[id].mode == TIMER_MODE_ON_DELAY) {//计算结束时间
        for (i = 0; i < TIMER_NUM; i++) {
            if (ext_inf.timer[id].enable != STA_ON)
                continue;

            ext_inf.timer[id].stop_time[i] = ext_inf.timer[id].start_time[i] + ext_inf.timer[id].delay_min[i];
        }
    }

    for (i = 0; i < TIMER_NUM; i++) {
        if (ext_inf.timer[id].num_en[i] != STA_ON)
            continue;

// 定时开关时，如果停止时间小于启动时间判断逻辑修改一下
        if (ext_inf.timer[id].mode == TIMER_MODE_ON_OFF && ext_inf.timer[id].stop_time[i] < ext_inf.timer[id].start_time[i]) {
            if (now_time >= ext_inf.timer[id].start_time[i] || now_time < (ext_inf.timer[id].stop_time[i] % DAY_TO_MIN)) {
                return STA_ON;
            }
        }

        if ((ext_inf.timer[id].stop_time[i] / DAY_TO_MIN) == 0 &&
                (now_time >= ext_inf.timer[id].start_time[i] && now_time < ext_inf.timer[id].stop_time[i])) {
            return STA_ON;
        }

        if ((ext_inf.timer[id].stop_time[i] / DAY_TO_MIN) > 0 && //结束时间超过一天了
                (now_time >= ext_inf.timer[id].start_time[i] || now_time < (ext_inf.timer[id].stop_time[i] % DAY_TO_MIN))) {
            return STA_ON;
        }
    }

    return STA_OFF;
}

//结果为0时 在死区和目标之间的
int Check_Sencon(u8 i)
{
    unsigned short 	target;				//目标值

    if (cal_inf.target_hum != SENSOR_ER_VALUE && i == SENCON_HUM) {
        target = cal_inf.target_hum;
    } else {
        target = ctrl_inf.sencon[i].target;
    }

    if (cal_inf.SEN[i + SENSOR_TYPE_HUM] == (short)SENSOR_ER_VALUE) {
        return -1;
    }

//低于加热
    if (ctrl_inf.sencon[i].heat_flag == STA_OFF && cal_inf.temp < cal_inf.heat_temp) {
        return -2;
    }

//降温幅度
    if (run_inf.sencon_Mode[i] == SENCON_START && (cal_inf.temp + ctrl_inf.sencon[i].temp_drop) < run_inf.sencon_Temp[i]) {
        return -3;
    }

    if (cal_inf.SEN[i + SENSOR_TYPE_HUM] >= target) {
        return 1;
    }

    if (cal_inf.SEN[i + SENSOR_TYPE_HUM] < (target - ctrl_inf.sencon[i].dead_zone)) {
        return -4;
    }

//在死区和目标之间的
    return 0;
}
void Sencon_Default(u8 i)
{
    run_inf.sencon_Mode[i] = SENCON_READY;
    run_inf.sencon_Lv[i] = 0;
    run_inf.sencon_Temp[i] = 0;
}
void Sencon_Proc(void)
{
    u8 i = 0;

    for (i = 0; i < SENCON_NUM; i++) {
        if (cal_inf.SEN_EN[i + SENSOR_TYPE_HUM] != STA_ON ||
                ctrl_inf.sencon[i].enable != STA_ON ) {
            Sencon_Default(i);
            continue;
        }

        if (run_inf.sencon_Time[i] != 0) {
            if (Check_Sencon(i) <= 0) {
                Sencon_Default(i);
                run_inf.sencon_Time[i] = 0;
            }

            continue;
        }

        switch (run_inf.sencon_Mode[i]) {
        case SENCON_READY:
            if (Check_Sencon(i) > 0) {
                run_inf.sencon_Time[i] = ctrl_inf.sencon[i].delay_min * MIN_TO_SEC * RUN_FREQ;
                run_inf.sencon_Mode[i] = SENCON_DELAY;
            }

            break;

        case SENCON_DELAY:
            if (Check_Sencon(i) > 0) {
                run_inf.sencon_Temp[i] = cal_inf.temp;
                run_inf.sencon_Mode[i] = SENCON_START;
            } else {
                Sencon_Default(i);
            }

            break;

        case SENCON_START:
            if (Check_Sencon(i) >= 0) {
                if (run_inf.sencon_Lv[i] < CTRL_LV_NUM - 1) {
                    run_inf.sencon_Lv[i]++;
                }

                run_inf.sencon_Time[i] = ctrl_inf.sencon[i].continue_sec * RUN_FREQ;
            } else {
                Sencon_Default(i);
            }

            break;

        default:
            Sencon_Default(i);
            break;
        }
    }
}

void VAC_Default()
{
    run_inf.VAC_ctrl_flag = VAC_CTRL_NONE;
    run_inf.VAC_Time = 0;
    run_inf.VAC_MoveTime = 0;
    run_inf.VAC_Mode = SENCON_READY;
    run_inf.VAC_hold_time = 0;
}

int Check_VAC(void)
{
    if (cal_inf.SEN[SENSOR_TYPE_VAC] == VAC_ER_VALUE || cal_inf.total_fan_num == 0) {
        return VAC_CTRL_NONE;
    }

    if (run_inf.VAC_Mode == SENCON_START && run_inf.VAC_ctrl_flag == VAC_CTRL_BIG) { //校准过程中，而且之前的静压是大的
        if (abs(cal_inf.SEN[SENSOR_TYPE_VAC]) > abs(cal_inf.target_vac) + 1) {
            return VAC_CTRL_BIG;
        }
    } else {
        if (abs(cal_inf.SEN[SENSOR_TYPE_VAC]) > abs(cal_inf.target_vac) + ctrl_inf.vac.dead_zone) {
            return VAC_CTRL_BIG;
        }
    }

    if (run_inf.VAC_Mode == SENCON_START && run_inf.VAC_ctrl_flag == VAC_CTRL_SAMLL) { //校准过程中，而且之前的静压是小的
        if (abs(cal_inf.SEN[SENSOR_TYPE_VAC]) < abs(cal_inf.target_vac) - 1) {
            return VAC_CTRL_SAMLL;
        }
    } else {
        if (abs(cal_inf.SEN[SENSOR_TYPE_VAC]) < abs(cal_inf.target_vac) - ctrl_inf.vac.dead_zone) {
            return VAC_CTRL_SAMLL;
        }
    }

//在死区和目标之间的
    return VAC_CTRL_NONE;
}

//计算目标压力
int get_vac_target(void)
{
    signed short temp_cal = 0;
    int vac_res;
    float scale = 0;

    if (dev_inf.sensor_adj.TEMP_EN[TEMP_OUT_SID] == STA_OFF || sta_in.sensor_real.TEMP[TEMP_OUT_SID] == TEMP_ER_VALUE) {
        temp_cal = cal_inf.temp;
    } else {
        temp_cal = sta_in.sensor_real.TEMP[TEMP_OUT_SID];
    }

    if (ctrl_inf.vac.targetL == ctrl_inf.vac.targetH) {
        return ctrl_inf.vac.targetH;
    }

    if (temp_cal <= cal_inf.target_temp + ctrl_inf.vac.low_tempdiff) {
        return ctrl_inf.vac.targetL;
    } else if (temp_cal >= cal_inf.target_temp + ctrl_inf.vac.high_tempdiff) {
        return ctrl_inf.vac.targetH;
    }

    scale = (float)((ctrl_inf.vac.targetH - ctrl_inf.vac.targetL) * 1.0f / (ctrl_inf.vac.high_tempdiff - ctrl_inf.vac.low_tempdiff) );
    vac_res = ctrl_inf.vac.targetL + scale * (temp_cal -  (cal_inf.target_temp + ctrl_inf.vac.low_tempdiff));

    return vac_res;

}

int get_swin_vac_control(u8 id, u8 lv_id)
{
    u8 degree = 0;

    if (dev_inf.SWIN.detail[id].enable != STA_ON) {
        return -1;
    }

	if(lv_id < ctrl_inf.vac.min_ctrllv || lv_id > ctrl_inf.vac.max_ctrllv){//静压控制级别限制
        return -1;
	}

    degree = ctrl_inf.lv[lv_id].SWIN[id].degree;

    if ((ctrl_inf.vac.type == STA_ON && dev_inf.SWIN.detail[id].ctrl_mode == WIN_VAC_CTRL)
            && degree != 0) {
        return 1;
    }

    return -1;
}

int get_win_vac_control(u8 id, u8 lv_id)
{
    u8 degree = 0;

    if (dev_inf.WIN.detail[id].enable != STA_ON) {
        return -1;
    }
	
	if(lv_id < ctrl_inf.vac.min_ctrllv || lv_id > ctrl_inf.vac.max_ctrllv){//静压控制级别限制
        return -1;
	}
	
    if (id <= WIN2_ID) {
        degree = ctrl_inf.lv[lv_id].WIN[id].degree;
    } else {
        degree = ctrl_inf.exlv[lv_id].WIN[id - WIN3_ID].degree;
    }

    if ((ctrl_inf.vac.type == STA_ON && dev_inf.WIN.detail[id].ctrl_mode == WIN_VAC_CTRL)
            && degree != 0) {
        return 1;
    }

    return -1;
}

void swin_degree_save(u8 lv_id)
{
    int i = 0;

    for (i = 0; i < SWINDOW_NUM_MAX; i++) {
        if (get_swin_vac_control(i, lv_id) > 0) {
            ext_inf.lv_save[lv_id].SWIN[i] = sta_out.ALLPWM[i];
            run_inf.SWIN_SETdegree[i] = sta_out.ALLPWM[i];//防止回调
        }
    }
}

void win_degree_save(u8 lv_id)
{
    int i = 0;

    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (get_win_vac_control(i, lv_id) > 0) {
            ext_inf.lv_save[lv_id].WIN[i] = sta_out.WIN[i];
            run_inf.WIN_LvDegree[i] = sta_out.WIN[i];//防止回调
        }
    }
}

int get_win_adjust(void)
{
    int i = 0;

    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (dev_inf.WIN.detail[i].enable != STA_ON || dev_inf.WIN.detail[i].mode != WIN_MODE_TIME) {
            continue;
        }

        if (run_inf.WIN_CalMode[i] != ADJ_READY) {//校准中
            return STA_ON;
        }
    }

    return STA_OFF;
}
int get_win_only_vac_control(u8 id, u8 lv_id)
{
    u8 degree = 0;

    if (dev_inf.WIN.detail[id].enable != STA_ON) {
        return -1;
    }

    if (id <= WIN2_ID) {
        degree = ctrl_inf.lv[lv_id].WIN[id].degree;
    } else {
        degree = ctrl_inf.exlv[lv_id].WIN[id - WIN3_ID].degree;
    }

    if ((ctrl_inf.vac.type == STA_ON && dev_inf.WIN.detail[id].ctrl_mode == WIN_ONLY_VAC_CTRL)
            && degree != 0) {
        return 1;
    }

    return -1;
}

int GetVacAdjustQuick(void)
{
    if (abs(cal_inf.target_vac - cal_inf.SEN[SENSOR_TYPE_VAC]) > 20) {
        return 1;
    }

    return -1;
}


void WIN_VAC_CTR(u8 VAC_ctrl_flag)
{
    int i = 0;

    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (dev_inf.WIN.detail[i].enable != STA_ON || dev_inf.WIN.detail[i].ctrl_mode != WIN_ONLY_VAC_CTRL) {
            continue;
        }

        if (run_inf.WIN_SWflag[i] == STA_ON) {
            continue;
        }

        if (get_win_only_vac_control(i, cal_inf.lv) <= 0) {
            run_inf.VAC_MoveTime = 0;
            run_inf.VAC_Time = 0;
            continue;
        }

        if (VAC_ctrl_flag == VAC_CTRL_BIG) {
            WIN_DOWN_CON(i, STA_OFF);
            WIN_UP_CON(i, STA_ON);

            if (ctrl_inf.vac.adj_move_s == 0 || ctrl_inf.vac.adj_delay_s == 0 || GetVacAdjustQuick() > 0) { //不停
                run_inf.VAC_MoveTime = 0;
            } else {
                run_inf.VAC_MoveTime = ctrl_inf.vac.adj_move_s * RUN_FREQ;
            }

            run_inf.VAC_Time = (ctrl_inf.vac.adj_move_s + ctrl_inf.vac.adj_delay_s) * RUN_FREQ; //增加停风窗控制
        } else if (VAC_ctrl_flag == VAC_CTRL_SAMLL) {
            WIN_UP_CON(i, STA_OFF);
            WIN_DOWN_CON(i, STA_ON);

            if (ctrl_inf.vac.adj_move_s == 0 || ctrl_inf.vac.adj_delay_s == 0 || GetVacAdjustQuick() > 0) {//不停
                run_inf.VAC_MoveTime = 0;
            } else {
                run_inf.VAC_MoveTime = ctrl_inf.vac.adj_move_s * RUN_FREQ;
            }

            run_inf.VAC_Time = (ctrl_inf.vac.adj_move_s + ctrl_inf.vac.adj_delay_s) * RUN_FREQ; //增加停风窗控制
        } else {
            WIN_UP_CON(i, STA_OFF);
            WIN_DOWN_CON(i, STA_OFF);
            run_inf.VAC_MoveTime = 0;
            run_inf.VAC_Time = 0;
        }
    }
}
void VAC_Proc(void)
{
    int i = 0;

    if (sys_sta.auto_mode == STA_OFF || get_win_adjust_sta() == STA_ON) {
        return;
    }

    cal_inf.target_vac = get_vac_target();

//	if(ctrl_inf.vac.type == VAC_CTRL_STOP){//静压不控制，保存的数据恢复默认
//		lv_save_default();
//	}

    if (cal_inf.SEN_EN[SENSOR_TYPE_VAC] != STA_ON || ctrl_inf.vac.type != STA_ON
            || cal_inf.SEN[SENSOR_TYPE_VAC] == VAC_ER_VALUE
            || cal_inf.lv < ctrl_inf.vac.min_ctrllv || cal_inf.lv > ctrl_inf.vac.max_ctrllv) {
        VAC_Default();
        return;
    }

    if (run_inf.lv_win_delay != 0 || get_win_adjust() == STA_ON) { //切换级别或者风机校准时 不调节 不同级别没调到合适不保存
        VAC_Default();
    }

    if (Check_VAC() == VAC_CTRL_NONE) { //检测负压合适 同一级别没调到合适保存
        run_inf.VAC_ctrl_flag = VAC_CTRL_NONE;
        WIN_VAC_CTR(run_inf.VAC_ctrl_flag);

        if (run_inf.VAC_Mode == SENCON_START) {//当前负压合适，等待保存数据
            for (i = 0; i < WINDOW_NUM_MAX; i++) {//当前负压合适，关闭继电器动作
                if (get_win_vac_control(i, cal_inf.lv) > 0) {
                    if (dev_inf.WIN.detail[i].enable == STA_ON) {
                        WIN_UP_CON(i, STA_OFF);
                        WIN_DOWN_CON(i, STA_OFF);
                    }
                }
            }

            run_inf.VAC_hold_time++;
            swin_degree_save(cal_inf.lv);//第一次先保存小窗的数据防止小窗回调

            if (run_inf.VAC_hold_time >= 3) {
                VAC_Default();
                win_degree_save(cal_inf.lv);
                lv_save_data(cal_inf.lv);
            }
        }

        if (run_inf.VAC_hold_time == 0) {
            VAC_Default();
        }

        return;
    }

    //从风机启动后或者切换级别后需要一个产生延时,此时间过后静压不合适直接调整

    if (run_inf.VAC_Time != 0) {
        return;
    }

    //风机稳定后需要一个阵风延时
    switch (run_inf.VAC_Mode) {
    case SENCON_READY:
        run_inf.VAC_Time = ctrl_inf.vac.delay_s * RUN_FREQ;
        run_inf.VAC_Mode = SENCON_START;
        break;

    case SENCON_START:
        run_inf.VAC_ctrl_flag = Check_VAC();
        WIN_VAC_CTR(run_inf.VAC_ctrl_flag);
        break;

    default:
        VAC_Default();
        break;
    }

}


// 当前级别是否有时控启停的
int Find_LV_TimeCon(int id )
{
    int i = 0;

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        if (dev_inf.JDQ[i].enable == STA_ON &&
                (ctrl_inf.lv[id].JDQ[i].mode == MODE_ON_OFF
                 || ctrl_inf.lv[id].JDQ[i].mode == MODE_ROTATE))
            return 1;
    }

    for (i = 0; i < PWM_NUM_MAX; i++) {
        if (dev_inf.PWM[i].enable == STA_ON &&
                (ctrl_inf.lv[id].PWM[i].mode == MODE_ON_OFF
                 || ctrl_inf.lv[id].PWM[i].mode == MODE_ROTATE))
            return 1;
    }

    return -1;
}

void Lv_Time_Def(void)
{
    if (Find_LV_TimeCon(cal_inf.lv) < 0 || Find_LV_TimeCon(cal_inf.old_lv) < 0) {
        run_inf.LV_StopTime = 1;
        run_inf.LV_StartTime = 1;
        run_inf.JDQ_ROTATE_ID = JDQ_NUM_MAX;
        run_inf.PWM_ROTATE_ID = PWM_NUM_MAX;
        run_inf.time_on = STA_OFF;
    }
}
void temp_simple_set(void)
{
    int i = 0;

    if (ctrl_inf.extemp.temp_simple != STA_ON) {
        return ;
    }

    for (i = 0; i < TEMP_CURVE_NUM; i++) {
        ctrl_inf.temp[i].vertical_temp = ctrl_inf.temp[i].target_temp;
    }

}

void Update_Aim(void)
{
    int res, offset, i, temp_fix;

    temp_simple_set();
    cal_inf.exminair_lv = cal_ex_minair();//最小通风扩展控制
    Update_MinAir(0);
    res = Update_Aim_MinAir();
    cal_inf.min_air = EMAX(res, 0);
    get_hum_fix();
    CalDayTemp();


    //温度修正
    if (basic_inf.grow_day >= ctrl_inf.extemp.start_day && basic_inf.grow_day <= ctrl_inf.extemp.stop_day) {
        cal_inf.tar_hand_temp = ctrl_inf.temp_adj;
    } else {
        cal_inf.tar_hand_temp = 0;
    }

    temp_fix = cal_inf.tar_hand_temp + cal_inf.tar_hum_temp + cal_inf.tar_day_temp;



    offset = ((U32)&ctrl_inf.temp[0].heat_temp - (uint32_t)&ctrl_inf.temp[0].enable);
    res = Update_Aim_Temp(offset, STA_OFF);
    cal_inf.heat_cal_temp = EMAX(res, 0);
    cal_inf.heat_temp = cal_inf.heat_cal_temp + temp_fix;

    offset = ((U32)&ctrl_inf.temp[0].target_temp - (uint32_t)&ctrl_inf.temp[0].enable);
    res = Update_Aim_Temp(offset, STA_OFF);
    cal_inf.target_cal_temp = EMAX(res, 0);
    cal_inf.target_temp = cal_inf.target_cal_temp + temp_fix;

    offset = ((U32)&ctrl_inf.temp[0].vertical_temp - (uint32_t)&ctrl_inf.temp[0].enable);
    res = Update_Aim_Temp(offset, STA_OFF);
    cal_inf.fan_cal_temp = EMAX(res, 0);
    cal_inf.fan_temp = cal_inf.fan_cal_temp + temp_fix;

    offset = ((U32)&ctrl_inf.temp[0].min_temp - (uint32_t)&ctrl_inf.temp[0].enable);

    if (ctrl_inf.extemp.ala_diff_en == STA_ON) {
        res = Update_Aim_Temp(offset, STA_ON);
        cal_inf.ala_cal_Ltemp = EMAX(res, 0);
        cal_inf.ala_Ltemp = cal_inf.ala_cal_Ltemp;
        cal_inf.real_ala_Ltemp = abs(cal_inf.target_temp - cal_inf.ala_cal_Ltemp);
    } else {
        res = Update_Aim_Temp(offset, STA_OFF);
        cal_inf.ala_cal_Ltemp = EMAX(res, 0);
        cal_inf.ala_Ltemp = cal_inf.ala_cal_Ltemp + temp_fix;
        cal_inf.real_ala_Ltemp = cal_inf.ala_Ltemp;
    }

    offset = ((U32)&ctrl_inf.temp[0].max_temp - (uint32_t)&ctrl_inf.temp[0].enable);

    if (ctrl_inf.extemp.ala_diff_en == STA_ON) {
        res = Update_Aim_Temp(offset, STA_ON);
        cal_inf.ala_cal_Htemp = EMAX(res, 0);
        cal_inf.ala_Htemp = cal_inf.ala_cal_Htemp;
        cal_inf.real_ala_Htemp = cal_inf.target_temp + cal_inf.ala_cal_Htemp;
    } else {
        res = Update_Aim_Temp(offset, STA_OFF);
        cal_inf.ala_cal_Htemp = EMAX(res, 0);
        cal_inf.ala_Htemp = cal_inf.ala_cal_Htemp + temp_fix;
        cal_inf.real_ala_Htemp = cal_inf.ala_Htemp;
    }

    LV_Assist_Proc();
    Sencon_Proc();//传感器辅助级别控制

    if (sys_sta.sl_set.lv_en == STA_ON) {
        cal_inf.lv = sys_sta.sl_set.lv;
    } else {
        cal_inf.lv = LV_Proc();
    }

    if (cal_inf.old_lv != cal_inf.lv) {//更新级别后立刻重新开始计算

        if (cal_inf.old_lv != LV_DEF_VA) {//第一次不更新时间
            if (cal_inf.lv > cal_inf.old_lv) {
                run_inf.fan_delay = get_max_win_run_time(cal_inf.lv);
                run_inf.lv_win_delay = run_inf.fan_delay;
            } else {
                run_inf.lv_win_delay = get_max_win_run_time(cal_inf.lv);
            }
        }

        Lv_Time_Def();
    }

    cal_inf.old_lv = cal_inf.lv;
    VAC_Proc();//计算负压调节小窗的修正比例

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        cal_inf.HEAT_lv[i] = HEAT_Proc(i);

//        if (cal_inf.HEAT_lv[i] != (HEAT_NUM + 1)) {
//            cal_inf.old_HEAT_lv[i] = cal_inf.HEAT_lv[i];
//        }
    }

    cal_inf.PHEAT_lv = PHEAT_Proc();

    for (i = 0; i < WET_NUM_MAX; i++) {
        cal_inf.WET_lv[i] = WET_Proc(i);

//        if (cal_inf.old_WET_lv[i] != cal_inf.WET_lv[i] && cal_inf.WET_lv[i] < WET_NUM) { //更新级别且不是中间状态后立刻重新开始计算
//            run_inf.WET_Time[i] = 0;
//        }

//        if (cal_inf.WET_lv[i] != (WET_NUM + 1)) {
//            cal_inf.old_WET_lv[i] = cal_inf.WET_lv[i];
//        }
    }

    cal_inf.TEMPC_lv = TEMPC_Proc();
    cal_inf.TEMPC_pwm_lv = TEMPC_Pwm_Proc();


    cal_inf.SPRAY_lv = SPRAY_Proc();

//    if (cal_inf.old_SPRAY_lv != cal_inf.SPRAY_lv && cal_inf.SPRAY_lv < SPRAY_NUM) { //更新级别且不是中间状态后立刻重新开始计算
//        run_inf.SPRAY_Time = 0;
//    }

//    if (cal_inf.SPRAY_lv != (SPRAY_NUM + 1)) {
//        cal_inf.old_SPRAY_lv = cal_inf.SPRAY_lv;
//    }

    cal_inf.light_value = Update_Aim_Light();
    cal_inf.circle_sta = Update_Circle_Sta();

    for (i = 0; i < TR_TYPE_NUM; i++) {
        cal_inf.timer_sta[i] = Update_Timer_Sta(i);
    }
}


void DIN_ReadIn(void)
{
    int i = 0, id = 0;

    if (getbit(ext_inf.din.alarm, 7) != STA_ON) {
        sta_in.din.alarm = STA_OFF;
        return;
    }

    sta_in.din.alarm &= 0x0F;//高4位取0

    for (i = 0, id = 0; i < ALARM_NUM_MAX; i++) {
        if (getbit(ext_inf.din.alarm, i) != STA_ON) {
            clrbit(sta_in.din.alarm, i);
            continue;
        }

        if (Get_DIN_State(DIN_ALA + id) == STA_ON) {
            setbit(sta_in.din.alarm, i);
        } else {
            clrbit(sta_in.din.alarm, i);
        }

        id++;
    }

}

void Pulse_ReadIn(void)
{
    int pulse = 0, i = 0, ret = 0;

    for (i = 0; i < PULSE_WATER_MAX; i++) {
        if (ext_inf.din.pro_data[0].water_sta != WL_PULSE) {
            break;
        }

        if (i == 0) {
            ret = DIN_WATER;
        } else {
            ret = DIN_WATER2 + i - 1;
        }

        pulse = Get_Pluse_Value(ret);

        if (ext_inf.din.pro_data[i].water_sta == STA_ON && pulse > 0) {
            index_inf.water[i][PRO_DATA_ALL] += (pulse * ext_inf.din.pro_data[0].water_unit);
            Clear_Pluse_Value(ret);
        }
    }

    pulse = Get_Pluse_Value(DIN_FOOD);

    if (ext_inf.din.pro_data[0].food_sta == FL_PULSE && pulse > 0) {
        index_inf.food[0][PRO_DATA_ALL] += (pulse * ext_inf.din.pro_data[0].food_unit * (FOOD_RATE / 10));
        Clear_Pluse_Value(DIN_FOOD);
    }
}

void Food_ReadIn(void)
{
    static u8 sta = STA_OFF;

    if (ext_inf.din.pro_data[0].food_sta != FL_TIME) {
        sta = STA_OFF;
        run_inf.fl_timewait = 0;
        return;
    }

    if (sta == STA_ON) {
        sta = Get_DIN_State(DIN_FOOD);

//        if (sta != STA_OFF)//不需要等待全部打完
//            return;

        if (run_inf.fl_timewait > 180 || sta == STA_OFF) {//3分钟或者全部打完
            index_inf.food[0][PRO_DATA_ALL] += ((run_inf.fl_timewait - 1) * ext_inf.din.pro_data[0].food_unit * (FOOD_RATE * 1.0 / 600));
            Config_Save(SINDEX_ID);//记录运行时间

            if (sta == STA_OFF) {
                run_inf.fl_timewait = 0;
            } else {
                run_inf.fl_timewait = 1;
            }
        }

        return;
    }

    sta = Get_DIN_State(DIN_FOOD);

    if (sta == STA_ON) {
        run_inf.fl_timewait = 1;
    }
}

int Find_Next_JDQROTATE(u8 cur_id)
{
    u8 i = 0;

    if (cur_id > JDQ_NUM_MAX) {
        return -1;
    }

    for (i = (cur_id + 1); i < JDQ_NUM_MAX; i++) { //后面的
        if (dev_inf.JDQ[i].enable != STA_ON ||
                ctrl_inf.lv[cal_inf.lv].JDQ[i].mode != MODE_ROTATE) {
            continue;
        }

        return i;
    }

    for (i = 0; i < (cur_id + 1); i++) { //前面的
        if (dev_inf.JDQ[i].enable != STA_ON ||
                ctrl_inf.lv[cal_inf.lv].JDQ[i].mode != MODE_ROTATE) {
            continue;
        }

        return i;
    }

    return -2;
}


int Find_Next_PWMROTATE(u8 cur_id)
{
    u8 i = 0;

    if (cur_id > PWM_NUM_MAX) {
        return -1;
    }

    for (i = (cur_id + 1); i < PWM_NUM_MAX; i++) { //后面的
        if (dev_inf.PWM[i].enable != STA_ON ||
                ctrl_inf.lv[cal_inf.lv].PWM[i].mode != MODE_ROTATE) {
            continue;
        }

        return i;
    }

    for (i = 0; i < (cur_id + 1); i++) { //前面的
        if (dev_inf.PWM[i].enable != STA_ON ||
                ctrl_inf.lv[cal_inf.lv].PWM[i].mode != MODE_ROTATE) {
            continue;
        }

        return i;
    }

    return -2;
}

//更新运行条件
// 最短开关时间单独限制
void Update_JDQWork(void)
{
    u8 i = 0;

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        if (ctrl_inf.lv[cal_inf.lv].JDQ[i].mode == MODE_OFF || dev_inf.JDQ[i].enable != STA_ON) {
            run_inf.JDQ_TSTA[i] = STA_OFF;
            continue;
        }

        if (ctrl_inf.lv[cal_inf.lv].JDQ[i].mode == MODE_ON) {
            run_inf.JDQ_TSTA[i] = STA_ON;
            continue;
        }
    }
}


//更新运行条件
void Update_PWMWork(void)
{
    u8 i = 0, temp = 0;

    for (i = 0; i < PWM_NUM_MAX; i++) {
        if (ctrl_inf.lv[cal_inf.lv].PWM[i].mode == MODE_OFF || dev_inf.PWM[i].enable != STA_ON) {
            run_inf.PWM_Tdegree[i] = 0;
            continue;
        }

        if (ctrl_inf.lv[cal_inf.lv].PWM[i].mode == MODE_ON) {
            if (ctrl_inf.lv[cal_inf.lv].PWM[i].degree == 0) {
                temp = 0;
            } else {
                temp = EMAX(ctrl_inf.lv[cal_inf.lv].PWM[i].degree, (dev_inf.PWM[i].min_vol * 10));
            }

            run_inf.PWM_Tdegree[i] = temp;
//            run_inf.PWM_Tdegree[i] = EMIN(temp, (dev_inf.PWM[i].max_vol * 10));
            continue;
        }
    }
}

//级别切换等待，不是判断输出的开度是否到最大，而是判断记忆的开度是否为最大
static int GetVacOpenLvWait(u8 prior_ctrl)
{
    int i = 0;

    if (prior_ctrl ==  STA_ON) { //优先开启的不用等
        return STA_OFF;//不需要等待
    }

    for (i = 0; i < SWINDOW_NUM_MAX; i++) {
        if (get_swin_vac_control(i, cal_inf.lv) < 0) { //是否有导流板静压控制
            continue;
        }

        if (dev_inf.SWIN.detail[i].prior_ctrl != STA_ON) { //后动作的不计算
            continue;
        }

        if (ext_inf.lv_save[cal_inf.lv].SWIN[i] < 100) {//静压控制没有调到最大
            return STA_ON;//需要等待
        }
    }

    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (get_win_vac_control(i, cal_inf.lv) < 0) { //是否有导流板静压控制
            continue;
        }

        if (dev_inf.WIN.detail[i].prior_ctrl != STA_ON) { //后动作的不计算
            continue;
        }

        if (ext_inf.lv_save[cal_inf.lv].WIN[i] < 100) {//静压控制没有调到最大
            return STA_ON;//需要等待
        }
    }

    return STA_OFF;//不需要等待

}


int get_swin_degree(u8 id, u8 lv_id)
{
    u8 degree = 0;
    degree = ctrl_inf.lv[lv_id].SWIN[id].degree;

    if (get_swin_vac_control(id, lv_id) > 0) {
        if (ext_inf.lv_save[lv_id].SWIN[id] < degree) {
            if (sta_out.ALLPWM[PWM_SWIN1 + id] >= degree) {
                ext_inf.lv_save[lv_id].SWIN[id] = sta_out.ALLPWM[PWM_SWIN1 + id];
            } else {
                ext_inf.lv_save[lv_id].SWIN[id] = degree;
            }

            lv_save_data(lv_id);
        }

        if (GetVacOpenLvWait(dev_inf.SWIN.detail[id].prior_ctrl) >= STA_ON) { //高优先级的还没开完，需要等待
            ext_inf.lv_save[lv_id].SWIN[id] = degree;
        }

        return 	ext_inf.lv_save[lv_id].SWIN[id];
    }

    return degree;
}

int get_swin_run_time(u8 id, u8 lv_id)
{
    int time = 0;
    u8 degree = 0;

    if (id > SWINDOW_NUM_MAX || lv_id > CTRL_LV_NUM)
        return 0;

    if (dev_inf.SWIN.detail[id].enable != STA_ON)
        return 0;

    degree = get_swin_degree(id, lv_id);

    if (degree > sta_out.ALLPWM[id]) {
        time = (degree - sta_out.ALLPWM[id]) * dev_inf.SWIN.detail[id].open_time / 100;
    } else if (degree < sta_out.ALLPWM[id]) {
        time = (sta_out.ALLPWM[id] - degree) * dev_inf.SWIN.detail[id].close_time / 100;
    } else {
        time = 0;
    }

    return time;
}

int get_win_degree(u8 id, u8 lv_id)
{
    u8 degree = 0;

    if (id <= WIN2_ID) {
        degree = ctrl_inf.lv[lv_id].WIN[id].degree;
    } else {
        degree = ctrl_inf.exlv[lv_id].WIN[id - WIN3_ID].degree;
    }

    if (get_win_vac_control(id, lv_id) > 0) {
        if (ext_inf.lv_save[lv_id].WIN[id] < degree) {
            if (sta_out.WIN[id] >= degree) {
                ext_inf.lv_save[lv_id].WIN[id] = sta_out.WIN[id];
            } else {
                ext_inf.lv_save[lv_id].WIN[id] = degree;
            }

            lv_save_data(lv_id);
        }

        if (GetVacOpenLvWait(dev_inf.WIN.detail[id].prior_ctrl) >= STA_ON) { //高优先级的还没开完，需要等待
            ext_inf.lv_save[lv_id].WIN[id] = degree;
        }

        return 	ext_inf.lv_save[lv_id].WIN[id];
    }

    return degree;
}

int get_win_run_time(u8 id, u8 lv_id)
{
    int time = 0;
    u8 degree = 0;

    if (id > WINDOW_NUM_MAX || lv_id > CTRL_LV_NUM)
        return 0;

    if (dev_inf.WIN.detail[id].enable != STA_ON )
        return 0;

    if (dev_inf.WIN.detail[id].ctrl_mode == WIN_ONLY_VAC_CTRL) { //纯负压控制
        if (run_inf.WIN_SWflag[id] == STA_ON) {//纯负压控制时，启用启停模式，如果切换级别时变为开级别或关级别时间不够，重新计算时间
            if (run_inf.LV_StopTime <= ctrl_inf.lv[lv_id].PWM[0].off_time) {
                if (ctrl_inf.lv[lv_id].PWM[0].off_time - run_inf.LV_StopTime <= ctrl_inf.var.win_pretime + 2) {//如果剩下的时间不够提前开启时间，把提前开始时间预留进去
                    run_inf.LV_StartTime = 1;
                    run_inf.LV_StartLast = 1;
                    run_inf.LV_StopLast = ctrl_inf.var.win_pretime + 2;
                    run_inf.LV_StopTime = ctrl_inf.lv[lv_id].PWM[0].off_time - run_inf.LV_StopLast;
                    time = ctrl_inf.var.win_pretime + 2;
                }
            } else {
                if (run_inf.LV_StopLast > ctrl_inf.var.win_pretime + 2) {//上一级别还没到提前开启时间
                    run_inf.LV_StartTime = 1;
                    run_inf.LV_StartLast = 1;
                    run_inf.LV_StopLast = ctrl_inf.var.win_pretime + 2;
                    run_inf.LV_StopTime = ctrl_inf.lv[lv_id].PWM[0].off_time - run_inf.LV_StopLast;
                    time = ctrl_inf.var.win_pretime + 2;
                } else {//上一级别的提前开启已经动作了，先重新计算时间停止时间
                    run_inf.LV_StartTime = 1;
                    run_inf.LV_StartLast = 1;
                    run_inf.LV_StopTime = ctrl_inf.lv[lv_id].PWM[0].off_time - run_inf.LV_StopLast;
                    time = run_inf.LV_StopLast + 2;
                }
            }

            return time;
        } else {
            return 0;
        }
    }

    if (sta_out.WIN[id] == WIN_DEF_VA)//正在校准的不计算时间
        return 0;

    degree = get_win_degree(id, lv_id);

    if (degree > sta_out.WIN[id]) {
        time = (degree - sta_out.WIN[id]) * dev_inf.WIN.detail[id].open_time / 100 + 1;
    } else if (degree < sta_out.WIN[id]) {
        time = (sta_out.WIN[id] - degree) * dev_inf.WIN.detail[id].close_time / 100 + 1;
    } else {
        time = 0;
    }

    return time;
}

int get_max_win_run_time(u8 lv_id)
{
    int i = 0, max_time = 0, cur_time;

    for (i = 0; i < SWINDOW_NUM_MAX; i++) {
        if (dev_inf.SWIN.detail[i].enable != STA_ON)
            continue;

        cur_time = get_swin_run_time(i, lv_id);

        if (max_time < cur_time) {
            max_time = cur_time;
        }
    }

    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (dev_inf.WIN.detail[i].enable != STA_ON)
            continue;

        cur_time = get_win_run_time(i, lv_id);

        if (max_time < cur_time) {
            max_time = cur_time;
        }
    }

    return max_time;
}


int Check_FANROTATE_Sta(u8 lv, u8 jdq_id, u8 pwm_id)
{
    u8 i = 0;

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        if (dev_inf.JDQ[i].enable != STA_ON) {
            continue;
        }

        if (ctrl_inf.lv[lv].JDQ[i].mode != MODE_ROTATE) {
            continue;
        }

        //有轮转的设备,但是轮转号没有开启
        if (jdq_id >= JDQ_NUM_MAX) {
            return 1;
        }

        if (ctrl_inf.lv[lv].JDQ[jdq_id].mode != MODE_ROTATE || dev_inf.JDQ[jdq_id].enable != STA_ON) {
            return 2;
        }
    }

    for (i = 0; i < PWM_NUM_MAX; i++) {
        if (dev_inf.PWM[i].enable != STA_ON) {
            continue;
        }


        if (ctrl_inf.lv[lv].PWM[i].mode != MODE_ROTATE) {
            continue;
        }

        //有轮转的设备,但是轮转号没有开启
        if (pwm_id >= PWM_NUM_MAX) {
            return 1;
        }

        if (ctrl_inf.lv[lv].PWM[pwm_id].mode != MODE_ROTATE || dev_inf.PWM[pwm_id].enable != STA_ON) {
            return 3;
        }
    }

    return -1;
}

void Update_LVWork(void)
{
    u8 i = 0, temp = 0;
    u8 ret = 0, s = 0;
    int res;
    //先计算停止和持续运行的设备
    Update_JDQWork();
    Update_PWMWork();

    //再计算启停的设备
    //新的级别，先停止后开启
    if (run_inf.LV_StopTime <= ctrl_inf.lv[cal_inf.lv].PWM[0].off_time * RUN_FREQ) {

        for (i = 0; i < JDQ_NUM_MAX; i++) {
            if (dev_inf.JDQ[i].enable != STA_ON) {
                continue;
            }

            if (ctrl_inf.lv[cal_inf.lv].JDQ[i].mode == MODE_ON_OFF
                    || ctrl_inf.lv[cal_inf.lv].JDQ[i].mode == MODE_ROTATE) {
                run_inf.JDQ_TSTA[i] = STA_OFF;
                run_inf.time_on = STA_OFF;
                run_inf.LV_StartTime = 1;
                ret ++;
                continue;
            } else if (ctrl_inf.lv[cal_inf.lv].JDQ[i].mode == MODE_ON) {
                s ++;
            }
        }

        for (i = 0; i < PWM_NUM_MAX; i++) {
            if (dev_inf.PWM[i].enable != STA_ON) {
                continue;
            }

            if (ctrl_inf.lv[cal_inf.lv].PWM[i].mode == MODE_ON_OFF
                    || ctrl_inf.lv[cal_inf.lv].PWM[i].mode == MODE_ROTATE) {
                run_inf.PWM_Tdegree[i] = 0;
                run_inf.time_on = STA_OFF;
                run_inf.LV_StartTime = 1;
                ret ++;
                continue;
            } else if (ctrl_inf.lv[cal_inf.lv].PWM[i].mode == MODE_ON) {
                s ++;
            }
        }

        if (ret > 0) {
            run_inf.LV_StopLast = ctrl_inf.lv[cal_inf.lv].PWM[0].off_time * RUN_FREQ - run_inf.LV_StopTime;
            run_inf.LV_StartLast = 1;
        } else {
            run_inf.LV_StopLast = -1;
        }

        //更新小窗
        //小窗风窗风机时控下开启模式，再停止阶段就先开启小窗
        for (i = 0; i < SWINDOW_NUM_MAX; i++) {
            if (dev_inf.SWIN.detail[i].enable != STA_ON) {
                continue;
            }

            if (ctrl_inf.var.win_link != STA_ON || (ret == 0 || s != 0)
                    || cal_inf.lv >= ctrl_inf.var.win_link_lv) {//小窗随动
                run_inf.SWIN_Tdegree[i] = get_swin_degree(i, cal_inf.lv);
                run_inf.SWIN_SWflag[i] = STA_OFF;
                continue;
            }

            //计算过一次时间后开始动作就不再计算，电位计反馈会比时间快导致中间停顿
            if (run_inf.SWIN_SWflag[i] == STA_OFF && run_inf.LV_StopLast > get_swin_run_time(i, cal_inf.lv)) { //关闭剩余时间如果大于先开的延时时间就关闭或者打开
                run_inf.SWIN_Tdegree[i] = 0;
                run_inf.SWIN_SWflag[i] = STA_OFF;
            } else {
                run_inf.SWIN_Tdegree[i] = get_swin_degree(i, cal_inf.lv);
                run_inf.SWIN_SWflag[i] = STA_ON;
            }

        }

        //更新风窗
        for (i = 0; i < WINDOW_NUM_MAX; i++) {
            if (dev_inf.WIN.detail[i].enable != STA_ON) {
                continue;
            }

            if (ctrl_inf.var.win_link != STA_ON || (ret == 0 || s != 0)
                    || cal_inf.lv >= ctrl_inf.var.win_link_lv) {
                run_inf.WIN_LvDegree[i] = get_win_degree(i, cal_inf.lv);
                run_inf.WIN_SWflag[i] = STA_OFF;
                continue;
            }

            if (dev_inf.WIN.detail[i].ctrl_mode == WIN_ONLY_VAC_CTRL) { //纯负压控制的，没有风机就关，有风机的时候提前开多少秒
                if (get_win_only_vac_control(i, cal_inf.lv) <= 0) {
                    continue;
                }

                run_inf.WIN_SWflag[i] = STA_ON;//纯负压控制，根据风机时控自动开关状态

                if (run_inf.LV_StopLast > ctrl_inf.var.win_pretime + 2) { //关闭剩余时间如果大于先开的延时时间就关闭或者打开
                    WIN_UP_CON(i, STA_OFF);
                    WIN_DOWN_CON(i, STA_ON); //一直关窗
                } else {
                    if (run_inf.LV_StopLast <= 2) {
                        WIN_DOWN_CON(i, STA_OFF);
                        WIN_UP_CON(i, STA_OFF);
                    } else {
                        WIN_DOWN_CON(i, STA_OFF);
                        WIN_UP_CON(i, STA_ON); //提前开窗
                    }

                }

                continue;
            }

            //计算过一次时间后开始动作就不再计算，电位计反馈会比时间快导致中间停顿
            if (run_inf.WIN_SWflag[i] == STA_OFF && run_inf.LV_StopLast > get_win_run_time(i, cal_inf.lv)) { //关闭剩余时间如果大于先开的延时时间就关闭或者打开
                run_inf.WIN_LvDegree[i] = 0;
                run_inf.WIN_SWflag[i] = STA_OFF;
            } else {
                run_inf.WIN_LvDegree[i] = get_win_degree(i, cal_inf.lv);
                run_inf.WIN_SWflag[i] = STA_ON;
            }
        }

    } else if (run_inf.LV_StartTime <= ctrl_inf.lv[cal_inf.lv].PWM[0].on_time * RUN_FREQ) {


        if (run_inf.time_on == STA_OFF || Check_FANROTATE_Sta(cal_inf.lv, run_inf.JDQ_ROTATE_ID, run_inf.PWM_ROTATE_ID) > 0) { //需要切换风机 或者更新级别后原风机没有运行了
            res = Find_Next_JDQROTATE(run_inf.JDQ_ROTATE_ID);

            if (res >= 0)
                run_inf.JDQ_ROTATE_ID = res;

            res = Find_Next_PWMROTATE(run_inf.PWM_ROTATE_ID);

            if (res >= 0)
                run_inf.PWM_ROTATE_ID = res;
        }

        for (i = 0; i < JDQ_NUM_MAX; i++) {
            if (dev_inf.JDQ[i].enable != STA_ON) {
                continue;
            }

            if (ctrl_inf.lv[cal_inf.lv].JDQ[i].mode == MODE_ON_OFF) {
                run_inf.JDQ_TSTA[i] = STA_ON;
                run_inf.time_on = STA_ON;
                run_inf.LV_StopTime = TIME_4BIT + 1;
                ret ++;
                continue;
            }

            if (ctrl_inf.lv[cal_inf.lv].JDQ[i].mode == MODE_ROTATE) {
                if (run_inf.JDQ_ROTATE_ID == i) {
                    run_inf.JDQ_TSTA[i] = STA_ON;
                } else {
                    run_inf.JDQ_TSTA[i] = STA_OFF;
                }

                run_inf.time_on = STA_ON;
                run_inf.LV_StopTime = TIME_4BIT + 1;
                ret ++;
                continue;
            }

            if (ctrl_inf.lv[cal_inf.lv].JDQ[i].mode == MODE_ON) {
                s ++;
            }

        }

        for (i = 0; i < PWM_NUM_MAX; i++) {
            if (dev_inf.PWM[i].enable != STA_ON) {
                continue;
            }

            if (ctrl_inf.lv[cal_inf.lv].PWM[i].mode == MODE_ON_OFF) {
                if (ctrl_inf.lv[cal_inf.lv].PWM[i].degree == 0) {
                    temp = 0;
                } else {
                    temp = EMAX(ctrl_inf.lv[cal_inf.lv].PWM[i].degree, (dev_inf.PWM[i].min_vol * 10));
                }

                run_inf.PWM_Tdegree[i] = temp;
                run_inf.time_on = STA_ON;
                run_inf.LV_StopTime = TIME_4BIT + 1;
                ret ++;
                continue;
            }

            if (ctrl_inf.lv[cal_inf.lv].PWM[i].mode == MODE_ROTATE) {
                if (run_inf.PWM_ROTATE_ID == i) {
                    if (ctrl_inf.lv[cal_inf.lv].PWM[i].degree == 0) {
                        temp = 0;
                    } else {
                        temp = EMAX(ctrl_inf.lv[cal_inf.lv].PWM[i].degree, (dev_inf.PWM[i].min_vol * 10));
                    }

                    run_inf.PWM_Tdegree[i] = temp;
                } else {
                    run_inf.PWM_Tdegree[i] = 0;
                }

                run_inf.time_on = STA_ON;
                run_inf.LV_StopTime = TIME_4BIT + 1;
                ret ++;
                continue;
            }

            if (ctrl_inf.lv[cal_inf.lv].PWM[i].mode == MODE_ON) {
                s ++;
            }
        }

        if (ret > 0) {
            run_inf.LV_StartLast = ctrl_inf.lv[cal_inf.lv].PWM[0].on_time * RUN_FREQ - run_inf.LV_StartTime;
            run_inf.LV_StopLast = 1;
        } else {
            run_inf.LV_StartLast = -1;
        }

        //更新小窗
        for (i = 0; i < SWINDOW_NUM_MAX; i++) {
            if (dev_inf.SWIN.detail[i].enable != STA_ON) {
                run_inf.SWIN_SWflag[i] = STA_OFF;
                continue;
            }

            run_inf.SWIN_Tdegree[i] = get_swin_degree(i, cal_inf.lv);
            run_inf.SWIN_SWflag[i] = STA_OFF;
        }

        //更新风窗
        for (i = 0; i < WINDOW_NUM_MAX; i++) {
            if (dev_inf.WIN.detail[i].enable != STA_ON) {
                run_inf.WIN_SWflag[i] = STA_OFF;
                continue;
            }

            run_inf.WIN_LvDegree[i] = get_win_degree(i, cal_inf.lv);
            run_inf.WIN_SWflag[i] = STA_OFF;
        }

    } else {
        run_inf.LV_StopTime = 1;
        run_inf.LV_StartTime = 1;
        run_inf.time_on = STA_OFF;
    }

}
//更新运行条件
void Update_HEATWork(void)
{
    u8 i = 0;

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        if (cal_inf.HEAT_lv[i] < 0) {    //关闭
            run_inf.HEAT_Time[i] = TIME_4BIT;
            run_inf.HEAT_Topen[i] = 0;
            run_inf.HEAT_Tclose[i] = HEAT_RUN_MIN * RUN_FREQ;
        } else {  //开启启停模式
            if (basic_inf.tempc_en.mode != TEMPC_MODE_HIGH && basic_inf.tempc_en.start_temp != 0
                    && cal_inf.TEMPC_lv >= -1 && cal_inf.tempc_temp < basic_inf.tempc_en.start_temp) {//温度启用正常，温度小于启动温度
                run_inf.HEAT_Time[i] = TIME_4BIT;
                run_inf.HEAT_Topen[i] = 0;
                run_inf.HEAT_Tclose[i] = HEAT_RUN_MIN * RUN_FREQ;
                continue;
            }

            if (basic_inf.tempc_pwm_en.mode != TEMPC_MODE_HIGH && basic_inf.tempc_pwm_en.start_temp != 0
                    && cal_inf.TEMPC_pwm_lv >= -1 && cal_inf.tempc_pwm_temp < basic_inf.tempc_pwm_en.start_temp) {//温度启用正常，温度小于启动温度
                run_inf.HEAT_Time[i] = TIME_4BIT;
                run_inf.HEAT_Topen[i] = 0;
                run_inf.HEAT_Tclose[i] = HEAT_RUN_MIN * RUN_FREQ;
                continue;
            }

            run_inf.HEAT_Topen[i] = basic_inf.heat[i][cal_inf.HEAT_lv[i]].start_sec * RUN_FREQ;
            run_inf.HEAT_Tclose[i] = basic_inf.heat[i][cal_inf.HEAT_lv[i]].stop_sec * RUN_FREQ;
        }
    }

}

//更新运行条件
void Update_WETWork(void)
{
    u8 i = 0;

    for (i = 0; i < WET_NUM_MAX; i++) {
        if (cal_inf.WET_lv[i] < 0) {    //关闭
            run_inf.WET_Topen[i] = 0;
            if (run_inf.WET_Time[i] >= run_inf.WET_Tclose[i]) { //停止后运行一次关周期再开启下一次
                run_inf.WET_Tclose[i] = WET_RUN_MIN * RUN_FREQ;
            }
        } else {  //开启启停模式
            run_inf.WET_Topen[i] = dev_inf.wet[i][cal_inf.WET_lv[i]].start_sec * RUN_FREQ;
            run_inf.WET_Tclose[i] = dev_inf.wet[i][cal_inf.WET_lv[i]].stop_sec * RUN_FREQ;
        }
    }
}

//更新运行条件
void Update_TEMPCWork(void)
{
    if (cal_inf.TEMPC_lv < 0) {    //关闭
        run_inf.TEMPC_Time = TIME_4BIT;
        run_inf.TEMPC_Topen = 0;
        run_inf.TEMPC_Tclose = TEMPC_RUN_MIN * RUN_FREQ;
    } else {  //开启启停模式
        run_inf.TEMPC_Topen = basic_inf.tempc[cal_inf.TEMPC_lv].start_sec * RUN_FREQ;
        run_inf.TEMPC_Tclose = basic_inf.tempc[cal_inf.TEMPC_lv].stop_sec * RUN_FREQ;
    }
}

//更新运行条件
void Update_SPRAYWork(void)
{
    if (cal_inf.SPRAY_lv < 0) {    //关闭
        run_inf.SPRAY_Topen = 0;
        if (run_inf.SPRAY_Time >= run_inf.SPRAY_Tclose) { //停止后运行一次关周期再开启下一次
            run_inf.SPRAY_Tclose = SPRAY_RUN_MIN * RUN_FREQ;
        }
    } else {  //开启启停模式
        run_inf.SPRAY_Topen = dev_inf.spray[cal_inf.SPRAY_lv].start_sec * RUN_FREQ;
        run_inf.SPRAY_Tclose = dev_inf.spray[cal_inf.SPRAY_lv].stop_sec * RUN_FREQ;
    }
}

//更新运行条件
void Update_LightWork(void)
{
    if (run_inf.light_Time != 0) {
        return;
    }

    if (cal_inf.light_value < 0 && sta_out.ALLPWM[PWM_LIGHT] != 0) {
        run_inf.light_Time = dev_inf.light.delay_offtime * RUN_FREQ;
        run_inf.light_Value = 0;
        run_inf.light_Scale = (float)((sta_out.ALLPWM[PWM_LIGHT] - run_inf.light_Value) * 1.0f / run_inf.light_Time);
    } else if (cal_inf.light_value >= 0 && sta_out.ALLPWM[PWM_LIGHT] != cal_inf.light_value) {
        run_inf.light_Time = dev_inf.light.delay_ontime * RUN_FREQ;
        run_inf.light_Value = cal_inf.light_value;
        run_inf.light_Scale = (float)((sta_out.ALLPWM[PWM_LIGHT] - run_inf.light_Value) * 1.0f / run_inf.light_Time);
    }
}

void Update_APwm_Work(void)
{
    int i = 0, res = 0;

    for (i = 0; i < APWM_NUM_MAX; i++) {
        res = Update_APWM_Freq(i);

        if (res <= 0) {//等于0也直接关闭
            run_inf.APWM_Tdegree[i] = 0;
            continue;
        }

        if (res < basic_inf.area_pwm[i].min_vol * 10) {
            run_inf.APWM_Tdegree[i] = basic_inf.area_pwm[i].min_vol * 10;
            continue;
        }

        run_inf.APWM_Tdegree[i] = res;
    }
}

void Update_Work(void)
{
    Update_LVWork();
    Update_HEATWork();
    Update_WETWork();
    Update_TEMPCWork();
    Update_SPRAYWork();
    Update_LightWork();
    Update_APwm_Work();
}

int JDQ_Fan_CON(u8 id , u8 status)
{
    if (id > JDQ_ALA || status > STA_ON)
        return -1;

    if (status == STA_ON) { //刚打开过一个风机需要等待一段时间才能继续打开
        run_inf.JDQ_IDE = ctrl_inf.var.fan_delay * RUN_FREQ;
    }

    JDQ_Control(id, status);
    return 1;
}

int PWM_Fan_CON(u8 id , u8 status)
{
    if (id > PWM_NUM_MAX || status > MAX_PERCENT)
        return -1;

    PWM_Out(id, status);
    return 1;
}

static u8 win_degree_er = 0;
#define WIN_ER_MAX 	(300)
void win_pos_er(u8 i)
{
    sta_out.WIN[i] = WIN_DEF_VA;
//    run_inf.WIN_RunMode[i] = WIN_RUN_STOP;
//    WIN_UP_CON(i, STA_OFF);
//    WIN_DOWN_CON(i, STA_OFF);
//    run_inf.WIN_First[i] = STA_ON;
//    dev_inf.WIN.detail[i].mode = WIN_MODE_TIME;
//    Config_Save(SWIN_ID);
}

int WIN_Get_Pos(u8 i)
{
    int degree = 0;

    //增加一个判断 如果没启用就不控制
    if (dev_inf.WIN.detail[i].enable != STA_ON || dev_inf.WIN.detail[i].mode != WIN_MODE_POS) {
        return -1;
    }

    if (dev_inf.WIN.detail[i].open_v == dev_inf.WIN.detail[i].close_v) {//参数有问题，可以改为时间控制
        win_pos_er(i);
        return -2;
    }

    //实际开度
    degree = (sta_in.sensor_adj.WIN[i] - dev_inf.WIN.detail[i].close_v) * MAX_DEGREE / (dev_inf.WIN.detail[i].open_v - dev_inf.WIN.detail[i].close_v);

    // 考虑错误情况
    if (degree >= 0 && degree <= 100) { //正常值
        win_degree_er = 0;
    } else if (degree < 0) {
        if (degree >= -3) {
            win_degree_er = 0;
        } else {
            win_degree_er ++;
        }

        degree = 0;
    } else if (degree > 100) {
        if (degree <= 103) {
            win_degree_er = 0;
        } else {
            win_degree_er ++;
        }

        degree = 100;
    }

    if (win_degree_er >= WIN_ER_MAX) {//电位计有问题 可以改为时间控制 //同时选择报警
        ALA_Set_State(ALA_WINPOS, ALA_STA_ON);
        win_pos_er(i);
        return -3;
    }

    return degree;
}


static int GetVacOpenWait(u8 prior_ctrl)
{
    int i = 0;

    if (prior_ctrl ==  STA_ON) { //优先开启的不用等
        return STA_OFF;//不需要等待
    }

    for (i = 0; i < SWINDOW_NUM_MAX; i++) {
        if (get_swin_vac_control(i, cal_inf.lv) < 0) { //是否有导流板静压控制
            continue;
        }

        if (dev_inf.SWIN.detail[i].prior_ctrl != STA_ON) { //后动作的不计算
            continue;
        }

        if (sta_out.ALLPWM[PWM_SWIN1 + i] < 100) {//静压控制没有调到最大
            return STA_ON;//需要等待
        }
    }

    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (get_win_vac_control(i, cal_inf.lv) < 0) { //是否有导流板静压控制
            continue;
        }

        if (dev_inf.WIN.detail[i].prior_ctrl != STA_ON) { //后动作的不计算
            continue;
        }

        if (sta_out.WIN[i] < 100) {//静压控制没有调到最大
            return STA_ON;//需要等待
        }
    }

    return STA_OFF;//不需要等待

}


int GetWinVacWait(u8 id)
{
    int res;

    if (ctrl_inf.vac.adj_move_s == 0 || ctrl_inf.vac.adj_delay_s == 0 || GetVacAdjustQuick() > 0) {
        run_inf.WIN_VACTime[id] = 0;
        return -1;//不需要等待
    }

    if (run_inf.WIN_VACTime[id] == 0) {
        run_inf.WIN_VACTime[id] = (ctrl_inf.vac.adj_move_s + ctrl_inf.vac.adj_delay_s) * WIN_RUN_FREQ;
    }

    if (run_inf.WIN_VACTime[id] > ctrl_inf.vac.adj_delay_s * WIN_RUN_FREQ) {
        res = -1;
    } else {
        res = 1;
    }

    run_inf.WIN_VACTime[id]--;
    return res;
}

static int GetVacCloseWait(u8 prior_ctrl)
{
    int i = 0;
    u8 degree = 0;

    if (prior_ctrl != STA_ON) { //没有优先动作的不用等待
        return STA_OFF;//不需要等待
    }

    for (i = 0; i < SWINDOW_NUM_MAX; i++) {
        if (get_swin_vac_control(i, cal_inf.lv) < 0) { //是否有导流板静压控制
            continue;
        }

        if (dev_inf.SWIN.detail[i].prior_ctrl == STA_ON) { //优先动作的不计算
            continue;
        }

        degree = ctrl_inf.lv[cal_inf.lv].SWIN[i].degree;

        if (sta_out.ALLPWM[PWM_SWIN1 + i] > degree) {//静压控制没有调到最小
            return STA_ON;//需要等待
        }
    }

    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (get_win_vac_control(i, cal_inf.lv) < 0) { //是否有导流板静压控制
            continue;
        }

        if (dev_inf.WIN.detail[i].prior_ctrl ==  STA_ON) { //优先动作的不计算
            continue;
        }


        if (i <= WIN2_ID) {
            degree = ctrl_inf.lv[cal_inf.lv].WIN[i].degree;
        } else {
            degree = ctrl_inf.exlv[cal_inf.lv].WIN[i - WIN3_ID].degree;
        }

        if (sta_out.WIN[i] > degree) {//静压控制没有调到最小
            return STA_ON;//需要等待
        }
    }

    return STA_OFF;//不需要等待
}

void Win_Time_Proc(void)
{
    u8 i = 0;
    u8 degree = 0;
    unsigned short time = 0;

    if (sys_sta.auto_mode == STA_OFF || get_win_adjust_sta() == STA_ON) {
        return;
    }

    // 位置控制,计算错误时间
    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (dev_inf.WIN.detail[i].enable != STA_ON || dev_inf.WIN.detail[i].ctrl_mode == WIN_ONLY_VAC_CTRL) {
            run_inf.WIN_Time[i] = 0;
            run_inf.WIN_CalMode[i] = ADJ_READY;
            run_inf.WIN_ErTime[i] = 0;
            run_inf.WIN_RunMode[i] = WIN_RUN_STOP;
            continue ;
        }

        if (dev_inf.WIN.detail[i].mode != WIN_MODE_POS) {
            continue ;
        }

        run_inf.WIN_Time[i] = 0;
        run_inf.WIN_CalMode[i] = ADJ_READY;

        if (run_inf.WIN_ErTime[i] != 0) {
            run_inf.WIN_ErTime[i] ++;
        }

        //负压运行中
        if (get_win_vac_control(i, cal_inf.lv) > 0 && run_inf.VAC_ctrl_flag != VAC_CTRL_NONE) {
            if (run_inf.VAC_ctrl_flag == VAC_CTRL_BIG) {
                if (GetVacOpenWait(dev_inf.WIN.detail[i].prior_ctrl) >= STA_ON) { //需要等待调整完
                    continue;
                }

                if (sta_out.WIN[i] >= 99) {//调到99%就不再调了，防止电位计跳动反复开
                    sta_out.WIN[i] = 100;
                    ext_inf.lv_save[cal_inf.lv].WIN[i] = 100;
                    WIN_UP_CON(i, STA_OFF);
                    WIN_DOWN_CON(i, STA_OFF);
                    continue;//需要负压调节
                }

                if (GetWinVacWait(i) > 0) { //静压调节按时间启动
                    WIN_UP_CON(i, STA_OFF);
                    WIN_DOWN_CON(i, STA_OFF);
                    continue;
                }

                WIN_DOWN_CON(i, STA_OFF);
                WIN_UP_CON(i, STA_ON);
            } else if (run_inf.VAC_ctrl_flag == VAC_CTRL_SAMLL) {
                if (GetVacCloseWait(dev_inf.WIN.detail[i].prior_ctrl) >= STA_ON) { //需要等待调整完
                    continue;
                }

                if (i <= WIN2_ID) {
                    degree = ctrl_inf.lv[cal_inf.lv].WIN[i].degree;
                } else {
                    degree = ctrl_inf.exlv[cal_inf.lv].WIN[i - WIN3_ID].degree;
                }

                if (sta_out.WIN[i] <= degree + 1) {
                    sta_out.WIN[i] = degree;
                    ext_inf.lv_save[cal_inf.lv].WIN[i] = degree;
                    WIN_UP_CON(i, STA_OFF);
                    WIN_DOWN_CON(i, STA_OFF);
                    continue;//需要负压调节
                }

                if (GetWinVacWait(i) > 0) { //静压调节按时间启动
                    WIN_UP_CON(i, STA_OFF);
                    WIN_DOWN_CON(i, STA_OFF);
                    continue;
                }

                WIN_UP_CON(i, STA_OFF);
                WIN_DOWN_CON(i, STA_ON);
            }

            continue;//需要负压调节
        }
    }

    // 时间控制，计算校准和关闭时间
    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (dev_inf.WIN.detail[i].enable != STA_ON || dev_inf.WIN.detail[i].ctrl_mode == WIN_ONLY_VAC_CTRL) {
            continue ;
        }

        if (dev_inf.WIN.detail[i].mode != WIN_MODE_TIME) {
            continue ;
        }

        run_inf.WIN_ErTime[i] = 0;
        run_inf.WIN_RunMode[i] = WIN_RUN_STOP;

        if (run_inf.WIN_CalMode[i] != ADJ_READY) { //校准中
            switch (run_inf.WIN_CalMode[i]) {
            case ADJ_UP://开校准
            case ADJ_DOWN://关校准
                if (run_inf.WIN_Time[i] != 0) {
                    run_inf.WIN_Time[i]--;

                    if (run_inf.WIN_Time[i] == 0) {
                        if (run_inf.WIN_CalMode[i] == ADJ_UP) {
                            sta_out.WIN[i] = 100;
                        } else {
                            sta_out.WIN[i] = 0;
                        }

                        run_inf.WIN_CalMode[i] = ADJ_READY;
                        WIN_UP_CON(i, STA_OFF);
                        WIN_DOWN_CON(i, STA_OFF);
                        ADJ_ENFAN(i);
                        run_inf.WIN_Num[i] = 0;
                        sta_out.WIN_F[i] = sta_out.WIN[i];
                        //算一下调整到需要的位置要多久时间，在这个时间内负压不调节
                        time = get_win_run_time(i, cal_inf.lv);

                        if (run_inf.fan_delay < time) {
                            run_inf.fan_delay = time;//校准完风机延时开
                        }

                        if (run_inf.lv_win_delay < time) {
                            run_inf.lv_win_delay = time;
                        }
                    }
                }

                break;

            default:
                run_inf.WIN_CalMode[i] = ADJ_READY;
                break;

            }

            continue;
        }

        //运行中
        if (get_win_vac_control(i, cal_inf.lv) > 0 && run_inf.VAC_ctrl_flag != VAC_CTRL_NONE) {
            if (run_inf.VAC_ctrl_flag == VAC_CTRL_BIG) {
                if (GetVacOpenWait(dev_inf.WIN.detail[i].prior_ctrl) >= STA_ON) { //需要等待调整完
                    continue;
                }

                sta_out.WIN_F[i] += 100.0 / (dev_inf.WIN.detail[i].open_time * WIN_RUN_FREQ);

                if (sta_out.WIN_F[i] >= 100) {
                    sta_out.WIN_F[i] = 100;
                    sta_out.WIN[i] = (unsigned short)sta_out.WIN_F[i];
                    ext_inf.lv_save[cal_inf.lv].WIN[i] = 100;
                    WIN_UP_CON(i, STA_OFF);
                    WIN_DOWN_CON(i, STA_OFF);
                    continue;//需要负压调节
                }

                sta_out.WIN[i] = (unsigned short)sta_out.WIN_F[i];

                WIN_DOWN_CON(i, STA_OFF);
                WIN_UP_CON(i, STA_ON);
            } else if (run_inf.VAC_ctrl_flag == VAC_CTRL_SAMLL) {
                if (GetVacCloseWait(dev_inf.WIN.detail[i].prior_ctrl) >= STA_ON) { //需要等待调整完
                    continue;
                }

                sta_out.WIN_F[i] -= 100.0 / (dev_inf.WIN.detail[i].close_time * WIN_RUN_FREQ);

                if (i <= WIN2_ID) {
                    degree = ctrl_inf.lv[cal_inf.lv].WIN[i].degree;
                } else {
                    degree = ctrl_inf.exlv[cal_inf.lv].WIN[i - WIN3_ID].degree;
                }

                if (sta_out.WIN_F[i] <= degree) {
                    sta_out.WIN_F[i] = degree;
                    sta_out.WIN[i] = (unsigned short)sta_out.WIN_F[i];
                    ext_inf.lv_save[cal_inf.lv].WIN[i] = degree;
                    WIN_UP_CON(i, STA_OFF);
                    WIN_DOWN_CON(i, STA_OFF);
                    continue;//需要负压调节
                }

                sta_out.WIN[i] = (unsigned short)sta_out.WIN_F[i];
                WIN_UP_CON(i, STA_OFF);
                WIN_DOWN_CON(i, STA_ON);
            }

            continue;//需要负压调节
        }

        if (get_win_vac_control(i, cal_inf.lv) > 0 && run_inf.VAC_hold_time != 0) {
            continue;//等待保存
        }

        if (run_inf.WIN_Time[i] != 0) {
            run_inf.WIN_Time[i]--;

            if (run_inf.WIN_Degree[i] > sta_out.WIN[i]) {
                sta_out.WIN_F[i] += 100.0 / (dev_inf.WIN.detail[i].open_time * WIN_RUN_FREQ);

                if (sta_out.WIN_F[i] > run_inf.WIN_Degree[i]) {
                    sta_out.WIN_F[i] = run_inf.WIN_Degree[i];
                }

                sta_out.WIN[i] = (unsigned short)sta_out.WIN_F[i];
            } else if (run_inf.WIN_Degree[i] < sta_out.WIN[i]) {
                sta_out.WIN_F[i] -= 100.0 / (dev_inf.WIN.detail[i].close_time * WIN_RUN_FREQ);

                if (sta_out.WIN_F[i] < run_inf.WIN_Degree[i]) {
                    sta_out.WIN_F[i] = run_inf.WIN_Degree[i];
                }

                sta_out.WIN[i] = (unsigned short)sta_out.WIN_F[i];
            }

            if (run_inf.WIN_Time[i] == 0) {
                WIN_UP_CON(i, STA_OFF);
                WIN_DOWN_CON(i, STA_OFF);
                sta_out.WIN[i] = run_inf.WIN_Degree[i];
                sta_out.WIN_F[i] = sta_out.WIN[i];
                continue ;
            }

        }
    }
}

static void Net_Time_Proc(void)
{
    if (run_inf.NetSta_Time != 0) {
        run_inf.NetSta_Time--;
    } else {
        sys_sta.net_sta = STA_OFF;
    }
}

int win_defrost_en(unsigned char id)
{
    if (id >= (WINDOW_NUM_MAX + SWINDOW_NUM_MAX))
        return -1;

    if (ctrl_inf.var.swin_defrost_en[id] != STA_ON || ctrl_inf.var.swin_hold_time == 0
            || ctrl_inf.var.swin_defrost_degree == 0)
        return -2;

    return 1;
}
void win_pos_check(void)
{
    int i = 0;
    u8 jdq_id = 0;

    if (sys_sta.auto_mode == STA_OFF || get_win_adjust_sta() == STA_ON) {
        return;
    }

    for (i = WIN3_ID; i < WINDOW_NUM_MAX; i++) {
        //增加一个判断 如果没启用就不控制
        if (dev_inf.WIN.detail[i].enable != STA_ON || dev_inf.WIN.detail[i].mode != WIN_MODE_POS || dev_inf.WIN.detail[i].ctrl_mode == WIN_ONLY_VAC_CTRL) {
            continue ;
        }

        jdq_id = JDQ_WIN3_UP + (i - WIN3_ID) * 2;//开启

        if (sta_out.ALLJDQ[jdq_id] == STA_ON) {
            if (run_inf.WIN_PosState[i] == WIN_RUN_STOP) {
                run_inf.WIN_PosTime[i] = 0;
                run_inf.WIN_PosState[i] = WIN_RUN_UP;
                run_inf.WIN_PosDegree[i] = sta_out.WIN[i];
            }

            run_inf.WIN_PosTime[i] ++;

// 动作10秒以上，开度没有变化3%
            if (run_inf.WIN_PosTime[i] > 10 && abs(run_inf.WIN_PosDegree[i] - sta_out.WIN[i]) < 3) { //报警 切换模式
                run_inf.WIN_PosTime[i] = 0;
                ALA_Set_State(ALA_WINPOS, ALA_STA_ON);
                win_pos_er(i);
            }

            continue ;
        } else {
            if (run_inf.WIN_PosState[i] == WIN_RUN_UP) {
                run_inf.WIN_PosState[i] = WIN_RUN_STOP;
            }
        }

        jdq_id = JDQ_WIN3_DOWN + (i - WIN3_ID) * 2;//关闭

        if (sta_out.ALLJDQ[jdq_id] == STA_ON) {
            if (run_inf.WIN_PosState[i] == WIN_RUN_STOP) {
                run_inf.WIN_PosTime[i] = 0;
                run_inf.WIN_PosState[i] = WIN_RUN_DOWN;
                run_inf.WIN_PosDegree[i] = sta_out.WIN[i];
            }

            run_inf.WIN_PosTime[i] ++;

// 动作10秒以上，开度没有变化3%
            if (run_inf.WIN_PosTime[i] > 10 && abs(run_inf.WIN_PosDegree[i] - sta_out.WIN[i]) < 3) { //报警 切换模式
                run_inf.WIN_PosTime[i] = 0;
                ALA_Set_State(ALA_WINPOS, ALA_STA_ON);
                win_pos_er(i);
            }

            continue ;
        } else {
            if (run_inf.WIN_PosState[i] == WIN_RUN_DOWN) {
                run_inf.WIN_PosState[i] = WIN_RUN_STOP;
            }
        }

    }

}


int GetSwinVacWait(u8 id)
{
    int res;

    if (ctrl_inf.vac.adj_move_s == 0 || ctrl_inf.vac.adj_delay_s == 0 || GetVacAdjustQuick() > 0) {
        run_inf.SWIN_VACTime[id] = 0;
        return -1;//不需要等待
    }

    if (run_inf.SWIN_VACTime[id] == 0) {
        run_inf.SWIN_VACTime[id] = ctrl_inf.vac.adj_move_s + ctrl_inf.vac.adj_delay_s;
    }

    if (run_inf.SWIN_VACTime[id] > ctrl_inf.vac.adj_delay_s) {
        res = -1;
    } else {
        res = 1;
    }

    run_inf.SWIN_VACTime[id]--;
    return res;
}

void Run_Time_Proc(void)
{
    u8 i = 0;

    run_inf.LV_StopTime++;

    if (run_inf.time_on == STA_ON) {
        run_inf.LV_StartTime ++;
    }

    if (run_inf.JDQ_IDE != 0) {
        run_inf.JDQ_IDE --;
    }

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        if (++run_inf.HEAT_Time[i] >= DAY_TO_SEC) {
            run_inf.HEAT_Time[i] = 0;
        }
    }

    for (i = 0; i < WET_NUM_MAX; i++) {
        if (++run_inf.WET_Time[i] >= DAY_TO_SEC) {
            run_inf.WET_Time[i] = 0;
        }
    }

    if (++run_inf.TEMPC_Time >= DAY_TO_SEC) {
        run_inf.TEMPC_Time = 0;
    }

    if (++run_inf.SPRAY_Time >= DAY_TO_SEC) {
        run_inf.SPRAY_Time = 0;
    }

    if (run_inf.light_Time != 0) {
        run_inf.light_Time --;

        if (run_inf.light_Time == 0 && dev_inf.SWIN.detail[PWM_SWIN6].enable != STA_ON) {
            sta_out.ALLPWM[PWM_LIGHT] = run_inf.light_Value;
        }
    }

    win_pos_check();

    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        if (run_inf.WIN_ONLY_VAC_Time[i] != 0) {
            run_inf.WIN_ONLY_VAC_Time[i] --;
        }
    }

    if (run_inf.VAC_MoveTime != 0) {
        run_inf.VAC_MoveTime --;

        if (run_inf.VAC_MoveTime == 0) {
            for (i = 0; i < WINDOW_NUM_MAX; i++) {
                if (dev_inf.WIN.detail[i].enable != STA_ON || dev_inf.WIN.detail[i].ctrl_mode != WIN_ONLY_VAC_CTRL) {
                    continue;
                }

                if (get_win_only_vac_control(i, cal_inf.lv) <= 0) {
                    continue;
                }

                if (run_inf.WIN_SWflag[i] == STA_ON) {
                    continue;
                }

                WIN_UP_CON(i, STA_OFF);
                WIN_DOWN_CON(i, STA_OFF);
            }
        }
    }

    for (i = 0; i < TR_TYPE_NUM; i++) {
        if (++run_inf.timer_Time[i] >= DAY_TO_SEC) {
            run_inf.timer_Time[i] = 0;
        }
    }

    for (i = 0; i < SENCON_NUM; i++) {
        if (run_inf.sencon_Time[i] != 0) {
            run_inf.sencon_Time[i] --;
        }
    }

    if (run_inf.VAC_Time != 0) {
        run_inf.VAC_Time --;
    }

    Net_Time_Proc();

    if (run_inf.LV_Time != 0) {
        run_inf.LV_Time --;
    }

    if (run_inf.fan_delay != 0) {
        run_inf.fan_delay --;
    }

    if (run_inf.lv_win_delay != 0) {
        run_inf.lv_win_delay --;
    }

    for (i = 0; i < ALLPWM_NUM_MAX; i++) {
        if (run_inf.pwm_full[i] != 0) {
            run_inf.pwm_full[i] --;
        }
    }

    if (run_inf.fl_timewait > 0) {
        run_inf.fl_timewait ++;
    }

    for (i = 0; i < WINDOW_NUM_MAX + SWINDOW_NUM_MAX; i++) {
        if (win_defrost_en(i) > 0) {
            if (run_inf.swin_defrost_time[i] < DAY_TO_SEC) { //防止超出重新计时
                run_inf.swin_defrost_time[i]++;
            }
        } else {
            run_inf.swin_defrost_time[i] = 0;
        }
    }

    for (i = 0; i < TEMP_NUM_MAX; i++) {
        if (run_inf.temp[i] < DAY_TO_SEC) { //防止超出重新计时
            run_inf.temp[i]++;
        }
    }

    for (i = 0; i < SENSOR_NUM_MAX; i++) {
        if (run_inf.ai_temp[i] < DAY_TO_SEC) { //防止超出重新计时
            run_inf.ai_temp[i]++;
        }
    }

    for (i = 0; i < SWINDOW_NUM_MAX; i++) {
        if (dev_inf.SWIN.detail[i].enable != STA_ON)
            continue;

        if (get_swin_vac_control(i, cal_inf.lv) > 0 && run_inf.VAC_ctrl_flag != VAC_CTRL_NONE) {
            if (run_inf.VAC_ctrl_flag == VAC_CTRL_BIG) {
                if (GetVacOpenWait(dev_inf.SWIN.detail[i].prior_ctrl) >= STA_ON) { //需要等待调整完
                    continue;
                }

                if (GetSwinVacWait(i) > 0) { //静压调节按时间启动
                    continue;
                }

                sta_out.ALLPWM_F[i] += 100.0 / dev_inf.SWIN.detail[i].open_time;

                if (sta_out.ALLPWM_F[i] > 100) {
                    sta_out.ALLPWM_F[i] = 100;
                    ext_inf.lv_save[cal_inf.lv].SWIN[i] = 100;
                }

                sta_out.ALLPWM[i] = (unsigned short)sta_out.ALLPWM_F[i];
            } else if (run_inf.VAC_ctrl_flag == VAC_CTRL_SAMLL) {
                if (GetVacCloseWait(dev_inf.SWIN.detail[i].prior_ctrl) >= STA_ON) { //需要等待调整完
                    continue;
                }

                if (GetSwinVacWait(i) > 0) { //静压调节按时间启动
                    continue;
                }


                sta_out.ALLPWM_F[i] -= 100.0 / dev_inf.SWIN.detail[i].close_time;

                if (sta_out.ALLPWM_F[i] < ctrl_inf.lv[cal_inf.lv].SWIN[i].degree) {
                    sta_out.ALLPWM_F[i] = ctrl_inf.lv[cal_inf.lv].SWIN[i].degree;
                    ext_inf.lv_save[cal_inf.lv].SWIN[i] = ctrl_inf.lv[cal_inf.lv].SWIN[i].degree;
                }

                sta_out.ALLPWM[i] = (unsigned short)sta_out.ALLPWM_F[i];
            }

            continue;//需要负压调节
        }

        if (get_swin_vac_control(i, cal_inf.lv) > 0 && run_inf.VAC_hold_time != 0) {
            continue;//等待保存
        }

        if (run_inf.SWIN_SETdegree[i] > sta_out.ALLPWM[i]) {
            sta_out.ALLPWM_F[i] += 100.0 / dev_inf.SWIN.detail[i].open_time;

            if (sta_out.ALLPWM_F[i] > run_inf.SWIN_SETdegree[i]) {
                sta_out.ALLPWM_F[i] = run_inf.SWIN_SETdegree[i];
            }

            sta_out.ALLPWM[i] = (unsigned short)sta_out.ALLPWM_F[i];
        } else if (run_inf.SWIN_SETdegree[i] < sta_out.ALLPWM[i]) {
            sta_out.ALLPWM_F[i] -= 100.0 / dev_inf.SWIN.detail[i].close_time;

            if (sta_out.ALLPWM_F[i] < run_inf.SWIN_SETdegree[i]) {
                sta_out.ALLPWM_F[i] = run_inf.SWIN_SETdegree[i];
            }

            sta_out.ALLPWM[i] = (unsigned short)sta_out.ALLPWM_F[i];
        } else {
            sta_out.ALLPWM_F[i] = sta_out.ALLPWM[i];
        }
    }

    if (run_inf.report_time != 0) {
        run_inf.report_time --;
    }


    if (run_inf.alarm_shield_time != 0) {
        run_inf.alarm_shield_time --;
    }
}


void JDQ_Fan_Proc(void)
{
    u8 i = 0;

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        //增加一个判断 如果是复用而且其他功能启用了就不控制
        if (Get_Fan_Mul(i, STA_OFF) > 0) {
            continue ;
        }

        if (sta_out.ALLJDQ[JDQ_FAN1 + i] != run_inf.JDQ_TSTA[i]) {
            if (run_inf.JDQ_TSTA[i] == STA_ON && run_inf.JDQ_IDE != 0) { //不能连续开
                continue;
            }

            sta_out.ALLJDQ[JDQ_FAN1 + i] = run_inf.JDQ_TSTA[i];
            JDQ_Fan_CON((JDQ_FAN1 + i), sta_out.ALLJDQ[JDQ_FAN1 + i]);
        }
    }
}

void PWM_Fan_Proc(void)
{
    u8 i = 0;

    for (i = 0; i < PWM_NUM_MAX; i++) {
        if (i < APWM_NUM_MAX && basic_inf.area_pwm[i].enable == STA_ON) {
            continue;//增加一个判断
        }

        if (i == IM_PWM3_PHEAT && basic_inf.pheat_en == STA_ON) {
            continue;//增加一个判断，如果小窗2启用的话就不改变和记录状态
        }

        if (dev_inf.SWIN.detail[i].enable == STA_ON) {
            continue;//增加一个判断，如果小窗2启用的话就不改变和记录状态
        }

        if (sta_out.ALLPWM[(PWM_FAN1 + i)] != run_inf.PWM_Tdegree[i]) {

            if (run_inf.pwm_full[i] != 0) {
                if (run_inf.PWM_Tdegree[i] != 0)
                    continue;

                run_inf.pwm_full[i] = 0;
            }

            if (sta_out.ALLPWM[(PWM_FAN1 + i)] == 0 && ctrl_inf.var.pwm_fan_defrost_time != 0) {
                run_inf.pwm_full[i] = ctrl_inf.var.pwm_fan_defrost_time;
                sta_out.ALLPWM[(PWM_FAN1 + i)] = MAX_DEGREE;
            } else {
                sta_out.ALLPWM[(PWM_FAN1 + i)] = run_inf.PWM_Tdegree[i];
            }

            PWM_Fan_CON((PWM_FAN1 + i), sta_out.ALLPWM[(PWM_FAN1 + i)]);
        }
    }
}

void APWM_Fan_Proc(void)
{
    u8 i = 0;

    for (i = 0; i < APWM_NUM_MAX; i++) {
        if (basic_inf.area_pwm[i].enable != STA_ON) {
            continue;
        }

        if (sta_out.ALLPWM[(PWM_FAN1 + i)] != run_inf.APWM_Tdegree[i]) {

            if (run_inf.pwm_full[i] != 0) {
                if (run_inf.APWM_Tdegree[i] != 0)
                    continue;

                run_inf.pwm_full[i] = 0;
            }

            if (sta_out.ALLPWM[(PWM_FAN1 + i)] == 0 && ctrl_inf.var.pwm_fan_defrost_time != 0) {
                run_inf.pwm_full[i] = ctrl_inf.var.pwm_fan_defrost_time;
                sta_out.ALLPWM[(PWM_FAN1 + i)] = MAX_DEGREE;
            } else {
                sta_out.ALLPWM[(PWM_FAN1 + i)] = run_inf.APWM_Tdegree[i];
            }

            PWM_Fan_CON((PWM_FAN1 + i), sta_out.ALLPWM[(PWM_FAN1 + i)]);
        }
    }
}

void HEAT_Fan_Proc(u8 id)
{
    //增加一个判断 如果没启用就不控制
    if (basic_inf.heat_en[id] != STA_ON) {
        run_inf.HEAT_Time[id] = 0;
        return ;
    }

    if (basic_inf.tempc_en.mode != TEMPC_MODE_HIGH && basic_inf.tempc_en.protect_temp != 0
            && cal_inf.TEMPC_lv >= -1 && cal_inf.tempc_temp > basic_inf.tempc_en.protect_temp) {//强制散热
        run_inf.HEAT_Time[id] = 0;
        sta_out.ALLJDQ[JDQ_HEAT1 + id] = STA_ON;
        JDQ_Control(JDQ_HEAT1 + id, sta_out.ALLJDQ[JDQ_HEAT1 + id]);
        return ;
    }

    if (basic_inf.tempc_pwm_en.mode != TEMPC_MODE_HIGH && basic_inf.tempc_pwm_en.protect_temp != 0
            && cal_inf.TEMPC_pwm_lv >= -1 && cal_inf.tempc_pwm_temp > basic_inf.tempc_pwm_en.protect_temp) {//强制散热
        run_inf.HEAT_Time[id] = 0;
        sta_out.ALLJDQ[JDQ_HEAT1 + id] = STA_ON;
        JDQ_Control(JDQ_HEAT1 + id, sta_out.ALLJDQ[JDQ_HEAT1 + id]);
        return ;
    }

    if (sys_sta.temp_err == STA_ON) {
        run_inf.HEAT_Time[id] = 0;
        sta_out.ALLJDQ[JDQ_HEAT1 + id] = STA_OFF;
        JDQ_Control(JDQ_HEAT1 + id, sta_out.ALLJDQ[JDQ_HEAT1 + id]);
        return ;
    }

    if (sta_out.ALLJDQ[JDQ_HEAT1 + id] == STA_ON && run_inf.HEAT_Time[id] < run_inf.HEAT_Topen[id]) {
        return;
    }

    if (sta_out.ALLJDQ[JDQ_HEAT1 + id] == STA_OFF && run_inf.HEAT_Time[id] < run_inf.HEAT_Tclose[id]) {
        return;
    }

    if (sta_out.ALLJDQ[JDQ_HEAT1 + id] == STA_ON && run_inf.HEAT_Tclose[id] != 0) {
        sta_out.ALLJDQ[JDQ_HEAT1 + id] = STA_OFF;
        run_inf.HEAT_Time[id] = 0;
        JDQ_Control(JDQ_HEAT1 + id, sta_out.ALLJDQ[JDQ_HEAT1 + id]);
        return ;
    }

    if (sta_out.ALLJDQ[JDQ_HEAT1 + id] == STA_OFF && run_inf.HEAT_Topen[id] != 0) {
        sta_out.ALLJDQ[JDQ_HEAT1 + id] = STA_ON;
        run_inf.HEAT_Time[id] = 0;
        JDQ_Control(JDQ_HEAT1 + id, sta_out.ALLJDQ[JDQ_HEAT1 + id]);
        return;
    }

}
void PHEAT_Fan_Proc()
{
    //增加一个判断 如果没启用就不控制
    if (basic_inf.pheat_en != STA_ON) {
        return ;
    }

    if (sys_sta.temp_err == STA_ON) {
        sta_out.ALLPWM[PWM_HEAT] = 0;
        PWM_Out(PWM_HEAT, sta_out.ALLPWM[PWM_HEAT]);
        return ;
    }

    if (cal_inf.PHEAT_lv < 0) {    //关闭
        sta_out.ALLPWM[PWM_HEAT] = 0;
        PWM_Out(PWM_HEAT, sta_out.ALLPWM[PWM_HEAT]);
    } else {  //开启
        sta_out.ALLPWM[PWM_HEAT] = basic_inf.pheat[cal_inf.PHEAT_lv].start_freq;
        PWM_Out(PWM_HEAT, sta_out.ALLPWM[PWM_HEAT]);
    }
}


int AlarmShieldSta(void)
{
    int i = 0;
    uint64_t ala_state_temp = 0;

    ala_state_temp = ALA_State();

    // 是否新加报警
    for (i = 0; i < 64; i++) {
        if (getbit(run_inf.alarm_shield_state, i) == 0 && getbit(ala_state_temp, i) == 1) {//产生了新的报警
            run_inf.alarm_shield_state = 0;
            run_inf.alarm_shield_time = 0;
            break;
        }
    }

    if (run_inf.alarm_shield_time != 0) {
        return 1;
    }

    return -1;
}

void ALARM_Porc(void)
{
    if (basic_inf.ala_en == STA_OFF) {//报警继电器不启用默认关
        sta_out.ALLJDQ[JDQ_ALA] = STA_OFF;
        JDQ_Control(JDQ_ALA, sta_out.ALLJDQ[JDQ_ALA]);

        if (ALA_State() != 0) {
            ALARM_LED_ON;
        } else {
            ALARM_LED_OFF;
        }

        return;
    }

    if (ALA_State() != 0) { //有报警
        if (AlarmShieldSta() < 0) {
            sta_out.ALLJDQ[JDQ_ALA] = STA_ON;
            JDQ_Control(JDQ_ALA, sta_out.ALLJDQ[JDQ_ALA]);
        } else {
            sta_out.ALLJDQ[JDQ_ALA] = STA_OFF;
            JDQ_Control(JDQ_ALA, sta_out.ALLJDQ[JDQ_ALA]);
        }

        ALARM_LED_ON;
    } else { //无报警
        run_inf.alarm_shield_state = 0;
        run_inf.alarm_shield_time = 0;

        if (basic_inf.ala_test.enable == STA_ON && cal_inf.ala_test == STA_ON) {
            if ( sta_out.ALLJDQ[JDQ_ALA] == STA_OFF) { //报警测试
                sta_out.ALLJDQ[JDQ_ALA] = STA_ON;
                JDQ_Control(JDQ_ALA, sta_out.ALLJDQ[JDQ_ALA]);
                ALARM_LED_ON;
            }

            return;
        }

        if (sta_out.ALLJDQ[JDQ_ALA] == STA_ON) {
            sta_out.ALLJDQ[JDQ_ALA] = STA_OFF;
            JDQ_Control(JDQ_ALA, sta_out.ALLJDQ[JDQ_ALA]);
            ALARM_LED_OFF;
        }
    }
}

void WET_Porc(u8 id)
{
//增加一个判断 如果没启用就不控制
    if (dev_inf.wet_en[id] != STA_ON) {
        run_inf.WET_Time[id] = 0;
        return ;
    }

    if (sys_sta.temp_err == STA_ON) {
        run_inf.WET_Time[id] = 0;
        sta_out.ALLJDQ[JDQ_WET + id] = STA_OFF;
        JDQ_Control(JDQ_WET + id, sta_out.ALLJDQ[JDQ_WET + id]);
        return ;
    }

    if (sta_out.ALLJDQ[JDQ_WET + id] == STA_ON && run_inf.WET_Time[id] < run_inf.WET_Topen[id]) {
        return;
    }

    if (sta_out.ALLJDQ[JDQ_WET + id] == STA_OFF && run_inf.WET_Time[id] < run_inf.WET_Tclose[id]) {
        return;
    }

    if (sta_out.ALLJDQ[JDQ_WET + id] == STA_ON && run_inf.WET_Tclose[id] != 0) {
        sta_out.ALLJDQ[JDQ_WET + id] = STA_OFF;
        run_inf.WET_Time[id] = 0;
        JDQ_Control(JDQ_WET + id, sta_out.ALLJDQ[JDQ_WET + id]);
        return ;
    }

    if (sta_out.ALLJDQ[JDQ_WET + id] == STA_OFF && run_inf.WET_Topen[id] != 0) {
        sta_out.ALLJDQ[JDQ_WET + id] = STA_ON;
        run_inf.WET_Time[id] = 0;
        JDQ_Control(JDQ_WET + id, sta_out.ALLJDQ[JDQ_WET + id]);
        return;
    }
}

void TEMPC_Porc(void)
{
//增加一个判断 如果没启用就不控制
    if (basic_inf.tempc_en.enable != STA_ON) {
        run_inf.TEMPC_Time = 0;
        return ;
    }

    if (sys_sta.temp_err == STA_ON) {
        run_inf.TEMPC_Time = 0;
        sta_out.ALLJDQ[JDQ_TEMPC] = STA_OFF;
        JDQ_Control(JDQ_TEMPC, sta_out.ALLJDQ[JDQ_TEMPC]);
        return ;
    }

    if (sta_out.ALLJDQ[JDQ_TEMPC] == STA_ON && run_inf.TEMPC_Time < run_inf.TEMPC_Topen) {
        return;
    }

    if (sta_out.ALLJDQ[JDQ_TEMPC] == STA_OFF && run_inf.TEMPC_Time < run_inf.TEMPC_Tclose) {
        return;
    }

    if (sta_out.ALLJDQ[JDQ_TEMPC] == STA_ON && run_inf.TEMPC_Tclose != 0) {
        sta_out.ALLJDQ[JDQ_TEMPC] = STA_OFF;
        run_inf.TEMPC_Time = 0;
        JDQ_Control(JDQ_TEMPC, sta_out.ALLJDQ[JDQ_TEMPC]);
        return ;
    }

    if (sta_out.ALLJDQ[JDQ_TEMPC] == STA_OFF && run_inf.TEMPC_Topen != 0) {
        sta_out.ALLJDQ[JDQ_TEMPC] = STA_ON;
        run_inf.TEMPC_Time = 0;
        JDQ_Control(JDQ_TEMPC, sta_out.ALLJDQ[JDQ_TEMPC]);
        return;
    }
}

void TEMPC_Pwm_Porc(void)
{
    if (dev_inf.SWIN.detail[PWM_SWIN5].enable == STA_ON) {
        return;//增加一个判断，如果小窗5启用的话就不改变和记录状态
    }

    if (cal_inf.TEMPC_pwm_lv < 0) {    //关闭
        sta_out.ALLPWM[PWM_TEMPC] = 0;
        PWM_Out(PWM_TEMPC, sta_out.ALLPWM[PWM_TEMPC]);
        return;
    }

    sta_out.ALLPWM[PWM_TEMPC] = basic_inf.tempc_pwm[cal_inf.TEMPC_pwm_lv].start_freq;
    PWM_Out(PWM_TEMPC, sta_out.ALLPWM[PWM_TEMPC]);
}

void SPRAY_Porc(void)
{
//增加一个判断 如果没启用就不控制
    if (dev_inf.spray_en == SPRAY_MODE_OFF) {
        run_inf.SPRAY_Time = 0;
        return ;
    }

    if (sys_sta.temp_err == STA_ON) {
        run_inf.SPRAY_Time = 0;
        sta_out.ALLJDQ[JDQ_SPRAY] = STA_OFF;
        JDQ_Control(JDQ_SPRAY, sta_out.ALLJDQ[JDQ_SPRAY]);
        return ;
    }

    if (sta_out.ALLJDQ[JDQ_SPRAY] == STA_ON && run_inf.SPRAY_Time < run_inf.SPRAY_Topen) {
        return;
    }

    if (sta_out.ALLJDQ[JDQ_SPRAY] == STA_OFF && run_inf.SPRAY_Time < run_inf.SPRAY_Tclose) {
        return;
    }

    if (sta_out.ALLJDQ[JDQ_SPRAY] == STA_ON && run_inf.SPRAY_Tclose != 0) {
        sta_out.ALLJDQ[JDQ_SPRAY] = STA_OFF;
        run_inf.SPRAY_Time = 0;
        JDQ_Control(JDQ_SPRAY, sta_out.ALLJDQ[JDQ_SPRAY]);
        return ;
    }

    if (sta_out.ALLJDQ[JDQ_SPRAY] == STA_OFF && run_inf.SPRAY_Topen != 0) {
        sta_out.ALLJDQ[JDQ_SPRAY] = STA_ON;
        run_inf.SPRAY_Time = 0;
        JDQ_Control(JDQ_SPRAY, sta_out.ALLJDQ[JDQ_SPRAY]);
        return;
    }
}

void SWIN_Porc(void)
{
    //小窗
    int i = 0, pwm_set = 0;

    for (i = 0; i < SWINDOW_NUM_MAX; i++) {//只有第5个没有共用的
        if (i != PWM_SWIN5 && dev_inf.SWIN.detail[i].enable != STA_ON) {
            continue;//增加一个判断，如果小窗2没有启用的话就不改变和记录状态
        }

        if (get_swin_vac_control(i, cal_inf.lv) > 0 && (run_inf.VAC_ctrl_flag != VAC_CTRL_NONE || run_inf.VAC_hold_time != 0)) {
            PWM_Out(i, sta_out.ALLPWM[i]);
            continue;//需要负压调节，暂时不继续执行
        }


        if (dev_inf.SWIN.detail[i].enable == STA_ON) {
            pwm_set = run_inf.SWIN_Tdegree[i];
        } else {//没有启用
            pwm_set = 0;
            sta_out.ALLPWM[i] = 0;
            PWM_Out(i, sta_out.ALLPWM[i]);
            continue;
        }

        if (run_inf.swin_defrost_time[WINDOW_NUM_MAX + i] > 0 && run_inf.swin_defrost_time[WINDOW_NUM_MAX + i] < 20) {//小窗防冻调节20秒内不改变状态
            continue;
        }

        if (win_defrost_en(WINDOW_NUM_MAX + i) > 0 //小窗防冻
                && run_inf.swin_defrost_time[WINDOW_NUM_MAX + i] > ctrl_inf.var.swin_hold_time * MIN_TO_SEC) {
            if (sta_out.ALLPWM[i] == 0) {
                pwm_set = abs(ctrl_inf.var.swin_defrost_degree);
            } else if (sta_out.ALLPWM[i] == 100) {
                pwm_set = 100 - abs(ctrl_inf.var.swin_defrost_degree);
            } else {
                pwm_set = sta_out.ALLPWM[i] + ctrl_inf.var.swin_defrost_degree;
            }

            if (pwm_set < 0) {
                pwm_set = 0;
            }

            if (pwm_set > 100) {
                pwm_set = 100;
            }
        }

        if (sta_out.ALLPWM[i] != pwm_set) {//有修改
            run_inf.swin_defrost_time[WINDOW_NUM_MAX + i] = 1;
        }

        run_inf.SWIN_SETdegree[i] = pwm_set;//根据设定的在定时函数里变化实际的值。
        PWM_Out(i, sta_out.ALLPWM[i]);
    }
}

void Light_Porc(void)
{
    if (dev_inf.SWIN.detail[PWM_SWIN6].enable == STA_ON) {
        return;//增加一个判断，如果小窗6启用的话就不改变和记录状态
    }

    if (run_inf.light_Time == 0) {
        PWM_Out(PWM_LIGHT, sta_out.ALLPWM[PWM_LIGHT]);
        return;
    }

    sta_out.ALLPWM[PWM_LIGHT] = run_inf.light_Value + (run_inf.light_Time) * run_inf.light_Scale;
    PWM_Out(PWM_LIGHT, sta_out.ALLPWM[PWM_LIGHT]);

}

void ADJ_DISFAN(u8 sid, u8 dir)
{
    u8 i = 0;

    if (dev_inf.WIN.stop_fan_en == STA_OFF && dir == STA_ON) {//关闭校准时必须停风机
        return;
    }

    run_inf.WIN_StopFan[sid] = STA_ON;

    //关闭所有风机继电器
    for (i = 0; i < JDQ_NUM_MAX; i++) {
        //增加一个判断 如果是复用而且其他功能启用了就不控制
        if (Get_Fan_Mul(i, STA_OFF) > 0) {
            continue ;
        }

        sta_out.ALLJDQ[JDQ_FAN1 + i] = STA_OFF;
        JDQ_Fan_CON((JDQ_FAN1 + i), sta_out.ALLJDQ[JDQ_FAN1 + i]);
    }

    for (i = 0; i < PWM_NUM_MAX; i++) {
        if (dev_inf.PWM[i].enable != STA_ON) {
            continue;
        }

        sta_out.ALLPWM[(PWM_FAN1 + i)] = 0;
        PWM_Fan_CON((PWM_FAN1 + i), sta_out.ALLPWM[(PWM_FAN1 + i)]);
    }
}
void ADJ_ENFAN(u8 sid)
{
//    if (dev_inf.WIN[sid].stop_fan_en == STA_OFF) {
//        return;
//    }

    run_inf.WIN_StopFan[sid] = STA_OFF;
}

int WIN_UP_CON(u8 id , u8 status)
{
    u8 jdq_id;

    if (id > WINDOW_NUM_MAX) {
        return -1;
    }

    if (id == WIN1_ID || id == WIN2_ID) {
        jdq_id = JDQ_WIN1_UP + id * 2;
        sta_out.ALLJDQ[jdq_id] = status;
    } else if (id == WIN3_ID || id == WIN4_ID) {
        jdq_id = JDQ_WIN3_UP + (id - WIN3_ID) * 2;
        sta_out.ALLJDQ[jdq_id] = status;
    }

    JDQ_Control(jdq_id, sta_out.ALLJDQ[jdq_id]);
    return 1;
}
int WIN_DOWN_CON(u8 id , u8 status)
{
    u8 jdq_id;

    if (id > WINDOW_NUM_MAX) {
        return -1;
    }

    if (id == WIN1_ID || id == WIN2_ID) {
        jdq_id = JDQ_WIN1_DOWN + id * 2;
        sta_out.ALLJDQ[jdq_id] = status;
    } else if (id == WIN3_ID || id == WIN4_ID) {
        jdq_id = JDQ_WIN3_DOWN + (id - WIN3_ID) * 2;
        sta_out.ALLJDQ[jdq_id] = status;
    }

    JDQ_Control(jdq_id, sta_out.ALLJDQ[jdq_id]);
    return 1;
}

void WIN_AutoCal(u8 sid, u8 dir)
{
    if (run_inf.WIN_CalMode[sid] != ADJ_READY) {//正在校准中
        return;
    }

    ADJ_DISFAN(sid , dir);

    if (dir == STA_ON) {
        run_inf.WIN_CalMode[sid] = ADJ_UP;
        run_inf.WIN_Time[sid] =  dev_inf.WIN.detail[sid].open_time * WIN_RUN_FREQ;
        WIN_DOWN_CON(sid, STA_OFF);
        WIN_UP_CON(sid, STA_ON);
    } else {
        run_inf.WIN_CalMode[sid] = ADJ_DOWN;
        run_inf.WIN_Time[sid] =  dev_inf.WIN.detail[sid].close_time * WIN_RUN_FREQ;
        WIN_DOWN_CON(sid, STA_ON);
        WIN_UP_CON(sid, STA_OFF);
    }
}

int win_cal_time(void)
{
    time_t sys_time;
    int now_time;

    if (dev_inf.WIN.adjust_en != STA_ON || dev_inf.WIN.cal_time == 0) {
        return STA_OFF;
    }

    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟

    if (dev_inf.WIN.adjust_open_time < dev_inf.WIN.adjust_close_time) {
        if (now_time >= dev_inf.WIN.adjust_open_time && now_time < dev_inf.WIN.adjust_close_time) {
            return STA_ON;
        }
    } else {
        if (now_time >= dev_inf.WIN.adjust_open_time || now_time < dev_inf.WIN.adjust_close_time) {
            return STA_ON;
        }
    }

    return STA_OFF;

}

void WIN_Time_Porc(u8 i, int pwm_set)
{
    //增加一个判断 如果没启用就不控制
    if (dev_inf.WIN.detail[i].enable != STA_ON || dev_inf.WIN.detail[i].mode != WIN_MODE_TIME) {
        run_inf.WIN_Time[i] = 0;
        run_inf.WIN_CalMode[i] = ADJ_READY;
        return ;
    }

    if (run_inf.WIN_Time[i] != 0) {
        if (pwm_set != run_inf.WIN_Degree[i] && run_inf.WIN_CalMode[i] == ADJ_READY) { //这次和上次的目标值不一样，但是还没调整完
            run_inf.WIN_Time[i] = 0;
            WIN_UP_CON(i, STA_OFF);
            WIN_DOWN_CON(i, STA_OFF);
        } else {
            return;
        }
    }

    run_inf.WIN_Degree[i] = pwm_set;//得到目标值

    if (run_inf.WIN_Degree[i] == sta_out.WIN[i]) {
        WIN_UP_CON(i, STA_OFF);
        WIN_DOWN_CON(i, STA_OFF);
        return;
    }

    run_inf.swin_defrost_time[i] = 0;

    //开始自动校准,只有时间控制可以需要自动校准,校准次数为0就不校准
    if (run_inf.WIN_First[i] == STA_ON) {
        run_inf.WIN_First[i] = STA_OFF;
        WIN_AutoCal(i, STA_ON); //第一次上电全开校准
        return;
    }

    if ((win_cal_time() == STA_ON && run_inf.WIN_Num[i] > dev_inf.WIN.cal_time)) {
        run_inf.WIN_First[i] = STA_OFF;

        //之后的校准根据当前开度值比较
        if (dev_inf.WIN.close_degree != 0 && sta_out.WIN[i] < dev_inf.WIN.close_degree) {
            WIN_AutoCal(i, STA_OFF); //全关校准
        } else {
            WIN_AutoCal(i, STA_ON); //全开校准
        }

        return;
    }

    if (run_inf.WIN_Degree[i] - sta_out.WIN[i] > 0) {
        OSTimeDly(1000);

        if (run_inf.WIN_Degree[i] == 100) {
            run_inf.WIN_Num[i] = 0;
            run_inf.WIN_Time[i] = dev_inf.WIN.detail[i].open_time * WIN_RUN_FREQ;
        } else {
            run_inf.WIN_Time[i] = (run_inf.WIN_Degree[i] - sta_out.WIN[i]) * dev_inf.WIN.detail[i].open_time * WIN_RUN_FREQ / 100;
        }

        run_inf.WIN_Time[i] = run_inf.WIN_Time[i] + 1;
        WIN_DOWN_CON(i, STA_OFF);
        WIN_UP_CON(i, STA_ON);
    } else {
        OSTimeDly(1000);

        if (run_inf.WIN_Degree[i] == 0) {
            run_inf.WIN_Num[i] = 0;
            run_inf.WIN_Time[i] = dev_inf.WIN.detail[i].close_time * WIN_RUN_FREQ;
        } else {
            run_inf.WIN_Time[i] = (sta_out.WIN[i] - run_inf.WIN_Degree[i]) * dev_inf.WIN.detail[i].close_time * WIN_RUN_FREQ / 100;
        }

        run_inf.WIN_Time[i] = run_inf.WIN_Time[i] + 1;
        WIN_UP_CON(i, STA_OFF);
        WIN_DOWN_CON(i, STA_ON);
    }

    run_inf.WIN_Num[i]++;
}

// 增加超时停
#define WIN_ER_TIME (30)


void WIN_Pos_Porc(u8 i, int pwm_set)
{
    int degree = 0;

    //增加一个判断 如果没启用就不控制
    if (dev_inf.WIN.detail[i].enable != STA_ON || dev_inf.WIN.detail[i].mode != WIN_MODE_POS) {
        return ;
    }

    if (dev_inf.WIN.detail[i].open_v == dev_inf.WIN.detail[i].close_v) {
        return ;
    }

    run_inf.WIN_Degree[i] = pwm_set;//得到目标值
    degree = WIN_Get_Pos(i);//电位计反馈值

    if (degree < 0) {
        return;
    }

//	接近关闭的时候 30秒如果还没达到目标值就关闭
    if ((degree >= 97 || degree <= 3) && run_inf.WIN_RunMode[i] != WIN_RUN_STOP ) {
        if (run_inf.WIN_ErTime[i] == 0) {
            run_inf.WIN_ErTime[i]++;
        }

        if (run_inf.WIN_ErTime[i] >= WIN_ER_TIME * WIN_RUN_FREQ) {
            run_inf.WIN_ErTime[i] = 0;
            run_inf.WIN_RunMode[i] = WIN_RUN_STOP;
            WIN_UP_CON(i, STA_OFF);
            WIN_DOWN_CON(i, STA_OFF);
        }

    } else {
        run_inf.WIN_ErTime[i] = 0;
    }

    //不动作的情况下1%的变化跳动忽略
    if (run_inf.WIN_RunMode[i] == WIN_RUN_STOP) {
        if (abs(sta_out.WIN[i] - degree) <= 1) {
            degree = sta_out.WIN[i];
        }

        //与实际值相差2%就等于实际值
        if (abs(run_inf.WIN_Degree[i] - degree) <= 2) {
            degree = run_inf.WIN_Degree[i];
        }
    }

    sta_out.WIN[i] = degree;

    //停下来之后需要计算最小开度
    if (run_inf.WIN_RunMode[i] == WIN_RUN_STOP && abs(run_inf.WIN_Degree[i] - sta_out.WIN[i]) <= dev_inf.WIN.min_degree) {
        return;
    }

    run_inf.swin_defrost_time[i] = 0;

    //需要打开
    switch (run_inf.WIN_RunMode[i]) {
    case WIN_RUN_STOP:
        if (sta_out.WIN[i] < run_inf.WIN_Degree[i]) {
            run_inf.WIN_RunMode[i] = WIN_RUN_UP;
            WIN_DOWN_CON(i, STA_OFF);
            WIN_UP_CON(i, STA_ON);
            break;
        }

        if (sta_out.WIN[i] > run_inf.WIN_Degree[i]) {
            run_inf.WIN_RunMode[i] = WIN_RUN_DOWN;
            WIN_UP_CON(i, STA_OFF);
            WIN_DOWN_CON(i, STA_ON);
            break;
        }

        break;

    case WIN_RUN_UP://提前1%停
        WIN_DOWN_CON(i, STA_OFF);
        WIN_UP_CON(i, STA_ON);//静压控制完成会关闭这个继电器所以一直打开
    
        if ((sta_out.WIN[i] + 1) >= run_inf.WIN_Degree[i]) {
            run_inf.WIN_RunMode[i] = WIN_RUN_STOP;
            WIN_UP_CON(i, STA_OFF);
            WIN_DOWN_CON(i, STA_OFF);
            break;
        }

        break;

    case WIN_RUN_DOWN://提前1%停
        WIN_UP_CON(i, STA_OFF);
        WIN_DOWN_CON(i, STA_ON);//静压控制完成会关闭这个继电器所以一直打开
    
        if (run_inf.WIN_Degree[i] == 0) { //如果关闭就一直关闭
            break;
        }

        if (sta_out.WIN[i] <= (run_inf.WIN_Degree[i] + 1)) {
            run_inf.WIN_RunMode[i] = WIN_RUN_STOP;
            WIN_UP_CON(i, STA_OFF);
            WIN_DOWN_CON(i, STA_OFF);
            break;
        }

        break;

    default:
        break;
    }
}

extern U8 get_win_adjust_sta(void);
void WIN_Porc(void)
{
    int i = 0, pwm_set = 0, degree;

    if (sys_sta.auto_mode == STA_OFF || get_win_adjust_sta() == STA_ON) {
        return;
    }

    for (i = 0; i < WINDOW_NUM_MAX; i++) {
        //增加一个判断 如果没启用就不控制
        if (dev_inf.WIN.detail[i].enable != STA_ON ) {
            continue ;
        }

        if (dev_inf.WIN.detail[i].ctrl_mode == WIN_ONLY_VAC_CTRL) {//纯负压是单独控制的
            sta_out.WIN[i] = 0;

            if (get_win_only_vac_control(i, cal_inf.lv) <= 0) {//关闭的时候停一下
                WIN_UP_CON(i, STA_OFF);

                if (dev_inf.WIN.detail[i].open_time == 0 || dev_inf.WIN.detail[i].close_time == 0
                        || run_inf.WIN_ONLY_VAC_Count[i] > 30) { //关闭不停
                    WIN_DOWN_CON(i, STA_ON);
                    continue;
                }

                if (run_inf.WIN_ONLY_VAC_Time[i] == 0) {
                    run_inf.WIN_ONLY_VAC_Count[i]++;
                    run_inf.WIN_ONLY_VAC_Time[i] = dev_inf.WIN.detail[i].open_time + dev_inf.WIN.detail[i].close_time;
                }

                if (run_inf.WIN_ONLY_VAC_Time[i] > dev_inf.WIN.detail[i].close_time) {
                    WIN_DOWN_CON(i, STA_ON);
                } else {
                    WIN_DOWN_CON(i, STA_OFF);
                }
            } else {
                run_inf.WIN_ONLY_VAC_Time[i] = 0;
                run_inf.WIN_ONLY_VAC_Count[i] = 0;
            }


            continue ;
        }


        pwm_set = run_inf.WIN_LvDegree[i];

        if (get_win_vac_control(i, cal_inf.lv) > 0 && (run_inf.VAC_ctrl_flag != VAC_CTRL_NONE || run_inf.VAC_hold_time != 0)) {
            if (dev_inf.WIN.detail[i].mode == WIN_MODE_POS) {//电位计显示读数据
                degree = WIN_Get_Pos(i);

                if (degree >= 0) {
                    sta_out.WIN[i] = degree;
                }
            }

            continue;//需要负压调节，暂时不继续执行
        }

        if (win_defrost_en(i) > 0//小窗防冻
                && run_inf.swin_defrost_time[i] > ctrl_inf.var.swin_hold_time * MIN_TO_SEC) {
            if (sta_out.WIN[i] == 0) {
                pwm_set = abs(ctrl_inf.var.swin_defrost_degree);
            } else if (sta_out.WIN[i] == 100) {
                pwm_set = 100 - abs(ctrl_inf.var.swin_defrost_degree);
            } else {
                pwm_set = sta_out.WIN[i] + ctrl_inf.var.swin_defrost_degree;
            }

            if (pwm_set < 0) {
                pwm_set = 0;
            }

            if (pwm_set > 100) {
                pwm_set = 100;
            }
        }

        WIN_Time_Porc(i, pwm_set); //时间模式小窗处理
        WIN_Pos_Porc(i, pwm_set); //位置模式小窗处理
    }
}

void Circle_Porc(void)
{
    //增加一个判断 如果没启用就不控制
    if (dev_inf.circle.enable != STA_ON) {
        return ;
    }

    if (cal_inf.circle_sta == STA_ON && sta_out.ALLJDQ[JDQ_CIR_FAN] == STA_OFF) { //开
        sta_out.ALLJDQ[JDQ_CIR_FAN] = STA_ON;
        JDQ_Control(JDQ_CIR_FAN, sta_out.ALLJDQ[JDQ_CIR_FAN]);
    } else if (cal_inf.circle_sta == STA_OFF && sta_out.ALLJDQ[JDQ_CIR_FAN] == STA_ON) { //关
        sta_out.ALLJDQ[JDQ_CIR_FAN] = STA_OFF;
        JDQ_Control(JDQ_CIR_FAN, sta_out.ALLJDQ[JDQ_CIR_FAN]);
    }
}

void Timer_Porc(u8 id)
{
    u8 jdq_id = 0;

    if (id >= TR_TYPE_NUM)
        return;

    jdq_id = JDQ_TIMER1 + id;

//增加一个判断 如果没启用就不控制
    if (ext_inf.timer[id].enable != STA_ON) {
        return ;
    }

    if (ext_inf.timer[id].enable != STA_ON) {
        if (sta_out.ALLJDQ[jdq_id] == STA_OFF)return;

        sta_out.ALLJDQ[jdq_id] = STA_OFF;
        JDQ_Control(jdq_id, sta_out.ALLJDQ[jdq_id]);
        return ;
    }

    if (ext_inf.timer[id].mode == TIMER_MODE_CYCLE) {//循环控制模式，按时间来开关

        if (sta_out.ALLJDQ[jdq_id] == STA_ON && run_inf.timer_Time[id] < ext_inf.timer[id].start_min * 60 * RUN_FREQ)
            return;

        if (sta_out.ALLJDQ[jdq_id] == STA_OFF && run_inf.timer_Time[id] < ext_inf.timer[id].stop_min * 60 * RUN_FREQ)
            return;

        if (sta_out.ALLJDQ[jdq_id] == STA_ON && ext_inf.timer[id].stop_min != 0) {
            sta_out.ALLJDQ[jdq_id] = STA_OFF;
            run_inf.timer_Time[id] = 0;
            JDQ_Control(jdq_id, sta_out.ALLJDQ[jdq_id]);
            return ;
        }

        if (sta_out.ALLJDQ[jdq_id] == STA_OFF && ext_inf.timer[id].start_min != 0) {
            sta_out.ALLJDQ[jdq_id] = STA_ON;
            run_inf.timer_Time[id] = 0;
            JDQ_Control(jdq_id, sta_out.ALLJDQ[jdq_id]);
            return;
        }
    } else { //其他模式，按状态来开关
        if (sta_out.ALLJDQ[jdq_id] == STA_ON && cal_inf.timer_sta[id] != STA_ON) {
            sta_out.ALLJDQ[jdq_id] = STA_OFF;
            JDQ_Control(jdq_id, sta_out.ALLJDQ[jdq_id]);
            return ;
        }

        if (sta_out.ALLJDQ[jdq_id] == STA_OFF && cal_inf.timer_sta[id] != STA_OFF) {
            sta_out.ALLJDQ[jdq_id] = STA_ON;
            JDQ_Control(jdq_id, sta_out.ALLJDQ[jdq_id]);
            return;
        }
    }
}

void cal_fan_num(void)
{
    int i = 0, num = 0, air_value = 0;

    num = 0;

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        if (dev_inf.JDQ[i].enable != STA_ON) {
            continue ;
        }

        if (sta_out.ALLJDQ[i] == STA_ON) {
            num += dev_inf.JDQ[i].fan_num;
            air_value += dev_inf.JDQ[i].max_air * dev_inf.JDQ[i].fan_num * AIR_RATE / FAN_RATE;
        }
    }

    cal_inf.jdq_fan_num = num;

    num = 0;

    for (i = 0; i < PWM_NUM_MAX; i++) {
        if (dev_inf.PWM[i].enable == STA_ON) {
            if (sta_out.ALLPWM[i] != 0) {
                num += dev_inf.PWM[i].fan_num;
                air_value += sta_out.ALLPWM[i] * dev_inf.PWM[i].max_air * dev_inf.PWM[i].fan_num * AIR_RATE / (FAN_RATE * 100);
            }
        }
    }

    cal_inf.pwm_fan_num = num;
    cal_inf.total_fan_num = cal_inf.pwm_fan_num + cal_inf.jdq_fan_num;
    cal_inf.cur_air = air_value;
}


void fan_auto_sort(void)
{
    u8 i = 0, id = 0;

    for (i = 0, id = 0; i < JDQ_NUM_MAX; i++) {
        if (dev_inf.JDQ[i].enable == STA_ON) {
            cal_inf.jdq_fan_showid[i] = id++;
        } else {
            cal_inf.jdq_fan_showid[i] = 0;
        }
    }

    for (i = 0, id = 0; i < PWM_NUM_MAX; i++) {
        if (dev_inf.PWM[i].enable == STA_ON) {
            cal_inf.pwm_fan_showid[i] = id++;
        } else {
            cal_inf.pwm_fan_showid[i] = 0;
        }
    }

    for (i = 0, id = 0; i < WINDOW_NUM_MAX; i++) {
        if (dev_inf.WIN.detail[i].enable == STA_ON) {
            cal_inf.win_showid[i] = id++;
        } else {
            cal_inf.win_showid[i] = 0;
        }
    }

    for (i = 0, id = 0; i < SWINDOW_NUM_MAX; i++) {
        if (dev_inf.SWIN.detail[i].enable == STA_ON) {
            cal_inf.swin_showid[i] = id++;
        } else {
            cal_inf.swin_showid[i] = 0;
        }
    }

}

void Work_Proc(void)
{
    u8 i = 0;
    SWIN_Porc();     //先调整小窗

    if (run_inf.WIN_StopFan[0] == STA_OFF && run_inf.WIN_StopFan[1] == STA_OFF//校准时暂停通风
            && run_inf.WIN_StopFan[2] == STA_OFF && run_inf.WIN_StopFan[3] == STA_OFF
            && run_inf.fan_delay == 0) {//风机延时开
        JDQ_Fan_Proc();
        PWM_Fan_Proc();
        APWM_Fan_Proc();
    }

    for (i = 0; i < HEAT_NUM_MAX; i++) {
        HEAT_Fan_Proc(i);
    }

    PHEAT_Fan_Proc();

    ALARM_Porc();

    for (i = 0; i < WET_NUM_MAX; i++) {
        WET_Porc(i);
    }

    TEMPC_Porc();
    TEMPC_Pwm_Porc();
    SPRAY_Porc();
    Light_Porc();
//    WIN_Porc();
    Circle_Porc();

    for (i = 0; i < TR_TYPE_NUM; i++) {
        Timer_Porc(i);
    }

    cal_fan_num();
    fan_auto_sort();

    JDQ_Control_Hard();//统一再最后写一次
}


void Second_Proc(void)
{
    u8 i = 0;
    index_inf.run_time++;
    sys_sta.on_sec++;
    run_inf.water_flow_time++;

    for (i = 0; i < ALLPWM_NUM_MAX; i++) {
        if (sta_out.ALLPWM[i] != 0) {
            sta_out.PWM_TIME[i]++;
        }
    }

    for (i = 0; i < ALLJDQ_NUM_MAX; i++) {
        if (i == JDQ_WIN1_UP && dev_inf.WIN.detail[0].enable == STA_ON) {
            if (sta_out.WIN[0] != 0) {
                sta_out.JDQ_TIME[i]++;
            }

            continue;
        }

        if (i == JDQ_WIN2_UP && dev_inf.WIN.detail[1].enable == STA_ON) {
            if (sta_out.WIN[1] != 0) {
                sta_out.JDQ_TIME[i]++;
            }

            continue;
        }

        if (i == JDQ_WIN3_UP && dev_inf.WIN.detail[2].enable == STA_ON) {
            if (sta_out.WIN[2] != 0) {
                sta_out.JDQ_TIME[i]++;
            }

            continue;
        }

        if (i == JDQ_WIN4_UP && dev_inf.WIN.detail[3].enable == STA_ON) {
            if (sta_out.WIN[3] != 0) {
                sta_out.JDQ_TIME[i]++;
            }

            continue;
        }

        if (i == JDQ_WIN1_DOWN && dev_inf.WIN.detail[0].enable == STA_ON) {
            if (sta_out.ALLJDQ[i] != 0 || sta_out.ALLJDQ[i - 1] != 0) {
                sta_out.JDQ_TIME[i]++;
            }

            continue;
        }

        if (i == JDQ_WIN2_DOWN && dev_inf.WIN.detail[1].enable == STA_ON) {
            if (sta_out.ALLJDQ[i] != 0 || sta_out.ALLJDQ[i - 1] != 0) {
                sta_out.JDQ_TIME[i]++;
            }

            continue;
        }

        if (i == JDQ_WIN3_DOWN && dev_inf.WIN.detail[2].enable == STA_ON) {
            if (sta_out.ALLJDQ[i] != 0 || sta_out.ALLJDQ[i - 1] != 0) {
                sta_out.JDQ_TIME[i]++;
            }

            continue;
        }

        if (i == JDQ_WIN4_DOWN && dev_inf.WIN.detail[3].enable == STA_ON) {
            if (sta_out.ALLJDQ[i] != 0 || sta_out.ALLJDQ[i - 1] != 0) {
                sta_out.JDQ_TIME[i]++;
            }

            continue;
        }


        if (sta_out.ALLJDQ[i] != 0) {
            sta_out.JDQ_TIME[i]++;
        }
    }

    for (i = 0; i < ALA_TYPE_MAX; i++) {
        if (ala_inf[i].sec < TIME_4BIT) {
            ala_inf[i].sec++;
        }
    }
}

#define HUMTEMP_MAX (6)
#define HUMTEMP_HUM (60)
typedef struct {
    unsigned char grow_day;	//是否启用 1-使用
    float 	speed_ratio;	//风速系数
    float 	hum_ratio; 		//湿度系数
} THUMTEMP;
THUMTEMP humtemp_ratio[HUMTEMP_MAX] = {
    {1, 8, 0.12},
    {7, 6, 0.12},
    {14, 5, 0.10},
    {21, 4, 0.08},
    {28, 3.5, 0.06},
    {35, 3, 0.03},
};

//float AT0, AT1, AT2, AT3, AT4, AT10, AT5;
void HumTemp_Cal(void)
{
//     float e = 0; //水汽压
//     float T = cal_inf.temp * 1.0 / TEMP_RATE;
//     float AT = 0;//体感温度
    U8 i = 0, min_lv = 0, max_lv;
    int day = 0;
    float speed_ratio, hum_ratio;

    if (cal_inf.SEN[SENSOR_TYPE_SPEED] == (short)SENSOR_ER_VALUE || cal_inf.SEN_EN[SENSOR_TYPE_SPEED] != STA_ON) {
        if (basic_inf.wind_area == 0) {
            cal_inf.wind_speed = 0;
        } else {
            cal_inf.wind_speed = cal_inf.cur_air * WIND_RATE * 1.0 / (basic_inf.wind_area * 3.6 * AIR_RATE); //m/s
        }
    } else {
        cal_inf.wind_speed = cal_inf.SEN[SENSOR_TYPE_SPEED] * 1.0f / SPEED_RATE;
    }

    if (basic_inf.humtemp_en != STA_ON || cal_inf.temp < 150) {//当前温度小于15度也不显示
        cal_inf.humtemp = TEMP_ER_VALUE;
        return;
    }

    if (cal_inf.SEN_EN[SENSOR_TYPE_HUM] == STA_ON && cal_inf.SEN[SENSOR_TYPE_HUM] != (short)SENSOR_ER_VALUE) {
        cal_inf.humtemp_hum = cal_inf.SEN[SENSOR_TYPE_HUM];
    } else {
        cal_inf.humtemp_hum = HUMTEMP_HUM;//相当于不计算湿度
    }

//查找大小值
    for (max_lv = 0, min_lv = 0, i = 0; i < HUMTEMP_MAX; i++) {
        if (basic_inf.grow_day >= humtemp_ratio[i].grow_day) {
            min_lv = i;
        } else {
            max_lv = i;
            break;
        }
    }

    if (max_lv == 0) { //没有找到大的
        max_lv = min_lv;
    }

    day = humtemp_ratio[max_lv].grow_day - humtemp_ratio[min_lv].grow_day;

    if (day == 0) {
        cal_inf.humtemp = cal_inf.temp - cal_inf.wind_speed * humtemp_ratio[max_lv].speed_ratio * TEMP_RATE +
                          (cal_inf.humtemp_hum - HUMTEMP_HUM) * humtemp_ratio[max_lv].hum_ratio * TEMP_RATE;
        return;
    }

    speed_ratio = (float)((humtemp_ratio[max_lv].speed_ratio - humtemp_ratio[min_lv].speed_ratio ) * 1.0f / day);
    speed_ratio = humtemp_ratio[min_lv].speed_ratio + (basic_inf.grow_day - humtemp_ratio[min_lv].grow_day) * speed_ratio;
    hum_ratio = (float)((humtemp_ratio[max_lv].hum_ratio - humtemp_ratio[min_lv].hum_ratio ) * 1.0f / day);
    hum_ratio = humtemp_ratio[min_lv].hum_ratio + (basic_inf.grow_day - humtemp_ratio[min_lv].grow_day) * hum_ratio;

    cal_inf.humtemp = cal_inf.temp - cal_inf.wind_speed * speed_ratio * TEMP_RATE +
                      (cal_inf.humtemp_hum - HUMTEMP_HUM) * hum_ratio * TEMP_RATE;

//    if (cal_inf.SEN_EN[SENSOR_TYPE_HUM] == STA_ON && cal_inf.SEN[SENSOR_TYPE_HUM] != (short)SENSOR_ER_VALUE) {
//        e = cal_inf.SEN[SENSOR_TYPE_HUM] * 6.105 * exp(17.27 * T / (237.7 + T)) / 100.0;
//    }
//    AT = (1.07 * T + 0.2 * e - 0.65 * cal_inf.wind_speed - 2.7);
//    AT0 = (1.07 * T + 0.2 * e - 0.65 * cal_inf.wind_speed - 2.7) - T;
//    AT10 = (1.07 * T - 0.65 * cal_inf.wind_speed - 2.7) - T;
//    AT1 = (T + 0.33 * e - 0.7 * cal_inf.wind_speed - 4) - T; //AAT
//    AT2 = (T - 0.7 * cal_inf.wind_speed - 4) - T; //AAT no e
//    AT3 = (13.12 + 0.6215 * T - 11.37 * pow(cal_inf.wind_speed, 0.16) + 0.3965 * T * pow(cal_inf.wind_speed, 0.16)) - T; //WCI
//    AT4 = (0.567 * T + 0.393 * e + 3.94) - T; //AWBGT
//    AT5 = (T + 0.5555 * (e - 10.0)) - T; //HI_CA
//    cal_inf.humtemp = AT * 10;
}




