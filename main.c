#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <locale.h>
#include <pthread.h>

#define CNT_MAX 5

int endflag=0;
pthread_mutex_t mutex;

void* thread(void* args){
    int i=0,x,y,h,w;
    while(i<CNT_MAX){
        getmaxyx(stdscr,h,w);
        getyx(stdscr,y,x);
        mvprintw(4,w-5,"%d",CNT_MAX-i);
        move(y,x);
        refresh();
        pthread_testcancel();
        sleep(1);
        i++;
    }
    timeout(1);
    pthread_mutex_lock( &mutex );
    endflag=1;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(){

    int** idList;
    int** keyList;
    int* buf,p;
    int i=0,j,mflag=0,eflag=0;
    char *c,*pre;
    pthread_t th;
    pthread_mutex_init( &mutex, NULL );

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
    idList=malloc(sizeof(int *)*2000);
    keyList=malloc(sizeof(int *)*2000);
    if(input_file("key.txt",keyList)){
        endwin();
        free(keyList);
        free(idList);
        return 0;
    }
    nocbreak();
    echo();
    curs_set(1);
    p=(int *)calloc(100,sizeof(int));
    pre=malloc(sizeof(char)*100);
    pre="しりとり";
    if(mflag) pre="CPU";
    while(1){
        erase();
        mvprintw(cy,cx-strlen(pre)/2-10,pre);
        mvprintw(cy,cx-strlen(str7)/2-5,str7);
        refresh();

        pthread_create( &th, NULL, thread, (void *)NULL );
        c=malloc(sizeof(char)*100);
        if(!mflag) scanw("%s",c);
        else{
            if(i%2==0) scanw("%s",c);
            else{
                srtr_ai(idList[i-1],keyList,idList,1,p);
                itos(p,c);
            }
        }

        buf=(int *)calloc(100,sizeof(int));
        while(input_word(c,buf)!=0){
            c=malloc(sizeof(char)*100);
            erase();
            mvprintw(cy,cx-strlen(pre)/2-10,pre);
            mvprintw(cy,cx-strlen(str7)/2-5,str7);
            refresh();
            if(!mflag) scanw("%s",c);
            else{
                if(i%2!=0) scanw("%s",c);
                else{
                    srtr_ai(idList[i-1],keyList,idList,1,p);
                    itos(p,c);
                    break;
                }
            }
        }
        pthread_cancel(th);

        idList[i]=(int *)calloc(100,sizeof(int));
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
        pre=malloc(sizeof(char)*100);
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
    pthread_mutex_destroy( &mutex );
    free(keyList);
    free(idList);
    return 0;
}
