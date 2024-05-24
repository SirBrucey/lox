//
// Created by Brucey on 08/05/2024.
//
#include <stdio.h>

#include "debug.h"
#include "lines.h"
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

static int constant16Instruction(const char *name, const Chunk *chunk, const int offset) {
    const uint16_t constant = chunk->code[offset + 1] | (chunk->code[offset + 2] << 8);
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 3;
}

static int constant24Instruction(const char *name, const Chunk *chunk, const int offset) {
    const uint32_t constant = chunk->code[offset + 1] |
        (chunk->code[offset + 2] << 8) |
        (chunk->code[offset + 3] << 16);
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 4;
}

static int simpleInstruction(const char *name, const int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int disassembleInstruction(const Chunk *chunk, const int offset) {
    int currentLine = getLine(&chunk->lines, offset);

    printf("%04d ", offset);
    if (offset > 0 && currentLine == getLine(&chunk->lines, offset - 1)) {
        printf("   | ");
    } else {
        printf("%4d ", currentLine);
    }

    const uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_16:
            return constant16Instruction("OP_CONSTANT_16", chunk, offset);
        case OP_CONSTANT_24:
            return constant24Instruction("OP_CONSTANT_24", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}