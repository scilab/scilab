/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2005 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* This file is kept for compability: because MALLOC & others are "#define"
 * The dependency contains the directly call to MyAlloc, etc */

void * MyReAlloc(void * lpAddress, size_t dwSize, const char *file, int line)
{
    void * NewPointer = realloc(lpAddress, dwSize);

    if (NewPointer == NULL)
    {
#ifndef NDEBUG
        printf("REALLOC returns NULL Error File %s Line %d \n", file, line);
        fflush(NULL);
#endif
    }
    return NewPointer;
}
/*-----------------------------------------------------------------------------------*/
void * MyAlloc(size_t dwSize, const char *file, int line)
{
    void * NewPointer = malloc(dwSize);

    if (dwSize > 0)
    {
        if (NewPointer == NULL)
        {
#ifndef NDEBUG
            printf("MALLOC returns NULL Error File %s Line %d \n", file, line);
            fflush(NULL);
#endif
        }
    }
    else
    {
#ifndef NDEBUG
        printf("MALLOC incorrect Size Error File %s Line %d \n", file, line);
        fflush(NULL);
#endif
    }
    return NewPointer;

}

/*-----------------------------------------------------------------------------------*/
void * MyCalloc(size_t x, size_t y, const char *file, int line)
{
    void * NewPointer = calloc(x, y);

    if ((x) * (y) > 0)
    {
        if (NewPointer == NULL)
        {
#ifndef NDEBUG
            printf("CALLOC returns NULL Error File %s Line %d \n", file, line);
            fflush(NULL);
#endif
        }
    }
    else
    {
#ifndef NDEBUG
        printf("CALLOC incorrect size Error File %s Line %d \n", file, line);
        fflush(NULL);
#endif
    }
    return NewPointer;

}
/*-----------------------------------------------------------------------------------*/
void MyFree(void *x)
{
    if (x != NULL)
    {
        free(x);
    }
}
/*-----------------------------------------------------------------------------------*/
