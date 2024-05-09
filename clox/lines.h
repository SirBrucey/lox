//
// Created by Brucey on 09/05/2024.
//
// This feels a bit heavy weight when an array of bytes would do the job,
// however I think this is a good place to leave it for now and I can optimise
// it later.
//

#ifndef LINES_H
#define LINES_H

#include "common.h"

typedef struct {
    int line;
    int occurences;
} LineCount;

typedef struct {
    int capacity;
    int count;
    LineCount *line;
} LineCountArray;

void initLineCountArray(LineCountArray *array);
void writeLineCountArray(LineCountArray *array, int line);
void freeLineCountArray(LineCountArray *array);

int getLine(const LineCountArray *array, int offset);

#endif //LINES_H
