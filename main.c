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
#define CNT_MAX 40
#define AILV 1
#define FILENAME1 "pokemon.txt"
#define FILENAME2 "kokumei.txt"

pthread_t th1,th2;
int timeflag=0;
int tscore=0;
int score=0;

void* getWord(void* str){
        pthread_testcancel();
        getstr((char *)str);
        return NULL;
}

void* timer(void* args){
    int i=0,k;
    char *pro=(char *)malloc(sizeof(char)*C_MAX);
    for(k=0;k<CNT_MAX;k++){
        pro[k]='#';
    }
    pro[k]='\0';
    k--;
    while(i<CNT_MAX){
        tscore=CNT_MAX-i;
        printf("\e]0;%s\007",pro);
        refresh();
        if(i>CNT_MAX-4) flash();
        pro[k]='\0';
        pthread_testcancel();
        napms(500);
        k--;i++;
    }
    pthread_cancel(th1);
    timeflag=1;
    return 0;
}

int user_input(char *str){
    pthread_create( &th1, NULL, getWord, (void*)str );
    pthread_create( &th2, NULL, timer, (void *)NULL );
    pthread_join(th1, NULL);
    pthread_cancel(th2);
    return 0; 
}

void pri_ai(int y,int x,char *c){
    int i;
    curs_set(0);
    noecho();
    napms(500);
    for(i=0;i<strlen(c)/3;i++){
        mvprintw(y,x+2*i-3,"%c%c%c",c[3*i],c[3*i+1],c[3*i+2]);
        refresh();
        napms(500);
    }
    curs_set(1);
    echo();
}

int main(){

    int **idList;
    int **keyList;
    int *buf;
    int *p;
    int *pre_id;
    int i=1,j,mflag=0,eflag=0;
    char *instr,*pre;
    char *filename;

    int  x, y, w, h;
    char *str="し　り　ト　レ";
    char *str1="Please press <ESC> to exit...";
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
    char *str12="最後の文字が「ん」です";
    char *str13="YOU WIN! ( ^ - ^ )";
    char *str14="YOU LOSE... (. _ . )";
    char *str15="ジャンル選択";
    char *str16="ポケモン";
    char *str17="　国名　";
    int key;


    setlocale(LC_ALL,"");
    printf("\e]0;しりトレ\007");

    initscr();

    start_color();
    use_default_colors();
    init_pair(1,COLOR_RED,-1);
    init_pair(2,-1,COLOR_BLUE);

    noecho();
    cbreak();
    raw();
    curs_set(0);
    keypad(stdscr, TRUE);

    clear();

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
    y=cy;
    while(1){
        erase();
        mvprintw(cy-3,cx-strlen(str15)/2+3,str15);
        mvprintw(cy+1,cx-10,str16);
        mvprintw(cy+2,cx-10,str17);
        mvprintw(y+1,cx-strlen(str5)+10,str5);
        refresh();

        key = getch();
        if (key == 'q'){
            endwin();
            return 0;
        }
        if (key == 10 && y==cy){
            filename=FILENAME1;
            break;
        }else if(key==10 && y==cy+1){
            filename=FILENAME2;
            break;
        }
        switch (key) {
            case KEY_UP:	y=cy; break;
            case KEY_DOWN:	y=cy+1; break;
        }
    }

    idList=malloc(sizeof(int *)*K_MAX);
    keyList=malloc(sizeof(int *)*K_MAX);
    if(input_file(filename,keyList)){
        endwin();
        printf("ファイルが開けません\n");
        free(keyList);
        free(idList);
        return 0;
    }

    echo();
    curs_set(1);

    pre_id=calloc(I_MAX,sizeof(int));
    srtr_ai(NULL,keyList,idList,AILV,pre_id);
    pre=malloc(sizeof(char)*C_MAX);
    itos(pre_id,pre);
    idList[0]=calloc(I_MAX,sizeof(int));
    intcpy(idList[0],pre_id);
    score=0;
    key=0;

    while(1){

        if(i%2!=0){
            bkgdset(COLOR_PAIR(0));
            idcok(stdscr,true);
        }
        else{
            bkgdset(A_REVERSE);
            idcok(stdscr,false);
        }
        flushinp();
        erase();
        mvprintw(cy+2,cx-11,"SCORE:%d",score);
        mvprintw(cy,cx-strlen(pre)/2-10,pre);
        mvprintw(cy,cx-strlen(str7)/2-5,str7);
        refresh();

        idList[i]=(int *)calloc(I_MAX,sizeof(int));
        instr=(char *)malloc(sizeof(char)*C_MAX);
        instr=malloc(sizeof(char)*C_MAX);
        if(!mflag){
            user_input(instr);
        }else{
            if(i%2!=0){
                user_input(instr);
            }else{
                p=(int *)calloc(I_MAX,sizeof(int));
                pre=malloc(sizeof(char)*C_MAX);
                pthread_create( &th2, NULL, timer, (void *)NULL );
                if(srtr_ai(pre_id,keyList,idList,AILV,p)) instr="もうむり\0";
                else itos(p,instr);
                pthread_cancel(th2);
                pri_ai(cy,cx,instr);
            }
        }
        buf=(int *)calloc(I_MAX,sizeof(int));
        while(input_word(instr,buf)!=0&&!timeflag){
            instr=malloc(sizeof(char)*C_MAX);
            erase();
            mvprintw(cy,cx-strlen(pre)/2-10,pre);
            mvprintw(cy,cx-strlen(str7)/2-5,str7);
            refresh();
            user_input(instr);
        }
        pthread_cancel(th2);
        if(i%2!=0) score+=tscore;

        if(timeflag==1){
            eflag=1;
            break;
        }
        else{
            char *tes=malloc(sizeof(char)*C_MAX);
            itos(idList[0],tes);
            switch(judgeStr(pre_id,buf,idList,keyList)){
                case 0: eflag=0; break;
                case 1: eflag=2; break;
                case 2: eflag=3; break;
                case 3: eflag=4; break;
                case 4: eflag=5; break;
                default : eflag=-1; break;
            }
            if(eflag!=0) break;
        }

        j=0;
        intcpy(idList[i],buf);

        pre=malloc(sizeof(char)*C_MAX);
        strcpy(pre,instr);
        pre_id=calloc(I_MAX,sizeof(int));
        intcpy(pre_id,idList[i]);
        i++;
    }
    timeout(-1);
    while(1){
        erase();
        if(mflag){
            if(i%2==0){
                bkgdset(A_NORMAL);
                attrset(COLOR_PAIR(1));
                clear();
                mvprintw(cy-2,cx-strlen(str13)/2,str13);
                mvprintw(cy+4,cx-5,"Score:%d",score);
            }else{
                bkgdset(A_REVERSE);
                clear();
                attron(COLOR_PAIR(2));
                mvprintw(cy-2,cx-strlen(str14)/2,str14);
            }
            attrset(COLOR_PAIR(0));
        }
        switch(eflag){
            case 1: mvprintw(cy,cx-strlen(str8)/2+4,str8); break;
            case 2: mvprintw(cy,cx-strlen(str9)/2+4,str9,instr); break;
            case 3: mvprintw(cy,cx-strlen(str10)/2+4,str10,instr); break;
            case 4: mvprintw(cy,cx-strlen(str11)/2+8,str11,instr); break;
            case 5: mvprintw(cy,cx-strlen(str12)/2+8,str12,instr); break;
            default: mvprintw(cy,cx-strlen("ERROR")/2+8,"ERROR"); break; 
        }
        mvprintw(cy+1,cx-strlen(str1)/2,str1);
        refresh();
        key = getch();
        if(key==27) break;
    }
    endwin();
    free(keyList);
    free(idList);
    return 0;
}
