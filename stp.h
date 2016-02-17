/*
 * stp.h
 *
 * Created: 17/01/2016 12:23:05
 *  Author: Adolfina
 */ 


#ifndef SETUP_H_
#define SETUP_H_

#define EEPROM_PAGE 0

enum MODULATION {
	BPSK_20,
	BPSK_40 = 0b00000100,
	OQPSK_SIN_RC_100 = 0b00001000,
	OQPSK_SIN_250 = 0b00001100,
	OQPSK_RC_250 = 0b00011100
};

enum INTERFACE {
	isUSB,
	isUART,
	isSPI,
	isI2C	
};

typedef struct{
	uint16_t address;
	uint16_t panid;
	uint8_t band;
	uint8_t channel;
	uint8_t modulation;
	uint8_t txpower;
	char securityKey[17];
	uint8_t interface;
}settings_t;

settings_t settings;

void STP_Init(void);
void STP_TaskHandler(void);

void writeValue(settings_t *values);
void check(int arg_cnt, char **args);
void setAddress(int arg_cnt, char **args);
void setPanid(int arg_cnt, char **args);
void setBand(int arg_cnt, char **args);
void setChannel(int arg_cnt, char **args);
void setModulation(int arg_cnt, char **args);
void setTxPower(int arg_cnt, char **args);
void setSecurity(int arg_cnt, char **args);
void setInterface(int arg_cnt, char **args);
void setEEPROM(int arg_cnt, char **args);

#endif /* SETUP_H_ */