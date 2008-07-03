/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
#include "hashtable_core.h"
#include "getfunctionslist.h"
#include "existfunction.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
static _ENTRY   * htable = NULL;
static unsigned   hashtableSize;
static unsigned   filled;
/*--------------------------------------------------------------------------*/
static int Equal_id(int *x, int *y);

/**
 * Check if a number is prime (or not)
 * @param number the number
 */
static int isprime(unsigned int number);
/*--------------------------------------------------------------------------*/
int create_hashtable_scilab_functions(unsigned int nel)
{
	if (htable == NULL)
	{
		nel |= 1;      /* make odd */
		while (!isprime(nel)) nel += 2;

		hashtableSize  = nel;
		filled = 0;

		if ((htable = (_ENTRY *)MALLOC((hashtableSize+1)*sizeof(_ENTRY))) == NULL)	
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	return 0;
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
int action_hashtable_scilab_functions(int *key,char *name, int *data, SCI_HFUNCTIONS_ACTION action)
{

	if (action == SCI_HFUNCTIONS_BACKSEARCH)
	{
		unsigned int i=0;
		for ( i = 0 ; i < hashtableSize ; i++ ) if ( htable[i].used && htable[i].entry.data == *data ) 
		{
			int j=0;
			for (j = 0; j < nsiz ; j++ ) key[j] = htable[i].entry.key[j];
			return OK;
		}
	}
	else
	{
		register unsigned len = nsiz;
		register unsigned hval=0;
		register unsigned hval2=0;
		register unsigned count=0;

		register unsigned idx=0;

		if (action == SCI_HFUNCTIONS_ENTER && filled == hashtableSize) return FAILED;

		hval  = len;
		count = len;

		/* @TODO add comment : Why are doing this ? */
		while (count-- > 0) 
		{
			hval += key[count];
			hval %= hashtableSize;
		}

		if (hval == 0) hval++;

		idx = hval;

		if (htable[idx].used) 
		{
			if (htable[idx].used == hval )
			{
				if ( Equal_id(key, htable[idx].entry.key) == 0) 
				{
					switch (action) 
					{
					case SCI_HFUNCTIONS_ENTER :
						htable[idx].entry.data = *data;
						if (name) strcpy(htable[idx].entry.namefunction,name);
						return OK;
						break;
					case SCI_HFUNCTIONS_DELETE :
						htable[idx].used = 0;
						filled--;
						return OK ;
						break;

					case SCI_HFUNCTIONS_FIND :
						*data = htable[idx].entry.data;
						return OK;
						break;

					case SCI_HFUNCTIONS_BACKSEARCH :
						return FAILED;
						break;
					}
				}
			}

			hval2 = 1 + hval % (hashtableSize-2);

			do 
			{
				if (idx <= hval2) idx = hashtableSize+idx-hval2;
				else idx -= hval2;

				if (htable[idx].used == hval ) 
				{
					if ( Equal_id(key, htable[idx].entry.key) == 0) 
					{
						switch (action) 
						{
						case SCI_HFUNCTIONS_ENTER :
							htable[idx].entry.data = *data; 
							if (name) strcpy(htable[idx].entry.namefunction,name);
							return OK;
						case SCI_HFUNCTIONS_DELETE :
							htable[idx].used = 0;
							filled--;
							return OK;
							break;
						case SCI_HFUNCTIONS_FIND :
							*data = htable[idx].entry.data; 
							return OK;
						case SCI_HFUNCTIONS_BACKSEARCH :
							return FAILED;
							break;
						}
					}
				}
			} while (htable[idx].used);
		}

		if (action == SCI_HFUNCTIONS_ENTER) 
		{
			int i=0;
			htable[idx].used  = hval;
			for ( i=0 ; i < nsiz ; i++ ) htable[idx].entry.key[i] = key[i];
			htable[idx].entry.data = *data;
			if (name) strcpy(htable[idx].entry.namefunction,name);
			filled++;
			return OK ;
		}
		else return FAILED;
	}

	return FAILED;
}
/*--------------------------------------------------------------------------*/  
static int Equal_id(int *x, int *y)
{
	int i;
	for (i = 0; i < nsiz ; i++ ) if ( x[i] != y[i] ) return(1);
	return(0);
}
/*--------------------------------------------------------------------------*/  
static int isprime(unsigned int number)
{
	unsigned div_ = 3;

	while (div_*div_ < number && number%div_ != 0) div_ += 2;

	return number%div_ != 0;
}
/*--------------------------------------------------------------------------*/  
char **GetFunctionsList(int *sizeList)
{
	char **ListFunctions=NULL;
	unsigned int i=0;
	int j=0;

	*sizeList=0;

	for ( i = 0 ; i < hashtableSize ; i++ ) if ( htable[i].used) 
	{
		if (htable[i].entry.namefunction) j++;
	}
	*sizeList=j;

	ListFunctions=(char**)MALLOC(sizeof(char*)*(*sizeList));

	j=0;

	for ( i = 0 ; i < hashtableSize ; i++ ) if ( htable[i].used) 
	{
		if (htable[i].entry.namefunction)
		{
			ListFunctions[j] = strdup(htable[i].entry.namefunction);
			j++;
		}
	}
	return ListFunctions;
}
/*--------------------------------------------------------------------------*/  
BOOL ExistFunction(char *name)
{
	int i=0;

	for ( i = 0 ; i < (int)hashtableSize ; i++ ) {
		if ( htable[i].used) 
			{
				if (strcmp(htable[i].entry.namefunction,name) == 0)
					{
						return TRUE;
					}
			}
	}

	return FALSE;
}
/*--------------------------------------------------------------------------*/  
