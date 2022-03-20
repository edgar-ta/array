#ifndef _ARRAY_
#define _ARRAY_

#include "any.h"
#include "array-node.h"
#include "array-iterator.h"
#include "class.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct Array {
    ArrayNode *head, *tail, *current;
    int length, index;
} Array;

typedef any (*ArrayGenerateCallback)(int);
typedef any (*ArrayCallback)(any, int);
typedef any (*ArrayReduceCallback)(any, any, int);

// control methods
CONSTRUCTOR(Array);
DESTRUCTOR(Array);

void PRIVATE(Array, init, ArrayNode* head, ArrayNode* tail, ArrayNode* current, int length, int index);
void PRIVATE(Array, defaultInit);

void PRIVATE(Array, moveBy, ArrayNode* node, int initialIndex, int offset);
void PRIVATE(Array, moveTo, int dynamicIndex);

ArrayIterator* PUBLIC(Array, iterator);

// utils
bool PUBLIC(Array, empty);
bool PUBLIC(Array, single);
bool PUBLIC(Array, inBounds, int index);

int PUBLIC(Array, parseIndex, int dynamicIndex);

bool STATIC(Array, lesserByMagnitude, int a, int b);

// standard methods
void PUBLIC(Array, push, any value);
void PUBLIC(Array, unshift, any value);

any PUBLIC(Array, get, int dynamicIndex);
void PUBLIC(Array, set, int dynamicIndex, any value);

void PUBLIC(Array, remove, int dynamicIndex);
void PUBLIC(Array, clear);

// javascript-like methods
Array* PUBLIC(Array, reduce, ArrayReduceCallback callback, any initialValue);
Array* PUBLIC(Array, map, ArrayCallback callback);

any PUBLIC(Array, find, ArrayCallback callback);

bool PUBLIC(Array, some, ArrayCallback callback);
bool PUBLIC(Array, every, ArrayCallback callback);

void PUBLIC(Array, forEach, ArrayCallback callback);

// another methods
Array* STATIC(Array, generate, ArrayGenerateCallback callback, int start, int stop, int step);

#endif