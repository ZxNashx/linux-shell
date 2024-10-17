#ifndef PARSER_H
#define PARSER_H

#include <unistd.h>
#include <fcntl.h>
#include "defs.h"

/*
  Input: char *str_to_split - A string to be split
         char *tokens[]     - Array to hold tokenized strings
         int *count         - Holds the amount of tokens in the token array
         char split_on      - The requested delimiter for which to split the string
         char *original     - A string from main for which to freely operate on

  Returns: Nothing yet

*/
int split_str(char *str_to_split, char *tokens[], int *count, char split_on, char *original);
//int tokenize(char *str_to_split, char *tokens[], int *count, char split_on);

#endif


