#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils.h"
#include "hashmap.h"

// delimitatoare: \t []{}<>=+-*/%!&|^.,:;()\.

void str_replace(char *line, char *original, char *replacement) {
	char *buffer = NULL;	
	char *insert = NULL;
    char *tmp = line;
    int line_lenght = 0;
	int key_length = 0;
	int val_length = 0;
	char *find = NULL;
	// printf("inceput\n");
	// printf("linie:%s\n", line);
	// printf("original:%s\n", original);
	// printf("replacement:%s\n", replacement);
	val_length = strlen(replacement);
	line_lenght = strlen(line) + val_length;
	buffer = calloc(line_lenght, sizeof(char));
	if(buffer == NULL) exit(12);

	key_length = strlen(original);
	insert = &buffer[0];

    while (1) {
		// printf("in while\n");
        find = strstr(tmp, original);
        if (find == NULL) {
            strncpy(insert, tmp, strlen(tmp));
            break;
        }
        memcpy(insert, tmp, find - tmp);
        insert += find - tmp;

        memcpy(insert, replacement, val_length);
        insert += val_length;
        tmp = find + key_length;
    }
    strcpy(line, buffer);
	free(buffer);
}

void read_input(FILE *fsrc, FILE *fdest,char **dirs, int dir_no, struct hashTable* ht) {
	char line[MAX_LINE_LEN];
	char *buffer = NULL;
	char *token = NULL;
	char *token2 = NULL;
	char *new_val = NULL;
	int length = 0;
	int multiple_lines_define = 0;
	char *key = NULL;
	char *val = NULL;
	char *copy_input = NULL;
	int if_flag = 0;
	int else_flag = 0;
	char *test = NULL;

	// printf("dirs:%d\n", dir_no);

	// for (i = 0; i < dir_no; i++) {
	// 	printf("%s\n", dirs[i]);
	// }
	// result_after_define = calloc(10000, sizeof(char));
	// if(result_after_define == NULL) exit(12);
	
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

			token = strtok(NULL, "\n");

			length = strlen(token) + 1;
			val = calloc(length, sizeof(char));
			if(val == NULL) exit(12);
			// printf("token:%s\n", token);
			length = strlen(token);
			memcpy(val, token, length);
			val[strlen(val) - 1] = '\0';

			// multiple-lines define
			// printf("val=%s\n", val);

			// if(strchr(val, 92)) {
			// 	// printf("dadadadadadadadada\n");
			// 	continue;
			// }
			// multiple-lines define


			char value_copy[strlen(val) + 100];
			int change_value = 0;
			length = strlen(val) + 100;
			char *new_val = calloc(length, sizeof(char));

			strcpy(value_copy, val);

			// printf("copie=%s\n", value_copy);
			token = strtok(value_copy, " ");
			
			while(token != NULL){
				if(getValueByKey(ht, token) != NULL) {
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
			token = strtok(buffer, " ");
			token = strtok(NULL, "\r\n");

			// printf("token:%s\n", token);

			length = strlen(token);
			// printHashTable(ht);
			if(getValueByKey(ht, token) != NULL) {
				test = getValueByKey(ht, token);
				// printf("token = %s, value = %s\n", token, val);
			}
			// } else {
			// 	printf("else\n");
			// 	strcpy(test, token);
			// }
			if(test) {
				// printf("test:%s atoi:%d\n", test, atoi(test));
				if(atoi(test)){
					if_flag = 0;
				}			
				else if_flag = 1;
			} else {
				if(atoi(token)){
					// printf("da\n");
					if_flag = 0;
				}			
				else if_flag = 1;
			}

			
			free(buffer);
			continue;
		}
		else if (strncmp(buffer, "#endif", 6) == 0) {
			if_flag = 0;
			else_flag = 0;
			free(buffer);
			continue;
		}
		else if (strncmp(buffer, "#elif", 5) == 0) {
			if (if_flag == 1) {
				token = strtok(buffer, " ");
				token = strtok(NULL, "\r\n");
				length = strlen(token) + 1;
				if(getValueByKey(ht, token) != NULL) {
					test = getValueByKey(ht, token);
					// printf("token = %s, value = %s\n", token, val);
				} else {
					strcpy(test, token);
				}
				if(atoi(test)) if_flag = 0;
				else if_flag = 1;
			}
			free(buffer);
			continue;
		}
		else if (strncmp(buffer, "#else", 5) == 0) {
			if (if_flag == 1){
				else_flag = 0;
				if_flag = 0;
			} 
			else else_flag = 1;
			free(buffer);
			continue;
		}
		else if (strncmp(buffer, "#undef", 6) == 0) {
			token = strtok(buffer, " ");
			token = strtok(NULL, "\r\n");
			if (getValueByKey(ht, token) != NULL) {
				deleteItem(ht, token);
			}
			free(buffer);
			continue;
		}
		else {
			// printf("%s\n", buffer);
			if (if_flag == 1 || else_flag == 1) {
				free(buffer);
				continue;
			}
			
			for (int k = 0; k < ht->capacity; k++) {
				if (ht->table[k] != NULL) {
					if(strstr(buffer, ht->table[k]->key)!= NULL) {
						if (strlen(ht->table[k]->key) < strlen((char *)ht->table[k]->value)) {
							length = strlen(buffer) - strlen(ht->table[k]->key) + 3 * strlen((char *)ht->table[k]->value);
							buffer = (char *) realloc(buffer, length);
						}
						str_replace(buffer, ht->table[k]->key, (char *)ht->table[k]->value);
					}
				}
			}
			strcat(copy_input, buffer);
		}
		free(buffer);
	}
	fprintf(fdest, "%s", copy_input);
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