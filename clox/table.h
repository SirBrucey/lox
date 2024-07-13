//
// Created by Brucey on 13/07/2024.
//

#ifndef TABLE_H
#define TABLE_H

#include "common.h"
#include "value.h"

typedef struct {
    ObjString* key;
    Value value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry* entries;
} Table;

void initTable(Table* table);
void freeTable(Table* table);
bool tableGet(const Table* table, const ObjString* key, Value* value);
bool tableSet(Table* table, ObjString* key, Value value);
bool tableDelete(const Table* table, const ObjString* key);
void tableAddAll(const Table* from, Table* to);
ObjString* tableFindString(const Table* table, const char* chars, const int length, const uint32_t hash);

#endif //TABLE_H
