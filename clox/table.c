//
// Created by Brucey on 13/07/2024.
//

#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table *table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void freeTable(Table* table) {
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}

static Entry* findEntry(Entry* entries, const int capacity, const Value key) {
    uint32_t index = hashValue(key) % capacity;
    Entry* tombstone = NULL;

    for (;;) {
        Entry* entry = &entries[index];
        if (IS_EMPTY(entry->key)) {
            if (IS_NIL(entry->value)) {
                // Empty entry
                return tombstone != NULL ? tombstone : entry;
            } else {
                // Found tombstone
                if (tombstone == NULL) tombstone = entry;
            }
        } else if (valuesEqual(key, entry->key)) {
            // Found key
            return entry;
        }

        index = (index + 1) % capacity;
    }
}

// Mutates value with the value from the table.
bool tableGet(const Table *table, const Value key, Value *value) {
    if (table->count == 0) return false;

    const Entry* entry = findEntry(table->entries, table->capacity, key);
    if (IS_NIL(entry->key)) return false;

    *value = entry->value;
    return true;
}

static void adjustCapacity(Table* table, const int capacity) {
    // Create new table
    Entry* entries = ALLOCATE(Entry, capacity);
    for (int i = 0; i < capacity; i++) {
        entries[i].key = EMPTY_VAL;
        entries[i].value = NIL_VAL;
    }

    // Copy entries to new table, using find entry to generate new indexes.
    table->count = 0;
    for (int i = 0; i < table->capacity; i++) {
        const Entry* entry = &table->entries[i];
        if (IS_EMPTY(entry->key)) continue;

        Entry* dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    // Free old table and move new in table.
    FREE_ARRAY(Entry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

bool tableSet(Table *table, const Value key, const Value value) {
    if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        const int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }

    Entry* entry = findEntry(table->entries, table->capacity, key);
    const bool isNewKey = IS_EMPTY(entry->key);

    entry->key = key;
    entry->value = value;

    if (isNewKey) table->count++;
    return isNewKey;
}

bool tableDelete(const Table *table, const Value key) {
    if (table->count == 0) return false;

    // Find entry
    Entry* entry = findEntry(table->entries, table->capacity, key);
    if (IS_EMPTY(entry->key)) return false;

    // Place tombstone in the entry
    entry->key = EMPTY_VAL;
    entry->value = BOOL_VAL(true);
    return true;
}

void tableAddAll(const Table* from, Table* to) {
    for (int i = 0; i < from->capacity; i++) {
        const Entry* entry = &from->entries[i];
        if (!IS_EMPTY(entry->key)) {
            tableSet(to, entry->key, entry->value);
        }
    }
}

ObjString * tableFindString(const Table * table, const char *chars, const int length, const uint32_t hash) {
    if (table->count == 0) return NULL;

    uint32_t index = hash % table->capacity;
    for (;;) {
        const Entry* entry = &table->entries[index];

        if (IS_EMPTY(entry->key)) return NULL;

        ObjString* string = AS_STRING(entry->key);
        if (
            string->length == length &&
            memcmp(string->chars, chars, length) == 0
        ) {
            // Found it
            return string;
        }

        index = (index + 1) % table->capacity;
    }
}
