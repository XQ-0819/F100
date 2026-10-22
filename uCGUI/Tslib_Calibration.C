#include "tslib_calibration.h"
#include "gui.h"
#include "ads7843.h"
#include "eeprom.h"
#include "datasave.h"
#include "parameters.h"
#include "app.h"
#include "ucos_ii.h"

int perform_calibration(calibration *cal)
{
    int j;
    float n, x, y, x2, y2, xy, z, zx, zy;
    float det, a, b, c, e, f, i;
    float scaling = 65536.0;

// Get sums for matrix
    n = x = y = x2 = y2 = xy = 0;

    for (j = 0; j < 5; j++) {
        n += 1.0;
        x += (float)cal->x[j];
        y += (float)cal->y[j];
        x2 += (float)(cal->x[j] * cal->x[j]);
        y2 += (float)(cal->y[j] * cal->y[j]);
        xy += (float)(cal->x[j] * cal->y[j]);
    }

//        printf("%f %f %f\n\n",x,y,n);
//        printf("%f %f %f\n\n",x2,xy,x);
//       printf("%f %f %f\n\n",xy,y2,y);
// Get determinant of matrix -- check if determinant is too small
    det = n * (x2 * y2 - xy * xy) + x * (xy * y - x * y2) + y * (x * xy - y * x2);

    if (det < 0.1 && det > -0.1) {
//            printf("ts_calibrate: determinant is too small -- %f\n",det);
        return 0;
    }

// Get elements of inverse matrix
    a = (x2 * y2 - xy * xy) / det;
    b = (xy * y - x * y2) / det;
    c = (x * xy - y * x2) / det;
    e = (n * y2 - y * y) / det;
    f = (x * y - n * xy) / det;
    i = (n * x2 - x * x) / det;
    //printf("%f %f %f\n\n",a,b,c);
    //printf("%f %f %f\n\n",e,f,i);
// Get sums for x calibration
    z = zx = zy = 0;

    for (j = 0; j < 5; j++) {
        z += (float)cal->xfb[j];
        zx += (float)(cal->xfb[j] * cal->x[j]);
        zy += (float)(cal->xfb[j] * cal->y[j]);
    }

//        printf("%f %f %f\n\n",z,zx,zy);
// Now multiply out to get the calibration for framebuffer x coord
    cal->a[0] = (int)((a * z + b * zx + c * zy) * (scaling));
    cal->a[1] = (int)((b * z + e * zx + f * zy) * (scaling));
    cal->a[2] = (int)((c * z + f * zx + i * zy) * (scaling));

//    printf("%f %f %f\n",(a*z + b*zx + c*zy),
//           (b*z + e*zx + f*zy),
//           (c*z + f*zx + i*zy));

// Get sums for y calibration
    z = zx = zy = 0;

    for (j = 0; j < 5; j++) {
        z += (float)cal->yfb[j];
        zx += (float)(cal->yfb[j] * cal->x[j]);
        zy += (float)(cal->yfb[j] * cal->y[j]);
    }

//        printf("\n\n%f %f %f\n\n",z,zx,zy);
// Now multiply out to get the calibration for framebuffer y coord
    cal->a[3] = (int)((a * z + b * zx + c * zy) * (scaling));
    cal->a[4] = (int)((b * z + e * zx + f * zy) * (scaling));
    cal->a[5] = (int)((c * z + f * zx + i * zy) * (scaling));

//      printf("%f %f %f\n",(a*z + b*zx + c*zy),
//                       (b*z + e*zx + f*zy),
//                       (c*z + f*zx + i*zy));
//          printf("%d %d %d \n\n%d %d %d\n\n",cal->a[0],cal->a[1],cal->a[2],cal->a[3],cal->a[4],cal->a[5],cal->a[6]);


// If we got here, we're OK, so assign scaling to a[6] and return
    cal->a[6] = (int)scaling;
    return 1;
}


static void _StoreUnstable(int x, int y)
{
    static int _xLast = -1;
    static int _yLast = -1;
    int xOut, yOut;

    if ((x != -1) && (y != -1) && (_xLast != -1) && (_yLast != -1)) {
        xOut = _xLast;
        yOut = _yLast;
    } else {
        xOut = -1;
        yOut = -1;
    }

    _xLast = x;
    _yLast = y;
    GUI_TOUCH_StoreUnstable(xOut, yOut);
}

void GUI_TOUCH(void)
{
    int x, y;
	TOUCH_SCAN();

    x = g_tTP.usAdcNowX;
    y = g_tTP.usAdcNowY;

    if ((x < 0) || (y < 0) ) {
        _StoreUnstable(-1, -1);
    } else {
        x = AD2GUI_COORDX(x, y);
        y = AD2GUI_COORDY(x, y);
        _StoreUnstable(x, y);	
    }

}
calibration cal;
__IO u8 calibration_break_flag = 0;
void Calibration(void)
{
//   // unsigned int  x ,y ,Tx,Ty;
//   // int ax_Phys[2],ay_Phys[2];
    /* calculate log. Positions */
    int ax[5] = { 20,  LCD_XSIZE - 20, LCD_XSIZE - 20, 20 ,           LCD_XSIZE / 2};
    int ay[5] = { 20,  20,           LCD_YSIZE - 20, LCD_YSIZE - 20,  LCD_YSIZE / 2};

    GUI_PID_STATE State;
    calibration_break_flag = 0;


    /* _Calibrate upper left */
//	GUI_TOUCH_SetDefaultCalibration();
    GUI_SetBkColor(GUI_RED);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(&GUI_Font16_ASCII);
    GUI_FillCircle(ax[0], ay[0], 10);
    GUI_SetColor(GUI_RED);
    GUI_FillCircle(ax[0], ay[0], 5);
    GUI_SetColor(GUI_WHITE);
    GUI_DispStringAt("Press here", ax[0] + 20, ay[0]);

    do {
        if (calibration_break_flag == 1) {
            return ;
        }

//        GUI_PID_STATE State;
//        GUI_TOUCH();
        GUI_TOUCH_GetState(&State);

        if (State.Pressed) {
            cal.xfb[0] = 20;
            cal.yfb[0] = 20;
            cal.x[0] = g_tTP.usAdcNowX;
            cal.y[0] = g_tTP.usAdcNowY;
            break;
        }

        IWDG_ReloadCounter();
        GUI_Delay (30);
    } while (1);

    GUI_Clear();
    GUI_DispStringAt("OK", ax[0] + 20, ay[0]);

    do {
        if (calibration_break_flag == 1) {
            return ;
        }

//        GUI_PID_STATE State;
//        GUI_TOUCH();
        GUI_TOUCH_GetState(&State);

        if (State.Pressed == 0) {
            break;
        }

        IWDG_ReloadCounter();
        GUI_Delay (30);
    } while (1);

    GUI_SetBkColor(GUI_RED);
    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_FillCircle(ax[1], ay[1], 10);
    GUI_SetColor(GUI_RED);
    GUI_FillCircle(ax[1], ay[1], 5);
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextAlign(GUI_TA_RIGHT);
    GUI_DispStringAt("Press here", ax[1] - 20, ay[1]);

    do {
        if (calibration_break_flag == 1) {
            return ;
        }

//        GUI_PID_STATE State;
//        GUI_TOUCH();
        GUI_TOUCH_GetState(&State);

        if (State.Pressed) {
            cal.xfb[1] = LCD_XSIZE - 20;
            cal.yfb[1] = 20;
            cal.x[1] = g_tTP.usAdcNowX;
            cal.y[1] = g_tTP.usAdcNowY;
			break;
        }

        IWDG_ReloadCounter();
        GUI_Delay (30);
    } while (1);

    GUI_Clear();
    GUI_DispStringAt("OK", ax[1] - 20, ay[1]);

    do {
        if (calibration_break_flag == 1) {
            return ;
        }

//        GUI_PID_STATE State;
//        GUI_TOUCH();
        GUI_TOUCH_GetState(&State);

        if (State.Pressed == 0) {
            break;
        }

        IWDG_ReloadCounter();
        GUI_Delay (30);
    } while (1);

    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_FillCircle(ax[2], ay[2], 10);
    GUI_SetColor(GUI_RED);
    GUI_FillCircle(ax[2], ay[2], 5);
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextAlign(GUI_TA_RIGHT);
    GUI_DispStringAt("Press here", ax[2] - 20, ay[2]);

    do {
        if (calibration_break_flag == 1) {
            return ;
        }

//       GUI_PID_STATE State;
//        GUI_TOUCH();
        GUI_TOUCH_GetState(&State);

        if (State.Pressed) {
            cal.xfb[2] = LCD_XSIZE - 20;
            cal.yfb[2] = LCD_YSIZE - 20;
            cal.x[2] = g_tTP.usAdcNowX;
            cal.y[2] = g_tTP.usAdcNowY;
            break;
        }

        IWDG_ReloadCounter();
        GUI_Delay (30);
    } while (1);

    GUI_Clear();
    GUI_DispStringAt("OK", ax[2] - 20, ay[2]);

    do {
        if (calibration_break_flag == 1) {
            return ;
        }

//        GUI_PID_STATE State;
//        GUI_TOUCH();
        GUI_TOUCH_GetState(&State);

        if (State.Pressed == 0) {
            break;
        }

        IWDG_ReloadCounter();
        GUI_Delay (30);
    } while (1);

    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_FillCircle(ax[3], ay[3], 10);
    GUI_SetColor(GUI_RED);
    GUI_FillCircle(ax[3], ay[3], 5);
    GUI_SetColor(GUI_WHITE);
    //GUI_SetTextAlign(GUI_TA_RIGHT);
    GUI_SetTextAlign(GUI_TA_LEFT);
    GUI_DispStringAt("Press here", ax[3] + 20, ay[3]);

    do {
        if (calibration_break_flag == 1) {
            return ;
        }

//       GUI_PID_STATE State;
//        GUI_TOUCH();
        GUI_TOUCH_GetState(&State);

        if (State.Pressed) {
            cal.xfb[3] = 20;
            cal.yfb[3] = LCD_YSIZE - 20;
            cal.x[3] = g_tTP.usAdcNowX;
            cal.y[3] = g_tTP.usAdcNowY;
            break;
        }

        IWDG_ReloadCounter();
        GUI_Delay (30);
    } while (1);

    GUI_Clear();
    GUI_DispStringAt("OK", ax[3] + 20, ay[3]);

    do {
        if (calibration_break_flag == 1) {
            return ;
        }

//        GUI_PID_STATE State;
//        GUI_TOUCH();
        GUI_TOUCH_GetState(&State);

        if (State.Pressed == 0) {
            break;
        }

        IWDG_ReloadCounter();
        GUI_Delay (30);
    } while (1);

    GUI_Clear();
    GUI_SetColor(GUI_WHITE);
    GUI_FillCircle(ax[4], ay[4], 10);
    GUI_SetColor(GUI_RED);
    GUI_FillCircle(ax[4], ay[4], 5);
    GUI_SetColor(GUI_WHITE);
    GUI_SetTextAlign(GUI_TA_LEFT);
    GUI_DispStringAt("Press here", ax[4] + 20, ay[4]);

    do {
        if (calibration_break_flag == 1) {
            return ;
        }

//        GUI_PID_STATE State;
//        GUI_TOUCH();
        GUI_TOUCH_GetState(&State);

        if (State.Pressed) {
            cal.xfb[4] = LCD_XSIZE / 2;
            cal.yfb[4] = LCD_YSIZE / 2;
            cal.x[4] = g_tTP.usAdcNowX;
            cal.y[4] = g_tTP.usAdcNowY;
            break;
        }

        IWDG_ReloadCounter();
        GUI_Delay (30);
    } while (1);

    GUI_Clear();
    GUI_DispStringAt("OK", ax[4] + 20, ay[4]);

    do {
        if (calibration_break_flag == 1) {
            return ;
        }

//       GUI_PID_STATE State;
//        GUI_TOUCH();
        GUI_TOUCH_GetState(&State);

        if (State.Pressed == 0) {
            break;
        }

        IWDG_ReloadCounter();
        GUI_Delay (30);
    } while (1);

    if (perform_calibration(&cal) == 0) {
        return ;
    }

//	sd_writeinfor("config/touch.db", (u8 *)cal.a, 0,28);
//	SST25_Flash_Write(TOUCH_ADDR, (u8 *)&cal.a[0], TOUCH_NUM);
//	FM24CXX_Write(TOUCH_ADDR, (u8 *)&cal.a[0], TOUCH_NUM);

    EEPROM_Write( EE_CAL_ADDR, (INT8U*)&cal.a[0], sizeof(cal.a) );

//  ret = save_data(FM_FUNTION_CALIB_LCD,0);
//	if(ret == false){
//    g_mssage_type = MESSAGE_MEM_ERR;
//    ret = message_Task(0);
//	}

    GUI_Clear();
    GUI_DispString  ("x0: ");
    GUI_DispDec(cal.x[0], 4);
    GUI_DispString  ("  y0: ");
    GUI_DispDec(cal.y[0], 4);
    GUI_DispNextLine();

    GUI_DispString  ("x1: ");
    GUI_DispDec(cal.x[1], 4);
    GUI_DispString  ("  y1: ");
    GUI_DispDec(cal.y[1], 4);
    GUI_DispNextLine();

    GUI_DispString  ("x2: ");
    GUI_DispDec(cal.x[2], 4);
    GUI_DispString  ("  y2: ");
    GUI_DispDec(cal.y[2], 4);
    GUI_DispNextLine();

    GUI_DispString  ("x3: ");
    GUI_DispDec(cal.x[3], 4);
    GUI_DispString  ("  y3: ");
    GUI_DispDec(cal.y[3], 4);
    GUI_DispNextLine();

    GUI_DispString  ("x4: ");
    GUI_DispDec(cal.x[4], 4);
    GUI_DispString  ("  y4: ");
    GUI_DispDec(cal.y[4], 4);
    GUI_DispNextLine();

    GUI_DispString  ("a0: ");
    GUI_DispHex(cal.a[0], 8);
    GUI_DispString  ("  a1: ");
    GUI_DispHex(cal.a[1], 8);
    GUI_DispNextLine();

    GUI_DispString  ("a2: ");
    GUI_DispHex(cal.a[2], 8);
    GUI_DispString  ("  a3: ");
    GUI_DispHex(cal.a[3], 8);
    GUI_DispNextLine();

    GUI_DispString  ("a4: ");
    GUI_DispHex(cal.a[4], 8);
    GUI_DispString  ("  a5: ");
    GUI_DispHex(cal.a[5], 8);
    GUI_DispNextLine();

    GUI_DispString  ("a6: ");
    GUI_DispHex(cal.a[6], 8);
    GUI_DispNextLine();
//    GUI_DispString  ("Please touch display to continue...");

//    do
//    {
//
//        GUI_PID_STATE State;
//        GUI_TOUCH_Exec();
//		GUI_TOUCH_GetState(&State);
//        if (State.Pressed)
//            break;
//        GUI_Delay (30);
//    }
//    while (1);

}



//void Calibration(void)
//{
//  //    unsigned int  x ,y ,Tx,Ty;
//  //    int ax_Phys[2],ay_Phys[2];
//    /* calculate log. Positions */
//    int ax[5] = { 20,  LCD_XSIZE -20,LCD_XSIZE -20,20 ,           LCD_XSIZE/2};
//    int ay[5] = { 20,  20,           LCD_YSIZE-20, LCD_YSIZE-20,  LCD_YSIZE/2};

//    /* _Calibrate upper left */
//    GUI_TOUCH_SetDefaultCalibration();
//    GUI_SetBkColor(GUI_RED);
//    GUI_Clear();
//    GUI_SetColor(GUI_WHITE);
//    GUI_FillCircle(ax[0], ay[0], 10);
//    GUI_SetColor(GUI_RED);
//    GUI_FillCircle(ax[0], ay[0], 5);
//    GUI_SetColor(GUI_WHITE);
//    GUI_DispStringAt("Press here", ax[0]+20, ay[0]);
//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            cal.xfb[0] = 20;
//            cal.yfb[0] = 20;
//            cal.x[0] = g_tTP.usAdcNowX;
//            cal.y[0] = g_tTP.usAdcNowY;
//            break;
//        }
//        GUI_Delay (100);
//    }
//    while (1);

//    GUI_Clear();
//    GUI_DispStringAt("OK", ax[0]+20, ay[0]);
//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            break;
//        }
//        GUI_Delay (100);
//    }
//    while (1);

//    GUI_SetBkColor(GUI_RED);
//    GUI_Clear();
//    GUI_SetColor(GUI_WHITE);
//    GUI_FillCircle(ax[1], ay[1], 10);
//    GUI_SetColor(GUI_RED);
//    GUI_FillCircle(ax[1], ay[1], 5);
//    GUI_SetColor(GUI_WHITE);
//    GUI_SetTextAlign(GUI_TA_RIGHT);
//    GUI_DispStringAt("Press here", ax[1]-20, ay[1]);
//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            cal.xfb[1] = 20;
//            cal.yfb[1] = 20;
//            cal.x[1] = g_tTP.usAdcNowX;
//            cal.y[1] = g_tTP.usAdcNowY;
//            break;
//        }
//        GUI_Delay (100);
//    }
//    while (1);
//    GUI_Clear();
//    GUI_DispStringAt("OK", ax[1]-20, ay[1]);
//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            break;
//        }
//        GUI_Delay (100);
//    }
//    while (1);
//    GUI_Clear();
//    GUI_SetColor(GUI_WHITE);
//    GUI_FillCircle(ax[2], ay[2], 10);
//    GUI_SetColor(GUI_RED);
//    GUI_FillCircle(ax[2], ay[2], 5);
//    GUI_SetColor(GUI_WHITE);
//    GUI_SetTextAlign(GUI_TA_RIGHT);
//    GUI_DispStringAt("Press here", ax[2]-20, ay[2]);
//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            cal.xfb[2] = 20;
//            cal.yfb[2] = 20;
//            cal.x[2] = g_tTP.usAdcNowX;
//            cal.y[2] = g_tTP.usAdcNowY;
//            break;
//        }
//        GUI_Delay (100);
//    }
//    while (1);
//    GUI_Clear();
//    GUI_DispStringAt("OK", ax[2]-20, ay[2]);
//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            break;
//        }
//        GUI_Delay (100);
//    }
//    while (1);
//    GUI_Clear();
//    GUI_SetColor(GUI_WHITE);
//    GUI_FillCircle(ax[3], ay[3], 10);
//    GUI_SetColor(GUI_RED);
//    GUI_FillCircle(ax[3], ay[3], 5);
//    GUI_SetColor(GUI_WHITE);
//    //GUI_SetTextAlign(GUI_TA_RIGHT);
//    GUI_SetTextAlign(GUI_TA_LEFT);
//    GUI_DispStringAt("Press here", ax[3]+20, ay[3]);
//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            cal.xfb[3] = 20;
//            cal.yfb[3] = 20;
//            cal.x[3] = g_tTP.usAdcNowX;
//            cal.y[3] = g_tTP.usAdcNowY;
//            break;
//        }
//        GUI_Delay (100);
//    }
//    while (1);
//    GUI_Clear();
//    GUI_DispStringAt("OK", ax[3]+20, ay[3]);
//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            break;
//        }
//        GUI_Delay (100);
//    }
//    while (1);
//    GUI_Clear();
//    GUI_SetColor(GUI_WHITE);
//    GUI_FillCircle(ax[4], ay[4], 10);
//    GUI_SetColor(GUI_RED);
//    GUI_FillCircle(ax[4], ay[4], 5);
//    GUI_SetColor(GUI_WHITE);
//    GUI_SetTextAlign(GUI_TA_LEFT);
//    GUI_DispStringAt("Press here", ax[4]+20, ay[4]);
//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            cal.xfb[4] = 20;
//            cal.yfb[4] = 20;
//            cal.x[4] = g_tTP.usAdcNowX;
//            cal.y[4] = g_tTP.usAdcNowY;
//            break;
//        }
//        GUI_Delay (100);
//    }
//    while (1);
//    GUI_Clear();
//    GUI_DispStringAt("OK", ax[4]+20, ay[4]);
//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            break;
//        }
//		GUI_Delay (100);
//    }
//    while (1);
//    perform_calibration(&cal);
//    GUI_Clear();
//    GUI_DispString  ("x0: ");
//    GUI_DispDec(cal.x[0], 4);
//    GUI_DispString  ("  y0: ");
//    GUI_DispDec(cal.y[0], 4);
//    GUI_DispNextLine();

//    GUI_DispString  ("x1: ");
//    GUI_DispDec(cal.x[1], 4);
//    GUI_DispString  ("  y1: ");
//    GUI_DispDec(cal.y[1], 4);
//    GUI_DispNextLine();

//    GUI_DispString  ("x2: ");
//    GUI_DispDec(cal.x[2], 4);
//    GUI_DispString  ("  y2: ");
//    GUI_DispDec(cal.y[2], 4);
//    GUI_DispNextLine();

//    GUI_DispString  ("x3: ");
//    GUI_DispDec(cal.x[3], 4);
//    GUI_DispString  ("  y3: ");
//    GUI_DispDec(cal.y[3], 4);
//    GUI_DispNextLine();

//    GUI_DispString  ("x4: ");
//    GUI_DispDec(cal.x[4], 4);
//    GUI_DispString  ("  y4: ");
//    GUI_DispDec(cal.y[4], 4);
//    GUI_DispNextLine();

//	GUI_DispString  ("a0: ");
//    GUI_DispHex(cal.a[0], 8);
//    GUI_DispString  ("  a1: ");
//    GUI_DispHex(cal.a[1], 8);
//    GUI_DispNextLine();

//	GUI_DispString  ("a2: ");
//    GUI_DispHex(cal.a[2], 8);
//    GUI_DispString  ("  a3: ");
//    GUI_DispHex(cal.a[3], 8);
//    GUI_DispNextLine();

//	GUI_DispString  ("a4: ");
//    GUI_DispHex(cal.a[4], 8);
//    GUI_DispString  ("  a5: ");
//    GUI_DispHex(cal.a[5], 8);
//    GUI_DispNextLine();
//
//	GUI_DispString  ("a6: ");
//    GUI_DispDec(cal.a[6], 8);
//    GUI_DispNextLine();

//
//    GUI_DispString  ("Please touch display to continue...");

//    do
//    {
//		TOUCH_SCAN();
//        if (g_tTP.usAdcNowX != -1&&g_tTP.usAdcNowY != -1)
//        {
//            break;
//        }
//		GUI_Delay (100);
//    }
//    while (1);
//}

//以下为转换函数，转换得到lcd的实际值
int  AD2GUI_COORDX(unsigned  int ad_x, unsigned int ad_y)
{
    int temp;
    temp = (unsigned int)((cal.a[0] + ad_x * cal.a[1] + ad_x * cal.a[2]) / cal.a[6]);
    return temp;
}
int  AD2GUI_COORDY(unsigned  int ad_x, unsigned int ad_y)
{
    int temp;
    temp = (unsigned int)((cal.a[3] + ad_x * cal.a[4] + ad_y * cal.a[5]) / cal.a[6]);
    return temp;
}

void Touch_Task(void * pdata)
{
    for (;;) {
		GUI_TOUCH();
        OSTimeDly(10);
    }
}

