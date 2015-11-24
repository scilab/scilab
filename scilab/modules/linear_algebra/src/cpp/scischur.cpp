/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "configvariable.hxx"
#include "callable.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "function.hxx"

extern "C"
{
#include "scischur.h"
#include "elem_common.h"
}

int schur_sb02mw(double* _real, double* _img)
{
    return dpythags(*_real, *_img) < 1 ? 1 : 0;
}
int schur_sb02mv(double* _real, double* _img)
{
    /* original Fortran code does not use _img aka IEIG (SB02MV = REIG.LT.ZERO) */
    return *_real < 0 ? 1 : 0;
}
int schur_dgees(double* _real, double* _img)
{
    types::Callable* pCall = ConfigVariable::getSchurFunction();
    int iRet = 0;

    if (pCall)
    {
        types::typed_list in;
        types::typed_list out;
        types::optional_list opt;
        int iRetCount = 1;

        types::Double* pDbl = new types::Double(*_real, *_img);
        pDbl->IncreaseRef();
        in.push_back(pDbl);

        bool bOk = pCall->call(in, opt, iRetCount, out) == types::Function::OK;
        pDbl->DecreaseRef();
        delete pDbl;
        pDbl = NULL;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
        }

        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            iRet = pDblOut->get(0) == 0 ? 0 : 1;
            delete pDblOut;
            pDblOut = NULL;

            return iRet;
        }
        else if (out[0]->isBool())
        {
            types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
            iRet = pBoolOut->get(0) == 0 ? 0 : 1;
            delete pBoolOut;
            pBoolOut = NULL;

            return iRet;
        }
    }
    return 0;
}

int schur_sb02ox(double* _real, double* _img, double* _beta) // discrete
{
    return dpythags(*_real, *_img) < fabs(*_beta) ? 1 : 0;
}
int schur_sb02ow(double* _real, double* _img, double* _beta) // continu
{
    return  (*_real < 0 && *_beta > 0) ||
            ((*_real > 0 && *_beta < 0) &&
             (fabs(*_beta) > fabs(*_real) * nc_eps_machine())) ? 1 : 0;
}
int schur_dgges(double* _real, double* _img, double* _beta)
{
    types::Callable* pCall = ConfigVariable::getSchurFunction();
    int iRet = 0;

    if (pCall)
    {
        types::typed_list in;
        types::typed_list out;
        types::optional_list opt;
        int iRetCount = 1;

        types::Double* pDblAlpha = new types::Double(*_real, *_img);
        pDblAlpha->IncreaseRef();
        types::Double* pDblBeta  = new types::Double(*_beta);
        pDblBeta->IncreaseRef();
        in.push_back(pDblAlpha);
        in.push_back(pDblBeta);

        bool bOk = pCall->call(in, opt, iRetCount, out) == types::Function::OK;
        pDblAlpha->DecreaseRef();
        delete pDblAlpha;
        pDblAlpha = NULL;
        pDblBeta->DecreaseRef();
        delete pDblBeta;
        pDblBeta = NULL;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
        }

        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            iRet = pDblOut->get(0) == 0 ? 0 : 1;
            delete pDblOut;
            pDblOut = NULL;

            return iRet;
        }
        else if (out[0]->isBool())
        {
            types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
            iRet = pBoolOut->get(0) == 0 ? 0 : 1;
            delete pBoolOut;
            pBoolOut = NULL;

            return iRet;
        }
    }

    return 0;
}

int schur_zb02mw(doublecomplex* _complex)
{
    return dpythags(_complex->r, _complex->i) < 1 ? 1 : 0;
}
int schur_zb02mv(doublecomplex* _complex)
{
    return _complex->r < 0 ? 1 : 0;
}
int schur_zgees(doublecomplex* _complex)
{
    types::Callable* pCall = ConfigVariable::getSchurFunction();
    int iRet = 0;

    if (pCall)
    {
        types::typed_list in;
        types::typed_list out;
        types::optional_list opt;
        int iRetCount = 1;

        types::Double* pDbl = new types::Double(_complex->r, _complex->i);
        pDbl->IncreaseRef();
        in.push_back(pDbl);

        bool bOk = pCall->call(in, opt, iRetCount, out) == types::Function::OK;
        pDbl->DecreaseRef();
        delete pDbl;
        pDbl = NULL;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
        }

        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            iRet = pDblOut->get(0) == 0 ? 0 : 1;
            delete pDblOut;
            pDblOut = NULL;

            return iRet;
        }
        else if (out[0]->isBool())
        {
            types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
            iRet = pBoolOut->get(0) == 0 ? 0 : 1;
            delete pBoolOut;
            pBoolOut = NULL;

            return iRet;
        }
    }
    return 0;
}

int schur_zb02ox(doublecomplex* _alpha, doublecomplex* _beta) // discrete
{
    return dpythags(_alpha->r, _alpha->i) < dpythags(_beta->r, _beta->i) ? 1 : 0;
}
int schur_zb02ow(doublecomplex* _alpha, doublecomplex* _beta) // continu
{
    double absolute = dpythags(_beta->r, _beta->i);
    int res = 0;

    if (absolute)
    {
        res = ((_alpha->r * _beta->r + _alpha->i * _beta->i) / (_beta->r * _beta->r + _beta->i + _beta->i)) < 0 ? 1 : 0;
    }

    return res;
}
int schur_zgges(doublecomplex* _alpha, doublecomplex* _beta)
{
    types::Callable* pCall = ConfigVariable::getSchurFunction();
    int iRet = 0;

    if (pCall)
    {
        types::typed_list in;
        types::typed_list out;
        types::optional_list opt;
        int iRetCount = 1;

        types::Double* pDblAlpha = new types::Double(_alpha->r, _alpha->i);
        pDblAlpha->IncreaseRef();
        types::Double* pDblBeta  = new types::Double(_beta->r, _beta->i);
        pDblBeta->IncreaseRef();
        in.push_back(pDblAlpha);
        in.push_back(pDblBeta);

        bool bOk = pCall->call(in, opt, iRetCount, out) == types::Function::OK;
        pDblAlpha->DecreaseRef();
        delete pDblAlpha;
        pDblAlpha = NULL;
        pDblBeta->DecreaseRef();
        delete pDblBeta;
        pDblBeta = NULL;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
        }

        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            iRet = pDblOut->get(0) == 0 ? 0 : 1;
            delete pDblOut;
            pDblOut = NULL;

            return iRet;
        }
        else if (out[0]->isBool())
        {
            types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
            iRet = pBoolOut->get(0) == 0 ? 0 : 1;
            delete pBoolOut;
            pBoolOut = NULL;

            return iRet;
        }
    }
    return 0;
}

