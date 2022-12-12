#ifndef EXLNK_SET_HEADER_H_
#define EXLNK_SET_HEADER_H_

#include "exactolink/exlnk_options.h"
#include "exactolink/exlnk_Cmd.h"

typedef struct exlnk_set_header_str{
    uint16_t adr;
    uint8_t type_msg;
    uint8_t type_pack;
    uint16_t len;
    uint32_t cnt;
    uint16_t ovrflw;
    uint16_t pt_data;
    uint8_t * data;
    uint8_t packs;
    uint8_t is_closed;
}exlnk_set_header_str_t;


extern void exlnk_setHeader( uint16_t adr, uint8_t * trg, uint8_t type_msg, uint8_t type_pack, uint16_t len, uint32_t cnt, uint16_t ovrflw);
extern void exlnk_setEnder(uint8_t * trg);
extern uint8_t exlnk_isEmptySetHeader(exlnk_set_header_str_t * trg);
extern void exlnk_clearSetHeader(exlnk_set_header_str_t * trg);

extern void exlnk_initHeader(exlnk_set_header_str_t * trg, uint8_t * buffer);
extern void exlnk_fillHeader(exlnk_set_header_str_t * trg, uint16_t adr, uint8_t type_msg, uint8_t type_pack, uint16_t len, uint32_t cnt, uint16_t ovrflw);
extern void exlnk_uploadHeader(exlnk_set_header_str_t * trg, uint8_t * datasrc, uint16_t datalen);
extern void exlnk_closeHeader(exlnk_set_header_str_t * trg);
extern void exlnk_uploadCmdHeader(exlnk_set_header_str_t * trg, exlnk_cmd_str_t * cmd);



#endif