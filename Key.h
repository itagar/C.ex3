#ifndef _MY_KEY_H_
#define _MY_KEY_H_

/**
 * @file Key.h
 * @author Itai Tagar <itagar>
 * @version 1.1
 * @date 23 Aug 2016
 *
 * @brief A Header file for the Key Functions. It defines several Macros and Functions
 *        pointers for Generic Key, to use during the Generic Hash Table operations.
 *
 * @section LICENSE
 * This program is free to use in every operation system.
 *
 * @section DESCRIPTION
 * A Header file for the Key Functions. It defines several Macros and Functions
 * pointers for Generic Key, to use during the Generic Hash Table operations.
 * Input:       No particular input.
 * Process:     This file associate several pointer to functions that are relevant
 *              to the Hash Table operations.
 * Output:      No particular output.
 */


/*-----=  Includes  =-----*/


#include <stddef.h>
#include "TableErrorHandle.h"


/*-----=  Definitions  =-----*/


/**
 * @def HASH_CODE_LOWER_BOUND 0
 * @brief A Macro that sets the lower bound number for a generated Hash Code.
 */
#define HASH_CODE_LOWER_BOUND 0

/**
 * @def INVALID_HASH_CODE -1
 * @brief A Macro that sets a number to represent an invalid Hash Code.
 */
#define INVALID_HASH_CODE -1

/**
 * @def EQUALITY_VALUE 0
 * @brief A Macro that sets the value of equality when comparing keys.
 */
#define EQUALITY_VALUE 0

/**
 * @def NON_EQUALITY_VALUE 1
 * @brief A Macro that sets the value of non equality when comparing keys.
 */
#define NON_EQUALITY_VALUE 1

/**
 * @def COMPARE_ERROR_VALUE -1
 * @brief A Macro that sets the value of error when comparing keys.
 */
#define COMPARE_ERROR_VALUE -1


/*-----=  Type Definitions  =-----*/


/**
 * KeyP is a pointer for Generic Key, i.e. a void pointer.
 */
typedef void *KeyP;


/*-----=  Forward Declarations  =-----*/


/**
 * @brief Allocate memory for an object which contain duplication of the given key.
 *        If run out of memory, free all the memory that was already allocated by the function,
 *        report error MEM_OUT to the standard error and return NULL.
 * @param key The key to clone.
 * @return Return the clone key if succeed, NULL otherwise.
 */
typedef void * (*CloneKeyFcn)(const void * key);

/**
 * @brief Free all the memory allocated for an object.
 * @param key The object to free.
 */
typedef void (*FreeKeyFcn)(void * key);

/**
 * @brief Generates the Hash Code of the given key for HashTable with size tableSize.
 * @param key The key to generate Hash Code to.
 * @param tableSize The size of the Hash Table.
 * @return A Number between 0 - (tableSize-1) or negative number in case of an error.
 */
typedef int (*HashFcn)(const void * key, size_t tableSize);

/**
 * @brief Prints the given key to the standard output.
 * @param key The key to print.
 */
typedef void (*PrintKeyFcn)(const void * key);

/**
 * @brief Compares 2 given keys.
 * @param key1 The first key to compare.
 * @param key2 The second key to compare.
 * @return 0 if both keys are equal, otherwise return a non-zero value.
 */
typedef int (*ComparisonFcn)(const void * key1, const void * key2);

#endif  // _MY_KEY_H_