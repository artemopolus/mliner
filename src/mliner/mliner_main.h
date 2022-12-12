#ifndef EXACTO_MLINER_MLINER_MAIN_H_
#define EXACTO_MLINER_MLINER_MAIN_H_

#include <stdint.h>
#include <stddef.h>
#include "exactolink/exlnk_Cmd.h"

#include "mliner/mliner.h"

#define MLINER_SEC_MSG_SIZE EXACTO_BUFFER_UINT8_SZ

extern void exmliner_Init(uint8_t type, uint16_t address);
extern void exmliner_Upload(void * data, size_t len, uint8_t id, uint16_t adr);
extern void exmliner_Update(uint16_t adr);
extern void exmliner_setCmdAction(int(*cmdaction)(exlnk_cmd_str_t * out));
extern void exmliner_setCmdAckAction(int(*cmdackaction)(exlnk_cmdack_str_t * out));
extern void exmliner_setResetAction(int(*resetaction)());
extern void exmliner_setRepeatAction(int(*repeataction)(uint8_t id, uint32_t mnum));
extern void exmliner_setErrorAction(int(*erroraction)(int id));

extern uint8_t exmliner_checkAck(exlnk_cmdack_str_t * ack, uint16_t address);

extern uint8_t exmliner_getRxIRQ();
extern uint8_t exmliner_getTxIRQ();


#endif //EXACTO_MLINER_MLINER_MAIN_H_
