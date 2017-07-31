CC= gcc
CFLAGS= -c -Wextra -Wvla -Wall -std=c99 -DNDEBUG
CODEFILES= ex3.tar GenericHashTable.c MyStringFunctions.c MyIntFunctions.c MyStringFunctions.h MyIntFunctions.h Key.h Makefile
MAXROWELEMENTS= -D MAX_ROW_ELEMENTS=2
LIBOBJECTS= GenericHashTable.o


# Default
default: HashIntSearch
	./HashIntSearch 1 0


# Executables
GenericHashTable: $(LIBOBJECTS)
	ar rcs libgenericHashTable.a $(LIBOBJECTS)

HashIntSearch: GenericHashTable HashIntSearch.o MyIntFunctions.o TableErrorHandle.o
	$(CC) HashIntSearch.o MyIntFunctions.o TableErrorHandle.o -L. -lgenericHashTable -o HashIntSearch

HashStrSearch: GenericHashTable HashStrSearch.o MyStringFunctions.o TableErrorHandle.o
	$(CC) HashStrSearch.o MyStringFunctions.o TableErrorHandle.o -L. -lgenericHashTable -o HashStrSearch


# Object Files
GenericHashTable.o: GenericHashTable.c GenericHashTable.h TableErrorHandle.h Key.h
	$(CC) $(CFLAGS) $(MAXROWELEMENTS) GenericHashTable.c -o GenericHashTable.o

HashIntSearch.o: HashIntSearch.c GenericHashTable.h MyIntFunctions.h
	$(CC) $(CFLAGS) HashIntSearch.c -o HashIntSearch.o

HashStrSearch.o: HashStrSearch.c GenericHashTable.h MyStringFunctions.h
	$(CC) $(CFLAGS) HashStrSearch.c -o HashStrSearch.o

MyIntFunctions.o: MyIntFunctions.c MyIntFunctions.h Key.h
	$(CC) $(CFLAGS) MyIntFunctions.c -o MyIntFunctions.o

MyStringFunctions.o: MyStringFunctions.c MyStringFunctions.h Key.h
	$(CC) $(CFLAGS) MyStringFunctions.c -o MyStringFunctions.o

TableErrorHandle.o: TableErrorHandle.c TableErrorHandle.h
	$(CC) $(CFLAGS) TableErrorHandle.c -o TableErrorHandle.o


# tar
tar:
	tar -cvf $(CODEFILES)


# Other Targets
clean:
	-rm -vf *.o GenericHashTable HashIntSearch HashStrSearch GenericHashTable.o HashIntSearch.o HashStrSearch.o MyIntFunctions.o MyStringFunctions.o TableErrorHandle.o libgenericHashTable.a

