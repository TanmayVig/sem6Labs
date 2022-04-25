#include<p89v51rd2.h>

sbit led1=P3^0;
sbit led2=P3^1;
sbit led3=P3^6;
sbit led4=P3^7;
//led declarations
sbit sw1=P3^2; //msb
sbit sw2=P3^3;
sbit sw3=P3^4;
sbit sw4=P3^5; //lsb
//switch declarations
sbit bzz = P0^3;

void delay(unsigned int dela) // delay function
{
unsigned int i,j;
for(i=0;i<=1000;i++)
for(j=0;j<=dela;j++);
	
}

void buzzer(){
	bzz=0;
	delay(1000);
	bzz=1;
}
unsigned int i,j,t,num,temp;
void main(void)
{
	led1=led2=led3=led4=1;
	sw1=sw2=sw3=sw4=1; // setting all switche's default value
	
	while(1)
	{
		num=0;
		if(sw1==0) num=num+8;
		if(sw2==0) num=num+4;
		if(sw3==0) num=num+2;
		if(sw4==0) num=num+1;
		
		bzz=1;
		for(i = 2;i<num;i++){
			if(num%i==0){ // checking for factor
				for(j=0;j<4;++j){ // setting up leds
					temp = 1 << j;
					if(j == 0) led1 = !(temp & i);
					else if(j==1) led2 = !(temp & i);
					else if (j==2) led3 = !(temp & i);
					else led4 = !(temp & i);
				}
				delay(2500);
				buzzer();
				led1=led2=led3=led4=1;
			}
		}		
	}
}