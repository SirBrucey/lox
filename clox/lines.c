//
// Created by Brucey on 09/05/2024.
//

#include "memory.h"
#include "lines.h"

void initLineCountArray(LineCountArray *array) {
    array->capacity = 0;
    array->count = 0;
    array->line = NULL;
}

void writeLineCountArray(LineCountArray *array, int line) {
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->line = GROW_ARRAY(LineCount, array->line, oldCapacity, array->capacity);
    }

    // If the array is empty or the last line is different from the current line.
    // We add a new line to the array.
    // Otherwise, we increment the occurences of the last line.
    if (array->count == 0 || array->line[array->count - 1].line != line) {
        array->line[array->count].line = line;
        array->line[array->count].occurences = 1;
        array->count++;
    } else {
        array->line[array->count - 1].occurences++;
    }
}

void freeLineCountArray(LineCountArray *array) {
    FREE_ARRAY(LineCount, array->line, array->capacity);
    initLineCountArray(array);
}

int getLine(const LineCountArray *array, int offset) {
    for (int i = 0; i < array->count; i++) {
        offset -= array->line[i].occurences;
        if (offset < 0) {
            return array->line[i].line;
        }
    }
    return -1;
}
