/**
 * @file GenericHashTable.c
 * @author Itai Tagar <itagar>
 * @version 2.7.1
 * @date 23 Aug 2016
 *
 * @brief A program that handles a Generic Hah Table data structure and support
 *        several operations that associated with this data structure.
 *
 * @section LICENSE
 * This program is free to use in every operation system.
 *
 * @section DESCRIPTION
 * A program that handles a Generic Hah Table data structure and support several operations
 * that associated with this data structure.
 * Input:       No particular input.
 * Process:     The program handles the entire process of several operations on the Hash Table.
 *              The user is revealed only to a Pointer for the Hash Table, and all the
 *              Data Structure and the functions implementation are hidden.
 *              Also, the Hash Table implementation is Generic, which means that the user determine
 *              a specific data type in run-time.
 * Output:      Depends on the specific driver that used these operations.
 *              The program outputs several results to the standard output and errors
 *              to the standard error output.
 */


/*-----=  Includes  =-----*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "TableErrorHandle.h"
#include "GenericHashTable.h"


/*-----=  Definitions  =-----*/


/**
 * @def INITIAL_INDEX 0
 * @brief A Macro that sets the initial index in the Hash Table.
 */
#define INITIAL_INDEX 0

/**
 * @def INVALID_INDEX -1
 * @brief A Macro that sets the value of an invalid index.
 */
#define INVALID_INDEX -1

/**
 * @def MINIMAL_TABLE_SIZE 1
 * @brief A Macro that sets the minimal size of a Hash Table.
 */
#define MINIMAL_TABLE_SIZE 1

/**
 * @def INITIAL_SIZE_FACTOR 1
 * @brief A Macro that sets the initial size factor for the Hash Table.
 */
#define INITIAL_SIZE_FACTOR 1

/**
 * @def RESIZE_FACTOR 2
 * @brief A Macro that sets the factor for the table resize process.
 */
#define RESIZE_FACTOR 2

/**
 * @def NO_ELEMENTS 0
 * @brief A Macro that sets the number for no elements in the Hash Table.
 */
#define NO_ELEMENTS 0

#ifndef MAX_ROW_ELEMENTS
/**
 * @def MAX_ROW_ELEMENTS 2
 * @brief A Macro that sets the max number of elements in a single cell in the Hash Table.
 */
#define MAX_ROW_ELEMENTS 2
#endif

/**
 * @def CELL_PRINT "[%d]"
 * @brief A Macro that sets the output format of printing a cell while calling the print function.
 */
#define CELL_PRINT "[%d]"

/**
 * @def END_OF_CELL_PRINT "\t\n"
 * @brief A Macro that sets the output format of end of cell print while calling the print function.
 */
#define END_OF_CELL_PRINT "\t\n"

/**
 * @def PREFIX_ELEMENT_PRINT "\t"
 * @brief A Macro that sets the output of prefix before the print of key and data while printing.
 */
#define PREFIX_ELEMENT_PRINT "\t"

/**
 * @def SUFFIX_ELEMENT_PRINT "\t-->"
 * @brief A Macro that sets the output of suffix after the print of key and data while printing.
 */
#define SUFFIX_ELEMENT_PRINT "\t-->"

/**
 * @def SEPARATOR_PRINT ","
 * @brief A Macro that sets the output of the separator between key and data while printing.
 */
#define SEPARATOR_PRINT ","


/*-----=  Type Definitions  =-----*/


/**
 * BucketP is a pointer for a Bucket.
 */
typedef struct Bucket *BucketP;

/**
 * ElementP is a pointer for an Element in Bucket.
 */
typedef struct Element *ElementP;


/*-----=  Structs  =-----*/


/**
 * @brief A Structure representing a single Element in the Bucket.
 *        An Element contains it's key and it's data.
 *        The Element holds a pointer to the next Element associated to him.
 */
typedef struct Element
{
    KeyP key;
    DataP data;
    ElementP next;
} Element;

/**
 * @brief A Structure representing the Bucket, which is a Linked List of Elements.
 *        Each Bucket holds its size, which is it's capacity, and the current
 *        number of Elements in the Bucket. Also it holds a pointer to the head of
 *        the Bucket, i.e. the first Element in the Bucket.
 */
typedef struct Bucket
{
    ElementP head;
    size_t bucketSize;
    size_t numberOfElements;
} Bucket;

/**
 * @brief A Structure representing the Generic Hash Table.
 *        Each Hash Table holds its size, which is it's capacity, and the current
 *        number of Elements in the Hash Table. Also it holds a pointer for a pointer to a Bucket,
 *        and several pointers for the required functions associated with the Hash Table.
 *        The Hash Table also holds it's original size (used for the hash code calculations),
 *        and a track of the resize process of the Table.
 */
typedef struct Table
{
    // Table Properties.
    BucketP *table;
    size_t tableSize;
    size_t originalSize;
    int sizeFactor;

    // Table Functions.
    CloneKeyFcn cloneKey;
    FreeKeyFcn freeKey;
    HashFcn hfun;
    PrintKeyFcn printKeyFun;
    PrintDataFcn printDataFun;
    ComparisonFcn fcomp;
} Table;


/*-----=  Element Functions  =-----*/


/**
 * @brief Initialize an Element for the Hash Table with the given key and data.
 *        The function allocated memory for the new Element, if the allocation was failed at
 *        some point, the function will free all the memory that was already allocated
 *        and will return a NULL pointer.
 * @param key A pointer for the key of the Element.
 * @param object A pointer for the data of the Element.
 * @return A pointer for the new initialized Element, or NULL if the process failed.
 */
static ElementP initializeElement(KeyP key, DataP object)
{
    assert(key != NULL && object != NULL);

    ElementP pElement = NULL;
    pElement = (ElementP)malloc(sizeof(Element));
    // We continue the process only if the allocation for memory succeed.
    if (pElement != NULL)
    {
        pElement -> data = object;
        pElement -> key = key;
        pElement -> next = NULL;
    }

    return pElement;
}

/**
 * @brief Frees the memory and resources allocated to the given Element.
 *        If the given Element is NULL, no operation is performed.
 *        The function returns the next Element of the Element we are about to free.
 * @param pElement A pointer to the Element to free.
 * @param freeKey A pointer for the Free Key function.
 * @return A pointer to the next Element of the Element we are about to free.
 */
static ElementP freeElement(ElementP pElement, FreeKeyFcn freeKey)
{
    assert(freeKey != NULL);

    ElementP pNext = NULL;
    if (pElement != NULL)
    {
        freeKey(pElement -> key);
        pElement -> key = NULL;

        pNext = pElement -> next;
        pElement -> next = NULL;

        free(pElement);
    }
    return pNext;
}

/**
 * @brief Return the pointer to the Element that exist in the table in cell number arrCell
 *        (where 0 is the first cell), and placement at listNode in the list
 *        (when 0 is the first node in the list, i.e. the node that is pointed from the
 *        table itself). If such Element not exist return NULL
 * @param table A pointer to the Table to search.
 * @param arrCell The cell number in the Hash Table.
 * @param listNode The placement of the Element in the specific cell number.
 * @return A pointer to the Element in the desired place, if such Element not exist return NULL.
 */
static ElementP reachElement(const TableP pTable, int arrCell, int listNode)
{
    assert(pTable != NULL);
    assert(arrCell >= INITIAL_INDEX || arrCell < (int)(pTable -> tableSize) - 1);

    BucketP currentBucket = NULL;
    currentBucket = (pTable -> table)[arrCell];
    assert(currentBucket != NULL);
    assert(listNode >= INITIAL_INDEX);

    if (listNode >= (int)(currentBucket -> bucketSize))
    {
        return NULL;
    }

    ElementP pElement = NULL;
    pElement = currentBucket -> head;

    int i = INITIAL_INDEX;
    while ((i < listNode) && (pElement != NULL))
    {
        pElement = pElement -> next;
        i++;
    }

    return pElement;
}


/*-----=  Bucket Functions  =-----*/


/**
 * @brief Initialize a new Bucket in the Hash Table with the given Bucket size.
 *        The function allocated memory for the new Bucket, if the allocation was failed at
 *        some point, the function will free all the memory that was already allocated
 *        and will return a NULL pointer.
 * @param bucketSize The size of the Bucket, i.e. the number of Elements each Bucket contains.
 * @return A pointer for the new initialized Bucket, or NULL if the process failed.
 */
static BucketP initializeBucket(size_t const bucketSize)
{
    assert((int)bucketSize >= NO_ELEMENTS);

    BucketP pBucket = NULL;
    pBucket = (BucketP)malloc(sizeof(Bucket));
    // We continue the process only if the allocation for memory succeed.
    if (pBucket != NULL)
    {
        pBucket -> head = NULL;
        pBucket -> bucketSize = bucketSize;
        pBucket -> numberOfElements = NO_ELEMENTS;
    }

    return pBucket;
}

/**
 * @brief Frees the memory and resources allocated to the given Bucket.
 *        If the given Bucket is NULL, no operation is performed.
 * @param pBucket A pointer to the Bucket to free.
 * @param freeKey A pointer for the Free Key function.
 */
static void freeBucket(BucketP pBucket, FreeKeyFcn freeKey)
{
    assert(freeKey != NULL);

    if (pBucket != NULL)
    {
        ElementP currentElement = pBucket -> head;

        // Free each Element in the Bucket.
        while (currentElement != NULL)
        {
            currentElement = freeElement(currentElement, freeKey);
        }

        // Free the Bucket itself.
        free(pBucket);
    }
}

/**
 * @brief Insert a new Element to the Bucket with the given key and data.
 *        The function creates a new Element and add it to the end of the Bucket chain.
 *        If the process fail for any reason, the function free all the
 *        allocated memory and return false.
 * @param pBucket A pointer to the Bucket to insert to.
 * @param key The key of the new Element to insert.
 * @param object The data of the new Element to insert.
 * @return true (non-zero value) if completed with no errors, false (zero value) otherwise.
 */
static int bucketInsertElement(BucketP pBucket, KeyP key, DataP object)
{
    assert(pBucket != NULL && key != NULL && object != NULL);

    ElementP newElement = initializeElement(key, object);
    if (newElement != NULL)
    {
        // Insert new Element to the end of the Bucket.
        ElementP previousElement = NULL;
        ElementP currentElement = pBucket -> head;
        while (currentElement != NULL)
        {
            previousElement = currentElement;
            currentElement = currentElement -> next;
        }

        // Update the last Element to point to the new Element.
        if (previousElement == NULL)
        {
            pBucket -> head = newElement;
        }
        else
        {
            previousElement -> next = newElement;
        }

        assert((pBucket -> numberOfElements) < (pBucket -> bucketSize));
        (pBucket -> numberOfElements)++;
        return true;
    }

    return false;
}

/**
 * @brief Removes the Element represented by the given key from the Bucket.
 *        If everything is OK, return the pointer to the ejected data, otherwise return NULL.
 * @param pBucket A pointer to the Bucket to remove from.
 * @param key The key to remove.
 * @param fcomp A pointer for the Key Comparison function.
 * @param freeKey A pointer for the Free Key function.
 * @return A pointer for the ejected data if succeed, otherwise return NULL.
 */
static DataP bucketRemoveElement(BucketP pBucket, ConstKeyP key, ComparisonFcn fcomp,
                                 FreeKeyFcn freeKey)
{
    assert((pBucket != NULL) && (key != NULL));
    assert((fcomp != NULL) && (freeKey != NULL));

    DataP removedItem = NULL;

    ElementP previousElement = NULL;
    ElementP currentElement = pBucket -> head;
    while (currentElement != NULL)
    {
        if (!fcomp(currentElement -> key, key))  // 'fcomp' returns 0 if the keys are equal.
        {
            // In case we found the matching Element, we store it's data, remove it from the Bucket
            // and free it's memory.
            removedItem = currentElement -> data;
            if (previousElement == NULL)
            {
                // In case the Element to delete is the Head of the Bucket.
                pBucket -> head = freeElement(currentElement, freeKey);
            }
            else
            {
                previousElement -> next = freeElement(currentElement, freeKey);
            }
            (pBucket -> numberOfElements)--;
            break;
        }
        else
        {
            previousElement = currentElement;
            currentElement = currentElement -> next;
        }
    }
    return removedItem;
}

/**
 * @brief Search the Bucket and look for an object with the given key.
 *        If such object is found fill its its placement in the list into listNode
 *        (when 0 is the first node in the Bucket).
 *        If the key was not found, fill both pointers with value of -1.
 * @param pBucket A pointer to the Bucket to search in.
 * @param key The key to search.
 * @param listNode A pointer to update with the proper Node placement.
 * @param fcomp A pointer to the Comparison function.
 * @return A pointer to the data if found, otherwise return NULL.
 */
static DataP bucketFindData(const BucketP pBucket, ConstKeyP key, int *listNode,
                            ComparisonFcn fcomp)
{
    assert((pBucket != NULL) && (key != NULL));
    assert((fcomp != NULL) && (listNode != NULL));

    DataP foundData = NULL;

    int bucketPlacement = INITIAL_INDEX;
    ElementP currentElement = NULL;
    currentElement = pBucket -> head;
    while (currentElement != NULL)
    {
        if (!fcomp(currentElement -> key, key))  // 'fcomp' returns 0 if the keys are equal.
        {
            *listNode = bucketPlacement;
            foundData = currentElement -> data;
            break;
        }
        currentElement = currentElement -> next;
        bucketPlacement++;
    }

    return foundData;
}

/**
 * @brief Print the given Bucket from the Hash Table.
 *        The function prints all the Elements in the given Bucket with the desired pattern.
 * @param pBucket A pointer to the Bucket to print.
 * @param printKeyFun A pointer for the Print Key function.
 * @param printDataFun A pointer for the Print Data function.
 */
static void printBucket(const BucketP pBucket, PrintKeyFcn printKeyFun, PrintDataFcn printDataFun)
{
    ElementP currentElement = NULL;

    currentElement = pBucket -> head;
    while (currentElement != NULL)
    {
        // Print the current Element.
        printf(PREFIX_ELEMENT_PRINT);
        printKeyFun(currentElement -> key);
        printf(SEPARATOR_PRINT);
        printDataFun(currentElement -> data);
        printf(SUFFIX_ELEMENT_PRINT);
        currentElement = currentElement -> next;

    }
}


/*-----=  Table Functions  =-----*/


/**
 * @brief Generates the proper HashCode that accustomed to the given Hash Table for the given key.
 * @param pTable A pointer to the Hash Table.
 * @param key A pointer to the key to generate HashCode.
 * @return The modified HashCode that accustomed to the given Hash Table.
 */
static inline int generateHashCode(TableP pTable, ConstKeyP key)
{
    assert(pTable != NULL && key != NULL);
    return (pTable -> sizeFactor) * (pTable -> hfun)(key, pTable -> originalSize);
}

/**
 * @brief Updates the current size factor for the given Hash Table.
 *        The size factor is the Resize Factor raised to the number of times we resize the Table.
 *        where the Resize Factor is the number in which we multiply the table size each
 *        time we resize it.
 * @param pTable A pointer to the Hash Table to update.
 */
static inline void updateSizeFactor(TableP pTable)
{
    assert(pTable != NULL);
    pTable -> sizeFactor *= RESIZE_FACTOR;
}

/**
 * @brief Allocate a Bucket in each cell in the given Table (a pointer to pointer to Buckets).
 * @param table The Table to set.
 * @param tableSize The size of the Table.
 * @param freeKey The pointer for the Free Key Function.
 * @return true if the process succeed, false otherwise.
 */
static bool setTableBuckets(BucketP *table, size_t tableSize, FreeKeyFcn freeKey)
{
    assert(table != NULL && freeKey != NULL);

    // Attempt to set each Bucket in the Table.
    int currentIndex = INITIAL_INDEX;
    while (currentIndex < (int)tableSize)
    {
        BucketP pBucket = initializeBucket(MAX_ROW_ELEMENTS);
        if (pBucket == NULL)
        {
            // If memory allocation failed, we stop the process.
            break;
        }
        table[currentIndex] = pBucket;
        currentIndex++;
    }

    if (currentIndex != (int)tableSize)
    {
        // In this case, the While Loop broke during the run, which means that some
        // memory allocation was failed, so we will free all the memory that was already allocated.
        for (int j = INITIAL_INDEX; j < currentIndex; j++)
        {
            freeBucket(table[currentIndex], freeKey);
            table[currentIndex] = NULL;
        }
        return false;
    }
    return true;
}

/**
 * @brief Initialize a new Hash Table with the given table size.
 *        The function allocated memory for the new Hash Table, if the allocation was failed at
 *        some point, the function will free all the memory that was already allocated
 *        and will return a NULL pointer.
 * @param tableSize The size of the Hash Table to set.
 * @param cloneKey A pointer for the Key Cloning function.
 * @param freeKey A pointer for the Free Key function.
 * @param hfun A pointer for the Hash function.
 * @param printKeyFun A pointer for the Print Key function.
 * @param printDataFun A pointer for the Print Data function.
 * @param fcomp A pointer for the Key Comparison function.
 * @return A pointer for the new initialized Hash Table, or NULL if the process failed.
 */
static TableP initializeTable(size_t tableSize, CloneKeyFcn cloneKey, FreeKeyFcn freeKey,
                              HashFcn hfun, PrintKeyFcn printKeyFun, PrintDataFcn printDataFun,
                              ComparisonFcn fcomp)
{
    assert(cloneKey != NULL && freeKey != NULL && hfun != NULL && printKeyFun != NULL
           && printDataFun != NULL && fcomp != NULL);

    TableP pTable = NULL;

    pTable = (TableP)malloc(sizeof(Table));
    // We continue the process only if the allocation for memory succeed.
    if (pTable != NULL)
    {
        pTable -> table = (BucketP *)malloc(tableSize * sizeof(BucketP));
        // We continue the process only if the allocation for memory succeed.
        if ((pTable -> table) != NULL)
        {
            pTable -> tableSize = tableSize;
            pTable -> originalSize = tableSize;
            pTable -> sizeFactor = INITIAL_SIZE_FACTOR;

            // Set the Bucket in the Hash Table.
            if (setTableBuckets((pTable -> table), tableSize, freeKey))
            {
                // If the allocation for memory succeed.
                // Assign the given functions to the Hash Table.
                pTable -> cloneKey = cloneKey;
                pTable -> freeKey = freeKey;
                pTable -> hfun = hfun;
                pTable -> printKeyFun = printKeyFun;
                pTable -> printDataFun = printDataFun;
                pTable -> fcomp = fcomp;
            }
            else
            {
                // If memory allocation failed, we free all the memory that was already allocated.
                free(pTable -> table);
                pTable -> table = NULL;
                free(pTable);
                pTable = NULL;
            }
        }
        else
        {
            // If memory allocation failed, we free all the memory that was already allocated.
            free(pTable);
            pTable = NULL;
        }
    }
    return pTable;
}

/**
 * @brief Resize the Hash Table and allocate the current Elements in the updated cells in the Table.
 * @param pTable A pointer to the Hash Table to resize.
 * @return true if the process succeed, false if out of memory.
 */
static bool resizeTable(TableP pTable)
{
    assert(pTable != NULL);

    size_t currentSize = (pTable -> tableSize);
    size_t newSize = currentSize * RESIZE_FACTOR;

    BucketP *newTable = (BucketP *)malloc(newSize * sizeof(BucketP));
    if (newTable != NULL)
    {
        // Set the Buckets in the new Table.
        if (setTableBuckets(newTable, newSize, pTable -> freeKey))
        {
            for (int i = INITIAL_INDEX; i < (int)currentSize; i++)
            {
                assert((i * RESIZE_FACTOR) < (int)newSize);
                freeBucket(newTable[i * RESIZE_FACTOR], pTable -> freeKey);
                newTable[i * RESIZE_FACTOR] = (pTable -> table)[i];
            }

            // Update the Hash Table size.
            (pTable -> tableSize) = newSize;
            updateSizeFactor(pTable);

            // Release the old Table.
            free(pTable -> table);
            pTable -> table = newTable;
            return true;
        }
        free(newTable);
    }
    return false;
}

/**
 * @brief Allocate memory for a Hash Table with which uses the given functions.
 *        If run out of memory, free all the memory that was already allocated by the function,
 *        report error MEM_OUT to the standard error and return NULL.
 * @param tableSize The number of cells in the hash table.
 * @param cloneKey A pointer for the Key Cloning function.
 * @param freeKey A pointer for the Free Key function.
 * @param hfun A pointer for the Hash function.
 * @param printKeyFun A pointer for the Print Key function.
 * @param printDataFun A pointer for the Print Data function.
 * @param fcomp A pointer for the Key Comparison function.
 * @return A pointer for the new allocated Hash Table if allocation was successful,
 *         otherwise return NULL.
 */
TableP createTable(size_t tableSize, CloneKeyFcn cloneKey, FreeKeyFcn freeKey, HashFcn hfun,
                   PrintKeyFcn printKeyFun, PrintDataFcn printDataFun, ComparisonFcn fcomp)
{
    TableP pTable = NULL;

    if (tableSize < MINIMAL_TABLE_SIZE)
    {
        reportError(GENERAL_ERROR);
        return NULL;
    }

    if (cloneKey == NULL || freeKey == NULL || hfun == NULL
        || printKeyFun == NULL || printDataFun == NULL || fcomp == NULL)
    {
        // If some function input is invalid, we can't continue our program.
        reportError(GENERAL_ERROR);
        return NULL;
    }

    pTable = initializeTable(tableSize, cloneKey, freeKey, hfun, printKeyFun,
                             printDataFun, fcomp);
    if (pTable == NULL)
    {
        // If some part of the memory allocation for the Hash Table was failed.
        reportError(MEM_OUT);
    }

    return pTable;
}

/**
 * @brief Insert an object to the Hash Table with key.
 *        If all the cells appropriate for this object are full, duplicate the table.
 *        If run out of memory, report MEM_OUT and do nothing (the table should stay at
 *        the same situation as it was before the duplication).
 *        If everything is OK, return true, otherwise (an error occurred) return false.
 * @param table A pointer for the Hash Table to insert to.
 * @param key The key to insert.
 * @param object The object that is stored by the given key.
 * @return true (non-zero value) if completed with no errors, false (zero value) otherwise.
 */
int insert(TableP table, const void *key, DataP object)
{
    if (table == NULL || key == NULL || object == NULL)
    {
        reportError(GENERAL_ERROR);
        return false;
    }

    // If the given key is already exists in the Hash Table, we replace it's data with the new data.
    int arrCell = INVALID_INDEX;
    int listNode = INVALID_INDEX;
    if (findData(table, key, &arrCell, &listNode))
    {
        if ((arrCell != INVALID_INDEX) && (listNode != INVALID_INDEX))
        {
            ElementP pElement = reachElement(table, arrCell, listNode);
            assert(pElement != NULL);
            pElement -> data = object;
            return true;
        }
    }

    // Generate the Hash Code for the given key.
    int hashCode = generateHashCode(table, key);
    // If the Hash Code is lower than the lower bound, it means there was an error and
    // we can't continue with the insertion process.
    if (hashCode < HASH_CODE_LOWER_BOUND)
    {
        reportError(GENERAL_ERROR);
        return false;
    }
    assert(hashCode <= (int)(table -> tableSize) - 1);

    BucketP currentBucket = NULL;
    // Iterate through the possible Buckets to insert.
    for (int i = INITIAL_INDEX; i < (table -> sizeFactor); i++)
    {
        currentBucket = (table -> table)[hashCode + i];
        assert(currentBucket != NULL);

        if ((currentBucket -> numberOfElements) < (currentBucket -> bucketSize))
        {
            // If we enter this Scope, the current Bucket has place to store the new Element.

            // Clone the key.
            KeyP cloneKey = NULL;
            cloneKey = (table -> cloneKey)(key);
            if (cloneKey == NULL)
            {
                // The cloneKey function already reports of MEM_OUT.
                return false;
            }

            if (!bucketInsertElement(currentBucket, cloneKey, object))
            {
                // If 'bucketInsertElement' return false, it means that there
                // wasn't enough memory to allocate the new Element.
                reportError(MEM_OUT);
                return false;
            }
            return true;
        }
    }

    // If all the associated Buckets are not available, we have to resize the Hash Table.
    if (resizeTable(table))
    {
        // Attempt to try again inserting the new key.
        return (insert(table, key, object));
    }
    else
    {
        // If 'resizeTable' return false, it means that there
        // wasn't enough memory to allocate the new Table.
        reportError(MEM_OUT);
        return false;
    }
}

/**
 * @brief Remove a data from the Hash Table.
 *        If everything is OK, return the pointer to the ejected data, otherwise return NULL.
 * @param table A pointer for the Hash Table to remove from.
 * @param key The key to remove.
 * @return pointer to the ejected data if the remove process succeed, otherwise return NULL.
 */
DataP removeData(TableP table, const void *key)
{
    if (table == NULL || key == NULL)
    {
        reportError(GENERAL_ERROR);
        return NULL;
    }

    DataP removedData = NULL;

    // Track the desired Element to remove.
    int arrCell = INVALID_INDEX;
    int listNode = INVALID_INDEX;
    if (findData(table, key, &arrCell, &listNode))
    {
        if ((arrCell != INVALID_INDEX) && (listNode != INVALID_INDEX))
        {
            BucketP currentBucket = NULL;
            currentBucket = (table -> table)[arrCell];
            assert(currentBucket != NULL);

            removedData = bucketRemoveElement(currentBucket, key, table -> fcomp, table -> freeKey);
        }
    }
    return removedData;
}

/**
 * @brief Search the table and look for an object with the given key.
 *        If such object is found fill its cell number into arrCell (where 0 is the first cell),
 *        and its placement in the list into listNode (when 0 is the first node in the list,
 *        i.e. the node that is pointed from the table itself).
 *        If the key was not found, fill both pointers with value of -1.
 * @param table A pointer for the Hash Table to search in.
 * @param key The key to search.
 * @param arrCell A pointer to update with the proper cell number.
 * @param listNode A pointer to update with the proper Node placement.
 * @return A pointer to the data if found, otherwise return NULL.
 */
DataP findData(const TableP table, const void *key, int *arrCell, int *listNode)
{
    if (table == NULL || key == NULL || arrCell == NULL || listNode == NULL)
    {
        reportError(GENERAL_ERROR);
        return NULL;
    }

    DataP foundData = NULL;
    *arrCell = INVALID_INDEX;
    *listNode = INVALID_INDEX;

    // Generate the Hash Code for the given key.
    int hashCode = generateHashCode(table, key);
    // If the Hash Code is lower than the lower bound, it means there was an error and
    // we can't continue with the search process.
    if (hashCode < HASH_CODE_LOWER_BOUND)
    {
        reportError(GENERAL_ERROR);
        return NULL;
    }
    assert(hashCode <= (int)(table -> tableSize) - 1);

    BucketP currentBucket = NULL;
    // Iterate through the possible Buckets to search.
    for (int i = INITIAL_INDEX; i < (table -> sizeFactor); i++)
    {
        // Find the proper Bucket to search the key.
        currentBucket = (table -> table)[hashCode + i];
        assert(currentBucket != NULL);

        // Search inside the current Bucket.
        foundData = bucketFindData(currentBucket, key, listNode, table -> fcomp);
        if (foundData != NULL)
        {
            *arrCell = hashCode + i;
            break;
        }
    }
    return foundData;
}

/**
 * @brief Return a pointer to the data that exist in the table in cell number arrCell
 *        (where 0 is the first cell), and placement at listNode in the list
 *        (when 0 is the first node in the list, i.e. the node that is pointed from the
 *        table itself). If such data not exist return NULL
 * @param table A pointer to the Table to search.
 * @param arrCell The cell number in the Hash Table.
 * @param listNode The placement of the Element in the specific cell number.
 * @return A pointer to the data in the desired place, if such data not exist return NULL.
 */
DataP getDataAt(const TableP table, int arrCell, int listNode)
{
    if (table == NULL)
    {
        reportError(GENERAL_ERROR);
        return NULL;
    }

    // If the given indices are out of bounds, there is no need to search.
    if (listNode < INITIAL_INDEX || arrCell < INITIAL_INDEX
        || arrCell > (int)(table -> tableSize) - 1)
    {
        return NULL;
    }

    DataP foundData = NULL;
    ElementP pElement = NULL;
    pElement = reachElement(table, arrCell, listNode);
    if (pElement != NULL)
    {
        foundData = pElement -> data;
    }
    return foundData;
}

/**
 * @brief Return the pointer to the key that exist in the table in cell number arrCell
 *        (where 0 is the first cell), and placement at listNode in the list
 *        (when 0 is the first node in the list, i.e. the node that is pointed from the
 *        table itself). If such key not exist return NULL
 * @param table A pointer to the Table to search.
 * @param arrCell The cell number in the Hash Table.
 * @param listNode The placement of the Element in the specific cell number.
 * @return A pointer to the key in the desired place, if such key not exist return NULL.
 */
ConstKeyP getKeyAt(const TableP table, int arrCell, int listNode)
{
    if (table == NULL)
    {
        reportError(GENERAL_ERROR);
        return NULL;
    }

    // If the given indices are out of bounds, there is no need to search.
    if (listNode < INITIAL_INDEX || arrCell < INITIAL_INDEX
        || arrCell > (int)(table -> tableSize) - 1)
    {
        return NULL;
    }

    ConstKeyP foundKey = NULL;
    ElementP pElement = NULL;
    pElement = reachElement(table, arrCell, listNode);
    if (pElement != NULL)
    {
        foundKey = pElement -> key;
    }
    return foundKey;
}

/**
 * @brief Free all the memory allocated for the table.
 *        It's the user responsibility to call this function before exiting the program.
 * @param table A pointer to the Table to free.
 */
void freeTable(TableP table)
{
    if (table != NULL)
    {
        if (table -> table != NULL)
        {
            // Free each Bucket and each Element in the Hash Table.
            for (int i = INITIAL_INDEX; i < (int)(table -> tableSize); i++)
            {
                freeBucket((table -> table)[i], table -> freeKey);
                (table -> table)[i] = NULL;
            }

            free(table -> table);
            table -> table = NULL;
        }
        free(table);
    }
}

/**
 * @brief Print the Hash Table (use the format presented in PrintTableExample).
 * @param table A pointer to the Hash Table to print.
 */
void printTable(const TableP table)
{
    if (table == NULL)
    {
        reportError(GENERAL_ERROR);
    }
    else
    {
        BucketP currentBucket = NULL;
        for (int i = INITIAL_INDEX; i < (int)(table -> tableSize); i++)
        {
            // Print the current Bucket.
            currentBucket = (table -> table)[i];
            assert(currentBucket != NULL);
            printf(CELL_PRINT, i);

            // Print Bucket's Elements.
            printBucket(currentBucket, table -> printKeyFun, table -> printDataFun);

            printf(END_OF_CELL_PRINT);
        }
    }

}

