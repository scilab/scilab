/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009-2010 - DIGITEO - Allan CORNET
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
#include "dynamic_parallel.h"
/*--------------------------------------------------------------------------*/
extern int C2F(adjuststacksize)();
/*--------------------------------------------------------------------------*/
#define MIN_STACKSIZE 180000
#define PARAM_MAX_STR "max"
#define PARAM_MIN_STR "min"
/*--------------------------------------------------------------------------*/
static int sci_stacksizeNoRhs(char *fname);
static int sci_stacksizeOneRhs(char *fname);
static int sci_stacksizeMax(char *fname);
static int sci_stacksizeMin(char *fname);
static int setStacksizeMin(char *fname);
static int setStacksizeMax(char *fname);
static int setStacksize(unsigned long newsize);
static unsigned long getCurrentStacksize(void);
static unsigned long getUsedStacksize(void);
/*--------------------------------------------------------------------------*/
/**
* stacksize - set scilab stack size
* Calling Sequence
* stacksize(n)
* stacksize('max')
* stacksize('min')
* sz=stacksize()
* Parameters
* n : integer, the required stack size given in number of double precision words
* 'max' : try to allocate the maximum of memory
* 'max' : allocate the minimum of memory
* sz : 2-vector [total used]
*/
/*--------------------------------------------------------------------------*/
int C2F(sci_stacksize)(char *fname,unsigned long fname_len)
{

    Rhs = Max(0,Rhs);
    CheckRhs(0,1);
    CheckLhs(0,1);

    if (Rhs == 0)
    {
        return sci_stacksizeNoRhs(fname);
    }
    else
    {
        return dynParallelConcurrency() ? dynParallelForbidden(fname) : sci_stacksizeOneRhs(fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_stacksizeNoRhs(char *fname)
{
    int l1 = 0, n1 = 0, m1 = 0;
    int *paramoutINT = NULL;
    int total = 0;
    int used = 0;
    paramoutINT = (int *)MALLOC(sizeof(int)*2);

    C2F(getstackinfo)(&total, &used);
    paramoutINT[0] = total;
    paramoutINT[1] = used;

    n1 = 1;m1 = 2;
    CreateVarFromPtr(Rhs + 1, MATRIX_OF_INTEGER_DATATYPE, &n1, &m1, (int *)&paramoutINT);

    LhsVar(1) = Rhs + 1;
    C2F(putlhsvar)();

    if (paramoutINT) {FREE(paramoutINT); paramoutINT = NULL;}
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_stacksizeOneRhs(char *fname)
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
                if ( (NEWMEMSTACKSIZE >= MIN_STACKSIZE) && (NEWMEMSTACKSIZE <= get_max_memory_for_scilab_stack()) )
                {
                    /* we backup previous size */
                    unsigned long backupSize = getCurrentStacksize();

                    if (setStacksizeMin(fname))
                    {
                        if (setStacksize(NEWMEMSTACKSIZE))
                        {
                            LhsVar(1) = 0;
                            C2F(putlhsvar)();
                            return 0;
                        }
                        else
                        {
                            /* restore previous size */
                            setStacksize(backupSize);
                            Scierror(999,_("%s: Cannot allocate memory.\n"),fname);
                        }
                    }
                    else
                    {
                        /* restore previous size */
                        setStacksize(backupSize);
                        Scierror(999,_("%s: Cannot allocate memory.\n"),fname);
                    }

                }
                else
                {
                    Scierror(1504, _("%s: Out of bounds value. Not in [%lu,%lu].\n"), fname, MIN_STACKSIZE, get_max_memory_for_scilab_stack() - 1);
                }
            }
            else
            {
                Scierror(1504, _("%s: Out of bounds value. Not in [%lu,%lu].\n"), fname, MIN_STACKSIZE, get_max_memory_for_scilab_stack() - 1);
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
            return sci_stacksizeMax(fname);
        }
        else if (strcmp(PARAM_MIN_STR, param) == 0)
        {
            return sci_stacksizeMin(fname);
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
static int sci_stacksizeMax(char *fname)
{
    if (setStacksizeMax(fname))
    {
        LhsVar(1) = 0;
        C2F(putlhsvar)();
    }
    else
    {
        Scierror(999,_("%s: Cannot allocate memory.\n"),fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_stacksizeMin(char *fname)
{
    if (setStacksizeMin(fname))
    {
        LhsVar(1) = 0;
        C2F(putlhsvar)();
    }
    else
    {
        Scierror(999,_("%s: Cannot allocate memory.\n"),fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int setStacksizeMin(char *fname)
{
    unsigned long memstackused = getUsedStacksize();
    unsigned long newminstack = getUsedStacksize();

    if (memstackused < MIN_STACKSIZE)
    {
        newminstack = MIN_STACKSIZE;
    }
    else
    {
        /* Add 3000 security for the stack */
        newminstack = memstackused + 3000;
    }

    return setStacksize(newminstack);
}
/*--------------------------------------------------------------------------*/
static int setStacksizeMax(char *fname)
{
    /* we backup previous size */
    unsigned long backupSize = getCurrentStacksize();
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
    if (setStacksizeMin(fname))
    {
        unsigned long memmaxavailablebyscilab = get_max_memory_for_scilab_stack();
        unsigned long newMemSizeMax = maxmemfree;

        if (memmaxavailablebyscilab < newMemSizeMax)
        {
            newMemSizeMax = memmaxavailablebyscilab;
        }

        if (newMemSizeMax < MIN_STACKSIZE)
        {
            newMemSizeMax = MIN_STACKSIZE;
        }

        setStacksize(newMemSizeMax);
        return 1;
    }
    else
    {
        /* stacksize('min') fails */
        /* restore previous size */
        setStacksize(backupSize);
        Scierror(999,_("%s: Cannot allocate memory.\n"), fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
static int setStacksize(unsigned long newsize)
{
    if (newsize != getCurrentStacksize())
    {
        if ( (newsize >= MIN_STACKSIZE) && (newsize <= get_max_memory_for_scilab_stack()) )
        {
            unsigned long ptr = 0;

            C2F(scimem)(&newsize, &ptr);
            if (ptr)
            {
                LhsVar(1) = 0;
                C2F(putlhsvar)();

                C2F(adjuststacksize)(&newsize, &ptr);
                return 1;
            }
        }
        return 0;
    }
    return 1;
}
/*--------------------------------------------------------------------------*/
static unsigned long getCurrentStacksize(void)
{
    unsigned long memstacktotal = 0;
    unsigned long memstackused = 0;

    C2F(getstackinfo)(&memstacktotal, &memstackused);

    return memstacktotal;
}
/*--------------------------------------------------------------------------*/
static unsigned long getUsedStacksize(void)
{
    unsigned long memstacktotal = 0;
    unsigned long memstackused = 0;

    C2F(getstackinfo)(&memstacktotal, &memstackused);

    return memstackused;
}
/*--------------------------------------------------------------------------*/
