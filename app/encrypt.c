#include "encrypt.h"

#if USE_BOOT== 1
// 密码存储区
const unsigned char encrypt_code[FLASH_SECTOR_SIZE] __attribute__((at(ADDR_FLASH_SECTOR))) = {
    eID0, eID1, eID2, eID3, eID4, eID5, eID6, eID7, eID8, eID9, eID10, eID11,
};
#endif

// 通用密码和CRC计算数据
const unsigned char crc_tmp_data[128] = {
    // 		0 	  1 	2 	  3 	4 	  5	   6 	  7 	8 	  9 	A 	  B 	C 	  D 	E 	 F
    /*0*/ 0x32, 0x18, 0xCb, 0x5a, 0xdE, 0x21, 0xff, eID0, 0x62, 0x00, eID6, 0x24, 0x66, 0x27, 0x34, 0x5F,
    /*1*/ 0x2b, 0x93, 0x21, 0x62, 0x42, 0x54, 0x68, 0x76, 0xea, eID1, eID7, 0x89, 0xfe, 0xef, 0x2c, 0xcd,
    /*2*/ 0x00, 0x21, UID0, 0x12, 0x44, 0x45, 0xc6, 0x77, 0x88, eID2, 0xfe, 0xbC, eID8, 0x3a, 0xce, 0x2c,
    /*3*/ 0x31, UID1, 0x32, 0xf0, 0x55, 0x44, eID3, eID9, 0x34, 0x04, 0x45, 0x01, 0x23, 0x67, 0x54, 0x05,
    /*4*/ 0xaa, 0x2b, 0x88, UID2, 0xf7, 0xff, 0xcc, 0x3d, eID4, 0x33, eID10, 0x00, 0xd7, 0x66, 0x55, 0xb4,
    /*5*/ 0x5b, 0xaa, UID3, 0xe4, 0x2f, 0xf7, 0xdd, 0xfe, 0xff, 0x12, eID11, 0xcf, 0x00, 0x0a, 0x91, 0x78,
    /*6*/ 0x18, 0xa9, 0xca, UID3 + 4, 0xdc, 0x2d, 0xf3, 0x6f, 0x2c, eID5, 0x30, 0x20, 0x50, 0x25, 0x76, 0x3a,
    /*7*/ 0x48, 0x55, 0x87, 0x08, UID3 + 8, 0x61, 0x22, 0x23, 0x66, 0x2c, 0x4c, 0x55, 0x2c, 0x33, 0x00, 0x41,
};

// 本机ID存储变量
volatile union {
    uint32_t dat32[3];
    uint8_t dat8[12];
} mcuID;
// 本机密码存储变量
volatile uint8_t eCODE[12];
/*
函数名称: calculate_eCODE
功 能: 计算本机密码
(本函数当前没有进行复杂的算法设计,只简单地进行按位取反)
输 入: pUID,UID数组指针
输 出: peCODE,本机密码
返 回: 无
*/
void calculate_eCODE(uint8_t *pUID, uint8_t *peCODE)
{
    peCODE[0] = pUID[0] ^ eID0_OFFSET;
    peCODE[1] = pUID[1] ^ eID1_OFFSET;
    peCODE[2] = pUID[2] ^ eID2_OFFSET;
    peCODE[3] = pUID[3] ^ eID3_OFFSET;
    peCODE[4] = pUID[4] ^ eID4_OFFSET;
    peCODE[5] = pUID[5] ^ eID5_OFFSET;
    peCODE[6] = pUID[6] ^ eID6_OFFSET;
    peCODE[7] = pUID[7] ^ eID7_OFFSET;
    peCODE[8] = pUID[8] ^ eID8_OFFSET;
    peCODE[9] = pUID[9] ^ eID9_OFFSET;
    peCODE[10] = pUID[10] ^ eID10_OFFSET;
    peCODE[11] = pUID[11] ^ eID11_OFFSET;
}

//写内部flash里面
void encrypt_code_save(void)
{
    uint32_t write_addr;
    uint32_t i;
    uint8_t *pbuf;
    uint16_t data;
    FLASH_Unlock();
#if USE_F1 == 1

    if (FLASH_ErasePage(ADDR_FLASH_SECTOR) == FLASH_COMPLETE) {
        write_addr = ADDR_FLASH_SECTOR;
        pbuf = (uint8_t *)eCODE;

        for (i = 0; i < 12;) {
            data = pbuf[i] + (pbuf[i + 1] << 8);
            FLASH_ProgramHalfWord((write_addr + i), data);
            i += 2;
        }
    }

#else

    if (FLASH_EraseSector(ADDR_FLASH_REAL, VoltageRange_3) == FLASH_COMPLETE) {
        write_addr = ADDR_FLASH_SECTOR;
        pbuf = (uint8_t *)eCODE;

        for (i = 0; i < 12; ) {
            data = pbuf[i] + (pbuf[i + 1] << 8);
            FLASH_ProgramHalfWord((write_addr + i), data);
            i += 2;
        }
    }

#endif
    FLASH_Lock();
}
/*
函数名称: read_ID
功 能: 读取ID(cpu id ,18b20 ID可以直接读，剩余的直接可以直接填入固定的值)
输 入: 无
输 出: 读到的UID存入pUID_dat32指向的存储空间
返 回: 无
*/
void read_ID(uint32_t *pUID_dat32)
{
    uint32_t mcuID_add;
    uint32_t tmp[4];
// 31:0
// 为了更安全,不使用用立即数访问UID,
// 所以将UID的地址分散在crc_tmp_data中,然后通过读取crc_tmp_data合成UID地址
    tmp[0] = *(__IO uint8_t*)&crc_tmp_data[UID0_OFFSET];
    tmp[1] = *(__IO uint8_t*)&crc_tmp_data[UID1_OFFSET];
    tmp[2] = *(__IO uint8_t*)&crc_tmp_data[UID2_OFFSET];
    tmp[3] = *(__IO uint8_t*)&crc_tmp_data[UID3_OFFSET_1];
    mcuID_add = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3]; // 合成UID地址
    pUID_dat32[0] = *(__IO uint32_t*)(mcuID_add); // 读取UID[31:0]
// 63:32
    tmp[0] = *(__IO uint8_t*)&crc_tmp_data[UID0_OFFSET];
    tmp[1] = *(__IO uint8_t*)&crc_tmp_data[UID1_OFFSET];
    tmp[2] = *(__IO uint8_t*)&crc_tmp_data[UID2_OFFSET];
    tmp[3] = *(__IO uint8_t*)&crc_tmp_data[UID3_OFFSET_2];
    mcuID_add = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3];
    pUID_dat32[1] = *(__IO uint32_t*)(mcuID_add);
// 95:64
    tmp[0] = *(__IO uint8_t*)&crc_tmp_data[UID0_OFFSET];
    tmp[1] = *(__IO uint8_t*)&crc_tmp_data[UID1_OFFSET];
    tmp[2] = *(__IO uint8_t*)&crc_tmp_data[UID2_OFFSET];
    tmp[3] = *(__IO uint8_t*)&crc_tmp_data[UID3_OFFSET_3];
    mcuID_add = (tmp[0] << 24) + (tmp[1] << 16) + (tmp[2] << 8) + tmp[3];
    pUID_dat32[2] = *(__IO uint32_t*)(mcuID_add);
}

#if USE_BOOT== 1
/*
函数名称: check_ID
功 能: 校验ID
输 入: 无
返 回: HAL_OK,校验成功;HAL_ERROR,失败
*/
int Check_ID(void)
{
// Step1.读取本机ID,存入mcuID联合体
    read_ID((uint32_t*)mcuID.dat32);
// Step2.使用本机ID计算本机密码,存入eCODE数组
    calculate_eCODE((uint8_t*)mcuID.dat8, (uint8_t*)eCODE);

// Step3.对比FLASH中存储的密码是否等于通用密码
    if ((crc_tmp_data[eID0_OFFSET] == *(__IO uint8_t*)&encrypt_code[0])
            && (crc_tmp_data[eID1_OFFSET] == *(__IO uint8_t*)&encrypt_code[1])
            && (crc_tmp_data[eID2_OFFSET] == *(__IO uint8_t*)&encrypt_code[2])
            && (crc_tmp_data[eID3_OFFSET] == *(__IO uint8_t*)&encrypt_code[3])
            && (crc_tmp_data[eID4_OFFSET] == *(__IO uint8_t*)&encrypt_code[4])
            && (crc_tmp_data[eID5_OFFSET] == *(__IO uint8_t*)&encrypt_code[5])
            && (crc_tmp_data[eID6_OFFSET] == *(__IO uint8_t*)&encrypt_code[6])
            && (crc_tmp_data[eID7_OFFSET] == *(__IO uint8_t*)&encrypt_code[7])
            && (crc_tmp_data[eID8_OFFSET] == *(__IO uint8_t*)&encrypt_code[8])
            && (crc_tmp_data[eID9_OFFSET] == *(__IO uint8_t*)&encrypt_code[9])
            && (crc_tmp_data[eID10_OFFSET] == *(__IO uint8_t*)&encrypt_code[10])
            && (crc_tmp_data[eID11_OFFSET] == *(__IO uint8_t*)&encrypt_code[11])
       ) {
// 如果密码等于通用密码,则将计算的结果写入FLASH
        encrypt_code_save();
        return 0;//第一次运行
    } else

// Step4.对比计算的本机密码是否等于FLASH中存储的密码
        if ((eCODE[0] == *(__IO uint8_t*)&encrypt_code[0])
                && (eCODE[1] == *(__IO uint8_t*)&encrypt_code[1])
                && (eCODE[2] == *(__IO uint8_t*)&encrypt_code[2])
                && (eCODE[3] == *(__IO uint8_t*)&encrypt_code[3])
                && (eCODE[4] == *(__IO uint8_t*)&encrypt_code[4])
                && (eCODE[5] == *(__IO uint8_t*)&encrypt_code[5])
                && (eCODE[6] == *(__IO uint8_t*)&encrypt_code[6])
                && (eCODE[7] == *(__IO uint8_t*)&encrypt_code[7])
                && (eCODE[8] == *(__IO uint8_t*)&encrypt_code[8])
                && (eCODE[9] == *(__IO uint8_t*)&encrypt_code[9])
                && (eCODE[10] == *(__IO uint8_t*)&encrypt_code[10])
                && (eCODE[11] == *(__IO uint8_t*)&encrypt_code[11])
           ) {
// 如果密码校验正确,则不进行任何操作
            return 1;
        } else {
// 如果密码校验错位,则关机或者销毁程序
            return -1;
        }
}
#endif


int NoWrite_Check_ID(void)
{
	uint8_t *save_code = (uint8_t*)ADDR_FLASH_SECTOR;
// Step1.读取本机ID,存入mcuID联合体
    read_ID((uint32_t*)mcuID.dat32);
// Step2.使用本机ID计算本机密码,存入eCODE数组
    calculate_eCODE((uint8_t*)mcuID.dat8, (uint8_t*)eCODE);

// Step4.对比计算的本机密码是否等于FLASH中存储的密码
    if ((eCODE[0] == save_code[0])
            && (eCODE[1] == save_code[1])
            && (eCODE[2] == save_code[2])
            && (eCODE[3] == save_code[3])
            && (eCODE[4] == save_code[4])
            && (eCODE[5] == save_code[5])
            && (eCODE[6] == save_code[6])
            && (eCODE[7] == save_code[7])
            && (eCODE[8] == save_code[8])
            && (eCODE[9] == save_code[9])
            && (eCODE[10] == save_code[10])
            && (eCODE[11] == save_code[11])
       ) {
// 如果密码校验正确,则不进行任何操作
        return 1;
    } else {
// 如果密码校验错位,则关机或者销毁程序
        return -1;
    }
}

uint8_t * Get_McuID(void)
{
    return (uint8_t*)mcuID.dat8;
}

uint32_t Get_McuID8(uint8_t id)
{
    if (id >= 12)return 0;

    return mcuID.dat8[id];
}

uint8_t Get_McuID_Len(void)
{
    return sizeof(mcuID);
}



