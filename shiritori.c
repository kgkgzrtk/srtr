#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#define sleep(n) Sleep(n * 1000)

typedef unsigned char hirar[4];

int input_word(int *list, int *val);
int input_file(char *fname,int** s);
int toid(int h);
int tostr(int h);
int itos(int *h, unsigned char *a, int size);
int stoi(unsigned char *a, int *h);
int checkStyle(int *h, int size);

int** idList;
int** keyList;

int main(){

    int i=0,j,k,m,cnt=0, slen=0, *val, lastid, flag=1,flag2=0, keyval;
    char *c;
    unsigned char *last_str;

    c=malloc(sizeof(char)*10);
    val=malloc(sizeof(int)*2000);
    idList=malloc(sizeof(int *)*2000);
    keyList=malloc(sizeof(int *)*2000);
    keyval=input_file("key.txt",keyList);
    printf("_/_/_/_/_/_/_/_/_/_/_/\n一人でしりとり →  1\n終了 →  0\n_/_/_/_/_/_/_/_/_/_/_/\n");
    gets(c);
    m=atoi(c);
    while(m!=0){
        idList[i] = malloc(sizeof(int)*20);
        last_str=malloc(sizeof(char)*100);

        if(i!=0){
            itos(idList[i-1],last_str,val[i-1]);
            printf("%s",last_str);
            printf(" →  ");
        }
        if(input_word(idList[i],&val[i])) return 0;

        cnt++;
        printf("id = ");
        for(j=0;j<val[i];j++) printf("%d ",idList[i][j]);
        printf("\n");

        for(k=0;k<keyval;k++){
            for(j=0;j<val[i];j++){
                printf("%d == %d ?\n",idList[i][j],keyList[k][j]);
                if(idList[i][j]==keyList[k][j]) flag2=1;
            }
            if(flag2) flag=0;
        }
        if(flag){
            puts("out");
            return 0;
        }

        if((idList[i][0]!=lastid)&&cnt!=1){
            printf("%d回目で失敗・・・\n",cnt);
            return 0;
        }
        lastid=idList[i][val[i]-1];
        i++;
    }
    free(idList);
    return 0;
}


int input_file(char *fname,int** s){
    FILE *fp;
    int i=0,j=0,val=0;
    int *h;
    char **a,c;
    fp = fopen(fname, "r");
    if( fp == NULL ){
        printf( "%sファイルが開けません\n", fname );
        return -1;
    }
    a=malloc(sizeof(char**)*2000);
    a[0]=malloc(sizeof(char*)*100);
    while((c=fgetc(fp))!=EOF){
        if(c!='\n'){
            a[i][j]=c;
            j++;val++;
        }else{
            h=malloc(sizeof(int)*20);
            s[i]=malloc(sizeof(int*)*100);
            a[i][j]='\0';
            printf("input_file => [%s]\n",a[i]);
            if(stoi(a[i],h)){
                printf("stoi faild...\n");
                return 1;
            }
            if(checkStyle(h, (int)(val/3))){
                printf("Not Hiragana!!\n");
                return 1;
            }

            for(j=0;j<(int)(val/3);j++){
                s[i][j]=toid(h[j]);
            }
            free(a[i]);i++;j=0;val=0;
            a[i]=malloc(sizeof(char*)*100);
        }

    }
    free(h);
    fclose(fp);
    return i-1;
}

int input_word(int *list, int *val){
    int i,j;
    unsigned char *a=malloc(sizeof(hirar)*20);
    int *h=malloc(sizeof(int)*20);

    gets(a);
    *val = (int)(strlen(a)/3);
    printf("input %2X%2X%2X～～～\n",a[0]& 0x000000FF, a[1]& 0x000000FF, a[2]& 0x000000FF);
    if(stoi(a,h)){
        printf("stoi faild...\n");
        return 1;
    }
    printf("h=%X\n",h[0]);
    if(checkStyle(h, *val)){
        printf("Not Hiragana!!\n");
        return 1;
    }

    printf("%2X[%d]\n",h[*val-1],toid(h[*val-1]));

    for(i=0;i<*val;i++){
        list[i]=toid(h[i]);
    }
    free(h);
    free(a);
    return 0;
}



int toid(int h){
    int x0=0xE38181,x81=0xE381BF,x82=0xE38280;

    if(h==0xE383BC) return 83;

    h-=x0;
    if(h>62) h-=(x82-x81)-1;
    return h;
}

int tostr(int h){
    int x0=0xE38181,x81=0xE381BF,x82=0xE38280;

    if(h==83) return 0xE383BC;

    h+=x0;
    if(h>x81) h+=(x82-x81)-1;
    return h;
}


int stoi(unsigned char *a, int *h){
    int i;
    if(strlen(a)%3!=0) return 1;
    for(i=0;i<(int)(strlen(a)/3);i++){
        h[i]+=a[0+3*i]<<16; printf("%2X\n",a[0+3*i]<<16);
        h[i]+=a[1+3*i]<<8; printf("%2X\n",a[1+3*i]<<8);
        h[i]+=a[2+3*i]; printf("%2X\n",a[2+3*i]);
    }
    return 0;
}

int itos(int *h, unsigned char *a, int size){
    int i;
    for(i=0;i<size;i++){
        a[0+3*i]=tostr(h[i])>>16&0x000000FF;
        a[1+3*i]=tostr(h[i])>>8&0x000000FF;
        a[2+3*i]=tostr(h[i])&0x000000FF;
        printf("itos a= [%2X%2X%2X]\n",a[0+3*i]& 0x000000FF, a[1+3*i]& 0x000000FF, a[2+3*i]& 0x000000FF);
    }
    a[3+3*(i-1)]='\0';
    return 0;
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

