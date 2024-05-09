//
// Created by Brucey on 08/05/2024.
//
#include <stdlib.h>

#include "chunk.h"
#include "lines.h"
#include "memory.h"

void initChunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    initLineCountArray(&chunk->lines);
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    writeLineCountArray(&chunk->lines, line);
    chunk->count++;
}

void freeChunk(Chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeLineCountArray(&chunk->lines);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

int addConstant(Chunk *chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}