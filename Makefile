libsrtr.so: srtr.c
	gcc -shared srtr.c -o libsrtr.so -fPIC
main: main.c
	gcc main.c -o main ./libsrtr.so
