/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
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
/*------------------------------------------------------------------------*/
/* file: returnProperty.c                                                 */
/* desc : a set of functions used to return values in Scilab              */
/*------------------------------------------------------------------------*/

#include "returnProperty.h"
#include <string.h>
#include "sci_malloc.h"
#include "double.hxx"
#include "int.hxx"
#include "string.hxx"
#include "graphichandle.hxx"

/*--------------------------------------------------------------------------*/
void* sciReturnEmptyMatrix()
{
    return types::Double::Empty();
}
/*--------------------------------------------------------------------------*/
void* sciReturnString(const char * value)
{
    return new types::String(value);
}
/*--------------------------------------------------------------------------*/
void* sciReturnChar(char value)
{
    char pstValue[2] = {value, 0};//createSingleString needs null terminated characters string
    return new types::String(pstValue);
}
/*--------------------------------------------------------------------------*/
void* sciReturnDouble(double value)
{
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
    return new types::GraphicHandle(handle);
}
/*--------------------------------------------------------------------------*/
void* sciReturnRowHandleVector(const long handles[], int nbValues)
{
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
    types::Double* pOut = new types::Double(ndims, dims);
    double* pValues = pOut->get();
    memcpy(pValues, values, pOut->getSize() * sizeof(double));
    return pOut;
}
/*--------------------------------------------------------------------------*/
void* sciReturnHypermatOfInteger8(int dims[], int ndims, char values[])
{
    types::Int8* pOut = new types::Int8(ndims, dims);
    char* pValues = pOut->get();
    memcpy(pValues, values, pOut->getSize() * sizeof(char));
    return pOut;
}
/*--------------------------------------------------------------------------*/
void* sciReturnHypermatOfUnsignedInteger8(int dims[], int ndims, unsigned char values[])
{
    types::UInt8* pOut = new types::UInt8(ndims, dims);
    unsigned char* pValues = pOut->get();
    memcpy(pValues, values, pOut->getSize() * sizeof(unsigned char));
    return pOut;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfInteger8(char values[], int nbRow, int nbCol)
{
    types::Int8* pOut = new types::Int8(nbRow, nbCol);
    char* pValues = pOut->get();
    memcpy(pValues, values, pOut->getSize() * sizeof(char));
    return pOut;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfUnsignedInteger8(unsigned char values[], int nbRow, int nbCol)
{
    types::UInt8* pOut = new types::UInt8(nbRow, nbCol);
    unsigned char* pValues = pOut->get();
    memcpy(pValues, values, pOut->getSize() * sizeof(unsigned char));
    return pOut;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfInteger16(short values[], int nbRow, int nbCol)
{
    types::Int16* pOut = new types::Int16(nbRow, nbCol);
    short* pValues = pOut->get();
    memcpy(pValues, values, pOut->getSize() * sizeof(short));
    return pOut;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfUnsignedInteger16(unsigned short values[], int nbRow, int nbCol)
{
    types::UInt16* pOut = new types::UInt16(nbRow, nbCol);
    unsigned short* pValues = pOut->get();
    memcpy(pValues, values, pOut->getSize() * sizeof(unsigned short));
    return pOut;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfInteger32(int values[], int nbRow, int nbCol)
{
    types::Int32* pOut = new types::Int32(nbRow, nbCol);
    int* pValues = pOut->get();
    memcpy(pValues, values, pOut->getSize() * sizeof(int));
    return pOut;
}
/*--------------------------------------------------------------------------*/
void* sciReturnMatrixOfUnsignedInteger32(unsigned int values[], int nbRow, int nbCol)
{
    types::UInt32* pOut = new types::UInt32(nbRow, nbCol);
    unsigned int* pValues = pOut->get();
    memcpy(pValues, values, pOut->getSize() * sizeof(unsigned int));
    return pOut;
}
/*--------------------------------------------------------------------------*/
