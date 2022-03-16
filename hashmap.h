#ifndef _HASHMAP_H
#define _HASHMAP_H 

#define INITIAL_HMAP_SIZE 1000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct hashData {
    char* key;
    void* value;
    // struct hashData* next;
};

struct hashTable {
    // array de hashData
    struct hashData** table;
    int capacity;
    int current_size;
};

struct hashTable* initTable();
void addItem(struct hashTable* t, char* key, void* newValue);
void deleteItem(struct hashTable* t, char* key);
void setValueForKey(struct hashTable* t, char* key, void* newValue);
void* getValueByKey(struct hashTable* t, char* key);
void freeTable(struct hashTable* t);
void printHashTable(struct hashTable* t);
unsigned long hashFunction(char* key);

#endif