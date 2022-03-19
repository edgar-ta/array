#include "include/array.h"
#include "include/array-iterator.h"
#include "include/array-node.h"
#include <stdarg.h>

void print(const char* text, ...) {
    va_list list;
    va_start(list, text);
    vprintf(text, list);
    putchar('\n');
    va_end(list);
}

int __randInt(int min, int max) {
    return min + (rand() % (max - min));
}

int randInt(int max) {
    return __randInt(0, max);
}

void printElement(any value, int index) {
    int integer = (int) value;
    print("The value of the element at index %d is %d", index, integer);
}

int main() {
    Array* array = Array_generate((ArrayGenerateCallback) &randInt, 10, 20, 1);
    Array_forEach(array, (ArrayCallback) &printElement);
    Array_destroy(array);
    return 0;
}