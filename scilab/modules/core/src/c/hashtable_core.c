/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <search.h>
#include "MALLOC.h"
#include "stack-def.h"
#include "hashtable_core.h"
#include "getfunctionslist.h"
#include "existfunction.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#define lfind _lfind
#endif
/*--------------------------------------------------------------------------*/
#define MAXLENGHTFUNCTIONNAME nlgh+1 /* 24 in scilab */
typedef struct primitive_table 
{
	unsigned int used;
	int keyIdentity[nsiz];
	int primitivePointer;
	char namefunction[MAXLENGHTFUNCTIONNAME];
} PRIMITIVES_TABLE;
/*--------------------------------------------------------------------------*/
static PRIMITIVES_TABLE *primitiveTableByName = NULL;
static PRIMITIVES_TABLE *primitiveTableByPrimitivePointer = NULL;
static PRIMITIVES_TABLE *primitiveTableByKeyIdentity = NULL;
static PRIMITIVES_TABLE *primitiveTableByUsed = NULL;
static unsigned int primitiveTableSize = 0;
static unsigned int primitiveTableFilled = 0;
/*--------------------------------------------------------------------------*/
extern int C2F(cvname)(int *,char *,int *, unsigned long int);
/*--------------------------------------------------------------------------*/
static BOOL realloc_table_scilab_functions(void);
static BOOL doFindFunction(int *key,char *name, int *scilab_funptr);
static BOOL doBackSearchFunction(int *key, int *scilab_funptr);
static BOOL doEnterFunction(int *key,char *name, int *scilab_funptr);
static BOOL doDeleteFunction(int *key, int *scilab_funptr);
static void deleteEntryInTable(PRIMITIVES_TABLE *table, int index,
							   int (*_PtFuncCompare)(const void *, const void *));
/*--------------------------------------------------------------------------*/
static int compareByName(const void *x, const void *y);
static int compareByPrimitivepointer(const void *x, const void *y);
static int compareByKeyIdentity(const void *x, const void *y);
static int compareByUsed(const void *x, const void *y);
/*--------------------------------------------------------------------------*/
BOOL create_hashtable_scilab_functions(void)
{
	if ((primitiveTableByName) &&
		(primitiveTableByPrimitivePointer) &&
		(primitiveTableByKeyIdentity) &&
		(primitiveTableByUsed)) return FALSE;

	primitiveTableSize = DEFAULT_ELEMENTFUNCTIONLIST;
	primitiveTableFilled = 0;

	/* Default allocation */
	primitiveTableByName = (PRIMITIVES_TABLE *) CALLOC((primitiveTableSize + 1), sizeof(PRIMITIVES_TABLE));
	primitiveTableByPrimitivePointer = (PRIMITIVES_TABLE *) CALLOC((primitiveTableSize + 1), sizeof(PRIMITIVES_TABLE));
	primitiveTableByKeyIdentity = (PRIMITIVES_TABLE *) CALLOC((primitiveTableSize + 1), sizeof(PRIMITIVES_TABLE));
	primitiveTableByUsed = (PRIMITIVES_TABLE *) CALLOC((primitiveTableSize + 1), sizeof(PRIMITIVES_TABLE));

	/* initialization to a empty struct */
	if ((primitiveTableByName) &&
		(primitiveTableByPrimitivePointer) &&
		(primitiveTableByKeyIdentity) &&
		(primitiveTableByUsed))
	{
		unsigned int i = 0;

		PRIMITIVES_TABLE emptyPRIMITIVES_TABLE;
		emptyPRIMITIVES_TABLE.primitivePointer = 0;
		emptyPRIMITIVES_TABLE.used = 0;
		memset(emptyPRIMITIVES_TABLE.keyIdentity, 0, sizeof(int) * nsiz);
		strcpy(emptyPRIMITIVES_TABLE.namefunction, "");

		for(i = 0; i < primitiveTableSize; i++)
		{
			memcpy(&primitiveTableByName[i], &emptyPRIMITIVES_TABLE, sizeof(emptyPRIMITIVES_TABLE));
		}
		/* copy struct to others tables */
		memcpy(primitiveTableByPrimitivePointer, primitiveTableByName, sizeof(PRIMITIVES_TABLE)*primitiveTableSize);
		memcpy(primitiveTableByKeyIdentity, primitiveTableByName, sizeof(PRIMITIVES_TABLE)*primitiveTableSize);
		memcpy(primitiveTableByUsed, primitiveTableByName, sizeof(PRIMITIVES_TABLE)*primitiveTableSize);

		return TRUE;
	}
	else
	{
		destroy_hashtable_scilab_functions();
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void destroy_hashtable_scilab_functions()
{
	if (primitiveTableByName) {FREE(primitiveTableByName); primitiveTableByName = NULL;}
	if (primitiveTableByPrimitivePointer) {FREE(primitiveTableByPrimitivePointer); primitiveTableByPrimitivePointer = NULL;}
	if (primitiveTableByKeyIdentity) {FREE(primitiveTableByKeyIdentity); primitiveTableByKeyIdentity = NULL;}
	if (primitiveTableByUsed) {FREE(primitiveTableByUsed); primitiveTableByUsed = NULL;}
	primitiveTableSize = 0;
	primitiveTableFilled = 0;
}
/*--------------------------------------------------------------------------*/
BOOL realloc_table_scilab_functions(void)
{
	if (primitiveTableFilled >= primitiveTableSize)
	{
		unsigned int newPrimitiveTableSize = primitiveTableFilled * 2;

		if (newPrimitiveTableSize > MAXELEMENTFUNCTIONLIST) 
		{
			newPrimitiveTableSize = MAXELEMENTFUNCTIONLIST;
		}

		if ((primitiveTableSize != newPrimitiveTableSize) &&
			(primitiveTableSize != MAXELEMENTFUNCTIONLIST))
		{
			primitiveTableByName = (PRIMITIVES_TABLE *) REALLOC(primitiveTableByName, (newPrimitiveTableSize + 1) * sizeof(PRIMITIVES_TABLE));
			primitiveTableByPrimitivePointer = (PRIMITIVES_TABLE *) REALLOC(primitiveTableByPrimitivePointer, (newPrimitiveTableSize + 1) * sizeof(PRIMITIVES_TABLE));
			primitiveTableByKeyIdentity = (PRIMITIVES_TABLE *) REALLOC(primitiveTableByKeyIdentity, (newPrimitiveTableSize + 1) * sizeof(PRIMITIVES_TABLE));
			primitiveTableByUsed = (PRIMITIVES_TABLE *) REALLOC(primitiveTableByUsed, (newPrimitiveTableSize + 1) * sizeof(PRIMITIVES_TABLE));

			if (primitiveTableByName && 
				primitiveTableByPrimitivePointer && 
				primitiveTableByKeyIdentity && 
				primitiveTableByUsed)
			{
				unsigned int i = 0;

				PRIMITIVES_TABLE emptyPRIMITIVES_TABLE;
				emptyPRIMITIVES_TABLE.primitivePointer = 0;
				emptyPRIMITIVES_TABLE.used = 0;
				memset(emptyPRIMITIVES_TABLE.keyIdentity, 0, sizeof(int) * nsiz);
				strcpy(emptyPRIMITIVES_TABLE.namefunction, "");

				for (i = primitiveTableSize; i < newPrimitiveTableSize; i++)
				{
					memcpy(&primitiveTableByName[i], &emptyPRIMITIVES_TABLE, sizeof(emptyPRIMITIVES_TABLE));
					memcpy(&primitiveTableByPrimitivePointer[i], &emptyPRIMITIVES_TABLE, sizeof(emptyPRIMITIVES_TABLE));
					memcpy(&primitiveTableByKeyIdentity[i], &emptyPRIMITIVES_TABLE, sizeof(emptyPRIMITIVES_TABLE));
					memcpy(&primitiveTableByUsed[i], &emptyPRIMITIVES_TABLE, sizeof(emptyPRIMITIVES_TABLE));
				}

				primitiveTableSize = newPrimitiveTableSize;
				return TRUE;
			}
			return FALSE;
		}
	}
	return TRUE;
}
/*--------------------------------------------------------------------------*/ 
BOOL action_hashtable_scilab_functions(int *key,char *name, int *scilab_funptr,
									   SCI_HFUNCTIONS_ACTION action)
{
	switch(action)
	{
	case SCI_HFUNCTIONS_FIND:
		return doFindFunction(key, name, scilab_funptr);

	case SCI_HFUNCTIONS_BACKSEARCH:
		return doBackSearchFunction(key, scilab_funptr);

	case SCI_HFUNCTIONS_ENTER:
		return doEnterFunction(key, name, scilab_funptr);

	case SCI_HFUNCTIONS_DELETE:
		return doDeleteFunction(key, scilab_funptr);
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/  
char **GetFunctionsList(int *sizeList)
{
	char **ListFunctions = NULL;
	unsigned int i = 0;
	unsigned int j = 0;

	*sizeList = 0;

	/* get numbers of named functions */
	for ( i = 0 ; i < primitiveTableFilled; i++ ) 
	{
		if (primitiveTableByName[i].namefunction) 
		{
			if (strlen(primitiveTableByName[i].namefunction) > 0) j++;
		}
	}
	*sizeList = j;

	ListFunctions = (char**)MALLOC(sizeof(char*)*(*sizeList));
	j = 0;
	if (ListFunctions)
	{
		for ( i = 0 ; i < primitiveTableFilled; i++ ) 
		{
			if (primitiveTableByName[i].namefunction)
			{
				if (strlen(primitiveTableByName[i].namefunction) > 0)
				{
					ListFunctions[j] = strdup(primitiveTableByName[i].namefunction);
					j++;
				}
			}
		}
	}
	return ListFunctions;
}
/*--------------------------------------------------------------------------*/  
BOOL ExistFunction(char *name)
{
	PRIMITIVES_TABLE searched;
	PRIMITIVES_TABLE *found = NULL;
	if (name)
	{
		strcpy(searched.namefunction, name);
		found = bsearch(&searched, primitiveTableByName, 
			primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByName);

		if (found)
		{
			return TRUE;
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/  
static BOOL doFindFunction(int *key,char *name, int *scilab_funptr)
{
	PRIMITIVES_TABLE searched;
	PRIMITIVES_TABLE *found = NULL;

	if (name)
	{
		strcpy(searched.namefunction, name);
		found = bsearch(&searched, primitiveTableByName, 
			primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByName);
	}
	else
	{
		memcpy(searched.keyIdentity, key, sizeof(int) * nsiz);
		found = bsearch(&searched, primitiveTableByKeyIdentity, 
			primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity);
	}

	if (found)
	{
		*scilab_funptr = found->primitivePointer;
		return TRUE;
	}

	return FALSE;
}
/*--------------------------------------------------------------------------*/  
static BOOL doBackSearchFunction(int *key, int *scilab_funptr)
{
	PRIMITIVES_TABLE searched;            
	PRIMITIVES_TABLE *found = NULL;

	searched.primitivePointer = *scilab_funptr;

	found = bsearch(&searched, primitiveTableByPrimitivePointer, 
		primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByPrimitivepointer);

	if (found)
	{
		memcpy(key, found->keyIdentity, sizeof(int)* nsiz);
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/  
static BOOL doEnterFunction(int *key,char *name, int *scilab_funptr)
{
	PRIMITIVES_TABLE newPRIMITIVES_TABLE;

	realloc_table_scilab_functions();

	if (primitiveTableFilled == primitiveTableSize) return FALSE;

	/* add function name by name */
	if (name)
	{
		int zero = 0;
		strcpy(newPRIMITIVES_TABLE.namefunction, name);
		C2F(cvname)(newPRIMITIVES_TABLE.keyIdentity, name, &zero,(unsigned long)strlen(name));
		memcpy(key, newPRIMITIVES_TABLE.keyIdentity, sizeof(int) * nsiz);
	}
	else /* add function by key identity eq. to name */
	{
		strcpy(newPRIMITIVES_TABLE.namefunction, "");
		memcpy(newPRIMITIVES_TABLE.keyIdentity, key, sizeof(int) * nsiz);
	}
	newPRIMITIVES_TABLE.primitivePointer = *scilab_funptr;
	newPRIMITIVES_TABLE.used = 1;

	/* copy in others tables */
	memcpy(&primitiveTableByName[primitiveTableFilled], &newPRIMITIVES_TABLE, sizeof(PRIMITIVES_TABLE));
	memcpy(&primitiveTableByPrimitivePointer[primitiveTableFilled], &newPRIMITIVES_TABLE, sizeof(PRIMITIVES_TABLE));
	memcpy(&primitiveTableByKeyIdentity[primitiveTableFilled], &newPRIMITIVES_TABLE, sizeof(PRIMITIVES_TABLE));
	memcpy(&primitiveTableByUsed[primitiveTableFilled], &newPRIMITIVES_TABLE, sizeof(PRIMITIVES_TABLE));
	primitiveTableFilled++;

	/* sort tables with new entry */
	qsort(primitiveTableByName, primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByName);
	qsort(primitiveTableByPrimitivePointer, primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByPrimitivepointer);
	qsort(primitiveTableByKeyIdentity, primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity);
	qsort(primitiveTableByUsed, primitiveTableFilled, sizeof(PRIMITIVES_TABLE), compareByUsed);

	return TRUE;
}
/*--------------------------------------------------------------------------*/  
static BOOL doDeleteFunction(int *key, int *scilab_funptr)
{
	unsigned int idxMax = 0;

	int idxName = -1;
	int idxPrimitivePointer = -1;
	int idxKeyIdendity = -1;
	int idxUsed = -1;

	PRIMITIVES_TABLE *found = NULL;
	PRIMITIVES_TABLE searched;
	searched.primitivePointer = *scilab_funptr;
	searched.used = 1;

	// delete by KeyIdentity
	memcpy(searched.keyIdentity, key,sizeof(int) * nsiz);

	/* search in tables and get index position in each table */
	idxMax = primitiveTableFilled;
	found = lfind(&searched, primitiveTableByPrimitivePointer, 
		&idxMax, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity);
	if (found)
	{
		if (idxPrimitivePointer == -1) idxPrimitivePointer = abs(found - primitiveTableByPrimitivePointer);
		found = NULL;
	}

	idxMax = primitiveTableFilled;
	found = lfind(&searched, primitiveTableByKeyIdentity, 
		&idxMax, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity);
	if (found)
	{
		if (idxKeyIdendity == -1) idxKeyIdendity = abs(found - primitiveTableByKeyIdentity);
		found = NULL;
	}

	idxMax = primitiveTableFilled;
	found = lfind(&searched, primitiveTableByName, 
		&idxMax, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity);
	if (found)
	{
		if (idxName == -1) idxName = abs(found - primitiveTableByName);
		found = NULL;
	}

	idxMax = primitiveTableFilled;
	found = lfind(&searched, primitiveTableByUsed, 
		&idxMax, sizeof(PRIMITIVES_TABLE), compareByKeyIdentity);
	if (found)
	{
		if (idxUsed == -1) idxUsed = abs(found - primitiveTableByUsed);
		found = NULL;
	}

	if ((idxName >= 0) && (idxPrimitivePointer >= 0) && (idxKeyIdendity >= 0) && (idxUsed >= 0))
	{
		/* delete entry in each table */
		deleteEntryInTable(primitiveTableByName, idxName, compareByName);
		deleteEntryInTable(primitiveTableByPrimitivePointer, idxPrimitivePointer, compareByPrimitivepointer);
		deleteEntryInTable(primitiveTableByKeyIdentity, idxKeyIdendity, compareByKeyIdentity);
		deleteEntryInTable(primitiveTableByUsed, idxUsed, compareByUsed);

		primitiveTableFilled--;

		return TRUE;
	}

	return FALSE;
}
/*--------------------------------------------------------------------------*/  
static int compareByName(const void *x, const void *y)
{
	return strcmp(((PRIMITIVES_TABLE*)x)->namefunction, 
		((PRIMITIVES_TABLE*)y)->namefunction);
}
/*--------------------------------------------------------------------------*/  
static int compareByPrimitivepointer(const void *x, const void *y)
{
	return (((PRIMITIVES_TABLE*)x)->primitivePointer - 
		((PRIMITIVES_TABLE*)y)->primitivePointer);
}
/*--------------------------------------------------------------------------*/  
static int compareByKeyIdentity(const void *x, const void *y)
{
	return memcmp(((PRIMITIVES_TABLE*)x)->keyIdentity, 
		((PRIMITIVES_TABLE*)y)->keyIdentity, sizeof(int)*nsiz);
}
/*--------------------------------------------------------------------------*/  
static int compareByUsed(const void *x, const void *y)
{
	return ((PRIMITIVES_TABLE*)x)->used - ((PRIMITIVES_TABLE*)y)->used;
}
/*--------------------------------------------------------------------------*/  
static void deleteEntryInTable(PRIMITIVES_TABLE *table, int index,
							   int (*_PtFuncCompare)(const void *, const void *))
{
	memset(table[index].keyIdentity, 0, sizeof(int)*nsiz);
	strcpy(table[index].namefunction, "");
	table[index].primitivePointer = 0;
	table[index].used = 0;
	qsort(table, primitiveTableFilled, sizeof(PRIMITIVES_TABLE), *_PtFuncCompare);
}
/*--------------------------------------------------------------------------*/  

