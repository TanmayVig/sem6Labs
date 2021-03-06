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
#if USE_ETHERNET
#include "ethernet\emac.h"
#endif

void init_comms(void)
{
  #if USE_ETHERNET
	init_emac();
	#endif
}

unsigned getline(char * buf,int max_len,int * count)
{
  #if USE_ETHERNET
	return (emac_getline(buf,max_len,count));
	#endif
}

unsigned sendline_crlf(char * buf)
{
  #if USE_ETHERNET
	return (emac_sendline_crlf(buf));
	#endif
}

void comm_handshake(void) {
    
	#if USE_ETHERNET
	emac_handshake();
	#endif   

}
