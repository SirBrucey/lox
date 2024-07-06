//
// Created by Brucey on 01/07/2024.
//

#ifndef COMPILER_H
#define COMPILER_H

#include "object.h"
#include "vm.h"

bool compile(const char *source, Chunk *chunk);

#endif //COMPILER_H
