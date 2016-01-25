/*
 * UART.h
 *
 * Created: 24/01/2016 15:03:25
 *  Author: Adolfina
 */ 


#ifndef UART_H_
#define UART_H_

/*! \name Configuration
 */
//! @{
#define USART_SERIAL                     &USARTE0
#define USART_SERIAL_BAUDRATE            9600
#define USART_SERIAL_CHAR_LENGTH         USART_CHSIZE_8BIT_gc
#define USART_SERIAL_PARITY              USART_PMODE_DISABLED_gc
#define USART_SERIAL_STOP_BIT            false

//! @}

void uart_start(void);
bool uart_is_rx_ready(void);
int uart_putc(int value);
int uart_getc(void);

#endif /* UART_H_ */