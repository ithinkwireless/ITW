
#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "stp.h"
#include "sys.h"
#include "phy.h"
#include "nwk.h"
#include "uart.h"
#include "mySPI.h"
#include "ITW.h"
#include "nvm.h"

bool newCommand = false;

static void appDataConf(NWK_DataReq_t *req)
{
	appDataReqBusy = false;
	(void)req;
}

static bool appDataInd(NWK_DataInd_t *ind)
{
	for (uint8_t i = 0; i < ind->size; i++)
	{
		byteWrite(ind->data[i]);
	}
	return true;
}

static bool SPIappDataInd(NWK_DataInd_t *ind)
{
	uint8_t i = 0;
	
	gpio_set_pin_high(GPIO_DATAPIN);
	
	while (!byteAvailable()) { }
		
	while (byteAvailable())
	{
		byteWrite(ind->data[i]);
		i++;
	}
	
	gpio_set_pin_low(GPIO_DATAPIN);
	
	return true;
}

void APP_Init(void)
{
	settings_t EEPROM;
	
	for (uint8_t i = 0; i < sizeof(settings_t); i++)
	{
		*((uint8_t*)&EEPROM + i) = nvm_eeprom_read_byte(EEPROM_PAGE * EEPROM_PAGE_SIZE + i);
	}
	
	NWK_SetAddr(EEPROM.address);
	NWK_SetPanId(EEPROM.panid);
	PHY_SetBand(EEPROM.band);
	PHY_SetChannel(EEPROM.channel);
	PHY_SetModulation(EEPROM.modulation);
	PHY_SetTxPower(EEPROM.txpower);
	PHY_SetRxState(true);
	NWK_SetSecurityKey((uint8_t *)EEPROM.securityKey);
	
	switch(EEPROM.interface)
	{
		case isUART:
		byteAvailable = uart_is_rx_ready;
		byteWrite = uart_putc;
		byteRead = uart_getc;
		
		NWK_OpenEndpoint(APP_ENDPOINT, appDataInd);
		uart_start();
		break;
		
		case isUSB:
		byteAvailable = udi_cdc_is_rx_ready;
		byteWrite = udi_cdc_putc;
		byteRead = udi_cdc_getc;
		
		NWK_OpenEndpoint(APP_ENDPOINT, appDataInd);
		udc_start();
		break;
		
		case isSPI:
		byteAvailable = mySPI_is_rx_ready;
		byteWrite = mySPI_putc;
		byteRead = mySPI_getc;
		
		NWK_OpenEndpoint(APP_ENDPOINT, SPIappDataInd);
		mySPI_start();
		break;
		
		default:
		udc_start();
		break;
	}
}

static bool checkPacket(void)
{
	// Inizializza la variabile che conterrà la somma di tutti i bytes ricevuti.
	uint8_t check_sum = 0;

	// Esegue la somma dei primi (PACKET_LENGHT - 1) bytes.
	for (uint8_t i = 0; i < (appBuffer[PACKET_LENGHT_BYTE]-1); i++)
		{
			check_sum += appBuffer[i];
		}
	
	// Esegue il controllo fra la somma calcolata e il byte di checksum
	// ricevuto col pacchetto.
	if (check_sum == appBuffer[appBuffer[PACKET_LENGHT_BYTE]-1])
		{
			return 1; // Se corrisponde.
		}
	else
		{
			return 0; // Se non corrisponde.
		}
}

static void APP_TaskHandler(void)
{
	while (byteAvailable())
	{
	  appBuffer[appBufferPtr] = byteRead();
	  
	  if (appBufferPtr == (appBuffer[PACKET_LENGHT_BYTE] - 1))
	  {
		  newCommand = true;
	  }
	  else
	  {
		  appBufferPtr++;
	  }
	}
	
	if (checkPacket() & newCommand)
	{
		switch(appBuffer[MESSAGE_ID_BYTE])
		{
			case SEND_DATA_MESSAGEID:
			{
				uint16_t addr;
				
				memcpy(&addr, &appBuffer[ADDRESS_MSB_BYTE], 2);
				
				if (appDataReqBusy == true)
				return;

				appDataReq.dstAddr = addr;
				appDataReq.dstEndpoint = APP_ENDPOINT;
				appDataReq.srcEndpoint = APP_ENDPOINT;
				appDataReq.options = NWK_OPT_ENABLE_SECURITY;
				appDataReq.data = appBuffer;
				appDataReq.size = (appBuffer[PACKET_LENGHT_BYTE]);
				appDataReq.confirm = appDataConf;
				NWK_DataReq(&appDataReq);

				appDataReqBusy = true;
			}
			break;
		}

		appBufferPtr = 0;
		newCommand = false;
	}
}

int main (void)
{
	sysclk_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	board_init();
	
	SYS_Init();
	APP_Init();

	while (1)
		{
			if (gpio_pin_is_low(GPIO_PUSH_BUTTON_1))
				{
					STP_Init();
						
					while(1)
						{
							STP_TaskHandler();
						}
				}
					
			SYS_TaskHandler();
			APP_TaskHandler();
		}
}
