#include "energy.h"
#include "modbus.h"
#include "dlt645.h"
#include "cjt188.h"
#include "usart.h"
#include "userconfig.h"
#include "parameters.h"
#include <string.h>
#include <math.h>
#include "recode.h"
#include "bsp.h"
#include "menuinc.h"

#define COLLECT_NUM		(12)
#define CL_ENERGY		(0)
#define CL_VA			(1)
#define CL_VB			(2)
#define CL_VC			(3)
#define CL_IA			(4)
#define CL_IB			(5)
#define CL_IC			(6)
#define CL_PM2_5		(7)
#define CL_PM10			(8)
#define CL_LIGHT		(9)
#define CL_WATER		(10)
#define CL_FOOD			(11)

int UARTX_SENSOR = 0;
static BYTE UART3RxBuf[BUF_SIZE_256];
static BYTE UART3TxBuf[BUF_SIZE_256];

int collect_result[COLLECT_NUM];
BYTE collect_er[COLLECT_NUM];

typedef enum {
    MSG_ACTION_TX = 0,
    MSG_ACTION_RX
} XMSG_ACTION;

typedef enum {
    MEM_1234 = 0,	// small-endian
    MEM_4321,	// big-endian   slave的float inverse 模式
    MEM_2143	// 两两打断模式  slave的float模式
} XTN_MEM;
typedef enum {
    EN_HEX = 0,	// hexicode
    EN_BCD,		// bcd
    EN_FLOAT,	// float
    EN_ASC
} XTN_ENCODER;
typedef union {
    long	lX;
    float	fX;
    double	dX;
    BYTE	x[sizeof(long)];
} TSHARE_X, *PSHARE_X;

static unsigned long EN_REG[PL_NUM][MB_NUM] = {{0x9010, 0xB611, 0xB612, 0xB613, 0xB621, 0xB622, 0xB623},
    {0x0010000, 0x02010100, 0x02010200, 0x02010300, 0x02020100, 0x02020200, 0x02020300},
    {0x1d, 0, 1, 2, 3, 4, 5},
};

#define CJT188_ADDR 	(0x901F)
#define CJT188_LEN	 	(4)
#define CJT188_POINT 	(2)
#define CJT188_UNIT_ADDR		(4)

TSET_PARM SET_PAPM[COLLECT_NUM] = {
    {0, TYPE_MODBUS, "1", 0, 2},
    {0, TYPE_MODBUS, "1", 0, 1},
    {0, TYPE_MODBUS, "1", 0, 1},
    {0, TYPE_MODBUS, "1", 0, 1},
    {0, TYPE_MODBUS, "1", 0, 1},
    {0, TYPE_MODBUS, "1", 0, 1},
    {0, TYPE_MODBUS, "1", 0, 1},

    {0, TYPE_MODBUS, "1", 4, 1,},
    {0, TYPE_MODBUS, "1", 9, 1,},
    {0, TYPE_MODBUS, "1", 7, 2,},

    {0, TYPE_MODBUS, "1", 1, 2,},
    {0, TYPE_MODBUS, "1", 1, 2,},
};

typedef struct {
    BYTE	len;			// 数据长度[0..255]
    DWORD	mem;		// 存储格式[0..31]::0=BYTE_1234,1=BYTE_4321,2= BYTE_2143
    DWORD	encode;		// 编码格式[0..7]::0=hex,1=bcd,2=float,3=ascii,4..7=保留
    char	ten_exp;		// 小数位置
    WORD	wRatio;			// 倍率[0..65535]
} TAP_PARM, *PAP_PARM;
TAP_PARM EN_PARM[PL_NUM][MB_NUM] = {
    {
        4, MEM_1234, EN_BCD, 2, ENERG_RATE,
        2, MEM_1234, EN_BCD, 0, ENERG_PARA_RATE,
        2, MEM_1234, EN_BCD, 0, ENERG_PARA_RATE,
        2, MEM_1234, EN_BCD, 0, ENERG_PARA_RATE,
        2, MEM_1234, EN_BCD, 2, ENERG_PARA_RATE,
        2, MEM_1234, EN_BCD, 2, ENERG_PARA_RATE,
        2, MEM_1234, EN_BCD, 2, ENERG_PARA_RATE
    },
    {
        4, MEM_1234, EN_BCD, 2, ENERG_RATE,
        2, MEM_1234, EN_BCD, 1, ENERG_PARA_RATE,
        2, MEM_1234, EN_BCD, 1, ENERG_PARA_RATE,
        2, MEM_1234, EN_BCD, 1, ENERG_PARA_RATE,
        3, MEM_1234, EN_BCD, 3, ENERG_PARA_RATE,
        3, MEM_1234, EN_BCD, 3, ENERG_PARA_RATE,
        3, MEM_1234, EN_BCD, 3, ENERG_PARA_RATE
    },
    {
        4, MEM_4321, EN_HEX, 2, ENERG_RATE,
        2, MEM_4321, EN_HEX, 1, ENERG_PARA_RATE,
        2, MEM_4321, EN_HEX, 1, ENERG_PARA_RATE,
        2, MEM_4321, EN_HEX, 1, ENERG_PARA_RATE,
        2, MEM_4321, EN_HEX, 3, ENERG_PARA_RATE,
        2, MEM_4321, EN_HEX, 3, ENERG_PARA_RATE,
        2, MEM_4321, EN_HEX, 3, ENERG_PARA_RATE
    },

};
TAP_PARM AP_PARM[COLLECT_NUM] = {
    {4, MEM_1234, EN_BCD, 2, ENERG_RATE,},
    {2, MEM_1234, EN_BCD, 0, ENERG_PARA_RATE,},
    {2, MEM_1234, EN_BCD, 0, ENERG_PARA_RATE,},
    {2, MEM_1234, EN_BCD, 0, ENERG_PARA_RATE,},
    {2, MEM_1234, EN_BCD, 2, ENERG_PARA_RATE,},
    {2, MEM_1234, EN_BCD, 2, ENERG_PARA_RATE,},
    {2, MEM_1234, EN_BCD, 2, ENERG_PARA_RATE,},
    {2, MEM_4321, EN_HEX, 0, PM_RATE,},
    {2, MEM_4321, EN_HEX, 0, PM_RATE,},
    {4, MEM_4321, EN_HEX, 0, LIGHT_RATE,},

    {2, MEM_4321, EN_HEX, 0, WATER_RATE,},
    {4, MEM_4321, EN_HEX, 0, FOOD_RATE,},
};

// 暂停采集，开始校准
void collect_suspend(void)
{
    while (sys_sta.food_adjut_state == STA_ON) {
        task_time[Sensors_id] = 0;
        OSTimeDlyHMSM(0, 0, 3, 0);
    }
}

int msg_action(const PSET_PARM pSetParm, XMSG_ACTION action)
{
    int			msgLn, bytes;
    u8 			func = 0;

    if ( pSetParm == NULL || pSetParm->enable != STA_ON) {
        return -1;
    }

    switch (pSetParm->type) {
    case TYPE_MODBUS:
        if ( action == MSG_ACTION_TX ) {
            ClearUartRx(UARTX_SENSOR);
            bytes = MODBUS_PackMsg( pSetParm->addr[0], COM_RD_RHOLD,
                                    (BYTE*)&pSetParm->reg, pSetParm->reg_num, UART3TxBuf, sizeof(UART3TxBuf) );
            SendUartData( UARTX_SENSOR, UART3TxBuf, bytes );
        } else {
            msgLn = GetUartData( UARTX_SENSOR, UART3RxBuf, sizeof(UART3RxBuf) );
            bytes = MODBUS_GetDatSeg( pSetParm->addr[0], COM_RD_RHOLD, UART3RxBuf, msgLn);
        }

        break;

    case TYPE_DL645_97:
    case TYPE_DL645_07:
        func = (pSetParm->type == TYPE_DL645_97) ? 0x01 : 0x11;

        if ( action == MSG_ACTION_TX ) {
            ClearUartRx(UARTX_SENSOR);
            bytes = DLT645_PackMsg( 2, pSetParm->type, pSetParm->addr, func,
                                    (BYTE *)&pSetParm->reg, UART3TxBuf, sizeof(UART3TxBuf) );
            SendUartData( UARTX_SENSOR, UART3TxBuf, bytes );
        } else {
            msgLn = GetUartData( UARTX_SENSOR, UART3RxBuf, sizeof(UART3RxBuf) );
            bytes = DLT645_GetDatSeg( pSetParm->type, pSetParm->addr, func,
                                      (BYTE *)&pSetParm->reg, UART3RxBuf, msgLn );
        }

        break;

    case TYPE_CJT_188:
        if ( action == MSG_ACTION_TX ) {
            ClearUartRx( UARTX_SENSOR );
            bytes = CJT188_PackMsg( 2, TYPE_WATER_COLD, pSetParm->addr, CJT188_FUNC_RD,
                                    (BYTE*)&pSetParm->reg, UART3TxBuf, sizeof(UART3TxBuf) );
            SendUartData( UARTX_SENSOR, UART3TxBuf, bytes );
        } else {
            msgLn = GetUartData(UARTX_SENSOR, UART3RxBuf, sizeof(UART3RxBuf) );
            bytes = CJT188_GetDatSeg( TYPE_WATER_COLD, pSetParm->addr, CJT188_FUNC_RD,
                                      (BYTE*)&pSetParm->reg, UART3RxBuf, msgLn );

            if (UART3RxBuf[CJT188_UNIT_ADDR] >= 0x29 && UART3RxBuf[CJT188_UNIT_ADDR] <= 0x2C) {
                AP_PARM[CL_WATER].wRatio = pow(10, (UART3RxBuf[CJT188_UNIT_ADDR] - 0x29));
            }
        }

        break;

    default:
        return -2;
    }

    if ( bytes < 1 ) {
        return -3;
    }

    return bytes;
}


/*
* brief : reverse buffer data
* param : buf[in]:data buffer
*         size[in]:buffer size
* result: -1:param error
*          0:ok
*/
int ReverseBuffer( void* buf, int size )
{
    int		i, n;
    char	temp;
    char*	p;

    if ( buf == NULL || size < 1 ) {
        return -1;
    }

    n = size / 2;
    p = (char*)buf;

    for ( i = 0; i < n; i++ ) {
        temp = p[i];
        p[i] = p[size - 1 - i];
        p[size - 1 - i] = temp;
    }

    return 0;
}

/*
* brief : reverse buffer pair data
* param : buf[in]:data buffer
*         size[in]:buffer size
* result: -1:param error
*          0:ok
*/
int ReverseBufferPair( void* buf, int size )
{
    int		i, n;
    char	temp;
    char*	p;

    if ( buf == NULL || size < 1 ) {
        return -1;
    }

    n = size / 2;
    p = (char*)buf;

    for ( i = 0; i < n; i++ ) {
        temp = p[i * 2];
        p[i * 2] = p[i * 2 + 1];
        p[i * 2 + 1] = temp;
    }

    return 0;
}



int DeTnData( const PAP_PARM pApParm, int *result, const BYTE* buf, int datLn )
{
    int		i, x;
    BYTE	bSignFlag = 0, error = 0;
    BYTE*	pData;
    BYTE	memd[64];
    TSHARE_X	shareX;
    TAP_PARM	apParm;

    if (datLn < 0 || datLn > sizeof(memd) ) {
        return -1;
    }

    memcpy( &apParm, pApParm, sizeof(apParm) );
    memcpy( memd, buf, datLn );

    // 数据检查
    if ( apParm.len < 1 || datLn < (apParm.len) ) {
        return -2;
    }

    // 存储格式
    pData = &memd[0];

    switch ( apParm.mem ) {
    case MEM_4321:
        ReverseBuffer( pData, apParm.len );
        break;

    case MEM_2143:
        ReverseBufferPair( pData, apParm.len );
        break;

    default: // MEM_1234
        break;
    }

    // 编码格式
    memset( &shareX, 0, sizeof(shareX) );

    switch ( apParm.encode ) {
    case EN_HEX:
        x = (apParm.len > sizeof(shareX.x)) ? sizeof(shareX.x) : apParm.len;
        memcpy( shareX.x, pData, x );

        shareX.lX *= (pow(10, -apParm.ten_exp) * apParm.wRatio);
        *result = shareX.lX;
        break;

    case EN_BCD://
        // 符号位
        bSignFlag = 0;

        if ( (pData[apParm.len - 1] & 0x80) ) {
            pData[apParm.len - 1] &= 0x7f;
            bSignFlag = 1;
        }

        for ( i = apParm.len - 1; i >= 0;  i-- ) {
            shareX.lX *= 100;
            x = Bcd2Hex( pData[i] );

            if ( x < 0 )	break;

            shareX.lX += x;
        }

        if ( i >= 0 ) {
            shareX.lX = 0;
            error = 1;
        } else {
            if ( bSignFlag > 0 ) {
                shareX.lX = -shareX.lX;
            }

            shareX.lX *= (pow(10, -apParm.ten_exp) * apParm.wRatio);
        }

        *result = shareX.lX;
        break;

    case EN_FLOAT:
        x = (apParm.len > sizeof(shareX.x)) ? sizeof(shareX.x) : apParm.len;
        memcpy( shareX.x, pData, x );

        if ( apParm.len > 4 ) {
            shareX.dX *= (pow(10, -apParm.ten_exp) * apParm.wRatio);
            *result = shareX.dX;
        } else {
            shareX.fX *= (pow(10, -apParm.ten_exp) * apParm.wRatio);
            *result = shareX.fX;
        }

        break;

    default:
        error = 1;
        break;
    }

    if (error > 0 ) {
        return -3;
    }

    return 0;
}
static int water_collect_time = 0;
extern u8 water_realtime_page;
void Sensors_Collect(void)
{
    int res = 0, i = 0;
    unsigned char os_err = 0;

    for (i = 0; i < COLLECT_NUM; i++) {
        collect_suspend();

        if (i == CL_WATER ) {
            if (water_realtime_page != STA_ON) { //水表且不在实时数据界面
                if (water_collect_time++ < WATER_INTERVAL) {
                    OSTimeDly(50);
                    continue;
                } else {
                    water_collect_time = 0;
                }
            } else {
                water_collect_time = 0;
            }
        }


        if (i == 0 && SET_PAPM[i].enable == STA_ON) { //默认波特率9600N81
            UartInit( UARTX_SENSOR, ext_inf.din.mb_data[MB_DATA_ENERGY].baud, ext_inf.din.mb_data[MB_DATA_ENERGY].ux.dbits,
                      ext_inf.din.mb_data[MB_DATA_ENERGY].ux.parity, ext_inf.din.mb_data[MB_DATA_ENERGY].ux.stopb);
            OSTimeDly(50);
        }

        if (SET_PAPM[i].enable != STA_ON) {
            continue;
        }

        if (i >= CL_PM2_5 && i <= CL_LIGHT) {
            UartInit(UARTX_SENSOR, 0x03, UDATA_8B, PARITY_NONE, STOP_BIT1); //9600
            OSTimeDly(50);
        } else if (i == CL_WATER) {
            UartInit( UARTX_SENSOR, ext_inf.din.mb_data[MB_DATA_WATER].baud, ext_inf.din.mb_data[MB_DATA_WATER].ux.dbits,
                      ext_inf.din.mb_data[MB_DATA_WATER].ux.parity, ext_inf.din.mb_data[MB_DATA_WATER].ux.stopb);
            OSTimeDly(50);
        } else if (i == CL_FOOD) {
            UartInit( UARTX_SENSOR, ext_inf.din.mb_data[MB_DATA_FOOD].baud, ext_inf.din.mb_data[MB_DATA_FOOD].ux.dbits,
                      ext_inf.din.mb_data[MB_DATA_FOOD].ux.parity, ext_inf.din.mb_data[MB_DATA_FOOD].ux.stopb);
            OSTimeDly(50);
        }

        OSTimeDly(50);
		ClearUartIdle(UARTX_SENSOR);
        msg_action( &SET_PAPM[i], MSG_ACTION_TX );
        os_err = WaitUartIdle( UARTX_SENSOR, UART_TIME_OUT);

        if (os_err == OS_ERR_NONE) { //没有超时
            res = msg_action( &SET_PAPM[i], MSG_ACTION_RX );

            if (res < 1) {
                collect_result[i] = 0;
                collect_er[i] ++;
                continue;
            }

            res = DeTnData( &AP_PARM[i], &collect_result[i], UART3RxBuf, res);

            if (res < 0) {
                collect_result[i] = 0;
                collect_er[i] ++;
                continue;
            } else {
                collect_er[i] = 0;
            }

        } else {
            collect_result[i] = 0;
            collect_er[i] ++;
        }
    }

}

typedef struct {
    u8 energy; //三个电表采集，一次循环采集一个
    u8 energy_er[PRO_DATA_MAX];
    u8 water; //6个料表采集，一次循环采集一个
    u8 water_er[PRO_DATA_MAX];
    u8 food; //6个料表采集，一次循环采集一个
    u8 food_er[PRO_DATA_MAX];
} TID;

static TID id;

static void DL645_Addr_SW(void)
{
    unsigned char i = 0, len = 0;
    unsigned char temp_buf[20];
    unsigned char addr_buf[12];
    unsigned char addr_buf2[6];

    memset(addr_buf, 0x30, sizeof(addr_buf));

    if (id.energy == 0) {
        memcpy(temp_buf, ext_inf.din.pro_data[0].energy_addr, sizeof(ext_inf.din.pro_data[0].energy_addr));
        temp_buf[sizeof(ext_inf.din.pro_data[0].energy_addr)] = 0;
    } else if (id.energy == 1) {
        memcpy(temp_buf, ext_inf.din.pro_data[1].energy_addr, sizeof(ext_inf.din.pro_data[1].energy_addr));
        temp_buf[sizeof(ext_inf.din.pro_data[1].energy_addr)] = 0;
    } else {
        memcpy(temp_buf, dev_inf.fan_chk.energy_addr, sizeof(dev_inf.fan_chk.energy_addr));
        temp_buf[sizeof(dev_inf.fan_chk.energy_addr)] = 0;
    }

    len = strlen((const char *)temp_buf);

    if (len > 12)len = 12;

    memcpy(&addr_buf[sizeof(addr_buf) - len], temp_buf, len);

    for (i = 0; i < 6; i++) { //ascii转bcd
        if (addr_buf[2 * i] < 0x30 || addr_buf[2 * i] > 0x39)
            break;

        if (addr_buf[2 * i + 1] < 0x30 || addr_buf[2 * i] > 0x39)
            break;

        addr_buf2[i] = (addr_buf[2 * i] - 0x30) * 0x10 + (addr_buf[2 * i + 1] - 0x30);
    }

    //反序


    for (i = 0; i < 6; i++) {
        SET_PAPM[MB_ENERGY].addr[i] = addr_buf2[5 - i];
    }

    for (i = MB_VA; i <= MB_IC; i++) {
        memcpy(SET_PAPM[i].addr, SET_PAPM[MB_ENERGY].addr, sizeof(SET_PAPM[MB_ENERGY].addr));
    }
}

static void CJT188_Addr_SW(void)
{
    unsigned char i = 0, len = 0;
    unsigned char temp_buf[20];
    unsigned char addr_buf[14];
    unsigned char addr_buf2[7];

    memset(addr_buf, 0x30, sizeof(addr_buf));
    memcpy(temp_buf, ext_inf.din.pro_data[id.water].water_addr, sizeof(ext_inf.din.pro_data[id.water].water_addr));
    temp_buf[sizeof(ext_inf.din.pro_data[id.water].water_addr)] = 0;
    len = strlen((const char *)temp_buf);

    if (len > 14)len = 14;

    memcpy(&addr_buf[sizeof(addr_buf) - len], temp_buf, len);

    for (i = 0; i < 7; i++) { //ascii转bcd
        if (addr_buf[2 * i] < 0x30 || addr_buf[2 * i] > 0x39)
            break;

        if (addr_buf[2 * i + 1] < 0x30 || addr_buf[2 * i] > 0x39)
            break;

        addr_buf2[i] = (addr_buf[2 * i] - 0x30) * 0x10 + (addr_buf[2 * i + 1] - 0x30);
    }

    //反序
    for (i = 0; i < 7; i++) {
        SET_PAPM[CL_WATER].addr[i] = addr_buf2[6 - i];
    }
}

int check_energy_addr(unsigned char * energy_addr)
{
    if (energy_addr[0] == 0 ||
            strcmp((const char *)energy_addr, "0") == 0) {
        return -1;
    }

    return 1;
}
u8 energy_enable[ENERGY_MAX];
void Energy_Init(void)
{
    u8 i = 0, type = 0, ct = 0, num = 0;

    if (ext_inf.din.pro_data[0].energy_sta == PL_DIS) {
        for (i = 0; i < MB_NUM; i++) {
            SET_PAPM[i].enable = STA_OFF;
        }

        memset(collect_er, 0, MB_NUM);
        memset(id.energy_er, 0, ENERGY_MAX);
        memset(energy_enable, 0, sizeof(energy_enable));
        sys_sta.energy_num = 0;
        return;
    }

    //检测电表地址有效性
    num = 0;
    memset(energy_enable, 0, sizeof(energy_enable));

    if (check_energy_addr(ext_inf.din.pro_data[0].energy_addr) > 0 ) {
        num++;
        energy_enable[0] = 1;
    } else {
        id.energy_er[0] = 0;
    }

    if (check_energy_addr(ext_inf.din.pro_data[1].energy_addr) > 0) {
        num++;
        energy_enable[1] = 1;
    } else {
        id.energy_er[1] = 0;
    }

    if (check_energy_addr(dev_inf.fan_chk.energy_addr) > 0
            && dev_inf.fan_chk.enable == STA_ON) {
        num++;
        energy_enable[2] = 1;
    } else {
        id.energy_er[2] = 0;
    }

    sys_sta.energy_num = num;//电表个数

    for (i = 0; i < ENERGY_MAX; i++) {
        id.energy = (id.energy + 1) % ENERGY_MAX;//采集序号

        if (energy_enable[id.energy] == 1) {
            break;
        }
    }

    //修改采集参数
    if (ext_inf.din.pro_data[0].energy_sta == PL_DL645_97 || ext_inf.din.pro_data[0].energy_sta == PL_DL645_07) {
        type = ext_inf.din.pro_data[0].energy_sta;
    } else {
        type = TYPE_MODBUS;
    }

    for (i = 0; i < MB_NUM; i++) {
        SET_PAPM[i].enable = STA_ON;
        SET_PAPM[i].type = type;
        SET_PAPM[i].reg = EN_REG[ext_inf.din.pro_data[0].energy_sta - 1][i];
    }

    if (ext_inf.din.pro_data[0].energy_sta == PL_DL645_97 || ext_inf.din.pro_data[0].energy_sta == PL_DL645_07) { //地址转换
        DL645_Addr_SW();
    } else {
        for (i = 0; i < MB_NUM; i++) {
            if (id.energy == 0) {
                SET_PAPM[i].addr[0] = ext_inf.din.pro_data[0].energy_addr[0];
            } else if (id.energy == 1) {
                SET_PAPM[i].addr[0] = ext_inf.din.pro_data[1].energy_addr[0];
            } else {
                SET_PAPM[i].addr[0] = dev_inf.fan_chk.energy_addr[0];
            }
        }
    }

    if (id.energy == ENERGY_FAN) {
        ct = dev_inf.fan_chk.energy_ct;
    } else {
        ct = ext_inf.din.pro_data[0].energy_ct;
    }

    //修改结果参数
    EN_PARM[ext_inf.din.pro_data[0].energy_sta - 1][MB_ENERGY].wRatio = ENERG_RATE * ct;
    EN_PARM[ext_inf.din.pro_data[0].energy_sta - 1][MB_IA].wRatio = ENERG_PARA_RATE * ct;
    EN_PARM[ext_inf.din.pro_data[0].energy_sta - 1][MB_IB].wRatio = ENERG_PARA_RATE * ct;
    EN_PARM[ext_inf.din.pro_data[0].energy_sta - 1][MB_IC].wRatio = ENERG_PARA_RATE * ct;

    for (i = 0; i < MB_NUM; i++) {
        memcpy(&AP_PARM[i], &EN_PARM[ext_inf.din.pro_data[0].energy_sta - 1][i], sizeof(TAP_PARM));
    }
}
void Sensors_Init(void)
{
    int i = 0;

    memset(collect_result, 0, sizeof(collect_result));
    Energy_Init();

    if (dev_inf.env_dev[MB_DEV_PM2_5].enable == STA_OFF) {
        SET_PAPM[CL_PM2_5].enable = STA_OFF;
        collect_er[CL_PM2_5] = 0;
        SET_PAPM[CL_PM10].enable = STA_OFF;
        collect_er[CL_PM10] = 0;
    } else {
        //修改采集参数
        SET_PAPM[CL_PM2_5].enable = STA_ON;
        SET_PAPM[CL_PM2_5].addr[0] = dev_inf.env_dev[MB_DEV_PM2_5].addr;
        SET_PAPM[CL_PM10].enable = STA_ON;
        SET_PAPM[CL_PM10].addr[0] = dev_inf.env_dev[MB_DEV_PM2_5].addr;
        //修改结果参数
    }

    if (dev_inf.env_dev[MB_DEV_LIGHT].enable == STA_OFF) {
        SET_PAPM[CL_LIGHT].enable = STA_OFF;
        collect_er[CL_LIGHT] = 0;
    } else {
        //修改采集参数
        SET_PAPM[CL_LIGHT].enable = STA_ON;
        SET_PAPM[CL_LIGHT].addr[0] = dev_inf.env_dev[MB_DEV_LIGHT].addr;
        //修改结果参数
    }

    if (ext_inf.din.pro_data[0].water_sta != WL_CJT_188 && ext_inf.din.pro_data[0].water_sta != WL_MB_OWN) {
        SET_PAPM[CL_WATER].enable = STA_OFF;
        collect_er[CL_WATER] = 0;
        memset(id.water_er, 0, PRO_DATA_MAX);
    } else if (ext_inf.din.pro_data[0].water_sta == WL_CJT_188) {
        for (i = 0; i < PRO_DATA_MAX; i++) {
            id.water = (id.water + 1) % PRO_DATA_MAX; //采集序号

            if (ext_inf.din.pro_data[id.water].water_sta != 0) {
                break;
            }
        }

        //修改采集参数
        SET_PAPM[CL_WATER].enable = STA_ON;
        SET_PAPM[CL_WATER].type = TYPE_CJT_188;
        CJT188_Addr_SW();
//        SET_PAPM[CL_WATER].addr[0] = ext_inf.din.water_addr[0];
        SET_PAPM[CL_WATER].reg = CJT188_ADDR;
//        SET_PAPM[CL_WATER].reg_num = (ext_inf.din.mb_data[MB_DATA_WATER].dx.len + 1) / 2;
        //修改结果参数
        AP_PARM[CL_WATER].len = CJT188_LEN;
        AP_PARM[CL_WATER].mem = MEM_1234;
        AP_PARM[CL_WATER].encode = EN_BCD;
        AP_PARM[CL_WATER].ten_exp = CJT188_POINT;
    } else {
        for (i = 0; i < PRO_DATA_MAX; i++) {
            id.water = (id.water + 1) % PRO_DATA_MAX; //采集序号

            if (ext_inf.din.pro_data[id.water].water_sta != 0) {
                break;
            }
        }

        //修改采集参数
        SET_PAPM[CL_WATER].enable = STA_ON;
        SET_PAPM[CL_WATER].type = TYPE_MODBUS;
        SET_PAPM[CL_WATER].addr[0] = ext_inf.din.pro_data[id.water].water_addr[0];
        SET_PAPM[CL_WATER].reg = ext_inf.din.mb_data[MB_DATA_WATER].reg_addr;
        SET_PAPM[CL_WATER].reg_num = (ext_inf.din.mb_data[MB_DATA_WATER].dx.len + 1) / 2;

        //修改结果参数
        AP_PARM[CL_WATER].len = ext_inf.din.mb_data[MB_DATA_WATER].dx.len;
        AP_PARM[CL_WATER].mem = ext_inf.din.mb_data[MB_DATA_WATER].dx.mem;
        AP_PARM[CL_WATER].encode = ext_inf.din.mb_data[MB_DATA_WATER].dx.encode;
        AP_PARM[CL_WATER].ten_exp = ext_inf.din.mb_data[MB_DATA_WATER].dx.point;
    }


    if (ext_inf.din.pro_data[0].food_sta != FL_MB_OWN && ext_inf.din.pro_data[0].food_sta != FL_MB_XK3102) {
        SET_PAPM[CL_FOOD].enable = STA_OFF;
        collect_er[CL_FOOD] = 0;
        memset(id.food_er, 0, PRO_DATA_MAX);
    } else {
        for (i = 0; i < PRO_DATA_MAX; i++) {
            id.food = (id.food + 1) % PRO_DATA_MAX;//采集序号

            if (ext_inf.din.pro_data[id.food].food_sta != 0) {
                break;
            }
        }

        //修改采集参数
        SET_PAPM[CL_FOOD].enable = STA_ON;
        SET_PAPM[CL_FOOD].type = TYPE_MODBUS;
        SET_PAPM[CL_FOOD].addr[0] = ext_inf.din.pro_data[id.food].food_addr;
        SET_PAPM[CL_FOOD].reg = ext_inf.din.mb_data[MB_DATA_FOOD].reg_addr;
        SET_PAPM[CL_FOOD].reg_num = (ext_inf.din.mb_data[MB_DATA_FOOD].dx.len + 1) / 2;

        //修改结果参数
        AP_PARM[CL_FOOD].len = ext_inf.din.mb_data[MB_DATA_FOOD].dx.len;
        AP_PARM[CL_FOOD].mem = ext_inf.din.mb_data[MB_DATA_FOOD].dx.mem;
        AP_PARM[CL_FOOD].encode = ext_inf.din.mb_data[MB_DATA_FOOD].dx.encode;
        AP_PARM[CL_FOOD].ten_exp = ext_inf.din.mb_data[MB_DATA_FOOD].dx.point;
    }

}
static u8 phase_cur_num = 0;
void phase_cur_chk(void)
{
    int i = 0, min = 99999, max = 0;

    if (ext_inf.din.pro_data[0].energy_sta == PL_DIS) {
        phase_cur_num = 0;
        ALA_Set_State(ALA_AFAN_BL, ALA_STA_OFF);
        return ;
    }

    if (dev_inf.fan_chk.enable != STA_ON || dev_inf.fan_chk.phase_cur == 0
            || id.energy_er[ENERGY_FAN] >= UART_TIMES) {
        phase_cur_num = 0;
        ALA_Set_State(ALA_AFAN_BL, ALA_STA_OFF);
        return ;
    }

    if (id.energy != ENERGY_FAN) {
        return;
    }

    for (i = MB_IA; i <= MB_IC; i++) {
        if (max < sta_in.ele[ENERGY_FAN][i]) {
            max = sta_in.ele[ENERGY_FAN][i];
        }

        if (min > sta_in.ele[ENERGY_FAN][i]) {
            min = sta_in.ele[ENERGY_FAN][i];
        }
    }

    if (abs(max - min) >= abs(dev_inf.fan_chk.phase_cur)) {
        phase_cur_num ++;
    } else {
        phase_cur_num = 0;
    }

    if (phase_cur_num >= CUR_CHK_TIMES) {
        phase_cur_num = CUR_CHK_TIMES;
        ALA_Set_State(ALA_AFAN_BL, ALA_STA_ON);
    } else {
        ALA_Set_State(ALA_AFAN_BL, ALA_STA_OFF);
    }
}

void cal_fan_cur(void)
{
    int i = 0, cur = 0, error = 0, max = 0;

    for (i = 0; i < JDQ_NUM_MAX; i++) {
        if (dev_inf.JDQ[i].enable != STA_ON) {
            continue ;
        }

        if (sta_out.ALLJDQ[i] == STA_ON) {
            cur += dev_inf.JDQ[i].current * dev_inf.JDQ[i].fan_num;
            error += dev_inf.fan_chk.error_cur * dev_inf.JDQ[i].fan_num;
            max = EMAX(max, dev_inf.JDQ[i].current);
        }
    }

    cal_inf.jdq_fan_cur = cur;
    cal_inf.fan_cur_max = max;
    cal_inf.fan_error_cur = error;

    if (cal_inf.fan_error_cur == 0 && cal_inf.fan_cur_max == 0) {
        cal_inf.fan_error_cur = dev_inf.fan_chk.error_cur;
    }
}

static u8 fan_cur_num = 0;
void fan_cur_chk(void)
{
    int i = 0, avg = 0;

    if (ext_inf.din.pro_data[0].energy_sta == PL_DIS) {
        fan_cur_num = 0;
        ALA_Set_State(ALA_AFAN, ALA_STA_OFF);
        return ;
    }

    if (dev_inf.fan_chk.enable != STA_ON || id.energy_er[ENERGY_FAN] >= UART_TIMES) {
        fan_cur_num = 0;
        ALA_Set_State(ALA_AFAN, ALA_STA_OFF);
        return ;
    }

    if (id.energy != ENERGY_FAN) {
        return;
    }

    cal_fan_cur();

    for (i = MB_IA, avg = 0; i <= MB_IC; i++) {
        avg += sta_in.ele[ENERGY_FAN][i];
    }

    avg = avg / 3;

    if (abs(avg + dev_inf.fan_chk.offset_cur - cal_inf.jdq_fan_cur) > (cal_inf.fan_cur_max * dev_inf.fan_chk.percent / 100 + cal_inf.fan_error_cur)) {
        fan_cur_num ++;
    } else {
        fan_cur_num = 0;
    }

    if (fan_cur_num >= CUR_CHK_TIMES) {
        fan_cur_num = CUR_CHK_TIMES;
        ALA_Set_State(ALA_AFAN, ALA_STA_ON);
    } else {
        ALA_Set_State(ALA_AFAN, ALA_STA_OFF);
    }
}

void Sensors_Result(void)
{
    u8 i = 0;
    signed short food_temp = 0;

    for (i = 0; i < MB_NUM; i++) {

        if (collect_er[i] == 0) { //没有故障才更新数据
            sta_in.ele[id.energy][i] = collect_result[i];
        }

//		电表报警单独计算
        if (i != MB_ENERGY)continue;

        if (collect_er[i] != 0) {
            collect_er[i] = 1;
            id.energy_er[id.energy]++;
        } else {
            id.energy_er[id.energy] = 0;
        }

        if (id.energy_er[id.energy] >= UART_TIMES) {
            id.energy_er[id.energy] = UART_TIMES;
            memset(sta_in.ele[id.energy], 0, sizeof(sta_in.ele[id.energy]));
            sta_in.ele[id.energy][MB_ENERGY] = DATA_ER_VALUE;
        }
    }

    for (i = 0; i < ENERGY_MAX; i++) {
        if (id.energy_er[i] >= UART_TIMES) {
            ALA_Set_State(ALA_EENERG, ALA_STA_ON);
            break;
        }
    }

    if (i >= ENERGY_MAX) {
        ALA_Set_State(ALA_EENERG, ALA_STA_OFF);
    }

    phase_cur_chk();
    fan_cur_chk();

    if (collect_er[CL_PM2_5] >= UART_TIMES) {
        collect_er[CL_PM2_5] = UART_TIMES;
        ALA_Set_State(ALA_EPM2_5, ALA_STA_ON);
    } else {
        ALA_Set_State(ALA_EPM2_5, ALA_STA_OFF);
    }

    if (dev_inf.env_dev[MB_DEV_PM2_5].enable == STA_ON && ALA_Get_State(ALA_EPM2_5) == ALA_STA_OFF) {
        if (collect_er[CL_PM2_5] == 0) {
            sta_in.pm2_5 = collect_result[CL_PM2_5];
            sta_in.pm10 = collect_result[CL_PM10];
        }
    } else {
        sta_in.pm2_5 = SENSOR_ER_VALUE;
        sta_in.pm10 = SENSOR_ER_VALUE;
    }

    if (collect_er[CL_PM10] >= UART_TIMES) {
        collect_er[CL_PM10] = UART_TIMES;
    }

    if (collect_er[CL_LIGHT] >= UART_TIMES) {
        collect_er[CL_LIGHT] = UART_TIMES;
        ALA_Set_State(ALA_ELIGHT, ALA_STA_ON);
    } else {
        ALA_Set_State(ALA_ELIGHT, ALA_STA_OFF);
    }

    if (dev_inf.env_dev[MB_DEV_LIGHT].enable == STA_ON && ALA_Get_State(ALA_ELIGHT) == ALA_STA_OFF) {
        if (collect_er[CL_LIGHT] == 0) {
            sta_in.light = collect_result[CL_LIGHT];
        }
    } else {
        sta_in.light = LIGHT_ER_VALUE;
    }


    if (water_collect_time == 0 || water_realtime_page == STA_ON) { //采集的时候才计算
//计算水表值
        if (ext_inf.din.pro_data[0].water_sta == WL_CJT_188 || ext_inf.din.pro_data[0].water_sta == WL_MB_OWN) {
            if (collect_er[CL_WATER] == 0) {
                sta_in.water[id.water] = collect_result[CL_WATER];
            }
        } else {
            sta_in.water[id.water] = (unsigned int)DATA_ER_VALUE;
        }

//计算水表报警
        if (collect_er[CL_WATER] != 0) {
            collect_er[CL_WATER] = 1;
            id.water_er[id.water]++;
        } else {
            id.water_er[id.water] = 0;
        }

        if (id.water_er[id.water] >= UART_TIMES / 3) { // 10次就报警
            id.water_er[id.water] = UART_TIMES;
            sta_in.water[id.water] = (unsigned int)DATA_ER_VALUE;
        }

        for (i = 0; i < PRO_DATA_MAX; i++) {
            if (id.water_er[i] >= UART_TIMES) {
                ALA_Set_State(ALA_EWATER, ALA_STA_ON);
                break;
            }
        }

        if (i >= PRO_DATA_MAX) {
            ALA_Set_State(ALA_EWATER, ALA_STA_OFF);
        }
    }

//计算料塔值
    if (ext_inf.din.pro_data[0].food_sta == FL_MB_OWN || ext_inf.din.pro_data[0].food_sta == FL_MB_XK3102) {
        if (collect_er[CL_FOOD] == 0) {
            if (ext_inf.din.mb_data[MB_DATA_FOOD].dx.len == 2) {
                food_temp = (signed short)(collect_result[CL_FOOD] * pow(10, ext_inf.din.mb_data[MB_DATA_FOOD].dx.point) / FOOD_RATE);

                if (food_temp < 0) {
                    sta_in.food[id.food] = 0;
                } else {
                    sta_in.food[id.food] = collect_result[CL_FOOD];
                }
            } else {
                if (collect_result[CL_FOOD] < 0) {
                    sta_in.food[id.food] = 0;
                } else {
                    sta_in.food[id.food] = collect_result[CL_FOOD];
                }
            }
        }
    } else {
        sta_in.food[id.food] = (unsigned int)DATA_ER_VALUE;
    }

//计算料塔报警
    if (collect_er[CL_FOOD] != 0) {
        collect_er[CL_FOOD] = 1;
        id.food_er[id.food]++;
    } else {
        id.food_er[id.food] = 0;
    }

    if (id.food_er[id.food] >= UART_TIMES) {
        id.food_er[id.food] = UART_TIMES;
        sta_in.food[id.food] = (unsigned int)DATA_ER_VALUE;
    }

    for (i = 0; i < PRO_DATA_MAX; i++) {
        if (id.food_er[i] >= UART_TIMES) {
            ALA_Set_State(ALA_EFOOD, ALA_STA_ON);
            break;
        }
    }

    if (i >= PRO_DATA_MAX) {
        ALA_Set_State(ALA_EFOOD, ALA_STA_OFF);
    }
}

void cal_food_total(void)
{
    int i = id.food;

    if (ext_inf.din.pro_data[0].food_sta != FL_MB_OWN && ext_inf.din.pro_data[0].food_sta != FL_MB_XK3102) { //不是称重
        return;
    }

    if (collect_er[CL_FOOD] == 1 || id.food_er[i] >= UART_TIMES || sta_in.food[i] == (unsigned int)DATA_ER_VALUE) {
        return;
    }

    if (sta_in.old_food[i] == (unsigned int)DATA_ER_VALUE) {
        sta_in.old_food[i] = sta_in.food[i];
        return;
    }

    if (abs(sta_in.old_food[i] - sta_in.food[i]) < (ctrl_inf.var.food_min * FOOD_RATE)) {
        return;
    }

    if (sta_in.old_food[i] > sta_in.food[i]) {
        index_inf.food[i][0] += (sta_in.old_food[i] - sta_in.food[i]);
    } else {
        index_inf.addfood[i][0] += (sta_in.food[i] - sta_in.old_food[i]);
    }

    sta_in.old_food[i] = sta_in.food[i];

    Config_Save(SINDEX_ID);

}

void Sensors_Task(void * pdata)
{

    UARTX_SENSOR = UART3_SENSOR;

#if USE_GD == 1

    if (basic_inf.comm_info.ux.port == 1) {
        UARTX_SENSOR = UART6_COMM;
    }

#endif


    OSTimeDlyHMSM(0, 0, 5, 0);

    memset(&id.energy, 0, sizeof(TID));
    memset(&sta_in.old_food[0], 0xFF, sizeof(sta_in.old_food));
    sys_sta.food_adjut_state = STA_OFF;

    for (;;) {
        task_time[Sensors_id] = 0;
        Sensors_Init();
        Sensors_Collect();
        Sensors_Result();
        cal_food_total();
        collect_suspend();
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

void Uart2_Rec_Task(void * pdata)
{
    u8 os_err = 0;

    for (;;) {

        os_err = WaitUartIdle( UART2_HMI, 0 );

        if (os_err == OS_ERR_NONE) {
        }

        OSTimeDlyHMSM(0, 0, 0, 10);
    }
}



TWR_PAPM WR_PAPM[FOOD_ADJUST_NUM + 1] = {
    //xk3102
    {1, 0x16, 0x88},
    {1, 0x17, 0},
    {1, 0x18, 0},
};

void food_adjust_int(unsigned char id)
{
    int i = 0;

    if (id >= PRO_DATA_MAX) {
        return;
    }

    if (ext_inf.din.pro_data[0].food_sta != FL_MB_XK3102) {
        return;
    }

    UartInit( UARTX_SENSOR, ext_inf.din.mb_data[MB_DATA_FOOD].baud, ext_inf.din.mb_data[MB_DATA_FOOD].ux.dbits,
              ext_inf.din.mb_data[MB_DATA_FOOD].ux.parity, ext_inf.din.mb_data[MB_DATA_FOOD].ux.stopb);

    for (i = 0; i < FOOD_ADJUST_NUM + 1; i++) {
        WR_PAPM[i].addr = ext_inf.din.pro_data[id].food_addr;
    }
}

int MODBUS_Write_Data(const PWR_PAPM pWrParm)
{
    int			msgLn, ret, bytes;
    unsigned char os_err = 0;

    ClearUartRx(UARTX_SENSOR);
	ClearUartIdle(UARTX_SENSOR);
    bytes = MODBUS_WriteMsg( pWrParm->addr, COM_WR_REG, (BYTE*)&pWrParm->reg, (BYTE*)&pWrParm->data, UART3TxBuf, sizeof(UART3TxBuf));
    SendUartData( UARTX_SENSOR, UART3TxBuf, bytes );
    os_err = WaitUartIdle( UARTX_SENSOR, UART_TIME_OUT);

    if (os_err != OS_ERR_NONE) {
        return -3;
    }

    msgLn = GetUartData( UARTX_SENSOR, UART3RxBuf, sizeof(UART3RxBuf) );
    ret = MODBUS_GetWrAck( pWrParm->addr, COM_WR_REG, (BYTE*)&pWrParm->reg, (BYTE*)&pWrParm->data, UART3RxBuf, msgLn);

    return ret;
}

int food_adjust_proc(unsigned char id, unsigned char type, unsigned short value)
{
    int ret = -1;

    if (id >= PRO_DATA_MAX || type > FOOD_ADJUST_NUM) {
        return -1;
    }

    if (type == FOOD_ADJUST_NUM) {
        WR_PAPM[type].data = value * pow(10, ext_inf.din.mb_data[MB_DATA_FOOD].dx.point);
    }

    ret = MODBUS_Write_Data((PWR_PAPM)&WR_PAPM[type].addr);

    return ret;
}




