/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file includes the usb host library user callbacks
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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
#include <string.h>
#include "usbh_usr.h"
#include "ff.h"       /* FATFS */
#include "diskio.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "usb_hcd_int.h"
#include "ucos_ii.h"
#include "datasave.h"

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;




//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
u8 exf_getfree(u8 *drv,u32 *total,u32 *free) {
	FATFS *fs1;
	u8 res;
	u32 fre_clust=0, fre_sect=0, tot_sect=0;
	//得到磁盘信息及空闲簇数量
	res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
	if(res==0) {
		tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
		fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数
#if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB
	}
	return res;
}


//用户测试主程序
//返回值:0,正常
//       1,有问题
int USH_User_App(void) {
	u32 total,free;
	int res=0;
	res=exf_getfree("2:",&total,&free);
	if(res==0) {
		total = total>>10;
		free = free>>10;
	}
	while(HCD_IsDeviceConnected(&USB_OTG_Core)) { //设备连接成功
		
#ifdef OS_TICKS_PER_SEC     //如果时钟节拍数定义了,说明要使用ucosII了. 
	OSTimeDlyHMSM(0, 0,0,400);
#endif
	}
	res = 1;
	return res;
}

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_MSC_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR
* @brief    This file includes the usb host stack user callbacks
* @{
*/

/** @defgroup USBH_USR_Private_TypesDefinitions
* @{
*/
/**
* @}
*/


/** @defgroup USBH_USR_Private_Defines
* @{
*/

/**
* @}
*/


/** @defgroup USBH_USR_Private_Macros
* @{
*/
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
/**
* @}
*/
#ifdef USE_USB_OTG_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
#ifdef OS_TICKS_PER_SEC     //如果时钟节拍数定义了,说明要使用ucosII了. 
    OSIntEnter();
#endif

	USBH_OTG_ISR_Handler(&USB_OTG_Core);
	
#ifdef OS_TICKS_PER_SEC     //如果时钟节拍数定义了,说明要使用ucosII了. 
    OSIntExit();
#endif
}

/** @defgroup USBH_USR_Private_Variables
* @{
*/
//用户状态机控制
uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;

FATFS fatfs_USB;
FIL file_USB;

uint8_t filenameString[15]  = {0};

#define IMAGE_BUFFER_SIZE    512
uint8_t Image_Buf[IMAGE_BUFFER_SIZE];
uint8_t line_idx = 0;

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_cb = {
	USBH_USR_Init,
	USBH_USR_DeInit,
	USBH_USR_DeviceAttached,
	USBH_USR_ResetDevice,
	USBH_USR_DeviceDisconnected,
	USBH_USR_OverCurrentDetected,
	USBH_USR_DeviceSpeedDetected,
	USBH_USR_Device_DescAvailable,
	USBH_USR_DeviceAddressAssigned,
	USBH_USR_Configuration_DescAvailable,
	USBH_USR_Manufacturer_String,
	USBH_USR_Product_String,
	USBH_USR_SerialNum_String,
	USBH_USR_EnumerationDone,
	USBH_USR_UserInput,
	USBH_USR_MSC_Application,
	USBH_USR_DeviceNotSupported,
	USBH_USR_UnrecoveredError

};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/
/*--------------- LCD Messages ---------------*/
const uint8_t MSG_HOST_INIT[]        = "> Host Library Initialized\n";
const uint8_t MSG_DEV_ATTACHED[]     = "> Device Attached \n";
const uint8_t MSG_DEV_DISCONNECTED[] = "> Device Disconnected\n";
const uint8_t MSG_DEV_ENUMERATED[]   = "> Enumeration completed \n";
const uint8_t MSG_DEV_HIGHSPEED[]    = "> High speed device detected\n";
const uint8_t MSG_DEV_FULLSPEED[]    = "> Full speed device detected\n";
const uint8_t MSG_DEV_LOWSPEED[]     = "> Low speed device detected\n";
const uint8_t MSG_DEV_ERROR[]        = "> Device fault \n";

const uint8_t MSG_MSC_CLASS[]        = "> Mass storage device connected\n";
const uint8_t MSG_HID_CLASS[]        = "> HID device connected\n";
const uint8_t MSG_DISK_SIZE[]        = "> Size of the disk in MBytes: \n";
const uint8_t MSG_LUN[]              = "> LUN Available in the device:\n";
const uint8_t MSG_ROOT_CONT[]        = "> Exploring disk flash ...\n";
const uint8_t MSG_WR_PROTECT[]       = "> The disk is write protected\n";
const uint8_t MSG_UNREC_ERROR[]      = "> UNRECOVERED ERROR STATE\n";

/**
* @}
*/


/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/
static uint8_t Explore_Disk (char* path , uint8_t recu_level);
//static uint8_t Image_Browser (char* path);
/**
* @}
*/


/** @defgroup USBH_USR_Private_Functions
* @{
*/


/**
* @brief  USBH_USR_Init
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
//USB HOST 初始化
void USBH_USR_Init(void) {

}

/**
* @brief  USBH_USR_DeviceAttached
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
//检测到U盘插入
void USBH_USR_DeviceAttached(void) {
//  LCD_UsrLog((void *)MSG_DEV_ATTACHED);
//  printf("检测到USB设备插入!\r\n");
}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
//无法恢复的错误!!
void USBH_USR_UnrecoveredError (void) {
	/* Set default screen color*/
	// LCD_ErrLog((void *)MSG_UNREC_ERROR);
//	printf("无法恢复的错误!!!\r\n\r\n");
}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Status
*/
//检测到U盘拔出
void USBH_USR_DeviceDisconnected (void) {
	USBH_USR_ApplicationState = USH_USR_FS_INIT;
	/* Set default screen color*/
	// LCD_ErrLog((void *)MSG_DEV_DISCONNECTED);
//	printf("USB设备拔出!\r\n");
}
/**
* @brief  USBH_USR_ResetUSBDevice
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void) {
	/* callback for USB-Reset */
//	printf("复位设备...\r\n");
}


/**
* @brief  USBH_USR_DeviceSpeedDetected
*         Displays the message on LCD for device speed
* @param  Device speed
* @retval None
*/
//检测到从机速度
//DeviceSpeed:从机速度(0,1,2 / 其他)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed) {
	if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED) {
		// LCD_UsrLog((void *)MSG_DEV_HIGHSPEED);
//		printf("高速(HS)USB设备!\r\n");
	} else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED) {
		// LCD_UsrLog((void *)MSG_DEV_FULLSPEED);
//		printf("全速(FS)USB设备!\r\n");
	} else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED) {
		// LCD_UsrLog((void *)MSG_DEV_LOWSPEED);
//		printf("低速(LS)USB设备!\r\n");
	} else {
		// LCD_UsrLog((void *)MSG_DEV_ERROR);
//		printf("设备错误!\r\n");
	}
}

/**
* @brief  USBH_USR_Device_DescAvailable
*         Displays the message on LCD for device descriptor
* @param  device descriptor
* @retval None
*/
//检测到从机的描述符
//DeviceDesc:设备描述符指针
void USBH_USR_Device_DescAvailable(void *DeviceDesc) {
//	USBH_DevDesc_TypeDef *hs;
//	hs = DeviceDesc;

//	printf("VID: %04Xh\r\n" , (uint32_t)(*hs).idVendor);
//	printf("PID: %04Xh\r\n" , (uint32_t)(*hs).idProduct);
}

/**
* @brief  USBH_USR_DeviceAddressAssigned
*         USB device is successfully assigned the Address
* @param  None
* @retval None
*/
//从机地址分配成功
void USBH_USR_DeviceAddressAssigned(void) {
//	printf("从机地址分配成功!\r\n");
}


/**
* @brief  USBH_USR_Conf_Desc
*         Displays the message on LCD for configuration descriptor
* @param  Configuration descriptor
* @retval None
*/
//配置描述符获有效
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
    USBH_InterfaceDesc_TypeDef *itfDesc,
    USBH_EpDesc_TypeDef *epDesc) {
	USBH_InterfaceDesc_TypeDef *id;

	id = itfDesc;

	if((*id).bInterfaceClass  == 0x08) {
//		printf("可移动存储器设备!\r\n");
	} else if((*id).bInterfaceClass  == 0x03) {
//		printf("HID 设备!\r\n");
	}
}

/**
* @brief  USBH_USR_Manufacturer_String
*         Displays the message on LCD for Manufacturer String
* @param  Manufacturer String
* @retval None
*/
//获取到设备Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString) {
//	printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
}

/**
* @brief  USBH_USR_Product_String
*         Displays the message on LCD for Product String
* @param  Product String
* @retval None
*/
//获取到设备Product String
void USBH_USR_Product_String(void *ProductString) {
//	printf("Product: %s\r\n",(char *)ProductString);
}

/**
* @brief  USBH_USR_SerialNum_String
*         Displays the message on LCD for SerialNum_String
* @param  SerialNum_String
* @retval None
*/
//获取到设备SerialNum String
void USBH_USR_SerialNum_String(void *SerialNumString) {
//  LCD_UsrLog( "Serial Number : %s\n", (char *)SerialNumString);
//	printf("Serial Number: %s\r\n",(char *)SerialNumString);
}



/**
* @brief  EnumerationDone
*         User response request is displayed to ask application jump to class
* @param  None
* @retval None
*/
//设备USB枚举完成
void USBH_USR_EnumerationDone(void) {

	/* Enumeration complete */
	// LCD_UsrLog((void *)MSG_DEV_ENUMERATED);

	// LCD_SetTextColor(Green);
	// LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "To see the root content of the disk : " );
	// LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "Press Key...                       ");
	// LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);
//	printf("设备枚举完成!\r\n\r\n");
}


/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
//无法识别的USB设备
void USBH_USR_DeviceNotSupported(void) {
//LCD_ErrLog ("No registered class for this device. \n\r");
//	printf("无法识别的USB设备!\r\n\r\n");
}


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
//等待用户输入按键,执行下一步操作
USBH_USR_Status USBH_USR_UserInput(void) {
	// USBH_USR_Status usbh_usr_status;

	// usbh_usr_status = USBH_USR_NO_RESP;

	// /*Key B3 is in polling mode to detect user action */
	// if(STM_EVAL_PBGetState(Button_KEY) == RESET)
	// {

	//   usbh_usr_status = USBH_USR_RESP_OK;

	// }
	// return usbh_usr_status;

//	printf("跳过用户确认步骤!\r\n");
	return USBH_USR_RESP_OK;
}

/**
* @brief  USBH_USR_OverCurrentDetected
*         Over Current Detected on VBUS
* @param  None
* @retval Status
*/
//USB接口电流过载
void USBH_USR_OverCurrentDetected (void) {
//  LCD_ErrLog ("Overcurrent detected.");
//	printf("端口电流过大!!!\r\n");
}


/**
* @brief  USBH_USR_MSC_Application
*         Demo application for mass storage
* @param  None
* @retval Status
*/
//USB HOST MSC类用户应用程序
static USB_OP usbh_op;
void Set_UsbOP_Code(unsigned char op)
{
	usbh_op.operate = op;
}

// TODO:计算时间
int Get_UsbOP_Res(void)
{
	int ret = 0,time = 0;
    while (1) {
        if (usbh_op.operate == USB_IDLE) {
            ret = usbh_op.ret;
            break;
        }

        if (time++ > 5000) {
            ret = -2;
            break;
        }

        OSTimeDly(100);
    }
	
	return ret;
}

int USBH_USR_MSC_Application(void) {
//	int res;

	switch(USBH_USR_ApplicationState) {
		case USH_USR_FS_INIT:

			/* Initialises the File System*/
			if ( f_mount(2,&fatfs_USB) != FR_OK ) {
				/* efs initialisation fails*/
//				printf("> Cannot initialize File System.\n");
				return(-1);
			}
			USBH_USR_ApplicationState = USH_USR_FS_TEST;
		break;
		case USH_USR_FS_TEST:	//执行USB OTG 测试主程序
			switch(usbh_op.operate)
			{
				case USB_SAVE_DATA:
					usbh_op.ret = Data_USB_OUT();
//TODO:					usbh_op.ret = His_USB_OUT();
					usbh_op.operate = USB_IDLE;
					break;
				case USB_SAVE_CONFIG:
					usbh_op.ret = SetInf_USB_OUT();
					usbh_op.operate = USB_IDLE;
					break;
				case USB_READ_CONFIG:
					usbh_op.ret = SetInf_USB_IN();
					usbh_op.operate = USB_IDLE;
					break;
				case USB_IDLE:
				default:
					break;
			}

			break;
			
		default:
			break;
	}
	return(0);
}

/**
* @brief  Explore_Disk
*         Displays disk content
* @param  path: pointer to root path
* @retval None
*/
static uint8_t Explore_Disk (char* path , uint8_t recu_level) {

	FRESULT res;
	FILINFO fno;
	DIR dir;
	char *fn;
	char tmp[14];

	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		while(HCD_IsDeviceConnected(&USB_OTG_Core)) {
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) {
				break;
			}
			if (fno.fname[0] == '.') {
				continue;
			}

			fn = fno.fname;
			strcpy(tmp, fn);

			line_idx++;
			if(line_idx > 9) {
				line_idx = 0;
				// LCD_SetTextColor(Green);
				// LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                              ");
				// LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "Press Key to continue...");
				// LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR);

				// /*Key B3 in polling*/
				// while((HCD_IsDeviceConnected(&USB_OTG_Core)) && \
				//   (STM_EVAL_PBGetState (BUTTON_KEY) == SET))
				// {
				//   Toggle_Leds();

				// }
			}

			if(recu_level == 1) {
//        LCD_DbgLog("   |__");
			} else if(recu_level == 2) {
//        LCD_DbgLog("   |   |__");
			}
			if((fno.fattrib & AM_MASK) == AM_DIR) {
				strcat(tmp, "\n");
//				printf((void *)tmp);
			} else {
				strcat(tmp, "\n");
//				printf((void *)tmp);
			}

			if(((fno.fattrib & AM_MASK) == AM_DIR)&&(recu_level == 1)) {
				Explore_Disk(fn, 2);
			}
		}
	}
	return res;
}

//static uint8_t Image_Browser (char* path) {
////  USBH_USR_ApplicationState = USH_USR_FS_READLIST;
//return 0;
//}

/**
* @brief  USBH_USR_DeInit
*         Deinit User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void) {
	USBH_USR_ApplicationState = USH_USR_FS_INIT;
}



////////////////////////////////////////////////////////////////////////////////////////
//用户定义函数,实现fatfs diskio的接口函数

//获取U盘状态
//返回值:0,U盘未就绪
//      1,就绪
u8 USB_disk_status(void) {
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&USBH_USR_ApplicationState == USH_USR_FS_TEST){
		return 1;
	}else{
		return 0;
	}
}

//读U盘
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数
//返回值:错误状态;0,正常;其他,错误代码;
u8 USB_disk_read(u8* buf,u32 sector,u32 cnt) {
	u8 res=1;
	u32 retry = 0;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&USBH_USR_ApplicationState == USH_USR_FS_TEST) { //连接还存在,且是APP测试状态
		do {
			res=USBH_MSC_Read10(&USB_OTG_Core,buf,sector,512*cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
			if(!HCD_IsDeviceConnected(&USB_OTG_Core)) {
				res=1;//读写错误
				break;
			};
			retry++;
		} while(res==USBH_MSC_BUSY&&retry < 0x7FFFFFF);//防止程序死掉
		if(retry == 0x7FFFFFF) res = 1;
	} else res=1;
	if(res==USBH_MSC_OK)res=0;
	return res;
}

//写U盘
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数
//返回值:错误状态;0,正常;其他,错误代码;
u8 USB_disk_write(u8* buf,u32 sector,u32 cnt) {
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&USBH_USR_ApplicationState == USH_USR_FS_TEST) { //连接还存在,且是APP测试状态
		do {
			res=USBH_MSC_Write10(&USB_OTG_Core,buf,sector,512*cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
			if(!HCD_IsDeviceConnected(&USB_OTG_Core)) {
				res=1;//读写错误
				break;
			};
		} while(res==USBH_MSC_BUSY);
	} else res=1;
	if(res==USBH_MSC_OK)res=0;
	return res;
}

u8 USB_disk_ioctl( BYTE cmd, void* buff) {
	u8 res=1;
	switch(cmd) {
		case CTRL_SYNC:
			res = RES_OK;
			break;
		case GET_SECTOR_SIZE:
			*(WORD*)buff=512;
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE:
			*(WORD*)buff=512;
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT:
			*(DWORD*)buff=USBH_MSC_Param.MSCapacity;
			res = RES_OK;
			break;
		default:
			res = RES_PARERR;
			break;
	}
	return res;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

