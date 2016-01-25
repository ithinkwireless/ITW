/*
 * UART.c
 *
 * Created: 24/01/2016 15:03:13
 *  Author: Adolfina
 */ 

#include <asf.h>
#include "uart.h"

static usart_rs232_options_t usart_options = {
	.baudrate = USART_SERIAL_BAUDRATE,
	.charlength = USART_SERIAL_CHAR_LENGTH,
	.paritytype = USART_SERIAL_PARITY,
	.stopbits = USART_SERIAL_STOP_BIT
};

void uart_start(void)
{
	usart_init_rs232(USART_SERIAL, &usart_options);
}

bool uart_is_rx_ready(void)
{
	return usart_rx_is_complete(USART_SERIAL);
}

int uart_putc(int value)
{
	return usart_putchar(USART_SERIAL, value);
}

int uart_getc(void)
{
	return usart_getchar(USART_SERIAL);
}