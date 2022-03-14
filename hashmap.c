#include "hashmap.h"

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

int hashFunction(char* key) {

}
 

void addItem(struct hashTable* t) {

}

void freeTable(struct hashTable* t) {
    for (int i = 0; i < t->current_size; ++i) {
        if (t->table[i] != NULL) {
            free(t->table[i]);
            // free si next daca il mai folosesti
        }
    }
    free(t->table);
    free(t);
}