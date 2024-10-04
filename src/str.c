#include <unistd.h>
#include <fcntl.h>
#include "str.h"

#include "defs.h"
int kstrcmp(char *s1, char *s2) {
    while (*s1 == *s2) {
        if (*s1 == '\0' && *s2 == '\0') {
            return true; 
        }
        s1++;
        s2++;
    }
    return false; 
}

int kstrlen(char *str){
    int count = 0;
    if(*str == '\0'){
        return 0;
    }
    while(*str != '\0'){
        count++;
    }
    return count;
}

int kstrcpy(char *src, char *dst) {
    if (src == NULL || dst == NULL) {
        return 0;
    }
    int c = 0;
    while (*src != '\0') {
        c++;
        *dst++ = *src++;
    }
    *dst = '\0'; // Ensure the destination string is null-terminated
    return c;
}

