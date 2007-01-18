/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* Allan CORNET */
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "hashtable_core.h"
/*-----------------------------------------------------------------------------------*/
static _ENTRY   * htable = NULL;
static unsigned   hsize;
static unsigned   filled;
/*-----------------------------------------------------------------------------------*/
static int Equal_id(int *x, int *y);
static int isprime(unsigned int number);
/*-----------------------------------------------------------------------------------*/
int create_hashtable_scilab_functions(unsigned int nel)
{
	int bOK=0;

	if (htable == NULL)
	{
		nel |= 1;      /* make odd */
		while (!isprime(nel)) nel += 2;

		hsize  = nel;
		filled = 0;

		if ((htable = (_ENTRY *)MALLOC((hsize+1)*sizeof(_ENTRY))) == NULL)	
		{
			bOK=0;
		}
		else
		{
			bOK=1;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
void destroy_hashtable_scilab_functions()
{
	if (htable)
	{
		FREE(htable);
		htable = NULL;
	}
}
/*-----------------------------------------------------------------------------------*/
int action_hashtable_scilab_functions(int *key,char *name, int *data, SCI_HFUNCTIONS_ACTION action)
{
	int bOK=FAILED;

	if (action == SCI_HFUNCTIONS_BACKSEARCH)
	{
		unsigned int i=0;
		for ( i = 0 ; i < hsize ; i++ ) if ( htable[i].used && htable[i].entry.data == *data ) 
		{
			int j=0;
			for (j = 0; j < nsiz ; j++ ) key[j] = htable[i].entry.key[j];
			return (bOK=OK);
		}
	}
	else
	{
		register unsigned len = nsiz;
		register unsigned hval=0;
		register unsigned hval2=0;
		register unsigned count=0;

		register unsigned idx=0;

		if (action == SCI_HFUNCTIONS_ENTER && filled == hsize) return FAILED;

		hval  = len;
		count = len;
		while (count-- > 0) 
		{
			hval += key[count];
			hval %= hsize;
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
					}
				}
			}

			hval2 = 1 + hval % (hsize-2);

			do 
			{
				if (idx <= hval2) idx = hsize+idx-hval2;
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

	return bOK;
}
/*-----------------------------------------------------------------------------------*/  
static int Equal_id(int *x, int *y)
{
	int i;
	for (i = 0; i < nsiz ; i++ ) if ( x[i] != y[i] ) return(1);
	return(0);
}
/*-----------------------------------------------------------------------------------*/  
static int isprime(unsigned int number)
{
	unsigned div = 3;

	while (div*div < number && number%div != 0) div += 2;

	return number%div != 0;
}
/*-----------------------------------------------------------------------------------*/  
char **GetFunctionsList(int *sizeList)
{
	char **ListFunctions=NULL;
	unsigned int i=0;
	int j=0;

	*sizeList=0;

	for ( i = 0 ; i < hsize ; i++ ) if ( htable[i].used) 
	{
		if (htable[i].entry.namefunction) j++;
	}
	*sizeList=j;

	ListFunctions=(char**)MALLOC(sizeof(char*)*(*sizeList));

	j=0;

	for ( i = 0 ; i < hsize ; i++ ) if ( htable[i].used) 
	{
		if (htable[i].entry.namefunction)
		{
			ListFunctions[j]=(char*)MALLOC(sizeof(char)*(strlen(htable[i].entry.namefunction)+1));
			strcpy(ListFunctions[j],htable[i].entry.namefunction);
			j++;
		}
	}
	return ListFunctions;
}
/*-----------------------------------------------------------------------------------*/  
BOOL ExistFunction(char *name)
{
	BOOL bOK=FALSE;
	int i=0;

	for ( i = 0 ; i < (int)hsize ; i++ ) if ( htable[i].used) 
	{
		if (strcmp(htable[i].entry.namefunction,name) == 0)
		{
			return TRUE;
		}
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/  
