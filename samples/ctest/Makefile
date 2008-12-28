libtestlib.so: testlib.o
	gcc -shared -o libtestlib.so testlib.o

testlib.o: testlib.c
	gcc -g -Wall -fpic -c testlib.c -o testlib.o

