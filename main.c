#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"
#include "hashmap.h"

int main(int argc, char **argv) {

	struct hashTable* ht = initTable();
	char *key = "Ana";
	char *val = "Banana";
	addItem(ht, key, val);
	

	printHashTable(ht);
	freeTable(ht);

	return 0; 
}