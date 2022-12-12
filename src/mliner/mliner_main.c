#include "mliner/mliner_main.h"

#include "operator/exacto_buffer.h"

#include "exactolink/exlnk_setHeader.h"
#include "exactolink/exlnk_getHeader.h"


#include "mliner/mliner_maindev.h"

#include <string.h>


static uint8_t NeedToSend = 0;

static uint8_t IsMain = 0;

static uint8_t TmpBuffer[EXACTO_BUFFER_UINT8_SZ] = {0};

static uint16_t OwnAddress = 0;

#define MLINER_MAIN

#define MLINER_SEC_PACKCNT_MAX 10

#define MLINER_SEC_ADRCNT_MAX 2




typedef struct mliner_pack_buffer
{
	uint16_t address;
	exlnk_set_header_str_t header;
	mliner_cmd_info_t sendpacks[MLINER_SEC_PACKCNT_MAX];
	uint8_t sendpacks_cnt;
	uint32_t counter;
	uint8_t dma[MLINER_SEC_MSG_SIZE];
	ExactoBufferUint8Type indata;
}mliner_pack_buffer_t;

typedef struct mliner_sec_out_dev
{
	ExactoBufferUint8Type store;
	mliner_pack_buffer_t buffer[MLINER_SEC_ADRCNT_MAX];
}mliner_sec_out_dev_t;


typedef struct mliner_sec_in_dev
{
	exlnk_get_header_str_t buffer;
	uint32_t counter;
	uint8_t dma[MLINER_SEC_MSG_SIZE];
	ExactoBufferUint8Type store;
	uint8_t cmdaction_on;
	int(*cmdaction)(exlnk_cmd_str_t * out);
	uint8_t cmdackaction_on;
	int(*cmdackaction)(exlnk_cmdack_str_t * out);
	uint8_t onreset_on;
	int(*onreset)();
	uint8_t repeataction_on;
	int(*repeataction)(uint8_t id, uint32_t mnum);
	uint8_t erroraction_on;
	int(*erroraction)(int error);
}mliner_sec_in_dev_t;


static uint16_t Addresses[] = {7,16};

static mliner_sec_out_dev_t Transmit ={0};
static mliner_sec_in_dev_t Receive = {0};

static mliner_pack_buffer_t * TargetPack = NULL;
void exmliner_initAckPack(mliner_pack_buffer_t * trg)
{
	for(int i = 0; i < trg->sendpacks_cnt; i++)
	{
		trg->sendpacks[i].ack = 1; //on init all sent
	}
}
uint8_t exmliner_addAck(mliner_pack_buffer_t * trg, uint8_t id, uint32_t mnum)
{
	for(int i = 0; i < trg->sendpacks_cnt; i++)
	{
		if(trg->sendpacks[i].ack == 1)
		{
			trg->sendpacks[i].id = id;
			trg->sendpacks[i].mnum = mnum;
			trg->sendpacks[i].ack = 0;
		}
	}
	return 1;
}
uint8_t exmliner_checkAck(exlnk_cmdack_str_t * ack, uint16_t address)
{
	for(int i = 0; i < MLINER_SEC_ADRCNT_MAX; i++)
	{
		if(Transmit.buffer[i].address == address)
		{
			for (int j = 0; j < Transmit.buffer[i].sendpacks_cnt; j++)
			{
				if(Transmit.buffer[i].sendpacks[j].mnum == ack->mnum)
				{
					Transmit.buffer[i].sendpacks[j].ack = 1;
					return 1;
				}
			}
		}
	}
	return 0;
}
int exmliner_ReceiveProcess(uint8_t * data, uint16_t datalen)
{
	memset(&Receive.buffer, 0, sizeof(Receive.buffer));
	for(int k =0; k < 10; k++)
	{
		if(exlnk_getHeader(&data[k], datalen - k, &Receive.buffer))
		{
			for (int i = 0; i < MLINER_SEC_ADRCNT_MAX; i++)
			{
				if(Receive.buffer.adr == Transmit.buffer[i].address)
				{
					pshsftPack_exbu8(&Transmit.buffer[i].indata, &Receive.buffer.data[0], Receive.buffer.datalen);
					memset(data, 0, datalen);
					break;
				}
			}
			Receive.counter = 0;
			return 0;	
		}
	}
	Receive.counter++;
	return 0;
}
int exmliner_TransmitProcess(uint8_t * trg, uint16_t trglen)
{
	if(TargetPack == NULL)
		return 0;
	
	exlnk_closeHeader(&TargetPack->header);

	for(int i = 0; i < trglen && i < TargetPack->header.pt_data; i++)
	{
		trg[i] = TargetPack->header.data[i];
	}
	exlnk_initHeader(&TargetPack->header, TargetPack->dma);
	exlnk_fillHeader(&TargetPack->header, TargetPack->address, EXLNK_MSG_SIMPLE, EXLNK_PACK_SIMPLE, 0, TargetPack->counter++, 0);
	return TargetPack->header.pt_data;
}


void exmliner_Init(uint8_t type, uint16_t address)
{
	IsMain = type;
	OwnAddress = address;
	setini_exbu8(&Receive.store);
	setini_exbu8(&Transmit.store);
	// mldev_setRxBuff(&Receive.store);
	// mldev_setTxBuff(&Transmit.store);
	mldev_setReceiver(exmliner_ReceiveProcess);
	mldev_setTransmit(exmliner_TransmitProcess);
	Receive.counter = 0;
	NeedToSend = 0;

	for (int i = 0; i < MLINER_SEC_ADRCNT_MAX; i++)
	{
		Transmit.buffer[i].counter = 0;
		Transmit.buffer[i].address = Addresses[i];
		setini_exbu8(&Transmit.buffer[i].indata);
		exlnk_initHeader(&Transmit.buffer[i].header, Transmit.buffer[i].dma);
		exlnk_fillHeader(&Transmit.buffer[i].header, Addresses[i], EXLNK_MSG_SIMPLE, EXLNK_PACK_SIMPLE, 0, Transmit.buffer[i].counter, 0);
	}

}

void exmliner_Upload(void * data, size_t len, uint8_t id, uint16_t adr)
{
	mliner_pack_buffer_t * trg = NULL;
	for (int i = 0; i < MLINER_SEC_ADRCNT_MAX; i++)
	{
		if(adr == Transmit.buffer[i].address)
			trg = &Transmit.buffer[i];
	}
	if(trg==NULL)
		return;
	if (id == EXLNK_DATA_ID_CMD)
	{
		exlnk_cmd_str_t * cmd = (exlnk_cmd_str_t*)data;
		// exlnk_CmdToArray(cmd, TmpBuffer, 100);
		exlnk_uploadCmdHeader(&trg->header, cmd);
		exmliner_addAck(trg, cmd->id, cmd->mnum);
	}
	else if (id == EXLNK_DATA_ID_CMDACK)
	{
		exlnk_CmdAckToArray((exlnk_cmdack_str_t*)data, TmpBuffer, 100);
		exlnk_uploadHeader(&trg->header, TmpBuffer, len);
	}
	else
		return;
}
void exmliner_Update(uint16_t adr)
{
	mliner_pack_buffer_t * trg = NULL;
	// if(IsMain)
	if(1)
	{
		for (int i = 0; i < MLINER_SEC_ADRCNT_MAX; i++)
		{
			if(adr == Transmit.buffer[i].address)
				trg = &Transmit.buffer[i];
		}
	}
	else
	{
		trg = &Transmit.buffer[1];
	}
	if (trg == NULL)
		return;

	//process
	uint16_t len = getlen_exbu8(&trg->indata);
	if(len && (IsMain || (!IsMain && trg->address == OwnAddress)))
	{
		NeedToSend = 1;
		while(getlen_exbu8(&trg->indata) > 0)
		{
			uint8_t len_to_copy = 100;
			cpFromPack_exbu8(&trg->indata, TmpBuffer, len_to_copy);
			exlnk_cmd_str_t in;
			exlnk_cmdack_str_t ack;
				
			if(exlnk_getCmd(&in, &TmpBuffer[0], len_to_copy))
			{
				mvMarkPack_exbu8(&trg->indata, sizeof( exlnk_cmd_str_t));
				//process
				if(trg != NULL)
				{
					exlnk_cmdack_str_t ack1;
					exlnk_setCmdAck(&ack1, in.id, in.mnum, in.reg);
					exmliner_Upload(&ack1, sizeof(exlnk_cmdack_str_t), EXLNK_DATA_ID_CMDACK, trg->address);
					if(Receive.cmdaction_on)
						Receive.cmdaction(&in);
				}
			}
			else if(exlnk_getCmdAck(&ack, &TmpBuffer[0], len_to_copy ))
			{
				mvMarkPack_exbu8(&trg->indata, sizeof( exlnk_cmdack_str_t));
				if(trg != NULL)
				{
					for(int i = 0; i < trg->sendpacks_cnt; i++)
					{
						if(trg->sendpacks[i].mnum == ack.mnum)
							trg->sendpacks[i].ack = 1;
					}
					if(Receive.cmdackaction_on)
						Receive.cmdackaction(&ack);
				}
			}
			else
				break;
		}
	}
	//switch

	if(trg != NULL)
	{
		if(IsMain ||(!IsMain && NeedToSend))
		{
			TargetPack = trg;
				//transmit

			if(!mldev_transmit()&& Receive.erroraction_on)
				Receive.erroraction(1);
			trg->counter++;

			TargetPack = NULL;
		}
		else if(!IsMain)
		{
			mldev_receive();
		}
	}
	if(!IsMain && Receive.counter >= 999)
	{
		if(Receive.onreset_on)
			Receive.onreset();
		Receive.counter = 0;
		mldev_disable();
	}

	NeedToSend = 0;
}
void exmliner_setCmdAction(int(*cmdaction)(exlnk_cmd_str_t * out))
{
	Receive.cmdaction = cmdaction;
	Receive.cmdaction_on = 1;
}
void exmliner_setCmdAckAction(int(*cmdackaction)(exlnk_cmdack_str_t * out))
{
	Receive.cmdackaction = cmdackaction;
	Receive.cmdackaction_on = 1;
}
void exmliner_setResetAction(int(*resetaction)())
{
	Receive.onreset = resetaction;
	Receive.onreset_on = 1;
}

void exmliner_setRepeatAction(int(*repeataction)(uint8_t id, uint32_t mnum))
{
	Receive.repeataction = repeataction;
	Receive.repeataction_on = 1;
}
void exmliner_setErrorAction(int(*erroraction)(int id))
{
	Receive.erroraction = erroraction;
	Receive.erroraction_on = 1;
}
uint8_t exmliner_getRxIRQ()
{
	return mldev_getReceiveResult();
}
uint8_t exmliner_getTxIRQ()
{
	return mldev_getTransmitResult();
}
