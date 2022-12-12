#ifndef EXLNK_OPTIONS_H_
#define EXLNK_OPTIONS_H_

#include "stdint.h"
#define EXLNK_START_DATA_POINT_ADR 4
#define EXLNK_START_DATA_POINT_VAL 13
#define EXLNK_XLXLGR_START_DATA_POINT_VAL 20
#define EXLNK_PCK_ID 0x11
#define EXLNK_SNS_ID 0x17
#define EXLNK_RAW_ID 0x19

#define EXLNK_MSG_NONE          0x00
#define EXLNK_MSG_SIMPLE        0x01
#define EXLNK_MSG_SIMPLE_CRC    0x02
#define EXLNK_MSG_SMPL_CMD      0x03
#define EXLNK_MSG_SMPL_CMD_CRC  0x04


#define EXLNK_DATA_ID_CMD	        3
#define EXLNK_DATA_ID_CMDACK        4
#define EXLNK_DATA_ID_DATA	        5
#define EXLNK_DATA_ID_AMPERVOLTS    25
#define EXLNK_DATA_ID_PH            24
#define EXLNK_DATA_ID_TEMPERATURE   23

#define EXLNK_PACK_SIMPLE 0x14


#endif