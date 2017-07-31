#ifndef _MY_INT_FUNCTIONS_H_
#define _MY_INT_FUNCTIONS_H_

/**
 * @file MyIntFunctions.h
 * @author Itai Tagar <itagar>
 * @version 1.0
 * @date 23 Aug 2016
 *
 * @brief A Header file for the Int Functions. It defines several Macros and declares Functions
 *        for an Int Key, to use during the Generic Hash Table operations
 *        which holds Integers as keys.
 *
 * @section LICENSE
 * This program is free to use in every operation system.
 *
 * @section DESCRIPTION
 * A Header file for the Int Functions. It defines several Macros and declares Functions
 * for an Int Key, to use during the Generic Hash Table operations which holds Integers as keys.
 * Input:       No particular input.
 * Process:     This file declares several functions that are relevant to the Hash Table operations.
 * Output:      No particular output.
 */


/*-----=  Includes  =-----*/


#include "Key.h"


/*-----=  Forward Declarations  =-----*/


/**
 * @brief Allocate memory for a pointer to an int which contain duplication of the given key.
 *        If run out of memory, free all the memory that was already allocated by the function,
 *        report error MEM_OUT to the standard error and return NULL.
 * @param i The key to clone which holds an int data.
 * @return Return the clone key if succeed, NULL otherwise.
 */
void * cloneInt(const void *i);

/**
 * @brief Free all the memory allocated for the given object.
 * @param i The key to free which holds an int data.
 */
void freeInt(void *i);

/**
 * @brief Generates the Hash Code of the given key for HashTable with size tableSize.
 *        The Hash Code is the given key modulus the Table size.
 * @param key The key to generate Hash Code to.
 * @param tableSize The size of the Hash Table.
 * @return A Number between 0 - (tableSize-1) or negative number in case of an error.
 */
int intFcn(const void *key, size_t tableSize);

/**
 * @brief Prints the given key to the standard output.
 * @param key The key to print.
 */
void intPrint(const void *key);

/**
 * @brief Compares 2 given keys.
 * @param key1 The first key to compare.
 * @param key2 The second key to compare.
 * @return 0 if both keys are equal, otherwise return a non-zero value.
 */
int intCompare(const void * key1, const void * key2);

#endif // _MY_INT_FUNCTIONS_H_
