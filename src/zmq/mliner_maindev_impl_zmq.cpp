#include "mliner/mliner_maindev_impl.h"

#include "zmq/exmliner.h"

exMlinerZMQ Server;

void connectRxPortMlDev(const char* name)
{
	Server.connectToPort(name);
}
void createTxPortMlDev(const char* name)
{
	Server.createServer(name);
}
void forceInitBoardMlDev(void)
{
   mlimpl_initBoard();
}
int mlimpl_initBoard(void)
{
	Server.init();
    return 0;
}

void mlimpl_setBoardBuffer(
	spi_mline_dev_t * transmit, spi_mline_dev_t * receive
	// uint8_t * rxdata, uint32_t rxlen, uint8_t * txdata, uint32_t txlen, int (*download)(uint32_t len), int(*upload)(uint32_t len)
	)
{
   //  LL_DMA_ConfigAddresses(SPI_MLINE_DMA, 
   //                          SPI_MLINE_DMA_STREAM_RX,
   //                         LL_SPI_DMA_GetRegAddr(SPI_MLINE_SPI),
	// 						(uint32_t)receive->dmabufferdata,
   //                         LL_DMA_GetDataTransferDirection(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX));
   //  LL_DMA_SetDataLength(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX, receive->dmabufferlen );

   //  LL_DMA_ConfigAddresses(SPI_MLINE_DMA,
   //                         SPI_MLINE_DMA_STREAM_TX, 
	// 						(uint32_t)transmit->dmabufferdata,
   //                         LL_SPI_DMA_GetRegAddr(SPI_MLINE_SPI),
   //                         LL_DMA_GetDataTransferDirection(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX));
   //  LL_DMA_SetDataLength(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX, transmit->dmabufferlen);
	Server.RxBuffer = receive->dmabufferdata;
	Server.RxBuffLen = &receive->dmabufferlen;
	Server.TxBuffer = transmit->dmabufferdata;
	Server.TxBuffLen = &transmit->dmabufferlen;
}

uint8_t mlimpl_runBoardIRQhandlerRX(void)
{
   //  if (LL_DMA_IsActiveFlag_TC0(SPI_MLINE_DMA) != RESET)
   //  {
   //      LL_DMA_ClearFlag_TC0(SPI_MLINE_DMA);
         return 1;
   //  }
    //return 0;
}
uint8_t mlimpl_runBoardIRQhandlerTX(void)
{
   //  if (LL_DMA_IsActiveFlag_TC5(SPI_MLINE_DMA) != RESET)
   //  {
   //      LL_DMA_ClearFlag_TC5(SPI_MLINE_DMA);
         return 1;
   //  }
    //return 0;
}
void mlimpl_enableBoard(uint32_t rxlen, uint32_t txlen)
{
   //  LL_DMA_EnableIT_TC(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX);
   //  LL_DMA_EnableIT_TE(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX);
   //  LL_DMA_EnableIT_TC(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX);
   //  LL_DMA_EnableIT_TE(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX);

   //  LL_DMA_SetDataLength    (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX, txlen); //устанавливаем сколько символов передачть
   //  LL_DMA_SetDataLength    (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX, rxlen); //устанавливаем сколько символов передачть
   //  LL_SPI_EnableDMAReq_RX(SPI_MLINE_SPI);
   //  LL_SPI_EnableDMAReq_TX(SPI_MLINE_SPI);
   //  LL_DMA_EnableStream     (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX); //enable receive
   //  LL_DMA_EnableStream     (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX); //enable transmit 
   //  LL_SPI_Enable(SPI_MLINE_SPI);
}
void mlimpl_disableBoard(void)
{
   //  LL_DMA_DisableStream(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX); //enable receive
   //  LL_DMA_DisableStream(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX); //enable transmit 
   //  LL_SPI_Disable(SPI_MLINE_SPI);

}
void mlimpl_receiveBoard(spi_mline_dev_t * receiver)
{
   //  mlimpl_resetBoardRx(receiver);
   int len = Server.recv(Server.RxBuffer, *(Server.RxBuffLen));
   *(Server.RxBuffLen) = len;
   receiver->isready = 1;
}
void mlimpl_resetBoardRx(spi_mline_dev_t * receiver)
{
   //  LL_DMA_DisableStream(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX); //enable receive
   //  LL_DMA_SetDataLength    (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX, receiver->dmabufferlen); //устанавливаем сколько символов передачть
   //  LL_DMA_EnableStream     (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX); //enable receive
   mlimpl_receiveBoard(receiver);
}
void mlimpl_resetBoardRxTx(spi_mline_dev_t * receiver, spi_mline_dev_t * transmit)
{
   //  LL_DMA_DisableStream(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX); //enable receive
   //  LL_DMA_DisableStream(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX); //enable transmit 


   //  LL_DMA_SetDataLength(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX, transmit->dmabufferlen);
   //  LL_DMA_SetDataLength    (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX, receiver->dmabufferlen); //устанавливаем сколько символов передачть

   //  LL_DMA_EnableStream     (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX); //enable receive
   //  LL_DMA_EnableStream     (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX); //enable transmit 
   Server.send(Server.RxBuffer, *(Server.RxBuffLen));
   int len = Server.recv(Server.RxBuffer, *(Server.RxBuffLen));
   //*(Server.RxBuffLen) = len;
   //*(Server.TxBuffLen) = 0;
   receiver->isready = 1;
   transmit->isready = 1;
}

void mlimpl_receiveTransmitBoard(spi_mline_dev_t * receiver, spi_mline_dev_t * transmit)
{
   //  LL_DMA_DisableStream(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX); //enable receive
   //  LL_DMA_DisableStream(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX); //enable transmit 

    // if(receiver->collect_on)
    //     receiver->collect(receiver->dmabufferdata, receiver->dmabufferlen);
   receiver->isready = 1;
	receiver->processData(receiver->dmabufferlen);
	transmit->processData(transmit->dmabufferlen);
   Server.send(Server.TxBuffer, *(Server.TxBuffLen));
   int len = Server.recv(Server.RxBuffer, *(Server.RxBuffLen));
   //*(Server.RxBuffLen) = len;
   //*(Server.TxBuffLen) = 0;
   transmit->isready = 1;
      //  LL_DMA_SetDataLength(SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX, transmit->dmabufferlen);
   //  LL_DMA_SetDataLength    (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX, receiver->dmabufferlen); //устанавливаем сколько символов передачть

   //  LL_DMA_EnableStream     (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_RX); //enable receive
   //  LL_DMA_EnableStream     (SPI_MLINE_DMA, SPI_MLINE_DMA_STREAM_TX); //enable transmit 

}