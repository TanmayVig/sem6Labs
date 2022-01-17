 /*----------------------------------------------------------------------------
 * which provides customers with programming information regarding the  
 * products. This software is supplied "AS IS" without any warranties.  
 * NXP Semiconductors assumes no responsibility or liability for the 
 * use of the software, conveys no license or title under any patent, 
 * copyright, or mask work right to the product. NXP Semiconductors 
 * reserves the right to make changes in the software without 
 * notification. NXP Semiconductors also make no representation or 
 * warranty that such application will be suitable for the specified 
 * use without further testing or modification. 
 *---------------------------------------------------------------------------*/

#ifndef  _SBL_CONFIG_H
#define  _SBL_CONFIG_H

//*** <<< Use Configuration Wizard in Context Menu >>> ***

/*
// <h> Flash Configuration
//   <o0> Bank A User Start Sector <2=> Sector 2 <3=> Sector 3
//   <o1> Bank A User Start Sector Address <0x1A004000=> Sector 2 <0x1A006000=> Sector 3
//   <o2> Bank A User End Sector <10=> Sector 10 <14=> Sector 14
//   <o3> Bank A User End Sector Address <0x1A03FFFF=> Sector 10 <0x1A07FFFF=> Sector 14
//   <o4> Bank B User End Sector <10=> Sector 10 <14=> Sector 14
//   <o5> Bank B User End Sector Address <0x1B03FFFF=> Sector 10 <0x1B07FFFF=> Sector 14
//   <o6> Code Read Protection
//        <0x11223344=> NO CRP <0x12345678=> CRP1 <0x87654321=> CRP2 <0x43218765=> CRP3
// </h>
*/

#define BANK_A_USER_START_SECTOR 2
#define BANK_A_USER_START_SECTOR_ADDRESS 0x1A004000
#define BANK_A_USER_END_SECTOR 14
#define BANK_A_USER_END_SECTOR_ADDRESS 0x1A07FFFF
#define BANK_B_USER_END_SECTOR 14
#define BANK_B_USER_END_SECTOR_ADDRESS 0x1B07FFFF
#define CRP 0x11223344

#define BANK_A_USER_START_SECTOR_SIZE 0x2000
#define RAM_START 0x10000000
#define RAM_END	  0x10007FFF
#define BANK_B_USER_START_SECTOR 0
#define BANK_B_USER_START_SECTOR_ADDRESS 0x1B000000

extern const unsigned crp;

#define CRP1  0x12345678
#define CRP2  0x87654321
#define CRP3  0x43218765
#define NOCRP 0x11223344

/*
// <h> Update Entry Pin
//   <o0> IO Port <0-15>
//   <o1> IO Pin <0-31>
//   <o2> GPIO Port
//        <0x400F6100=> Port 0
//        <0x400F6104=> Port 1
//        <0x400F6108=> Port 2
//        <0x400F610C=> Port 3
//        <0x400F6110=> Port 4
//        <0x400F6114=> Port 5
//        <0x400F6118=> Port 6
//        <0x400F611C=> Port 7
//   <o3> GPIO Pin <0-31>
// </h>
*/
#define ISP_ENTRY_IOPORT   4
#define ISP_ENTRY_IOPIN    0
#define ISP_ENTRY_GPIO_REG 0x400F6108  /* Port */
#define ISP_ENTRY_PIN 	   0           /* Pin  */


/*
// <h> Ethernet Options
//   <o0> MAC_Address_6 <0x00-0xFF>   
//   <o1> MAC_Address_5 <0x00-0xFF>  
//   <o2> MAC_Address_4 <0x00-0xFF>  
//   <o3> MAC_Address_3 <0x00-0xFF>  
//   <o4> MAC_Address_2 <0x00-0xFF>  
//   <o5> MAC_Address_1 <0x00-0xFF>  
// </h>
*/
#define MYMAC_6         0x0C		   /* MAC_6 */
#define MYMAC_5         0x1D		   /* MAC_5 */
#define MYMAC_4         0x12		   /* MAC_4 */
#define MYMAC_3         0xE0		   /* MAC_3 */
#define MYMAC_2         0x1F		   /* MAC_2 */
#define MYMAC_1         0x10       /* MAC_1 */ 
            

/*
// <h> Protocol Options
//   <o0> Timeout (milliseconds) <0-1000000>   
// </h>
*/
#define ETHERNET_TIMEOUT 60000         /* Ethernet timeout */

#endif  /* __SBL_CONFIG_H__ */
