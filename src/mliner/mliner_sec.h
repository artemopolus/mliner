#ifndef EXACTO_MLINER_MLINER_SEC_H_
#define EXACTO_MLINER_MLINER_SEC_H_

#include <stdint.h>
#include <stddef.h>
#include "exactolink/exlnk_Cmd.h"

#define MLINER_SEC_MODULE_ADDRESS	7 


extern void exmliner_Init(uint16_t address);
extern void exmliner_Upload(void * data, size_t len, uint8_t id);
extern void exmliner_Update();
extern void exmliner_setCmdAction(int(*cmdaction)(exlnk_cmd_str_t * out));
extern void exmliner_setCmdAckAction(int(*cmdackaction)(exlnk_cmdack_str_t * out));
extern void exmliner_setResetAction(int(*resetaction)());
extern void exmliner_setRepeatAction(int(*repeataction)(uint8_t id, uint32_t mnum));
extern void exmliner_setErrorAction(int(*erroraction)(int id));

extern uint8_t exmliner_getRxIRQ();
extern uint8_t exmliner_getTxIRQ();


#endif //EXACTO_MLINER_MLINER_SEC_H_
