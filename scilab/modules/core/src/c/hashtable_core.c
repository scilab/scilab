/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "stack-def.h"
#include "hashtable_core.h"
#include "getfunctionslist.h"
#include "existfunction.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#define MAXLENGHTFUNCTIONNAME 256 /* 24 in fact in scilab */
typedef struct entry 
{
	int key[nsiz];
	int data;
	char namefunction[MAXLENGHTFUNCTIONNAME];
} ENTRY;

typedef struct 
{ 
	unsigned int   used;
	ENTRY entry;
} _ENTRY;
/*--------------------------------------------------------------------------*/
static _ENTRY   *htable = NULL;
static unsigned hashtableSize = 0;
static unsigned filled = 0;
/*--------------------------------------------------------------------------*/
extern int C2F(cvname)(int *,char *,int *, unsigned long int);
/*--------------------------------------------------------------------------*/
static BOOL Equal_id(int *x, int *y);
static BOOL realloc_hashtable_scilab_functions(void);
/*--------------------------------------------------------------------------*/
BOOL create_hashtable_scilab_functions(void)
{
	if (htable == NULL)
	{
		hashtableSize  = DEFAULT_ELEMENTFUNCTIONLIST;
		filled = 0;

		htable = (_ENTRY *) CALLOC((hashtableSize + 1), sizeof(_ENTRY));

		if (htable)	
		{
			unsigned int i = 0;
			htable[0].used = 0;
			strcpy(htable[0].entry.namefunction, "");
			for (i = 0; i < nsiz; i++)
			{
				htable[0].entry.key[i] = 0;
			}
			htable[0].entry.data = 0;

			for(i = 1; i < hashtableSize; i++)
			{
				memcpy(&htable[i], &htable[0], sizeof(htable[0]));
			}
			return TRUE;
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
void destroy_hashtable_scilab_functions()
{
	if (htable)
	{
		FREE(htable);
		htable = NULL;
	}
}
/*--------------------------------------------------------------------------*/
BOOL action_hashtable_scilab_functions(int *key,char *name, int *scilab_funptr, SCI_HFUNCTIONS_ACTION action)
{
	switch(action)
	{
	case SCI_HFUNCTIONS_FIND:
		{
			/* linear search algorithm */
			unsigned int idx = 0;
			int keyToSearch[nsiz];

			if (name)
			{
				/* faster than a strcmp */
				int job = 0; /* convert name to id */
				C2F(cvname)(keyToSearch, name, &job, (unsigned long)strlen(name));
			}
			else
			{
				unsigned int i = 0;
				for(i = 0; i < nsiz; i++)
				{
					keyToSearch[i] = key[i];
				}
			}

			/* linear search algorithm */
			for ( idx = 0 ; idx < filled + 1; idx++ ) 
			{
				if ( htable[idx].used)
				{
					if (Equal_id(keyToSearch, htable[idx].entry.key) == TRUE)
					{
						*scilab_funptr = htable[idx].entry.data;
						return TRUE;
					}
				}
			}
			return FALSE;
		}
		break;
	case SCI_HFUNCTIONS_BACKSEARCH:
		{
			/* linear search algorithm */
			unsigned int idx = 0;
			for ( idx = 0 ; idx < filled + 1; idx++ ) 
			{
				if ( (htable[idx].used) && (htable[idx].entry.data == *scilab_funptr) ) 
				{
					int j = 0;
					for (j = 0; j < nsiz ; j++ ) key[j] = htable[idx].entry.key[j];
					return TRUE;
				}
			}
			return FALSE;
		}
		break;
	case SCI_HFUNCTIONS_ENTER:
		{
			unsigned int idx = 0;

			realloc_hashtable_scilab_functions();

			if (filled ==  hashtableSize) return FALSE;

			for (idx = 0; idx < hashtableSize; idx++)
			{
				if (htable[idx].used == 0)
				{
					int zero = 0;

					htable[idx].entry.data = *scilab_funptr;

					if (name)
					{
						strcpy(htable[idx].entry.namefunction, name);
						C2F(cvname)(htable[idx].entry.key, name, &zero,(unsigned long)strlen(name));
					}
					else
					{
						unsigned int i = 0;
						strcpy(htable[idx].entry.namefunction, "");
						for(i = 0; i < nsiz; i++)
						{
							htable[idx].entry.key[i] = key[i];
						}
					}
					htable[idx].used = 1;
					filled++;
					return TRUE;
				}
			}
			return FALSE;
		}
		break;
	case SCI_HFUNCTIONS_DELETE:
		{
			unsigned int idx = 0;
			for (idx = 0; idx < filled + 1; idx++)
			{
				if ( (htable[idx].used) &&
					 (htable[idx].entry.data == *scilab_funptr) &&
					 (Equal_id(key, htable[idx].entry.key) == TRUE) )
				{
					int i = 0;
					htable[idx].used = 0;
					htable[idx].entry.data = 0;
					strcpy(htable[idx].entry.namefunction, "");
					for (i = 0; i < nsiz; i++)
					{
						htable[idx].entry.key[i] = 0;
					}
					filled--;
					return TRUE;
				}
			}
			return FALSE;
		}
		break;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/  
static BOOL Equal_id(int *x, int *y)
{
	int i = 0;
	for (i = 0; i < nsiz ; i++ )
	{
		if ( x[i] != y[i] ) return FALSE;
	}
	return TRUE;
}
/*--------------------------------------------------------------------------*/  
char **GetFunctionsList(int *sizeList)
{
	char **ListFunctions = NULL;
	unsigned int i = 0;
	int j = 0;

	*sizeList = 0;

	for ( i = 0 ; i < filled + 1; i++ ) if ( htable[i].used) 
	{
		if (htable[i].entry.namefunction) 
		{
			if (strlen(htable[i].entry.namefunction) > 0) j++;
		}
	}
	*sizeList = j;

	ListFunctions = (char**)MALLOC(sizeof(char*)*(*sizeList));
	j = 0;
	if (ListFunctions)
	{
		for ( i = 0 ; i < filled + 1; i++ ) if ( htable[i].used) 
		{
			if (htable[i].entry.namefunction)
			{
				if (strlen(htable[i].entry.namefunction) > 0)
				{
					ListFunctions[j] = strdup(htable[i].entry.namefunction);
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
	int i = 0;

	for ( i = 0 ; i < (int)filled + 1; i++ ) 
	{
		if (htable[i].used) 
		{
			if (strcmp(htable[i].entry.namefunction, name) == 0)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/  
BOOL realloc_hashtable_scilab_functions(void)
{
	if ( (filled) >= hashtableSize)
	{
		unsigned int newhashtableSize = filled * 2;

		if (newhashtableSize > MAXELEMENTFUNCTIONLIST) newhashtableSize = MAXELEMENTFUNCTIONLIST;

		if ( (hashtableSize != newhashtableSize) && (hashtableSize != MAXELEMENTFUNCTIONLIST) )
		{

			htable = (_ENTRY *) REALLOC(htable, (newhashtableSize + 1) * sizeof(_ENTRY));
			if (htable)	
			{
				_ENTRY emptyEntry;
				unsigned int i = 0;

				emptyEntry.used = 0;
				strcpy(emptyEntry.entry.namefunction, "");
				for (i = 0; i < nsiz; i++)
				{
					emptyEntry.entry.key[i] = 0;
				}
				emptyEntry.entry.data = 0;

				for (i = hashtableSize; i < newhashtableSize; i++)
				{
					memcpy(&htable[i], &emptyEntry, sizeof(emptyEntry));
				}
				hashtableSize = newhashtableSize;
				return TRUE;
			}
			return FALSE;
		}
	}
	return TRUE;
}
/*--------------------------------------------------------------------------*/  
