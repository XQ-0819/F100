/*****************************************************************************/
#ifndef __RECODE_H__
#define __RECODE_H__
#include "stm32f4xx.h"

#define 	ALA_TYPE_MAX	(64)
#define		ALA_STA_OFF		(0)
#define		ALA_STA_ON		(1)

//报警类别序号
#define ALA_HTEMP		 		(0) //高温
#define ALA_LTEMP		 		(1) //低温
#define ALA_ETEMP1		 		(2) //传感器1
#define ALA_ETEMP2		 		(3) //传感器2
#define ALA_ETEMP3		 		(4) //传感器3
#define ALA_ETEMP4		 		(5) //传感器4
#define ALA_ETEMP5		 		(6) //传感器5
#define ALA_ETEMP6		 		(7) //传感器6
#define ALA_ETEMP7		 		(8) //传感器7
#define ALA_ETEMP8		 		(9) //传感器8
#define ALA_HHUM		 		(10) //高湿
#define ALA_LHUM		 		(11) //低湿
#define ALA_EHUM		 		(12) //传感器
#define ALA_HCO2		 		(13) //高CO2
#define ALA_LCO2		 		(14) //低CO2
#define ALA_ECO2		 		(15) //传感器
#define ALA_HNH3		 		(16) //高NH3
#define ALA_LNH3		 		(17) //低NH3
#define ALA_ENH3		 		(18) //传感器
#define ALA_HVAC		 		(19) //高VAC
#define ALA_LVAC		 		(20) //低VAC
#define ALA_EVAC		 		(21) //传感器
#define ALA_ELE					(22) //环控电箱
#define ALA_FOOD				(23) //喂料系统
#define ALA_CLEAN				(24) //清粪系统
#define ALA_OTHER				(25) //其它报警
#define ALA_EENERG				(26) //电量通讯
#define ALA_EPM2_5				(27) //pm2.5通讯
#define ALA_EH2S				(28) //H2S
#define ALA_EFOOD				(29) //称重通讯
#define ALA_ELIGHT				(30) //光照
#define ALA_HPM2_5				(31) //pm2.5高
#define ALA_HPM10				(32) //pm10高
#define ALA_ATEMP1		 		(33) //传感器1
#define ALA_ATEMP2		 		(34) //传感器2
#define ALA_ATEMP3		 		(35) //传感器3
#define ALA_ATEMP4		 		(36) //传感器4
#define ALA_ATEMP5		 		(37) //传感器5
#define ALA_ATEMP6		 		(38) //传感器6
#define ALA_ATEMP7		 		(39) //传感器7
#define ALA_ATEMP8		 		(40) //传感器8
#define ALA_EWATER				(41) //水表通讯
#define ALA_ESPEED				(42) //SPEED
#define ALA_AFAN				(43) //风机电流超限
#define ALA_AFAN_BL				(44) //风机电流不平衡
#define ALA_AHTEMP		 		(45) //体感高温
#define ALA_ALTEMP		 		(46) //体感低温
#define ALA_WINPOS		 		(47) //电位计反馈
#define ALA_POWER		 		(48) //断电报警
#define ALA_SWHTEMP		 		(49) //温控开关高温
#define ALA_SWLTEMP		 		(50) //温控开关低温
#define ALA_LEAKAGEW		 	(51) //漏水报警
#define ALA_LACKW		 		(52) //缺水报警


#define ALA_LV					(53) //级别报警

#define ALA_MAX					(ALA_LV+1)

#define ALA_TIME		(10)

typedef struct {
    int time;		//次数
    int sec; 		//秒
} TSALA;
extern TSALA ala_inf[ALA_TYPE_MAX];
extern const u16 alarm_show[ALA_TYPE_MAX];
int ALA_Set_State(u8 type, u8 state);
int ALA_Get_State(u8 type);
uint64_t ALA_State(void);
void Update_ALA(void);
void Updata_Data(void);
void Update_Pro_Data(void);
void Update_Cal_Data(void);
void Cal_Temp_Def(void);

#endif 




