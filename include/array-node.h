#ifndef _ARRAY_NODE_
#define _ARRAY_NODE_

#include "any.h"
#include "class.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct ArrayNode {
    struct ArrayNode *previous, *next;
    any value;
} ArrayNode;

CONSTRUCTOR(ArrayNode);
DESTRUCTOR(ArrayNode);

ArrayNode* PUBLIC(ArrayNode, getPrevious);
ArrayNode* PUBLIC(ArrayNode, getNext);

void STATIC(ArrayNode, bind, ArrayNode* previous, ArrayNode* next);

#endif