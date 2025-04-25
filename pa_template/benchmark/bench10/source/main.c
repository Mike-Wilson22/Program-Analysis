#include <stdio.h>

void foo(int *x) { *x = 10;}

typedef void (*FP)(int *x);

FP f = foo;

int main(int argc, char** argv) {   
    f (&argc);
    return 0;
}

void valid_function() {
    int x = 1;
    return;
}

int main2(int argc, char** argv) {
    valid_function();
    return 0;
}


void dead_function() {
    int y = 2;
    return;
}