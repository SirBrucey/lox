//
// Created by Brucey on 08/05/2024.
//
#include <stdio.h>

#include "debug.h"
#include "value.h"

void disassembleChunk(const Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

static int constantInstruction(const char *name, const Chunk *chunk, const int offset) {
    const uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

static int simpleInstruction(const char *name, const int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(const Chunk *chunk, const int offset) {
    printf("%04d ", offset);

    const uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}