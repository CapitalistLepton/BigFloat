euler: bigfloat.o euler.c
	gcc -o euler bigfloat.o euler.c

test: bigfloat.o bigfloat_test.c
	gcc -o test bigfloat.o bigfloat_test.c -L/usr/local/lib -ltap

bigfloat.o: bigfloat.c bigfloat.h
	gcc -c bigfloat.c
