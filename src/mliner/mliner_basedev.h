#ifndef MLINER_BASEDEV_H_
#define MLINER_BASEDEV_H_


#include <stdint.h>
#include "operator/exacto_buffer.h"

typedef struct spi_mline_dev
{
	uint8_t isfull;
	uint8_t isready;
	uint32_t 	dmabufferlen;
	uint8_t * 	dmabufferdata;
	ExactoBufferUint8Type * storage;
	int ( *processData )(uint32_t len);
	uint8_t collect_on;
	int ( *collect )(uint8_t * data, uint16_t datalen);
}spi_mline_dev_t;

struct mliner_dev_params
{
	uint32_t len;
};
struct mliner_dev;
struct mliner_dev_ops
{
	int(*open)(struct mliner_dev * dev, const struct mliner_dev_params * params);
	int(*close)(struct mliner_dev * dev);
	int(*read)(struct mliner_dev * dev, uint8_t * data, uint16_t len);

	int(*updateTxRx)(struct mliner_dev * dev);
	int(*updateRx)(struct mliner_dev * dev);

};
struct mliner_dev
{
	struct mliner_dev_ops * driver;
	uint32_t addr;

	uint8_t is_enabled;
	uint8_t is_opened;
	uint8_t readyTx;
	uint8_t readyRx;
	struct mliner_dev_params params;
	int ( *transmitDataProcess )(uint8_t * data, uint16_t datalen);
	int ( *receiveDataProcess )(uint8_t * data, uint16_t datalen);
};

#define MLINER_BASEDEV_DEVSBLOCK_CNT 10
typedef struct mliner_devs_block
{
	struct mliner_dev devices[MLINER_BASEDEV_DEVSBLOCK_CNT];
}mliner_devs_block_t;


#endif //MLINER_BASEDEV_H_
