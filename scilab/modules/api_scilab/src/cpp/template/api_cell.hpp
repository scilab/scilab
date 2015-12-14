/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#include "gatewaystruct.hxx"
#include "cell.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createCellMatrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createCell", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createCell", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Cell* c = new types::Cell(dim, dims);
#ifdef __API_SCILAB_SAFE__
    if (c == nullptr)
    {
        scilab_setInternalError(env, L"createCell", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)c;
}

scilabVar API_PROTO(createCellMatrix2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};
#ifdef __API_SCILAB_SAFE__
    for (int i = 0; i < 2; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createCell2d", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Cell* c = new types::Cell(2, dims);
#ifdef __API_SCILAB_SAFE__
    if (c == nullptr)
    {
        scilab_setInternalError(env, L"createCell2d", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)c;
}

scilabStatus API_PROTO(getCellValue)(scilabEnv env, scilabVar var, int* index, scilabVar* const val)
{
    types::Cell* c = (types::Cell*)var;
#ifdef __API_SCILAB_SAFE__
    if (c->isCell() == false)
    {
        scilab_setInternalError(env, L"getCellValue", _W("var must be a cell variable"));
        return STATUS_ERROR;
    }
#endif

    int i = c->getIndex(index);
    *val = (scilabVar)c->get(i);
    return STATUS_OK;
}

scilabStatus API_PROTO(getCell2dValue)(scilabEnv env, scilabVar var, int row, int col, scilabVar* const val)
{
    int index[2] = {row, col};
    types::Cell* c = (types::Cell*)var;
#ifdef __API_SCILAB_SAFE__
    if (c->isCell() == false)
    {
        scilab_setInternalError(env, L"getCell2dValue", _W("var must be a cell variable"));
        return STATUS_ERROR;
    }
#endif

    int i = c->getIndex(index);
    *val = (scilabVar)c->get(i);
    return STATUS_OK;
}

scilabStatus API_PROTO(setCellValue)(scilabEnv env, scilabVar var, int* index, scilabVar val)
{
    types::Cell* c = (types::Cell*)var;
#ifdef __API_SCILAB_SAFE__
    if (c->isCell() == false)
    {
        scilab_setInternalError(env, L"setCellArray", _W("var must be a cell variable"));
        return STATUS_ERROR;
    }
#endif
    int i = c->getIndex(index);
    bool bset = c->set(i, (types::InternalType*)val);
#ifdef __API_SCILAB_SAFE__
    if (bset == false)
    {
        scilab_setInternalError(env, L"setCellValue", _W("unable to set data"));
        return STATUS_ERROR;
    }
#endif
    return bset ? STATUS_OK : STATUS_ERROR;
}

scilabStatus API_PROTO(setCell2dValue)(scilabEnv env, scilabVar var, int row, int col, scilabVar val)
{
    int index[2] = {row, col};
    types::Cell* c = (types::Cell*)var;
#ifdef __API_SCILAB_SAFE__
    if (c->isCell() == false)
    {
        scilab_setInternalError(env, L"setCell2dValue", _W("var must be a cell variable"));
        return STATUS_ERROR;
    }
#endif
    int i = c->getIndex(index);
    bool bset = c->set(i, (types::InternalType*)val);
#ifdef __API_SCILAB_SAFE__
    if (bset == false)
    {
        scilab_setInternalError(env, L"setCell2dValue", _W("unable to set data"));
        return STATUS_ERROR;
    }
#endif
    return bset ? STATUS_OK : STATUS_ERROR;
}
