/*
 ******************************************************************************
 * @file    Parameters.h
 * @author
 * @version V1.0.0
 * @date    2019-2-18
 * @brief   .
 * @details
 * .
 ******************************************************************************
  F100
 ******************************************************************************
 */

#ifndef _PARAMETERS_H
#define _PARAMETERS_H
#include <time.h>
#include "energy.h"

#define EMAX(A,B) ( (A) > (B) ? (A) : (B) )
#define EMIN(A,B) ( (A) < (B) ? (A) : (B) )


//数据存储ID
//PWM
#define  FREQ_FAN1_SID      (0)
#define  FREQ_FAN2_SID      (1)
#define  FREQ_FAN3_SID      (2)
#define  FREQ_FAN4_SID      (3) //变频风机 

//#define  SWINDOW_SID        (4) //小窗
//#define  LIGHT_SID          (5) //光照


//温度
#define  TEMP1_SID      	(0) //温度1
#define  TEMP2_SID      	(1) //温度2
#define  TEMP3_SID      	(2) //温度3
#define  TEMP4_SID      	(3) //温度4
#define  TEMP5_SID      	(4) //温度5
#define  TEMP6_SID      	(5) //温度6
#define  TEMP_OUT_SID       (6) //室外
#define  TEMP7_SID      	(7) //温度7

#define  SENSOR_TYPE_NUM		(9)
#define  SENSOR_TYPE_DIS      	(0)
#define  SENSOR_TYPE_TEMP      	(1)
#define  SENSOR_TYPE_HUM      	(2)
#define  SENSOR_TYPE_CO2      	(3)
#define  SENSOR_TYPE_NH3      	(4)
#define  SENSOR_TYPE_H2S      	(5)
#define  SENSOR_TYPE_VAC       	(6)
#define  SENSOR_TYPE_SPEED      (7)
#define  SENSOR_TYPE_OUTHUM     (8)


#define SENCON_NUM 				(3)  //传感器控制个数
#define SENCON_HUM 				(0)  //湿度
#define SENCON_CO2 				(1)  //co2
#define SENCON_NH3 				(2)  //nh3

#define SENCON_READY   			(0)
#define SENCON_DELAY   			(1)
#define SENCON_START   			(2)

#define VAC_CTRL_NONE  			(0)
#define VAC_CTRL_SAMLL 			(1)
#define VAC_CTRL_BIG 			(2)


#define  ALA_RECORD_MAX 	(100)
#define  DATA_RECORD_MAX 	(100)
#define  DATA_PRO_RECORD_MAX (60)
#define  CLOCK_RECORD_MAX 	(100)

//-----------------------------------------

#define KC_MAGIC	(0x3984)	//存储关键字

#define	HOME_HOUSE			(0)	//主页风格1-房子模型
#define	HOME_CIRCLE			(1)	//主页风格2-3个圆圈

#define ROOM_MODE_CHICKEN	(0)
#define ROOM_MODE_SOW		(1)

#define PRO_KC				(0)
#define PRO_HL				(1)


#define	STA_OFF				(0)	//停用、关
#define	STA_ON				(1)	//启用、开
#define	STA_DIS				(2)	//禁用

#define	MIN_WIND_OFF				(0)	//停用
#define	MIN_WIND_AVG				(1)	//平均温度
#define	MIN_WIND_AREA				(2)	//区域温度



#define	WIN_MODE_TIME			(0)	//时间控制 STA_OFF
#define	WIN_MODE_POS			(1)	//位置控制 STA_ON

#define WIN_RUN_STOP   		(0)
#define WIN_RUN_UP   		(1)
#define WIN_RUN_DOWN   		(2)

#define ADJ_READY   		(0)
#define ADJ_UP	    		(1)
#define ADJ_START   		(2)
#define ADJ_DOWN   			(3)
#define ADJ_FINISH  		(4)

#define ALLJDQ_NUM_MAX 		(20)
#define ALLPWM_NUM_MAX 		(6)
//通道数量
#define TEMP_NUM_MAX 		(7)
#define SENSOR_NUM_MAX 		(6)
#define PWM_NUM_MAX 		(4)
#define JDQ_NUM_FIXED 		(4)			//固定的8个定速风机  21/7/9 修改为4个固定的定速风机
#define JDQ_NUM_MAX 		(19)		//
#define HEAT_NUM_MAX 		(3)
#define WINDOW_NUM_MAX 		(4) 		//4个风窗
#define SWINDOW_NUM_MAX 	(6)  		//小窗
#define WET_NUM_MAX 		(2)  		//水泵，另外一个与定时器共用
#define APWM_NUM_MAX 		(3)			// 3区变频风机


#define HEAT_AREA_A			(0)
#define HEAT_AREA_B			(1)
#define HEAT_AREA_C			(2)

#define SWIN1_ID			(0)
#define SWIN2_ID			(1)
#define SWIN3_ID			(2)
#define SWIN4_ID			(3)
#define SWIN5_ID			(4)
#define SWIN6_ID			(5)

#define WIN1_ID             (0)
#define WIN2_ID             (1)
#define WIN3_ID             (2)
#define WIN4_ID             (3)

//数据存储序号
#define SSENSOR_ID		(0)
#define SSWIN_ID		(1)
#define SDIN_ID			(2)
#define SPWM_ID			(3)
#define SJDQ_ID			(4)
#define STEMP_ID		(5)
#define SMINAIRC_ID		(6)
#define SLVPWM_ID		(7)
#define SLVWIN_ID		(8)
#define SLVJDQ_ID		(9)
#define SBASIC_ID		(10)
#define SSPRAY_ID		(11)
#define SWET_ID			(12)
#define SHEAT_ID		(13)
#define SINDEX_ID		(14)
#define SCIRCLE_ID		(15)
#define STIMER_ID		(16)
#define SSCOPE_ID		(17)
#define SSENCON_ID		(18)
#define SWIN_ID		    (19)
#define SLIGHT_ID		(20)
#define SVAC_ID			(21)
#define SSOFT_ID		(22)
#define SENV_ID			(23)
#define SVAR_ID			(24)
#define STEMPC_ID		(25)
#define SAPWM_ID		(26)
#define SFAN_CHK_ID		(27)
#define SHUM_ID			(28)
#define SLVMORE_ID		(29)
#define SSENSOR_RG_ID	(30)
#define SLV_SAVE_ID		(31)
#define STEMP_DEF_ID	(32)
#define STEMPC_PWM_ID	(33)
#define SDEV_SHOW_ID	(34)
#define SCLOCK_ID		(35)
#define SDAYTEMP_ID		(36)



#define SSOFT_INIT_ID	(50)	//FLASH写


#define WIN_DEF_VA		(999)
#define LV_DEF_VA		(0x80)

//倍率
#define TEMP_RATE		(10)
#define HUM_RATE		(1)
#define VAC_RATE		(1)
#define CO2_RATE		(1)
#define NH3_RATE		(1)
#define H2S_RATE		(1)
#define FAN_RATE		(10)
#define WT_RATE			(100)
#define BREATH_RATE		(100)
#define AIR_RATE		(100)
#define AIR_KM			(1000)
#define ENERG_RATE		(100)
#define ENERG_PARA_RATE	(10)
#define PM_RATE			(1)
#define LIGHT_RATE		(1)
#define WATER_RATE		(1000)
#define FOOD_RATE		(100)
#define WIND_RATE		(10)
#define VOL_RATE		(100)
#define SPEED_RATE		(100)
#define CUR_RATE		(10)
#define EXBREATH_RATE	(10)
#define HUM_SCALE		(100)


//限值
#define TEMP_MAX		(999)
#define TEMP_MIN		(-999)
#define TEMP_ADJMAX		(127)
#define TEMP_ADJMIN		(-127)
#define HUM_ADJMAX		(9)
#define HUM_ADJMIN		(-9)
#define VAC_ADJMAX		(9)
#define VAC_ADJMIN		(-9)
#define CO2_ADJMAX		(999)
#define CO2_ADJMIN		(-999)
#define NH3_ADJMAX		(9)
#define NH3_ADJMIN		(-9)
#define H2S_ADJMAX		(9)
#define H2S_ADJMIN		(-9)
#define MAX_DEGREE		(100)
#define ID_MAX	    	(65535)
#define SPEED_ADJMAX	(127)
#define SPEED_ADJMIN	(-127)

#define MAX_GROUP_NUM	(255)
#define MAX_TIME_DAY	(1439)
#define MAX_PERCENT		(100)
//by jx
#define HUM_MAX			(100)
#define CO2_MAX			(10000)
#define NH3_MAX			(100)
#define VAC_MAX			(500)
#define VAC_MIN			(-500)
#define ROOM_MAX 		(999)
#define GROUP_MAX 		(999)
#define VOL_MAX 		(10)
#define AIR_MAX 		(9999)
#define TIME_4BIT		(9999)
#define TIME_3BIT		(999)
#define TIME_2BIT 		(99)
#define TIME_1BIT 		(9)
#define MAX_1BYTE 		(255)
#define TEMP_OFFSET_MAX (200)
#define TEMP_OFFSET_MIN (-200)
#define DAY_MIN 		(-5)
#define DAY_MAX 		(999)
#define DEFAULT_ON_TIME  	(100)
#define DEFAULT_OFF_TIME  	(100)
#define DEFAULT_CYCLE_TIME  (200)
#define ALARM_NUM_MAX 	(4)
#define WATER_ALA_DELAY		(30)
#define DAYTEMP_FIX_TEMP_MIN				(-99)
#define DAYTEMP_FIX_TEMP_MAX				(99)
#define DAYTEMP_FIX_TIMES_MIN				(1)
#define DAYTEMP_FIX_TIMES_MAX				(9)

//报警类型
#define ALARM_ELE	(0)	//环控电箱
#define ALARM_FOOD	(1)	//喂料系统
#define ALARM_CLEAN	(2)	//清粪系统
#define ALARM_OTHER	(3)	//其它报警

//复用
#define MUL_FAN		(0)	//风机复用
#define MUL_OTHER	(1)	//其他复用


//曲线数量
#define HEAT_NUM				(10)
#define WET_NUM					(10)
#define SPRAY_NUM				(10)
#define LIGHT_NUM				(10)
#define TEMP_CURVE_NUM			(10)
#define MINAIR_NUM				(10)
#define CTRL_LV_NUM				(30)
#define TIMER_NUM 				(10) //定时器数据条数
#define LV_SCOPE_NUM			(10)
#define LV_SCOPE_KIND			(3)
#define TEMPC_NUM				(5) //温度控制曲线
#define FREQ_NUM				(10)
#define EXMINAIR_NUM			(10)
#define HUM_CURVE_NUM			(10)
#define DAYTEMP_CURVE_NUM		(10)


//风机尺寸
#define FAN_SIZE_NUM_MAX	(10)
#define FAN_SIZE_NUM		(6)
#define FAN_18I				(0)
#define FAN_24I				(1)
#define FAN_36I				(2)
#define FAN_50I				(3)
#define FAN_51I				(4)
#define FAN_54I				(5)



//为时控启停、持续轮转、持续运行或停止
#define MODE_NUM 			(4)  //个数
#define MODE_OFF 			(0)  //关闭停用
#define MODE_ON  			(1)  //持续运行
#define MODE_ON_OFF 		(2)  //时控起停
#define MODE_ROTATE  		(3)  //持续轮转


//猪舍类型
#define ROOM_TYPE_NUM 			(18) //个数
#define ROOM_TYPE_BOAR 			(0) //公猪舍
#define ROOM_TYPE_RESERVE 		(1) //后备舍
#define ROOM_TYPE_PREGNANCY 	(2)	//妊娠舍       
#define ROOM_TYPE_FARROWING 	(3)	//分娩舍
#define ROOM_TYPE_CONSERVATION 	(4)	//保育舍
#define ROOM_TYPE_FATTEN 		(5) //育肥舍
#define ROOM_TYPE_SOW	 		(6) //母猪舍
#define ROOM_CHICKEN_SEED 		(7) //种鸡舍
#define ROOM_CHICKEN_EGG		(8) //蛋鸡舍
#define ROOM_CHICKEN_MEAT	 	(9)	//肉鸡舍       
#define ROOM_CHICKEN_BROOD	 	(10)//育雏舍
#define ROOM_CHICKEN_GROW 		(11)//育成舍
#define ROOM_CHICKEN_ADULT	 	(12)//成鸡舍
#define ROOM_DUCK_MEAT   		(13)//肉鸭舍
#define ROOM_DUCK_EGG   		(14)//蛋鸭舍
#define ROOM_DUCK_SEED 			(15)//种鸭舍
#define ROOM_LAY_EGGS   		(16)//产蛋舍
#define ROOM_QUAIL   			(17)//鹌鹑舍


#define ROOM_TYPE_CHICKEN_ID	(ROOM_CHICKEN_SEED) //禽类开始序号



//喷淋模式
#define SPRAY_MODE_NUM 			(3)  //个数
#define SPRAY_MODE_OFF 			(0)  //关闭停用
#define SPRAY_MODE_HUM  		(1)  //加湿
#define SPRAY_MODE_TEMP 		(2)  //降温


//定时器模式
#define TIMER_MODE_NUM 			(3)  //个数
#define TIMER_MODE_CYCLE		(0)  //周期运行
#define TIMER_MODE_ON_OFF		(1)  //准点开关
#define TIMER_MODE_ON_DELAY		(2)  //准点开延时关

#define TR_TYPE_NUM 		(4)  //个数
#define TR_TYPE_TIMER1		(0)  //定时器1
#define TR_TYPE_TIMER2		(1)  //定时器2
#define TR_TYPE_TIMER3		(2)  //定时器3
#define TR_TYPE_TIMER4		(3)  //定时器4

#define TIMER_TYPE_NONE		(0)  //定时器1
#define TIMER_TYPE_LIGHT	(1)  //灯光
#define TIMER_TYPE_WATER	(2)  //水线
#define TIMER_TYPE_FEED		(3)  //喂料



//负压控制小窗类型
#define WIN_LV_CTRL 			(0)  //级别开度控制
#define WIN_VAC_CTRL			(1)  //负压控制
#define WIN_ONLY_VAC_CTRL		(2)  //纯负压控制


//#define VAC_CTRL_NUM 			(12)  //个数
//#define VAC_CTRL_STOP 			(0)  //停用
//#define VAC_CTRL_LV 			(1)  //跟随级别
//#define VAC_CTRL_WIN1 			(2)  //风窗1
//#define VAC_CTRL_WIN2 			(3)  //风窗2
//#define VAC_CTRL_WIN3 			(4)  //风窗3
//#define VAC_CTRL_WIN4 			(5)  //风窗4
//#define VAC_CTRL_SWIN1 			(6)  //小窗1
//#define VAC_CTRL_SWIN2 			(7)  //小窗2
//#define VAC_CTRL_SWIN6 			(11) //小窗6



//温控参考温度
#define TEMPC_MODE_HIGH 		(0)  //高温开启
#define TEMPC_MODE_LOW 			(1)  //低温开启
#define TEMPC_MODE_ECIDF		(2)  //节能引风机

//温控开关模式
#define TEMPC_TEMPAVG 		(0)  //平均温度
#define TEMPC_TEMP1			(1)  //温度
#define TEMPC_TEMP2			(2)  //温度
#define TEMPC_TEMP3			(3)  //温度
#define TEMPC_TEMP4			(4)  //温度
#define TEMPC_TEMP5			(5)  //温度
#define TEMPC_TEMP6			(6)  //温度

//登陆类型
#define LOGIN_ADMIN				(0)  //管理员
#define LOGIN_GUEST				(1)  //操作员
#define LOGIN_SUPADMIN			(2)  //超级管理员

#define PRO_DATA_NUM			(3)
#define PRO_DATA_ALL			(0)
#define PRO_DATA_ONE_AGO		(1)
#define PRO_DATA_TWO_AGO		(2)

#define UPDATA_MASTER			(0)
#define UPDATA_SLAVE			(1)


#define PL_DIS					(0)	//不使用
#define PL_DL645_97				(1)	//DL645-97
#define PL_DL645_07				(2)	//DL645-07
#define PL_MB_YC700E			(3)	//YC700E浮点型

#define PL_NUM					(PL_MB_YC700E)

#define WL_DIS					(0)	//不使用
#define WL_PULSE				(1)	//脉冲
#define WL_CJT_188				(2)	//cjt 188
#define WL_MB_OWN				(3)	//modbus
#define WL_NUM					(WL_MB_OWN)

#define FL_DIS					(0)	//不使用
#define FL_PULSE				(1)	//脉冲
#define FL_MB_OWN				(2)	//modbus
#define FL_TIME                 (3) //计时
#define FL_MB_XK3102			(4)	//鼎力modbus
#define FL_NUM					(FL_MB_XK3102)


#define WATER_MODE_CJT		(0)  //
#define WATER_MODE_MB		(1)  //
#define FOOD_MODE_MB		(2)  //
#define ENERGY_MODE_MORE	(3)  //
#define WATER_MODE_PULSE    (4)

//#define PL_MB_FLOAT_INVERSE		(4)	//浮点型反序
//#define PL_MB_INT				(5)	//整型
#define PULSE_WATER_MAX			(5)//脉冲水表个数
#define PRO_DATA_MAX			(6)//水表个数
#define PRO_SAVE_NUM			(8)//保存数量

#define ENERGY_MAX				(3)
#define ENERGY_FAN				(2)

//电表参数个数
#define MB_NUM					(7)
#define MB_ENERGY				(0)
#define MB_VA					(1)
#define MB_VB					(2)
#define MB_VC					(3)
#define MB_IA					(4)
#define MB_IB					(5)
#define MB_IC					(6)

//MODBUS
#define MB_DEV_MAX				(10)
#define MB_DEV_NUM				(2)
#define MB_DEV_PM2_5			(0)
#define MB_DEV_LIGHT			(1)

#define MB_DATA_MAX				(3)
#define MB_DATA_ENERGY			(0)
#define MB_DATA_WATER			(1)
#define MB_DATA_FOOD			(2)


#define UP_MAX_SIZE				(1024*1024)
#define UP_DATA_SIZE			(512)
#define UP_END					(0)
#define UP_START				(1)

#define UP_VER_BAK				(0)	//备份
#define UP_VER_CUR				(1)	//升级
#define UP_VER_RUN				(2)	//正常运行

#define UP_VER_BAK				(0)	//备份
#define UP_VER_CUR				(1)	//升级
#define UP_VER_RUN				(2)	//正常运行


//#define UPDATE_TEMP_HOUR		(4)		//温度更新小时

#define LANGUAGE_NUM				(7)	//语言
#define LANGUAGE_GBK				(0)	//简体中文
#define LANGUAGE_ENGLISH			(1)	//英语
#define LANGUAGE_RUSSIAN			(2)	//俄语
#define LANGUAGE_Viet			    (3)	//越南语
#define LANGUAGE_French			    (4)	//法语
#define LANGUAGE_Spanish			(5)	//西班牙语
#define LANGUAGE_Arabic			    (6)	//阿拉伯语

#define DATA_FACTORY            (0)
#define DATA_SPRING             (1)
#define DATA_SUMMER             (2)
#define DATA_AUTUMN             (3)
#define DATA_WINTER             (4)


#define DEV_ITEM_NUM	(10)
#define CTRL_ITEM_NUM	(8)



#pragma pack(push,1)

typedef struct {
    unsigned short 	wMagic;
    unsigned char 	sta;	//状态
    unsigned char 	type;	//类型
    int 			len;
} TUP_ST, *PUP_ST;

typedef struct {
    unsigned short 	len;
    unsigned char 	buf[1];
} *PUP_FLIE;

typedef struct {
    short TEMP[TEMP_NUM_MAX];	//温度
    short SENSOR[SENSOR_NUM_MAX]; //其他传感器
    short WIN[WINDOW_NUM_MAX]; 	//风窗幕帘反馈
} TSAIN;


typedef struct {
    unsigned char alarm;					//报警
    unsigned char win_up[WINDOW_NUM_MAX];	//风窗上限位
    unsigned char win_down[WINDOW_NUM_MAX];	//风窗下限位
//    unsigned int  water;					//水量 需要保存index里面
//    unsigned int  food;						//料量
} TSDIN;

typedef struct {
    TSAIN 			sensor_real;			//传感器实际值
    TSAIN 			sensor_adj;				//传感器修正值
    TSDIN			din;

    int    			light;						//光照强度
    int				ele[PRO_DATA_MAX][MB_NUM];	//电表数据
    unsigned int 	water[PRO_DATA_MAX];	  	//累积水量
    unsigned int  	food[PRO_DATA_MAX];			//剩余料量
    unsigned int  	old_food[PRO_DATA_MAX];		//上次剩余料量

    signed short  	pm2_5;					//pm2.5 ug/m3
    signed short  	pm10;					//pm10  ug/m3


} TSTA_IN;

typedef struct {
    unsigned char 	up;						//风窗上
    unsigned char 	down;					//风窗下
} TWIN;

typedef struct {
    unsigned char	ALLJDQ[ALLJDQ_NUM_MAX];					//JDQ总数
    int			 	JDQ_TIME[ALLJDQ_NUM_MAX];				//一个周期开启秒

    unsigned short	ALLPWM[ALLPWM_NUM_MAX];					//PWM总数
    int			 	PWM_TIME[ALLPWM_NUM_MAX];				//一个周期开启秒

    unsigned short	WIN[WINDOW_NUM_MAX];		//风窗幕帘，实际是使用了继电器来调节开度的

    float			ALLPWM_F[ALLPWM_NUM_MAX];					//PWM总数，为了计算按秒调节的变化
    float			WIN_F[WINDOW_NUM_MAX];		//风窗幕帘，实际是使用了继电器来调节开度的 为了计算按秒调节的变化
} TSTA_OUT;


typedef struct {
    signed short 	heat_temp;				//加热温度
    signed short 	target_temp;			//目标温度
    signed short 	fan_temp;				//纵向温度
    signed short 	ala_Ltemp;				//低温报警(绝对温度)
    signed short 	ala_Htemp;				//高温报警(绝对温度)

//计算温度曲线用 只包含手动修正不包含湿热补偿
    signed short 	heat_cal_temp;				//加热温度
    signed short 	target_cal_temp;			//目标温度
    signed short 	fan_cal_temp;				//纵向温度
    signed short 	ala_cal_Ltemp;				//低温报警(绝对温度)
    signed short 	ala_cal_Htemp;				//高温报警(绝对温度)

    signed short	tar_hum_temp;			//湿热补偿目标温度
    signed short	tar_hand_temp;			//手动补偿目标温度
    signed short 	lv_ex_offset;			//室外级别偏差修正温度
    unsigned short	target_hum;				//目标湿度 在湿度设置里面有,如果启用曲线使用这个目标
    unsigned char	VaiTemp_num;			//有效温度个数
    unsigned char	VaiTemp_id[TEMP_NUM_MAX];//有效温度ID
    unsigned char	oldVaiTemp_id[TEMP_NUM_MAX];//上一次有效温度ID
    unsigned char	VaiSen_id[SENSOR_NUM_MAX];//有效传感器ID
    unsigned char	oldVaiSen_id[SENSOR_NUM_MAX];//上一次有效传感器ID
    signed short 	BaseTemp;				//多个探头的基准温度
    signed short 	temp;					//平均温度
    signed short 	tempArea[HEAT_NUM_MAX];	//平均温度A 平均温度B 平均温度C
    signed short 	real_ala_Ltemp;			//实际低温报警(绝对温度)
    signed short 	real_ala_Htemp;			//实际高温报警(绝对温度)
    signed short 	tempc_temp;				//温控开关的温度
    signed short 	tempc_pwm_temp;			//模拟量温控开关的温度


    unsigned char 	SEN_ER[SENSOR_TYPE_NUM];//其他传感器错误
    unsigned char 	SEN_EN[SENSOR_TYPE_NUM];//其他传感器启用
    signed short 	SEN[SENSOR_TYPE_NUM];	//其他传感器平均值
    unsigned int	cur_air;				//实时风量
    unsigned int	min_air;				//最小通风量
    unsigned int	lv_air[CTRL_LV_NUM];	//每个级别通风量
    signed char		lv;						//当前级别
    signed char		old_lv;					//上一次级别
    signed char		max_lv;					//设置的最大级别
    signed char		exminair_lv;			//最小通风扩展级别  <0不控制
    signed char		lv_l;					//最小通风的最低级别
    signed char		lv_h;					//最大的偏差0级别
    signed char		as_min_lv;				//辅助控制的最小级别
    signed char		as_max_lv;				//辅助控制的最大级别
//    unsigned char	HEAT_STA[HEAT_NUM_MAX]; //加热状态位
    signed char   	HEAT_lv[HEAT_NUM_MAX];    //加热当前曲线
//    signed char   	old_HEAT_lv[HEAT_NUM_MAX];//加热上一次曲线
    signed char   	PHEAT_lv;    			//加热当前曲线
    signed char   	light_value;            //当前光照量
    signed char   	WET_lv[WET_NUM_MAX];    //湿帘水泵当前曲线
//    signed char   	old_WET_lv[WET_NUM_MAX];//湿帘水泵上一次曲线
    signed char   	TEMPC_lv;    			//温控开关当前曲线
    signed char   	TEMPC_pwm_lv;    		//模拟量温控开关当前曲线
//    signed char   	old_TEMPC_lv;			//温控开关上一次曲线
    signed char   	SPRAY_lv;               //喷淋喷雾当前曲线
//    signed char   	old_SPRAY_lv;           //喷淋喷雾上一次曲线
    unsigned char	circle_sta;				//环流风机开启关闭
    unsigned char	timer_sta[TR_TYPE_NUM];	//定时器开启关闭
    unsigned char	ala_test;				//报警测试开启关闭

    float			wind_speed;				//风速 m/s
    float			humtemp_hum;			//湿度 %
    signed short  	humtemp;				//体感温度

    unsigned short	pwm_fan_num;
    unsigned short	jdq_fan_num;
    unsigned short	total_fan_num;			//风机个数
    unsigned short	jdq_fan_cur;
    unsigned short  fan_cur_max;            //最大的风机额定电流
    unsigned short	fan_error_cur;			//风机误差电流

    signed short 	target_vac;				//目标压力

    unsigned char	jdq_fan_showid[JDQ_NUM_MAX];			//定速风机显示id
    unsigned char	pwm_fan_showid[PWM_NUM_MAX];			//变频风机显示id
    unsigned char	win_showid[WINDOW_NUM_MAX];				//风窗显示id
    unsigned char	swin_showid[SWINDOW_NUM_MAX];			//小窗显示id
    
    signed char		tar_day_temp_lv;		//昼夜调整当前运行曲线 <0不运行
    signed short	tar_day_temp;			//昼夜调整目标温度
	

} TCAL;

typedef struct {
    unsigned char	enable;					//启用
    unsigned char	id;						//当前运行的序号
    int 			time;  					//当前运行时间
} TROTATE;

typedef struct {
    int 			LV_StopTime;					//级别启停运转时间
    int 			LV_StartTime;					//级别启停运转时间

    int 			LV_StopLast;					//级别停止时间倒计时
    int 			LV_StartLast;					//级别启动时间倒计时
//	int 			LV_RotateTime;					//级别轮转运转时间

    unsigned char 	JDQ_TSTA[JDQ_NUM_MAX];			//定速风机状态
    unsigned char 	JDQ_ROTATE_ID;					//定速风机轮转ID
    int 			JDQ_IDE;						//开关一个风机之后的延时

    unsigned char 	PWM_Tdegree[PWM_NUM_MAX];   	//调速风机开度
    unsigned char 	PWM_ROTATE_ID;					//调速风机轮转ID
    unsigned char 	APWM_Tdegree[APWM_NUM_MAX];		//分区调速风机开度
    unsigned char 	SWIN_Tdegree[SWINDOW_NUM_MAX];	//小窗开度
    unsigned char 	SWIN_SWflag[SWINDOW_NUM_MAX];	//小窗开度切换标志
    int 		 	SWIN_VACTime[SWINDOW_NUM_MAX];	//小窗负压调节启动计时


    int 		 	HEAT_Topen[HEAT_NUM_MAX];		//加热开时间
    int 		 	HEAT_Tclose[HEAT_NUM_MAX];		//加热关时间
    int 		 	HEAT_Time[HEAT_NUM_MAX];		//加热计时

    int 		 	WET_Topen[WET_NUM_MAX];			//湿帘水泵开时间
    int 		 	WET_Tclose[WET_NUM_MAX];		//湿帘水泵关时间
    int 		 	WET_Time[WET_NUM_MAX];			//湿帘水泵计时

    int 		 	TEMPC_Topen;					//温控开关开时间
    int 		 	TEMPC_Tclose;					//温控开关关时间
    int 		 	TEMPC_Time;						//温控开关计时

    int 		 	SPRAY_Topen;					//喷淋喷雾开时间
    int 		 	SPRAY_Tclose;					//喷淋喷雾关时间
    int 		 	SPRAY_Time;						//喷淋喷雾计时

    unsigned char 	light_Value;					//光照量
    int 		 	light_Time;						//光照计时
    float		 	light_Scale;					//光照运行比例

    unsigned char	WIN_First[WINDOW_NUM_MAX];		//风窗幕帘第一次上电
    unsigned char 	WIN_StopFan[WINDOW_NUM_MAX];	//风窗幕帘校准暂停通风
    unsigned char 	WIN_CalMode[WINDOW_NUM_MAX];	//风窗幕帘校准模式 时间控制用
    unsigned char 	WIN_RunMode[WINDOW_NUM_MAX];	//风窗幕帘校准模式 位置控制用
    unsigned char 	WIN_Degree[WINDOW_NUM_MAX];		//风窗幕帘目标角度
    int 		 	WIN_Time[WINDOW_NUM_MAX];		//风窗幕帘计时
    int 		 	WIN_ErTime[WINDOW_NUM_MAX];		//风窗幕帘最后电位计时
    unsigned short	WIN_Num[WINDOW_NUM_MAX];		//风窗幕帘调整次数
    unsigned char   WIN_LvDegree[WINDOW_NUM_MAX];   //级别控制的风窗开度
    unsigned char 	WIN_SWflag[WINDOW_NUM_MAX];		//风窗幕帘开度切换标志
    
    int 		 	WIN_PosTime[WINDOW_NUM_MAX];	//风窗幕帘电位计检测
    unsigned char 	WIN_PosState[WINDOW_NUM_MAX];	//风窗幕帘电位计检测时保存上一次的状态
    unsigned char 	WIN_PosDegree[WINDOW_NUM_MAX];	//风窗幕帘电位计检测时保存开度
    int 		 	WIN_ONLY_VAC_Time[WINDOW_NUM_MAX];//风窗幕帘纯负压计时
    int 		 	WIN_ONLY_VAC_Count[WINDOW_NUM_MAX];//风窗幕帘纯负压次数
    int 		 	WIN_VACTime[WINDOW_NUM_MAX];	//风窗负压调节启动计时  
	
    int 		 	timer_Time[TR_TYPE_NUM];		//定时器计时

    int 		 	sencon_Time[SENCON_NUM];		//辅助传感器计时
    short			sencon_Temp[SENCON_NUM];		//辅助传感器开始时温度
    unsigned char 	sencon_Mode[SENCON_NUM];		//辅助传感器模式
    unsigned char 	sencon_Lv[SENCON_NUM];			//辅助传感器级别

    int 		 	VAC_MoveTime;					//VAC纯负压控制动作时间
    int 		 	VAC_Time;						//VAC计时 
    unsigned char 	VAC_Mode;						//VAC模式 
	int				VAC_ctrl_flag;					//VAC调节标志，<=0 不调节， 1 负压过小， 2 负压过大
    int 		 	VAC_hold_time;					//负压稳定时间 次数
		
    int 		 	NetSta_Time;					//网络测试计时

    int 		 	LV_Time;						//级别保持时间

    unsigned char	time_on;						//时控启停标志
    unsigned char   win_flg;                        //小窗调整标志
    unsigned short  fan_delay;						//风机延时开

    unsigned short  pwm_full[ALLPWM_NUM_MAX];       //调速风机全开延时

    unsigned short  fl_timewait;                    //料量计时使用

    int 			swin_defrost_time[WINDOW_NUM_MAX + SWINDOW_NUM_MAX];			//小窗防冻计时
    int				temp[TEMP_NUM_MAX];                    		//温度计时用
    int				ai_temp[SENSOR_NUM_MAX];                    //传感器计时用
    
    unsigned char 	SWIN_SETdegree[SWINDOW_NUM_MAX];	//小窗设定开度，用来比较计算使用
    unsigned short  lv_win_delay;					//级别切换过程中小窗需要开启的时间,时间不为0时负压不受控制
    unsigned char	old_adj_hum;					//上一次调节的湿度

	int             report_time;            		//水电表上传间隔

    unsigned char	clock_in_state;					//打卡标志
    unsigned char	clock_in_report;				//打卡上报

	int             alarm_shield_time;            	//报警屏蔽计时
	uint64_t 		alarm_shield_state;				// 报警屏蔽时的报警

	int				water_flow_time;	
	unsigned int    water_record[WATER_ALA_DELAY];		//每分钟饮水记录
	
} TRUN;

// 不要修改类型
typedef struct {
    short TEMP[TEMP_NUM_MAX];	//温度
    short SENSOR[SENSOR_NUM_MAX]; //其他传感器
    unsigned char TEMP_EN[TEMP_NUM_MAX];	//温度
    unsigned char SENSOR_EN[SENSOR_NUM_MAX]; //其他传感器
} TSENSOR;

typedef struct {
    struct {
        unsigned char 	work_mode: 3;				//工作模式 0-时间 1-位置
        unsigned char 	auto_cal_mode: 3;			//自动校准模式 0-不校准 1-校准
        unsigned char 	stop_control_flag: 1; 		//暂停控制通风
        unsigned char 	enable: 1;					//是否启用 1-使用
    } x;

    unsigned short 	total_time;				//总行程运行时间
    unsigned char 	auto_cal_time;			//自动校准次数
    unsigned short 	close_v;				//关电压值 100倍
    unsigned short 	open_v;					//开电压值 100倍
} TWINDOWS_BAK;

typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned char 	open_time;				//开运行时间
    unsigned char 	close_time;				//关运行时间
    unsigned char 	ctrl_mode;				//控制模式 0-开度控制 1-静压控制
    unsigned char 	prior_ctrl;				//优先控制
} TSWIN_X;

typedef struct {
    unsigned char 	bak[8];					//备用
	TSWIN_X			detail[SWINDOW_NUM_MAX];	//小窗
} TSWIN;

typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned char 	mode;					//工作模式 0-时间 1-位置
    unsigned char 	open_time;				//开运行时间
    unsigned char 	close_time;				//关运行时间
    unsigned short 	open_v;					//开电压值 100倍
    unsigned short 	close_v;				//关电压值 100倍
    unsigned char 	ctrl_mode;				//控制模式 0-开度控制 1-静压控制 2-纯静压控制
    unsigned char 	prior_ctrl;				//优先控制
} TWINDOWS_X;

typedef struct {
	unsigned short  adjust_open_time;		//自动校准时间范围
	unsigned short  adjust_close_time;		//自动校准时间范围
    unsigned char 	adjust_en;				//自动校准模式 0-不校准 1-校准
    unsigned char 	stop_fan_en; 			//暂停控制通风
    unsigned char 	cal_time;				//自动校准次数
    unsigned char 	min_degree;				//最小运行开度
    unsigned char 	close_degree;			//风窗开口小于这个开度值时，校准关闭且停止风机,0不启用
    unsigned char 	bak[4];					//备用
	TWINDOWS_X		detail[WINDOW_NUM_MAX];
} TWINDOWS;


typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned char 	addr;					//地址
} TMODBUS;

typedef struct {
    struct {
        unsigned char 	port: 2;				//串口号 预留
        unsigned char	dbits: 2;			// 0~3:5-8停止位
        unsigned char	stopb: 2;			// 0=1bit,1=1.5bits,2=2bits
        unsigned char	parity: 2;			// 0=none,1=odd,2=even
    } ux;

    unsigned char 		baud;				//波特率
    unsigned short		reg_addr;			//寄存器地址

    struct {
        unsigned short 	len: 4;				//寄存器长度
        unsigned short	point: 4;			// 小数位
        unsigned short	mem: 4;				// 存储格式
        unsigned short	encode: 4;			// 编码格式
    } dx;
} TMB_DATE;

typedef struct {
    unsigned char 	energy_sta;				//电表类型 0-停用  1-DL645-97 2-DL645-07 3-modbus浮点型
    unsigned char 	energy_addr[12]; 		//表地址 modbus地址取0 dl645地址使用ascii码,不够后补0
    unsigned char 	water_en;				//水量计
    unsigned char 	water_addr;				//水量计地址
    unsigned short 	water_unit;				//水量计单位
    unsigned char 	food_sta; 				//料量计 0-停用  1-脉冲 2-英孚克斯
    unsigned char 	food_addr;				//料量计地址
    unsigned short 	food_unit;				//料量计单位
    struct {
        unsigned char	type: 7;				//外部报警类型
        unsigned char	en: 1;				//外部报警
    } alarm;
    unsigned char 	energy_ct;				//电表CT变比
} TDIN_BAK;



typedef struct {
    unsigned char 	energy_sta;				//电表类型 0-停用  1-DL645-97 2-DL645-07 3-modbus浮点型
    unsigned char 	energy_bak;				//舍号备用
    unsigned char 	energy_addr[12]; 		//表地址 modbus地址取0 dl645地址使用ascii码,不够后补0
    unsigned char 	energy_ct;				//电表CT变比
    unsigned char 	water_sta;				//水量计
    unsigned char 	water_bak;				//舍号备用
    unsigned char 	water_addr[14];			//水量计地址
    unsigned short 	water_unit;				//水量计单位
    unsigned char 	food_sta; 				//料量计 0-停用  1-脉冲 2-英孚克斯
    unsigned char 	food_bak;				//舍号备用
    unsigned char 	food_addr;				//料量计地址
    unsigned short 	food_unit;				//料量计单位
} TPRO_DATA;



typedef struct {
    TPRO_DATA		pro_data[PRO_DATA_MAX];
    TMB_DATE		mb_data[MB_DATA_MAX];

	unsigned char   alarm;					//外部报警
	unsigned char	egg_en[PRO_DATA_MAX];	//启用集蛋计数
    unsigned char 	leakage_enable;					    //是否启用 1-使用
    unsigned char 	leakage_max_flow;					//最大流量
    unsigned char 	leakage_delay_min;					//报警延时
    unsigned char 	lack_enable;			    //是否启用 1-使用
    unsigned char 	lack_min_flow;				//最小流量
    unsigned char 	lack_delay_min;				//报警延时	
} TDIN;

//PWM模拟量
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned char 	type;  					//风机型号
    unsigned short 	max_air;				//最大风量
    unsigned char 	fan_num;				//风机个数
    unsigned char 	bak[2];					//预留
    unsigned char 	min_vol;				//最小电压
} TPWM;
//JDQ 定速风机
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned char 	type;  					//风机型号
    unsigned short 	max_air;				//最大风量
    unsigned char 	fan_num;				//风机个数
    unsigned char 	current;				//额定电流
    unsigned short	bak;					//预留
} TJDQ;

//湿帘水泵
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned short	start_time;				//开始时间
    unsigned short	stop_time;				//停止时间
    unsigned char 	start_offtemp;  		//启动偏差
    unsigned char 	stop_offtemp;			//停止偏差
    unsigned char 	max_hum;				//最高湿度
    unsigned short 	start_sec;  			//启动时间
    unsigned short 	stop_sec;				//关闭时间
} TWET;

//湿帘水泵
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned short	start_time;				//开始时间
    unsigned short	stop_time;				//停止时间
    unsigned char 	start_hum;  			//启动湿度
    unsigned char 	stop_hum;				//停止湿度
    signed char 	min_offtemp;			//最小偏差
    unsigned short 	start_sec;  			//启动时间
    unsigned short 	stop_sec;				//关闭时间
} TSPRAY;

//光照曲线
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned short 	grow_day;     			//日龄
    unsigned short	start_time;				//开始时间
    unsigned short	stop_time;				//停止时间
    unsigned char 	value;					//光照强度
} TLIGHT_CURVE;


//光照
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned short 	delay_ontime;			//延时开时间 S
    unsigned short 	delay_offtime;			//关时间 S
    TLIGHT_CURVE	curve[LIGHT_NUM];
} TLIGHT;


//加热器
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned short	min_start;				//最短开始时间
    unsigned short	min_stop;				//最短停止时间
} THEAT_BAK;
//加热器
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned short	start_time;				//开始时间
    unsigned short	stop_time;				//停止时间
    signed char 	start_offtemp;  		//启动偏差
    signed char 	stop_offtemp;			//停止偏差
    unsigned short 	start_sec;  			//启动时间
    unsigned short 	stop_sec;				//关闭时间
} THEAT;

//模拟量加热器
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned short	start_time;				//开始时间
    unsigned short	stop_time;				//停止时间
    signed char 	start_offtemp;  		//启动偏差
    signed char 	stop_offtemp;			//停止偏差
    unsigned char 	start_freq;  			//启动频率
} TPHEAT;

//温控
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned short	start_time;				//开始时间
    unsigned short	stop_time;				//停止时间
    signed short 	start_offtemp;  		//启动温度
    signed short 	stop_offtemp;			//停止温度
    unsigned short 	start_sec;  			//启动时间
    unsigned short 	stop_sec;				//关闭时间
} TTEMPC;

//温控 PWM
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned short	start_time;				//开始时间
    unsigned short	stop_time;				//停止时间
    signed short 	start_offtemp;  		//启动温度
    signed short 	stop_offtemp;			//停止温度
    unsigned char 	start_freq;  			//启动频率
} TTEMPC_PWM;

//温控启用
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned char	mode;					//工作模式
    unsigned char	temp_id;				//参考温度0-平均 1-温度1。。。
    unsigned short	protect_temp;			//强制散热水温  
    unsigned short	start_temp;				//暖风机启动水温 
    unsigned short	min_ala_temp;			//低温报警温度  
    unsigned short	max_ala_temp;			//高温报警温度
    unsigned char	bak[8];					//备用
} TTEMPE;

//环流风机
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    signed char 	offset;					//启动偏差
    unsigned char 	temp_sta[TEMP_NUM_MAX + 1]; //启用比较 兼容之前的协议
    unsigned short 	start_time;				//开时刻
    unsigned short 	stop_time; 				//关时刻
    unsigned char 	start_lv;				//开始级别
    unsigned char 	stop_lv; 				//关闭级别
} TCIRCLE;


//定时器
typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    unsigned char 	type;					//定时器类型
    unsigned char 	back_time;				//回退时间
    unsigned char 	mode;					//模式
    unsigned short 	start_min;				//开周期
    unsigned short 	stop_min; 				//关周期
    unsigned char	bak[10];				//备用
    unsigned char 	num_en[TIMER_NUM];		//是否启用 1-使用
    unsigned short 	start_time[TIMER_NUM];	//开时刻
    unsigned short 	stop_time[TIMER_NUM];	//关时刻
    unsigned short 	delay_min[TIMER_NUM]; 	//延时关
} TTIMER;


typedef struct {
    unsigned short	pm2_5_H;				//PM2.5高报警
    unsigned short	pm10_H;					//PM10高报警
    unsigned short	x[18];					//预留报警
} TLIMIT;

typedef struct {
    unsigned char 	enable;			//是否启用 1-使用
    unsigned char 	percent;		//报警比例
    signed char 	offset_cur;		//偏差电流
    unsigned char 	phase_cur;		//三相电流最大差值  0 不检测
    unsigned char 	energy_sta;				//电表类型 0-停用  1-DL645-97 2-DL645-07 3-modbus浮点型
    unsigned char 	energy_addr[12]; 		//表地址 modbus地址取0 dl645地址使用ascii码,不够后补0
    unsigned char 	energy_ct;				//电表CT变比
    unsigned char 	error_cur;		//风机电流误差
    unsigned char   bak[23];			//预留
} TFAN_CUR;

typedef struct {
    unsigned char 	enable;			//是否启用 1-使用
    signed short 	min_value;		//最大值
    signed short 	max_value;		//最小值
} TSENSOR_RG;

typedef struct {
    unsigned short 	wMagic;
    TSENSOR 		sensor_adj;
	unsigned char	dev_equ_show[DEV_ITEM_NUM];		//设备显示数据位置
	unsigned char	ctrl_equ_show[CTRL_ITEM_NUM];	//设备显示数据位置
    unsigned char	din_bak[5];
    TPWM			PWM[PWM_NUM_MAX];
    TJDQ			JDQ[JDQ_NUM_MAX];
    THEAT_BAK		HEAT_BAK[HEAT_NUM_MAX];//兼容原来的程序的参数 保留
    unsigned char	swin_en_bak;
    unsigned char	bak;
    TWINDOWS_BAK    WIN_BAK[2];            //只有两个窗户能接反馈
    unsigned char	wet_en[WET_NUM_MAX];
    TWET			wet[WET_NUM_MAX][WET_NUM];
    unsigned char	spray_en;
    TSPRAY			spray[SPRAY_NUM];
    TLIGHT			light;
    TCIRCLE			circle;
    unsigned char	TEMP_Define[HEAT_NUM_MAX][TEMP_NUM_MAX+3];	
    unsigned char	timer_bak[11];
    TMODBUS			env_dev[MB_DEV_MAX];	//其他环境设备 modbus协议
    TLIMIT			limit;
    TSENSOR_RG		sensor_rg[SENSOR_TYPE_NUM];//第一个不用
    TFAN_CUR		fan_chk;
    TSWIN			SWIN;
    TWINDOWS		WIN;
} TDEV;

//级别
typedef struct {
    unsigned char		WIN[WINDOW_NUM_MAX]; 		//风窗1，风窗2
    unsigned char 		SWIN[SWINDOW_NUM_MAX]; 	    //小窗
} TLV_SAVE;

typedef struct {
    u8 		enable;   			//是否启用  
    s16 	grow_day;     		//日龄     
    u16		start_time;			//开始时间
    u16		end_time;			//停止时间
    s8 		fix_temp; 			//补偿温度
    u8 		fix_times;   		//调节次数
} TDAYTEMP,*PDAYTEMP;

typedef struct {
    unsigned short 	wMagic;
    TDIN			din;
	TTIMER			timer[TR_TYPE_NUM];
	TLV_SAVE		lv_save[CTRL_LV_NUM];
	TDAYTEMP		day_temp[DAYTEMP_CURVE_NUM];	//昼夜调整
	
} TEXT;

//温度曲线,顺序不能修改
typedef struct {
    unsigned char 	enable;   			//是否启用 1-使用
    unsigned short 	grow_day;     		//日龄
    signed short 	heat_temp; 			//加热温度
    signed short 	target_temp; 		//目标温度
    signed short 	vertical_temp; 		//纵向温度
    signed short 	min_temp;			//最低温度
    signed short 	max_temp;			//最高温度
} TTEMP;

typedef struct {
    unsigned char 	start_day;   		//开始日龄
    unsigned char 	stop_day;   		//结束日龄
    unsigned char   hum_Ltemp;           //体感的低温报警偏差
    unsigned char   hum_Htemp;           //体感的高温报警偏差
    unsigned char 	ala_diff_en;   		//报警值使用偏差值
    unsigned char 	day_temp_en;   		//日龄温度多次变化
    unsigned char 	chg_hours;   		//日龄温度变化时间
    unsigned char 	chg_times;   		//日龄温度变化次数
    unsigned char 	minwind_temp_sta;   //低于加热最小通风 0-2
    unsigned char   temp_simple;		//温度简化
    unsigned char	bak[40];// 40
} TEXTEMP;

//最小通风
typedef struct {
    unsigned char 	enable;        		//是否启用 1-使用
    unsigned short 	grow_day;     		//日龄
    unsigned short 	weight;       		//重量
    unsigned short 	breath;   			//呼吸系数
    unsigned int 	min_air; 			//最低通风量 KM/H
} TMINAIR;

typedef struct {
    unsigned char 	enable;        		//是否启用 1-使用
    signed short 	out_temp;     		//室外温度
    signed short 	lv_temp;       		//级别温差修正
    unsigned char 	breath;   			//呼吸系数比
    unsigned char	bak;
} TEXMINAIR_X;

typedef struct {
    unsigned char 	enable;        		//是否启用 1-使用
    TEXMINAIR_X		x[EXMINAIR_NUM];	//
    unsigned char	bak[24];
} TEXMINAIR;


//定速风机级别
typedef struct {
    unsigned char 	mode;  				//运行模式
    unsigned short 	on_time; 			//开启时间
    unsigned short 	off_time; 			//关闭时间
    unsigned short 	cycle;				//周期
} TJDQ_LV;

//变频风机级别
typedef struct {
    unsigned char 	mode;  				//运行模式
    unsigned char 	degree;				//运行开度
    unsigned short 	on_time; 			//开启时间
    unsigned short 	off_time; 			//关闭时间
    unsigned short 	cycle;				//周期
} TPWM_LV;

typedef struct {
    unsigned char 	enable;  	//是否启用
    signed short 	offset;    	//温度偏差
    unsigned char 	freq; 		//相对应的频率
} TFREQ;

//分区变频风机级别
typedef struct {
    unsigned char 	enable;
    unsigned short 	target_temp; 	//目标温度
    unsigned short 	start_sec; 		//开机时间
    unsigned short 	stop_sec; 		//关机时间
    unsigned char 	min_vol;	  	//最小电压
    unsigned char	bak[2];
    TFREQ 			freq[FREQ_NUM]; //曲线
} TAPWM_LV;

//小窗级别
typedef struct {
//    unsigned char 	mode;  				//运行模式 不使用这个值
    unsigned char 	degree;				//运行开度
} TWIN_LV;

//级别
typedef struct {
    unsigned char 	enable;					    //是否启用 1-使用
    unsigned char 	offset;					    //启动偏差
    signed char 	re_diff;				    //回差温度
    TPWM_LV 		PWM[PWM_NUM_MAX]; 		    //变频风机级别
    TWIN_LV			WIN[WINDOW_NUM_MAX - 2]; 	//风窗1，风窗2
    TWIN_LV 		SWIN[SWINDOW_NUM_MAX]; 	    //小窗
    TJDQ_LV 		JDQ[JDQ_NUM_MAX]; 		    //定速风机
} TLV;

//级别扩展
typedef struct {
    TWIN_LV			WIN[WINDOW_NUM_MAX - 2]; 	//风窗3，风窗4
    unsigned char   bk[20];
} EXLV;

//辅助控制
typedef struct {
    unsigned char 	enable;				//是否启用 1-使用
    signed short 	day_time;			//日龄或者时间(分钟)或者室外温度
    unsigned char 	min_lv;				//最小级别
    unsigned char 	max_lv;				//最大级别
    unsigned char 	cold_lv;			//小冷级别
    unsigned char 	warm_lv;			//小暖级别
    unsigned char 	temp_type;			//温度类型  室内 室外
    signed char 	temp_diff;			//温度差
    signed short 	temp_cold;			//寒冷温度
    signed short 	temp_warm;			//温暖温度
} TSCOPE_CUR;

typedef struct {
    unsigned char 	enable;					//是否启用 1-使用
    TSCOPE_CUR		curve[LV_SCOPE_NUM];
} TSCOPE;

//传感器控制
typedef struct {
    unsigned char 	enable;				//是否启用 1-使用
    unsigned short 	target;				//目标值
    unsigned short 	alaL;				//报警低值
    unsigned short 	alaH;				//报警高值
    unsigned short 	delay_min;			//延迟分钟
    unsigned short 	continue_sec;		//持续秒
    unsigned short 	dead_zone;			//死区 过渡区
    unsigned char 	heat_flag;			//低于加热
    unsigned short 	temp_drop;			//降温幅度
} TSENCON;


typedef struct {
    unsigned char 	enable;				//是否启用 1-使用小窗1 小窗2 风窗 幕帘
    short 			target;				//目标值
    short 			alaL;				//报警低值
    short 			alaH;				//报警高值
    unsigned char 	delay_s;			//延迟秒
    unsigned char 	continue_s;			//持续秒
    unsigned short  dead_zone;			//死区 过渡区
    unsigned short 	adjust_pp;			//调整比例
} TVAC_BAK;

typedef struct {
    unsigned char 	type;				//是否启用 1-使用小窗1 小窗2 风窗 幕帘
    signed char 			targetL;			//低温静压
    signed char 			targetH;			//高温静压
    signed char 			alaL;				//报警低值
    signed char 			alaH;				//报警高值
    signed char  			dead_zone;			//死区 过渡区
    unsigned char 	delay_s;			//阵风延迟秒
    unsigned char 	make_s;				//产生静压秒,这个参数目前和阵风延迟使用同一个就行   
    short			low_tempdiff;		//低温静压目标差值
    short			high_tempdiff;		//高温静压目标差值
    unsigned char	min_ctrllv;			//静压控制最小级别
    unsigned char	max_ctrllv;			//静压控制最大级别
    unsigned char	min_alalv;			//低压报警最小级别
    unsigned char	adj_move_s;			//单次调整时间
    unsigned char	adj_delay_s;		//调整间隔时间
    unsigned char	ala_delay_s;		//报警延迟时间
	unsigned char	bak[22];	   
} TVAC;

typedef struct {
    unsigned char	mode;				//工作模式 0--空栏  1--正常
    unsigned short 	lv_delay;			//级别保持时间
    unsigned short 	fan_delay;			//风机间隔时间
    unsigned short 	fan_lvdelay_bak;	//风机开启延时时间 
    unsigned short 	alarm_delay;		//报警延时时间
    unsigned char	bak1;				//纵向通风起始级别
    unsigned char	bak2;				//低压报警最小级别
    unsigned char	temp_diff;			//温差故障值

    unsigned char   win_link;           //时控模式小窗是否动作
    unsigned char 	food_min;			//料量的最小单位

    unsigned short 	win_pretime;		//小窗提前开启时间 
    unsigned char	win_link_lv;		//小窗随动级别限制

    unsigned char	pwm_fan_defrost_time;//调速风机除霜时间
    unsigned char	swin_defrost_en[10]; //小窗防冻   WINDOW_NUM_MAX+SWINDOW_NUM_MAX
    unsigned char	swin_hold_time;		 //小窗防冻稳定时间 min
    unsigned char	swin_defrost_bak;	 //小窗防冻预留
    signed char		swin_defrost_degree; //小窗防冻开度
    unsigned char	temp_diff_ala;		//温差报警值
    unsigned char	shield_ala_time;	//报警屏蔽时间
	
    unsigned char	x[100];//预留 102
} TVAR;

//湿热曲线
typedef struct {
    unsigned char 	enable;   			//是否启用 1-使用
    unsigned short 	grow_day;     		//日龄
    unsigned char 	target_hum;   		//目标湿度
    unsigned char 	scale; 				//比例系数
    unsigned short 	bak; 				//预留
} THUM_CURVE;

typedef struct {
    unsigned char 	enable;					    //是否启用 1-使用
    unsigned char 	max_temp;					//最大补偿温度
    unsigned char 	start_hum;				    //湿度下限
    unsigned char 	stop_hum;				    //湿度上限
    unsigned char 	min_adj_hum;				//湿度最小调节
    unsigned short	start_time;					//开始时间
    unsigned short	end_time;					//停止时间
    unsigned char 	bak[1];
    THUM_CURVE		x[HUM_CURVE_NUM];
} THUM;

//通风级别帮助参数
typedef struct {
    unsigned char   lv_fan_cycle_en;     //启用风机循环周期
    unsigned short  lv_fan_cycle;        //风机循环周期
    unsigned char   lv_re_diff_en;       //启用固定回差
    unsigned char   lv_re_diff;        	 //固定回差值
    signed char     lv_temp_adj;         //级别偏差修正
    unsigned char   fan_auto_sort;       //风机自动排序
    unsigned char 	bak[57]; 				//预留
} TLV_MORE;


// sizeof(TCTRL) = 1AD7   6871 BYTE
typedef struct {
    unsigned short 	wMagic;
    TLV				lv[CTRL_LV_NUM];
    signed char	    temp_adj;              //温度独立调节值
    TTEMP			temp[TEMP_CURVE_NUM];
    unsigned char	minair_en;
    TMINAIR			minair[MINAIR_NUM];
    TSCOPE			lv_scope[LV_SCOPE_KIND];
    TSENCON			sencon[SENCON_NUM];
    TVAC_BAK		vac_bak;
    TVAR			var;
    EXLV            exlv[CTRL_LV_NUM];     //级别存储扩展
    TEXTEMP			extemp;
    TEXMINAIR		exminair;
    THUM			hum;
    TLV_MORE		lv_more;
	TVAC			vac;
	
    unsigned char	x[10];//预留
} TCTRL;

typedef struct {
    unsigned char 	enable;			//是否启用 1-使用
    unsigned short 	time;	  		//开始时刻,从0点开始的分钟数
    unsigned short 	hold_time; 		//测试持续时间 s
} TALAT;


typedef struct {
    unsigned short 	wMagic;
    unsigned char 	group;			//群组，新建删除所有记录
    unsigned char 	type;			//类型
    unsigned short	room;			//房间号
    unsigned short  grow_day;		//生长天数，每天加1
    unsigned short	survival_bak;	//存活数量
    unsigned short	total_bak;		//动物总量
    unsigned char 	update_mode; 	//0-主动上传 1--被动采集
    unsigned short  update_time;    //上传时间间隔min
    unsigned short  dayupdate_time; //日龄更新时间0点的分钟数 限制最大为23:30

//以下部分不上传协议
    unsigned short 	id;  			//设备ID，物联网使用
    time_t 			group_time;		//建立新群的时候保存时间，日龄计算时使用
    char 			supadmin_pwd[8];//超级管理员密码
    char 			admin_pwd[8];	//管理员密码
    unsigned char   ala_en;			//报警指示灯启用
    TALAT			ala_test;		//继电器测试
    unsigned char   ala_open_mode;	//断开报警模式

    unsigned char   humtemp_en;		//体感温度启用
    unsigned short  wind_area;		//洞口面积
    unsigned short  ss_time;		//屏保时间
    unsigned char   home_style;		//主页风格
    TMB_DATE		comm_info;		//通讯串口信息
    unsigned char   touch_beep;		//触摸声音
    unsigned char   temp_simple_bak;	//温度简化
    unsigned char   language;		//语音类型
    unsigned char   cur_season;		//当前季节

    unsigned int	survival;		//存活数量// 需要设置和保存 协议
    unsigned int	total;			//动物总量

    time_t 			data_time;		 //消耗数据计算时使用
    unsigned short  dataupdate_time; //消耗数据更新时间0点的分钟数 限制最大为23:30

    unsigned char	x[160]; 		//预留 229-69

    TTEMPE  		tempc_pwm_en;
    TTEMPC_PWM		tempc_pwm[TEMPC_NUM];

    unsigned char	heat_en[HEAT_NUM_MAX];	//dev结构体存储空间不够，为了不改变之前老版本的程序配置故增加在这里
    THEAT			heat[HEAT_NUM_MAX][HEAT_NUM];
    unsigned char	pheat_en;	//dev结构体存储空间不够，为了不改变之前老版本的程序配置故增加在这里
    TPHEAT			pheat[HEAT_NUM];

    TTEMPE  		tempc_en;
    TTEMPC			tempc[TEMPC_NUM];
    TAPWM_LV		area_pwm[APWM_NUM_MAX];

} TBASIC;


//产生报警和报警消除记录一次
typedef struct {
    time_t			cur_time;		//当前时间
    signed short 	avg_temp;		//平均温度
    signed short 	heat_temp;				//加热温度
    signed short 	target_temp;			//目标温度
    signed short 	fan_temp;				//纵向温度
    signed short 	avg_hum;		//平均湿度
    unsigned long long	ala;		//报警内容
    unsigned char 	bak[8];		//备用
    unsigned short	crc;			//校验
} THDATA, *PHDATA;//历史数据

typedef struct {
    unsigned short  grow_day;		//生长天数
    time_t			alarm_time;		//报警时间
    unsigned long long	state;			//报警内容，按编码解析
} TALA, *PALA;

typedef struct {
    unsigned short  grow_day;		//生长天数
    time_t		  	time;			//时间   
    short 			min_temp;		//最小温度
    short 			avg_temp;		//平均温度
    short 			max_temp;		//最大温度
    short 			out_temp;		//室外平均温度
    unsigned int 	dead_total;		//死亡总量
    unsigned int	survival;		//动物存量
} TDATA, *PDATA;


//数据记录
typedef struct {
    TALA 			ala;			//报警
    TDATA			data;			//数据
} TRECODE;

//数据存储索引
typedef struct {
    unsigned short 	wMagic;
    unsigned short  ala_id;				//报警记录当前ID
    unsigned char 	ala_state;			//1//数据已经存储一圈
    unsigned short  data_id;			//数据记录当前ID
    unsigned char 	data_state;			//1//数据已经存储一圈

    unsigned short  his_buf_id;		//历史数据缓冲区id,单条数据
    unsigned short  his_id;			//历史数据当前ID 128条数据
    unsigned char 	his_state;		//1//数据已经存储一圈

    unsigned int  	animal[PRO_SAVE_NUM];//动物数量
    unsigned int  	water[PRO_DATA_MAX][PRO_SAVE_NUM];//水量 总计 昨日总计 前日总计 //
    unsigned int  	energy[PRO_DATA_MAX][PRO_SAVE_NUM];//电量 总计 昨日总计 前日总计(总计用的是sta_in)
    unsigned int  	food[PRO_DATA_MAX][PRO_SAVE_NUM];	//料量 总计 昨日总计 前日总计
    unsigned int  	addfood[PRO_DATA_MAX][PRO_SAVE_NUM];	//料量 总计 昨日总计 前日总计
//    unsigned char	x[10];			//预留

//不清除
    unsigned short 	start_time;		//启动次数
    int				run_time;		//运行总时间s
    unsigned short 	restart_time;	//重启次数-故障

    time_t          start_t[DATA_RECORD_MAX];   //启动时间记录  最多100条

    unsigned int  	egg[PRO_DATA_MAX][PRO_SAVE_NUM];	//蛋量 总计 昨日总计 前日总计

    unsigned short  data_pro_id;			//数据记录当前ID
    unsigned char 	data_pro_state;			//1//数据已经存储一圈
//    unsigned char	bak[250];			//预留
} TINDEX;

//消耗数据记录
typedef struct {
    unsigned short  grow_day;		//生长天数
    time_t		  	time;				//时间
    int				survival;		//动物存量
    unsigned int  	water[PRO_DATA_MAX];//水量
    unsigned int  	energy[PRO_DATA_MAX];//电量
    unsigned int  	food[PRO_DATA_MAX];	//料量 
    unsigned int  	egg[PRO_DATA_MAX];	//蛋量
} TUSE_DATA;


//打卡次数存储
typedef struct {
    unsigned short 	wMagic;
    unsigned short  clock_in_id;		//打卡记录当前ID
    unsigned char 	clock_in_state;		//打卡记录已经存储一圈
    unsigned int  	clock_in_time;		//打卡记录总次数   
    unsigned char	bak[50];			//预留
    time_t          clock_in_t[CLOCK_RECORD_MAX];   //启动时间记录  最多100条
} TCLOCK_IN;





typedef struct {
    unsigned short 	wMagic;
    unsigned char	creg_en;		//1表示需要注册 0表示不要注册
    uint64_t 		ala_en;//报警屏蔽
    unsigned char	room_mode;		//0养禽 1养猪
    unsigned char	update_pro;		//0kc 1HL
} TSOFT;

typedef struct {
    unsigned char 	lv_en;				//级别是否启用 1-使用
    unsigned char 	lv;					//级别
    unsigned char 	temp_en;			//温度是否启用 1-使用
    signed short 	temp;				//温度

    unsigned char 	SEN_EN[SENSOR_TYPE_NUM]; //temp 湿度 CO2 NH3 h2s vac 风速
    unsigned short 	SEN[SENSOR_TYPE_NUM]; 	//temp 湿度 CO2 NH3 h2s vac 风速
} TSL;

typedef struct {
    unsigned char 	start_time;
    unsigned char	restart_time;
    unsigned char	run_time;
    unsigned char	reset_en;
    unsigned char	new_group_en;
    unsigned char	reset_group_en;
    unsigned char	work_mode;
    unsigned char	clear_water;
    unsigned char	clear_food;
    unsigned char	trial_en;
    unsigned short	trial_time;
    unsigned char	bak[116];
} TCLEAR;

typedef struct {
    unsigned char 	lcd_state;			//屏幕状态
    time_t 			on_time;			//上电的时间
    unsigned short 	on_hour;			//上电小时数
    int				on_sec;				//上电秒数
    unsigned char 	update_hour;		//日龄改变后温度更新小时
    time_t          update_time;        //日龄改变后当前时间

    unsigned char 	login_type;
    unsigned char 	copy_home_pic;		//拷贝完主界面图片
    unsigned char 	copy_bg_pic;		//拷贝完背景图片


    unsigned char 	net_sta;			//0--未联网 1--联网
    unsigned char 	net_creg;			//0--未注册 1--注册
    unsigned char 	net_ack;			//报文回应成功
    int 			net_cycletime;		//实时数据发送周期时间
    int 			net_sendtime;		//实时数据发送时间
    unsigned char 	net_times;			//重发次数


    unsigned short  delay_rst;			//重启延时

    TSL				sl_set;				//模拟运行设置

    unsigned char	reset_flag;			//重启模拟量的标志
    unsigned char	temp_success;		//采集温度成功

    unsigned char 	temp_err;			//所有温度错误标志
    unsigned char 	auto_mode;			//自动模式

    unsigned char 	energy_num;			//电表个数

    TCLEAR			clear;				//清除数据

    int 			his_cycletime;		//历史数据保存周期时间

    unsigned char 	food_adjut_state;	//校准状态 1-开始校准
    int 			food_adjut_value;   //校准值

	unsigned char   trial_end;			//试用结束
	unsigned char   ac_power_off;		//外部断电

	unsigned char	dev_equ_en[DEV_ITEM_NUM];	//设备显示数据位置
	unsigned char	ctrl_equ_en[CTRL_ITEM_NUM];	//设备显示数据位置
	
} TSYS;


//存储季节相关结构体

typedef struct {
    unsigned short 	wMagic;
    time_t          save_time;        //数据保存时间
    unsigned short  crc16;			  //crc校验
    unsigned char   bak[128];		  //备用
} TSEASON;



#pragma pack(pop)





extern TUP_ST		update_st;
extern TSTA_IN		sta_in;
extern TSTA_OUT		sta_out;
extern TCAL			cal_inf;
extern TSYS			sys_sta;
extern TRUN			run_inf;
extern TRECODE		recode_cur;
extern TUSE_DATA	use_data_cur;
extern THDATA		his_data;
extern TSEASON		season_inf;


extern TDEV			dev_inf;
extern TCTRL		ctrl_inf;
extern TBASIC		basic_inf;
extern TINDEX		index_inf;
extern TEXT			ext_inf;

extern TSOFT		soft_inf;
extern TCLOCK_IN	clock_inf;	//打卡








#endif

