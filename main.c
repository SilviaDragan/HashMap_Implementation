#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"
#include "hashmap.h"

// delimitatoare: \t []{}<>=+-*/%!&|^.,:;()\.

void str_replace(char *target, const char *needle, const char *replacement)
{
    char buffer[1024] = { 0 };
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1) {
        const char *p = strstr(tmp, needle);

        // walked past last occurrence of needle; copy remaining part
        if (p == NULL) {
            strcpy(insert_point, tmp);
            break;
        }

        // copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        // adjust pointers, move on
        tmp = p + needle_len;
    }

    // write altered string back to target
    strcpy(target, buffer);
}

void read_input(FILE *fsrc, FILE *fdest,char **dirs, int dir_no, struct hashTable* ht) {
	char line[MAX_LINE_LEN];
	char *buffer = NULL;
	char *token = NULL;
	char *token2 = NULL;
	char *new_val = NULL;
	int length = 0;

	char *key = NULL;
	char *val = NULL;
	char *result_after_define = NULL;
	char *copy_input = NULL;
	// int if_flag = 0;
	// int else_flag = 0;

	// printf("dirs:%d\n", dir_no);

	// for (i = 0; i < dir_no; i++) {
	// 	printf("%s\n", dirs[i]);
	// }
	result_after_define = calloc(10000, sizeof(char));
	if(result_after_define == NULL) exit(12);
	
	copy_input = calloc(10000, sizeof(char));
	if(copy_input == NULL) exit(12);

	while(fgets(line, MAX_LINE_LEN, fsrc)) {
		// printf("line: %s\n", line);
		length = strlen(line) + 1;
		buffer = calloc(length, sizeof(char));
		if(buffer == NULL) exit(12);

		memcpy(buffer, line, length);
		buffer[strlen(buffer) - 1] = '\n';
		if(strcmp(line, "\r\n") == 0) {
			free(buffer);
			continue;
		}

		if (strncmp(buffer, "#define", 7) == 0) {
			token = strtok(buffer, " ");
			token = strtok(NULL, " ");

			length = strlen(token) + 1;
			key = calloc(length, sizeof(char));
			if(key == NULL) exit(12);

			memcpy(key, token, length);
			key[strlen(key)] = '\0';
			// printf("define key:%s\n", key);

			token = strtok(NULL, "\\\n");

			length = strlen(token) + 1;
			val = calloc(length, sizeof(char));
			if(val == NULL) exit(12);
			// printf("token:%s\n", token);
			length = strlen(token);
			memcpy(val, token, length);
			val[strlen(val) - 1] = '\0';
			// printf("initial value:%s\n", val);

			// token = strtok(NULL, "\\\n");
			// printf("token:a%sa\n", token);

			// nu merge asta pt testul 12 fmm
			// while(token != NULL) {
			// 	token = strtok(NULL, DELIMS);
			// 	printf("token:%s\n", token);
			// 	strcat(val, token);
			// }
			// printf("key:%s value:%s\n",key, val);

			// nested defines
			char value_copy[strlen(val) + 100];
			int change_value = 0;
			length = strlen(val) + 100;
			char *new_val = calloc(length, sizeof(char));

			strcpy(value_copy, val);

			// printf("copie=%s\n", value_copy);
			token = strtok(value_copy, " ");
			
			while(token != NULL){
				if(getValueByKey(ht, token) != NULL) {
					// printf("da am gasit\n");
					strcat(new_val, getValueByKey(ht, token));
					strcat(new_val, " ");
					change_value = 1;
				}
				else {
					strcat(new_val, token);
					strcat(new_val, " ");
				}
				token = strtok(NULL, " ");
			}
			new_val[strlen(new_val)-1] = '\0';


			// printf("new val = %s\n", new_val);
			(change_value == 1) ? addItem(ht, key, new_val) : addItem(ht, key, val);

			free(key);
			free(val);
			free(new_val);
		}
		
		else if (strncmp(buffer, "#if", 3) == 0) {
			free(buffer);
			// token = strtok(buffer, " ");
			// char *test =  NULL;
			// // cauta tokenul in mapa
			// if(getValueByKey(ht, token) != NULL) {
			// 	test = getValueByKey(ht, token);
			// 	printf("token = %s, value = %s\n", token, value);

			// } else {
			// 	test = token;
			// }
			// // verifica valoarea de adevar a lui atoi(test)
			// if(atoi(test)) {
			// 	// print next lines
			// }
			// else {
			// 	// don't print print lines in if, print the ones in else
			// }
			continue;

		}
		else if (strncmp(buffer, "#endif", 6) == 0) {
			free(buffer);
			continue;
			
		}

		else if (strncmp(buffer, "#elif", 5) == 0) {
			free(buffer);
			continue;
		}

		else if (strncmp(buffer, "#else", 5) == 0) {
			free(buffer);
			continue;
		}

		else {
			strcat(copy_input, buffer);
		}
		free(buffer);
	}


	// printHashTable(ht);

	fseek(fsrc, 0, SEEK_SET);
	token2 = strtok(copy_input, "\n");
	while (token2 != NULL) {
		// printf("tok=%s\n", token2);
		if (strncmp(token2, "#define", 7)) {
			strcat(result_after_define, token2);
			result_after_define[strlen(result_after_define)] = '\n';
		}
		token2 = strtok(NULL, "\n");
	}

	for (int k = 0; k < ht->capacity; k++) {
		if (ht->table[k] != NULL) {
			// check for undef :)
			str_replace(result_after_define, ht->table[k]->key,(char *)ht->table[k]->value);
		}
	}

	fprintf(fdest, "%s", result_after_define);
	free(result_after_define);
	free(copy_input);
}

void addArgInMap(char *arg, struct hashTable* ht) {
	char *key = NULL;
	char *val = NULL;

	// printf("argument: %s\n", arg);
	key = strtok(arg, "=");
	val = strtok(NULL, arg);

	// printf("key: %s val: %s\n", token, val);
	addItem(ht, key, val);
	// printHashTable(ht);
}

int main(int argc, char **argv) {
	FILE *fsrc = NULL, *fdest = NULL;
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
				// printf("caz d\n");
				addArgInMap(argv[i+1], ht);
				i++;
			}
			else if(strncmp("-I", argv[i], 2) == 0) {
				// printf("-i %s\n", argv[i+1]);
				dirs[dir_no]= calloc((strlen(argv[i+1]) + 1), sizeof(char));
				if(dirs[dir_no] == NULL) {
					exit(12);
				}
				memcpy(dirs[dir_no], argv[i+1], strlen(argv[i + 1]) + 1);
				dir_no++;
				i++;
			}
			else if(strncmp("-o", argv[i], 2) == 0) {
				write_to_output = 1;
				// printf("-o %s\n", argv[i+1]);

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
			// printf("filename = %s\n", argv[i]);
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
		fsrc = stdin;
	}

	if (write_to_output != 1) {
		fdest = stdout;
	}


	read_input(fsrc, fdest, dirs, dir_no, ht);


	if(read_from_input == 1) {
		fclose(fsrc);
	}
	if (write_to_output == 1) {
		fclose(fdest);
	}

	for(i = 0; i < dir_no; i++) {
		free(dirs[i]);
	}
	free(dirs);

	freeTable(ht);
	return 0; 
}