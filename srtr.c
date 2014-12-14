#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C_MAX 301
#define I_MAX 100
#define KEY_MAX 2000

int input_word(char *s, int *list){
	int i,j;
	s=malloc(sizeof(unsigned char)*C_MAX);
	int *h=malloc(sizeof(int)*I_MAX);
	int val = (int)(strlen(s)/3);

	printf("input %2X%2X%2X〜〜〜\n",s[0]& 0x000000FF, s[1]& 0x000000FF, s[2]& 0x000000FF);
	if(stoi(s,h)){
		printf("stoi faild...\n");
		return 0;
	}
		printf("h=%X\n",h[0]);
	if(checkStyle(h, val)){
		printf("Not Hiragana!!\n");
		return 0;
	}
	
	printf("%2X[%d]\n",h[val-1],toid(h[val-1]));
	
	for(i=0;i<val;i++){
		list[i]=toid(h[i]);
	}

	free(h);
	return val;
}

int input_file(char *fname,int** s){
	FILE *fp;
	int i=0,val=0,n=0;
	char *a,c;
	fp = fopen(fname, "r");
	if( fp == NULL ){
		printf( "%sファイルが開けません\n", fname );
		return -1;
	}
	a=malloc(sizeof(char*)*C_MAX);
	while((c=fgetc(fp))!=EOF){
		if(c!='\n'){
			a[i]=c;
			i++;val++;
		}else{
			a[i]='\0';
			s[n]=malloc(sizeof(int)*I_MAX);
			input_word(a,s[n]);
			i=0;val=0;n++;
			a=malloc(sizeof(char*)*C_MAX);
		}
		
	}
	free(a);
	fclose(fp);
	return n-1;
}

int stoi(unsigned char *s, int *h){
	int i;
	if(strlen(s)%3!=0) return 1;
	for(i=0;i<(int)(strlen(s)/3);i++){
		h[i]+=s[0+3*i]<<16; printf("%2X\n",s[0+3*i]<<16);
		h[i]+=s[1+3*i]<<8; printf("%2X\n",s[1+3*i]<<8);
		h[i]+=s[2+3*i]; printf("%2X\n",s[2+3*i]);
	}
	return 0;
}

int toid(int h){
	int x0=0xE38181,x81=0xE381BF,x82=0xE38280;
	
	if(h==0xE383BC) return 83;
	
	h-=x0;
	if(h>62) h-=(x82-x81)-1;
   	return h;
}

int checkStyle(int *h, int size){
	int i;
	
	printf("size = %d\n",size);
		for(i=0;i<size;i++){
			printf("check [%2X]\n",h[i]);
			printf("%d %d %d\n",h[i]>0xE38294,h[i]<0xE38180,!(h[i]!=0x83BC));
			if((h[i]>0xE38294||h[i]<0xE38180)&&h[i]!=0xE383BC){
				printf("false [%2X]\n",h[i]);
				return 1;
			}
		}
		printf("success [%2X]\n",h[i-1]);
	return 0;
}
