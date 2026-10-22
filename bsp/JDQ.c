#include "jdq.h"
#include "parameters.h"
#include "bsp.h"
#include "ucos_ii.h"

OS_EVENT    *jdq_ctrl_sem;

#define HC_595_OE_ENABLE


#if USE_GD == 1

#define HC595_DATA_PIN               	GPIO_Pin_13
#define HC595_DATA_GPIO_PORT              GPIOC
#define HC595_DATA_GPIO_CLK               RCC_AHB1Periph_GPIOC

#define HC595_SCLK_PIN                   GPIO_Pin_15
#define HC595_SCLK_GPIO_PORT             GPIOC
#define HC595_SCLK_GPIO_CLK              RCC_AHB1Periph_GPIOC

#define HC595_RCLK_PIN                   GPIO_Pin_14
#define HC595_RCLK_GPIO_PORT             GPIOC
#define HC595_RCLK_GPIO_CLK              RCC_AHB1Periph_GPIOC

#else

#define HC595_DATA_PIN               	GPIO_Pin_11
#define HC595_DATA_GPIO_PORT              GPIOI
#define HC595_DATA_GPIO_CLK               RCC_AHB1Periph_GPIOI

#define HC595_SCLK_PIN                   GPIO_Pin_3
#define HC595_SCLK_GPIO_PORT             GPIOH
#define HC595_SCLK_GPIO_CLK              RCC_AHB1Periph_GPIOH

#define HC595_RCLK_PIN                   GPIO_Pin_2
#define HC595_RCLK_GPIO_PORT             GPIOH
#define HC595_RCLK_GPIO_CLK              RCC_AHB1Periph_GPIOH

#define HC595_OE_PIN                   GPIO_Pin_14
#define HC595_OE_GPIO_PORT             GPIOC
#define HC595_OE_GPIO_CLK              RCC_AHB1Periph_GPIOC

#define HC595_OE_ON  		GPIO_SetBits(HC595_OE_GPIO_PORT ,HC595_OE_PIN)
#define HC595_OE_OFF 		GPIO_ResetBits(HC595_OE_GPIO_PORT ,HC595_OE_PIN)

#endif


//OUTPUT: SCLK
#define HC595_SCLK_ON  		GPIO_SetBits(HC595_SCLK_GPIO_PORT ,HC595_SCLK_PIN)
#define HC595_SCLK_OFF 		GPIO_ResetBits(HC595_SCLK_GPIO_PORT ,HC595_SCLK_PIN)
//OUTPUT: RCLK
#define HC595_RCLK_ON  		GPIO_SetBits(HC595_RCLK_GPIO_PORT ,HC595_RCLK_PIN)
#define HC595_RCLK_OFF 		GPIO_ResetBits(HC595_RCLK_GPIO_PORT ,HC595_RCLK_PIN)
//OUTPUT: SI
#define HC595_SI_ON  		GPIO_SetBits(HC595_DATA_GPIO_PORT ,HC595_DATA_PIN)
#define HC595_SI_OFF 		GPIO_ResetBits(HC595_DATA_GPIO_PORT ,HC595_DATA_PIN)





//595clk的上升沿有效
//数据高位对应高位  Q0为低位（15）Q7为高位（7）
static void delay_595_us(u16 time)
{
    while (time != 0) {
        time--;
    }
}
u32 jdq_595value = 0;

#define HC595_DELAY		(200)
void HC595_Write_Data(u32 data)
{
    u8 i;
    HC595_SCLK_OFF;
    delay_595_us(HC595_DELAY);

    for (i = 0 ; i < 24; i++) {
        if (data & 0x800000)
            HC595_SI_ON;
        else
            HC595_SI_OFF;

        data <<= 1;
        delay_595_us(HC595_DELAY);
        HC595_SCLK_ON;//上升沿移位
        delay_595_us(HC595_DELAY);
        HC595_SCLK_OFF;
        delay_595_us(HC595_DELAY);
    }

    //锁存开,输出数据
    HC595_RCLK_OFF;
    delay_595_us(HC595_DELAY);
    HC595_RCLK_ON;//上升沿输出
    delay_595_us(HC595_DELAY);
    HC595_RCLK_OFF;
}
static u8 jdq_ctrl_state = 0;
void JDQ_Control(u8 id , u8 status)
{
	u32 jdq_595value_temp = 0;

    if (id > ALLJDQ_NUM_MAX || status > 1) {
        return ;
    }

//    OSSemPend(jdq_ctrl_sem, 0, &os_err); //等待其他操作完成
//    jdq_ctrl_state = STA_ON;

    if (id == JDQ_ALA && basic_inf.ala_open_mode == STA_ON) {
        status = (status + 1) % 2;
    }
	
	jdq_595value_temp = jdq_595value;

    if (status == 1) {
        setbit(jdq_595value_temp, id);
    } else {
        clrbit(jdq_595value_temp, id);
    }
	
	jdq_595value = jdq_595value_temp;


//    HC595_Write_Data(jdq_595value);

//    OSSemPost(jdq_ctrl_sem); //数据发送完成
//    jdq_ctrl_state = STA_OFF;
}

void JDQ_Control_Hard(void)
{
    u8 os_err;
	
    OSSemPend(jdq_ctrl_sem, 0, &os_err); //等待其他操作完成
    jdq_ctrl_state = STA_ON;

    HC595_Write_Data(jdq_595value);
	
    OSSemPost(jdq_ctrl_sem); //数据发送完成	
    jdq_ctrl_state = STA_OFF;

    OSTimeDly(20);
}

u8 get_jdq_ctrl_state(void)
{
    return jdq_ctrl_state;
}

void HC595_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(HC595_RCLK_GPIO_CLK, ENABLE);

    //Rclk输出
    GPIO_InitStructure.GPIO_Pin = HC595_RCLK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(HC595_RCLK_GPIO_PORT, &GPIO_InitStructure);

    RCC_AHB1PeriphClockCmd(HC595_SCLK_GPIO_CLK, ENABLE);
    //SCLK输出
    GPIO_InitStructure.GPIO_Pin = HC595_SCLK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(HC595_SCLK_GPIO_PORT , &GPIO_InitStructure);

    RCC_AHB1PeriphClockCmd(HC595_DATA_GPIO_CLK, ENABLE);
    //DATA
    GPIO_InitStructure.GPIO_Pin = HC595_DATA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(HC595_DATA_GPIO_PORT , &GPIO_InitStructure);

#if USE_GD == 1

#else
    RCC_AHB1PeriphClockCmd(HC595_OE_GPIO_CLK, ENABLE);
    //EN
    GPIO_InitStructure.GPIO_Pin = HC595_OE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(HC595_OE_GPIO_PORT , &GPIO_InitStructure);
    HC595_OE_OFF;//输出使能

#endif
    HC595_SCLK_OFF;//移位寄存器时钟为低
    HC595_RCLK_ON;//寄存器输出时钟为低
    jdq_595value = 0x0;
    HC595_Write_Data(jdq_595value);
}
void JDQ_Init(void)
{

#if USE_GD == 1

#else	
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    //Rclk输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC , GPIO_Pin_14);
    GPIO_ResetBits(GPIOC , GPIO_Pin_15);
    GPIO_ResetBits(GPIOC , GPIO_Pin_1);
#endif

    HC595_Init();

    jdq_ctrl_sem = OSSemCreate(1);
}

