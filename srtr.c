#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef DEBUG
#define depri printf
#else
#define depri 1 ? (void) 0 : printf
#endif

#define C_MAX 301
#define I_MAX 100
#define KEY_MAX 2000

int input_word(char *s, int *list){
    int i,j;
    int *h=calloc(I_MAX,sizeof(int));
    int val = (int)(strlen(s)/3);

    depri("input %2X%2X%2X〜〜〜\n",s[0]& 0x000000FF, s[1]& 0x000000FF, s[2]& 0x000000FF);
    if(stoi(s,h)){
      depri("stoi faild...\n");
      return 1;
    }
    depri("h=%X\n",h[0]);
    if(checkStyle(h, val)){
        depri("Not Hiragana!!\n");
        return 1;
    }
    	
    depri("%2X[%d]\n",h[val-1],toid(h[val-1]));
    for(i=0;i<val;i++){
        list[i]=toid(h[i]);
        depri("add list => %d\n",list[i]);
    }
    free(h);
    return 0;
}

int input_file(char *fname,int** s){
    FILE *fp;
    int i=0,val=0,n=0;
    char *a,c;
    fp = fopen(fname, "r");
    if( fp == NULL ){
        depri( "%sファイルが開けません\n", fname );
        return 1;
    }
    a=malloc(sizeof(char*)*C_MAX);
    s[0]=calloc(I_MAX,sizeof(int));
    while((c=fgetc(fp))!=EOF){
        if(c!='\n'){
            a[i]=c;
            i++;val++;
        }else{
            a[i]='\0';
            depri("inputfile => %s",a);
            if(input_word(a,s[n])){
                return 1;
            }
            i=0;val=0;n++;
            a=malloc(sizeof(char*)*C_MAX);
            s[n]=calloc(I_MAX,sizeof(int));
        }
    }
    free(a);
    fclose(fp);
    return 0;
}

int stoi(unsigned char *s, int *h){
    int i;
    if(strlen(s)%3!=0) return 1;
    for(i=0;i<(int)(strlen(s)/3);i++){
        h[i]+=s[0+3*i]<<16; depri("%2X\n",s[0+3*i]<<16);
        h[i]+=s[1+3*i]<<8; depri("%2X\n",s[1+3*i]<<8);
        h[i]+=s[2+3*i]; depri("%2X\n",s[2+3*i]);
    }
    return 0;
}

int toid(int h){
    int x0=0xE38181,x81=0xE381BF,x82=0xE38280;
    
    if(h==0xE383BC) return 83;
    
    h-=x0;
    if(h>62) h-=(x82-x81)-1;
   	return h+1;
}

int checkStyle(int *h, int size){
    int i,j;
    
    depri("size = %d\n",size);
        for(i=0;i<size;i++){
            depri("check [%2X]\n",h[i]);
            depri("%d %d %d\n",h[i]>0xE38294,h[i]<0xE38180,!(h[i]!=0x83BC));
            if((h[i]>0xE38294||h[i]<0xE38180)&&h[i]!=0xE383BC){
                depri("false [%2X]\n",h[i]);
                return 1;
            }
        }
    depri("success [");
    for(j=0;j<i;j++) depri("%d ",toid(h[j]));
    depri("]\n");
    return 0;
}

int intcmp(int *a, int *b){
    int i=0;
    while(a[i]!=0||b[i]!=0){
        depri("%d == %d\n",a[i],b[i]);
        if(a[i]!=b[i]) return 1;
        i++;
    }
    return 0;
}

int searchid(int *h, int **list){
    int i=0;
    depri("list[0] = %d\n",list[0][0]);
    while(list[i][0]!=0){
        if(!intcmp(h,list[i])) return 0;
        i++;
    }
    return 1;
}

