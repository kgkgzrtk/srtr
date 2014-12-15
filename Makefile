libsrtr.so: srtr.c
	gcc -shared srtr.c -o libsrtr.so -fPIC
main: main.c
	gcc main.c -o main ./libsrtr.so
all:
	gcc -shared srtr.c -o libsrtr.so -fPIC
	gcc -g main.c -o main ./libsrtr.so -lncursesw -lpthread