/*--------------------------------------------------------------------------
N76E885.H

Header file for Nuvoton N76E885
--------------------------------------------------------------------------*/

#ifndef __N76E885_H__
#define __N76E885_H__

sfr P0          = 0x80;
sfr SP          = 0x81;
sfr DPL         = 0x82;
sfr DPH         = 0x83;
sfr RWK         = 0x86;
sfr PCON        = 0x87;

sfr TCON        = 0x88;
sfr TMOD        = 0x89;
sfr TL0         = 0x8A;
sfr TL1         = 0x8B;
sfr TH0         = 0x8C;
sfr TH1         = 0x8D;
sfr CKCON       = 0x8E;
sfr WKCON       = 0x8F;

sfr P1          = 0x90;
sfr SFRS        = 0x91; //TA Protection
sfr CAPCON0     = 0x92;
sfr CAPCON1     = 0x93;
sfr CAPCON2     = 0x94;
sfr CKDIV       = 0x95;
sfr CKSWT       = 0x96; //TA Protection
sfr CKEN        = 0x97; //TA Protection

sfr SCON        = 0x98;
sfr SBUF        = 0x99;
sfr SBUF_1      = 0x9A;
sfr EIE         = 0x9B;
sfr EIE1        = 0x9C;
sfr CHPCON      = 0x9F; //TA Protection

sfr P2          = 0xA0;
sfr AUXR1       = 0xA2;
sfr BODCON0     = 0xA3; //TA Protection
sfr IAPTRG      = 0xA4; //TA Protection
sfr IAPUEN      = 0xA5;
sfr IAPAL       = 0xA6;
sfr IAPAH       = 0xA7;

sfr IE          = 0xA8;
sfr SADDR       = 0xA9;
sfr WDCON       = 0xAA; //TA Protection
sfr BODCON1     = 0xAB; //TA Protection
sfr P3M1        = 0xAC;
sfr P3S         = 0xAC; //Page1
sfr P3M2        = 0xAD;
sfr P3SR        = 0xAD; //Page1
sfr IAPFD       = 0xAE;
sfr IAPCN       = 0xAF;

sfr P3          = 0xB0;
sfr P0M1        = 0xB1;
sfr P0S         = 0xB1; //Page1
sfr P0M2        = 0xB2;
sfr P0SR        = 0xB2; //Page1
sfr P1M1        = 0xB3;
sfr P1S         = 0xB3; //Page1
sfr P1M2        = 0xB4;
sfr P1SR        = 0xB4; //Page1
sfr P2M1        = 0xB5;
sfr P2S         = 0xB5; //Page1
sfr P2M2        = 0xB6;
sfr P2SR        = 0xB6; //Page1
sfr IPH         = 0xB7;

sfr IP          = 0xB8;
sfr SADEN       = 0xB9;
sfr SADEN_1     = 0xBA;
sfr SADDR_1     = 0xBB;
sfr I2DAT       = 0xBC;
sfr I2STAT      = 0xBD;
sfr I2CLK       = 0xBE;
sfr I2TOC       = 0xBF;

sfr I2CON       = 0xC0;
sfr I2ADDR      = 0xC1;
sfr ADCRL       = 0xC2;
sfr ADCRH       = 0xC3;
sfr T3CON       = 0xC4;
sfr RL3         = 0xC5;
sfr RH3         = 0xC6;
sfr TA          = 0xC7;

sfr T2CON       = 0xC8;
sfr T2MOD       = 0xC9;
sfr RCMP2L      = 0xCA;
sfr RCMP2H      = 0xCB;
sfr TL2         = 0xCC;
sfr TH2         = 0xCD;
sfr ADCMPL      = 0xCE;
sfr ADCMPH      = 0xCF;

sfr PSW         = 0xD0;
sfr PWMPH       = 0xD1;
sfr PWM01H      = 0xD2;
sfr PWM23H      = 0xD3;
sfr PWM67H      = 0xD4;
sfr PWM45H      = 0xD5;
sfr PNP         = 0xD6;
sfr FBD         = 0xD7;

sfr PWMCON0     = 0xD8;
sfr PWMPL       = 0xD9;
sfr PWM01L      = 0xDA;
sfr PWM23L      = 0xDB;
sfr PWM67L      = 0xDC;
sfr PWM45L      = 0xDD;
sfr PIO         = 0xDE;
sfr PWMCON1     = 0xDF;

sfr ACC         = 0xE0;
sfr ADCCON1     = 0xE1;
sfr ADCCON2     = 0xE2;
sfr ADCDLY      = 0xE3;
sfr C0L         = 0xE4;
sfr C0H         = 0xE5;
sfr C1L         = 0xE6;
sfr C1H         = 0xE7;

sfr ADCCON0     = 0xE8;
sfr PICON       = 0xE9;
sfr PINEN       = 0xEA;
sfr PIPEN       = 0xEB;
sfr PIF         = 0xEC;
sfr C2L         = 0xED;
sfr C2H         = 0xEE;
sfr EIP         = 0xEF;

sfr B           = 0xF0;
sfr ADCAQT      = 0xF2;
sfr SPCR        = 0xF3;
sfr SPSR        = 0xF4;
sfr SPDR        = 0xF5;
sfr P0DIDS      = 0xF6;
sfr EIPH        = 0xF7;

sfr SCON_1      = 0xF8;
sfr PDTEN       = 0xF9; //TA Protection
sfr PDTCNT      = 0xFA; //TA Protection
sfr PMEN        = 0xFB;
sfr PMD         = 0xFC;
sfr EIP1        = 0xFE;
sfr EIPH1       = 0xFF;

/*  BIT Registers  */
/*  SCON_1  */
sbit SM0_1      = SCON_1^7;
sbit FE_1       = SCON_1^7; 
sbit SM1_1      = SCON_1^6; 
sbit SM2_1      = SCON_1^5; 
sbit REN_1      = SCON_1^4; 
sbit TB8_1      = SCON_1^3; 
sbit RB8_1      = SCON_1^2; 
sbit TI_1       = SCON_1^1; 
sbit RI_1       = SCON_1^0; 

/*  ADCCON0  */
sbit ADCF       = ADCCON0^7;
sbit ADCS       = ADCCON0^6;
sbit ETGSEL1    = ADCCON0^5;
sbit ETGSEL0    = ADCCON0^4;
sbit ADCHS3     = ADCCON0^3;
sbit ADCHS2     = ADCCON0^2;
sbit ADCHS1     = ADCCON0^1;
sbit ADCHS0     = ADCCON0^0;

/*  PWMCON0  */
sbit PWMRUN     = PWMCON0^7;
sbit LOAD       = PWMCON0^6;
sbit PWMF       = PWMCON0^5;
sbit CLRPWM	= PWMCON0^4;
sbit INTTYP1    = PWMCON0^3;
sbit INTTYP0    = PWMCON0^2;
sbit INTSEL1    = PWMCON0^1;
sbit INTSEL0    = PWMCON0^0;

/*  PSW */
sbit CY         = PSW^7;
sbit AC         = PSW^6;
sbit F0         = PSW^5;
sbit RS1        = PSW^4;
sbit RS0        = PSW^3;
sbit OV         = PSW^2;
sbit P          = PSW^0;

/*  T2CON  */
sbit TF2        = T2CON^7;
sbit TR2        = T2CON^2;
sbit CM_CR2     = T2CON^0;
 
/*  I2CON  */
sbit I2CEN      = I2CON^6;
sbit STA        = I2CON^5;
sbit STO        = I2CON^4;
sbit SI         = I2CON^3;
sbit AA         = I2CON^2;

/*  IP  */  
sbit PADC       = IP^6;
sbit PBOD       = IP^5;
sbit PS         = IP^4;
sbit PT1        = IP^3;
sbit PX1        = IP^2;
sbit PT0        = IP^1;
sbit PX0        = IP^0;

/*  P3  */  
sbit P30        = P3^0;
sbit P31        = P3^1;
sbit P32        = P3^2;
sbit P33        = P3^3;
sbit P34        = P3^4;
sbit P35        = P3^5;
sbit P36        = P3^6;
sbit P37        = P3^7;

/*  IE  */
sbit EA         = IE^7;
sbit EADC       = IE^6;
sbit EBOD       = IE^5;
sbit ES         = IE^4;
sbit ET1        = IE^3;
sbit EX1        = IE^2;
sbit ET0        = IE^1;
sbit EX0        = IE^0;

/*  P2  */ 
sbit P20        = P2^0;
sbit RXD        = P2^0; 
sbit T0         = P2^0; 
sbit IC0        = P2^0; 
sbit AIN9       = P2^0;

sbit P21        = P2^1;
sbit MOSI       = P2^1;
sbit T1         = P2^1;
sbit IC1        = P2^1;

sbit P22        = P2^2;
sbit MISO       = P2^2;
sbit IC2        = P2^2;

sbit P23        = P2^3;
sbit SDA        = P2^3;
sbit FB         = P2^3;
sbit STADC      = P2^3;

sbit P24        = P2^4;
sbit RXD_1      = P2^4;

sbit P25        = P2^5;
sbit TXD_1      = P2^5;

sbit P26        = P2^6;
sbit CLO        = P2^6;
sbit AIN8       = P2^6;
sbit PWM7       = P2^6;

/*  SCON  */
sbit SM0        = SCON^7;
sbit FE         = SCON^7; 
sbit SM1        = SCON^6; 
sbit SM2        = SCON^5; 
sbit REN        = SCON^4; 
sbit TB8        = SCON^3; 
sbit RB8        = SCON^2; 
sbit TI         = SCON^1; 
sbit RI         = SCON^0; 

/*  P1  */                      
sbit P10        = P1^0;
sbit PWM0       = P1^0;
sbit P11        = P1^1;
sbit PWM1       = P1^1;
sbit P12        = P1^2;

/*  TCON  */
sbit TF1        = TCON^7;
sbit TR1        = TCON^6;
sbit TF0        = TCON^5;
sbit TR0        = TCON^4;
sbit IE1        = TCON^3;
sbit IT1        = TCON^2;
sbit IE0        = TCON^1;
sbit IT0        = TCON^0;

/*  P0  */  
sbit P00        = P0^0;
sbit INT0       = P0^0;
sbit AIN0       = P0^0;
sbit VREF       = P0^0;

sbit P01        = P0^1;
sbit INT1       = P0^1;
sbit AIN1       = P0^1;

sbit P02        = P0^2;
sbit PWM2       = P0^2;
sbit AIN2       = P0^2;

sbit P03        = P0^3;
sbit PWM3       = P0^3;
sbit TXD        = P0^3;
sbit AIN3       = P0^3;

sbit P04        = P0^4;
sbit SS         = P0^4;
sbit AIN4       = P0^4;

sbit P05        = P0^5;
sbit PWM4       = P0^5;
sbit SPICK      = P0^5;
sbit AIN5       = P0^5;

sbit P06        = P0^6;
sbit SCL        = P0^6;
sbit AIN6       = P0^6;
sbit PWM5       = P0^6;

sbit P07        = P0^7;
sbit AIN7       = P0^7;
sbit PWM6       = P0^7;
                

#endif