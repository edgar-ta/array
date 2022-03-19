#include "../include/array.h"

// START::CONTROL-METHODS::START
CONSTRUCTOR(Array) {
    MALLOC(Array);
    __PRIVATE(Array, defaultInit)(instance);
    return instance;
}

DESTRUCTOR(Array) {
    __PUBLIC(Array, clear)(self);
    free(self);
}

void PRIVATE(Array, init, ArrayNode* head, ArrayNode* tail, ArrayNode* current, int length, int index) {
    self->head = head;
    self->tail = tail;
    self->current = current;
    self->length = length;
    self->index = index;
}

void PRIVATE(Array, defaultInit) {
    __PRIVATE(Array, init)(self, NULL, NULL, NULL, 0, -1);
}

void PRIVATE(Array, moveBy, ArrayNode* node, int initialIndex, int offset) {
    bool advance = offset > 0;
    ArrayNode* (*slipper)(ArrayNode*) = advance? __PUBLIC(ArrayNode, getNext): __PUBLIC(ArrayNode, getPrevious);
    int absoluteOffset = advance? offset: -offset;
    int i;
    for (i = 0; i < absoluteOffset; i++) {
        node = slipper(node);
    }
    self->index = initialIndex + offset;
    self->current = node;
}

void PRIVATE(Array, moveTo, int dynamicIndex) {
    int parsedIndex = __PUBLIC(Array, parseIndex)(self, dynamicIndex);

    if (!__PUBLIC(Array, inBounds)(self, parsedIndex)) {
        print("Index out of bounds: %d", parsedIndex);
        exit(1);
        return;
    }

    int offsetFromHead = parsedIndex;
    int offsetFromTail = parsedIndex - self->length + 1;
    int offsetFromCurrent = parsedIndex - self->index;

    bool first = __PUBLIC(Array, lesserByMagnitude)(offsetFromHead, offsetFromTail);
    bool second = __PUBLIC(Array, lesserByMagnitude)(offsetFromTail, offsetFromCurrent);
    // makes more sense that this comparison tells whether or not to use the current node
    if (!first && second) __PRIVATE(Array, moveBy)(self, self->tail, self->length - 1, offsetFromTail);
    else {
        bool third = __PUBLIC(Array, lesserByMagnitude)(offsetFromHead, offsetFromCurrent);
        if (first && third) __PRIVATE(Array, moveBy)(self, self->head, 0, offsetFromHead);
        else __PRIVATE(Array, moveBy)(self, self->current, self->index, offsetFromCurrent);
    }
}

ArrayIterator* PUBLIC(Array, iterator) {
    ArrayIterator* instance = __CONSTRUCT(ArrayIterator)(self->head);
    return instance;
}

// END::CONTROL-METHODS::END
// -------------------------
// START::UTILS::START
bool PUBLIC(Array, empty) {
    return self->length == 0;
}

bool PUBLIC(Array, single) {
    return self->length == 1;
}

bool PUBLIC(Array, inBounds, int index) {
    return index > -1 && index < self->length;
}

int PUBLIC(Array, parseIndex, int index) {
    if (index < 0) return self->length + index;
    return index;
}

bool STATIC(Array, lesserByMagnitude, int a, int b) {
    int absoluteA = abs(a);
    int absoluteB = abs(b);
    return absoluteA < absoluteB;
}

// END::UTILS::END
// ---------------
// START::STANDARD-METHODS::START

void PUBLIC(Array, push, any value) {
    ArrayNode* arrayNode = __CONSTRUCT(ArrayNode)(value);
    __PUBLIC(ArrayNode, bind)(self->tail, arrayNode);
    if (__PUBLIC(Array, empty)(self)) {
        self->head = arrayNode;
        self->current = arrayNode;
        self->index = 0;
    }
    self->tail = arrayNode;
    self->length++;
}

void PUBLIC(Array, unshift, any value) {
    ArrayNode* arrayNode = __CONSTRUCT(ArrayNode)(value);
    __PUBLIC(ArrayNode, bind)(arrayNode, self->head);
    if (__PUBLIC(Array, empty)(self)) {
        self->tail = arrayNode;
        self->current = arrayNode;
        self->index = 0;
    }
    self->head = arrayNode;
    self->length++;
}

any PUBLIC(Array, get, int dynamicIndex) {
    __PRIVATE(Array, moveTo)(self, dynamicIndex);
    return self->current->value;
}

void PUBLIC(Array, set, int dynamicIndex, any value) {
    __PRIVATE(Array, moveTo)(self, dynamicIndex);
    self->current->value = value;
}

void PUBLIC(Array, remove, int dynamicIndex) {
    __PRIVATE(Array, moveTo)(self, dynamicIndex);
    ArrayNode* previous = self->current->previous;
    ArrayNode* next = self->current->next;
    __PUBLIC(ArrayNode, bind)(previous, next);
    bool usePrevious = previous != NULL;
    self->current = usePrevious? previous: next;
    self->index += usePrevious? -1: 1;
    self->length--;
}

void PUBLIC(Array, clear) {
    ArrayIterator* iterator = __PUBLIC(Array, iterator)(self);
    while (__PUBLIC(ArrayIterator, hasNext)(iterator)) {
        ArrayNode* current = __PUBLIC(ArrayIterator, nextNode)(iterator);
        __PUBLIC(ArrayNode, destroy)(current);
    }
    __PUBLIC(ArrayIterator, destroy)(iterator);
    self->head = NULL;
    self->tail = NULL;
    self->current = NULL;
    self->length = 0;
    self->index = -1;
}

// END::STANDARD-METHODS::END
// --------------------------
// START::JAVASCRIPT-LIKE-METHODS::START

Array* PUBLIC(Array, reduce, ArrayReduceCallback callback, any initialValue) {
    any previous = initialValue;
    ArrayIterator* iterator = __PUBLIC(Array, iterator)(self);
    int i = 0;
    while (__PUBLIC(ArrayIterator, hasNext)(iterator)) {
        any current = __PUBLIC(ArrayIterator, next)(iterator);
        previous = callback(previous, current, i++);
    }
    __PUBLIC(ArrayIterator, destroy)(iterator);
}

Array* PUBLIC(Array, map, ArrayCallback callback) {
    Array* mapped = __CONSTRUCT(Array)();
    ArrayIterator* iterator = __PUBLIC(Array, iterator)(self);
    int i = 0;
    while (__PUBLIC(ArrayIterator, hasNext)(iterator)) {
        any value = __PUBLIC(ArrayIterator, next)(iterator);
        any result = callback(value, i++);
        __PUBLIC(Array, push)(self, result);
    }
    __PUBLIC(ArrayIterator, destroy)(iterator);
    return mapped;
}

any PUBLIC(Array, find, ArrayCallback callback) {
    callback = (bool (*)(any, int)) callback;
    ArrayIterator* iterator = __PUBLIC(Array, iterator)(self);
    int i = 0;
    while (__PUBLIC(ArrayIterator, hasNext)(iterator)) {
        any current = __PUBLIC(ArrayIterator, next)(iterator);
        if (callback(current, i++)) {
            return current;
        }
    }
    __PUBLIC(ArrayIterator, destroy)(iterator);
    return NULL;
}

bool PUBLIC(Array, some, ArrayCallback callback) {
    callback = (bool (*)(any, int)) callback;
    ArrayIterator* iterator = __PUBLIC(Array, iterator)(self);
    int i = 0;
    while (__PUBLIC(ArrayIterator, hasNext)(iterator)) {
        any current = __PUBLIC(ArrayIterator, next)(iterator);
        if (callback(current, i++)) {
            return true;
        }
    }
    __PUBLIC(ArrayIterator, destroy)(iterator);
    return false;
}

bool PUBLIC(Array, every, ArrayCallback callback) {
    callback = (bool (*)(any, int)) callback;
    ArrayIterator* iterator = __PUBLIC(Array, iterator)(self);
    int i = 0;
    while (__PUBLIC(ArrayIterator, hasNext)(iterator)) {
        any current = __PUBLIC(ArrayIterator, next)(iterator);
        if (!callback(current, i++)) {
            return false;
        }
    }
    __PUBLIC(ArrayIterator, destroy)(iterator);
    return true;
}

void PUBLIC(Array, forEach, ArrayCallback callback) {
    callback = (void (*)(any, int)) callback;
    ArrayIterator* iterator = __PUBLIC(Array, iterator)(self);;
    int i = 0;
    while (__PUBLIC(ArrayIterator, hasNext)(iterator)) {
        any current = __PUBLIC(ArrayIterator, next)(iterator);
        callback(current, i++);
    }
    __PUBLIC(ArrayIterator, destroy)(iterator);
}
// END::JAVASCRIPT-LIKE-METHODS::END
// ---------------------------------
// START::ANOTHER-METHODS::START

Array* STATIC(Array, generate, ArrayGenerateCallback callback, int start, int stop, int step) {
    Array* instance = __CONSTRUCT(Array)();
    int i;
    for (i = start; i < stop; i += step) {
        any value = callback(i);
        __PUBLIC(Array, push)(instance, value);
    }
    return instance;
}

// END::ANOTHER-METHODS::END