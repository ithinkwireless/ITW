/*
 * setup.c
 *
 * Created: 17/01/2016 12:22:53
 *  Author: Adolfina
 */ 

#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "led.h"
#include "stp.h"
#include "cmd.h"
#include "menu.h"
#include "nvm.h"

static bool cdc_opened = false;
static bool autorize_cdc_transfert = false;

void cdc_set_dtr(uint8_t port, bool b_enable)
{
	cdc_opened = b_enable;
}

bool cdc_enable(void)
{
	autorize_cdc_transfert = true;
	return true;
}
void cdc_disable(void)
{
	autorize_cdc_transfert = false;
}

void STP_Init(void)
{
	chb_cmd_init();
	
	if (!autorize_cdc_transfert)
	{
		udc_start();
	}
	
	chb_cmd_add("check", check);
	chb_cmd_add("address", setAddress);
	chb_cmd_add("panid", setPanid);
	chb_cmd_add("band", setBand);
	chb_cmd_add("channel", setChannel);
	chb_cmd_add("modulation", setModulation);
	chb_cmd_add("txpower", setTxPower);
	chb_cmd_add("security", setSecurity);
	chb_cmd_add("interface", setInterface);
	chb_cmd_add("write", setEEPROM);
	
	LED_On(LED0);
}

void STP_TaskHandler(void)
{
	if (cdc_opened)
	{
		chb_cmd_putmsg_PROGMEM(welcome);
		chb_cmd_display();
		cdc_opened = false;
		
		for (uint8_t i = 0; i < sizeof(settings_t); i++)
		{
			*((uint8_t*)&settings + i) = nvm_eeprom_read_byte(EEPROM_PAGE * EEPROM_PAGE_SIZE + i);
		}
	}
	chb_cmd_poll();
}

void writeValue(settings_t *values)
{
	char interfaceValue[7];
	
	if (values->isUart)
	{
		strcpy(interfaceValue, "SERIAL");
		interfaceValue[6] = '\0';
	}
	else
	{
		strcpy(interfaceValue, "USB");
		interfaceValue[3] = '\0';
	}
	
	char modValue[17];
	
	switch (values->modulation)
	{
		case BPSK_20:
		strcpy(modValue, "BPSK-20");
		modValue[strlen(modValue)] = '\0';
		break;
		
		case BPSK_40:
		strcpy(modValue, "BPSK-40");
		modValue[strlen(modValue)] = '\0';
		break;
		
		case OQPSK_SIN_RC_100:
		strcpy(modValue, "OQPSK-SIN-RC-100");
		modValue[strlen(modValue)] = '\0';
		break;
		
		case OQPSK_SIN_250:
		strcpy(modValue, "OQPSK-SIN-250");
		modValue[strlen(modValue)] = '\0';
		break;
		
		case OQPSK_RC_250:
		strcpy(modValue, "OQPSK-RC-250");
		modValue[strlen(modValue)] = '\0';
		break;
	}
	
	char buf[150];
	sprintf_P(buf, settingsValue,	interfaceValue,
									values->address,
									values->panid,
									values->band,
									values->channel,
									modValue,
									values->txpower,
									values->securityKey);
	udi_cdc_write_buf(buf, strlen(buf));
}

void check(int arg_cnt, char **args)
{
	writeValue(&settings);
}

void setAddress(int arg_cnt, char **args)
{
	if (arg_cnt == 1)
	{
		chb_cmd_putmsg_PROGMEM(addressHELP);
	}
	else
	{
		long addr = atol(args[1]);
		
		if (addr >=0 && addr <= 65535)
		{
			settings.address = addr;
			chb_cmd_putmsg_PROGMEM(commandOK);
		} else
		{
			chb_cmd_putmsg_PROGMEM(commandERROR);
		}
	}
}

void setPanid(int arg_cnt, char **args)
{
	if (arg_cnt == 1)
	{
		chb_cmd_putmsg_PROGMEM(panidHELP);
	}
	else
	{
		long panid = atol(args[1]);
		
		if (panid >= 0 && panid <= 65535)
		{
			settings.panid = panid;
			chb_cmd_putmsg_PROGMEM(commandOK);
		} else
		{
			chb_cmd_putmsg_PROGMEM(commandERROR);
		}
	}
}

void setBand(int arg_cnt, char **args)
{
	if (arg_cnt == 1)
	{
		chb_cmd_putmsg_PROGMEM(bandHELP);
	} else
	{
		int band = atoi(args[1]);
		
		if (band >=0 && band <= 6)
		{
			settings.band = band;
			chb_cmd_putmsg_PROGMEM(commandOK);
		} else
		{
			chb_cmd_putmsg_PROGMEM(commandERROR);
		}
	}
}

void setChannel(int arg_cnt, char **args)
{
	if (arg_cnt == 1)
	{
		chb_cmd_putmsg_PROGMEM(channelHELP);
	} else
	{
		int channel = atoi(args[1]);
		
		switch (settings.band)
		{
			case 0 :
			if (channel >= 0 && channel <= 10)
			{
				settings.channel = channel;
				chb_cmd_putmsg_PROGMEM(commandOK);
			} else
			{
				chb_cmd_putmsg_PROGMEM(commandERROR);
			}
			break;
			
			case 1 :
			case 2 :
			case 3 :
			case 6 :
			if (channel >= 0 && channel <= 255)
			{
				settings.channel = channel;
				chb_cmd_putmsg_PROGMEM(commandOK);
			} else
			{
				chb_cmd_putmsg_PROGMEM(commandERROR);
			}
			break;
			
			case 4 :
			if (channel >= 0 && channel <= 94)
			{
				settings.channel = channel;
				chb_cmd_putmsg_PROGMEM(commandOK);
			} else
			{
				chb_cmd_putmsg_PROGMEM(commandERROR);
			}
			break;
			
			case 5 :
			if (channel >= 0 && channel <= 102)
			{
				settings.channel = channel;
				chb_cmd_putmsg_PROGMEM(commandOK);
			} else
			{
				chb_cmd_putmsg_PROGMEM(commandERROR);
			}
			break;
		}
	}
}

void setModulation(int arg_cnt, char **args)
{
	if (arg_cnt == 1)
	{
		chb_cmd_putmsg_PROGMEM(modulationHELP);
	} else if (!strcmp(args[1], "bpsk-20"))
	{
		settings.modulation = BPSK_20;
		chb_cmd_putmsg_PROGMEM(commandOK);
	} else if (!strcmp(args[1], "bpsk-40"))
	{
		settings.modulation = BPSK_40;
		chb_cmd_putmsg_PROGMEM(commandOK);
	} else if (!strcmp(args[1], "oqpsk-sin-rc-100"))
	{
		settings.modulation = OQPSK_SIN_RC_100;
		chb_cmd_putmsg_PROGMEM(commandOK);
	} else if (!strcmp(args[1], "oqpsk-sin-250"))
	{
		settings.modulation = OQPSK_SIN_250;
		chb_cmd_putmsg_PROGMEM(commandOK);
	} else if (!strcmp(args[1], "oqpsk-rc-250"))
	{
		settings.modulation = OQPSK_RC_250;
		chb_cmd_putmsg_PROGMEM(commandOK);
	} else
	{
		chb_cmd_putmsg_PROGMEM(commandERROR);
	}
}

void setTxPower(int arg_cnt, char **args)
{
	if (arg_cnt == 1)
	{
		chb_cmd_putmsg_PROGMEM(txpowerHELP);
	} else
	{
		settings.txpower = (uint8_t)strtol(args[1], NULL, 0);
		chb_cmd_putmsg_PROGMEM(commandOK);
	}
}

void setSecurity(int arg_cnt, char **args)
{
	if (arg_cnt == 1)
	{
		chb_cmd_putmsg_PROGMEM(securityHELP);
	} else
		{
			if (strlen(args[1]) == 16)
				{
					strcpy(settings.securityKey, args[1]);
					settings.securityKey[strlen(settings.securityKey)] = '\0';
					chb_cmd_putmsg_PROGMEM(commandOK);
				}
				else
				{
					chb_cmd_putmsg_PROGMEM(commandERROR);
				}
		
	}
}

void setInterface(int arg_cnt, char **args)
{
	if (arg_cnt == 1)
	{
		chb_cmd_putmsg_PROGMEM(interfaceHELP);
	} else
		{
			if (!strcmp(args[1], "serial"))
				{
					settings.isUart = true;
					chb_cmd_putmsg_PROGMEM(commandOK);
				} else if (!strcmp(args[1], "usb"))
					{
						settings.isUart = false;
						chb_cmd_putmsg_PROGMEM(commandOK);
					} else
						chb_cmd_putmsg_PROGMEM(commandERROR);
	}
}

void setEEPROM(int arg_cnt, char **args)
{
	nvm_eeprom_erase_page(EEPROM_PAGE);
	
	for (uint8_t i = 0; i < sizeof(settings_t); i++)
	{
		nvm_eeprom_write_byte((EEPROM_PAGE * EEPROM_PAGE_SIZE + i) , *((uint8_t*)&settings + i));
	}
	
	for (uint8_t i = 0; i < sizeof(settings_t); i++)
	{
		if (nvm_eeprom_read_byte(EEPROM_PAGE * EEPROM_PAGE_SIZE + i) == *((uint8_t*)&settings + i))
		{
			continue;
		} 
		else
		{
			chb_cmd_putmsg_PROGMEM(commandERROR);
		}
	}
	// Controllare se in caso di messagio commandERROR inviato poi invia anche commandOK
	chb_cmd_putmsg_PROGMEM(commandOK);
}