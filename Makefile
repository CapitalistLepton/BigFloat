bin/euler: bin/bigfloat.o src/euler.c
	gcc -o bin/euler bin/bigfloat.o src/euler.c

bin/test: bin/bigfloat.o src/bigfloat_test.c
	gcc -o bin/test bin/bigfloat.o src/bigfloat_test.c -L/usr/local/lib -ltap

bin/bigfloat.o: src/bigfloat.c src/bigfloat.h make-bin
	gcc -c src/bigfloat.c -o bin/bigfloat.o

make-bin:
	mkdir -p bin/

clean:
	rm -rf bin/
