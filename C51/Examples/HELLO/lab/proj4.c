#include<p89v51rd2.h>
void delay(unsigned int dela)
{
unsigned int i,j;
for(i=0;i<=1000;i++)
for(j=0;j<=dela;j++);
	
}

unsigned int i,j,temp,max;
void main(void)
{
	P3 = 0;
while(1)
{
	if(INT0 == 0 && INT1 == 1){
		max = 2;
	}else if(INT1 == 0 && INT0 ==1){
		max = 4;
	}else if(INT0 == 0 && INT1 ==0){
		max = 8;
	}else{
		max = 0;
	}
	// Max count 7 for 3 bits
	for(i=0;i< max;++i){
		// Assigning appropriate bits to pins of led
		for(j=0;j<3;++j){
			temp = i >> j;
			if(j == 0){
				RxD = temp & 1;
			}
			else if(j==1){
				TxD = temp & 1;
			}
			else{
				WR = temp & 1;
			}
		}
		delay(1500);
	}
}
}