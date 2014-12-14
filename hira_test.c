#include <stdio.h>
#include <stdlib.h>

int main(){
    int i,x=0x8181;
    char *a;
	
	a=(char*)malloc(4);
	
    for(i=0; x<=0x8293; ++i){
		a[0]=0xE3;
		a[1]=(x>>8)&0xff;
		a[2]=(x)&0xff;
		a[3]='\0';
		printf("%s[%2X%2X%2X]\n", a,a[0]& 0x000000FF, a[1]& 0x000000FF, a[2]& 0x000000FF);
    	if(x==0x81BF) x=0x8280; else x++;
    }
    
    printf("\n");
 	
	free(a);
    return 0; 
}