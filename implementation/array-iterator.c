#include "../include/array-iterator.h"

CONSTRUCTOR(ArrayIterator, ArrayNode* root) {
    MALLOC(ArrayIterator);
    instance->root = root;
    return instance;
}

DEF_DESTRUCTOR(ArrayIterator);

void PRIVATE(ArrayIterator, advance) {
    self->root = self->root->next;
}

any PUBLIC(ArrayIterator, next) {
    any value = self->root->value;
    __PRIVATE(ArrayIterator, advance)(self);
    return value;
}

ArrayNode* PUBLIC(ArrayIterator, nextNode) {
    ArrayNode* node = self->root;
    __PRIVATE(ArrayIterator, advance)(self);
    return node;
}

bool PUBLIC(ArrayIterator, hasNext) {
    return self->root != NULL;
}