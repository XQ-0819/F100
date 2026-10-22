#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "stm32f4xx.h"


#define FUTURE100 		(5)

#define SYSTEM_TYPE 	FUTURE100 //K9P
#define SOFTWARE_VER 	(SYS_VER)


#define SEND_TIME_OUT 	(4000)
#define SEND_TIMES   	(1)
#define HEART_TIME		(18)


#define SRART_BUF_LEN   (8)
#define END_BUF_LEN   	(5)


#define Success_Result 	(0)
#define Failed_Result 	(0xff)

#define TEMP_DIS 	(0)
#define TEMP_IN 	(1)
#define TEMP_OUT 	(2)
#define TEMP_WATER 	(3)
#define TEMP_HUMAN 	(4)
#define TEMP_TAR_HAND 	(5)
#define TEMP_TAR_HUM 	(6)
#define TEMP_LV_HAND 	(7)
#define TEMP_LV_OUT 	(8)


#define CLOCK_IN_SUCCESS 	(100)


#define MAX_REPORT_TEMP (16)
#define MAX_REPORT_HUM 	(8)
#define MAX_REPORT_CO2 	(8)
#define MAX_REPORT_NH3 	(8)
#define MAX_REPORT_H2S 	(4)
#define MAX_REPORT_VAC 	(4)
#define MAX_REPORT_LIGHT (4)
#define MAX_REPORT_PWM 	(8)
#define MAX_REPORT_JDQ 	(20)

#define JDQ_STA_FAN		(2)
#define JDQ_STA_WET		(10)
#define JDQ_STA_SPRAY	(11)
#define JDQ_STA_CIRCLE	(13)
#define JDQ_STA_TIMER	(12)
#define JDQ_STA_HEAT	(1)
#define JDQ_STA_TEMPC	(23)
//#define JDQ_STA_WIN		(5)
#define JDQ_STA_ALA		(9)
#define JDQ_STA_LIGHT	(26)
#define JDQ_STA_WATER	(27)
#define JDQ_STA_FEED	(20)

#define PWM_STA_FAN		(1)
#define PWM_STA_SWIN	(2)
#define PWM_STA_HEAT	(3)
#define PWM_STA_LIGHT	(4)
#define PWM_STA_AFAN	(8)
#define PWM_STA_TEMPC	(10)
#define JDQ_STA_WIN_F100		(14)
#define JDQ_STA_WIN2_F100		(16)


#define SET_TIME_INFO		(0X03)
#define HEART_BEAT_DATA		(0X91)

#define Read_Current_Data   (0x05)
#define SET_BASIC_INFO		(0X06)
#define READ_BASIC_INFO		(0X07)
#define SET_TEMP_INFO		(0X08)
#define READ_TEMP_INFO		(0X09)
#define SET_JDQ_INFO		(0X0A)
#define READ_JDQ_INFO		(0X0B)
#define SET_PWM_INFO		(0X0C)
#define READ_PWM_INFO		(0X0D)

//V2.0)
#define SET_TEMP_V2_INFO	(0X0E)
#define READ_TEMP_V2_INFO	(0X0F)
#define SET_MINAIR_V2_INFO	(0X10)
#define READ_MINAIR_V2_INFO	(0X11)
#define SET_EXMINAIR_V2_INFO	(0X7F)
#define READ_EXMINAIR_V2_INFO	(0X90)
#define SET_LV_V2_INFO		(0X12)
#define READ_LV_V2_INFO		(0X13)
#define READ_LVAIR_V2_INFO	(0X14)
#define SET_SENSOR_V2_INFO	(0X15)
#define READ_SENSOR_V2_INFO	(0X16)
#define SET_PWM_V2_INFO		(0X17)
#define READ_PWM_V2_INFO	(0X18)
#define SET_JDQ_V2_INFO		(0X19)
#define READ_JDQ_V2_INFO	(0X1A)
#define SET_WATER_V2_INFO	(0X1B)
#define READ_WATER_V2_INFO	(0X1C)
#define SET_SPRAY_V2_INFO	(0X1D)
#define READ_SPRAY_V2_INFO	(0X1E)
#define SET_LIGHT_V2_INFO	(0X1F)
#define READ_LIGHT_V2_INFO	(0X20)
#define SET_HEAT_V2_INFO	(0X21)
#define READ_HEAT_V2_INFO	(0X22)
//#define SET_WIN_V2_INFO		(0X23)
//#define READ_WIN_V2_INFO	(0X24)
//#define SET_WIN_V2_INFO		(0X72)
//#define READ_WIN_V2_INFO	(0X73)
#define SET_WIN_V2_INFO		(0XD6)
#define READ_WIN_V2_INFO	(0XD7)
//#define SET_SG_V2_INFO		(0X25)
//#define READ_SG_V2_INFO		(0X26)
//#define SET_SG_V2_INFO		(0X4C)
//#define READ_SG_V2_INFO		(0X4D)
#define SET_SG_V2_INFO		(0X88)
#define READ_SG_V2_INFO		(0X89)
#define SET_CIRCLE_V2_INFO	(0X28)
#define READ_CIRCLE_V2_INFO	(0X29)
//#define SET_TIMER_V2_INFO	(0X2A)
//#define READ_TIMER_V2_INFO	(0X2B)
//#define SET_TIMER_V2_INFO	(0X4A)
//#define READ_TIMER_V2_INFO	(0X4B)//东方希望定义
#define SET_TIMER_V2_INFO	(0XD4)
#define READ_TIMER_V2_INFO	(0XD5)
#define SET_SYS_V2_INFO		(0X2C)
#define READ_SYS_V2_INFO	(0X2D)
#define SET_ASLV_V2_INFO	(0X33)
#define READ_ASLV_V2_INFO	(0X34)
#define SET_OTSEN_V2_INFO	(0X35)
#define READ_OTSEN_V2_INFO	(0X36)
//#define SET_VAC_V2_INFO		(0X37)
//#define READ_VAC_V2_INFO	(0X38)
#define SET_VAC_V2_INFO		(0XDC)
#define READ_VAC_V2_INFO	(0XDD)
#define SET_ENV_V2_INFO		(0X41)
#define READ_ENV_V2_INFO	(0X42)
#define SET_VAR_V2_INFO		(0X44)
#define READ_VAR_V2_INFO	(0X45)
#define SET_PWD_V2_INFO		(0X48)
#define READ_PWD_V2_INFO	(0X49)

#define SET_PHEAT_V2_INFO	(0X62)
#define READ_PHEAT_V2_INFO	(0X63)
#define CLEAR_DATA_INFO		(0X64)
#define SET_TEMPC_V2_INFO	(0X65)
#define READ_TEMPC_V2_INFO	(0X66)

#define SET_TEMPC_PWM_V2_INFO	(0XBC)
#define READ_TEMPC_PWM_V2_INFO	(0XBD)


#define SET_APWM_V2_INFO	(0X6B)
#define READ_APWM_V2_INFO	(0X6C)

#define SET_FAN_CUR_V2_INFO	    (0X6D)
#define READ_FAN_CUR_V2_INFO	(0X6E)

#define READ_OPEN_RECODE_INFO	(0X7C)
#define READ_WIN_DEGREE_INFO	(0XDE)

#define SET_TEMP_DEF_V2_INFO	    (0XF8)
#define READ_TEMP_DEF_V2_INFO	(0XF9)

#define SET_UPDATE_ST		(0XE0)
#define SET_UPDATE_FILE		(0XE1)
#define SET_UPDATE_CHG		(0XE2)


#define Report_Current_Data_V2  (0xA0)
#define Read_Current_Data_V2   	(0x27)
#define Report_Creg_Data_V2		(0XB0)

#define Report_Max_Buffer_Size  (1024)


//V2.1

#define SET_LV_V2_1_INFO    (0X78)
#define READ_LV_V2_1_INFO	(0X79)

#define SET_TEMP_V2_1_INFO    (0X7D)
#define READ_TEMP_V2_1_INFO	  (0X7E)
#define SET_MINAIR_V2_1_INFO  (0X7F)
#define READ_MINAIR_V2_1_INFO (0X90)

#define SET_HUM_V2_INFO		(0X92)
#define READ_HUM_V2_INFO	(0X93)

#define SET_LVMORE_V2_INFO	(0X98)
#define READ_LVMORE_V2_INFO	(0X99)

#define SET_DAYTEMP_V2_INFO		(0XFD)
#define READ_DAYTEMP_V2_INFO	(0XFE)

//modbus

#define modbus_function_Read_Holding_Registers 3
#define modbus_function_Read_Keep_Registers 4


#define modbus_function_Read_Holding_Registers_data_lenth 6
#define modbus_function_Preset_Single_Register 6
#define modbus_function_Preset_Single_Register_data_lenth 6

#define modbus_function_Preset_Multiple_Registers 16


#define SYSTEM_TYPE_ADDR 1
#define SOFTWARE_VER_ADDR 2
#define HARDWARE_VER_ADDR 3
#define LOGIN_TYPE_ADDR 4
#define ROOM_TYPE_ADDR 5
#define ROOM_ADDR 6
#define TEMP_ADDR 7
#define SET_TARGET_TEMP_ADDR 8
#define HUM_ADDR 9
#define TARGET_HUM_ADDR 10
#define GROW_DAY_ADDR 11
#define OUT_TEMP_ADDR 12

#define TEMPA_ADDR 13
#define TEMPB_ADDR 14
#define TEMPC_ADDR 15

#define HEAT_TEMP_ADDR 16
#define TARGET_TEMP_ADDR 17
#define FAN_TEMP_ADDR 18


#define MAX_TEMP_ADDR 19
#define MIN_TEMP_ADDR 20
#define MAX_HUM_ADDR 21
#define MIN_HUM_ADDR 22

#define CUR_CO2_ADDR 23
#define SET_CO2_ADDR 24
#define MAX_CO2_ADDR 25
#define MIN_CO2_ADDR 26

#define CUR_NH3_ADDR 27
#define SET_NH3_ADDR 28
#define MAX_NH3_ADDR 29
#define MIN_NH3_ADDR 30

#define CUR_H2S_ADDR 31
#define SET_H2S_ADDR 32
#define MAX_H2S_ADDR 33
#define MIN_H2S_ADDR 34

#define CUR_VAC_ADDR 35
#define SET_VAC_ADDR 36
#define MAX_VAC_ADDR 37
#define MIN_VAC_ADDR 38

#define MIN_AIR_ADDR MIN_VAC_ADDR+1
#define MAX_LV_ADDR MIN_AIR_ADDR+2
#define MIN_LV_ADDR MAX_LV_ADDR+1
#define RUN_LV_ADDR MIN_LV_ADDR+1
#define RUN_AIR_ADDR RUN_LV_ADDR+1
#define ANIMAL_DEAD_ADDR RUN_AIR_ADDR+2
#define ANIMAL_SERIVAL_ADDR ANIMAL_DEAD_ADDR+1

#define FEED_ADDR  ANIMAL_SERIVAL_ADDR+1
#define POWER_ADDR  FEED_ADDR+2
#define WATER_ADDR  POWER_ADDR+2
#define SCRP_ADDR  WATER_ADDR+2

#define SBK_ADDR  SCRP_ADDR+2
#define SBK_ADDR_LEN  5

#define ALL_TEMP_ADDR  100
#define ALL_TEMP_ADDR_LEN 32



#define ALL_HUM_ADDR ALL_TEMP_ADDR+ALL_TEMP_ADDR_LEN
#define ALL_HUM_ADDR_LEN 16

#define ALL_CO2_ADDR ALL_HUM_ADDR+ALL_HUM_ADDR_LEN
#define ALL_CO2_ADDR_LEN 24

#define ALL_H2S_ADDR ALL_CO2_ADDR+ALL_CO2_ADDR_LEN
#define ALL_H2S_ADDR_LEN 12

#define ALL_VAC_ADDR ALL_H2S_ADDR+ALL_H2S_ADDR_LEN
#define ALL_VAC_ADDR_LEN 12



#define ALL_PWM_ADDR 500
#define ALL_PWM_ADDR_LEN 16


#define ALL_JDQ_ADDR 600
#define ALL_JDQ_ADDR_LEN 40



#define ALL_ALARM_ADDR 1000
#define ALL_ALARM_ADDR_LEN 10




extern unsigned char Current_Data_Buf[];
extern unsigned char Data_Send_Buf[Report_Max_Buffer_Size];//发送处理程序使用
extern unsigned short Data_Send_Num;//发送个数
extern unsigned char set_result;
extern unsigned short set_seq;

#pragma pack(push,1)




typedef struct {
    unsigned char Head1;
    unsigned char Head2;
    unsigned char Commend;
    unsigned short Equipmend_ID;
    unsigned short Send_Seq;
    unsigned char Length;
    unsigned char Result;
    unsigned short Crc;
    unsigned char Tail1;
    unsigned char Tail2;

} Packet_Head;


typedef struct {
    unsigned char  year;
    unsigned char  month;
    unsigned char  day;
    unsigned char  hour;
    unsigned char  min;
} SYSTEM_TIME;


typedef struct {
    unsigned char type;
    signed short value;

} SYSTEM_TEMP;

typedef struct {
    unsigned char type;
    signed short value;

} SYSTEM_LIGHT;

typedef struct {
    unsigned char type;
    unsigned char value;

} SYSTEM_HUM;

typedef struct {
    unsigned char type;
    unsigned short value;

} SYSTEM_SENSORS;

typedef struct {
    unsigned char type;
    short value;

} SYSTEM_PRESS;


typedef struct  SYSTEM_PWM {
    unsigned char type;
	unsigned short time;
    unsigned char value;
} SYSTEM_PWM;

typedef struct {
    unsigned char type;
	unsigned short time;
    unsigned char value;

} SYSTEM_JDQ;



typedef struct {
    SYSTEM_TIME systime;

    unsigned char device_type;
    unsigned short Software_VER;
    unsigned char work_mode;
	
    struct {
        unsigned char	id: 4;				//外部报警类型
        unsigned char	num: 4;				//外部报警
    } bak_x;	

    unsigned char login_type;
    unsigned char type;
    unsigned short room;


    signed short 	temp;		//平均温度
    signed short 	set_temp;		//设置温度
    signed char 	hum;		//平均温度
    signed char 	target_hum;		//设置温度
    unsigned short  grow_day;		//生长天数
    signed short 	out_temp;		//室外平均温度
    signed short 	tempA;					//平均温度A
    signed short 	tempB;					//平均温度B
    signed short 	tempC;					//平均温度C
    signed short 	heat_temp; 			//加热温度
    signed short 	target_temp; 		//目标温度
    signed short 	fan_temp; 		//纵向温度

    signed short 	max_temp; 			//高温报警
    signed short 	min_temp; 			//低温报警
    signed char 	max_hum; 			//高温报警
    signed char 	min_hum; 			//低温报警
    signed short 	cur_CO2; 			//
    signed short 	target_CO2; 			//
    signed short 	max_CO2; 			//高温报警
    signed short 	min_CO2; 			//低温报警
    signed short 	cur_NH3; 			//
    signed short 	target_NH3; 			//
    signed short 	max_NH3; 			//高温报警
    signed short 	min_NH3; 			//低温报警
    signed short 	cur_H2S; 			//
    signed short 	target_H2S; 			//
    signed short 	max_H2S; 			//高温报警
    signed short 	min_H2S; 			//低温报警
    signed short 	cur_VAC; 			//
    signed short 	target_VAC; 			//
    signed short 	max_VAC; 			//负压报警
    signed short 	min_VAC; 			//负压报警

	unsigned short 	cur_light; 			//光照
    unsigned short 	target_light; 			//
    unsigned short 	max_light; 			//
    unsigned short 	min_light; 			//

    unsigned int min_air;  //最小通风量
    unsigned char max_lv;    //最大运行级别
    unsigned char min_lv;    //最小运行级别
    unsigned char lv;        //当前运行级别
    unsigned int lv_air;   //当前级别通风量

    unsigned short 	dead_total;		//死亡总量
    unsigned short	survival_bak;		//动物存量


    unsigned int food;  //喂食
    unsigned int power; //用电量
    unsigned int water; //计水量
    unsigned int scrap; //刮粪
    unsigned int run_time; //运行时间
    unsigned int residue_food;
//    unsigned short	start_time;//开机次数
//    unsigned short	restart_time;//重启次数
    unsigned char auto_control_state;
	unsigned char group_num;
	unsigned char air_mode;
	unsigned char air_alarm;
//	unsigned short heat_water;
//	unsigned short cold_water;
    unsigned int	survival;		//动物存量
	
    unsigned short voltage[3];	//三相电压
    unsigned short current[3];	//三相电流	


    SYSTEM_TEMP TEMP[MAX_REPORT_TEMP];
    SYSTEM_HUM  HUM[MAX_REPORT_HUM];
    SYSTEM_SENSORS CO2[MAX_REPORT_CO2];
    SYSTEM_SENSORS NH3[MAX_REPORT_NH3];
    SYSTEM_SENSORS H2S[MAX_REPORT_H2S];
    SYSTEM_PRESS   VAC[MAX_REPORT_VAC];
	SYSTEM_SENSORS   LIGHT[MAX_REPORT_LIGHT];


    SYSTEM_PWM PWM[MAX_REPORT_PWM];
    SYSTEM_JDQ JDQ[MAX_REPORT_JDQ];

    unsigned char ALAM_sys1;
	
    unsigned char ALAM_sys2;
	
    unsigned short ALAM_temp;
    unsigned char ALAM_hum;
    unsigned char ALAM_CO2;
    unsigned char ALAM_NH3;
    unsigned char ALAM_VAC;	
    unsigned char ALAM_H2S;
	unsigned char ALAM_LIGHT;  //
	unsigned char ALAM_OTHER;    //
    unsigned char ALAM_OUT;
    unsigned short ALAM_BK;

} SYSTEM_REPORT;

#define PWD_LEN		(6)
typedef struct {
    unsigned char pwd1_type;
    unsigned char pwd1[PWD_LEN];
    unsigned char pwd2_type;
    unsigned char pwd2[PWD_LEN];
} TPWD,*PPWD;





#pragma pack(pop)


extern  SYSTEM_REPORT report;



extern  Packet_Head  Packet_Buffer;
void Report_Data_Buffer(unsigned char* Enter_Send_Buffer, unsigned short Packet_Len, unsigned char Report_Commond);
void Read_Set_Cmd(unsigned char *Ethernet_Recv_Buf, unsigned short Ethernet_Recv_len);
void Packet_Init(void);
void Report_Data_Init(unsigned char update_sta);

void Comm_Rec_Task(void *pdata);
void Update_Task(void * pdtat);

#endif



