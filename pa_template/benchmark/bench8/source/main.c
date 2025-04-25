#include <stdio.h>

void foo(int *x) { *x = 10;}

void bar(int *x) { *x = 20;}

void execute(void (*fptr)(int *), int *y) {
    fptr(y);
}

void dead_function() {
    int x = 1;
    return;
}

typedef void (*FP)(int *x);
int main() {
    int a = 5, b = 15;
    FP f = foo;
    execute(f, &a);
    f = bar;
    execute(f, &b);
    return 0;
}