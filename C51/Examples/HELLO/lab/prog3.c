#include<p89v51rd2.h>
void delay(int d){
	int i,j;
	for(i=0;i<=1000;i++)
		for(j=0;j<=d;j++);
}
void main(void){
	int i,j,k;
	while(1){
		for(i=0;i<=1;i++){
			for(j=0;j<=1;j++){
				for(k=0;k<=1;k++){
					delay(1000);
					RxD = i; TxD=j; WR=k;
					delay(1000);
				}
			}
		}
	}
}