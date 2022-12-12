#include "mliner/mliner_maindev_impl.h"

#include "zmq/exmliner.h"

#ifndef MLINE_RXTX_BUFFER_SIZE
#define MLINE_RXTX_BUFFER_SIZE 512
#endif

exMlinerZMQ Server;
static uint8_t ReceiveBuffer[MLINE_RXTX_BUFFER_SIZE];
static uint8_t TransmitBuffer[MLINE_RXTX_BUFFER_SIZE];

static uint32_t TxLen;
static uint32_t RxLen;

static struct mliner_dev * MlinerDev;
struct mliner_dev_ops ZmqDev;

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
static int updateRx(struct mliner_dev * dev)
{
   int len = Server.recv(Server.RxBuffer, *(Server.RxBuffLen));
   dev->readyRx = 1;
   dev->receiveDataProcess(Server.RxBuffer, *(Server.RxBuffLen));
   return 1;
}
static int updateTxRx(struct mliner_dev * dev)
{
   dev->readyRx = 1;
   dev->receiveDataProcess(Server.RxBuffer, *(Server.RxBuffLen));
   dev->transmitDataProcess(Server.TxBuffer, *(Server.TxBuffLen));
   Server.send(Server.TxBuffer, *(Server.TxBuffLen));
   int len = Server.recv(Server.RxBuffer, *(Server.RxBuffLen));
   dev->readyTx = 1;
   return len != 0 ? 1 : 0;
}
static int read(struct mliner_dev * dev, uint8_t * data, uint16_t len)
{
   return 0;
}
static int close (struct mliner_dev * dev)
{
   dev->is_opened = 0;
   return 0;
}
static int open(struct mliner_dev * dev, const struct mliner_dev_params * params)
{
   Server.init();
   if(params != &dev->params)
   {
      TxLen = params->len;
      RxLen = params->len;
      dev->params = *params;
   }
   Server.RxBuffLen = &TxLen;
   Server.TxBuffLen = &RxLen;
   Server.TxBuffer = TransmitBuffer;
   Server.RxBuffer = ReceiveBuffer;
   dev->is_opened = 1;
   return 0;
}






int mlimpl_initBoard(void)
{
   for(int i = 0; i < MLINER_BASEDEV_DEVSBLOCK_CNT; i++)
   {
      if(MlinerDevsBlock.devices[i].is_enabled == 0)
      {
         MlinerDev = &MlinerDevsBlock.devices[i];
         MlinerDev->is_enabled = 1;
         MlinerDev->driver = &ZmqDev;
         MlinerDev->driver->open = open;
         MlinerDev->driver->close = close;
         MlinerDev->driver->read = read;
         MlinerDev->driver->updateRx = updateRx;
         MlinerDev->driver->updateTxRx = updateTxRx;
         mliner_dev_params params;
         params.len = MLINE_RXTX_BUFFER_SIZE;
         open(MlinerDev, &params);

         break;
      }
   }
   Server.init();
   return 0;
}

void mlimpl_setBoardBuffer(
	spi_mline_dev_t * transmit, spi_mline_dev_t * receive
	)
{
	Server.RxBuffer = receive->dmabufferdata;
	Server.RxBuffLen = &receive->dmabufferlen;
	Server.TxBuffer = transmit->dmabufferdata;
	Server.TxBuffLen = &transmit->dmabufferlen;
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
}
void mlimpl_disableBoard(void)
{
}
void mlimpl_receiveBoard(spi_mline_dev_t * receiver)
{
   int len = Server.recv(Server.RxBuffer, *(Server.RxBuffLen));
   *(Server.RxBuffLen) = len;
   receiver->isready = 1;
}
void mlimpl_resetBoardRx(spi_mline_dev_t * receiver)
{
   mlimpl_receiveBoard(receiver);
}
void mlimpl_resetBoardRxTx(spi_mline_dev_t * receiver, spi_mline_dev_t * transmit)
{
   Server.send(Server.RxBuffer, *(Server.RxBuffLen));
   int len = Server.recv(Server.RxBuffer, *(Server.RxBuffLen));
   receiver->isready = 1;
   transmit->isready = 1;
}

void mlimpl_receiveTransmitBoard(spi_mline_dev_t * receiver, spi_mline_dev_t * transmit)
{
   receiver->isready = 1;
	receiver->processData(receiver->dmabufferlen);
	transmit->processData(transmit->dmabufferlen);
   Server.send(Server.TxBuffer, *(Server.TxBuffLen));
   int len = Server.recv(Server.RxBuffer, *(Server.RxBuffLen));
   transmit->isready = 1;
}
