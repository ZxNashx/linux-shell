#include <unistd.h>
#include <fcntl.h>
#include "defs.h"
int strcmp(char *s1, char *s2) {
	int count = 0;
    while (*s1 == *s2) {
    	if(*s1 == '\0' && *s2 == '\0'){
    		return true;
    	}
    	s1++;
    	s2++;
    }
    return false;
}

int strlen(char *str){
    int count = 0;
    if(*str == '\0'){
        return 0;
    }
    while(*str != '\0'){
        count++;
    }
    return count;
}

int strcpy(char * src, char * dst){
    while(*src != '\0') {
        *dst++ = *src++;
    }
    *dst = '\0';
    return dst;
}
