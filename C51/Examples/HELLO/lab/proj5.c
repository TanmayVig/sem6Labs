#include<p89v51rd2.h>

sbit led1=P3^0;
sbit led2=P3^1;
sbit led3=P3^6;
sbit led4=P3^7;
//led declarations
sbit sw1=P3^2;
sbit sw2=P3^3;
sbit sw3=P3^4;
sbit sw4=P3^5;
//switch declarations

void delay(unsigned int dela) // delay function
{
unsigned int i,j;
for(i=0;i<=1000;i++)
for(j=0;j<=dela;j++);
	
}

unsigned int i,j,temp,max;
void main(void)
{
	led1=led2=led3=led4=0;
	sw1=sw2=sw3=sw4=1; // setting all switche's default value
	while(1)
	{
		if(sw1 == 0 && sw2 == 1 && sw3 == 1) // 4 bit counter
			max = 16;
		else if(sw1 == 1 && sw2 == 0 && sw3 == 1) // 2 bit counter
			max = 4;
		else if(sw1 == 1 && sw2 == 1 && sw3 ==0) // 1 bit counter
			max = 2;
		else if( sw1 == 1 && sw2 == 0 && sw3 ==0) // 3 bit counter
			max = 8;
		else // 0 bit counter
			max = 1;
		
		for(i=0;i< max;++i){ // counting upto max

			for(j=0;j<4;++j){ //setting up leds
				temp = 1 << j;
				if(j == 0) led1 = !(temp & i);
				else if(j==1) led2 = !(temp & i);
				else if (j==2) led3 = !(temp & i);
				else led4 = !(temp & i);
			}
			delay(1500);
		}
	}
}