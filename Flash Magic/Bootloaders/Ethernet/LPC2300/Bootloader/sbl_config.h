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
//   <o0> User Start Sector <2=> Sector 2 <3=> Sector 3
//   <o1> User Start Sector Address <0x2000=> Sector 2 <0x3000=> Sector 3
//   <o2> User End Sector <10=> Sector 10 <14=> Sector 14 <27=> Sector 27
//   <o3> User End Sector Address <0x1FFFF=> Sector 10 <0x3FFFF=> Sector 14 <0x7DFFF=> Sector 27
//   <o4> Code Read Protection
//        <0x11223344=> NO CRP <0x12345678=> CRP1 <0x87654321=> CRP2 <0x43218765=> CRP3
// </h>
*/

#define USER_START_SECTOR 2
#define USER_START_SECTOR_ADDRESS 8192
#define USER_END_SECTOR 27
#define USER_END_SECTOR_ADDRESS 516095
#define CRP 0x11223344
#define USER_START_SECTOR_SIZE 0x1000
#define RAM_START 0x40000000
#define RAM_END	  0x4000FFFF

extern const unsigned fcclk_KHz;
extern const unsigned crp;

#define CRP1  0x12345678
#define CRP2  0x87654321
#define CRP3  0x43218765
#define NOCRP 0x11223344

/*
// <h> Update Entry Pin
//   <o0> Port
//        <0xE0028000=> Port 0 <0xE0028010=> Port 1
//   <o1> Pin <0-31>
// </h>
*/
#define ISP_ENTRY_GPIO_REG 0xE0028000  /* Port */
#define ISP_ENTRY_PIN 	   15          /* Pin  */

/*
// <h> Interface Selection
//   <o0> Interface
//        <1=> Ethernet <0=> Other
// </h>
*/
#define USE_ETHERNET 	1			   /* Use Interface */

/*
// <h> Ethernet Options
//   <o0> Use Debug
//        <1=> Yes <0=> No 
//   <o1> PHY chip
//        <0=> National PHY <1=> Other PHY
//   <o2> PHY address  <00-31>
//   <o3> MAC_Address_6 <0x00-0xFF>   
//   <o4> MAC_Address_5 <0x00-0xFF>  
//   <o5> MAC_Address_4 <0x00-0xFF>  
//   <o6> MAC_Address_3 <0x00-0xFF>  
//   <o7> MAC_Address_2 <0x00-0xFF>  
//   <o8> MAC_Address_1 <0x00-0xFF>  
// </h>
*/
#define ETHERNET_DEBUG 	0			   /* Use Debug */
#define PHY_CHIP		0			   /* PHY chip */
#define PHY_ADDRESS		1			   /* PHY address */
#define MYMAC_6         0x0C		   /* MAC_6 */
#define MYMAC_5         0x1D		   /* MAC_5 */
#define MYMAC_4         0x12		   /* MAC_4 */
#define MYMAC_3         0xE0		   /* MAC_3 */
#define MYMAC_2         0x1F		   /* MAC_2 */
#define MYMAC_1         0x10           /* MAC_1 */ 
            
/*
// <h> Activity Indicator LED
//   <o0> Enabled
//        <1=> Yes  <0=> No
//   <o1> Select LED (Port 2 - pin X)
//        LED <0-7>
// </h>
*/
#define  LED_ENABLED	1			   /* LED enabled */
#define  PORT2_PIN		7			   /* Port 2 pin */

/*
// <h> Protocol Options
//   <o0> Timeout (milliseconds) <0-1000000>   
// </h>
*/
#define ETHERNET_TIMEOUT 60000         /* Ethernet timeout */

#endif  /* __SBL_CONFIG_H__ */
