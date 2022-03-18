#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"
#include "hashmap.h"

int main(int argc, char **argv) {
	// declar variabilele aici:
	FILE *fsrc = NULL;
	FILE *fdest = NULL;
	int i = 0, k1, k2, directory = 1;
	// schimba numele variabilelor astora 
	char *filename_in = NULL, *filename_out = NULL;

	struct hashTable* ht = initTable();

	char **fileDirectories = NULL;
	fileDirectories = malloc(10 * sizeof(char *));
	if(fileDirectories == NULL)
		exit(1);
	for(i = 0; i < 10; i++) {
		fileDirectories[i] = NULL;
	}

	// parseaza argumentele
	// foloseste while in loc de for
	while (i < argc) {
		if(strncmp("-D", argv[i], 2) == 0) {
			printf("caz d\n");
		}
		else if(strncmp("-I", argv[i], 2) == 0) {
			printf("caz i\n");
			//adauga director
			fileDirectories[directory]= malloc(
				(strlen(argv[i+1]) + 1) * sizeof(char));
			if(fileDirectories[directory] == NULL)
				exit(1);
			memcpy(fileDirectories[directory], argv[i+1], 
				strlen(argv[i + 1]) + 1);
			directory++;
			i++;

		}
		else if(strncmp("-o", argv[i], 2) == 0) {
			printf("caz o\n");

			filename_out = malloc((strlen(argv[i + 1]) + 1) * sizeof(char));
			// foloseste DIE
			if(filename_out == NULL)
				exit(1);
			memcpy(filename_out, argv[i+1], strlen(argv[i + 1] + 1));
			k2 = 1;
			i++;
		}
		else {
			// am nume fisier input


		}
		i++;
	}


	// deschid fisierul de input primit ca argument mai sus
	fsrc = fopen("ceva.txt", "r");


	// citeste din fisierul de input cu fgets? fread?




	// va trebui sa extrag argumentele 

	fclose(fsrc);
	fclose(fdest);
	freeTable(ht);
	return 0; 
}


/*
struct hashTable* ht = initTable();
	char *key = "Ana";
	char *val = "Banana";
	addItem(ht, key, val);

	char *key1 = "cheie";
	char *val1 = "test";
	addItem(ht, key1, val1);

	// char *x = calloc (7, sizeof(char));
	char *x = (char*) getValueByKey(ht, key);
	printf("%s\n", x);

	deleteItem(ht, key1);
	
	printHashTable(ht);
	// free(x);

	freeTable(ht);
*/
