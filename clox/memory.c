//
// Created by Brucey on 08/05/2024.
//
#include <stdlib.h>

#include "memory.h"

#include "object.h"
#include "vm.h"

void * reallocate(void *pointer, size_t oldSize, const size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;
}

void freeObject(Obj *object) {
    switch (object->type) {
        case OBJ_STRING: {
            const ObjString *string = (ObjString*)object;
            reallocate(object, sizeof(ObjString) + string->length + 1, 0);
            break;
        }
    }
}

void freeObjects(void) {
   Obj *object = vm.objects;
    while (object != NULL) {
        Obj *next = object->next;
        freeObject(object);
        object = next;
    }
}
