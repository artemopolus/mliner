#ifndef MLINE_SECDEV_H_
#define MLINE_SECDEV_H_

#include "operator/exacto_buffer.h"

#ifndef MLINE_RXTX_BUFFER_SIZE
#define MLINE_RXTX_BUFFER_SIZE 512
#endif


extern void mldev_setTxBuff(ExactoBufferUint8Type * buffer);
extern void mldev_setRxBuff(ExactoBufferUint8Type * buffer);

extern void mldev_setReceiver(int(*receiveProcess)(uint8_t * data, uint16_t datalen));
extern void mldev_setTransmit(int(*transmitProcess)(uint8_t * data, uint16_t datalen));

extern void mldev_receive();
extern uint8_t mldev_repeatTransmit();
extern uint8_t mldev_transmit();
extern void mldev_enable();
extern void mldev_disable();

extern uint16_t mldev_getReceivedData(uint8_t * trg, uint16_t trglen);
extern uint16_t mldev_setTransmitData(uint8_t * src, uint16_t srclen);

extern uint8_t mldev_getTransmitResult();
extern uint8_t mldev_getReceiveResult();




#endif