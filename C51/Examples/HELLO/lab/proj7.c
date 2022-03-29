#include <lcd.h>
 
 void main(void){
	 LCD_INIT();
	 LCD_WRITE("Tanmay Vig",0,0);
	 LCD_WRITE("19BCS061",1,0);
	 while(1);
}