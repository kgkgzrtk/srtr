libsrtr.so: srtr.c
	gcc -g -O0 -shared srtr.c -o libsrtr.so -fPIC
main: main.c
	gcc main.c -o main ./libsrtr.so
all:
	gcc -shared srtr.c -o libsrtr.so -fPIC
	gcc -g main.c -o main ./libsrtr.so -lncursesw -lpthread
sudo:
	sudo gcc -shared srtr.c -o libsrtr.so -fPIC
	sudo gcc main.c -o main ./libsrtr.so -lncursesw -lpthread
test:
	gcc -shared srtr.c -o libsrtr.so -fPIC
	gcc -g hira_test.c -o test ./libsrtr.so
