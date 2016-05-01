/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2005 - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
