//
// Created by Brucey on 08/05/2024.
//

#ifndef DEBUG_H
#define DEBUG_H

#include "chunk.h"

void disassembleChunk(const Chunk *chunk, const char *name);
int disassembleInstruction(const Chunk *chunk, const int offset);

#endif //DEBUG_H
