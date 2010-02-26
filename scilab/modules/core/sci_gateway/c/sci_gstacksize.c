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
#include "gw_core.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "getmaxMALLOC.h"
#include "scimem.h"
#include "localization.h"
#include "stackinfo.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(adjustgstacksize)();
/*--------------------------------------------------------------------------*/
#define MIN_GSTACKSIZE 11000
#define PARAM_MAX_STR "max"
#define PARAM_MIN_STR "min"
/*--------------------------------------------------------------------------*/
/* gstacksize - set scilab global stack size

* Calling Sequence

* gstacksize(n)
* gstacksize('max')
* gstacksize('min')
* sz=gstacksize()

* Parameters

* n : integer, the required stack size given in number of double precision words
* 'max' : try to allocate the maximum of memory
* 'min' : allocate the minimum of memory
* sz : 2-vector [total used]
*/
/*--------------------------------------------------------------------------*/
static int sci_gstacksizeNoRhs(char *fname);
static int sci_gstacksizeOneRhs(char *fname);
static int sci_gstacksizeMax(char *fname);
static int sci_gstacksizeMin(char *fname);
static int setGStacksizeMin(char *fname);
static int setGStacksizeMax(char *fname);
static int setGStacksize(unsigned long newsize);
static unsigned long getCurrentGStacksize(void);
static unsigned long getUsedGStacksize(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_gstacksize)(char *fname,unsigned long fname_len)
{
	Rhs = Max(0,Rhs);
	CheckRhs(0,1);
	CheckLhs(0,1);

	if (Rhs == 0)
	{
		return sci_gstacksizeNoRhs(fname);
	}
	else
	{
		return sci_gstacksizeOneRhs(fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_gstacksizeNoRhs(char *fname)
{
	int l1 = 0, n1 = 0, m1 = 0;
	int *paramoutINT = NULL;
	int total = 0;
	int used = 0;
	paramoutINT = (int*)MALLOC(sizeof(int)*2);

	C2F(getgstackinfo)(&total, &used);
	if (total == (MIN_GSTACKSIZE-1)) total = MIN_GSTACKSIZE;
	paramoutINT[0] = total;
	paramoutINT[1] = used;

	n1 = 1;
	m1 = 2;
	CreateVarFromPtr(Rhs + 1, MATRIX_OF_INTEGER_DATATYPE, &n1, &m1, &paramoutINT);

	LhsVar(1) = Rhs + 1;
	C2F(putlhsvar)();

	if (paramoutINT) {FREE(paramoutINT); paramoutINT = NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_gstacksizeOneRhs(char *fname)
{
	int l1 = 0, n1 = 0, m1 = 0;

	if ( GetType(1) == sci_matrix )
	{
		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1 ,&l1);
		if ( (m1 == 1) && (n1 == 1) )
		{
			unsigned long NEWMEMSTACKSIZE = (unsigned long) * stk(l1);

			/* add 1 for alignment problems */
			if ( is_a_valid_size_for_scilab_stack(NEWMEMSTACKSIZE + 1) )
			{
				if ( (NEWMEMSTACKSIZE >= MIN_GSTACKSIZE) && (NEWMEMSTACKSIZE <= get_max_memory_for_scilab_stack()) )
				{
					/* we backup previous size */
					unsigned long backupSize = getCurrentGStacksize();

					if (setGStacksizeMin(fname))
					{
						if (setGStacksize(NEWMEMSTACKSIZE))
						{
							LhsVar(1) = 0;
							C2F(putlhsvar)();
							return 0;
						}
						else
						{
							/* restore previous size */
							setGStacksize(backupSize);
							Scierror(999,_("%s: Cannot allocate memory.\n"), fname);
						}
					}
					else
					{
						/* restore previous size */
						setGStacksize(backupSize);
						Scierror(999,_("%s: Cannot allocate memory.\n"), fname);
					}

				}
				else
				{
					Scierror(1504, _("%s: Out of bounds value. Not in [%lu,%lu].\n"), fname, MIN_GSTACKSIZE, get_max_memory_for_scilab_stack() - 1);
				}
			}
			else
			{
				Scierror(1504, _("%s: Out of bounds value. Not in [%lu,%lu].\n"), fname, MIN_GSTACKSIZE, get_max_memory_for_scilab_stack() - 1);
			}
		}
		else
		{
			Scierror(204, _("%s: Wrong size for input argument #%d: Scalar expected.\n"), fname, 1);
		}
	}
	else if ( GetType(1) == sci_strings )
	{
		char *param = NULL;

		GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
		param = cstk(l1);
		if (strcmp(PARAM_MAX_STR, param) == 0)
		{
			return sci_gstacksizeMax(fname);
		}
		else if (strcmp(PARAM_MIN_STR, param) == 0)
		{
			return sci_gstacksizeMin(fname);
		}
		else
		{
			Scierror(204,_("%s: Wrong type for input argument #%d: Scalar, '%s' or '%s'.\n"),fname,1, "min","max");
		}
	}
	else
	{
		Scierror(204,_("%s: Wrong type for input argument #%d: Scalar, '%s' or '%s'.\n"),fname,1, "min","max");
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_gstacksizeMax(char *fname)
{
	if (setGStacksizeMax(fname))
	{
		LhsVar(1) = 0;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Cannot allocate memory.\n"), fname);
	}
	return 0;

}
/*--------------------------------------------------------------------------*/
static int sci_gstacksizeMin(char *fname)
{
	if (setGStacksizeMin(fname))
	{
		LhsVar(1) = 0;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Cannot allocate memory.\n"), fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int setGStacksizeMin(char *fname)
{
	unsigned long memstackused = getUsedGStacksize();
	unsigned long newminstack = getUsedGStacksize();

	if (memstackused < MIN_GSTACKSIZE)
	{
		newminstack = MIN_GSTACKSIZE;
	}
	else
	{
		/* Add 3000 security for the stack */
		newminstack = memstackused + 3000;
	}

	return setGStacksize(newminstack);
}
/*--------------------------------------------------------------------------*/
static int setGStacksizeMax(char *fname)
{
	/* we backup previous size */
	unsigned long backupSize = getCurrentGStacksize();
	/* Bug 5495 on Windows 2000 -- WONT FIX GetLargestFreeMemoryRegion */
	/* it works on XP, Vista, S7ven */
	/* GetLargestFreeMemoryRegion() returns a superior size to real value */
	unsigned long maxmemfree = (GetLargestFreeMemoryRegion()) / sizeof(double);

	/* We have already max */
	if (maxmemfree <= backupSize)
	{
		LhsVar(1) = 0;
		C2F(putlhsvar)();
		return 1;
	}

	/* we do a stacksize('min') */
	if (setGStacksizeMin(fname))
	{
		unsigned long memmaxavailablebyscilab = get_max_memory_for_scilab_stack();
		unsigned long newMemSizeMax = maxmemfree;

		if (memmaxavailablebyscilab < newMemSizeMax)
		{
			newMemSizeMax = memmaxavailablebyscilab;
		}

		if (newMemSizeMax < MIN_GSTACKSIZE)
		{
			newMemSizeMax = MIN_GSTACKSIZE;
		}

		setGStacksize(newMemSizeMax);
		return 1;
	}
	else
	{
		/* stacksize('min') fails */
		/* restore previous size */
		setGStacksize(backupSize);
		Scierror(999,_("%s: Cannot allocate memory.\n"), fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int setGStacksize(unsigned long newsize)
{
	if (newsize != getCurrentGStacksize())
	{
		if ( (newsize >= MIN_GSTACKSIZE) && (newsize <= get_max_memory_for_scilab_stack()) )
		{
			unsigned long ptr = 0;
			int l = 0;

			C2F(scigmem)(&newsize, &ptr);
			l = C2F(vstk).lstk[C2F(vstk).gtop] - C2F(vstk).lstk[C2F(vstk).isiz + 1];

			if (ptr) 
			{
				LhsVar(1) = 0;
				C2F(putlhsvar)();

				C2F(adjustgstacksize)(&newsize, &ptr, &l);
				return 1;
			}
		}
		return 0;
	}
	return 1;
}
/*--------------------------------------------------------------------------*/
static unsigned long getCurrentGStacksize(void)
{
	unsigned long memstacktotal = 0;
	unsigned long memstackused = 0;

	C2F(getgstackinfo)(&memstacktotal, &memstackused);

	return memstacktotal;
}
/*--------------------------------------------------------------------------*/
static unsigned long getUsedGStacksize(void)
{
	unsigned long memstacktotal = 0;
	unsigned long memstackused = 0;

	C2F(getgstackinfo)(&memstacktotal, &memstackused);

	return memstackused;
}
/*--------------------------------------------------------------------------*/

