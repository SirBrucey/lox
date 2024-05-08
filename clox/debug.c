//
// Created by Brucey on 08/05/2024.
//
#include <stdio.h>

#include "debug.h"

void disassembleChunk(const Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int simpleInstruction(const char *name, const int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(const Chunk *chunk, const int offset) {
    printf("%04d ", offset);

    const uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}