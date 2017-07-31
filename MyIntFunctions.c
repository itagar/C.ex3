/**
 * @file MyIntFunctions.h
 * @author Itai Tagar <itagar>
 * @version 1.1
 * @date 23 Aug 2016
 *
 * @brief A file for the Int Functions. It defines several Macros and defines Functions
 *        for an Int Key, to use during the Generic Hash Table operations
 *        which holds Integers as keys.
 *
 * @section LICENSE
 * This program is free to use in every operation system.
 *
 * @section DESCRIPTION
 * A file for the Int Functions. It defines several Macros and defines Functions
 * for an Int Key, to use during the Generic Hash Table operations
 * Input:       No particular input.
 * Process:     This file defines several functions that are relevant to the Hash Table operations.
 * Output:      No particular output.
 */


/*-----=  Includes  =-----*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "MyIntFunctions.h"


/*-----=  Definitions  =-----*/

/**
 * @def PRINT_FORMAT "%d"
 * @brief A Macro that sets the output format for printing a given int.
 */
#define PRINT_FORMAT "%d"


/*-----=  My Int Functions  =-----*/


/**
 * @brief Allocate memory for a pointer to an int which contain duplication of the given key.
 *        If run out of memory, free all the memory that was already allocated by the function,
 *        report error MEM_OUT to the standard error and return NULL.
 * @param i The key to clone which holds an int data.
 * @return Return the clone key if succeed, NULL otherwise.
 */
void * cloneInt(const void *i)
{
    assert(i != NULL);

    int *pClone = NULL;
    pClone = malloc(sizeof(*(int *)i));
    if (pClone != NULL)
    {
        *pClone = *(int *)i;
    }
    else
    {
        reportError(MEM_OUT);
    }
    return pClone;
}

/**
 * @brief Free all the memory allocated for the given object.
 * @param i The key to free which holds an int data.
 */
void freeInt(void *i)
{
    if (i != NULL)
    {
        free(i);
    }
}

/**
 * @brief Generates the Hash Code of the given key for HashTable with size tableSize.
 *        The Hash Code is the given key modulus the Table size.
 * @param key The key to generate Hash Code to.
 * @param tableSize The size of the Hash Table.
 * @return A Number between 0 - (tableSize-1) or negative number in case of an error.
 */
int intFcn(const void *key, size_t tableSize)
{
    int hashCode = INVALID_HASH_CODE;

    if (key != NULL)
    {
        int intKey = (*(int *)key);
        hashCode = intKey % ((int)tableSize);
        if (hashCode < 0)
        {
            // If the key is negative, the '%' will not return the modulus but the remainder,
            // so we need to alter our Hash Code.
            hashCode = hashCode + ((int)tableSize);
        }
    }

    return hashCode;
}

/**
 * @brief Prints the given key to the standard output.
 * @param key The key to print.
 */
void intPrint(const void *key)
{
    if (key != NULL)
    {
        int intKey = (*(int *)key);
        printf(PRINT_FORMAT, intKey);
    }
}

/**
 * @brief Compares 2 given keys.
 * @param key1 The first key to compare.
 * @param key2 The second key to compare.
 * @return 0 if both keys are equal, otherwise return a non-zero value.
 */
int intCompare(const void * key1, const void * key2)
{
    if ((key1 != NULL) && (key2 != NULL))
    {
        int intKey1 = (*(int *)key1);
        int intKey2 = (*(int *)key2);

        if (intKey1 == intKey2)
        {
            return EQUALITY_VALUE;
        }

        return NON_EQUALITY_VALUE;
    }
    return COMPARE_ERROR_VALUE;
}
