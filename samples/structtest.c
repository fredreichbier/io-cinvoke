#include <stdio.h>

typedef struct {
	int a;
	char* b;
} MyStruct;

int testme(MyStruct* s) {
	printf("My b value is: %s\n", s->b);
//	printf("My a value is: %d!\n", s->a);
	return s->a;
}

typedef void (* TestFunc());

void test_callback(TestFunc func) {
	func(123);
}
