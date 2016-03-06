/*
 * ITW.h
 *
 * Created: 24/01/2016 13:37:40
 *  Author: Adolfina
 */ 

#ifndef ITW_H_
#define ITW_H_

/*- Definitions ------------------------------------------------------------*/
#ifdef NWK_ENABLE_SECURITY
#define APP_BUFFER_SIZE     (NWK_MAX_PAYLOAD_SIZE - NWK_SECURITY_MIC_SIZE)
#else
#define APP_BUFFER_SIZE     NWK_MAX_PAYLOAD_SIZE
#endif

enum {
PACKET_LENGHT_BYTE,
MESSAGE_ID_BYTE,
ADDRESS_MSB_BYTE,
ADDRESS_LSB_BYTE,
ENDPOINT_BYTE,
PAYLOAD_FIRST_BYTE,
};

enum {
	SEND_DATA_MESSAGEID = 0x01
};

static NWK_DataReq_t appDataReq;
static bool appDataReqBusy = false;
static uint8_t appBuffer[APP_BUFFER_SIZE];
static uint8_t appBufferPtr = 0;

//void (*interfaceStart)(void);
bool (*byteAvailable)(void);
int (*byteWrite)(int value);
int (*byteRead)(void);

void APP_Init(void);
static bool checkPacket(void);
static void APP_TaskHandler(void);

#endif /* ITW_H_ */