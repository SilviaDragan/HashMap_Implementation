#ifndef _UTILS_H
#define _UTILS_H 1

#include <errno.h>

#define MAX_DIRS_NO 20
#define MAX_LINE_LEN 256
#define DELIMS "\t \n[]{}<>=+-*/%!&|^.,:;()\\"
#define MAX_FILSIZE 10000
// #define DELIMS "\t \n\\"


#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

#endif