/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*------------------------------------------------------------------------*/
/* file: returnProperty.c                                                 */
/* desc : a set of functions used to return values in Scilab              */
/*------------------------------------------------------------------------*/

#include "api_scilab.h"
#include "returnProperty.h"
#include <string.h>

/*--------------------------------------------------------------------------*/
int sciReturnEmptyMatrix(void* _pvCtx)
{
    return createEmptyMatrix(_pvCtx, nbInputArgument(_pvCtx) + 1);
}
/*--------------------------------------------------------------------------*/
int sciReturnString(void* _pvCtx, const char * value)
{
    return createSingleString(_pvCtx, nbInputArgument(_pvCtx) + 1, value);
}
/*--------------------------------------------------------------------------*/
int sciReturnChar(void* _pvCtx, char value)
{
    char pstValue[2] = {value, 0};//createSingleString needs null terminated characters string
    return createSingleString(_pvCtx, nbInputArgument(_pvCtx) + 1, pstValue);
}
/*--------------------------------------------------------------------------*/
int sciReturnDouble(void* _pvCtx, double value)
{
    return createScalarDouble(_pvCtx, nbInputArgument(_pvCtx) + 1, value);
}
/*--------------------------------------------------------------------------*/
int sciReturnInt(void* _pvCtx, int value)
{
    return sciReturnDouble(_pvCtx, (double)value);
}
/*--------------------------------------------------------------------------*/
int sciReturnRowVector(void* _pvCtx, const double values[], int nbValues)
{
    SciErr sciErr = createMatrixOfDouble(_pvCtx, nbInputArgument(_pvCtx) + 1, 1, nbValues, values);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int sciReturnRowVectorFromInt(void* _pvCtx, const int values[], int nbValues)
{
    SciErr sciErr = createMatrixOfDoubleAsInteger(_pvCtx, nbInputArgument(_pvCtx) + 1, 1, nbValues, values);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int sciReturnRowIntVector(void* _pvCtx, const int values[], int nbValues)
{
    return sciReturnRowVectorFromInt(_pvCtx, values, nbValues);
}
/*--------------------------------------------------------------------------*/
int sciReturnRowStringVector(void* _pvCtx, char * values[], int nbValues)
{
    return sciReturnStringMatrix(_pvCtx, values, 1, nbValues);
}
/*--------------------------------------------------------------------------*/
int sciReturnHandle(void* _pvCtx, long handle)
{
    return createScalarHandle(_pvCtx, nbInputArgument(_pvCtx) + 1, handle);
}
/*--------------------------------------------------------------------------*/
int sciReturnRowHandleVector(void* _pvCtx, const long handles[], int nbValues)
{
    int i = 0;
    long long* pH = NULL;
    SciErr sciErr = allocMatrixOfHandle(_pvCtx, nbInputArgument(_pvCtx) + 1, 1, nbValues, &pH);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    for (i = 0 ; i < nbValues ; i++)
    {
        pH[i] = handles[i];
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int sciReturnColHandleVector(void* _pvCtx, const long handles[], int nbValues)
{
    int i = 0;
    long long* pH = NULL;
    SciErr sciErr = allocMatrixOfHandle(_pvCtx, nbInputArgument(_pvCtx) + 1, nbValues, 1, &pH);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    for (i = 0 ; i < nbValues ; i++)
    {
        pH[i] = handles[i];
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int sciReturnMatrix(void* _pvCtx, double values[], int nbRow, int nbCol)
{
    SciErr sciErr = createMatrixOfDouble(_pvCtx, nbInputArgument(_pvCtx) + 1, nbRow, nbCol, values);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int sciReturnStringMatrix(void* _pvCtx, char * values[], int nbRow, int nbCol)
{
    SciErr sciErr = createMatrixOfString(_pvCtx, nbInputArgument(_pvCtx) + 1, nbRow, nbCol, values);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int sciReturnUserData(void* _pvCtx, const int * userData, int userDataSize)
{
    /*NOT COMPATIBLE WITH SCILAB 6*/
    //int * data_ptr = NULL ;
    //int data_size = (userDataSize + 1) / 2 ;
    //SetWorkSize(nbInputArgument(_pvCtx) + 1, &data_size);
    //data_ptr = (int*)GetRawData(nbInputArgument(_pvCtx) + 1);
    //memcpy(data_ptr, userData, userDataSize * sizeof (int));
    return 0 ;
}
/*--------------------------------------------------------------------------*/
