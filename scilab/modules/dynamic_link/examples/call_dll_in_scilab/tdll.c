/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include <windows.h>
#include <stdio.h>
#include <math.h>

int WINAPI DllMain (HINSTANCE hInstance ,
                    DWORD reason,
                    PVOID pvReserved)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return 1;
}

void doit (double *i, double *j)
{
    printf("Enter doit %f %f\n", *i, *j);
    *j = *i + 2 * sin(3.14 / 2);
    printf("Exit doit %f %f\n", *i, *j);
}

