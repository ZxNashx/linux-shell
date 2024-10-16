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


int kstrhas(char *str, char *substring) {
    if (*substring == '\0') {
        // If the substring is empty, we assume it is always found
        return true;
    }

    for (int i = 0; str[i] != '\0'; i++) {
        int j;
        // Check if substring matches at position i
        for (j = 0; substring[j] != '\0'; j++) {
            if (str[i + j] == '\0' || str[i + j] != substring[j]) {
                break;  // If characters don't match, break out of the inner loop
            }
        }
        if (substring[j] == '\0') {
            // If we completed the inner loop, substring was found
            return true;
        }
    }

    // If no match was found after going through the main string
    return false;
}
