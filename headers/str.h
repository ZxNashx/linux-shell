#ifndef STR_H
#define STR_H

#include <stdbool.h>  // For the `bool` type

/**
 * @brief Compares two strings for equality.
 *
 * This function compares two null-terminated strings and returns true if they are identical.
 * 
 * @param s1 Pointer to the first string.
 * @param s2 Pointer to the second string.
 * @return true if the strings are identical, false otherwise.
 */
int kstrcmp(char *s1, char *s2);

/**
 * @brief Compares up to 'n' characters of two strings.
 *
 * This function compares the first 'n' characters of two strings and returns true 
 * if they are equal up to that point or if both strings end before 'n' characters.
 * 
 * @param s1 Pointer to the first string.
 * @param s2 Pointer to the second string.
 * @param n The maximum number of characters to compare.
 * @return true if the strings match up to 'n' characters, false otherwise.
 */
int kstrcmp_by_n(char *s1, char *s2, int n);

/**
 * @brief Computes the length of a string.
 *
 * This function counts the number of characters in a null-terminated string, excluding
 * the null terminator.
 * 
 * @param str Pointer to the string.
 * @return The length of the string.
 */
int kstrlen(char *str);

/**
 * @brief Copies a string from source to destination.
 *
 * Copies the contents of the source string into the destination string, including 
 * the null terminator.
 * 
 * @param src Pointer to the source string.
 * @param dst Pointer to the destination string.
 * @return The number of characters copied (excluding the null terminator).
 */
int kstrcpy(char *src, char *dst);

/**
 * @brief Concatenates two strings.
 *
 * Appends the contents of the second string to the first string. Assumes that 
 * the first string has enough space to hold the result.
 * 
 * @param s1 Pointer to the first string (destination).
 * @param s2 Pointer to the second string (source).
 */
void kstrconcat(char *s1, const char *s2);

/**
 * @brief Checks if a substring exists within a string.
 *
 * This function searches for the occurrence of a substring within a larger string.
 * 
 * @param str Pointer to the main string.
 * @param substring Pointer to the substring to search for.
 * @return true if the substring is found, false otherwise.
 */
int kstrhas(char *str, char *substring);

/**
 * @brief Checks if a character exists within a string.
 *
 * This function searches for a single character within a string.
 * 
 * @param str Pointer to the main string.
 * @param uchar The character to search for.
 * @return true if the character is found, false otherwise.
 */
int kstrhas_unary(char *str, char uchar);

#endif  // STR_H
