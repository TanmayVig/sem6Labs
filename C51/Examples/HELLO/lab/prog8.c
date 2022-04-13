#include <lcd.h>
#include <stdio.h>
void slow(int d){
	int i,j;
	for(i=0;i<=1000;i++)
		for(j=0;j<=d;j++);
}

void toScreen(INT0, INT1, T0, T1){
	int num, i;
	unsigned long fact;
	char charTemp[16];
	
	num = (8)*(INT0^1) + (4)*(INT1^1) + (2)*(T0^1) + (1)*(T1^1);
	
	for(i=1;i<=num;i++)
      fact = fact*i;
	
	sprintf(charTemp,"%lu",fact);
	
	LCD_WRITE(charTemp, 0, 0);
	slow(100000);
}

void main()
{
	INT0 = 0;
	INT1 = 0;
	T0 = 0;
	T1 = 0;

	LCD_INIT();
	
	while(1){
		toScreen(INT0, INT1, T0, T1);
	}
}