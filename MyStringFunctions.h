#ifndef _MY_STR_FUNCTIONS_H_
#define _MY_STR_FUNCTIONS_H_

/**
 * @file MyStringFunctions.h
 * @author Itai Tagar <itagar>
 * @version 1.0
 * @date 23 Aug 2016
 *
 * @brief A file for the String Functions. It defines several Macros and declares Functions
 *        for an String Key, to use during the Generic Hash Table operations
 *        which holds Strings as keys.
 *
 * @section LICENSE
 * This program is free to use in every operation system.
 *
 * @section DESCRIPTION
 * A file for the String Functions. It defines several Macros and declares Functions
 * for an String Key, to use during the Generic Hash Table operations which holds Strings as keys.
 * Input:       No particular input.
 * Process:     This file declares several functions that are relevant to the Hash Table operations.
 * Output:      No particular output.
 */


/*-----=  Includes  =-----*/


#include "Key.h"


/*-----=  Forward Declarations  =-----*/


/**
 * @brief Allocate memory for a pointer to a string which contain duplication of the given key.
 *        If run out of memory, free all the memory that was already allocated by the function,
 *        report error MEM_OUT to the standard error and return NULL.
 * @param s The key to clone which holds an string data.
 * @return Return the clone key if succeed, NULL otherwise.
 */
void * cloneStr(const void *s);

/**
 * @brief Free all the memory allocated for the given object.
 * @param s The key to free which holds an string data.
 */
void freeStr(void *s);

/**
 * @brief Generates the Hash Code of the given key for HashTable with size tableSize.
 *        The Hash Code is the given key modulus the Table size.
 * @param s The key to generate Hash Code to.
 * @param tableSize The size of the Hash Table.
 * @return A Number between 0 - (tableSize-1) or negative number in case of an error.
 */
int strFcn(const void *s, size_t tableSize);

/**
 * @brief Prints the given key to the standard output.
 * @param s The key to print.
 */
void strPrint(const void *s);

/**
 * @brief Compares 2 given keys.
 * @param key1 The first key to compare.
 * @param key2 The second key to compare.
 * @return 0 if both keys are equal, otherwise return a non-zero value.
 */
int strCompare(const void * key1, const void * key2);

#endif // _MY_STR_FUNCTIONS_H_
