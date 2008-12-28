#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int a;
	char* b;
} MyStruct;

int test_struct(MyStruct* s) {
	printf("My a value is: %d\n", s->a);
	printf("My b value is: %s\n", s->b);
	return s->a + 1;
}

MyStruct *create_struct(int a) {
    MyStruct *mys = malloc(sizeof(MyStruct));
    mys->a = a;
    mys->b = "Hello From C!";
    return mys;
}

typedef int (*TestFunc)();

void test_callback(TestFunc func) {
	int i = func(123);
	printf("That should be 456: %d\n", i);
}
