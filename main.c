#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <locale.h>
#include <pthread.h>

#define C_MAX 301
#define I_MAX 100
#define K_MAX 2000
#define CNT_MAX 20
#define FILENAME "key.txt"

int endflag=0;

void* thread(void* args){
    int i=0,k;
    char *pro=(char *)malloc(sizeof(char)*C_MAX);
    for(k=0;k<CNT_MAX;k++){
        pro[k]='#';
    }
    pro[k]='\0';
    k--;
    while(i<CNT_MAX){
        pthread_testcancel();
        printf("\e]0;%s\007",pro);
        refresh();
        if(i>CNT_MAX-4) flash();
        pro[k]='\0';
        napms(500);
        k--;i++;
    }
    timeout(1);
    endflag=1;
    return NULL;
}

void pri_ai(int y,int x,char *c){
    int i;
    curs_set(0);
    raw();
    noecho();
    cbreak();
    napms(500);
    for(i=0;i<strlen(c)/3;i++){
        mvprintw(y,x+2*i,"%c%c%c",c[3*i],c[3*i+1],c[3*i+2]);
        refresh();
        napms(500);
    }
    curs_set(1);
    noraw();
    echo();
    nocbreak();
}

int main(){

    int** idList;
    int** keyList;
    int* buf;
    int *p;
    int i=0,j,mflag=0,eflag=0;
    char *c,*pre;
    pthread_t th;

    int  x, y, w, h;
    char *str="し　り　ト　レ";
    char *str2="Please press <ENTER> to start!";
    char *str3="一人でしりとり";
    char *str4="ＣＰＵと対戦！";
    char *str5=" <= ";
    char *str6="しりとり";
    char *str7=" => ";
    char *str8="タイムアウトー！";
    char *str9="残念、「%s」はありません";
    char *str10="残念、「%s」は使用済みです";
    char *str11="最後と最初の文字が合ってないです";
    char *str12="Please press <ESC> to exit...";
    int key;

    setlocale(LC_ALL,"");
    printf("\e]0;しりトレ\007");
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);

    getmaxyx(stdscr, h, w);
    y = h/2;
    x = w/2;
    const int cy=y;
    const int cx=x;

    while (1){
        erase();
        mvprintw(cy-1,cx-strlen(str)/2+3,str);
        mvprintw(cy+1,cx-strlen(str2)/2,str2);
        key=getch();
        if(key==10) break;
    }
    while(1){
        erase();
        mvprintw(cy-3,cx-strlen(str)/2+3,str);
        mvprintw(cy+1,cx-10,str3);
        mvprintw(cy+2,cx-10,str4);
        mvprintw(y+1,cx-strlen(str5)+10,str5);
        refresh();

        key = getch();
        if (key == 'q'){
            endwin();
            return 0;
        }
        if (key == 10 && y==cy){
            break;
        }else if(key==10 && y==cy+1){
            mflag=1;
            break;
        }
        switch (key) {
            case KEY_UP:	y=cy; break;
            case KEY_DOWN:	y=cy+1; break;
        }
    }

    idList=malloc(sizeof(int *)*K_MAX);
    keyList=malloc(sizeof(int *)*K_MAX);
    if(input_file(FILENAME,keyList)){
        endwin();
        free(keyList);
        free(idList);
        return 0;
    }

    nocbreak();
    echo();
    curs_set(1);
    pre=malloc(sizeof(char)*C_MAX);
    pre="しりとり";
    if(mflag) pre="CPU";
    while(1){
        flushinp();
        erase();
        mvprintw(cy,cx-strlen(pre)/2-10,pre);
        mvprintw(cy,cx-strlen(str7)/2-5,str7);
        refresh();

        pthread_create( &th, NULL, thread, (void *)NULL );
        c=malloc(sizeof(char)*C_MAX);
        if(!mflag) scanw("%s",c);
        else{
            if(i%2==0) scanw("%s",c);
            else{
                p=(int *)calloc(I_MAX,sizeof(int));
                pre=malloc(sizeof(char)*C_MAX);
                if(srtr_ai(idList[i-1],keyList,idList,1,p)) c="もうむり\0";
                else itos(p,c);
                pri_ai(cy,cx,c);
            }
        }

        buf=(int *)calloc(I_MAX,sizeof(int));
        while(input_word(c,buf)!=0&&!endflag){
            c=malloc(sizeof(char)*C_MAX);
            erase();
            mvprintw(cy,cx-strlen(pre)/2-10,pre);
            mvprintw(cy,cx-strlen(str7)/2-5,str7);
            refresh();
            scanw("%s",c);
        }
        pthread_cancel(th);

        idList[i]=(int *)calloc(I_MAX,sizeof(int));
        if(endflag==1){
            eflag=1;
            break;
        }
        else if(searchid(buf,keyList)){
            eflag=2;
            break;
        }
        else if(!searchid(buf,idList)){
            eflag=3;
            break;
        }
        else if(i!=0&&!match_id(idList[i-1],buf)){
            eflag=4;
            break;
        }
        j=0;
        while(buf[j]!=0){
            idList[i][j]=buf[j]; j++;
        }

        i++;
        pre=malloc(sizeof(char)*C_MAX);
        strcpy(pre,c);
    }
    cbreak();
    timeout(-1);
    while(1){
        erase();
        switch(eflag){
            case 1: mvprintw(cy,cx-strlen(str8)/2+4,str8); break;
            case 2: mvprintw(cy,cx-strlen(str9)/2+4,str9,c); break;
            case 3: mvprintw(cy,cx-strlen(str10)/2+4,str10,c); break;
            case 4: mvprintw(cy,cx-strlen(str11)/2+8,str11,c); break;
            default: mvprintw(cy,cx-strlen("ERROR")/2+8,"ERROR"); break; 
        }
        mvprintw(cy+1,cx-strlen(str12)/2,str12);
        refresh();
        key = getch();
        if(key==27) break;
    }
    endwin();
    free(keyList);
    free(idList);
    return 0;
}
