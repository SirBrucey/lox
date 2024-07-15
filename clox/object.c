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

static uint32_t hashString(const char* key, const int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint32_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

ObjString* makeString(const bool ownsChars, char* chars, const int length) {
    const uint32_t hash = hashString(chars, length);

    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    // Return reference when copying a string if its already interned.
    if (!ownsChars && interned) {
        return interned;
    }
    // If ownership is being taken and the string has been interned we can just free the string that was passed in.
    if (ownsChars && interned != NULL) {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }

    ObjString* string = (ObjString*)allocateObject(
        sizeof(ObjString) + length + 1, OBJ_STRING
    );
    string->ownsChars = ownsChars;
    string->length = length;

    memcpy(string->chars, chars, length);
    string->chars[length] = '\0';

    string->hash = hash;
    tableSet(&vm.strings, OBJ_VAL(string), NIL_VAL);

    return string;
}

void printObject(const Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING: {
            printf("%.*s", AS_STRING(value)->length, AS_CSTRING(value));
            break;
        }
    }
}
