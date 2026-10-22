/******************************************************************************

  文 件 名   : stm324x9i_eval_lcd.c
  版 本 号   : 初稿
  作    者   : 邹稳
  生成日期   : 2015年4月22日星期三
  最近修改   :
  功能描述   : LTDC驱动
  函数列表   :
              LCD_AF_GPIOConfig
              LCD_Clear
              LCD_ClosedPolyLine
              LCD_ClosedPolyLineRelative
              LCD_Dis16PxTrans
              LCD_DisplayOff
              LCD_DisplayOn
              LCD_DispString
              LCD_DrawCircle
              LCD_DrawEllipse
              LCD_DrawFullCircle
              LCD_DrawFullEllipse
              LCD_DrawFullRect
              LCD_DrawLine
              LCD_DrawMonoPict
              LCD_DrawRect
              LCD_DrawUniLine
              LCD_FillPolyLine
              LCD_FillTriangle
              LCD_GetColors
              LCD_GetPixelHeight
              LCD_GetPixelWidth
              LCD_GetPoint
              LCD_Init
              LCD_LayerInit
              LCD_PolyLine
              LCD_PolyLineRelative
              LCD_PolyLineRelativeClosed
              LCD_ReadData
              LCD_ReSetColorKeying
              LCD_SelectBlock
              LCD_SetAddress
              LCD_SetBackColor
              LCD_SetColorKeying
              LCD_SetColors
              LCD_SetCursor
              LCD_SetDisplayWindow
              LCD_SetLayer
              LCD_SetPixelFormat
              LCD_SetPoint
              LCD_SetTextColor
              LCD_SetTransparency
              LCD_Triangle
              LCD_WindowModeDisable
              LCD_WriteBMP
              LCD_WriteData
              PutPixel
  修改历史   :
  1.日    期   : 2015年4月22日星期三
    作    者   : 邹稳
    修改内容   : 创建文件
    DMA2D功能:  DMA2D_M2M  		从flash拷贝至显存
    			DMA2D_M2M_PFC 	从flash拷贝至显存，并可以进行颜色的格式转换，替换/不更改/混合
    			DMA2D_M2M_BLEND 从flash拷贝至显存，并先进行前景与后景色混合
    			DMA2D_R2M		拷贝固定颜色至显存

******************************************************************************/


/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/



/*****************************************************************************/
/**
  ******************************************************************************
  * @file    STM324x9i_eval_lcd.c
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    13-November-2013
  * @brief   This file provides driver for AMPIRE 480x272 AM480272H3TMQW-T01H
  *          (MB1046) and 480x640 AM640480G5TNQW-T00H (MB1063) LCD modules mounted
  *          on STM324x9I-EVAL evaluation board (MB1045). Please note that AMPIRE
  *          480x272 LCD is provided with STM32429I-EVAL board AMPIRE 480x640 LCD
  *          is provided with STM32439I-EVAL board
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm324x9i_eval_lcd.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup STM324x9I_EVAL
  * @{
  */

/** @defgroup STM324x9I_EVAL_LCD
  * @brief This file includes the LCD driver for AM480272H3TMQW-T01H
  *        and AM640480G5TNQW-T00H Liquid Crystal Display Modules
  *        of STM324x9I_EVAL board.
  * @{
  */

/** @defgroup STM324x9I_EVAL_LCD_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup STM324x9I_EVAL_LCD_Private_Defines
  * @{
  */

#define POLY_X(Z)              ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)              ((int32_t)((Points + Z)->Y))


#define LCD_TIMEOUT_MAX    0x3000 /* The value of the maximal timeout for I2C waiting loops */

/**
  * @}
  */

/** @defgroup STM324x9I_EVAL_LCD_Private_Macros
  * @{
  */
#define ABS(X)  ((X) > 0 ? (X) : -(X))
/**
  * @}
  */

/** @defgroup STM324x9I_EVAL_LCD_Private_Variables
  * @{
  */
//static sFONT *LCD_Currentfonts;

/* Global variables to set the written text color */
static uint16_t CurrentTextColor   = 0x0000;
static uint16_t CurrentBackColor   = 0xFFFF;
/* Default LCD configuration with LCD Layer 1 */
uint32_t CurrentFrameBuffer = LCD_FRAME_BUFFER;
static uint32_t CurrentLayer = LCD_BACKGROUND_LAYER;

//static uint32_t CurrentLcd = USE_LCD_AM800480;
static uint32_t LCD_PIXEL_WIDTH  = 800;
static uint32_t LCD_PIXEL_HEIGHT = 480;

/**
  * @}
  */


/** @defgroup STM324x9I_EVAL_LCD_Private_FunctionPrototypes
  * @{
  */
static void PutPixel(int16_t x, int16_t y);
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed);
//static void LCD_I2C_Config(void);
static void LCD_AF_GPIOConfig(void);

#if USE_GD == 1
void LCD_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_7);
}
void LCD_SetBackLight(unsigned char light)
{
    if (light != 0) {
        GPIO_SetBits(GPIOB, GPIO_Pin_7);
    } else {
        GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    }
}

#else
void LCD_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

	
    GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14 , ENABLE);
    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = 700;//约为1khz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 255; 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

    /* Channel 1, 2,3 and 4 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 255;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM14, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM14, ENABLE);

    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM14, ENABLE);//输出PWM
	    /* TIM1 counter enable */
    TIM_Cmd(TIM14, ENABLE);
}
void LCD_SetBackLight(unsigned char light)
{
	 TIM14->CCR1 = light;
}
#endif

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
void LCD_Init(void)
{

    LTDC_InitTypeDef       LTDC_InitStruct;
    LTDC_Layer_TypeDef     LTDC_Layerx;

    /* Enable the LTDC Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

    /* Enable the DMA2D Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

    /* Configure the LCD Control pins */
    LCD_AF_GPIOConfig();

    /* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for LCD need not*/
    SDRAM_FMC_Config();

    /* Check if the LCD is AMPIRE480272 or AMPIRE640480  need not*/
//  LCD_CheckDevice();

    /* Polarity configuration */
    /* Initialize the horizontal synchronization polarity as active low */
    LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;
    /* Initialize the vertical synchronization polarity as active low */
    LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;
    /* Initialize the data enable polarity as active low */
    LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;
    /* Initialize the pixel clock polarity as input pixel clock */
    LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

    /* Configure R,G,B component values for LCD background color */
    LTDC_InitStruct.LTDC_BackgroundRedValue = 0;
    LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
    LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;


    /* Enable Pixel Clock --------------------------------------------------------*/
    /* Configure PLLSAI prescalers for LCD */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/3 = 64 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 64/2 = 32 Mhz */
    RCC_PLLSAIConfig(192, 2, 3);
    RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);

    /* Enable PLLSAI Clock */
    RCC_PLLSAICmd(ENABLE);
    /* Wait for PLLSAI activation */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET)
    {
    }


    /* Timing configuration */
    /* Horizontal synchronization width = Hsync - 1 */
    LTDC_InitStruct.LTDC_HorizontalSync = 63;
    /* Vertical synchronization height = Vsync - 1 */
    LTDC_InitStruct.LTDC_VerticalSync = 10;
    /* Accumulated horizontal back porch = Hsync + HBP - 1 */
    LTDC_InitStruct.LTDC_AccumulatedHBP = 127;
    /* Accumulated vertical back porch = Vsync + VBP - 1 */
    LTDC_InitStruct.LTDC_AccumulatedVBP = 22;
    /* Accumulated active width = Hsync + HBP + Active Width - 1 */
    LTDC_InitStruct.LTDC_AccumulatedActiveW = 927;
    /* Accumulated active height = Vsync + VBP + Active Heigh - 1 */
    LTDC_InitStruct.LTDC_AccumulatedActiveH = 502;
    /* Total width = Hsync + HBP + Active Width + HFP - 1 */
    LTDC_InitStruct.LTDC_TotalWidth = 1055;
    /* Total height = Vsync + VBP + Active Heigh + VFP - 1 */
    LTDC_InitStruct.LTDC_TotalHeigh = 524;
    LTDC_Init(&LTDC_InitStruct);
    LCD_LayerInit();
    LCD_DisplayOn();
}
/**
  * @brief  Initializes the LCD layers.
  * @param  None
  * @retval None
  */
void LCD_LayerInit(void)
{

    LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct;

    /* LCD AMPIRE800480 is used */

    /* Windowing configuration */
    /* Horizontal start = horizontal synchronization + Horizontal back porch = 43
    Vertical start   = vertical synchronization + vertical back porch     = 12
    Horizontal stop = Horizontal start + LCD_PIXEL_WIDTH -1
    Vertical stop   = Vertical start + LCD_PIXEL_HEIGHT -1
    */
    LTDC_Layer_InitStruct.LTDC_HorizontalStart = 128;
    LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_PIXEL_WIDTH + 128 - 1);
    LTDC_Layer_InitStruct.LTDC_VerticalStart = 23;
    LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_PIXEL_HEIGHT + 23 - 1);

    /* Pixel Format configuration*/
    LTDC_Layer_InitStruct.LTDC_PixelFormat = LTDC_Pixelformat_RGB565;
    /* Alpha constant (255 totally opaque) */
    LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255;
    /* Default Color configuration (configure A,R,G,B component values) */
    LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0;
    LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;
    LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;
    LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
    /* Configure blending factors */
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;

    /* the length of one line of pixels in bytes + 3 then :
    Line Lenth = Active high width x number of bytes per pixel + 3
    Active high width         = LCD_PIXEL_WIDTH
    number of bytes per pixel = 2    (pixel_format : RGB565)
    */
    LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_PIXEL_WIDTH * 2) + 3);
    /* the pitch is the increment from the start of one line of pixels to the
    start of the next line in bytes, then :
    Pitch = Active high width x number of bytes per pixel
    */
    LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_PIXEL_WIDTH * 2);

    /* Configure the number of lines */
    LTDC_Layer_InitStruct.LTDC_CFBLineNumber = LCD_PIXEL_HEIGHT;

    /* Start Address configuration : the LCD Frame buffer is defined on SDRAM */
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER;

    LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);

    /* Configure Layer2 */
    /* Start Address configuration : the LCD Frame buffer is defined on SDRAM w/ Offset */
    LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_FRAME_BUFFER + BUFFER_OFFSET;

    /* Configure blending factors */
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
    LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;

    LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);




    LTDC_ReloadConfig(LTDC_IMReload);

    /* Enable foreground & background Layers */
    LTDC_LayerCmd(LTDC_Layer1, ENABLE);
//    LTDC_LayerCmd(LTDC_Layer2, ENABLE);
    LTDC_ReloadConfig(LTDC_IMReload);

}

/**
  * @brief  Sets the LCD Layer.
  * @param  Layerx: specifies the Layer foreground or background.
  * @retval None
  */
void LCD_SetLayer(uint32_t Layerx)
{
    if (Layerx == LCD_BACKGROUND_LAYER)
    {
        CurrentFrameBuffer = LCD_FRAME_BUFFER;
        CurrentLayer = LCD_BACKGROUND_LAYER;
    }
    else
    {
        CurrentFrameBuffer = LCD_FRAME_BUFFER + BUFFER_OFFSET;
        CurrentLayer = LCD_FOREGROUND_LAYER;
    }
}

/**
  * @brief  Sets the LCD Text and Background colors.
  * @param  TextColor: specifies the Text Color.
  * @param  BackColor: specifies the Background Color.
  * @retval None
  */
void LCD_SetColors(uint16_t TextColor, uint16_t BackColor)
{
    CurrentTextColor = TextColor;
    CurrentBackColor = BackColor;
}

/**
  * @brief  Gets the LCD Text and Background colors.
  * @param  TextColor: pointer to the variable that will contain the Text
            Color.
  * @param  BackColor: pointer to the variable that will contain the Background
            Color.
  * @retval None
  */
void LCD_GetColors(uint16_t *TextColor, uint16_t *BackColor)
{
    *TextColor = CurrentTextColor;
    *BackColor = CurrentBackColor;
}

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetTextColor(uint16_t Color)
{
    CurrentTextColor = Color;
}


/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetBackColor(uint16_t Color)
{
    CurrentBackColor = Color;
}

/**
//  * @brief  Sets the Text Font.
//  * @param  fonts: specifies the font to be used.
//  * @retval None
//  */
//void LCD_SetFont(sFONT *fonts)
//{
//    LCD_Currentfonts = fonts;
//}

/**
  * @brief  Configure the transparency.
  * @param  transparency: specifies the transparency,
  *         This parameter must range from 0x00 to 0xFF.
  * @retval None
  */
void LCD_SetTransparency(uint8_t transparency)
{
    if (CurrentLayer == LCD_BACKGROUND_LAYER)
    {
        LTDC_LayerAlpha(LTDC_Layer1, transparency);
    }
    else
    {
        LTDC_LayerAlpha(LTDC_Layer2, transparency);
    }
    LTDC_ReloadConfig(LTDC_IMReload);
}

/**
  * @brief  Configure the pixel format.
  * @param  PixelFormat: PixelFormat: reconfigure the pixel format,
  *                      this parameter can be one of the following values:
  *                      LTDC_Pixelformat_ARGB8888
  *                      LTDC_Pixelformat_RGB888
  *                      LTDC_Pixelformat_RGB565
  *                      LTDC_Pixelformat_ARGB1555
  *                      LTDC_Pixelformat_ARGB4444
  *                      LTDC_Pixelformat_L8
  *                      LTDC_Pixelformat_AL44
  *                      LTDC_Pixelformat_AL88
  * @retval None
  */
void LCD_SetPixelFormat(uint32_t PixelFormat)
{
    if (CurrentLayer == LCD_BACKGROUND_LAYER)
    {
        LTDC_LayerPixelFormat(LTDC_Layer1, PixelFormat);
    }
    else
    {
        LTDC_LayerPixelFormat(LTDC_Layer2, PixelFormat);
    }
    LTDC_ReloadConfig(LTDC_IMReload);
}

/**
  * @brief  Configure the Address.
  * @param  transparency: specifies the Address.
  * @retval None
  */
void LCD_SetAddress(uint32_t Address)
{
    if (CurrentLayer == LCD_BACKGROUND_LAYER)
    {
        LTDC_LayerAddress(LTDC_Layer1, Address);
    }
    else
    {
        LTDC_LayerAddress(LTDC_Layer2, Address);
    }
    LTDC_ReloadConfig(LTDC_IMReload);
}

/**
//  * @brief  Gets the Text Font.
//  * @param  None.
//  * @retval the used font.
//  */
//sFONT *LCD_GetFont(void)
//{
//    return LCD_Currentfonts;
//}

/**
//  * @brief  Clears the selected line.
//  * @param  Line: the Line to be cleared.
//  *   This parameter can be one of the following values:
//  *     @arg Linex: where x can be 0..n
//  * @retval None
//  */
//void LCD_ClearLine(uint16_t Line)
//{
//    uint16_t refcolumn = LCD_PIXEL_WIDTH - 1;
//    /* Send the string character by character on lCD */
//    while (((refcolumn + 1)& 0xFFFF) >= LCD_Currentfonts->Width)
//    {
//        /* Display one character on LCD */
//        LCD_DisplayChar(Line, refcolumn, ' ');
//        /* Decrement the column position by 16 */
//        refcolumn -= LCD_Currentfonts->Width;
//    }
//}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: the color of the background.
  * @retval None
  */
void LCD_Clear(uint16_t Color)
{
    uint16_t *index ;

    /* erase memory */
    for (index = (uint16_t *)CurrentFrameBuffer; index < (uint16_t *)(CurrentFrameBuffer+(BUFFER_OFFSET)); index++)
    {
        *index = Color;
    }
}

/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval Display Address
  */
uint32_t LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    uint32_t SetFrameBuffer;
    if (CurrentLayer == LCD_BACKGROUND_LAYER)
    {
        SetFrameBuffer = LCD_FRAME_BUFFER + 2*(Xpos + (LCD_PIXEL_WIDTH*Ypos));
    }
    else
    {
        SetFrameBuffer = LCD_FRAME_BUFFER + BUFFER_OFFSET + 2*(Xpos + (LCD_PIXEL_WIDTH*Ypos));
    }
    return SetFrameBuffer;
}

/**
  * @brief  Config and Sets the color Keying.
  * @param  RGBValue: Specifies the Color reference.
  * @retval None
  */
void LCD_SetColorKeying(uint32_t RGBValue)
{
    uint32_t tmp = 0;

    LTDC_ColorKeying_InitTypeDef   LTDC_colorkeying_InitStruct;

    /* configure the color Keying */
    LTDC_colorkeying_InitStruct.LTDC_ColorKeyBlue = 0xFF & RGBValue;
    tmp = (0xFF00 & RGBValue);
    LTDC_colorkeying_InitStruct.LTDC_ColorKeyGreen = (tmp >> 8);
    tmp = (0xFF0000 & RGBValue);
    LTDC_colorkeying_InitStruct.LTDC_ColorKeyRed = (tmp >> 16);

    if (CurrentLayer == LCD_BACKGROUND_LAYER)
    {
        /* Enable the color Keying for Layer1 */
        LTDC_ColorKeyingConfig(LTDC_Layer1, &LTDC_colorkeying_InitStruct, ENABLE);
        LTDC_ReloadConfig(LTDC_IMReload);
    }
    else
    {
        /* Enable the color Keying for Layer2 */
        LTDC_ColorKeyingConfig(LTDC_Layer2, &LTDC_colorkeying_InitStruct, ENABLE);
        LTDC_ReloadConfig(LTDC_IMReload);
    }
}

/**
  * @brief  Disable the color Keying.
  * @param  RGBValue: Specifies the Color reference.
  * @retval None
  */
void LCD_ReSetColorKeying(void)
{
    LTDC_ColorKeying_InitTypeDef   LTDC_colorkeying_InitStruct;

    if (CurrentLayer == LCD_BACKGROUND_LAYER)
    {
        /* Disable the color Keying for Layer1 */
        LTDC_ColorKeyingConfig(LTDC_Layer1, &LTDC_colorkeying_InitStruct, DISABLE);
        LTDC_ReloadConfig(LTDC_IMReload);
    }
    else
    {
        /* Disable the color Keying for Layer2 */
        LTDC_ColorKeyingConfig(LTDC_Layer2, &LTDC_colorkeying_InitStruct, DISABLE);
        LTDC_ReloadConfig(LTDC_IMReload);
    }
}




/**
  * @brief  Sets a display window
  * @param  Xpos: specifies the X bottom left position.
  * @param  Ypos: specifies the Y bottom left position.
  * @param  Height: display window height.
  * @param  Width: display window width.
  * @retval None
  */
void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{

    if (CurrentLayer == LCD_BACKGROUND_LAYER)
    {
        /* reconfigure the layer1 position */
        LTDC_LayerPosition(LTDC_Layer1, Xpos, Ypos);
        LTDC_ReloadConfig(LTDC_IMReload);

        /* reconfigure the layer1 size */
        LTDC_LayerSize(LTDC_Layer1, Width, Height);
        LTDC_ReloadConfig(LTDC_IMReload);
    }
    else
    {
        /* reconfigure the layer2 position */
        LTDC_LayerPosition(LTDC_Layer2, Xpos, Ypos);
        LTDC_ReloadConfig(LTDC_IMReload);

        /* reconfigure the layer2 size */
        LTDC_LayerSize(LTDC_Layer2, Width, Height);
        LTDC_ReloadConfig(LTDC_IMReload);
    }
}

/**
  * @brief  Disables LCD Window mode.
  * @param  None
  * @retval None
  */
void LCD_WindowModeDisable(void)
{
    LCD_SetDisplayWindow(0, 0, LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT);
}

/**
  * @brief  Displays a line.
  * @param Xpos: specifies the X position.
  * @param Ypos: specifies the Y position.
  * @param Length: line length.
  * @param Direction: line direction.
  *   This parameter can be one of the following values: Vertical or Horizontal.
  * @retval None
  */
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
    DMA2D_InitTypeDef      DMA2D_InitStruct;

    uint32_t  Xaddress = 0;
    uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;

    Xaddress = CurrentFrameBuffer + 2*((LCD_PIXEL_WIDTH*Ypos) + Xpos);

    Red_Value = (0xF800 & CurrentTextColor) >> 11;
    Blue_Value = 0x001F & CurrentTextColor;
    Green_Value = (0x07E0 & CurrentTextColor) >> 5;

    /* Configure DMA2D */
    DMA2D_DeInit();
    DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
    DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
    DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;
    DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;
    DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;
    DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;
    DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;

    if(Direction == LCD_DIR_HORIZONTAL)
    {
        DMA2D_InitStruct.DMA2D_OutputOffset = 0;
        DMA2D_InitStruct.DMA2D_NumberOfLine = 1;
        DMA2D_InitStruct.DMA2D_PixelPerLine = Length;
    }
    else
    {
        DMA2D_InitStruct.DMA2D_OutputOffset = LCD_PIXEL_WIDTH - 1;
        DMA2D_InitStruct.DMA2D_NumberOfLine = Length;
        DMA2D_InitStruct.DMA2D_PixelPerLine = 1;
    }

    DMA2D_Init(&DMA2D_InitStruct);
    /* Start Transfer */
    DMA2D_StartTransfer();
    /* Wait for CTC Flag activation */
    while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
    {
    }

}

/**
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: display rectangle height.
  * @param  Width: display rectangle width.
  * @retval None
  */
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{
    /* draw horizontal lines */
    LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
    LCD_DrawLine(Xpos, (Ypos+ Height), Width, LCD_DIR_HORIZONTAL);

    /* draw vertical lines */
    LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
    LCD_DrawLine((Xpos + Width), Ypos, Height, LCD_DIR_VERTICAL);
}

/**
  * @brief  Draw a circle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius: radius of the circle.
  * @retval None
  */
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int x = -Radius, y = 0, err = 2-2*Radius, e2;
    do
    {
        *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
        *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
        *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;
        *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;

        e2 = err;
        if (e2 <= y)
        {
            err += ++y*2+1;
            if (-x == y && e2 <= x) e2 = 0;
        }
        if (e2 > x) err += ++x*2+1;
    }
    while (x <= 0);
}

/**
  * @brief  Draw a full ellipse.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius: minor radius of ellipse.
  * @param  Radius2: major radius of ellipse.
  * @retval None
  */

void LCD_DrawFullEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
    int x = -Radius, y = 0, err = 2-2*Radius, e2;
    float K = 0, rad1 = 0, rad2 = 0;

    rad1 = Radius;
    rad2 = Radius2;

    if (Radius > Radius2)
    {
        do
        {
            K = (float)(rad1/rad2);
            LCD_DrawLine((Xpos+x), (Ypos-(uint16_t)(y/K)), (2*(uint16_t)(y/K) + 1), LCD_DIR_VERTICAL);
            LCD_DrawLine((Xpos-x), (Ypos-(uint16_t)(y/K)), (2*(uint16_t)(y/K) + 1), LCD_DIR_VERTICAL);

            e2 = err;
            if (e2 <= y)
            {
                err += ++y*2+1;
                if (-x == y && e2 <= x) e2 = 0;
            }
            if (e2 > x) err += ++x*2+1;

        }
        while (x <= 0);
    }
    else
    {
        y = -Radius2;
        x = 0;
        do
        {
            K = (float)(rad2/rad1);
            LCD_DrawLine((Xpos-(uint16_t)(x/K)), (Ypos+y), (2*(uint16_t)(x/K) + 1), LCD_DIR_HORIZONTAL);
            LCD_DrawLine((Xpos-(uint16_t)(x/K)), (Ypos-y), (2*(uint16_t)(x/K) + 1), LCD_DIR_HORIZONTAL);

            e2 = err;
            if (e2 <= x)
            {
                err += ++x*2+1;
                if (-y == x && e2 <= y) e2 = 0;
            }
            if (e2 > y) err += ++y*2+1;
        }
        while (y <= 0);
    }
}

/**
  * @brief  Displays an Ellipse.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius: specifies Radius.
  * @param  Radius2: specifies Radius2.
  * @retval None
  */

void LCD_DrawEllipse(int Xpos, int Ypos, int Radius, int Radius2)
{
    int x = -Radius, y = 0, err = 2-2*Radius, e2;
    float K = 0, rad1 = 0, rad2 = 0;

    rad1 = Radius;
    rad2 = Radius2;

    if (Radius > Radius2)
    {
        do
        {
            K = (float)(rad1/rad2);
            *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K))))) = CurrentTextColor;
            *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos+(uint16_t)(y/K))))) = CurrentTextColor;
            *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K))))) = CurrentTextColor;
            *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-x) + LCD_PIXEL_WIDTH*(Ypos-(uint16_t)(y/K))))) = CurrentTextColor;

            e2 = err;
            if (e2 <= y)
            {
                err += ++y*2+1;
                if (-x == y && e2 <= x) e2 = 0;
            }
            if (e2 > x) err += ++x*2+1;
        }
        while (x <= 0);
    }
    else
    {
        y = -Radius2;
        x = 0;
        do
        {
            K = (float)(rad2/rad1);
            *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
            *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos+y)))) = CurrentTextColor;
            *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos+(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;
            *(__IO uint16_t*) (CurrentFrameBuffer + (2*((Xpos-(uint16_t)(x/K)) + LCD_PIXEL_WIDTH*(Ypos-y)))) = CurrentTextColor;

            e2 = err;
            if (e2 <= x)
            {
                err += ++x*2+1;
                if (-y == x && e2 <= y) e2 = 0;
            }
            if (e2 > y) err += ++y*2+1;
        }
        while (y <= 0);
    }
}

/**
  * @brief  Displays a mono-color picture.
  * @param  Pict: pointer to the picture array.
  * @retval None
  */
void LCD_DrawMonoPict(const uint32_t *Pict)
{
    uint32_t index = 0, counter = 0;


    for(index = 0; index < 2400; index++)
    {
        for(counter = 0; counter < 32; counter++)
        {
            if((Pict[index] & (1 << counter)) == 0x00)
            {
                *(__IO uint16_t*)(CurrentFrameBuffer) = CurrentBackColor;
            }
            else
            {
                *(__IO uint16_t*)(CurrentFrameBuffer) = CurrentTextColor;
            }
        }
    }
}

/**
  * @brief  Displays a bitmap picture loaded in the internal Flash.
  * @param  BmpAddress: Bmp picture address in the internal Flash.
  * @retval None
  */
void LCD_WriteBMP(uint32_t BmpAddress)
{
    uint32_t index = 0, size = 0, width = 0, height = 0, bit_pixel = 0;
    uint32_t Address;
    uint32_t currentline = 0, linenumber = 0;

    Address = CurrentFrameBuffer;

    /* Read bitmap size */
    size = *(__IO uint16_t *) (BmpAddress + 2);
    size |= (*(__IO uint16_t *) (BmpAddress + 4)) << 16;

    /* Get bitmap data address offset */
    index = *(__IO uint16_t *) (BmpAddress + 10);
    index |= (*(__IO uint16_t *) (BmpAddress + 12)) << 16;

    /* Read bitmap width */
    width = *(uint16_t *) (BmpAddress + 18);
    width |= (*(uint16_t *) (BmpAddress + 20)) << 16;

    /* Read bitmap height */
    height = *(uint16_t *) (BmpAddress + 22);
    height |= (*(uint16_t *) (BmpAddress + 24)) << 16;

    /* Read bit/pixel */
    bit_pixel = *(uint16_t *) (BmpAddress + 28);

    if (CurrentLayer == LCD_BACKGROUND_LAYER)
    {
        /* reconfigure layer size in accordance with the picture */
        LTDC_LayerSize(LTDC_Layer1, width, height);
        LTDC_ReloadConfig(LTDC_VBReload);

        /* Reconfigure the Layer pixel format in accordance with the picture */
        if ((bit_pixel/8) == 4)
        {
            LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_ARGB8888);
            LTDC_ReloadConfig(LTDC_VBReload);
        }
        else if ((bit_pixel/8) == 2)
        {
            LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_RGB565);
            LTDC_ReloadConfig(LTDC_VBReload);
        }
        else
        {
            LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_RGB888);
            LTDC_ReloadConfig(LTDC_VBReload);
        }
    }
    else
    {
        /* reconfigure layer size in accordance with the picture */
        LTDC_LayerSize(LTDC_Layer2, width, height);
        LTDC_ReloadConfig(LTDC_VBReload);

        /* Reconfigure the Layer pixel format in accordance with the picture */
        if ((bit_pixel/8) == 4)
        {
            LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_ARGB8888);
            LTDC_ReloadConfig(LTDC_VBReload);
        }
        else if ((bit_pixel/8) == 2)
        {
            LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_RGB565);
            LTDC_ReloadConfig(LTDC_VBReload);
        }
        else
        {
            LTDC_LayerPixelFormat(LTDC_Layer2, LTDC_Pixelformat_RGB888);
            LTDC_ReloadConfig(LTDC_VBReload);
        }
    }

    /* compute the real size of the picture (without the header)) */
    size = (size - index);

    /* bypass the bitmap header */
    BmpAddress += index;

    /* start copie image from the bottom */
    Address += width*(height-1)*(bit_pixel/8);

    for(index = 0; index < size; index++)
    {
        *(__IO uint8_t*) (Address) = *(__IO uint8_t *)BmpAddress;

        /*jump on next byte */
        BmpAddress++;
        Address++;
        currentline++;

        if((currentline/(bit_pixel/8)) == width)
        {
            if(linenumber < height)
            {
                linenumber++;
                Address -=(2*width*(bit_pixel/8));
                currentline = 0;
            }
        }
    }
}

/**
  * @brief  Displays a full rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: rectangle height.
  * @param  Width: rectangle width.
  * @retval None
  */
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    DMA2D_InitTypeDef      DMA2D_InitStruct;

    uint32_t  Xaddress = 0;
    uint16_t Red_Value = 0, Green_Value = 0, Blue_Value = 0;

    Red_Value = (0xF800 & CurrentTextColor) >> 11;
    Blue_Value = 0x001F & CurrentTextColor;
    Green_Value = (0x07E0 & CurrentTextColor) >> 5;

    Xaddress = CurrentFrameBuffer + 2*(LCD_PIXEL_WIDTH*Ypos + Xpos);

    /* configure DMA2D */
    DMA2D_DeInit();
    DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;
    DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;
    DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;
    DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;
    DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;
    DMA2D_InitStruct.DMA2D_OutputAlpha = 0x0F;
    DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Xaddress;
    DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_PIXEL_WIDTH - Width);
    DMA2D_InitStruct.DMA2D_NumberOfLine = Height;
    DMA2D_InitStruct.DMA2D_PixelPerLine = Width;
    DMA2D_Init(&DMA2D_InitStruct);

    /* Start Transfer */
    DMA2D_StartTransfer();

    /* Wait for CTC Flag activation */
    while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
    {
    }

    LCD_SetTextColor(CurrentTextColor);
}

/**
  * @brief  Displays a full circle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius
  * @retval None
  */
void LCD_DrawFullCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int32_t  D;    /* Decision Variable */
    uint32_t  CurX;/* Current X Value   */
    uint32_t  CurY;/* Current Y Value   */

    D = 3 - (Radius << 1);

    CurX = 0;
    CurY = Radius;

    while (CurX <= CurY)
    {
        if(CurY > 0)
        {
            LCD_DrawLine(Xpos - CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
            LCD_DrawLine(Xpos + CurX, Ypos - CurY, 2*CurY, LCD_DIR_VERTICAL);
        }

        if(CurX > 0)
        {
            LCD_DrawLine(Xpos - CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
            LCD_DrawLine(Xpos + CurY, Ypos - CurX, 2*CurX, LCD_DIR_VERTICAL);
        }
        if (D < 0)
        {
            D += (CurX << 2) + 6;
        }
        else
        {
            D += ((CurX - CurY) << 2) + 10;
            CurY--;
        }
        CurX++;
    }

    LCD_DrawCircle(Xpos, Ypos, Radius);
}

/**
  * @brief  Displays an uni-line (between two points).
  * @param  x1: specifies the point 1 x position.
  * @param  y1: specifies the point 1 y position.
  * @param  x2: specifies the point 2 x position.
  * @param  y2: specifies the point 2 y position.
  * @retval None
  */
void LCD_DrawUniLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = ABS(x2 - x1);        /* The difference between the x's */
    deltay = ABS(y2 - y1);        /* The difference between the y's */
    x = x1;                       /* Start x off at the first pixel */
    y = y1;                       /* Start y off at the first pixel */

    if (x2 >= x1)                 /* The x-values are increasing */
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else                          /* The x-values are decreasing */
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1)                 /* The y-values are increasing */
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else                          /* The y-values are decreasing */
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay)         /* There is at least one x-value for every y-value */
    {
        xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
        yinc2 = 0;                  /* Don't change the y for every iteration */
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;         /* There are more x-values than y-values */
    }
    else                          /* There is at least one y-value for every x-value */
    {
        xinc2 = 0;                  /* Don't change the x for every iteration */
        yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;         /* There are more y-values than x-values */
    }

    for (curpixel = 0; curpixel <= numpixels; curpixel++)
    {
        PutPixel(x, y);             /* Draw the current pixel */
        num += numadd;              /* Increase the numerator by the top of the fraction */
        if (num >= den)             /* Check if numerator >= denominator */
        {
            num -= den;               /* Calculate the new numerator value */
            x += xinc1;               /* Change the x as appropriate */
            y += yinc1;               /* Change the y as appropriate */
        }
        x += xinc2;                 /* Change the x as appropriate */
        y += yinc2;                 /* Change the y as appropriate */
    }
}

/**
  * @brief  Displays an triangle.
  * @param  Points: pointer to the points array.
  * @retval None
  */
void LCD_Triangle(pPoint Points, uint16_t PointCount)
{
    int16_t X = 0, Y = 0;
    pPoint First = Points;

    if(PointCount != 3)
    {
        return;
    }

    while(--PointCount)
    {
        X = Points->X;
        Y = Points->Y;
        Points++;
        LCD_DrawUniLine(X, Y, Points->X, Points->Y);
    }
    LCD_DrawUniLine(First->X, First->Y, Points->X, Points->Y);
}

/**
  * @brief  Fill an triangle (between 3 points).
  * @param  Points: pointer to the points array.
  * @retval None
  */
void LCD_FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{

    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
            yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
            curpixel = 0;

    deltax = ABS(x2 - x1);        /* The difference between the x's */
    deltay = ABS(y2 - y1);        /* The difference between the y's */
    x = x1;                       /* Start x off at the first pixel */
    y = y1;                       /* Start y off at the first pixel */

    if (x2 >= x1)                 /* The x-values are increasing */
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else                          /* The x-values are decreasing */
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1)                 /* The y-values are increasing */
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else                          /* The y-values are decreasing */
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay)         /* There is at least one x-value for every y-value */
    {
        xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
        yinc2 = 0;                  /* Don't change the y for every iteration */
        den = deltax;
        num = deltax / 2;
        numadd = deltay;
        numpixels = deltax;         /* There are more x-values than y-values */
    }
    else                          /* There is at least one y-value for every x-value */
    {
        xinc2 = 0;                  /* Don't change the x for every iteration */
        yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = deltay / 2;
        numadd = deltax;
        numpixels = deltay;         /* There are more y-values than x-values */
    }

    for (curpixel = 0; curpixel <= numpixels; curpixel++)
    {
        LCD_DrawUniLine(x, y, x3, y3);

        num += numadd;              /* Increase the numerator by the top of the fraction */
        if (num >= den)             /* Check if numerator >= denominator */
        {
            num -= den;               /* Calculate the new numerator value */
            x += xinc1;               /* Change the x as appropriate */
            y += yinc1;               /* Change the y as appropriate */
        }
        x += xinc2;                 /* Change the x as appropriate */
        y += yinc2;                 /* Change the y as appropriate */
    }


}
/**
  * @brief  Displays an poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLine(pPoint Points, uint16_t PointCount)
{
    int16_t X = 0, Y = 0;

    if(PointCount < 2)
    {
        return;
    }

    while(--PointCount)
    {
        X = Points->X;
        Y = Points->Y;
        Points++;
        LCD_DrawUniLine(X, Y, Points->X, Points->Y);
    }
}

/**
  * @brief  Displays an relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @param  Closed: specifies if the draw is closed or not.
  *           1: closed, 0 : not closed.
  * @retval None
  */
static void LCD_PolyLineRelativeClosed(pPoint Points, uint16_t PointCount, uint16_t Closed)
{
    int16_t X = 0, Y = 0;
    pPoint First = Points;

    if(PointCount < 2)
    {
        return;
    }
    X = Points->X;
    Y = Points->Y;
    while(--PointCount)
    {
        Points++;
        LCD_DrawUniLine(X, Y, X + Points->X, Y + Points->Y);
        X = X + Points->X;
        Y = Y + Points->Y;
    }
    if(Closed)
    {
        LCD_DrawUniLine(First->X, First->Y, X, Y);
    }
}

/**
  * @brief  Displays a closed poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLine(pPoint Points, uint16_t PointCount)
{
    LCD_PolyLine(Points, PointCount);
    LCD_DrawUniLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);
}

/**
  * @brief  Displays a relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_PolyLineRelative(pPoint Points, uint16_t PointCount)
{
    LCD_PolyLineRelativeClosed(Points, PointCount, 0);
}

/**
  * @brief  Displays a closed relative poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_ClosedPolyLineRelative(pPoint Points, uint16_t PointCount)
{
    LCD_PolyLineRelativeClosed(Points, PointCount, 1);
}


/**
  * @brief  Displays a  full poly-line (between many points).
  * @param  Points: pointer to the points array.
  * @param  PointCount: Number of points.
  * @retval None
  */
void LCD_FillPolyLine(pPoint Points, uint16_t PointCount)
{

    int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
    uint16_t  IMAGE_LEFT = 0, IMAGE_RIGHT = 0, IMAGE_TOP = 0, IMAGE_BOTTOM = 0;

    IMAGE_LEFT = IMAGE_RIGHT = Points->X;
    IMAGE_TOP= IMAGE_BOTTOM = Points->Y;

    for(counter = 1; counter < PointCount; counter++)
    {
        pixelX = POLY_X(counter);
        if(pixelX < IMAGE_LEFT)
        {
            IMAGE_LEFT = pixelX;
        }
        if(pixelX > IMAGE_RIGHT)
        {
            IMAGE_RIGHT = pixelX;
        }

        pixelY = POLY_Y(counter);
        if(pixelY < IMAGE_TOP)
        {
            IMAGE_TOP = pixelY;
        }
        if(pixelY > IMAGE_BOTTOM)
        {
            IMAGE_BOTTOM = pixelY;
        }
    }

    if(PointCount < 2)
    {
        return;
    }

    X_center = (IMAGE_LEFT + IMAGE_RIGHT)/2;
    Y_center = (IMAGE_BOTTOM + IMAGE_TOP)/2;

    X_first = Points->X;
    Y_first = Points->Y;

    while(--PointCount)
    {
        X = Points->X;
        Y = Points->Y;
        Points++;
        X2 = Points->X;
        Y2 = Points->Y;

        LCD_FillTriangle(X, X2, X_center, Y, Y2, Y_center);
        LCD_FillTriangle(X, X_center, X2, Y, Y_center, Y2);
        LCD_FillTriangle(X_center, X2, X, Y_center, Y2, Y);
    }

    LCD_FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
    LCD_FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
    LCD_FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);
}



/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOn(void)
{
	LCD_SetBackLight(200);     //ON/OFF pwm  by jx	
    /* Display On */
//    LTDC_Cmd(ENABLE); /* display ON */
	//GPIO_SetBits(GPIOF,GPIO_Pin_9);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOff(void)
{

	LCD_SetBackLight(0);	
    /* Display Off */
//    LTDC_Cmd(DISABLE);
}

/**
  * @brief  Gets the lcd pixel width.
  * @param  None.
  * @retval the used lcd pixel width
  */
uint32_t LCD_GetPixelWidth(void)
{
    return LCD_PIXEL_WIDTH;
}

/**
  * @brief  Gets the lcd pixel height.
  * @param  None.
  * @retval the used lcd pixel height
  */
uint32_t LCD_GetPixelHeight(void)
{
    return LCD_PIXEL_HEIGHT;
}

/**
  * @brief  Displays a pixel.
  * @param  x: pixel x.
  * @param  y: pixel y.
  * @retval None
  */
static void PutPixel(int16_t x, int16_t y)
{
    if(x < 0 || x > (LCD_PIXEL_WIDTH - 1) || y < 0 || y > (LCD_PIXEL_HEIGHT - 1))
    {
        return;
    }
    /* Draw pixel */
    *(__IO uint16_t*)(CurrentFrameBuffer + 2*((LCD_PIXEL_WIDTH*y) + x)) = CurrentTextColor;
}

/**
//  * @brief  Gets touch screen position for AMPIRE 640x480 LCD
//  * @param  ReadBuffer: buffer filled with the following parameters
//  *                     ReadBuffer [0]: The Byte count (0x0A)
//  *                     ReadBuffer [1]: X1 coordinate [15,8]
//  *                     ReadBuffer [2]: X1 coordinate [7,0]
//  *                     ReadBuffer [3]: Y1 coordinate [15,8]
//  *                     ReadBuffer [4]: Y1 coordinate [7,0]
//  *                     ReadBuffer [5]: X2 coordinate [15,8]
//  *                     ReadBuffer [6]: X2 coordinate [7,0]
//  *                     ReadBuffer [7]: Y2 coordinate [15,8]
//  *                     ReadBuffer [8]: Y2 coordinate [7,0]
//  *                     ReadBuffer [9]: Gesture command (01: Zoom In 02: Zoom out)
//  *                     ReadBuffer [10]: unused byte
//  *
//  * @retval 11 bytes buffer containing the touch panel information
//  */
//uint8_t LCD_GetPosition(uint8_t* ReadBuffer)
//{
//    uint32_t LCD_TimeOut = 0;

//    uint8_t i=0;

//    /* Begin the configuration sequence */
//    I2C_GenerateSTART(LCD_I2C, ENABLE);

//    /* Test on EV5 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_SB))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Transmit the slave address and enable writing operation */
//    I2C_Send7bitAddress(LCD_I2C, LCD_ADDR, I2C_Direction_Transmitter);

//    /* Test on EV6 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_ADDR))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Read status register 2 to clear ADDR flag */
//    LCD_I2C->SR2;

//    /* Test on EV8_1 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_TXE))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Transmit the first address for r/w operations */
//    I2C_SendData(LCD_I2C, 0x00);

//    /* Test on EV8 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_TXE))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Prepare the register value to be sent */
//    I2C_SendData(LCD_I2C, 0x81);

//    /* Test on EV8 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_TXE))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Prepare the register value to be sent */
//    I2C_SendData(LCD_I2C, 0x08);

//    /* Test on EV8_2 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while ((!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_BTF)))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* End the configuration sequence */
//    I2C_GenerateSTOP(LCD_I2C, ENABLE);

//    /* Enable the I2C peripheral */
//    I2C_GenerateSTART(LCD_I2C, ENABLE);

//    /* Test on EV5 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_SB))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Send device address for write */
//    I2C_Send7bitAddress(LCD_I2C, LCD_ADDR, I2C_Direction_Transmitter);

//    /* Test on EV6 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_ADDR))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Read status register 2 to clear ADDR flag */
//    LCD_I2C->SR2;

//    /* Test on EV8 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_TXE))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Send the device's internal address to write to */
//    I2C_SendData(LCD_I2C, 0x8A);

//    /* Send START condition a second time */
//    I2C_GenerateSTART(LCD_I2C, ENABLE);

//    /* Test on EV5 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_SB))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Send IO Expander address for read */
//    I2C_Send7bitAddress(LCD_I2C, LCD_ADDR, I2C_Direction_Receiver);

//    /* Test on EV6 and clear it */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_ADDR))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Read status register 2 to clear ADDR flag */
//    LCD_I2C->SR2;

//    for (i=0; i < 8; i++)
//    {
//        /* Test on EV7 and clear it */
//        LCD_TimeOut = LCD_TIMEOUT_MAX;
//        while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_RXNE))
//        {
//            if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//        }

//        /* Read the byte from the IO Expander */
//        ReadBuffer[i] = I2C_ReceiveData(LCD_I2C);
//    }

//    /* Test on BTF */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_BTF))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Disable Acknowledgement */
//    I2C_AcknowledgeConfig(LCD_I2C, DISABLE);

//    /* Read the first byte from the IO Expander */
//    ReadBuffer[8] = I2C_ReceiveData(LCD_I2C);

//    /* Test on BTF */
//    LCD_TimeOut = LCD_TIMEOUT_MAX;
//    while (!I2C_GetFlagStatus(LCD_I2C, I2C_FLAG_BTF))
//    {
//        if (LCD_TimeOut-- == 0) return(LCD_TimeOutUserCallback());
//    }

//    /* Send STOP Condition */
//    I2C_GenerateSTOP(LCD_I2C, ENABLE);

//    /* Read the byte from the IO Expander */
//    ReadBuffer[9] = I2C_ReceiveData(LCD_I2C);

//    /* Read the byte from the IO Expander */
//    ReadBuffer[10] = I2C_ReceiveData(LCD_I2C);

//    /* Enable Acknowledgement to be ready for another reception */
//    I2C_AcknowledgeConfig(LCD_I2C, ENABLE);

//    return 0;
//}

/**
//  * @brief  LCD_TIMEOUT_UserCallback
//  * @param  None
//  * @retval 0
//  */
//uint8_t LCD_TimeOutUserCallback(void)
//{
//    I2C_InitTypeDef I2C_InitStructure;

//    I2C_GenerateSTOP(LCD_I2C, ENABLE);
//    I2C_SoftwareResetCmd(LCD_I2C, ENABLE);
//    I2C_SoftwareResetCmd(LCD_I2C, DISABLE);

//    LCD_I2C_Config();

//    /* CODEC_I2C peripheral configuration */
//    I2C_DeInit(I2C1);
//    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//    I2C_InitStructure.I2C_OwnAddress1 = 0x33;
//    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;

//    /* Enable the I2C peripheral */
//    I2C_Cmd(LCD_I2C, ENABLE);
//    I2C_Init(LCD_I2C, &I2C_InitStructure);

//    return 1;
//}

/**
//  * @brief  LCD_CheckDevice
//  * @note   The AMPIRE 480x272 and 480x640 LCDs provided within STM324x9I_EVAL board
//  *         doesn抰 have an internal ID register, however for AMPIRE 480x272 LCD we
//  *         can workaround this limitation by detecting the ID of the IO expander
//  *         (STMPE811) mounted on MB1046 daughter board
//  * @param  None.
//  * @retval None.
//  */
//void LCD_CheckDevice(void)
//{
//    /* Initialize IO expander to detect if AMPIRE 480x272 LCD (MB1046) is connected  */
//    IOE_Config();

//    if(IOE_ReadID() == (uint16_t)STMPE811_ID)
//    {
//        CurrentLcd = USE_LCD_AM480272;
//    }
//    /* LCD Ampire 640x480 is used */
//    else
//    {
//        CurrentLcd = USE_LCD_AM640480;
//    }
//}

/**
//  * @brief  Initializes the GPIO pins used for AMPIRE 640x480 touch screen.
//  * @param  None
//  * @retval None
//  */
//static void LCD_I2C_Config(void)
//{
//    I2C_InitTypeDef I2C_InitStructure;
//    GPIO_InitTypeDef GPIO_InitStructure;

//    /* If the I2C peripheral is already enabled, don't reconfigure it */
//    if ((I2C1->CR1 & I2C_CR1_PE) == 0)
//    {
//        /* Enable LCD_I2C and LCD_I2C_GPIO_PORT & Alternate Function clocks */
//        RCC_APB1PeriphClockCmd(LCD_I2C_CLK, ENABLE);
//        RCC_AHB1PeriphClockCmd(LCD_I2C_SCL_GPIO_CLK | LCD_I2C_SDA_GPIO_CLK, ENABLE);
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

//        /* Reset LCD_I2C IP */
//        RCC_APB1PeriphResetCmd(LCD_I2C_CLK, ENABLE);
//        /* Release reset signal of LCD_I2C IP */
//        RCC_APB1PeriphResetCmd(LCD_I2C_CLK, DISABLE);

//        /* Connect PXx to I2C_SCL*/
//        GPIO_PinAFConfig(LCD_I2C_SCL_GPIO_PORT, LCD_I2C_SCL_SOURCE, LCD_I2C_SCL_AF);
//        /* Connect PXx to I2C_SDA*/
//        GPIO_PinAFConfig(LCD_I2C_SDA_GPIO_PORT, LCD_I2C_SDA_SOURCE, LCD_I2C_SDA_AF);

//        /* LCD_I2C SCL and SDA pins configuration */
//        GPIO_InitStructure.GPIO_Pin = LCD_I2C_SCL_PIN;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
//        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//        GPIO_Init(LCD_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

//        GPIO_InitStructure.GPIO_Pin = LCD_I2C_SDA_PIN;
//        GPIO_Init(LCD_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

//        /* LCD_I2C configuration */
//        I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//        I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//        I2C_InitStructure.I2C_OwnAddress1 = 0x00;
//        I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//        I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//        I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;

//        I2C_Init(LCD_I2C, &I2C_InitStructure);
//    }
//}

/**
//  * @brief GPIO configuration for LTDC.
//  * @param  None
//  * @retval None
//  */
#define GPIO_AF_L_09          ((uint8_t)0x09)  //特别的
#if USE_GD == 1
static void LCD_AF_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOG 
						    | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOD ,ENABLE);

    /* GPIOs Configuration */
    /*
     +------------------------+-----------------------+----------------------------+
     +                       LCD pins assignment                                   +
     +------------------------+-----------------------+----------------------------+
     |  LCD_TFT R0 <-> P      |  LCD_TFT G0 <-> P     |  LCD_TFT B0 <-> P          |
     |  LCD_TFT R1 <-> P      |  LCD_TFT G1 <-> P     |  LCD_TFT B1 <-> P          |
     |  LCD_TFT R2 <-> P      |  LCD_TFT G2 <-> PH.13 |  LCD_TFT B2 <-> P          |
     |  LCD_TFT R3 <-> PG.11  |  LCD_TFT G3 <-> PH.14 |  LCD_TFT B3 <-> PB.00      |
     |  LCD_TFT R4 <-> PG.12  |  LCD_TFT G4 <-> PH.15 |  LCD_TFT B4 <-> PA.11      |
     |  LCD_TFT R5 <-> PA.03  |  LCD_TFT G5 <-> PI.00 |  LCD_TFT B5 <-> PA.12      |
     |  LCD_TFT R6 <-> PI.06  |  LCD_TFT G6 <-> PI.01 |  LCD_TFT B6 <-> PA.08      |
     |  LCD_TFT R7 <-> PI.07  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PG.06      |
     -------------------------------------------------------------------------------
              |  LCD_TFT HSYNC <-> PI.9  | LCDTFT VSYNC <->  PI.10 |
              |  LCD_TFT CLK   <-> PG.7  | LCD_TFT DE   <->  PF.10 |
               -----------------------------------------------------
    */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* GPIOI configuration */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* GPIOI configuration */
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_L_09);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_7 ;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOG, &GPIO_InitStruct);


    GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 ;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStruct);
//ADD PWM PF9
/*   by jx
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 ;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStruct);
    GPIO_SetBits(GPIOF,GPIO_Pin_9);
*/
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 ;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_L_09);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LTDC);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

	LCD_PWM_Init();//add by jx
}

#else
static void LCD_AF_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIOI, GPIOJ, GPIOG, GPIOF, GPIOH AHB Clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOG | \
                           RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOB,ENABLE);

    /* GPIOs Configuration */
    /*
     +------------------------+-----------------------+----------------------------+
     +                       LCD pins assignment                                   +
     +------------------------+-----------------------+----------------------------+
     |  LCD_TFT R0 <-> P      |  LCD_TFT G0 <-> P     |  LCD_TFT B0 <-> P          |
     |  LCD_TFT R1 <-> P      |  LCD_TFT G1 <-> P     |  LCD_TFT B1 <-> P          |
     |  LCD_TFT R2 <-> P      |  LCD_TFT G2 <-> PH.13 |  LCD_TFT B2 <-> P          |
     |  LCD_TFT R3 <-> PG.11  |  LCD_TFT G3 <-> PH.14 |  LCD_TFT B3 <-> PB.00      |
     |  LCD_TFT R4 <-> PG.12  |  LCD_TFT G4 <-> PH.15 |  LCD_TFT B4 <-> PA.11      |
     |  LCD_TFT R5 <-> PA.03  |  LCD_TFT G5 <-> PI.00 |  LCD_TFT B5 <-> PA.12      |
     |  LCD_TFT R6 <-> PI.06  |  LCD_TFT G6 <-> PI.01 |  LCD_TFT B6 <-> PA.08      |
     |  LCD_TFT R7 <-> PI.07  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PG.06      |
     -------------------------------------------------------------------------------
              |  LCD_TFT HSYNC <-> PI.9  | LCDTFT VSYNC <->  PI.10 |
              |  LCD_TFT CLK   <-> PG.7  | LCD_TFT DE   <->  PF.10 |
               -----------------------------------------------------
    */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    /* GPIOI configuration */
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource9, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_9| GPIO_Pin_10;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOI, &GPIO_InitStruct);

    /* GPIOJ configuration */

    GPIO_PinAFConfig(GPIOH, GPIO_PinSource13, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource15, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;

    GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* GPIOI configuration */
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource6, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_L_09);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_LTDC);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource7, GPIO_AF_LTDC);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_7 ;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOG, &GPIO_InitStruct);


    GPIO_PinAFConfig(GPIOF, GPIO_PinSource10, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 ;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStruct);
//ADD PWM PF9
/*   by jx
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 ;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStruct);
    GPIO_SetBits(GPIOF,GPIO_Pin_9);
*/
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource3, GPIO_AF_LTDC);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 ;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStruct);


    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_L_09);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 ;

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

	LCD_PWM_Init();//add by jx

}
#endif

//------------------------------ADD by zouw96 15/04/01--------------------------

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : LCD寄存器数据
* Input          : - index: 寄存器数据
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_WriteData(uint32_t GetFrameBuffer,uint16_t data)
{
    *(__IO uint16_t *) (GetFrameBuffer)= data;
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : 读取控制器数据
* Input          : None
* Output         : None
* Return         : 返回读取到的数据
* Attention		 : None
*******************************************************************************/
__inline uint16_t LCD_ReadData(uint32_t GetFrameBuffer)
{
    uint16_t value=0;

    value=*(__IO uint16_t *) (GetFrameBuffer);
    value=*(__IO uint16_t *) (GetFrameBuffer);

    return value;
}

/******************************************************************************
* Function Name  : LCD_GetPoint
* Description    : 获取指定座标的颜色值
* Input          : - Xpos: Row Coordinate
*                  - Xpos: Line Coordinate
* Output         : None
* Return         : Screen Color
* Attention		 : None
*******************************************************************************/
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos)
{

    return LCD_ReadData(LCD_SetCursor(Xpos,Ypos));

}

/******************************************************************************
* Function Name  : LCD_SetPoint
* Description    : 在指定座标画点
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
    LCD_WriteData(LCD_SetCursor(Xpos,Ypos),point);
}

uint32_t DMA2D_OutAdd;
uint32_t DMA2D_InAdd;
void DMA2D_M2M_Config(uint32_t sdram_addr ,uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    DMA2D_InitTypeDef      DMA2D_InitStruct;
    DMA2D_FG_InitTypeDef   DMA2D_FG_InitStruct;

    /* Enable the DMA2D Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

    /* DMA2D configuration */
    DMA2D_DeInit();

    /* Transfer mode */
    DMA2D_InitStruct.DMA2D_Mode = DMA2D_M2M;

    /* Color mode */
    DMA2D_InitStruct.DMA2D_CMode = DMA2D_RGB565;

    DMA2D_OutAdd = CurrentFrameBuffer + 2*(LCD_PIXEL_WIDTH*Ypos + Xpos);

    /* Output Address */
    DMA2D_InitStruct.DMA2D_OutputMemoryAdd = DMA2D_OutAdd;

    /* Number of lines : height */
    DMA2D_InitStruct.DMA2D_NumberOfLine = Height;

    /* Number of pixel per line : width */
    DMA2D_InitStruct.DMA2D_PixelPerLine = Width;

    /* Initialize the alpha and RGB values */
    DMA2D_InitStruct.DMA2D_OutputGreen = 0;
    DMA2D_InitStruct.DMA2D_OutputBlue = 0;
    DMA2D_InitStruct.DMA2D_OutputRed = 0;
    DMA2D_InitStruct.DMA2D_OutputAlpha = 0;

    /* Initialize the output offset */
    DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_PIXEL_WIDTH - Width);

    /* Initialize DMA2D */
    DMA2D_Init(&DMA2D_InitStruct);

    /* Configure default values for foreground */
    DMA2D_FG_StructInit(&DMA2D_FG_InitStruct);

    /* Configure DMA2D foreground color mode */
    DMA2D_FG_InitStruct.DMA2D_FGCM = DMA2D_RGB565;

    /* Configure Input Address */
    DMA2D_FG_InitStruct.DMA2D_FGMA = sdram_addr;

    /* Initialize foreground */
    DMA2D_FGConfig(&DMA2D_FG_InitStruct);

    /* Start Transfer */
    DMA2D_StartTransfer();

    /* Wait for CTC Flag activation */
    while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET)
    {
    }
}

void LCD_Dis16PxTrans(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth,const u8 *gImage,uint16_t _usColor)
{
    u16 i = 0,j = 0,k = 0,id = 0;
    u8 temp = 0;
	uint32_t SetFrameBuffer = 0;
	
    for(i = 0; i<_usHeight; i++)
    {
		SetFrameBuffer = CurrentFrameBuffer + 2*(LCD_PIXEL_WIDTH*(_usY+i) + _usX);
		for(k =0;k<_usWidth/8;k++)
		{
	        temp = gImage[id++];
	        for(j=0; j<8; j++)
	        {
	            if((temp&0x80) == 0x80)
	            {
	                *(__IO uint16_t *)SetFrameBuffer = _usColor;
	            }
				SetFrameBuffer += 2;
	            temp<<=1;
	        }
		}
    }

}

void LCD_SelectBlock(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, u8 code,uint16_t _usColor)
{
    u16 i = 0,k = 0;
    u16 value = 0;
	uint32_t SetFrameBuffer = 0;
    for(i = 0; i<_usHeight; i++)
    {
		SetFrameBuffer = CurrentFrameBuffer + 2*(LCD_PIXEL_WIDTH*(_usY+i) + _usX);
		for(k =0;k<_usWidth;k++)
		{
	        value=*(__IO uint16_t *) (SetFrameBuffer);
	        *(__IO uint16_t *)SetFrameBuffer = 0x11020 - value;//取反
	        SetFrameBuffer += 2;
		}

    }	
	
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*********************************************************************************************************
*	                                 下面的函数被emWin所调用
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: LCD_ConfigLTDC
*	功能说明: 配置LTDC
*	形    参: 无
*	返 回 值: 无
*   笔    记:
*       LCD_TFT 同步时序配置（整理自官方做的一个截图，言简意赅）：
*       ----------------------------------------------------------------------------
*    
*                                                 Total Width
*                             <--------------------------------------------------->
*                       Hsync width HBP             Active Width                HFP
*                             <---><--><--------------------------------------><-->
*                         ____    ____|_______________________________________|____ 
*                             |___|   |                                       |    |
*                                     |                                       |    |
*                         __|         |                                       |    |
*            /|\    /|\  |            |                                       |    |
*             | VSYNC|   |            |                                       |    |
*             |Width\|/  |__          |                                       |    |
*             |     /|\     |         |                                       |    |
*             |  VBP |      |         |                                       |    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |         | / / / / / / / / / / / / / / / / / / / |    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Total    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Heigh    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Active|      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Heigh |      |         |/ / / / / / Active Display Area / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |                                                      |
*             |  VFP |      |                                                      |
*            \|/    \|/_____|______________________________________________________|
*            
*     
*     每个LCD设备都有自己的同步时序值：
*     Horizontal Synchronization (Hsync) 
*     Horizontal Back Porch (HBP)       
*     Active Width                      
*     Horizontal Front Porch (HFP)     
*   
*     Vertical Synchronization (Vsync)  
*     Vertical Back Porch (VBP)         
*     Active Heigh                       
*     Vertical Front Porch (VFP)         
*     
*     LCD_TFT 窗口水平和垂直的起始以及结束位置 :
*     ----------------------------------------------------------------
*   
*     HorizontalStart = (Offset_X + Hsync + HBP);
*     HorizontalStop  = (Offset_X + Hsync + HBP + Window_Width - 1); 
*     VarticalStart   = (Offset_Y + Vsync + VBP);
*     VerticalStop    = (Offset_Y + Vsync + VBP + Window_Heigh - 1);
*
*********************************************************************************************************
*/
/* LCD面板类型 */
enum
{
	LCD_35_480X320 = 0,	/* 3.5寸 480 * 320 */	
	LCD_43_480X272,		/* 4.3寸 480 * 272 */
	LCD_50_480X272,		/* 5.0寸 480 * 272 */
	LCD_50_800X480,		/* 5.0寸 480 * 272 */
	LCD_70_800X480,		/* 7.0寸 800 * 480 */	
	LCD_70_1024X600,	/* 7.0寸 1024 * 600 */		
};
__IO uint16_t Width, Height, HSYNC_W, VSYNC_W, HBP, HFP, VBP, VFP;
uint16_t g_LcdHeight ;			/* 显示屏分辨率-高度 */
uint16_t g_LcdWidth ;			/* 显示屏分辨率-宽度 */
void LCD_ConfigLTDC(void)
{
	LTDC_InitTypeDef       LTDC_InitStruct;
	LTDC_Layer_TypeDef     LTDC_Layerx;
	uint8_t g_LcdType;

	/* 使能LTDC */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE);

	/* 使能DMA2D */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE);

	/* 配置LCD引脚 */
	LCD_AF_GPIOConfig();
    /* Configure the FMC Parallel interface : SDRAM is used as Frame Buffer for LCD need not*/
    SDRAM_FMC_Config();

	/* 配置信号极性 */	
	LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;	/* HSYNC 低电平有效 */
	LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;	/* VSYNC 低电平有效 */
	LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;	/* DE 低电平有效 */
	LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;

	/* 背景色 */
	LTDC_InitStruct.LTDC_BackgroundRedValue = 0;
	LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;
	LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;
	
	/* 
	   LTDC时钟配置说明：
	     函数RCC_PLLSAIConfig的第一个参数是PLLSAI_N，第三个参数数PLLSAI_R。
	     函数RCC_LTDCCLKDivConfig的参数是RCC_PLLSAIDivR。
	   
	   下面举一个例子：PLLSAI_N = 400， PLLSAI_R = 4  RCC_PLLSAIDivR = 2:
	     首先，输入时钟 PLLSAI_VCO Input = HSE_VALUE / PLL_M = 8M / 8 = 1MHz 
	       输出时钟 PLLSAI_VCO Output  = PLLSAI_VCO Input * PLLSAI_N = 1 * 400 = 400 1MHz 
	       PLLLCDCLK = PLLSAI_VCO Output / PLLSAI_R = 400 / 4 = 100 1MHz 
	     最好，LTDC 时钟 = PLLLCDCLK / RCC_PLLSAIDivR = 100 / 2 = 50 1MHz 
	 */
				
	/* 支持6种面板 */
	g_LcdType = LCD_70_800X480;
	switch (g_LcdType)
	{
		case LCD_35_480X320:	/* 3.5寸 480 * 320 */	
			RCC_PLLSAIConfig(429, 2,  4);
			RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
		
			Width = 480;
			Height = 272;
			HSYNC_W = 10;
			HBP = 20;
			HFP = 20;
			VSYNC_W = 20;
			VBP = 20;
			VFP = 20;
			break;
		
		case LCD_43_480X272:		/* 4.3寸 480 * 272  选择LTDC输出20MHz，所有颜色深度都可以选择这个时钟频率 */
			RCC_PLLSAIConfig(280, 2,  7);
			RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);

			Width = 480;
			Height = 272;

			HSYNC_W = 40;
			HBP = 2;
			HFP = 2;
			VSYNC_W = 9;
			VBP = 2;
			VFP = 2;
			break;
		
		case LCD_50_480X272:		/* 5.0寸 480 * 272 */
			RCC_PLLSAIConfig(429, 2,  4);
			RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);
		
			Width = 480;
			Height = 272;
		
			HSYNC_W = 40;
			HBP = 2;
			HFP = 2;
			VSYNC_W = 9;
			VBP = 2;
			VFP = 2;			
			break;
		
		case LCD_50_800X480:		/* 5.0寸 800 * 480，24位或者32位色选择LTDC输出15MHz，16位或者8位30MHz */
			RCC_PLLSAIConfig(420, 2,  7);
			RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);

			Width = 800;
			Height = 480;

			HSYNC_W = 96;	
			HBP = 10;
			HFP = 10;
			VSYNC_W = 2;
			VBP = 10;
			VFP = 10;			
			break;
		
		case LCD_70_800X480:		/* 7.0寸 800 * 480，24位或者32位色选择LTDC输出15MHz，16位或者8位30MHz*/
    /* Enable Pixel Clock --------------------------------------------------------*/
    /* Configure PLLSAI prescalers for LCD */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAI_N = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAI_R = 192/3 = 64 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 64/2 = 32 Mhz */
			
//			RCC_PLLSAIConfig(420, 2,  7);
//			RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);
 			RCC_PLLSAIConfig(192, 2, 3);
  			RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);//192/3/2
//		    RCC_PLLSAIConfig(400, 2, 5);
//		    RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);//40	
			Width = 800;
			Height = 480;

//			HSYNC_W = 90;	
//			HBP = 10;
//			HFP = 10;
//		
//			VSYNC_W = 10;
//			VBP = 10;
//			VFP = 10;	

			HSYNC_W = 64;	
			HBP = 64;
			HFP = 128;
		
			VSYNC_W = 11;
			VBP = 11;
			VFP = 22;	
			break;
		
		case LCD_70_1024X600:		/* 7.0寸 1024 * 600 */
			LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;	/* HSYNC 低电平有效 */
			LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;	/* VSYNC 低电平有效 */
			LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;	/* DE 低电平有效 */
			LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IIPC;
		
			/* IPS 7寸 1024*600，  像素时钟频率范围 : 57 -- 65 --- 70.5MHz 
		
				PLLSAI_VCO Input   = HSE_VALUE / PLL_M = 8M / 4 = 2 Mhz
				PLLSAI_VCO Output  = PLLSAI_VCO Input * PLLSAI_N =   2 * 429 = 858 Mhz
				PLLLCDCLK = PLLSAI_VCO Output / PLLSAI_R = 858 / 4 = 214.5 Mhz
				LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDivR = 214.5 / 4 = 53.625 Mhz 	

				(429, 2, 4); RCC_PLLSAIDivR_Div4 实测像素时钟 = 53.7M
			*/
			RCC_PLLSAIConfig(429, 2, 6);
			RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div4);
		
			Width = 1024;
			Height = 600;

			HSYNC_W = 2;	/* =10时，显示错位，20时部分屏可以的,80时全部OK */
			HBP = 157;
			HFP = 160;
		
			VSYNC_W = 2;
			VBP = 20;
			VFP = 12;			
			break;
		
		default:
			RCC_PLLSAIConfig(429, 2,  4);
			RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div8);

			Width = 800;
			Height = 480;

			HSYNC_W = 80;	/* =10时，显示错位，20时部分屏可以的,80时全部OK */
			HBP = 10;
			HFP = 10;
			VSYNC_W = 10;
			VBP = 10;
			VFP = 10;			
			break;
	}
	
	g_LcdWidth  = Width;		/* 显示屏分辨率-宽度 */
	g_LcdHeight = Height;		/* 显示屏分辨率-高度 */
	
	/* 使能 PLLSAI */
	RCC_PLLSAICmd(ENABLE);
	/* 等待完成 */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);
	
	/* 配置LTDC的同步时序 */
	LTDC_InitStruct.LTDC_HorizontalSync = HSYNC_W;
	LTDC_InitStruct.LTDC_VerticalSync = VSYNC_W;
	LTDC_InitStruct.LTDC_AccumulatedHBP = LTDC_InitStruct.LTDC_HorizontalSync + HBP;
	LTDC_InitStruct.LTDC_AccumulatedVBP = LTDC_InitStruct.LTDC_VerticalSync + VBP;
	LTDC_InitStruct.LTDC_AccumulatedActiveW = Width + LTDC_InitStruct.LTDC_AccumulatedHBP;
	LTDC_InitStruct.LTDC_AccumulatedActiveH = Height + LTDC_InitStruct.LTDC_AccumulatedVBP;
	LTDC_InitStruct.LTDC_TotalWidth = LTDC_InitStruct.LTDC_AccumulatedActiveW + HFP;
	LTDC_InitStruct.LTDC_TotalHeigh = LTDC_InitStruct.LTDC_AccumulatedActiveH + VFP;

	LTDC_Init(&LTDC_InitStruct);
}





/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
