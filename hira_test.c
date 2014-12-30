#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include "MT.h"

#define C_MAX 301
#define I_MAX 100
#define K_MAX 2000


int input_word(char *s, int *list){
    int i,j;
    int *h=(int *)calloc(I_MAX,sizeof(int));
    int val = (int)(strlen(s)/3);

    if(stoCode(s,h)) return 1;
    if(checkStyle(h, val)) return 1;
    for(i=0;i<val;i++) list[i]=toid(h[i]);
    free(h);
    return 0;
}

int input_file(char *fname,int** s){
    FILE *fp;
    int i=0,val=0,n=0;
    char *a,c;
    fp = fopen(fname, "r");
    if( fp == NULL ) return 1;

    a=malloc(sizeof(char*)*C_MAX);
    s[0]=(int *)calloc(I_MAX,sizeof(int));
    while((c=fgetc(fp))!=EOF){
        if(c!='\n'){
            a[i]=c;
            i++;val++;
        }else{
            a[i]='\0';
            if(input_word(a,s[n])){
                return 1;
            }
            i=0;val=0;n++;
            a=malloc(sizeof(char*)*C_MAX);
            s[n]=(int *)calloc(I_MAX,sizeof(int));
        }
    }
    free(a);
    fclose(fp);
    return 0;
}

int stoCode(unsigned char *s, int *h){
    int i;
    if(strlen(s)%3!=0) return 1;
    for(i=0;i<(int)(strlen(s)/3);i++){
        h[i]+=s[0+3*i]<<16;
        h[i]+=s[1+3*i]<<8;
        h[i]+=s[2+3*i];
    }
    return 0;
}
int itos(int *h, unsigned char *a){
    int i;
    for(i=0;h[i]!=0;i++){
        a[0+3*i]=tocode(h[i])>>16&0x000000FF;
        a[1+3*i]=tocode(h[i])>>8&0x000000FF;
        a[2+3*i]=tocode(h[i])&0x000000FF;
    }
    a[3+3*(i-1)]='\0';
    return 0;
}

int toid(int h){
    int x0=0xE38181,x81=0xE381BF,x82=0xE38280;
    if(h==0xE383BC) return 84;
    h-=x0;
    if(h>62) h-=(x82-x81)-1;
    return h+1;
}


int tocode(int h){
    int x0=0xE38181,x81=0xE381BF,x82=0xE38280;
    if(h==84) return 0xE383BC;
    h--;
    h+=x0;
    if(h>x81) h+=(x82-x81)-1;
    return h;
}

int checkStyle(int *h, int size){
    int i;
    for(i=0;i<size;i++){
        if((h[i]>0xE38293||h[i]<0xE38181)&&h[i]!=0xE383BC){
            return 1;
        }
    }
    return 0;
}

int intcpy(int *a, int *b){
    int i=0;
    while(b[i]!=0){
        a[i]=b[i];
        i++;
    }
    return 0;
}

int intcmp(int *a, int *b){
    int i=0;
    while(a[i]!=0||b[i]!=0){
        if(a[i]!=b[i]) return 1;
        i++;
    }
    return 0;
}

int searchid(int *h, int **list){
    int i=0;
    while(list[i][0]!=0){
        if(!intcmp(h,list[i])) return 0;
        i++;
    }
    return 1;
}

int lastisN(int *str){
    int i=0;
    while(str[i]!=0) i++;
    if(str[i-1]==83) return 1;
    else return 0;
}

int match_id(int *pre_str, int *str){
    int i=0;
    while(pre_str[i]!=0) i++;
    if(pre_str[i-1]==84) pre_str[i-1]=pre_str[i-2];
    if(pre_str[i-1]%2==1&&pre_str[i-1]<10) pre_str[i-1]++;
    if(pre_str[i-1]==str[0]) return 0;
    else return 1;
}

int judgeStr(int *pre_str, int *str, int **idList, int **keyList){
    int i=0;
    if(pre_str==NULL){
        if(lastisN(str)) return 4;
        else return 0;
    }
    if(searchid(str,keyList)) return 1;
    else if(!searchid(str,idList)) return 2;
    else if(match_id(pre_str,str)) return 3;
    else if(lastisN(str)) return 4;
    else return 0;
    // ジャンル外 1, 使用済み 2, 頭尾不一致 3, 最後に'ん' 4
}

int init_randd(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    init_genrand((unsigned)tv.tv_usec);
    return 0;
}


int randd(int min, int max){
    return min+genrand_int32()%(max-min+1);
}

int srtr_ai(int *pre_str, int **keyList, int **idList, int lv, int *p){
    int i=0,k=0;
    int match_flag=0;
    int *key_id=(int *)calloc(100,sizeof(int));
    char *c=malloc(sizeof(char)*100);
        printf("aaa");
    init_randd();/*
    while(keyList[k][0]!=0){
        if(pre_str!=NULL){
            if(!judgeStr(pre_str,keyList[k],idList,keyList)) match_flag=1;
        }else match_flag=1;
        k++;
    }*/
    k=1;
    itos(keyList[randd(0,k-1)],c);
    intcpy(key_id,keyList[randd(0,k-1)]);
    while(judgeStr(pre_str,key_id,idList,keyList)&&match_flag){
        if(pre_str!=NULL) napms((int)((k/2)/lv));
        key_id=(int *)calloc(100,sizeof(int));
        intcpy(key_id,keyList[randd(0,k-1)]);
        i++;
        if(pre_str!=NULL) if(i>k*2) return 1;
        if(i>k*100) return 1;
    }
    i=0;
    while(!lastisN(key_id)&&!match_flag){
        if(pre_str!=NULL) napms((int)((k/2)/lv));
        key_id=(int *)calloc(100,sizeof(int));
        intcpy(key_id,keyList[randd(0,k-1)]);
        i++;
        if(i>k*2) return 1;
    }
    intcpy(p,key_id);
    return 0;
}

int main(){
    int i;
    int **id=calloc(2000,sizeof(int*));
    int **key=calloc(2000,sizeof(int*));
    int *n=calloc(100,sizeof(int));
    char *a=(char*)malloc(20);
    /*
    for(i=1;i<85;i++){
        n[0]=i;
        itos(n,a);
        printf("%s",a);
    }
    printf("\n");
    */
    id[0]=calloc(301,sizeof(int));
    key[0]=calloc(301,sizeof(int));
    key[0][0]=2;
    key[0][1]=1;
    n[0]=2;
    n[1]=4;
    int *p=calloc(100,sizeof(int));
    srtr_ai((int *)NULL,key,id,1,p);
    
    return 0; 
}
