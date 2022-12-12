#ifndef EXLNK_DATA_H_
#define EXLNK_DATA_H_


#include "exactolink/exlnk_options.h"

typedef struct exlnk_data_str{
	uint8_t id;
   uint32_t mnum;       // адрес, счетчик, номер по порядку
   uint8_t reg;
	uint16_t len;
	uint8_t * data;
}exlnk_data_str_t;

extern void exlnk_setData(exlnk_data_str_t * trg, uint8_t reg, uint16_t len, uint8_t *data);
extern uint8_t exlnk_DataToArray(exlnk_data_str_t * src, uint8_t * trg, uint16_t datalen);
extern uint8_t exlnk_getData(exlnk_data_str_t * trg, uint8_t * datastore, uint16_t datastorelen, uint8_t * src, uint16_t srclen);


#endif //EXLNK_DATA_H_

