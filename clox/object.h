//
// Created by Brucey on 06/07/2024.
//

#ifndef OBJECT_H
#define OBJECT_H

#include "chunk.h"
#include "common.h"
#include "value.h"

#define OBJ_TYPE(value)     (AS_OBJ(value)->type)

#define IS_FUNCTION(value)  isObjType(value, OBJ_FUNCTION)
#define IS_STRING(value)    isObjType(value, OBJ_STRING)

#define AS_FUNCTION(value)  ((ObjFunction*)AS_OBJ(value))
#define AS_STRING(value)    ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)   (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
    OBJ_FUNCTION,
    OBJ_STRING,
} ObjType;

struct Obj {
    ObjType type;
    struct Obj *next;
};

typedef struct {
    Obj obj;
    int arity;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

struct ObjString {
    Obj obj;
    bool ownsChars;
    int length;
    uint32_t hash;
    char chars[];
};

ObjFunction* newFunction();
ObjString *makeString(bool ownsChars, char* chars, int length);
void printObject(Value value);

static inline bool isObjType(const Value value, const ObjType type) {
    return IS_OBJ(value) && OBJ_TYPE(value) == type;
}

#endif //OBJECT_H
