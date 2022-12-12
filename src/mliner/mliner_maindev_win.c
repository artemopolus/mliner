#include "mliner/mliner_maindev.h"
#include "mliner/mliner_maindev_impl.h"


uint8_t ReceiveBuffer[MLINE_RXTX_BUFFER_SIZE];
uint8_t TransmitBuffer[MLINE_RXTX_BUFFER_SIZE];


spi_mline_dev_t TransmitMlDevDev;
spi_mline_dev_t ReceiveMlDevDev;

static uint8_t MlDevIsEnabled = 0;

// static irq_return_t TransmitIRQhandler(unsigned int irq_nr, void *data)
// {
//     if(mlimpl_runBoardIRQhandlerTX())
//     {
//         TransmitMlDevDev.isready = 1;
//         ex_disableGpio(EX_GPIO_SPI_MLINE); //теперь можно обновлять tx на аполлоне
//     }
//     return IRQ_HANDLED;
// }
// STATIC_IRQ_ATTACH(SPI_MLINE_DMA_IRQ_TX, TransmitIRQhandler, NULL);
// static irq_return_t ReceiveIRQhandler(unsigned int irq_nr, void *data)
// {
// 	if(mlimpl_runBoardIRQhandlerRX())
// 	{
// 		ReceiveMlDevDev.isready = 1;
// 	} 
//    return IRQ_HANDLED;
// }
// STATIC_IRQ_ATTACH(SPI_MLINE_DMA_IRQ_RX, ReceiveIRQhandler, NULL);

void mldev_enable()
{
   //  irq_attach(SPI_MLINE_DMA_IRQ_TX, TransmitIRQhandler, 0, NULL, "TransmitIRQhandler");
   //  irq_attach(SPI_MLINE_DMA_IRQ_RX, ReceiveIRQhandler, 0, NULL, "ReceiveIRQhandler");
    mlimpl_enableBoard(MLINE_RXTX_BUFFER_SIZE, MLINE_RXTX_BUFFER_SIZE);
	MlDevIsEnabled = 1;
}
void mldev_disable()
{
	// irq_detach(SPI_MLINE_DMA_IRQ_TX,NULL);
	// irq_detach(SPI_MLINE_DMA_IRQ_RX,NULL);
	mlimpl_disableBoard();
	MlDevIsEnabled = 0;
}
int downloadMlinerDevData(uint32_t len)
{
    if(!ReceiveMlDevDev.isready)
        return 0;
   //  pshsftPack_exbu8(ReceiveMlDevDev.storage, ReceiveMlDevDev.dmabufferdata, ReceiveMlDevDev.dmabufferlen);

	if(ReceiveMlDevDev.collect_on)
		ReceiveMlDevDev.collect(ReceiveMlDevDev.dmabufferdata, len);

	for (int i = 0; i < ReceiveMlDevDev.dmabufferlen; i++)
	{
		ReceiveMlDevDev.dmabufferdata[i] = 0;
	}
	
	return 1;
}
int uploadMlinerDevData(uint32_t len)
{
	// for(uint32_t i = 0; i < TransmitMlDevDev.dmabufferlen; i++)
	// {
	// 	if(!grbfst_exbu8(TransmitMlDevDev.storage, &TransmitMlDevDev.dmabufferdata[i]))
	// 		break;
	// }

	if(TransmitMlDevDev.collect_on)
	{
		if (TransmitMlDevDev.collect(TransmitMlDevDev.dmabufferdata, len))
		{
			TransmitMlDevDev.dmabufferlen = MLINE_RXTX_BUFFER_SIZE;
		}
	}

   // ex_enableGpio(EX_GPIO_SPI_MLINE); //block update apollon tx

	return 0;
}
// EMBOX_UNIT_INIT(initMlinerDev);
static int initMlinerDev(void)
{
	mlimpl_initBoard();
	TransmitMlDevDev.dmabufferdata = TransmitBuffer;
	TransmitMlDevDev.dmabufferlen = MLINE_RXTX_BUFFER_SIZE;
	TransmitMlDevDev.processData = uploadMlinerDevData;
	// TransmitMlDevDev.storage = &TransmitStore;
	TransmitMlDevDev.isfull = 0;
	TransmitMlDevDev.isready = 1;

	ReceiveMlDevDev.dmabufferdata = ReceiveBuffer;
	ReceiveMlDevDev.dmabufferlen = MLINE_RXTX_BUFFER_SIZE;
	ReceiveMlDevDev.processData = downloadMlinerDevData;
	// ReceiveMlDevDev.storage = &ReceiveStore;
	ReceiveMlDevDev.isfull = 0;
	ReceiveMlDevDev.isready = 1;

	mlimpl_setBoardBuffer(&TransmitMlDevDev, &ReceiveMlDevDev);

	MlDevIsEnabled = 0;
	return 0;
}
void mldev_setTxBuff(ExactoBufferUint8Type * buffer)
{
	TransmitMlDevDev.storage = buffer;
}
void mldev_setRxBuff(ExactoBufferUint8Type * buffer)
{
	ReceiveMlDevDev.storage = buffer;
}
void mldev_receive()
{
	if(MlDevIsEnabled == 0)
	{
		initMlinerDev();
		mldev_enable();
	}
	mlimpl_receiveBoard(&ReceiveMlDevDev);
}
uint8_t mldev_repeatTransmit()
{
    TransmitMlDevDev.isready = 0;
    ReceiveMlDevDev.isready = 0;	
	mlimpl_resetBoardRxTx(&ReceiveMlDevDev, &TransmitMlDevDev);
	return 1;
}
uint8_t mldev_transmit()
{
	if(MlDevIsEnabled == 0)
	{
		initMlinerDev();
		mldev_enable();
	}

    if(TransmitMlDevDev.isready )
    {
        TransmitMlDevDev.isready = 0;
        ReceiveMlDevDev.isready = 0;	
        mlimpl_receiveTransmitBoard(&ReceiveMlDevDev, &TransmitMlDevDev);
        return 1;
    }
	return 0;
}
uint16_t mldev_getReceivedData(uint8_t * trg, uint16_t trglen)
{
	if(downloadMlinerDevData(ReceiveMlDevDev.dmabufferlen))
		return grball_exbu8(ReceiveMlDevDev.storage, trg);
	return 0;
}
uint16_t mldev_setTransmitData(uint8_t * src, uint16_t srclen)
{
	int i;
	for(i = 0; i < srclen; i++)
	{
		if(!pshsft_exbu8(TransmitMlDevDev.storage, src[i]))
			break;
	}
	return i;
}
void mldev_setReceiver(int(*receiveProcess)(uint8_t * data, uint16_t datalen))
{
	ReceiveMlDevDev.collect = receiveProcess;
	ReceiveMlDevDev.collect_on = 1;
}
void mldev_setTransmit(int(*transmitProcess)(uint8_t * data, uint16_t datalen))
{
	TransmitMlDevDev.collect = transmitProcess;
	TransmitMlDevDev.collect_on = 1;
}
uint8_t mldev_getTransmitResult()
{
	return TransmitMlDevDev.isready;
}
uint8_t mldev_getReceiveResult()
{
	return ReceiveMlDevDev.isready;
}

