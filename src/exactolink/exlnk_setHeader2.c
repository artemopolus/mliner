#include "exactolink/exlnk_setHeader.h"


void exlnk_setEnder(uint8_t * trg)
{
    trg[0] = 5;
    trg[1] = 5;
    trg[2] = 5;
    trg[3] = 5;
}

uint8_t exlnk_isEmptySetHeader(exlnk_set_header_str_t * trg)
{
    if (trg->type_msg == EXLNK_MSG_NONE)
        return 1;
    else
        return 0;
}
void exlnk_clearSetHeader(exlnk_set_header_str_t * trg)
{
    trg->type_msg = EXLNK_MSG_NONE;
    trg->pt_data = 0;
    trg->packs = 0;
    trg->is_closed = 0;
}
