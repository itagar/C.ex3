/**
 * @file MyStringFunctions.c
 * @author Itai Tagar <itagar>
 * @version 1.4
 * @date 23 Aug 2016
 *
 * @brief A file for the String Functions. It defines several Macros and defines Functions
 *        for an String Key, to use during the Generic Hash Table operations
 *        which holds Strings as keys.
 *
 * @section LICENSE
 * This program is free to use in every operation system.
 *
 * @section DESCRIPTION
 * A file for the String Functions. It defines several Macros and defines Functions
 * for an String Key, to use during the Generic Hash Table operations which holds Strings as keys.
 * Input:       No particular input.
 * Process:     This file defines several functions that are relevant to the Hash Table operations.
 * Output:      No particular output.
 */


/*-----=  Includes  =-----*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "MyStringFunctions.h"


/*-----=  Definitions  =-----*/


/**
 * @def PRINT_FORMAT "%s"
 * @brief A Macro that sets the output format for printing a given string.
 */
#define PRINT_FORMAT "%s"

/**
 * @def STRING_TERMINATOR_COUNT 1
 * @brief A Macro that sets the number for the string terminator addition.
 */
#define STRING_TERMINATOR_COUNT 1

/**
 * @def ASCII_INITIAL_COUNT 0
 * @brief A Macro that sets the number for the initial count for the ascii value of the string.
 */
#define ASCII_INITIAL_COUNT 0

/**
 * @def STRING_TERMINATOR '\0'
 * @brief A Macro that sets the char for a String Terminator.
 */
#define STRING_TERMINATOR '\0'


/*-----=  My String Functions  =-----*/


/**
 * @brief Allocate memory for a pointer to a string which contain duplication of the given key.
 *        If run out of memory, free all the memory that was already allocated by the function,
 *        report error MEM_OUT to the standard error and return NULL.
 * @param s The key to clone which holds an string data.
 * @return Return the clone key if succeed, NULL otherwise.
 */
void * cloneStr(const void *s)
{
    assert(s != NULL);

    char *clone = NULL;
    clone = (char *)malloc(sizeof(char) * (strlen((char *)s) + STRING_TERMINATOR_COUNT));
    if (clone != NULL)
    {
        strcpy(clone, s);
    }
    else
    {
        reportError(MEM_OUT);
    }
    return clone;
}

/**
 * @brief Free all the memory allocated for the given object.
 * @param s The key to free which holds an string data.
 */
void freeStr(void *s)
{
    if (s != NULL)
    {
        free(s);
    }
}

/**
 * @brief Generates the Hash Code of the given key for HashTable with size tableSize.
 *        The Hash Code is the given key modulus the Table size.
 * @param s The key to generate Hash Code to.
 * @param tableSize The size of the Hash Table.
 * @return A Number between 0 - (tableSize-1) or negative number in case of an error.
 */
int strFcn(const void *s, size_t tableSize)
{
    int hashCode = INVALID_HASH_CODE;

    if (s != NULL)
    {
        int asciiValue = ASCII_INITIAL_COUNT;
        char *stringKey = (char *)s;

        while (*stringKey != STRING_TERMINATOR)
        {
            asciiValue += *stringKey;
            stringKey++;
        }

        hashCode = asciiValue % ((int)tableSize);
    }

    return hashCode;
}

/**
 * @brief Prints the given key to the standard output.
 * @param s The key to print.
 */
void strPrint(const void *s)
{
    if (s != NULL)
    {
        char *stringKey = (char *)s;
        printf(PRINT_FORMAT, stringKey);
    }
}

/**
 * @brief Compares 2 given keys.
 * @param key1 The first key to compare.
 * @param key2 The second key to compare.
 * @return 0 if both keys are equal, otherwise return a non-zero value.
 */
int strCompare(const void * key1, const void * key2)
{
    if ((key1 != NULL) && (key2 != NULL))
    {
        char *stringKey1 = (char *)key1;
        char *stringKey2 = (char *)key2;

        if (strcmp(stringKey1, stringKey2) == EQUALITY_VALUE)
        {
            return EQUALITY_VALUE;
        }

        return NON_EQUALITY_VALUE;
    }

    return COMPARE_ERROR_VALUE;
}
