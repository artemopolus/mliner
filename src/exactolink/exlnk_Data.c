#include "exactolink/exlnk_Data.h"
#include <string.h>

void exlnk_setData(exlnk_data_str_t * trg, uint8_t reg, uint16_t len, uint8_t *data)
{
	trg->id = EXLNK_DATA_ID_DATA;
	trg->reg = reg;
	trg->len = len;
	trg->data = data;
}
uint8_t exlnk_DataToArray(exlnk_data_str_t * src, uint8_t * trg, uint16_t datalen)
{
	size_t len = sizeof(exlnk_data_str_t) - sizeof(src->data);
	if(datalen < len + src->len)
		return 0;
	memcpy(trg, src, len);
	memcpy(&trg[len], src->data, src->len);
	return 1;
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

