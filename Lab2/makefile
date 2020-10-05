all: *.c *.h
	gcc -o main main.c buffer.c -lpthread -lrt -ansi -pedantic

zip: *.c *.h makefile
	zip lab2 main.c buffer.c buffer.h makefile
