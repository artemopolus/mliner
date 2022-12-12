#ifndef MLINER_MAINDEV_IMPL_H_
#define MLINER_MAINDEV_IMPL_H_



#include "mliner/mliner_basedev.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC extern
#endif


EXTERNC mliner_devs_block_t MlinerDevsBlock;

EXTERNC int mlimpl_initBoard(void);

EXTERNC void mlimpl_setBoardBuffer(
	spi_mline_dev_t * transmit, spi_mline_dev_t * receive
	// uint8_t * rxdata, uint32_t rxlen, uint8_t * txdata, uint32_t txlen, int (*download)(uint32_t len), int(*upload)(uint32_t len)
	);

EXTERNC uint8_t mlimpl_runBoardIRQhandlerTX(void);
EXTERNC uint8_t mlimpl_runBoardIRQhandlerRX(void);

EXTERNC void mlimpl_enableBoard(uint32_t rxlen, uint32_t txlen);
EXTERNC void mlimpl_disableBoard(void);

EXTERNC void mlimpl_receiveBoard(spi_mline_dev_t * receiver);

EXTERNC void mlimpl_resetBoardRx(spi_mline_dev_t * receiver);
EXTERNC void mlimpl_resetBoardRxTx(spi_mline_dev_t * receiver, spi_mline_dev_t * transmit);

EXTERNC void mlimpl_receiveTransmitBoard(spi_mline_dev_t * receiver, spi_mline_dev_t * transmit);

#endif //MLINER_MAINDEV_IMPL_H_