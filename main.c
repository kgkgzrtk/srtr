#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <locale.h>
#include <pthread.h>

int tflag=0,sflag=0;

void* thread(void* args){
		int i=0;
		while(1){
			pthread_testcancel();
			sleep(1);
			if(i>3){
				timeout(0);
				tflag=1;
			}
			i++;
		}
		return NULL;
}

int main(){

	int** idList;
	int** keyList;
	int i=0,j,k,m,cnt=0, slen=0, *val, lastid, flag=1,flag2=0, keyval;
	char *c;
	unsigned char *last_str;
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
	char *str9="Please press <ENTER> to exit...";
	char *str10="「%s」はありません";
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
		if(key==10||key=='　') break;
	}
	while(1){
		erase();
		mvprintw(cy-3,cx-strlen(str)/2+3,str);
		mvprintw(cy+1,cx-10,str3);
		mvprintw(cy+2,cx-10,str4);
		mvprintw(y+1,cx-strlen(str5)+10,str5);
		refresh();

		key = getch();
		if (key == 'q') break;
		if (key == 10 && y==cy) break;
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
	while(1){
		erase();
		mvprintw(cy,cx-strlen(str6)/2-10,str6);
		mvprintw(cy,cx-strlen(str7)/2-5,str7);
		refresh();
		
		curs_set(1);
		c=malloc(sizeof(char)*100);
		pthread_create( &th, NULL, thread, (void *)NULL );
		scanw("%s",c);
		idList[i]=calloc(100,sizeof(int));
		while(input_word(c,idList[i])!=0){
			c=malloc(sizeof(char)*100);
			erase();
			mvprintw(cy,cx-strlen(str6)/2-10,str6);
			mvprintw(cy,cx-strlen(str7)/2-5,str7);
			refresh();
			scanw("%s",c);
		}
		pthread_cancel(th);
		if(searchid(idList[i],keyList)){
				sflag=1;
				break;
		}
		str6=malloc(sizeof(char)*100);
		strcpy(str6,c);
	}
	cbreak();
	curs_set(0);
    while(tflag){
		sflag=0;
		erase();
		mvprintw(cy,cx-strlen(str8)/2,str8);
		mvprintw(cy+1,cx-strlen(str9)/2,str9);
		refresh();
		key = getch();
		if(key==10) break;
	}

	while(sflag){
		erase();
		mvprintw(cy,cx-strlen(str10)/2,str10,c);
		mvprintw(cy+1,cx-strlen(str9)/2,str9);
		refresh();
		key = getch();
		if(key==10) break;
	}
	endwin();
	free(keyList);
	free(idList);
	return 0;
}
