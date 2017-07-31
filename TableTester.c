#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>
#include "GenericHashTable.h"
#include "MyIntFunctions.h"
#include "MyStringFunctions.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Insert only memory.");
    }
    struct rlimit rl;
    getrlimit (RLIMIT_AS, &rl);
    printf("\n Default value is : %lld\n", (long long int)rl.rlim_cur);
    rl.rlim_cur = (size_t)atoi(argv[1]);
    setrlimit (RLIMIT_AS, &rl);
    getrlimit (RLIMIT_AS, &rl);
    printf("\n Default value now is : %lld\n", (long long int)rl.rlim_cur);



    TableP pTable = createTable(2, cloneInt, freeInt, intFcn, intPrint, intPrint, intCompare);
    if (pTable != NULL)
    {
        printf("Creating Hash Table in size: %d \n\n", 2);

        printf("------------------  Add  ------------------\n\n");

        int addArray[10] = {543, 6543, -22, 3, 4, -5, 10, 0, 6543, 22};
        for (int i = 0; i < 10; i++)
        {
            printf("Adding the value: %4d\n", addArray[i]);
            insert(pTable, &(addArray[i]), &(addArray[i]));
            printTable(pTable);
            printf("\n");
        }

        printf("------------------  Search  ------------------\n\n");

        int arrCell = -1;
        int listNode = -1;
        int searchArray[10] = {5, -22, 32, 0, 6543, -22, 1, 543, 3, 4};
        for (int i = 0; i < 10; i++)
        {
            printf("Searching the value: %4d\n", searchArray[i]);
            findData(pTable, &(searchArray[i]), &arrCell, &listNode);
            if (arrCell != -1 && listNode != -1)
            {
                printf("The desired value is in cell number %d, placement number %d\n", arrCell, listNode);
            }
            else
            {
                printf("Can't find what you're looking for...\n");
            }
            printf("\n");
        }

        printf("------------------  Remove  ------------------\n\n");

        int removeArray[10] = {15, -22, -22, 0, 6543, -22, 1, 543, 3, 4};
        for (int i = 0; i < 10; i++)
        {
            printf("Removing the value: %4d\n", removeArray[i]);
            removeData(pTable, &(removeArray[i]));
            printTable(pTable);
            printf("\n");
        }

        printf("------------------  Add  ------------------\n\n");

        int addArray2[5] = {1, -1, 1, 2, 9};
        for (int i = 0; i < 5; i++)
        {
            printf("Adding the value: %4d\n", addArray2[i]);
            insert(pTable, &(addArray2[i]), &(addArray2[i]));
            printTable(pTable);
            printf("\n");
        }

        freeTable(pTable);
    }

    printf("\n\n------------------  FIN  ------------------\n\n");

}



