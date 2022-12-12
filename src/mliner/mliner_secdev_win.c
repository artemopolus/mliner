#include "mliner/mliner_secdev.h"
#include "mliner/mliner_secdev_impl.h"

uint8_t ReceiveBuffer[MLINE_RXTX_BUFFER_SIZE];
uint8_t TransmitBuffer[MLINE_RXTX_BUFFER_SIZE];


spi_mline_dev_t TransmitMlDevDev;
spi_mline_dev_t ReceiveMlDevDev;

static uint8_t MlDevIsEnabled = 0;

// static irq_return_t TransmitIRQhandler(unsigned int irq_nr, void *data)
// {
// 	if(mlimpl_runBoardIRQhandlerTX())
// 	{
// 		TransmitMlDevDev.isready = 1;
// 	// 	    SPI2_FULL_DMA_tx_buffer.is_full = 0;
//    //  ExDtStr_Output_Storage[EX_THR_SPi_TX].isready = 1;
// 	}
//    return IRQ_HANDLED;
// }
// STATIC_IRQ_ATTACH(15, TransmitIRQhandler, NULL);
// static irq_return_t ReceiveIRQhandler(unsigned int irq_nr, void *data)
// {
// 	if(mlimpl_runBoardIRQhandlerRX())
// 	{
// 		ReceiveMlDevDev.isready = 1;
// 	// 	    SPI2_FULL_DMA_rx_buffer.is_full = 1;
//    //  ExDtStr_Output_Storage[EX_THR_SPi_RX].isready = 1;
//    //  ExDtStr_Output_Storage[EX_THR_SPi_RX].result = EX_THR_CTRL_WAIT;
//    //  ex_updateCounter_ExDtStr(EX_THR_SPi_RX);
// 	} 
//    return IRQ_HANDLED;
// }
// STATIC_IRQ_ATTACH(14, ReceiveIRQhandler, NULL);

void mldev_enable()
{
   // irq_attach(15, TransmitIRQhandler, 0, NULL, "TransmitIRQhandler");
   // irq_attach(14, ReceiveIRQhandler, 0, NULL, "ReceiveIRQhandler");
	mlimpl_enableBoard(MLINE_RXTX_BUFFER_SIZE, MLINE_RXTX_BUFFER_SIZE);
	MlDevIsEnabled = 1;
}
void mldev_disable()
{
	// irq_detach(15,NULL);
	// irq_detach(14,NULL);
	mlimpl_disableBoard();
	MlDevIsEnabled = 0;
}
int downloadDevSecData(uint32_t len)
{
	if(!ReceiveMlDevDev.isready)
		return 0;
   pshsftPack_exbu8(ReceiveMlDevDev.storage, ReceiveMlDevDev.dmabufferdata, ReceiveMlDevDev.dmabufferlen);
	
   for (int i = 0; i < ReceiveMlDevDev.dmabufferlen; i++)
   {
      ReceiveMlDevDev.dmabufferdata[i] = 0;
   }
   
   
   // for(uint32_t i = 0; i < ReceiveMlDevDev.dmabufferlen; i++)
	// {
	// 	if(!pshsft_exbu8(ReceiveMlDevDev.storage, ReceiveMlDevDev.dmabufferdata[i]))
	// 	{
	// 		break;
	// 	}
	// }
	return 1;
}
int uploadDevSecData(uint32_t len)
{
	for(uint32_t i = 0; i < TransmitMlDevDev.dmabufferlen; i++)
	{
		if(!grbfst_exbu8(TransmitMlDevDev.storage, &TransmitMlDevDev.dmabufferdata[i]))
			break;
	}
	return 0;
}
// EMBOX_UNIT_INIT(initDevSec);
static int initDevSec(void)
{
	mlimpl_initBoard();
	TransmitMlDevDev.dmabufferdata = TransmitBuffer;
	TransmitMlDevDev.dmabufferlen = MLINE_RXTX_BUFFER_SIZE;
	TransmitMlDevDev.processData = uploadDevSecData;
	// TransmitMlDevDev.storage = &TransmitStore;
	TransmitMlDevDev.isfull = 0;
	TransmitMlDevDev.isready = 1;

	ReceiveMlDevDev.dmabufferdata = ReceiveBuffer;
	ReceiveMlDevDev.dmabufferlen = MLINE_RXTX_BUFFER_SIZE;
	ReceiveMlDevDev.processData = downloadDevSecData;
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
		initDevSec();
		mldev_enable();
	}
		// if(!ex_checkGpio(EX_GPIO_SPI_MLINE))
	mlimpl_receiveBoard(&ReceiveMlDevDev);
}
uint8_t mldev_repeatTransmit()
{
	// if(!ex_checkGpio(EX_GPIO_SPI_MLINE))
	// {
	 	mlimpl_resetBoardRxTx(&ReceiveMlDevDev, &TransmitMlDevDev);
	 	return 1;
	// }
	//return 0;
}
uint8_t mldev_transmit()
{
	// if(!ReceiveMlDevDev.isready)
	// {
	// 	mlimpl_resetBoardRx(&ReceiveMlDevDev);	
	// 	return 0;
	// }
	// if(!ex_checkGpio(EX_GPIO_SPI_MLINE))
	// {
		if(TransmitMlDevDev.isready )
		{
			TransmitMlDevDev.isready = 0;
			ReceiveMlDevDev.isready = 0;	
			mlimpl_receiveTransmitBoard(&ReceiveMlDevDev, &TransmitMlDevDev);
			return 1;
		}
		else
		{
			mlimpl_receiveBoard(&ReceiveMlDevDev);
		}
	// }
	return 0;
}
uint16_t mldev_getReceivedData(uint8_t * trg, uint16_t trglen)
{
	if(downloadDevSecData(ReceiveMlDevDev.dmabufferlen))
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
uint8_t mldev_getTransmitResult()
{
	return TransmitMlDevDev.isready;
}
uint8_t mldev_getReceiveResult()
{
	return ReceiveMlDevDev.isready;
}

