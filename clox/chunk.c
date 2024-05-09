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

void writeConstant(Chunk *chunk, const Value value, const int line) {
    int index = addConstant(chunk, value);

    if (index < 256) {
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, index, line);
    } else if (index < 65536) {
        writeChunk(chunk, OP_CONSTANT_16, line);
        writeChunk(chunk, index & 0xff, line);
        writeChunk(chunk, (index >> 8) & 0xff, line);
    } else {
        writeChunk(chunk, OP_CONSTANT_24, line);
        writeChunk(chunk, index & 0xff, line);
        writeChunk(chunk, (index >> 8) & 0xff, line);
        writeChunk(chunk, (index >> 16) & 0xff, line);
    }
}
