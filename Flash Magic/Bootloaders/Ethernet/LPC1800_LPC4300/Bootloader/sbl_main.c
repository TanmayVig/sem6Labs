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

#include "chip.h"
#include "board.h"

#include "sbl_config.h"
#include "comms.h"
#include "isp_iap.h"

const unsigned crp __attribute__((section(".ARM.__at_0x1A0002FC"))) = CRP;

void enter_isp(void)
{
	SystemCoreClockUpdate();
	Board_Init();
  init_comms();
  
  /* loops when timeout occurs */
  while (1) {
    comm_handshake();
    isp_cmd_loop();
  }
}

/* Main Program */
int main (void)
{
  if( user_code_present() )
  {
    if ( crp == CRP3 )
	  {
      // CRP3 is enabled and user flash start sector is not blank, 
		  //execute the user code
		  execute_user_code();
    }
	  else
	  {
	    if ( check_isp_entry_pin() )
		  {
		    // isp entry not requested and CRP3 not enabled
		    execute_user_code();
		  }
		  else
		  {
		    // isp entry requested and CRP3 not enabled
			  enter_isp();
		  }
	  }
  }
  // User code not present, enter isp
  enter_isp();

  return 1;
}
