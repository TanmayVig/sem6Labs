// support for sending/receiving data to/from the host (Flash Magic)
// $Id: host.h 3893 2015-05-12 11:46:44Z andy $

#ifndef _HOSTH_
#define _HOSTH_

#include <stdint.h>

// host API
extern void Host_Initialize(uint32_t baudrate);
extern void Host_SendData(uint8_t *data, int length);
extern int Host_ReceiveData(uint8_t *buffer, int maxlength);
extern void Host_Debug(char *Message);

#endif // _HOSTH_
