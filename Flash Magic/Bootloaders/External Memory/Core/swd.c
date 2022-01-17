// support for sending/receiving data to/from SWD interface
// $Id: swd.c 4843 2017-10-23 21:24:32Z ajayre $

#ifndef _USE_UART

#include "host.h"
#include "descriptor.h"
#include <string.h>

// maximum number of bytes in a mailbox
// must be a multiple of four
#define MAX_DATA 400

// validity flags
#define VALID_NO  0x00
#define VALID_YES 0x01

// packet types
#define PACKETTYPE_DEBUG 0x10

extern const descriptor_t Descriptor;

// description of a mailbox
typedef struct _mailbox
{
  uint32_t Valid;
  uint32_t Length;
  uint8_t Data[MAX_DATA];
} MAILBOX;

// pointers to mailboxes
static MAILBOX *CommandMailboxes;
static MAILBOX *ResponseMailboxes;

static int CommandMailboxIndex  = 0;
static int ResponseMailboxIndex = 0;

// initializes the module for sending and receiving
void Host_Initialize(uint32_t baudrate)
{
  int m;
  
  // set up mailbox locations to point to RAM
  CommandMailboxes  = (MAILBOX *)(Descriptor.ramcmdmailboxaddress);
  ResponseMailboxes = (MAILBOX *)(Descriptor.ramrespmailboxaddress);
  
  // contents of mailboxes are not valid
  for (m = 0; m < Descriptor.numrammailboxes; m++)
  {
    CommandMailboxes[m].Valid = VALID_NO;
    ResponseMailboxes[m].Valid = VALID_NO;
  }
}

// sends data to the host
void Host_SendData(uint8_t *data, int length)
{
  int b;

  // if data still in response mailbox then overwrite
  if (ResponseMailboxes[ResponseMailboxIndex].Valid != VALID_NO)
  {
    ResponseMailboxes[ResponseMailboxIndex].Valid = VALID_NO;
  }
  
  // store data and length and mark as valid
  // the host will poll the valid location detect the data is valid
  // read it and set the mailbox to 'not valid'
  for (b = 0; b < length; b++) ResponseMailboxes[ResponseMailboxIndex].Data[b] = data[b];
  ResponseMailboxes[ResponseMailboxIndex].Length = length;
  ResponseMailboxes[ResponseMailboxIndex].Valid = VALID_YES;

  // move to next mailbox  
  if (++ResponseMailboxIndex == Descriptor.numrammailboxes) ResponseMailboxIndex = 0;
}

// sends a debug message to the host
// message must be null terminated
void Host_Debug(char *Message)
{
  int b;
  
  // if data still in response mailbox then overwrite
  if (ResponseMailboxes[ResponseMailboxIndex].Valid != VALID_NO)
  {
    ResponseMailboxes[ResponseMailboxIndex].Valid = VALID_NO;
  }
  
  // store data and length and mark as valid
  // the host will poll the valid location detect the data is valid
  // read it and set the mailbox to 'not valid'
  for (b = 0; b < strlen(Message); b++) ResponseMailboxes[ResponseMailboxIndex].Data[b] = Message[b];
  ResponseMailboxes[ResponseMailboxIndex].Data[strlen(Message)] = '\0';
  ResponseMailboxes[ResponseMailboxIndex].Length = strlen(Message) + 1;
  ResponseMailboxes[ResponseMailboxIndex].Valid = VALID_YES | PACKETTYPE_DEBUG;

  // move to next mailbox  
  if (++ResponseMailboxIndex == Descriptor.numrammailboxes) ResponseMailboxIndex = 0;
}

// receives data from the host and stores it in a buffer
// returns number of bytes received or zero for none
// should return immediately if there is no data to be read
int Host_ReceiveData(uint8_t *buffer, int maxlength)
{
  int b;
  int length;
  
  // if data is waiting to be read in the mailbox then read it
  if (CommandMailboxes[CommandMailboxIndex].Valid == VALID_YES)
  {
    // get the length of data and truncate to max length
    length = CommandMailboxes[CommandMailboxIndex].Length;
    if (length > maxlength)
    {
      length = maxlength;
    }
    
    // copy data
    for (b = 0; b < length; b++) buffer[b] = CommandMailboxes[CommandMailboxIndex].Data[b];

    // mark mailbox as not valid
    CommandMailboxes[CommandMailboxIndex].Valid = VALID_NO;
    
    // move to next mailbox  
    if (++CommandMailboxIndex == Descriptor.numrammailboxes) CommandMailboxIndex = 0;
    
    return length;
  }
  
  // no data
  return 0;
}

#endif // !_USE_UART
