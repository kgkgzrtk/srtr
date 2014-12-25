#include <stdio.h>
#include <stdlib.h>

int main(){
    int i,x=0x8181,*n=calloc(5,sizeof(int));
    char *a;
/*
    a=(char*)malloc(4);

    for(i=0; x<=0x8293; ++i){
        a[0]=0xE3;
        a[1]=(x>>8)&0xff;
        a[2]=(x)&0xff;
        a[3]='\0';
        printf("%s[%2X%2X%2X] id = %d\n", a,a[0]& 0x000000FF, a[1]& 0x000000FF, a[2]& 0x000000FF,i+1);
        if(x==0x81BF) x=0x8280; else x++;
    }

    printf("\n");
*/

/*
    char *c=malloc(sizeof(char)*100);
    scanf("%d",&n[0]);
    scanf("%d",&n[1]);
    scanf("%d",&n[2]);
    itos(n,c);
    printf("%s",c);

    free(c);
    */
    struct timeval tv;
    gettimeofday(&tv,NULL);
    init_genrand((unsigned)tv.tv_usec);
    for(i=0;i<9;i++) printf("%d ",randd(0,9));
    printf("\n");
    return 0; 
}
