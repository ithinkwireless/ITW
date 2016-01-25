/*
 * menu.h
 *
 * Created: 29/11/2015 13:19:59
 *  Author: Adolfina
 */ 


#ifndef MENU_H_
#define MENU_H_

PROGMEM_DECLARE(char const, welcome[]) =
"Welcome to ithinkwireless setup interface!\n";

PROGMEM_DECLARE(char const, commandOK[]) =
"COMMAND -> OK\n";

PROGMEM_DECLARE(char const, commandERROR[]) =
"COMMAND -> *** ERROR ***\n";

PROGMEM_DECLARE(char const, settingsValue[]) =
"\n"
"INTERFACE: %s\n"
"ADDRESS: %u\n"
"PANID: %u\n"
"BAND: %d\n"
"CHANNEL: %d\n"
"MODULATION: %s\n"
"TXPOWER: 0x%.2X\n"
"SECURITY KEY: %s\n";

PROGMEM_DECLARE(char const, addressHELP[]) =
"\n"
"*** ADDRESS HELP ***\n"
"\n"
"Syntax: address <value>\n"
"\n"
"For routing nodes <value> starts from 0 to 32767.\n"
"For non-routing nodes <value> starts from 32768 to 65535.\n"
"\n"
"Please refer to AVR2130: Lightweight Mesh Developer Guide 2.2 Network Topology and 5.2.1 Network Address.\n";

PROGMEM_DECLARE(char const, panidHELP[]) =
"\n"
"*** PANID HELP ***\n"
"\n"
"Syntax: panid <value>\n"
"\n"
"PanID <value> starts from 0 to 65535\n"
"\n"
"Please refer to AVR2130: Lightweight Mesh Developer Guide 2.1 Features and 5.2.2 Network Identifier.\n";

PROGMEM_DECLARE(char const, bandHELP[]) =
"\n"
"*** BAND HELP ***\n"
"\n"
"Syntax: band <value>\n"
"\n"
"BAND <value> starts from 0 to 6\n"
"\n"
"Please refer to AT86RF212B datasheet, Table 9-34 Frequency band and numbers and\n"
"AVR2130: Lightweight Mesh Developer Guide 5.2.4 Frequency Band.\n";

PROGMEM_DECLARE(char const, channelHELP[]) =
"\n"
"*** CHANNEL HELP ***\n"
"\n"
"Syntax: channel <value>\n"
"\n"
"Please refer to AT86RF212B datasheet, Table 9-34 Frequency band and numbers, Table 9-35 Channel and\n"
"AVR2130: Lightweight Mesh Developer Guide 5.2.3 Frequency Channel.\n"
"\n"
"Call command 'band' before command 'channel' in order to have a check on channel value inserted.\n";

PROGMEM_DECLARE(char const, modulationHELP[]) =
"\n"
"*** MODULATION HELP ***\n"
"\n"
"Syntax: modulation <value>\n"
"\n"
"Possible <value> for Modulation are:\n"
"\n"
"bpsk-20\n"
"bpsk-40\n"
"oqpsk-sin-rc-100\n"
"oqpsk-sin-250\n"
"oqpsk-rc-250\n"
"\n"
"Please refer to AT86RF212B datasheet, Table 9-8 Register 0x0C (TRX_CTRL_2) Bit Alignment and\n"
"AVR2130: Lightweight Mesh Developer Guide 5.2.5 Modulation Mode.\n";

PROGMEM_DECLARE(char const, txpowerHELP[]) =
"\n"
"*** TXPOWER HELP ***\n"
"\n"
"Syntax: txpower <value>\n"
"\n"
"Please refer to AT86RF212B datasheet, Table 9-15. Recommended Mapping of TX Power, Frequency Band, and PHY_TX_PWR (register 0x05).\n";

PROGMEM_DECLARE(char const, securityHELP[]) =
"\n"
"*** SECURITY KEY HELP ***\n"
"\n"
"Syntax: security <value>\n"
"\n"
"The size of the security key is 16 bytes.\n"
"Please refer to AVR2130: Lightweight Mesh Developer Guide 4.5 Security and 5.2.8 Security Key.\n";

PROGMEM_DECLARE(char const, interfaceHELP[]) =
"\n"
"*** INTERFACE HELP ***\n"
"\n"
"Syntax: interface <value>\n"
"\n"
"Possible <value> for Interface are:\n"
"\n"
"usb\n"
"serial\n";

#endif /* MENU_H_ */