/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*/

#include "gatewaystruct.hxx"
#include "double.hxx"
#include "polynom.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createPoly)(scilabEnv env, const wchar_t* varname, int complex)
{
    int dims[2] = {1, 1};
    return scilab_createPolyMatrix(env, varname, 2, dims, complex);
}

scilabVar API_PROTO(createPolyMatrix)(scilabEnv env, const wchar_t* varname, int dim, const int* dims, int complex)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createPolyMatrix", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createPolyMatrix", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Polynom* p = new types::Polynom(varname, dim, dims);
    if (p == nullptr)
    {
        scilab_setInternalError(env, L"createPolyMatrix", _W("memory allocation error"));
        return nullptr;
    }

    p->setComplex(complex != 0);
    return (scilabVar)p;
}

scilabVar API_PROTO(createPolyMatrix2d)(scilabEnv env, const wchar_t* varname, int row, int col, int complex)
{
    int dims[2] = {row, col};
    return scilab_createPolyMatrix(env, varname, 2, dims, complex);
}

int API_PROTO(getPolyVarname)(scilabEnv env, scilabVar var, const wchar_t** varname)
{
    types::Polynom* p = (types::Polynom*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPoly() == false)
    {
        scilab_setInternalError(env, L"getPolyArray", _W("var must be a polynomial variable"));
        return STATUS_ERROR;
    }
#endif

    std::wstring v = p->getVariableName();
    *varname = wcsdup(v.data());
    return STATUS_OK;
}

int API_PROTO(getPolyArray)(scilabEnv env, scilabVar var, int index, double** real)
{
    types::Polynom* p = (types::Polynom*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPoly() == false)
    {
        scilab_setInternalError(env, L"getPolyArray", _W("var must be a polynomial variable"));
        return STATUS_ERROR;
    }
#endif
    types::SinglePoly* sp = p->get(index);
    *real = sp->get();
    return sp->getRank();
}

int API_PROTO(getComplexPolyArray)(scilabEnv env, scilabVar var, int index, double** real, double** img)
{
    types::Polynom* p = (types::Polynom*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPoly() == false || p->isComplex() == false)
    {
        scilab_setInternalError(env, L"getComplexPolyArray", _W("var must be a polynomial variable"));
        return STATUS_ERROR;
    }
#endif
    types::SinglePoly* sp = p->get(index);
    *real = sp->get();
    *img = sp->getImg();
    return sp->getRank();
}

scilabStatus API_PROTO(setPolyArray)(scilabEnv env, scilabVar var, int index, int rank, const double* real)
{
    types::Polynom* p = (types::Polynom*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPoly() == false)
    {
        scilab_setInternalError(env, L"setPolyArray", _W("var must be a polynomial variable"));
        return STATUS_ERROR;
    }
#endif

    if (index < 0 || index >= p->getSize())
    {
#ifdef __API_SCILAB_SAFE__
        scilab_setInternalError(env, L"setPolyArray", _W("index out of bounds"));
#endif
        return STATUS_ERROR;
    }
    types::SinglePoly* sp = p->get(index);
    if (sp)
    {
        sp->setRank(rank);
        sp->set(real);
    }
    else
    {
        sp = new types::SinglePoly();
        sp->setRank(rank);
        sp->set(real);
        p->set(index, sp);
    }

    return STATUS_OK;
}

scilabStatus API_PROTO(setComplexPolyArray)(scilabEnv env, scilabVar var, int index, int rank, const double* real, const double* img)
{
    types::Polynom* p = (types::Polynom*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPoly() == false || p->isComplex() == false)
    {
        scilab_setInternalError(env, L"setComplexPolyArray", _W("var must be a polynomial complex variable"));
        return STATUS_ERROR;
    }
#endif

    if (index < 0 || index >= p->getSize())
    {
#ifdef __API_SCILAB_SAFE__
        scilab_setInternalError(env, L"setComplexPolyArray", _W("index out of bounds"));
#endif
        return STATUS_ERROR;
    }

    types::SinglePoly* sp = p->get(index);
    if (sp)
    {
        sp->setRank(rank);
        sp->set(real);
        sp->setImg(img);
    }
    else
    {
        sp = new types::SinglePoly();
        sp->setRank(rank);
        sp->set(real);
        sp->setImg(img);
        p->set(index, sp);
    }

    return STATUS_OK;
}
