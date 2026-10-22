/**
*****************************************************************************
* @copyright         @2014 - 2023 CCSDA(C)
*****************************************************************************
 * @file app_daytemp_cal.c
 * @version 版本           : Initial Draft
 * @author  作者           : zou
 * @date    创建日期       : 2023/10/10
 * @par     上次修改日期   :
 * @brief   描述           : 昼夜温度调整计算
 @verbatim
 * 函数列表:
              CalDayTemp
              CalDayTempLV
 * 修改历史:
        1.日期: 2023/10/10
          作者: zou
          修改的内容: Created file
 @endverbatim
*****************************************************************************
*/
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "app_daytemp_cal.h"
#include "parameters.h"
#include "pcf8563.h"


/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/
#define DAY_TEMP_CHG_TIME	(10)		//10分钟

/*----------------------------------------------*
 * 枚举定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 结构体定义                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 静态变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/


/**
  Prototype   : CalDayTempLV
 * @brief     : 计算昼夜调整当前曲线段
 * @param     : void  
 * @return    : < 0 不启用
 * @retval    : None
 * @author    :    
   @verbatim    
  修改历史:
        1.日期: 2023/10/11
          作者: zou
          修改的内容: Created function
    @endverbatim
*/
int CalDayTempLV(void)
{
    int i = 0, max_lv = -1;
    int now_time;
    time_t sys_time;

///<修正日龄设置
    for (i = 1; i < DAYTEMP_CURVE_NUM; i++) {
        if (ext_inf.day_temp[i].enable != STA_ON) {
            break;
        }

        if (ext_inf.day_temp[i].grow_day < ext_inf.day_temp[i - 1].grow_day) {
            ext_inf.day_temp[i].grow_day = ext_inf.day_temp[i - 1].grow_day;
        }
    }
	
    for (; i < DAYTEMP_CURVE_NUM; i++) {
		ext_inf.day_temp[i].enable = STA_OFF;
    }
	
///<查找日龄限制
    for (max_lv = -1, i = 0; i < DAYTEMP_CURVE_NUM; i++) {
        if (ext_inf.day_temp[i].enable != STA_ON) {
            break;
        }

        if (basic_inf.grow_day < ext_inf.day_temp[i].grow_day) {
            break;
        }

        max_lv = i;
    }

    if (max_lv < 0) { //没找到小的
        return -1;
    }


///<时间限制
    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟

    for (i = max_lv; i >= 0; i--) {
        if (ext_inf.day_temp[i].enable != STA_ON) {
            break;
        }

        if (ext_inf.day_temp[i].grow_day != ext_inf.day_temp[max_lv].grow_day) {
            continue;
        }

///<时间判断
        if (ext_inf.day_temp[i].start_time < ext_inf.day_temp[i].end_time) {
            if (now_time < ext_inf.day_temp[i].start_time || now_time > ext_inf.day_temp[i].end_time)
                continue;
        } else if (ext_inf.day_temp[i].start_time > ext_inf.day_temp[i].end_time) {
            if (now_time < ext_inf.day_temp[i].start_time && now_time > ext_inf.day_temp[i].end_time)
                continue;
        }

        return i;
    }

    return -2;
}


/**
  Prototype   : CalDayTemp
 * @brief     : 计算昼夜调整的温度值
 * @param     : void  
 * @return    : 
 * @retval    : None
 * @author    :    
   @verbatim    
  修改历史:
        1.日期: 2023/10/11
          作者: zou
          修改的内容: Created function
          cal_inf.tar_day_temp 温度值
    @endverbatim
*/
void CalDayTemp(void)
{
    int now_time, end_time;
    time_t sys_time;
    float temp_scale = 0;
    int cal_time = 0;//计算次数

    cal_inf.tar_day_temp_lv = CalDayTempLV();

///<昼夜温度
    if (cal_inf.tar_day_temp_lv < 0) { //关闭
        cal_inf.tar_day_temp = 0;
        return;
    }

    temp_scale = ext_inf.day_temp[cal_inf.tar_day_temp_lv].fix_temp * 1.0f / ext_inf.day_temp[cal_inf.tar_day_temp_lv].fix_times;

    sys_time = GetSysTime();
    now_time = sys_time % DAY_TO_SEC;
    now_time = now_time / 60; //转换成分钟

    if (now_time < ext_inf.day_temp[cal_inf.tar_day_temp_lv].start_time) {
        now_time = now_time + DAY_TO_MIN;
    }

    if (ext_inf.day_temp[cal_inf.tar_day_temp_lv].end_time < ext_inf.day_temp[cal_inf.tar_day_temp_lv].start_time) {
        end_time = ext_inf.day_temp[cal_inf.tar_day_temp_lv].end_time + DAY_TO_MIN;
    }else{
        end_time = ext_inf.day_temp[cal_inf.tar_day_temp_lv].end_time;
    }

///<进入时每10分钟调整一次，调整次数ctrl_inf.day_temp[cal_inf.tar_day_temp_lv].fix_times
    cal_time = (now_time - ext_inf.day_temp[cal_inf.tar_day_temp_lv].start_time) / DAY_TEMP_CHG_TIME;

    if (cal_time < ext_inf.day_temp[cal_inf.tar_day_temp_lv].fix_times) {
        cal_inf.tar_day_temp = temp_scale * (cal_time + 1);
        return;
    }
	
///<退出时每10分钟调整一次，调整次数ctrl_inf.day_temp[cal_inf.tar_day_temp_lv].fix_times
    cal_time = (end_time - now_time) / DAY_TEMP_CHG_TIME;
    
    if(cal_time<0){
        cal_inf.tar_day_temp = 0;
        return;        
    }
    
	if(cal_time >= ext_inf.day_temp[cal_inf.tar_day_temp_lv].fix_times){
        cal_inf.tar_day_temp = ext_inf.day_temp[cal_inf.tar_day_temp_lv].fix_temp;
	}else{//进入退出时间
        cal_inf.tar_day_temp = temp_scale * (cal_time + 1);
	}
}



