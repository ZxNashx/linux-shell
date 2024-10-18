#include <unistd.h>
#include <fcntl.h>
#include "defs.h"
#include "str.h"

// welcome to the kevin string library




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
void kstrconcat(char *s1, const char *s2) {
    // Move pointer to the end of s1
    while (*s1 != '\0') {
        s1++;
    }

    // Copy s2 to the end of s1
    while (*s2 != '\0') {
        *s1++ = *s2++;
    }

    // Null-terminate the resulting string
    *s1 = '\0';
}

int kstrcmp_by_n(char *s1, char *s2, int n) {
    int i = 0;
    while (i < n && *s1 == *s2) {  // Compare up to n characters
        if (*s1 == '\0' || *s2 == '\0') {  // Stop if either string ends before n characters
            break;
        }
        s1++;
        s2++;
        i++;
    }

    // Check if we stopped because we reached n characters or due to a mismatch
    return (i == n || (*s1 == *s2));
}


int kstrlen(char *str){
    int count = 0;
    if(*str == '\0'){
        return EXIT_SUCCESS;
    }
    while(*str != '\0'){
        count++;
        str++;
    }
    return count;
}

int kstrcpy(char *src, char *dst) {
    if (src == NULL) {
        return EXIT_SUCCESS;
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


int kstrhas_unary(char *str, char uchar) {
    if (kstrlen(str) == 0) {
        // If the uchar is empty, we assume it is always found
        return true;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        // Check if substring matches at position i
            if (str[i] == uchar) {
                return true;
        }
    }
    // If no match was found after going through the main string
    return false;
}
