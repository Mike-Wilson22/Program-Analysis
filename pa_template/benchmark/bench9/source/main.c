#include <stdio.h>

void called_function() {
    int x = 1;
}

int main() {
    int a = 5, b = 1;
    int *p;
    
    p = &a;
    p = &b;
    called_function();
    return *p;
}


