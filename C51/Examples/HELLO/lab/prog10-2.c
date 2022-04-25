#include<p89v51rd2.h>

sbit flag = P1^5;

void T1M2Delay(){
	
	TMOD=0x20; 	// Timer 1, mode 2
	TH1=0x1A;		// 1A for 250micro sec
	TR1=1;

	while (TF1==0);
	
	TR1=0;
	TF1=0;
	
}

void T0M1Delay(void)
{
	TMOD=0x01;
	TL1=0xFE;
	TH1=0xA5; // A5FE F0R 25 m sec
	TR1=1;
	while (TF1==0);
	TR1=0;
	TF1=0;
}
void main(void)
{
	unsigned int x;
	while (1)
	{
		flag=~flag;
		
		for (x=0;x<1000;x++) // 250 micro sec
			T1M2Delay();
		
		//for (x=0;x<10;x++) // 25 m sec
			//T0M1Delay();
		
		
	}
}