#include <unistd.h>
#include <fcntl.h>
#include "defs.h"

int kstrcmp(char *s1, char *s2);
int kstrlen(char *str);
int kstrcpy(char * src, char * dst);
int kstrconcat(char *src, char *dst);
int kstrhas(char *str, char *substring);
