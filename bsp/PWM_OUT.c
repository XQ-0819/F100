#include "pwm_out.h"
#include "parameters.h"

//500Hz输出频率

// H10	I5	A6	H6	F8	A2
#define OUT_FREQ 		(1000)

#if USE_GD == 1

#define DAC1_TIM                   TIM4
#define DAC1_TIM_CLK               RCC_APB1Periph_TIM4
#define DAC1_TIM_CLK_INIT          RCC_APB1PeriphClockCmd
#define DAC1_TIM_SOURCE            GPIO_PinSource6
#define DAC1_TIM_AF                GPIO_AF_TIM4
#define DAC1_GPIO_PIN              GPIO_Pin_6
#define DAC1_GPIO_PORT             GPIOB
#define DAC1_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define DAC1_TIM_OCInit			   TIM_OC1Init
#define DAC1_TIM_OCPreloadConfig   TIM_OC1PreloadConfig


#define DAC2_TIM                   TIM10
#define DAC2_TIM_CLK               RCC_APB2Periph_TIM10
#define DAC2_TIM_CLK_INIT          RCC_APB2PeriphClockCmd
#define DAC2_TIM_SOURCE            GPIO_PinSource6
#define DAC2_TIM_AF                GPIO_AF_TIM10
#define DAC2_GPIO_PIN              GPIO_Pin_6
#define DAC2_GPIO_PORT             GPIOF
#define DAC2_GPIO_CLK              RCC_AHB1Periph_GPIOF
#define DAC2_TIM_OCInit			   TIM_OC1Init
#define DAC2_TIM_OCPreloadConfig   TIM_OC1PreloadConfig


#define DAC3_TIM                   TIM2
#define DAC3_TIM_CLK               RCC_APB1Periph_TIM2
#define DAC3_TIM_CLK_INIT          RCC_APB1PeriphClockCmd
#define DAC3_TIM_SOURCE            GPIO_PinSource2
#define DAC3_TIM_AF                GPIO_AF_TIM2
#define DAC3_GPIO_PIN              GPIO_Pin_2
#define DAC3_GPIO_PORT             GPIOA
#define DAC3_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define DAC3_TIM_OCInit			   TIM_OC3Init
#define DAC3_TIM_OCPreloadConfig   TIM_OC3PreloadConfig


#define DAC4_TIM                   TIM2
#define DAC4_TIM_CLK               RCC_APB1Periph_TIM2
#define DAC4_TIM_CLK_INIT          RCC_APB1PeriphClockCmd
#define DAC4_TIM_SOURCE            GPIO_PinSource5
#define DAC4_TIM_AF                GPIO_AF_TIM2
#define DAC4_GPIO_PIN              GPIO_Pin_5
#define DAC4_GPIO_PORT             GPIOA
#define DAC4_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define DAC4_TIM_OCInit			   TIM_OC1Init
#define DAC4_TIM_OCPreloadConfig   TIM_OC1PreloadConfig


#define DAC5_TIM                   TIM3
#define DAC5_TIM_CLK               RCC_APB1Periph_TIM3
#define DAC5_TIM_CLK_INIT          RCC_APB1PeriphClockCmd
#define DAC5_TIM_SOURCE            GPIO_PinSource7
#define DAC5_TIM_AF                GPIO_AF_TIM3
#define DAC5_GPIO_PIN              GPIO_Pin_7
#define DAC5_GPIO_PORT             GPIOA
#define DAC5_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define DAC5_TIM_OCInit			   TIM_OC2Init
#define DAC5_TIM_OCPreloadConfig   TIM_OC2PreloadConfig


#define DAC6_TIM                   TIM3
#define DAC6_TIM_CLK               RCC_APB1Periph_TIM3
#define DAC6_TIM_CLK_INIT          RCC_APB1PeriphClockCmd
#define DAC6_TIM_SOURCE            GPIO_PinSource1
#define DAC6_TIM_AF                GPIO_AF_TIM3
#define DAC6_GPIO_PIN              GPIO_Pin_1
#define DAC6_GPIO_PORT             GPIOB
#define DAC6_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define DAC6_TIM_OCInit			   TIM_OC4Init
#define DAC6_TIM_OCPreloadConfig   TIM_OC4PreloadConfig

#else	

#define DAC1_TIM                   TIM5
#define DAC1_TIM_CLK               RCC_APB1Periph_TIM5
#define DAC1_TIM_CLK_INIT          RCC_APB1PeriphClockCmd
#define DAC1_TIM_SOURCE            GPIO_PinSource10
#define DAC1_TIM_AF                GPIO_AF_TIM5
#define DAC1_GPIO_PIN              GPIO_Pin_10
#define DAC1_GPIO_PORT             GPIOH
#define DAC1_GPIO_CLK              RCC_AHB1Periph_GPIOH
#define DAC1_TIM_OCInit			   TIM_OC1Init
#define DAC1_TIM_OCPreloadConfig   TIM_OC1PreloadConfig


#define DAC2_TIM                   TIM8
#define DAC2_TIM_CLK               RCC_APB2Periph_TIM8
#define DAC2_TIM_CLK_INIT          RCC_APB2PeriphClockCmd
#define DAC2_TIM_SOURCE            GPIO_PinSource5
#define DAC2_TIM_AF                GPIO_AF_TIM8
#define DAC2_GPIO_PIN              GPIO_Pin_5
#define DAC2_GPIO_PORT             GPIOI
#define DAC2_GPIO_CLK              RCC_AHB1Periph_GPIOI
#define DAC2_TIM_OCInit			   TIM_OC1Init
#define DAC2_TIM_OCPreloadConfig   TIM_OC1PreloadConfig


#define DAC3_TIM                   TIM3
#define DAC3_TIM_CLK               RCC_APB1Periph_TIM3
#define DAC3_TIM_CLK_INIT          RCC_APB1PeriphClockCmd
#define DAC3_TIM_SOURCE            GPIO_PinSource6
#define DAC3_TIM_AF                GPIO_AF_TIM3
#define DAC3_GPIO_PIN              GPIO_Pin_6
#define DAC3_GPIO_PORT             GPIOA
#define DAC3_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define DAC3_TIM_OCInit			   TIM_OC1Init
#define DAC3_TIM_OCPreloadConfig   TIM_OC1PreloadConfig


#define DAC4_TIM                   TIM12
#define DAC4_TIM_CLK               RCC_APB1Periph_TIM12
#define DAC4_TIM_CLK_INIT          RCC_APB1PeriphClockCmd
#define DAC4_TIM_SOURCE            GPIO_PinSource6
#define DAC4_TIM_AF                GPIO_AF_TIM12
#define DAC4_GPIO_PIN              GPIO_Pin_6
#define DAC4_GPIO_PORT             GPIOH
#define DAC4_GPIO_CLK              RCC_AHB1Periph_GPIOH
#define DAC4_TIM_OCInit			   TIM_OC1Init
#define DAC4_TIM_OCPreloadConfig   TIM_OC1PreloadConfig


#define DAC5_TIM                   TIM13
#define DAC5_TIM_CLK               RCC_APB1Periph_TIM13
#define DAC5_TIM_CLK_INIT          RCC_APB1PeriphClockCmd
#define DAC5_TIM_SOURCE            GPIO_PinSource8
#define DAC5_TIM_AF                GPIO_AF_TIM13
#define DAC5_GPIO_PIN              GPIO_Pin_8
#define DAC5_GPIO_PORT             GPIOF
#define DAC5_GPIO_CLK              RCC_AHB1Periph_GPIOF
#define DAC5_TIM_OCInit			   TIM_OC1Init
#define DAC5_TIM_OCPreloadConfig   TIM_OC1PreloadConfig


#define DAC6_TIM                   TIM5
#define DAC6_TIM_CLK               RCC_APB1Periph_TIM5
#define DAC6_TIM_CLK_INIT          RCC_APB1PeriphClockCmd
#define DAC6_TIM_SOURCE            GPIO_PinSource2
#define DAC6_TIM_AF                GPIO_AF_TIM5
#define DAC6_GPIO_PIN              GPIO_Pin_2
#define DAC6_GPIO_PORT             GPIOA
#define DAC6_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define DAC6_TIM_OCInit			   TIM_OC3Init
#define DAC6_TIM_OCPreloadConfig   TIM_OC3PreloadConfig
#endif



static	TIM_OCInitTypeDef  TIM_OCInitStructure;
static	uint16_t TimerPeriod = 0;

void PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

    DAC1_TIM_CLK_INIT(DAC1_TIM_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(DAC1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DAC1_GPIO_PIN;
    GPIO_Init(DAC1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DAC1_GPIO_PORT, DAC1_TIM_SOURCE, DAC1_TIM_AF);

    DAC2_TIM_CLK_INIT(DAC2_TIM_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(DAC2_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DAC2_GPIO_PIN;
    GPIO_Init(DAC2_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DAC2_GPIO_PORT, DAC2_TIM_SOURCE, DAC2_TIM_AF);

    DAC3_TIM_CLK_INIT(DAC3_TIM_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(DAC3_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DAC3_GPIO_PIN;
    GPIO_Init(DAC3_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DAC3_GPIO_PORT, DAC3_TIM_SOURCE, DAC3_TIM_AF);

    DAC4_TIM_CLK_INIT(DAC4_TIM_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(DAC4_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DAC4_GPIO_PIN;
    GPIO_Init(DAC4_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DAC4_GPIO_PORT, DAC4_TIM_SOURCE, DAC4_TIM_AF);

    DAC5_TIM_CLK_INIT(DAC5_TIM_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(DAC5_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DAC5_GPIO_PIN;
    GPIO_Init(DAC5_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DAC5_GPIO_PORT, DAC5_TIM_SOURCE, DAC5_TIM_AF);

    DAC6_TIM_CLK_INIT(DAC6_TIM_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(DAC6_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DAC6_GPIO_PIN;
    GPIO_Init(DAC6_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DAC6_GPIO_PORT, DAC6_TIM_SOURCE, DAC6_TIM_AF);

    /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
    //TimerPeriod = (SystemCoreClock / 17570 ) - 1;
    TimerPeriod = (SystemCoreClock / (180 - 1) / OUT_FREQ / 2); //500hz

    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = (180 - 1);
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = TimerPeriod - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;


    TIM_TimeBaseInit(DAC1_TIM, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel4 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    DAC1_TIM_OCInit(DAC1_TIM, &TIM_OCInitStructure);
    DAC1_TIM_OCPreloadConfig(DAC1_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(DAC1_TIM, ENABLE);
    TIM_CtrlPWMOutputs(DAC1_TIM, ENABLE); //定时器1 和定时器8  输出PWM 时 需要添加这条
    TIM_Cmd(DAC1_TIM, ENABLE);


    TIM_TimeBaseInit(DAC2_TIM, &TIM_TimeBaseStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    DAC2_TIM_OCInit(DAC2_TIM, &TIM_OCInitStructure);
    DAC2_TIM_OCPreloadConfig(DAC2_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(DAC2_TIM, ENABLE);
    TIM_CtrlPWMOutputs(DAC2_TIM, ENABLE); //定时器1 和定时器8  输出PWM 时 需要添加这条
    TIM_Cmd(DAC2_TIM, ENABLE);

    TIM_TimeBaseInit(DAC3_TIM, &TIM_TimeBaseStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    DAC3_TIM_OCInit(DAC3_TIM, &TIM_OCInitStructure);
    DAC3_TIM_OCPreloadConfig(DAC3_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(DAC3_TIM, ENABLE);
    TIM_CtrlPWMOutputs(DAC3_TIM, ENABLE); //定时器1 和定时器8  输出PWM 时 需要添加这条
    TIM_Cmd(DAC3_TIM, ENABLE);


    TIM_TimeBaseInit(DAC4_TIM, &TIM_TimeBaseStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    DAC4_TIM_OCInit(DAC4_TIM, &TIM_OCInitStructure);
    DAC4_TIM_OCPreloadConfig(DAC4_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(DAC4_TIM, ENABLE);
    TIM_CtrlPWMOutputs(DAC4_TIM, ENABLE); //定时器1 和定时器8  输出PWM 时 需要添加这条
    TIM_Cmd(DAC4_TIM, ENABLE);

    TIM_TimeBaseInit(DAC5_TIM, &TIM_TimeBaseStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    DAC5_TIM_OCInit(DAC5_TIM, &TIM_OCInitStructure);
    DAC5_TIM_OCPreloadConfig(DAC5_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(DAC5_TIM, ENABLE);
    TIM_CtrlPWMOutputs(DAC5_TIM, ENABLE); //定时器1 和定时器8  输出PWM 时 需要添加这条
    TIM_Cmd(DAC5_TIM, ENABLE);

    TIM_TimeBaseInit(DAC6_TIM, &TIM_TimeBaseStructure);
    TIM_OCInitStructure.TIM_Pulse = 0;
    DAC6_TIM_OCInit(DAC6_TIM, &TIM_OCInitStructure);
    DAC6_TIM_OCPreloadConfig(DAC6_TIM, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(DAC6_TIM, ENABLE);
    TIM_CtrlPWMOutputs(DAC6_TIM, ENABLE); //定时器1 和定时器8  输出PWM 时 需要添加这条
    TIM_Cmd(DAC6_TIM, ENABLE);

}

void PWM1_Set(u16 percent_10)
{
    uint16_t Pulse = 0;
    Pulse = (uint16_t) (((uint32_t) percent_10 * (TimerPeriod)) / 1000);
    TIM_OCInitStructure.TIM_Pulse = Pulse;
    DAC1_TIM_OCInit(DAC1_TIM, &TIM_OCInitStructure);
}
void PWM2_Set(u16 percent_10)
{
    uint16_t Pulse = 0;
    Pulse = (uint16_t) (((uint32_t) percent_10 * (TimerPeriod)) / 1000);
    TIM_OCInitStructure.TIM_Pulse = Pulse;
    DAC2_TIM_OCInit(DAC2_TIM, &TIM_OCInitStructure);
}
void PWM3_Set(u16 percent_10)
{
    uint16_t Pulse = 0;
    Pulse = (uint16_t) (((uint32_t) percent_10 * (TimerPeriod)) / 1000);
    TIM_OCInitStructure.TIM_Pulse = Pulse;
    DAC3_TIM_OCInit(DAC3_TIM, &TIM_OCInitStructure);
}
void PWM4_Set(u16 percent_10)
{
    uint16_t Pulse = 0;
    Pulse = (uint16_t) (((uint32_t) percent_10 * (TimerPeriod)) / 1000);
    TIM_OCInitStructure.TIM_Pulse = Pulse;
    DAC4_TIM_OCInit(DAC4_TIM, &TIM_OCInitStructure);
}

void PWM5_Set(u16 percent_10)
{
    uint16_t Pulse = 0;
    Pulse = (uint16_t) (((uint32_t) percent_10 * (TimerPeriod)) / 1000);
    TIM_OCInitStructure.TIM_Pulse = Pulse;
    DAC5_TIM_OCInit(DAC5_TIM, &TIM_OCInitStructure);
}

void PWM6_Set(u16 percent_10)
{
    uint16_t Pulse = 0;
    Pulse = (uint16_t) (((uint32_t) percent_10 * (TimerPeriod)) / 1000);
    TIM_OCInitStructure.TIM_Pulse = Pulse;
    DAC6_TIM_OCInit(DAC6_TIM, &TIM_OCInitStructure);
}
static u16 pwm_adjust[5] = {0,282,542,782,1000};
static u16 pwm_swtich(u8 degree)
{
    u16 i = degree / 25;
    u16 j = degree % 25;
    u16 result = 0;
    float per = 0;

    if (degree >= 100) return 1000;

    result = pwm_adjust[i];
    per = (pwm_adjust[i + 1] - pwm_adjust[i]) / 25.0 * j;
    result += per;
    return result;
}
void PWM_Out(u8 channel , u8 degree )
{
    u16 pwm_value = 0;

    if (channel > ALLPWM_NUM_MAX || degree > 100) {
        return ;
    }

    pwm_value = pwm_swtich(degree);

    switch (channel) {
    case  0:
        PWM1_Set(pwm_value);
        break;

    case  1:
        PWM2_Set(pwm_value);
        break;

    case  2:
        PWM3_Set(pwm_value);
        break;

    case  3:
        PWM4_Set(pwm_value);
        break;

    case  4:
        PWM5_Set(pwm_value);
        break;

    case  5:
        PWM6_Set(pwm_value);
        break;

    default:
        break;
    }

}



