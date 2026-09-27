#include "exactolink/exlnk_Data.h"
#include <string.h>

void exlnk_setData(exlnk_data_str_t * trg, uint8_t reg, uint16_t len, uint8_t *data)
{
	trg->id = EXLNK_DATA_ID_DATA;
	trg->reg = reg;
	trg->len = len;
	trg->data = data;
}
uint16_t exlnk_DataToArray(exlnk_data_str_t * src, uint8_t * trg, uint16_t datalen)
{
	uint16_t full_length = 8 + src->len;
	if(datalen < full_length)
		return 0;
	trg[0] = src->id;
	trg[1] = (uint8_t)src->mnum;
	trg[2] = (uint8_t)(src->mnum >> 8);
	trg[3] = (uint8_t)(src->mnum >> 16);
	trg[4] = (uint8_t)(src->mnum >> 24);
	trg[5] = src->reg;
	trg[6] = (uint8_t)(src->len);
	trg[7] = (uint8_t)(src->len >> 8);
	memcpy(&trg[8], src->data, src->len);
	return full_length;
}
uint8_t exlnk_getData(exlnk_data_str_t * trg, uint8_t * datastore, uint16_t datastorelen, uint8_t * src, uint16_t srclen)
{
	size_t len = sizeof(exlnk_data_str_t) - sizeof(trg->data);
	if(srclen < len)
		return 0;
	memcpy(trg, src, len);
	if (trg->id != EXLNK_DATA_ID_DATA || trg->len > datastorelen)
		return 0;
	trg->data = datastore;
	memcpy(trg->data, &src[len], trg->len);
	return 1;
}

