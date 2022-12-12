#include "mliner/mliner_maindev.h"
#include "mliner/mliner_maindev_impl.h"


mliner_devs_block_t MlinerDevsBlock = {0};







void mldev_disable()
{
	for(int i = 0; i < MLINER_BASEDEV_DEVSBLOCK_CNT; i++)
	{
		struct mliner_dev * trg = &MlinerDevsBlock.devices[i];
		if(trg->is_enabled && trg->readyRx)
		{
			trg->driver->close(trg);
		}
	}
}



uint16_t mldev_getReceivedData(uint8_t * trgdata, uint16_t trglen)
{
	for(int i = 0; i < MLINER_BASEDEV_DEVSBLOCK_CNT; i++)
	{
		struct mliner_dev * trg = &MlinerDevsBlock.devices[i];
		if(trg->is_enabled && trg->readyRx)
		{
			return trg->driver->read(trg, trgdata, trglen);
		}
	}
	return 0;
}



void mldev_receive()
{
	for(int i = 0; i < MLINER_BASEDEV_DEVSBLOCK_CNT; i++)
	{
		struct mliner_dev * trg = &MlinerDevsBlock.devices[i];
		if(trg->is_enabled)
		{
			if(!trg->is_opened)
				trg->driver->open(trg, &trg->params);
			trg->driver->updateRx(trg);	
		}
	}
}

uint8_t mldev_transmit()
{
	// if(MlDevIsEnabled == 0)
	// {
	// 	initMlinerDev();
	// 	mldev_enable();
	// }
	int res = 0;
	for(int i = 0; i < MLINER_BASEDEV_DEVSBLOCK_CNT; i++)
	{
		struct mliner_dev * trg = &MlinerDevsBlock.devices[i];
		if(trg->is_enabled)
		{
			if(!trg->is_opened)
				trg->driver->open(trg, &trg->params);
			if( trg->driver->updateTxRx(trg) && !res )
				res = 1;	
		}
	}

   //  if(TransmitMlDevDev.isready )
   //  {
   //      TransmitMlDevDev.isready = 0;
   //      ReceiveMlDevDev.isready = 0;	
   //      mlimpl_receiveTransmitBoard(&ReceiveMlDevDev, &TransmitMlDevDev);
   //      return 1;
   //  }
	return res;
}


void mldev_setReceiver(int(*receiveProcess)(uint8_t * data, uint16_t datalen))
{
	for(int i = 0; i < MLINER_BASEDEV_DEVSBLOCK_CNT; i++)
	{
		struct mliner_dev * trg = &MlinerDevsBlock.devices[i];
		//if(trg->is_enabled)
		//{
			trg->receiveDataProcess = receiveProcess;
		//}
	}
}
void mldev_setTransmit(int(*transmitProcess)(uint8_t * data, uint16_t datalen))
{
	for(int i = 0; i < MLINER_BASEDEV_DEVSBLOCK_CNT; i++)
	{
		struct mliner_dev * trg = &MlinerDevsBlock.devices[i];
		//if(trg->is_enabled)
		//{
			trg->transmitDataProcess = transmitProcess;
		//}
	}
}
uint8_t mldev_getTransmitResult()
{
	for(int i = 0; i < MLINER_BASEDEV_DEVSBLOCK_CNT; i++)
	{
		struct mliner_dev * trg = &MlinerDevsBlock.devices[i];
		if(trg->is_enabled && trg->readyTx)
			return 1;
	}
	return 0;
}
uint8_t mldev_getReceiveResult()
{
	for(int i = 0; i < MLINER_BASEDEV_DEVSBLOCK_CNT; i++)
	{
		struct mliner_dev * trg = &MlinerDevsBlock.devices[i];
		if(trg->is_enabled && trg->readyRx)
			return 1;
	}
	return 0;

}

