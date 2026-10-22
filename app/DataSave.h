/*****************************************************************************/
#ifndef __DATASAVE_H__
#define __DATASAVE_H__
#include "stm32f4xx.h"

#ifndef OFFSETOF
#define OFFSETOF(TYPE,member) ((size_t) &((TYPE *)0)->member)
#endif

#define EE_EETEST_ADDR		(0)		 		// 2	
#define EE_CAL_ADDR			(10)			// 28		

#define EE_BASIC_ADDR		(40)			// 21		max960
#define EE_DEV_ADDR			(0x0400)  		// 454		max1024
#define EE_CTRL_ADDR		(0x0800)  		// 			max7168
#define EE_EXT_ADDR			(0x2400)  		// 			max1024
//以下是数据区
#define EE_INDEX_ADDR		(0x2800)		// 5		max1024

#define EE_ALA_ADDR			(0x3000)
#define EE_DATA_ADDR		(0x3000 + sizeof(TALA)*ALA_RECORD_MAX)
#define EE_USE_ADDR			(EE_DATA_ADDR + sizeof(TDATA)*DATA_RECORD_MAX)
#define EE_MAX_ADDR			(EE_USE_ADDR + sizeof(TUSE_DATA)*DATA_PRO_RECORD_MAX)

#define EE_CLOCK_ADDR		(0x6800)// 512	
#define EE_TRIAL_ADDR		(0x6A00)// 256	
#define EE_SOFT_ADDR		(0x6B00)// 256	
#define EE_HIS_ADDR			(0x6C00)// 历史数据缓存区 4K 27K+4K


#define HIS_BUF_NUM			(4096/sizeof(THDATA))
#define HIS_NUM				(675)//1440*32*60/4096
#define HIS_FILE_NUM		(25)//1440*32*60/4096


#define RECODE_ALA			(0)
#define RECODE_DATA			(1)
#define RECODE_USE_DATA		(2)


//功能复用的接口
#define IM_PWM3_PHEAT			(3)



#define IM_WET_SPARY			(0)
#define IM_SPARY_WET			(1)

#define IM_CIRCLE_TIMER1			(0)
#define IM_TIMER1_CIRCLE			(1)
#define IM_TEMPC_TIMER2				(0)
#define IM_TIMER2_TEMPC				(1)
#define IM_HEAT1_TIMER3				(0)
#define IM_TIMER3_HEAT1				(1)
#define IM_HEAT2_TIMER4				(0)
#define IM_TIMER4_HEAT2				(1)


void IM_CHK(void);
void PWM_SWIN_CHK(u8 id);
void PWM3_PHEAT_CHK(u8 id);
void PWM_APWM_CHK(u8 id);
void Light_SWIN_CHK(u8 id);
void Tempc_SWIN_CHK(u8 id);
void H2S_SPEED_CHK(u8 id);
void JDQ_Mul_CHK(void);
int Get_Fan_Mul(u8 id, u8 set_off);
void JDQ_MulFan_CHK(void);
void Wet_Spary_Mul(u8 sid);
void JDQ_MulOther_CHK(u8 sid);
void Circle_Timer1_Mul(u8 sid);
void Tempc_Timer2_Mul(u8 sid);
void Heat1_Timer3_Mul(u8 sid);
void Heat2_Timer4_Mul(u8 sid);


INT8U EEPEOM_ALL_TEST(void);
int Update_SaveData( u8 type, int seq, int len, INT8U * buf);
int	EEPROM_SaveData(INT8U sid);
int	Load_Param(void);
void Param_Init(void);
int Read_Recode_Data(u8 *buf, u8 tpye, u16 id);
int Write_Recode_Data(u8 *buf, u8 tpye, u16 id);
void Recode_Def(void);
void Group_Def(void);
void SetInf_Def(void);
int SetInf_USB_OUT(void);
int SetInf_USB_IN(void);
int Data_USB_OUT(void);
int His_USB_OUT(void);
int his_data_save(void);
int Read_SeasonPara_Time(unsigned char num);
int Read_SeasonPara(unsigned char num);
int Write_SeasonPara(unsigned char num);
int	lv_save_data(INT8U lv);
int room_type_egg_en(void);
void clock_in_data_save(void);
void AIN_WIN_CHK(u8 id);

#endif
