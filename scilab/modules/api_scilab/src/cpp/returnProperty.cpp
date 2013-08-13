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

//#include "api_scilab.h"
#include "returnProperty.h"
#include <string.h>
#include "MALLOC.h"
#include "double.hxx"
#include "string.hxx"
#include "graphichandle.hxx"

/*--------------------------------------------------------------------------*/
void* sciReturnEmptyMatrix()
{
    //createEmptyMatrix(_pvCtx, nbInputArgument(_pvCtx) + 1);
    return types::Double::Empty();
}
/*--------------------------------------------------------------------------*/
void* sciReturnString(const char * value)
{
    //createSingleString(_pvCtx, nbInputArgument(_pvCtx) + 1, value)
    return new types::String(value);
}
/*--------------------------------------------------------------------------*/
void* sciReturnChar(char value)
{
    //createSingleString(_pvCtx, nbInputArgument(_pvCtx) + 1, pstValue)
    char pstValue[2] = {value, 0};//createSingleString needs null terminated characters string
    return new types::String(pstValue);
}
/*--------------------------------------------------------------------------*/
void* sciReturnDouble(double value)
{
    //createScalarDouble(_pvCtx, nbInputArgument(_pvCtx) + 1, value)
    return new types::Double(value);
}
/*--------------------------------------------------------------------------*/
void* sciReturnInt(int value)
{
    return sciReturnDouble((double)value);
}
/*--------------------------------------------------------------------------*/
void* sciReturnRowVector(const double values[], int nbValues)
{
    //SciErr sciErr = createMatrixOfDouble(_pvCtx, nbInputArgument(_pvCtx) + 1, 1, nbValues, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    //return 0;

    double* pdbl = NULL;
    types::Double* pD = new types::Double(1, nbValues, &pdbl);
    for (int i = 0 ; i < nbValues ; i++)
    {
        pdbl[i] = values[i];
    }
    return pD;
}
/*--------------------------------------------------------------------------*/
void* sciReturnRowVectorFromInt(const int values[], int nbValues)
{
    //SciErr sciErr = createMatrixOfDoubleAsInteger(_pvCtx, nbInputArgument(_pvCtx) + 1, 1, nbValues, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    //return 0;

    double* pdbl = NULL;
    types::Double* pD = new types::Double(1, nbValues, &pdbl);
    for (int i = 0 ; i < nbValues ; i++)
    {
        pdbl[i] = (double)values[i];
    }
    return pD;
}
/*--------------------------------------------------------------------------*/
void* sciReturnRowIntVector(const int values[], int nbValues)
{
    return sciReturnRowVectorFromInt(values, nbValues);
}
/*--------------------------------------------------------------------------*/
void* sciReturnRowStringVector(char * values[], int nbValues)
{
    return sciReturnStringMatrix(values, 1, nbValues);
}
/*--------------------------------------------------------------------------*/
void* sciReturnHandle(long handle)
{
    //createScalarHandle(_pvCtx, nbInputArgument(_pvCtx) + 1, handle)
    return new types::GraphicHandle(handle);
}
/*--------------------------------------------------------------------------*/
void* sciReturnRowHandleVector(const long handles[], int nbValues)
{
    //int i = 0;
    //long long* pH = NULL;
    //SciErr sciErr = allocMatrixOfHandle(_pvCtx, nbInputArgument(_pvCtx) + 1, 1, nbValues, &pH);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    //for (i = 0 ; i < nbValues ; i++)
    //{
    //    pH[i] = handles[i];
    //}

    //return 0;

    types::GraphicHandle* pH = new types::GraphicHandle(1, nbValues);
    long long* pHandle = pH->get();
    for (int i = 0 ; i < nbValues ; i++)
    {
        pHandle[i] = (long long)handles[i];
    }
    return pH;
}
/*--------------------------------------------------------------------------*/
void* sciReturnColHandleVector(const long handles[], int nbValues)
{
    //int i = 0;
    //long long* pH = NULL;
    //SciErr sciErr = allocMatrixOfHandle(_pvCtx, nbInputArgument(_pvCtx) + 1, nbValues, 1, &pH);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    //for (i = 0 ; i < nbValues ; i++)
    //{
    //    pH[i] = handles[i];
    //}

    //return 0;
    types::GraphicHandle* pH = new types::GraphicHandle(nbValues, 1);
    long long* pHandle = pH->get();
    for (int i = 0 ; i < nbValues ; i++)
    {
        pHandle[i] = (long long)handles[i];
    }
    return pH;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrix(double values[], int nbRow, int nbCol)
{
    //SciErr sciErr = createMatrixOfDouble(_pvCtx, nbInputArgument(_pvCtx) + 1, nbRow, nbCol, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    //return 0;
    double* pdbl = NULL;
    types::Double* pD = new types::Double(nbRow, nbCol, &pdbl);
    for (int i = 0 ; i < nbRow * nbCol ; i++)
    {
        pdbl[i] = values[i];
    }
    return pD;
}
/*--------------------------------------------------------------------------*/
void* sciReturnStringMatrix(char * values[], int nbRow, int nbCol)
{
    //SciErr sciErr = createMatrixOfString(_pvCtx, nbInputArgument(_pvCtx) + 1, nbRow, nbCol, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    //return 0;

    types::String* pS = new types::String(nbRow, nbCol);
    for (int i = 0 ; i < nbRow * nbCol ; i++)
    {
        wchar_t* pwstValue = to_wide_string(values[i]);
        pS->set(i, pwstValue);
        FREE(pwstValue);
    }
    return pS;
}
/*--------------------------------------------------------------------------*/
void* sciReturnUserData(const int * userData, int userDataSize)
{
    //take care of 32 and 64 bits arch.
    if (userDataSize == 1)
    {
        //32 bits
        int* p = (int*)userData;
        return ((types::InternalType*) * p);
    }
    else
    {
        //64 bits
        long long* p = (long long*)userData;
        return ((types::InternalType*) * p);
    }
}
/*--------------------------------------------------------------------------*/
void* sciReturnHypermatOfDouble(int dims[], int ndims, double values[])
{
    //SciErr sciErr = createHypermatOfDouble(_pvCtx, nbInputArgument(_pvCtx) + 1, dims, ndims, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    return 0;
}
/*--------------------------------------------------------------------------*/
void* sciReturnHypermatOfInteger8(int dims[], int ndims, char values[])
{
    //SciErr sciErr = createHypermatOfInteger8(_pvCtx, nbInputArgument(_pvCtx) + 1, dims, ndims, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    return 0;
}
/*--------------------------------------------------------------------------*/
void* sciReturnHypermatOfUnsignedInteger8(int dims[], int ndims, unsigned char values[])
{
    //SciErr sciErr = createHypermatOfUnsignedInteger8(_pvCtx, nbInputArgument(_pvCtx) + 1, dims, ndims, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    return 0;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfInteger8(char values[], int nbRow, int nbCol)
{
    //SciErr sciErr = createMatrixOfInteger8(_pvCtx, nbInputArgument(_pvCtx) + 1, nbRow, nbCol, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    return 0;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfUnsignedInteger8(unsigned char values[], int nbRow, int nbCol)
{
    //SciErr sciErr = createMatrixOfUnsignedInteger8(_pvCtx, nbInputArgument(_pvCtx) + 1, nbRow, nbCol, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    return 0;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfInteger16(short values[], int nbRow, int nbCol)
{
    //SciErr sciErr = createMatrixOfInteger16(_pvCtx, nbInputArgument(_pvCtx) + 1, nbRow, nbCol, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    return 0;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfUnsignedInteger16(unsigned short values[], int nbRow, int nbCol)
{
    //SciErr sciErr = createMatrixOfUnsignedInteger16(_pvCtx, nbInputArgument(_pvCtx) + 1, nbRow, nbCol, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    return 0;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfInteger32(int values[], int nbRow, int nbCol)
{
    //SciErr sciErr = createMatrixOfInteger32(_pvCtx, nbInputArgument(_pvCtx) + 1, nbRow, nbCol, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    return 0;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfUnsignedInteger32(unsigned int values[], int nbRow, int nbCol)
{
    //SciErr sciErr = createMatrixOfUnsignedInteger32(_pvCtx, nbInputArgument(_pvCtx) + 1, nbRow, nbCol, values);
    //if (sciErr.iErr)
    //{
    //    printError(&sciErr, 0);
    //    return 1;
    //}

    return 0;
}
/*--------------------------------------------------------------------------*/
