  
/******************************************************************************
  Copyright (c), 2000-2020, Froxy Company
  
  File: message_show.h
  Description: 
  Function List: //List main functions
  Revision:
         1. Author: yang   yangchongxin@126.com
            Date:  2017-12-17 21:30:30
            Changes: First create file
 ******************************************************************************/


#ifndef _MESSAGE_SHOW_H_
#define _MESSAGE_SHOW_H_


#ifdef __cplusplus
 extern "C" {
#endif

/****************************************************************************** 
 *                                 INCLUDE                                      * 
 ******************************************************************************/
#include "gui.h"
#include "WM.h"

#define MESSAGE_YES (1)
#define MESSAGE_NO  (0)

#define MESSAGE_NEW_GROUP 		(0)	//建立新群组
#define MESSAGE_MEM_GROUP_CLC  	(1)	//清除群号提醒
#define MESSAGE_SET_DEF 		(2)	//恢复出厂设置
#define MESSAGE_HARD_TEST 		(3)	//硬件测试
#define MESSAGE_ADJ_TIPS 		(4)	//校准提醒
#define MESSAGE_SOFT_VER 		(5)	//软件版本
#define MESSAGE_DEATH_TIPS 		(6)	//添加死亡量
#define MESSAGE_ADD_TIPS 		(7)	//动物添加量
#define MESSAGE_LVCOPY_TIPS 	(8)	//级别复制提示
#define MESSAGE_HOME_STYLE  	(9)	//主页风格提示
#define CONFIRM_NEW_GROUP 		(10)//确认建立新群
#define MESSAGE_RESTART         (11)//设备重启提醒
#define MESSAGE_FOOD_ADJ        (12)//料塔校准
#define MESSAGE_SAVE_PARA       (13)//保存当前参数
#define MESSAGE_IMPORT_PARA     (14)//导入当前参数
#define MESSAGE_ALARM_SHILED	(15)//报警屏蔽


//#define MESSAGE_MEM_ERR   		(1)	//内存错误提示
//#define MESSAGE_MEM_GROUP_NEW 	(3)	//提示勾选新群复选框
//#define MESSAGE_USB_OUT_OK    	(4)	//数据导出成功
//#define MESSAGE_USB_IN_OK    	(5)	//数据导入成功
//#define MESSAGE_USB_ERROR_OK    (6)	//数据导入成功
//#define MESSAGE_MEM_TEST_OK   (7)	//存储测试通过
//#define MESSAGE_MEM_TEST_ERR  (8)	//存储测试未通过
//#define MESSAGE_AUTO_CAL 		(9)	//自动校准中


/****************************************************************************** 
 *                                 MACRO                                      * 
 ******************************************************************************/





/****************************************************************************** 
 *                                 ENUM                                       * 
 ******************************************************************************/








/****************************************************************************** 
 *                               FUNCTION                                     * 
 ******************************************************************************/

//extern WM_HWIN message_hWin;
U8 MMessage_Task(WM_HWIN hParent,U8 type);


/****************************************************************************** 
 *                                 END                                        * 
 ******************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* _MESSAGE_SHOW_H_ */



