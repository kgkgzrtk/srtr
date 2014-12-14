#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

	int** idList;
	int** keyList;
	int i=0,j,k,m,cnt=0, slen=0, *val, lastid, flag=1,flag2=0, keyval;
	char *c;
	unsigned char *last_str;
	
	c=malloc(sizeof(char)*10);
	val=malloc(sizeof(int)*2000);
	idList=malloc(sizeof(int *)*2000);
	keyList=malloc(sizeof(int *)*2000);
	keyval=input_file("key.txt",keyList);
	printf("_/_/_/_/_/_/_/_/_/_/_/\n一人でしりとり →  1\n終了 →  0\n_/_/_/_/_/_/_/_/_/_/_/\n");	
	free(idList);
	return 0;
}
