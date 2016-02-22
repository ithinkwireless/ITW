/*
 * SPI.h
 *
 * Created: 22/02/2016 15:30:05
 *  Author: Adolfina
 */ 


#ifndef SPI_H_
#define SPI_H_

void mySPI_start(void);
bool mySPI_is_rx_ready(void);
int mySPI_putc(int val);
int mySPI_getc(void);

#endif /* SPI_H_ */