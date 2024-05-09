//
// Created by Brucey on 08/05/2024.
//

#include "chunk.h"
#include "common.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
    Chunk chunk;
    initChunk(&chunk);

    for (int i = 0; i < 65536; i++) {
        writeConstant(&chunk, i, i);
    }
    writeChunk(&chunk, OP_RETURN, 123);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);
    return 0;
}