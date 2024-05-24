//
// Created by Brucey on 08/05/2024.
//

#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
    initVM();
    Chunk chunk;
    initChunk(&chunk);

    writeConstant(&chunk, 123, 123);
    writeChunk(&chunk, OP_NEGATE, 123);
    writeChunk(&chunk, OP_RETURN, 123);

    disassembleChunk(&chunk, "test chunk");
    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);
    return 0;
}
