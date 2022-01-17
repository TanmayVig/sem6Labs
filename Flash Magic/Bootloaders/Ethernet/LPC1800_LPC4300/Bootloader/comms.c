//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#include "sbl_config.h"
#include "board.h"
#include "timer.h"
#include "isp_iap.h"
#include <stdio.h>
#include <string.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define ENET_NUM_TX_DESC 4
#define ENET_NUM_RX_DESC 4

#define EtherHdrLen 14
#define IpHdrLen		20
#define HdrLen      42			/* Ethernet hdr + IP hdr + UDP hdr  */

#define CR			      	0x0D
#define LF				      0x0A

#define HTONS(n) ((((unsigned short)((n) & 0xff)) << 8) | (((n) & 0xff00) >> 8))

static ENET_ENHTXDESC_T TXDescs[ENET_NUM_TX_DESC];
static ENET_ENHRXDESC_T RXDescs[ENET_NUM_RX_DESC];

/* Transmit/receive buffers and indices */
static uint8_t TXBuffer[ENET_NUM_TX_DESC][EMAC_ETH_MAX_FLEN];
static uint8_t RXBuffer[ENET_NUM_RX_DESC][EMAC_ETH_MAX_FLEN];
static int32_t rxFill, rxGet, rxAvail, rxNumDescs;
static int32_t txFill, txGet, txUsed, txNumDescs;

// our mac address
static uint8_t MyMAC[6];
// port numbers being used
static int srcport;
static int destport;
// host mac address
static uint8_t HostMAC[6];
// next transmit index
static int32_t txNextIndex = 0;
// ip addresses being used
static uint8_t srcipaddr[4];
static uint8_t destipaddr[4];
// flag to indicate if handshake completed
static char synchro = false;

static const char sync[] 	=	"Synchronized";
static const char Ok[] 	=   "OK";


/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Local delay function used by the ENET or PHY drivers . This can be
   replaced with something more accurate if needed. */
static void localMsDelay(uint32_t ms)
{
	ms = ms * 50000;
	while (ms > 0) {
		ms--;
	}
}

/* Local index and check function */
static __INLINE int32_t incIndex(int32_t index, int32_t max)
{
	index++;
	if (index >= max) {
		index = 0;
	}

	return index;
}

/* Initialize MAC descriptors for simple packet receive/transmit */
static void InitDescriptors(
	ENET_ENHTXDESC_T *pTXDescs, int32_t numTXDescs,
	ENET_ENHRXDESC_T *pRXDescs, int32_t numRXDescs)
{
	int i;

	/* Setup the descriptor list to a default state */
	memset(pTXDescs, 0, numTXDescs * sizeof(*pTXDescs));
	memset(pTXDescs, 0, numRXDescs * sizeof(*pRXDescs));
	rxFill = rxGet = 0;
	rxAvail = rxNumDescs = numRXDescs;
	txNumDescs = numTXDescs;
	txUsed = txGet = txFill = 0;

	/* Build linked list, CPU is owner of descriptors */
	for (i = 0; i < numTXDescs; i++) {
		pTXDescs[i].CTRLSTAT = 0;
		pTXDescs[i].B2ADD = (uint32_t) &pTXDescs[i + 1];
	}
	pTXDescs[numTXDescs - 1].B2ADD = (uint32_t) &pTXDescs[0];
	for (i = 0; i < numRXDescs; i++) {
		pRXDescs[i].STATUS = 0;
		pRXDescs[i].B2ADD = (uint32_t) &pRXDescs[i + 1];
		pRXDescs[i].CTRL = RDES_ENH_RCH;
	}
	pRXDescs[numRXDescs - 1].B2ADD = (uint32_t) &pRXDescs[0];
	pRXDescs[numRXDescs - 1].CTRL |= RDES_ENH_RER;

	/* Setup list pointers in Ethernet controller */
	Chip_ENET_InitDescriptors(LPC_ETHERNET, pTXDescs, pRXDescs);
}

/* Attach a buffer to a descriptor and queue it for reception */
static void ENET_RXQueue(void *buffer, int32_t bytes)
{
	if (rxAvail > 0) {
		/* Queue the next descriptor and start polling */
		RXDescs[rxFill].B1ADD = (uint32_t) buffer;
		RXDescs[rxFill].CTRL = RDES_ENH_BS1(bytes) | RDES_ENH_RCH;
		if (rxFill == (rxNumDescs - 1)) {
			RXDescs[rxFill].CTRL |= RDES_ENH_RER;
		}
		RXDescs[rxFill].STATUS = RDES_OWN;
		rxAvail--;
		rxFill = incIndex(rxFill, rxNumDescs);

		/* Start polling */
		Chip_ENET_RXStart(LPC_ETHERNET);
	}
}

/* Returns a pointer to a filled ethernet buffer or NULL if none are available */
static void *ENET_RXGet(int32_t *bytes)
{
	void *buffer;

	/* This doesn't check status of the received packet */
	if ((rxAvail < rxNumDescs) && (!(RXDescs[rxGet].STATUS & RDES_OWN))) {
		/* CPU owns descriptor, so a packet was received */
		buffer = (void *) RXDescs[rxGet].B1ADD;
		*bytes = (int32_t) RXDescs[rxGet].STATUS & 0xFFF;
		rxGet = incIndex(rxGet, rxNumDescs);
		rxAvail++;
	}
	else {
		/* Nothing received */
		*bytes = 0;
		buffer = NULL;
	}

	return buffer;
}

/* Returns a pointer to a buffer that has been transmitted */
static void *ENET_TXBuffClaim(void)
{
	void *buffer;

	/* Is packet done sending? */
	if ((txUsed > 0) && (!(TXDescs[txGet].CTRLSTAT & TDES_OWN))) {
		/* CPU owns descriptor, so the packet completed transmit */
		buffer = (void *) TXDescs[txGet].B1ADD;
		txGet = incIndex(txGet, txNumDescs);
		txUsed--;
	}
	else {
		buffer = NULL;
	}

	return buffer;
}

/* Attaches a buffer to a transmit descriptor and queues it for transmit */
static void ENET_TXQueue(void *buffer, int32_t bytes)
{
	if (txUsed < txNumDescs) {
		/* Queue the next descriptor and start polling */
		TXDescs[txFill].B1ADD = (uint32_t) buffer;
		TXDescs[txFill].BSIZE = TDES_ENH_BS1(bytes);
		TXDescs[txFill].CTRLSTAT = TDES_ENH_FS | TDES_ENH_LS | TDES_ENH_TCH;
		if (txFill == (txNumDescs - 1)) {
			TXDescs[txFill].CTRLSTAT |= TDES_ENH_TER;
		}
		TXDescs[txFill].CTRLSTAT |= TDES_OWN;
		txUsed++;
		txFill = incIndex(txFill, txNumDescs);

		/* Start polling */
		Chip_ENET_TXStart(LPC_ETHERNET);
	}
}

int chksum16(void *buf1, short len) {
  unsigned short * buf = buf1;
  int chksum16, chksum=0;

  while(len > 0) {	
    if (len == 1)
      chksum16 = ((*buf)&0x00FF);
    else
      chksum16 = (*buf);
    chksum = chksum + HTONS(chksum16);
    *buf++;
    len -=2;
  }
  return (~(chksum + ((chksum & 0xFFFF0000) >> 16))&0xFFFF);
}

// checks if we should receive an ethernet frame
// returns true to receive frame
bool filter(uint8_t *buf)
{
  uint16_t frame_srcport;
  uint16_t frame_destport;
  int i;
  
  // check mac address matches ours
  if ((buf[0] != MyMAC[0]) || (buf[1] != MyMAC[1]) || (buf[2] != MyMAC[2]) ||
      (buf[3] != MyMAC[3]) || (buf[4] != MyMAC[4]) || (buf[5] != MyMAC[5]))
  {
    return false;
  }
  
  // if already performed handshake then we know ip addresss and port numbers to check
  if (synchro)
  {
    // check port numbers
    frame_srcport  = HTONS(*(uint16_t *)(&buf[34]));
    frame_destport = HTONS(*(uint16_t *)(&buf[36]));
    if (frame_srcport != srcport) return false;
    if (frame_destport != destport) return false;
    
    // check ip addresses
    for (i = 0; i < 4; i++)
    {
      if (buf[26 + i] != srcipaddr[i]) return false;
      if (buf[30 + i] != destipaddr[i]) return false;
    }
  }
  
  return true;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from ethernet
 * @return	Nothing
 */
void ETH_IRQHandler(void)
{
	// Ethernet is polled
}

void init_comms(void)
{
  int32_t i;
	uint32_t physts = 0;

	/* Setup ethernet and PHY */
	Chip_ENET_Init(LPC_ETHERNET);
#if defined(USE_RMII)
	lpc_phy_init(true, localMsDelay);
#else
	lpc_phy_init(false, localMsDelay);
#endif

	// Setup MAC address for device
	MyMAC[0] = MYMAC_6;
	MyMAC[1] = MYMAC_5;
	MyMAC[2] = MYMAC_4;
	MyMAC[3] = MYMAC_3;
	MyMAC[4] = MYMAC_2;
	MyMAC[5] = MYMAC_1;
	Chip_ENET_SetADDR(LPC_ETHERNET, MyMAC);

	/* Setup descriptors */
	InitDescriptors(TXDescs, ENET_NUM_TX_DESC, RXDescs, ENET_NUM_RX_DESC);

	/* Attach a buffer to a RX descriptor and queue it for receive */
	i = 0;
	while (i < ENET_NUM_RX_DESC) {
		ENET_RXQueue(RXBuffer[i], EMAC_ETH_MAX_FLEN);
		i++;
	}

  // initialize transmit index
  txNextIndex = 0;
  
  // no handshaking yet
  synchro = false;
  
	/* Enable RX/TX after descriptors are setup */
	Chip_ENET_TXEnable(LPC_ETHERNET);
	Chip_ENET_RXEnable(LPC_ETHERNET);  

  Timer_init();
  
  // wait for ethernet to become functional  
  while (1)
  {
    // PHY status state machine. This function will not block.
		physts = lpcPHYStsPoll();

		// Only check for connection state when the PHY status has changed
		if (physts & PHY_LINK_CHANGED)
    {
			if (physts & PHY_LINK_CONNECTED)
      {
				// Set interface speed and duplex
				Chip_ENET_SetDuplex(LPC_ETHERNET, (bool) (physts & PHY_LINK_FULLDUPLX));
				Chip_ENET_SetSpeed(LPC_ETHERNET, (bool) (physts & PHY_LINK_SPEED100));
        break;        
			}
    }
  }
}

// call frequently to keep comms module operational
void comms_process(void)
{
  // Transmit buffers are 'zero-copy' buffers with the queue function, so
  // the buffer must remain in memory until the packet has been fully
  // transmitted. Call ENET_TXBuffClaim() to to reclaim a sent
  // packet. If a packet buffer address is return (not NULL), then the
  // packet can be de-allocated. Since the buffers in this examples are
  // static, there isn't too much to do
  ENET_TXBuffClaim();
}

// gets a command from the next packet received
// returns 0 if packet received, 1 if timeout
unsigned getline_ex(char * buf,int max_len,int * count, int *srcport, int *destport, uint8_t hostmac[],
  uint8_t srcipaddr[], uint8_t destipaddr[])
{
  uint8_t *workbuff;
  int32_t rxBytes;
  unsigned result = 1;
  uint32_t timeout;
  int i;

  /* get timestamp for end of timeout period */
  timeout = Timer_GetTimestamp() + ETHERNET_TIMEOUT;
  
  while (1)
  {    
    // check for receive packets
    workbuff = ENET_RXGet(&rxBytes);
    if (workbuff)
    {
      // is this a packet we want?
      if (filter(workbuff))
      {
        // get length of UDP data
        // length = IP payload len + Eth header len - Eth+IP+UDP header len
        *count = HTONS(*(uint16_t *)(&workbuff[16])) + EtherHdrLen - HdrLen; 
        // limit length to max size
        if (*count > max_len) *count = max_len;
        // copy to receive buffer
        memcpy(buf, workbuff + HdrLen, *count);
        // replace CR-LF with NUL
        for (i = 0; i < max_len; i++)
        {
          if (buf[i] == CR)
          {
            buf[i] = '\0';
            break;
          }
        }
        *count = i;
        
        // copy host mac address
        if (hostmac != NULL) memcpy(hostmac, workbuff + 6, 6);
        
        // get port numbers
        if (srcport != NULL)  *srcport  = HTONS(*(uint16_t *)(&workbuff[34]));
        if (destport != NULL) *destport = HTONS(*(uint16_t *)(&workbuff[36]));
        
        // get ip addresses
        if (srcipaddr != NULL) memcpy(srcipaddr, &workbuff[26], 4);
        if (destipaddr != NULL) memcpy(destipaddr, &workbuff[30], 4);
        
        // packet received
        result = 0;
        // Use the descriptor again to receive another frame
        ENET_RXQueue(workbuff, EMAC_ETH_MAX_FLEN);
        // done
        break;
      }
      // ignore this packet
      else
      {
        // Use the descriptor again to receive another frame
        ENET_RXQueue(workbuff, EMAC_ETH_MAX_FLEN);
      }
    }
    
    // check for timeout
    if (Timer_HasTimestampExpired(timeout))
    {
      // timeout
      result = 1;
      break;
    }
  }
  
  return result;
}

// gets a command from the next packet received
// returns 0 if packet received, 1 if timeout
unsigned getline(char * buf, int max_len, int * count)
{
  return getline_ex(buf, max_len, count, NULL, NULL, NULL, NULL, NULL);
}

unsigned sendline_crlf(char * buf)
{
  uint8_t *workbuff;
  int i;
  int data_size;
  int FrameSize;
  uint16_t temp;

  comms_process();
  
  // Get next available TX buffer
  workbuff = (uint8_t *) TXBuffer[txNextIndex];
  txNextIndex = incIndex(txNextIndex, ENET_NUM_TX_DESC);

  // Complete some IP & UDP header values
  workbuff[12] = 0x08;
  workbuff[13] = 0x00;
  workbuff[14] = 0x45;
  workbuff[15] = 0x00;
  workbuff[18] = 0xD8;
  workbuff[19] = 0xF4;
  workbuff[20] = 0x00;
  workbuff[21] = 0x00;
  workbuff[22] = 0x80;
  workbuff[23] = 0x11;
    
  // copy data, appending CR-LF
  for (i = 0; i < CMD_SIZE; i++)
  {
    workbuff[HdrLen + i] = buf[i];
    if (buf[i] == '\0')
    {
      workbuff[HdrLen + i]     = CR;
      workbuff[HdrLen + i + 1] = LF;
      break;
    }
  }
  data_size = i + 2;
  FrameSize = data_size + HdrLen;
  
  // set mac addresses
  memcpy(workbuff, HostMAC, 6);
  memcpy(workbuff + 6, MyMAC, 6);
  
  // set IP payload length
  temp = HTONS(FrameSize - EtherHdrLen);
  memcpy(workbuff + 16, &temp, 2);

  // set ip addresses
  memcpy(workbuff + 30, srcipaddr, 4);
  memcpy(workbuff + 26, destipaddr, 4);

  // set port numbers
  temp = HTONS(srcport);
  memcpy(workbuff + 36, &temp, 2);
  temp = HTONS(destport);
  memcpy(workbuff + 34, &temp, 2);

  // set udp payload length
  temp = HTONS(FrameSize - EtherHdrLen - IpHdrLen);
  memcpy(workbuff + 38, &temp, 2);

  // clear udp checksum
  temp = 0;
  memcpy(workbuff + 40, &temp, 2);

  // ip header checksum
  temp = 0;
  memcpy(workbuff + 24, &temp, 2);
  temp = HTONS(chksum16(workbuff + EtherHdrLen, IpHdrLen));
  memcpy(workbuff + 24, &temp, 2);

  // Queue TX packet
  ENET_TXQueue(workbuff, FrameSize);
  
  return 1;
}

void comm_handshake(void)
{
  int count;
  char buf[15];
  char buf1[15];

  synchro = false;
  
  while (1)
  {
    if (getline_ex(buf, 1, &count, &srcport, &destport, HostMAC, srcipaddr, destipaddr) == 0)
    {
      if (buf[0] == '?')
      {
        sendline_crlf((char *)&sync[0]);
        getline(buf, 15, &count);
        memcpy(&buf1[0],&sync[0],sizeof(sync));
	      if (str_cmp(&buf[0],&buf1[0])==0)
        {
          sendline_crlf((char *)&Ok[0]);
		      synchro = true;
		      break;
        }
      }
    }
  }
}
