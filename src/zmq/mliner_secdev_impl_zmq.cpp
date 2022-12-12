
#include "mliner/mliner_secdev_impl.h"

#include "zmq/exmliner.h"

exMlinerZMQ Client;

void connectRxPortMlDev(const char* name)
{
	Client.connectToPort(name);
}
void createTxPortMlDev(const char* name)
{
	Client.createServer(name);
}
void forceInitBoardMlDev(void)
{
   mlimpl_initBoard();
}

int mlimpl_initBoard(void)
{
	Client.init();
   return 0;
}
void mlimpl_setBoardBuffer(
	spi_mline_dev_t * transmit, spi_mline_dev_t * receive
	// uint8_t * rxdata, uint32_t rxlen, uint8_t * txdata, uint32_t txlen, int (*download)(uint32_t len), int(*upload)(uint32_t len)
	)
{
	// LL_DMA_ConfigAddresses(DMA1,
	// 								LL_DMA_CHANNEL_4,
	// 								LL_SPI_DMA_GetRegAddr(SPI2),
	// 								(uint32_t)receive->dmabufferdata,
	// 								LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_4));
	// LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, receive->dmabufferlen);
	// LL_DMA_ConfigAddresses(DMA1,
	// 								LL_DMA_CHANNEL_5, 
	// 								(uint32_t)transmit->dmabufferdata,
	// 								LL_SPI_DMA_GetRegAddr(SPI2),
	// 								LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5));
	// LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, transmit->dmabufferlen);
	Client.RxBuffer = receive->dmabufferdata;
	Client.RxBuffLen = &receive->dmabufferlen;
	Client.TxBuffer = transmit->dmabufferdata;
	Client.TxBuffLen = &transmit->dmabufferlen;
}
uint8_t mlimpl_runBoardIRQhandlerRX(void)
{
	return 1;
}
uint8_t mlimpl_runBoardIRQhandlerTX(void)
{

	return 1;
}
void mlimpl_enableBoard(uint32_t rxlen, uint32_t txlen)
{
	// LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_4);
	// LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_4);
	// LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
	// LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_5);

	// LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, txlen);
	// LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, rxlen);

	// LL_SPI_EnableDMAReq_RX(SPI2);
	// LL_SPI_EnableDMAReq_TX(SPI2);
	// LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);   //receive
	// LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);   //transmit
	// LL_SPI_Enable(SPI2);
}
void mlimpl_disableBoard(void)
{
	// LL_SPI_Disable(SPI2);
	// LL_SPI_DeInit(SPI2);
	// LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_4);
	// LL_DMA_DeInit(DMA1, LL_DMA_CHANNEL_5);
}

void mlimpl_receiveBoard(spi_mline_dev_t * receiver)
{
	//     LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4);
   //  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, receiver->dmabufferlen);
   //  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
	int len = Client.recv(Client.RxBuffer, *(Client.RxBuffLen));
	//*(Client.RxBuffLen) = len;
	receiver->isready = 1;
}

void mlimpl_resetBoardRx(spi_mline_dev_t * receiver)
{
	// LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4); //receive
	// LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, receiver->dmabufferlen);
	// LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);   //receive
	mlimpl_receiveBoard(receiver);
}
void mlimpl_resetBoardRxTx(spi_mline_dev_t * receiver, spi_mline_dev_t * transmit)
{
	// LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4); //receive
	// LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5); //transmit
	// LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, transmit->dmabufferlen);
	// LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, receiver->dmabufferlen);
	// LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);   //receive
	// LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5); //transmit
	Client.send(Client.TxBuffer, *(Client.TxBuffLen));
	int len = Client.recv(Client.RxBuffer, *(Client.RxBuffLen));
	//*(Client.RxBuffLen) = len;
	//*(Client.TxBuffLen) = 0;
	transmit->isready = 1;
	receiver->isready = 1;
}
void mlimpl_receiveTransmitBoard(spi_mline_dev_t * receiver, spi_mline_dev_t * transmit)
{
	// LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_4); //receive
   // LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5); //transmit

	// receiver->processData(receiver->dmabufferlen);
	transmit->processData(transmit->dmabufferlen);
	Client.send(Client.TxBuffer, *(Client.TxBuffLen));
	int len = Client.recv(Client.RxBuffer, *(Client.RxBuffLen));
	//*(Client.RxBuffLen) = len;
	//*(Client.TxBuffLen) = 0;
	transmit->isready = 1;
	receiver->isready = 1;

	// LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, transmit->dmabufferlen);
   // LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, receiver->dmabufferlen);
   // LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);   //receive
   // LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5); //transmit
}


