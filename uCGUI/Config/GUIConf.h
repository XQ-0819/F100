/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUIConf.h
Purpose     : Configures abilities, fonts etc.
----------------------------------------------------------------------
*/


#ifndef GUICONF_H
#define GUICONF_H


#define tslib_calibration 0  //是否采用tslib5点校准。修改函数在GUI_TOUCH_DriverAnalog中的GUI_TOUCH_Exec函数

/*********************************************************************
*
*       Multi layer/display support
*/
#define GUI_NUM_LAYERS            1    // Maximum number of available layers

#define GUI_OS                    (0)  /* Compile with multitasking support */
#define GUI_SUPPORT_TOUCH         (1)  /* Support a touch screen (req. win-manager) */
#define GUI_SUPPORT_UNICODE       (1)  /* Support mixed ASCII/UNICODE strings */

#define GUI_DEFAULT_FONT          &GUI_Font6x8
//#define GUI_ALLOC_SIZE            20000  /* Size of dynamic memory ... For WM and memory devices*/
//绿  0xefedac 0xdbcd7b 0xc9cd23
//青  0xd4d786 0xa69e40
//淡  0xd4d6b6
//#define GUI_MENU_COLOR 	0xd4d6b6		
//#define GUI_BACK_COLOR	0xc9cd23
//#define GUI_BUTTON_COLOR 0xa69e40
//#define GUI_FOUCE_COLOR GUI_YELLOW


/*********************************************************************
*
*         Configuration of available packages
*/


//#define GUI_SUPPORT_AA            0  /* Anti aliasing available */
#define GUI_SUPPORT_MOUSE             (0)    /* Support a mouse */
#define GUI_WINSUPPORT                (1)    /* Use window manager */
#define GUI_SUPPORT_MEMDEV            (1)    /* Memory device package available */
#define GUI_SUPPORT_DEVICES           (0)    /* Enable use of device pointers */
#define GUI_USE_ARGB 		 0    // Use ABGR
#endif  /* Avoid multiple inclusion */



