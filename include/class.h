#ifndef _CLASS_
#define _CLASS_

// for the MALLOC macro
#include <stdlib.h>

/** concatenates two strings **/
#define __CONCAT(a, b) a## ##b

/** allocates an instance **/
#define MALLOC(className) className* instance = (className*) malloc(sizeof(className))

/** name of public method **/
#define __PUBLIC(className, methodName) className##_##methodName
/** name of hidden method **/
#define __PRIVATE(className, methodName) __CONCAT(__##className, _##methodName)
/** parameter list that adds a "self" parameter; a "self list" **/
#define __SLFLST(className, ...) (className* self, ##__VA_ARGS__)

/** defines a public method **/
#define PUBLIC(className, methodName, ...) __PUBLIC(className, methodName) __SLFLST(className, ##__VA_ARGS__)
/** defines a static method **/
#define STATIC(className, methodName, ...) __PUBLIC(className, methodName)(__VA_ARGS__)
/** defines a static method **/
#define PRIVATE(className, methodName, ...) __PRIVATE(className, methodName) __SLFLST(className, ##__VA_ARGS__)

/** name of a constructor **/
#define __CONSTRUCT(className) className##_new
/** name of a destructor **/
#define __DESTRUCT(className) className##_destroy

#define CONSTRUCTOR(className, ...) className* __CONSTRUCT(className)(__VA_ARGS__)
#define DESTRUCTOR(className, ...) void __DESTRUCT(className) __SLFLST(className, ##__VA_ARGS__)
#define EXTENDS(className) typedef struct { className* className; } Extends##className

/** "default constructor" **/
#define DEF_CONSTRUCTOR(className) CONSTRUCTOR(className) { MALLOC(className); return instance; }
/** "default destructor" **/
#define DEF_DESTRUCTOR(className) DESTRUCTOR(className) { free(self); }

#endif