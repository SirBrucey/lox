//
// Created by Brucey on 10/05/2024.
//
#include "chunk.h";

#ifndef VM_H
#define VM_H

typedef struct {
    Chunk *chunk;
    uint8_t *ip;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk *chunk);

#endif //VM_H
