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
    printf("print table\n");
    // printf("table current size=%d\n", t->current_size);

    for (int i = 0; i < t->capacity; ++i) {
        if (t->table[i] != NULL) {
            printf("key:%s value:%s\n", t->table[i]->key, (char *)t->table[i]->value);
        }
    }
    // printf("\n");
  
}

// hash function using djb2  method
unsigned long hashFunction(char* key) {
    unsigned int hashed_index = HASH_CONST;
    int c;

    while ((c = *key++)) {
        hashed_index = hashed_index * HASH_MULTIPLIER + c;
    }
    return hashed_index;
}
 
// add item using linear probing when encountering a collision
void addItem(struct hashTable* t, char* key, void* newValue) {
    unsigned long hash = hashFunction(key);
    unsigned long index = 0;
    int i = 0;
    int len_key = 0;
    int len_value = 0;

    // printf("add %s %s\n", key, (char *)newValue);
    while(1) {
        index = (hash+i) %  t->capacity;
        // printf("i = %d\n", i);
        if(!t->table[index]) {
            // printf("nu exista element la indexul %ld\n", index);
            // insert value
            t->table[index] = (struct hashData *) calloc(1, sizeof(struct hashData));
            len_key = strlen(key) + 1;
            t->table[index]->key = calloc(len_key, sizeof(char));
            len_value = strlen(newValue) + 1;
            t->table[index]->value = calloc(len_value, sizeof(char));
            memcpy(t->table[index]->key, key, len_key - 1);
            memcpy(t->table[index]->value, newValue, len_value - 1);
            // hd->key = key;
            // hd->value = newValue;
            break;
        } 
        else {
            if (index < t->capacity) {
                i++;
            }
        }
    }
    t->current_size++;
    if (t->current_size >= t->capacity * 3 / 4 ) {
        t->table = realloc(t->table, 2 * t->capacity);
        t->capacity *= 2; 
    }
}


void* getValueByKey(struct hashTable* t, char* key) {
    // printf("get value by key: %s\n", key);
    unsigned long hash = hashFunction(key);
    unsigned long index = 0;
    int i = 0;

    while(1) {
        index = (hash+i) %  t->capacity;
        if(t->table[index] != NULL) {
            // printf("la indexul %ld am cheia %s\n", index, t->table[index]->key);
            if (strncmp(key, t->table[index]->key, strlen(key)) == 0) {
                return t->table[index]->value;
            } else {
                if (index < t->capacity) {
                    i++;
                } else break;
            }
        } 
        else {
            return NULL;
        }
    }
    return NULL;

}


void deleteItem(struct hashTable* t, char* key) {
    unsigned long hash = hashFunction(key);
    unsigned long index = 0;
    int i = 0;

    while(1) {
        index = (hash+i) %  t->capacity;
        if(t->table[index]) {
            // printf("la indexul %ld am cheia %s\n", index, t->table[index]->key);
            if (strncmp(key, t->table[index]->key, strlen(key)) == 0) {
                free(t->table[index]->key);
                free(t->table[index]->value);
                free(t->table[index]);
                t->table[index] = NULL;
                return;
            } else {
                if (index < t->capacity) {
                    i++;
                }
            }
        } 
       else {
            // printf("else\n");
            if (index < t->capacity) {
                i++;
            }
        }
    }
    t->current_size--;
}

void freeTable(struct hashTable* t) {
    for (int i = 0; i < t->capacity; ++i) {
        if (t->table[i] != NULL) {
            free(t->table[i]->key);
            free(t->table[i]->value);
            free(t->table[i]);
        }
    }
    free(t->table);
    free(t);
}