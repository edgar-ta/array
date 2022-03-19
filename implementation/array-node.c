#include "../include/array-node.h"

CONSTRUCTOR(ArrayNode, any value) {
    MALLOC(ArrayNode);
    instance->value = value;
    instance->previous = NULL;
    instance->next = NULL;
    return instance;
}

DEF_DESTRUCTOR(ArrayNode);

ArrayNode* PUBLIC(ArrayNode, getPrevious) {
    return self->previous;
}

ArrayNode* PUBLIC(ArrayNode, getNext) {
    return self->next;
}

void STATIC(ArrayNode, bind, ArrayNode* previous, ArrayNode* next) {
    if (previous != NULL) previous->next = next;
    if (next != NULL) next->previous = previous;
}