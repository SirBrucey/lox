//
// Created by Brucey on 06/07/2024.
//

#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

static Obj *allocateObject(const size_t size, const ObjType type) {
    Obj *object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;

    object->next=vm.objects;
    vm.objects = object;

    return object;
}

ObjString* makeString(const int length) {
    ObjString* string = (ObjString*)allocateObject(
        sizeof(ObjString) + length + 1, OBJ_STRING
    );
    string->length = length;
    return string;
}

ObjString* copyString(const char* chars, const int length) {
    ObjString* string = makeString(length);

    memcpy(string->chars, chars, length);
    string->chars[length] = '\0';

    return string;
}

void printObject(const Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING: {
            printf("%s", AS_CSTRING(value));
            break;
        }
    }
}
