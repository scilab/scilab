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

extern "C"
{
#include "doublecomplex.h"
#include "api_scilab.h"
}
/* Scilab 6 API*/
scilabVar API_PROTO(createEmptyMatrix)(scilabEnv env)
{
    scilabVar var = (scilabVar)types::Double::Empty();
#ifdef __API_SCILAB_SAFE__
    if (var == nullptr)
    {
        scilab_setInternalError(env, L"createEmptyMatrix", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return var;
}

scilabVar API_PROTO(createDoubleMatrix)(scilabEnv env, int dim, const int* dims, int complex)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createDoubleMatrix", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createDoubleMatrix", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Double* d = new types::Double(dim, dims, complex == 1);
#ifdef __API_SCILAB_SAFE__
    if (d == nullptr)
    {
        scilab_setInternalError(env, L"createDoubleMatrix", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)d;
}

scilabVar API_PROTO(createDoubleMatrix2d)(scilabEnv env, int row, int col, int complex)
{
    int dims[2] = {row, col};
#ifdef __API_SCILAB_SAFE__
    for (int i = 0; i < 2; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createDoubleMatrix2d", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Double* d = new types::Double(2, dims, complex == 1);
#ifdef __API_SCILAB_SAFE__
    if (d == nullptr)
    {
        scilab_setInternalError(env, L"createDoubleMatrix2d", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)d;
}

scilabVar API_PROTO(createDouble)(scilabEnv env, double val)
{
    types::Double* d = new types::Double(val);
#ifdef __API_SCILAB_SAFE__
    if (d == nullptr)
    {
        scilab_setInternalError(env, L"createDouble", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)d;
}

scilabVar API_PROTO(createDoubleComplex)(scilabEnv env, double real, double img)
{
    types::Double* d = new types::Double(real, img);
#ifdef __API_SCILAB_SAFE__
    if (d == nullptr)
    {
        scilab_setInternalError(env, L"createDoubleComplex", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)d;
}

scilabStatus API_PROTO(getDoubleArray)(scilabEnv env, scilabVar var, double** real)
{
    types::Double* d = (types::Double*)var;
#ifdef __API_SCILAB_SAFE__
    if (d->isDouble() == false)
    {
        scilab_setInternalError(env, L"getDoubleArray", _W("var must be a double variable"));
        return STATUS_ERROR;
    }
#endif
    *real = d->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(getDoubleComplexArray)(scilabEnv env, scilabVar var, double** real, double** img)
{
    types::Double* d = (types::Double*)var;
#ifdef __API_SCILAB_SAFE__
    if (d->isDouble() == false || d->isComplex() == false)
    {
        scilab_setInternalError(env, L"getDoubleComplexArray", _W("var must be a double complex variable"));
        return STATUS_ERROR;
    }
#endif
    *real = d->get();
    *img = d->getImg();
    return STATUS_OK;
}

scilabStatus API_PROTO(getDouble)(scilabEnv env, scilabVar var, double* real)
{
    types::Double* d = (types::Double*)var;
#ifdef __API_SCILAB_SAFE__
    if (d->isDouble() == false || d->isScalar() == false)
    {
        scilab_setInternalError(env, L"getDouble", _W("var must be a scalar double variable"));
        return STATUS_ERROR;
    }
#endif
    *real = d->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(getDoubleComplex)(scilabEnv env, scilabVar var, double* real, double* img)
{
    types::Double* d = (types::Double*)var;
#ifdef __API_SCILAB_SAFE__
    if (d->isDouble() == false || d->isScalar() == false || d->isComplex() == false)
    {
        scilab_setInternalError(env, L"getDoubleComplex", _W("var must be a scalar double complex variable"));
        return STATUS_ERROR;
    }
#endif
    *real = d->get()[0];
    *img = d->getImg()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(setDoubleArray)(scilabEnv env, scilabVar var, const double* real)
{
    types::Double* d = (types::Double*)var;
#ifdef __API_SCILAB_SAFE__
    if (d->isDouble() == false)
    {
        scilab_setInternalError(env, L"setBooleanArray", _W("var must be a double variable"));
        return STATUS_ERROR;
    }
#endif
    d->set(real);
    return STATUS_OK;
}

scilabStatus API_PROTO(setDoubleComplexArray)(scilabEnv env, scilabVar var, const double* real, const double* img)
{
    types::Double* d = (types::Double*)var;
#ifdef __API_SCILAB_SAFE__
    if (d->isDouble() == false || d->isComplex() == false)
    {
        scilab_setInternalError(env, L"setDoubleComplexArray", _W("var must be a double complex variable"));
        return STATUS_ERROR;
    }
#endif
    d->set(real);
    d->setImg(img);
    return STATUS_OK;
}

scilabStatus API_PROTO(setDouble)(scilabEnv env, scilabVar var, double real)
{
    types::Double* d = (types::Double*)var;
#ifdef __API_SCILAB_SAFE__
    if (d->isDouble() == false || d->isScalar() == false)
    {
        scilab_setInternalError(env, L"setDouble", _W("var must be a scalar double variable"));
        return STATUS_ERROR;
    }
#endif
    d->get()[0] = real;
    return STATUS_OK;
}

scilabStatus API_PROTO(setDoubleComplex)(scilabEnv env, scilabVar var, double real, double img)
{
    types::Double* d = (types::Double*)var;
#ifdef __API_SCILAB_SAFE__
    if (d->isDouble() == false || d->isScalar() == false || d->isComplex() == false)
    {
        scilab_setInternalError(env, L"setDoubleComplex", _W("var must be a scalar double complex variable"));
        return STATUS_ERROR;
    }
#endif
    d->get()[0] = real;
    d->getImg()[0] = img;
    return STATUS_OK;
}

