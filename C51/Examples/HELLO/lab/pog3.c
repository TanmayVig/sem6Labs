#include <p89v51rd2.h>
void delay(unsigned int d)
{
unsigned int i,j;
	for(i=0;i< 1000;i++)
		for(j=0;j< d;j++);
}
void main()
{
	int count;
	while(1){
		for(count=0;count<8;count++){
			delay(2000);
			P3=count;
			delay(2000);
		}
	}
}