#include "exactolink/exlnk_getHeader.h"

uint8_t exlnk_getHeaderPackData(exlnk_get_header_str_t * trg, uint8_t * value)
{
    if(trg->datapt < trg->datalen)
        *value = trg->data[trg->datapt++];
    else
        return 0;
    return 1;
}
uint16_t exlnk_isEmptyGetHeader(exlnk_get_header_str_t * trg)
{
    if( trg->len > trg->datapt + 4)
        return (trg->len - trg->datapt - 4);
    else
        return 0;
}
