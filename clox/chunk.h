//
// Created by Brucey on 08/05/2024.
//

#ifndef CHUNK_H
#define CHUNK_H

#include "common.h"
#include "lines.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t *code;
    LineCountArray lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
void freeChunk(Chunk *chunk);

int addConstant(Chunk *chunk, Value value);


#endif //CHUNK_H
