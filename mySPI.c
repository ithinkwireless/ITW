#include <asf.h>
#include "mySPI.h"

void mySPI_start(void)
{
	SPIC.CTRL = PIN5_bm; // MSB First, MODE 0.
	spi_enable(&SPIC); // Slave enable.
}

bool mySPI_is_rx_ready(void)
{
	return spi_is_tx_ok(&SPIC);
}

int mySPI_putc(int val)
{
	spi_put(&SPIC, val);
	return 0;
}

int mySPI_getc(void)
{
	return spi_get(&SPIC);
}