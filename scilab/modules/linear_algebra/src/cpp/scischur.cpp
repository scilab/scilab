/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
/*--------------------------------------------------------------------------*/
#include "configvariable.hxx"
#include "callable.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "function.hxx"
#include "commentexp.hxx"

extern "C"
{
#include "scischur.h"
#include "elem_common.h"
}

int schur_sb02mw(double* _real, double* _img)
{
    return dpythags(*_real, *_img) < 1 ? 1 : 0;
}
int schur_sb02mv(double* _real, double* /*_img*/)
{
    /* original Fortran code does not use _img aka IEIG (SB02MV = REIG.LT.ZERO) */
    return *_real < 0 ? 1 : 0;
}
int schur_dgees(double* _real, double* _img)
{
    types::Callable* pCall = ConfigVariable::getSchurFunction();
    if (pCall == NULL)
    {
        return 0;
    }

    char errorMsg[256];
    int iRet = 0;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;
    int iRetCount = 1;

    types::Double* pDbl = new types::Double(*_real, *_img);
    pDbl->IncreaseRef();
    in.push_back(pDbl);

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        pCall->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalAbort& ia)
    {
        pDbl->DecreaseRef();
        pDbl->killMe();
        throw ia;
    }
    catch (const ast::InternalError& ie)
    {
        pDbl->DecreaseRef();
        pDbl->killMe();
        throw ie;
    }

    pDbl->DecreaseRef();
    pDbl->killMe();

    if (out.size() != 1)
    {
        char* pstrName = wide_string_to_UTF8(pCall->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (out[0]->isDouble())
    {
        types::Double* pDblOut = out[0]->getAs<types::Double>();
        iRet = pDblOut->get(0) == 0 ? 0 : 1;
        pDblOut->killMe();
    }
    else if (out[0]->isBool())
    {
        types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
        iRet = pBoolOut->get(0) == 0 ? 0 : 1;
        pBoolOut->killMe();
    }

    return iRet;
}

int schur_sb02ox(double* _real, double* _img, double* _beta) // discrete
{
    return dpythags(*_real, *_img) < fabs(*_beta) ? 1 : 0;
}
int schur_sb02ow(double* _real, double* /*_img*/, double* _beta) // continu
{
    return  (*_real < 0 && *_beta > 0) ||
            ((*_real > 0 && *_beta < 0) &&
             (fabs(*_beta) > fabs(*_real) * nc_eps_machine())) ? 1 : 0;
}
int schur_dgges(double* _real, double* _img, double* _beta)
{
    types::Callable* pCall = ConfigVariable::getSchurFunction();
    if (pCall == NULL)
    {
        return 0;
    }

    char errorMsg[256];
    int iRet = 0;

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

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        pCall->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalAbort& ia)
    {
        pDblAlpha->DecreaseRef();
        pDblAlpha->killMe();
        pDblBeta->DecreaseRef();
        pDblBeta->killMe();
        throw ia;
    }
    catch (const ast::InternalError& ie)
    {
        pDblAlpha->DecreaseRef();
        pDblAlpha->killMe();
        pDblBeta->DecreaseRef();
        pDblBeta->killMe();
        throw ie;
    }

    pDblAlpha->DecreaseRef();
    pDblAlpha->killMe();
    pDblBeta->DecreaseRef();
    pDblBeta->killMe();

    if (out.size() != 1)
    {
        char* pstrName = wide_string_to_UTF8(pCall->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (out[0]->isDouble())
    {
        types::Double* pDblOut = out[0]->getAs<types::Double>();
        iRet = pDblOut->get(0) == 0 ? 0 : 1;
        pDblOut->killMe();
    }
    else if (out[0]->isBool())
    {
        types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
        iRet = pBoolOut->get(0) == 0 ? 0 : 1;
        pBoolOut->killMe();
    }

    return iRet;
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
    if (pCall == NULL)
    {
        return 0;
    }

    char errorMsg[256];
    int iRet = 0;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;
    int iRetCount = 1;

    types::Double* pDbl = new types::Double(_complex->r, _complex->i);
    pDbl->IncreaseRef();
    in.push_back(pDbl);

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        pCall->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalAbort& ia)
    {
        pDbl->DecreaseRef();
        pDbl->killMe();
        throw ia;
    }
    catch (const ast::InternalError& ie)
    {
        pDbl->DecreaseRef();
        pDbl->killMe();
        throw ie;
    }

    pDbl->DecreaseRef();
    pDbl->killMe();

    if (out.size() != 1)
    {
        char* pstrName = wide_string_to_UTF8(pCall->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (out[0]->isDouble())
    {
        types::Double* pDblOut = out[0]->getAs<types::Double>();
        iRet = pDblOut->get(0) == 0 ? 0 : 1;
        pDblOut->killMe();
    }
    else if (out[0]->isBool())
    {
        types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
        iRet = pBoolOut->get(0) == 0 ? 0 : 1;
        pBoolOut->killMe();
    }

    return iRet;
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
    if (pCall == NULL)
    {
        return 0;
    }

    char errorMsg[256];
    int iRet = 0;

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

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        pCall->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalAbort& ia)
    {
        pDblAlpha->DecreaseRef();
        pDblAlpha->killMe();
        pDblBeta->DecreaseRef();
        pDblBeta->killMe();
        throw ia;
    }
    catch (const ast::InternalError& ie)
    {
        pDblAlpha->DecreaseRef();
        pDblAlpha->killMe();
        pDblBeta->DecreaseRef();
        pDblBeta->killMe();
        throw ie;
    }

    pDblAlpha->DecreaseRef();
    pDblAlpha->killMe();
    pDblBeta->DecreaseRef();
    pDblBeta->killMe();

    if (out.size() != 1)
    {
        char* pstrName = wide_string_to_UTF8(pCall->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (out[0]->isDouble())
    {
        types::Double* pDblOut = out[0]->getAs<types::Double>();
        iRet = pDblOut->get(0) == 0 ? 0 : 1;
        pDblOut->killMe();
    }
    else if (out[0]->isBool())
    {
        types::Bool* pBoolOut = out[0]->getAs<types::Bool>();
        iRet = pBoolOut->get(0) == 0 ? 0 : 1;
        pBoolOut->killMe();
    }

    return iRet;
}

