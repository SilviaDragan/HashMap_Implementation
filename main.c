#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"
#include "hashmap.h"

// delimitatoare: \t []{}<>=+-*/%!&|^.,:;()\.

void read_input(FILE *fsrc, FILE *fdest,char **dirs, int dir_no, struct hashTable* ht) {
	char line[MAX_LINE_LEN];
	printf("in functie\n");

	while(fgets(line, MAX_LINE_LEN, fsrc)) {
		fprintf(fdest, "%s", line);
	}

}

int main(int argc, char **argv) {
	FILE *fsrc = NULL, *fdest = NULL;
	// char *fin = NULL, *fout = NULL;
	int i = 1, dir_no = 0;
	int write_to_output = 0, read_from_input = 0, files_witout_flag = 0;
	struct hashTable* ht = initTable();

	char **dirs = (char **) calloc(MAX_DIRS_NO, sizeof(char *));
	if(dirs == NULL) {
		exit(12);
	}

	while (i < argc) {
		if(strncmp("-", argv[i], 1) == 0) {
			if(strncmp("-D", argv[i], 2) == 0) {
				printf("caz d\n");
			}
			else if(strncmp("-I", argv[i], 2) == 0) {
				printf("-i %s\n", argv[i+1]);
				//adauga director
				dirs[dir_no]= calloc((strlen(argv[i+1]) + 1), sizeof(char));
				if(dirs[dir_no] == NULL) {
					exit(12);
				}

				// strcpy?
				memcpy(dirs[dir_no], argv[i+1], strlen(argv[i + 1]) + 1);
				dir_no++;
				i++;
			}
			else if(strncmp("-o", argv[i], 2) == 0) {
				write_to_output = 1;
				printf("-o %s\n", argv[i+1]);

				fdest = fopen(argv[i+1], "wb");
				DIE(fdest == NULL, "cannot open output file");
				i++;
			}
			else {
				exit(12);
			}
		}
		else {
			// am nume fisier input
			printf("filename = %s\n", argv[i]);
			files_witout_flag++;
			if (files_witout_flag == 1) {
				read_from_input = 1;
				fsrc = fopen(argv[i], "r");
				DIE(fsrc == NULL, "cannot open input file");
			}
			else if (files_witout_flag == 2 && write_to_output == 0) {
				write_to_output = 1;
				fdest = fopen(argv[i], "wb");
				DIE(fdest == NULL, "cannot open output file");
			}
			else {
				exit(-1);
			}
		}
		i++;
	}


	if (read_from_input != 1) {
		printf("read from stdin\n");
		fsrc = stdin;
	}

	if (write_to_output != 1) {
		printf("write to stdout\n");
		fdest = stdout;
	}


	read_input(fsrc, fdest, dirs, dir_no, ht);


	if(read_from_input == 1) {
		fclose(fsrc);
	}
	if (write_to_output == 1) {
		fclose(fdest);
	}
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


