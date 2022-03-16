#include "hashmap.h"

#define HASH_CONST 5381
#define HASH_MULTIPLIER 33

struct hashTable* initTable(){
    struct hashTable* htable = (struct hashTable*)calloc(1,sizeof(struct hashTable));

    if(htable == NULL) {
        printf("error creating table");
        exit(-1);
    }

    htable->capacity = INITIAL_HMAP_SIZE;
    htable->current_size = 0;

    htable->table = (struct hashData**)calloc(INITIAL_HMAP_SIZE, sizeof(struct hashData*));
    if(htable == NULL) {
        printf("error creating table contents");
        exit(-1);
    }
    return htable;
}

void printHashTable(struct hashTable* t) {
    printf("table current size=%d\n", t->current_size);

    for (int i = 0; i < t->current_size; ++i) {
        printf("key:%s value:%s", t->table[i]->key, (char *)t->table[i]->value);
    }
    printf("\n");
  
}

// hash function using djb2  method
unsigned long hashFunction(char* key) {
    unsigned int hashed_index = HASH_CONST;
    int c;

    while (c = *key++) {
        hashed_index = hashed_index * HASH_MULTIPLIER + c;
    }
    return hashed_index;
}
 
// add item using linear probing when encountering a collision
void addItem(struct hashTable* t, char* key, void* newValue) {
    unsigned long hash = hashFunction(key);
    printf("add %s %s hash = %ld\n", key, (char *)newValue, hash);
    while(1) {
        printf("while\n");
        if(!t->table[hash]) {
            printf("nu exista element la indexul %ld\n", hash);
            // insert value
            struct hashData *hd = (struct hashData *) calloc(1, sizeof(struct hashData));
            hd->key = key;
            hd->value = newValue;
            t->table[hash] = hd;
            break;
        } 
        else {
            printf("else\n");

            if (hash < t->capacity) {
                hash++;
            }
            else {
                hash = 0;
            }
        }
    }
    t->current_size++;
    if (t->current_size >= t->capacity * 3 / 4 ) {
        // resize 
        printf("need to resize table\n");    
    }
}

void* getValueByKey(struct hashTable* t, char* key) {
    unsigned long hash = hashFunction(key);
    while(1) {
        if(t->table[hash]) {
            printf("la indexul %ld am cheia %s\n", hash, t->table[hash]->key);
            if (strncmp(key, t->table[hash]->key, strlen(key)) == 0) {
                return t->table[hash]->value;
            } else {
                if (hash < t->capacity) {
                    hash++;
                }
                else {
                    hash = 0;
                }
            }
        } 
        else {
            if (hash < t->capacity) {
                hash++;
            }
            else {
                hash = 0;
            }
        }
    }
}


void deleteItem(struct hashTable* t, char* key) {
    unsigned long hash = hashFunction(key);
    while(1) {
        if(t->table[hash]) {
            printf("la indexul %ld am cheia %s\n", hash, t->table[hash]->key);
            if (strncmp(key, t->table[hash]->key, strlen(key)) == 0) {
                t->table[hash] = NULL;
            } else {
                if (hash < t->capacity) {
                    hash++;
                }
                else {
                    hash = 0;
                }
            }
        } 
        else {
            if (hash < t->capacity) {
                hash++;
            }
            else {
                hash = 0;
            }
        }
    }
}

void freeTable(struct hashTable* t) {
    for (int i = 0; i < t->current_size; ++i) {
        if (t->table[i] != NULL) {
            free(t->table[i]);
        }
    }
    free(t->table);
    free(t);
}