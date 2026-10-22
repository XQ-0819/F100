#include "din.h"
#include <string.h>
#include "Parameters.h"
#include "ucos_ii.h"
#include "common.h"


#define DIN_MAX		(7)
#define DIN_TIME	(4)//4次检测一样才判断为真

#define PLUSE_TIME	(20)//30次检测 脉冲大约一秒钟
#define PLUSE_W_L	(10)//脉冲最低 1ms
#define PLUSE_W_H	(30)//脉冲最低 3ms
#define PLUSE_EGG	(6)//计蛋脉冲位置


#if USE_GD == 1
#define Q7 (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_3))

#define SL_HIGH  GPIO_SetBits(GPIOC, GPIO_Pin_8)
#define SL_LOW   GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define CLK_HIGH  GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define CLK_LOW   GPIO_ResetBits(GPIOC, GPIO_Pin_9)
#endif


typedef struct {
    unsigned char input[DIN_MAX];
    unsigned char state[DIN_MAX];
    unsigned char open_time[DIN_MAX];
    unsigned char close_time[DIN_MAX];
    int 		  pluse_time[DIN_MAX];
    int 		  pluse[DIN_MAX];
    int 		  pluse_w[DIN_MAX];
} DIN_STRUCT;

#define DIN_CLOSE 	0

#define DIN1		PHin(12)
#define DIN2		PHin(11)
#define DIN3		PAin(4)
#define DIN4		PAin(5)
#define DIN5		PAin(7)
#define DIN6		PAin(0)
#define DIN7		PAin(1)

DIN_STRUCT din_inf;


void Pluse_Num_Timer(void)//100us
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能

    //定时器TIM2初始化
    TIM_TimeBaseStructure.TIM_Period = (100 - 1); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = (120 - 1); //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE ); //使能指定的TIM2中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
    TIM_Cmd(TIM2, ENABLE);  //使能TIM2

}



#if USE_GD == 1
void DIN_Init(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    NVIC_InitTypeDef   	NVIC_InitStructure;
    EXTI_InitTypeDef   	EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;    //13是q7 就是读取信息的引脚，14是SL也叫PL，当该引脚下拉一次的话，那么就将当前引脚的状态读取进入165芯片的寄存器中，等待倍读取
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;													//15是CLK  当检测到该引脚处于上升沿的时候吧就将所有的引脚信息左移一位
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;    //13是q7 就是读取信息的引脚，14是SL也叫PL，当该引脚下拉一次的话，那么就将当前引脚的状态读取进入165芯片的寄存器中，等待倍读取
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;													//15是CLK  当检测到该引脚处于上升沿的时候吧就将所有的引脚信息左移一位
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    memset( (unsigned char* )&din_inf, 0, sizeof(din_inf) );
	
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource12);
    EXTI_InitStructure.EXTI_Line = EXTI_Line12;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	Pluse_Num_Timer();
}
#else
void DIN_Init(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;
    NVIC_InitTypeDef   	NVIC_InitStructure;
    EXTI_InitTypeDef   	EXTI_InitStructure;


    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOH, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOH, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    memset( (unsigned char* )&din_inf, 0, sizeof(din_inf) );


    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	Pluse_Num_Timer();
}
#endif

void plue_width_cal(void)
{
    if (din_inf.pluse_w[PLUSE_EGG] != 0 && din_inf.pluse_w[PLUSE_EGG] <= PLUSE_W_H + 1) {
        din_inf.pluse_w[PLUSE_EGG]++;
    }
}

#if USE_GD == 1

void EXTI15_10IRQHandler(void)
{
#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif

    if (EXTI_GetITStatus(EXTI_Line12) != RESET ) {
        if (ext_inf.din.egg_en[0] == STA_ON) {
            if (PDin(12) == DIN_CLOSE) { //上升沿
                din_inf.pluse_w[PLUSE_EGG] = 1;
            } else { //下降沿
                if (din_inf.pluse_w[PLUSE_EGG] >= PLUSE_W_L && din_inf.pluse_w[PLUSE_EGG] <= PLUSE_W_H) {
                    index_inf.egg[0][PRO_DATA_ALL]++;
                }

                din_inf.pluse_w[PLUSE_EGG] = 0;
            }
        }

        EXTI_ClearITPendingBit(EXTI_Line12);
    }

#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}

#else


void EXTI1_IRQHandler(void)
{
#ifdef OS_TICKS_PER_SEC
    OSIntEnter();
#endif

    if (EXTI_GetITStatus(EXTI_Line1) != RESET ) {
        if (ext_inf.din.egg_en[0] == STA_ON) {
            if (DIN7 == DIN_CLOSE) { //上升沿
                din_inf.pluse_w[PLUSE_EGG] = 1;
            } else { //下降沿
                if (din_inf.pluse_w[PLUSE_EGG] >= PLUSE_W_L && din_inf.pluse_w[PLUSE_EGG] <= PLUSE_W_H) {
                    index_inf.egg[0][PRO_DATA_ALL]++;
                }

                din_inf.pluse_w[PLUSE_EGG] = 0;
            }
        }

        EXTI_ClearITPendingBit(EXTI_Line1);
    }

#ifdef OS_TICKS_PER_SEC
    OSIntExit();
#endif
}
#endif


#if USE_GD == 1
u8 HC165_DIN_Get(void)
{
	s8 i;
	u8 ret = 0;

    SL_LOW;
	delay_us(10);
	SL_HIGH;	

	for(i = 8;i > 0;i--) {

		CLK_LOW;
		delay_us(10);
        if(Q7 == 1) {
		    setbit(ret, (i-1));
        }
		CLK_HIGH;
		delay_us(10);
	}

	return ret;

}
#endif
void DIN_Proc(void)
{
#if USE_GD == 1
    u8 i = 0,ret;
	ret = HC165_DIN_Get();
	
    din_inf.input[0] = getbit(ret, 0);
    din_inf.input[1] = getbit(ret, 1);
    din_inf.input[2] = getbit(ret, 2);
    din_inf.input[3] = getbit(ret, 3);
    din_inf.input[4] = getbit(ret, 4);
    din_inf.input[5] = getbit(ret, 5);
    din_inf.input[6] = PDin(12);
#else	
    u8 i = 0;

    din_inf.input[0] = DIN1;
    din_inf.input[1] = DIN2;
    din_inf.input[2] = DIN3;
    din_inf.input[3] = DIN4;
    din_inf.input[4] = DIN5;
    din_inf.input[5] = DIN6;
    din_inf.input[6] = DIN7;
#endif


    for (i = 0; i < DIN_MAX; i++) {
        if (din_inf.input[i] == DIN_CLOSE) {
            din_inf.close_time[i]++;

            if (din_inf.close_time[i] > DIN_TIME) {
                din_inf.close_time[i] = DIN_TIME;
                din_inf.open_time[i] = 0;
                din_inf.state[i] = DIN_ON;
            }
        } else {
            din_inf.open_time[i]++;

            if (din_inf.open_time[i] > DIN_TIME) {
                din_inf.open_time[i] = DIN_TIME;
                din_inf.close_time[i] = 0;
                din_inf.state[i] = DIN_OFF;
            }
        }
    }

    for (i = 0; i < DIN_MAX; i++) {//脉冲
        if (din_inf.state[i] == DIN_ON) {
            din_inf.pluse_time[i]++;
        } else {
            if (din_inf.pluse_time[i] > PLUSE_TIME) {
                din_inf.pluse[i]++;
            }

            din_inf.pluse_time[i] = 0;
        }
    }
}

int Get_DIN_State(u8 sid)
{
    if (sid > DIN_MAX)
        return -1;

    return din_inf.state[sid];
}
int Get_Pluse_Value(u8 sid)
{
    if (sid > DIN_MAX)
        return -1;

    return din_inf.pluse[sid];
}

void Clear_Pluse_Value(u8 sid)
{

    if (sid > DIN_MAX)
        return ;

    din_inf.pluse[sid] = 0;
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) { //检查指定的TIM中断发生与否:TIM 中断源
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源
        
		plue_width_cal();
    }
}


