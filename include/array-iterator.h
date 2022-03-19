#ifndef _ARRAY_ITERATOR_
#define _ARRAY_ITERATOR_

#include "any.h"
#include "array-node.h"
#include "class.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct {
    ArrayNode* root;
} ArrayIterator;

CONSTRUCTOR(ArrayIterator, ArrayNode* root);

void PRIVATE(ArrayIterator, advance);

any PUBLIC(ArrayIterator, next);
ArrayNode* PUBLIC(ArrayIterator, nextNode);

bool PUBLIC(ArrayIterator, hasNext);

#endif