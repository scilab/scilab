/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
#include <list>

#include "string.hxx"
#include "double.hxx"
#include "differentialequationfunctions.hxx"
#include "configvariable.hxx"
#include "commentexp.hxx"

extern "C"
{
#include "elem_common.h"
#include "scifunctions.h"
#include "Ex-odedc.h"
#include "Ex-ode.h"
#include "Ex-daskr.h"
#include "localization.h"
}

/*
** differential equation functions
** \{
*/

// need the current thread, not the last running thread.

std::list<DifferentialEquationFunctions*> DifferentialEquation::m_DifferentialEquationFunctions;

using namespace types;
void DifferentialEquation::addDifferentialEquationFunctions(DifferentialEquationFunctions* _deFunction)
{
    m_DifferentialEquationFunctions.push_back(_deFunction);
}

void DifferentialEquation::removeDifferentialEquationFunctions()
{
    m_DifferentialEquationFunctions.pop_back();
}

DifferentialEquationFunctions* DifferentialEquation::getDifferentialEquationFunctions()
{
    return m_DifferentialEquationFunctions.back();
}


/*
** \}
*/
/*--------------------------------------------------------------------------*/
DifferentialEquationFunctions::DifferentialEquationFunctions(const std::wstring& callerName)
{
    m_odeYRows      = 0;
    m_odeYCols      = 0;
    m_odedcYDSize   = 0;
    m_odedcFlag     = 0;
    m_bvodeM        = 0;
    m_bvodeN        = 0;
    m_mu            = 0;
    m_ml            = 0;
    m_bandedJac     = false;

    m_wstrCaller = callerName;

    // callable
    m_pCallFFunction      = NULL;
    m_pCallJacFunction    = NULL;
    m_pCallGFunction      = NULL;
    m_pCallPjacFunction   = NULL;
    m_pCallPsolFunction   = NULL;

    // function extern
    m_pStringFFunctionDyn       = NULL;
    m_pStringJacFunctionDyn     = NULL;
    m_pStringGFunctionDyn       = NULL;
    m_pStringPjacFunctionDyn    = NULL;
    m_pStringPsolFunctionDyn    = NULL;

    // function static
    m_pStringFFunctionStatic    = NULL;
    m_pStringJacFunctionStatic  = NULL;
    m_pStringGFunctionStatic    = NULL;
    m_pStringPjacFunctionStatic = NULL;
    m_pStringPsolFunctionStatic = NULL;

    // bvode
    m_pCallFsubFunction     = NULL;
    m_pCallDfsubFunction    = NULL;
    m_pCallGsubFunction     = NULL;
    m_pCallDgsubFunction    = NULL;
    m_pCallGuessFunction    = NULL;

    m_pStringFsubFunctionDyn    = NULL;
    m_pStringDfsubFunctionDyn   = NULL;
    m_pStringGsubFunctionDyn    = NULL;
    m_pStringDgsubFunctionDyn   = NULL;
    m_pStringGuessFunctionDyn   = NULL;

    m_pStringFsubFunctionStatic     = NULL;
    m_pStringDfsubFunctionStatic    = NULL;
    m_pStringGsubFunctionStatic     = NULL;
    m_pStringDgsubFunctionStatic    = NULL;
    m_pStringGuessFunctionStatic    = NULL;

    // init static functions
    if (callerName == L"ode")
    {
        m_staticFunctionMap[L"arnol"]   = (void*) C2F(arnol);
        m_staticFunctionMap[L"fex"]     = (void*) fex;
        m_staticFunctionMap[L"fex2"]    = (void*) fex2;
        m_staticFunctionMap[L"fex3"]    = (void*) fex3;
        m_staticFunctionMap[L"fexab"]   = (void*) fexab;
        m_staticFunctionMap[L"loren"]   = (void*) C2F(loren);
        m_staticFunctionMap[L"bcomp"]   = (void*) C2F(bcomp);
        m_staticFunctionMap[L"lcomp"]   = (void*) C2F(lcomp);

        m_staticFunctionMap[L"jex"]     = (void*) jex;
    }
    else if (callerName == L"odedc")
    {
        m_staticFunctionMap[L"fcd"]     = (void*) fcd;
        m_staticFunctionMap[L"fcd1"]    = (void*) fcd1;
        m_staticFunctionMap[L"fexcd"]   = (void*) fexcd;
        m_staticFunctionMap[L"phis"]    = (void*) phis;
        m_staticFunctionMap[L"phit"]    = (void*) phit;

        m_staticFunctionMap[L"jex"]     = (void*) jex;
    }
    else if (callerName == L"intg")
    {
        m_staticFunctionMap[L"intgex"]  = (void*) C2F(intgex);
    }
    else if (callerName == L"int2d")
    {
        m_staticFunctionMap[L"int2dex"] = (void*) C2F(int2dex);
    }
    else if (callerName == L"int3d")
    {
        m_staticFunctionMap[L"int3dex"] = (void*) C2F(int3dex);
    }
    else if (callerName == L"feval")
    {
        m_staticFunctionMap[L"parab"]   = (void*) C2F(parab);
        m_staticFunctionMap[L"parabc"]  = (void*) C2F(parabc);
    }
    else if (callerName == L"bvode")
    {
        m_staticFunctionMap[L"cndg"]    = (void*) C2F(cndg);
        m_staticFunctionMap[L"cng"]     = (void*) C2F(cng);
        m_staticFunctionMap[L"cnf"]     = (void*) C2F(cnf);
        m_staticFunctionMap[L"cndf"]    = (void*) C2F(cndf);
        m_staticFunctionMap[L"cngu"]    = (void*) C2F(cngu);
    }
    else if (callerName == L"impl")
    {
        m_staticFunctionMap[L"resid"]   = (void*) C2F(resid);  // res
        m_staticFunctionMap[L"aplusp"]  = (void*) C2F(aplusp); // adda
        m_staticFunctionMap[L"dgbydy"]  = (void*) C2F(dgbydy); // jac
    }
    else if (callerName == L"dassl" ||
             callerName == L"dasrt" ||
             callerName == L"daskr")
    {
        //res
        m_staticFunctionMap[L"res1"]    = (void*) C2F(res1);
        m_staticFunctionMap[L"res2"]    = (void*) C2F(res2);
        m_staticFunctionMap[L"dres1"]   = (void*) C2F(dres1);
        m_staticFunctionMap[L"dres2"]   = (void*) C2F(dres2);

        // jac
        m_staticFunctionMap[L"jac2"]   = (void*) C2F(jac2);
        m_staticFunctionMap[L"djac2"]  = (void*) C2F(djac2);
        m_staticFunctionMap[L"djac1"]  = (void*) C2F(djac1);

        //g
        if (callerName == L"dasrt" || callerName == L"daskr")
        {
            m_staticFunctionMap[L"gr1"]  = (void*) C2F(gr1);
            m_staticFunctionMap[L"gr2"]  = (void*) C2F(gr2);
        }

        // pjac, psol
        if (callerName == L"daskr")
        {
            m_staticFunctionMap[L"pjac1"]  = (void*) pjac1;
            m_staticFunctionMap[L"psol1"]  = (void*) psol1;
        }
    }
}

DifferentialEquationFunctions::~DifferentialEquationFunctions()
{
    m_staticFunctionMap.clear();
}

/*------------------------------- public -------------------------------------------*/
void DifferentialEquationFunctions::execDasrtG(int* ny, double* t, double* y, int* ng, double* gout, double* rpar, int* ipar)
{
    char errorMsg[256];
    if (m_pCallGFunction)
    {
        callDasrtMacroG(ny, t, y, ng, gout, rpar, ipar);
    }
    else if (m_pStringGFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringGFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringGFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((dasrt_g_t)(func->functionPtr))(ny, t, y, ng, gout, rpar, ipar);
    }
    else if (m_pStringGFunctionStatic)
    {
        ((dasrt_g_t)m_staticFunctionMap[m_pStringGFunctionStatic->get(0)])(ny, t, y, ng, gout, rpar, ipar);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "g");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execDasslF(double* t, double* y, double* ydot, double* delta, int* ires, double* rpar, int* ipar)
{
    char errorMsg[256];
    if (m_pCallFFunction)
    {
        callDasslMacroF(t, y, ydot, delta, ires, rpar, ipar);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((dassl_f_t)(func->functionPtr))(t, y, ydot, delta, ires, rpar, ipar);
    }
    else if (m_pStringFFunctionStatic)
    {
        ((dassl_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(t, y, ydot, delta, ires, rpar, ipar);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "f");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execDasslJac(double* t, double* y, double* ydot, double* pd, double* cj, double* rpar, int* ipar)
{
    char errorMsg[256];
    if (m_pCallJacFunction)
    {
        callDasslMacroJac(t, y, ydot, pd, cj, rpar, ipar);
    }
    else if (m_pStringJacFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringJacFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringJacFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((dassl_jac_t)(func->functionPtr))(t, y, ydot, pd, cj, rpar, ipar);
    }
    else if (m_pStringJacFunctionStatic)
    {
        ((dassl_jac_t)m_staticFunctionMap[m_pStringJacFunctionStatic->get(0)])(t, y, ydot, pd, cj, rpar, ipar);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "jacobian");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execDaskrPjac(double* res, int* ires, int* neq, double* t, double* y, double* ydot,
        double* rewt, double* savr, double* wk, double* h, double* cj,
        double* wp, int* iwp, int* ier, double* rpar, int* ipar)
{
    char errorMsg[256];
    if (m_pCallPjacFunction)
    {
        callDaskrMacroPjac(res, ires, neq, t, y, ydot, rewt, savr,
                           wk, h, cj, wp, iwp, ier, rpar, ipar);
    }
    else if (m_pStringPjacFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringPjacFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringPjacFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((daskr_pjac_t)(func->functionPtr))(res, ires, neq, t, y, ydot, rewt, savr,
                                            wk, h, cj, wp, iwp, ier, rpar, ipar);
    }
    else if (m_pStringPjacFunctionStatic)
    {
        ((daskr_pjac_t)m_staticFunctionMap[m_pStringPjacFunctionStatic->get(0)])(res, ires, neq, t, y, ydot, rewt, savr,
                wk, h, cj, wp, iwp, ier, rpar, ipar);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "pjac");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execDaskrPsol(int* neq, double* t, double* y, double* ydot, double* savr, double* wk,
        double* cj, double* wght, double* wp, int* iwp, double* b, double* eplin,
        int* ier, double* rpar, int* ipar)
{
    char errorMsg[256];
    if (m_pCallPsolFunction)
    {
        callDaskrMacroPsol(neq, t, y, ydot, savr, wk, cj, wght,
                           wp, iwp, b, eplin, ier, rpar, ipar);
    }
    else if (m_pStringPsolFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringPsolFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringPsolFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((daskr_psol_t)(func->functionPtr))(neq, t, y, ydot, savr, wk, cj, wght,
                                            wp, iwp, b, eplin, ier, rpar, ipar);
    }
    else if (m_pStringPsolFunctionStatic)
    {
        ((daskr_psol_t)m_staticFunctionMap[m_pStringPsolFunctionStatic->get(0)])(neq, t, y, ydot, savr, wk, cj, wght,
                wp, iwp, b, eplin, ier, rpar, ipar);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "psol");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execImplF(int* neq, double* t, double* y, double* s, double* r, int* ires)
{
    char errorMsg[256];
    if (m_pCallFFunction)
    {
        callImplMacroF(neq, t, y, s, r, ires);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((impl_f_t)(func->functionPtr))(neq, t, y, s, r, ires);
    }
    else if (m_pStringFFunctionStatic)
    {
        ((impl_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(neq, t, y, s, r, ires);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "f");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execImplG(int* neq, double* t, double* y, double* ml, double* mu, double* p, int* nrowp)
{
    char errorMsg[256];
    if (m_pCallGFunction)
    {
        callImplMacroG(neq, t, y, ml, mu, p, nrowp);
    }
    else if (m_pStringGFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringGFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringGFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((impl_g_t)(func->functionPtr))(neq, t, y, ml, mu, p, nrowp);
    }
    else if (m_pStringGFunctionStatic)
    {
        ((impl_g_t)m_staticFunctionMap[m_pStringGFunctionStatic->get(0)])(neq, t, y, ml, mu, p, nrowp);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "g");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execImplJac(int* neq, double* t, double* y, double* s, double* ml, double* mu, double* p, int* nrowp)
{
    char errorMsg[256];
    if (m_pCallJacFunction)
    {
        callImplMacroJac(neq, t, y, s, ml, mu, p, nrowp);
    }
    else if (m_pStringJacFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringJacFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringJacFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((impl_jac_t)(func->functionPtr))(neq, t, y, s, ml, mu, p, nrowp);
    }
    else if (m_pStringJacFunctionStatic)
    {
        ((impl_jac_t)m_staticFunctionMap[m_pStringJacFunctionStatic->get(0)])(neq, t, y, s, ml, mu, p, nrowp);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "jacobian");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execBvodeGuess(double *x, double *z, double *d)
{
    char errorMsg[256];
    if (m_pCallGuessFunction)
    {
        callBvodeMacroGuess(x, z, d);
    }
    else if (m_pStringGuessFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringGuessFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringGuessFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((bvode_ddd_t)(func->functionPtr))(x, z, d);
    }
    else if (m_pStringGuessFunctionStatic)
    {
        ((bvode_ddd_t)m_staticFunctionMap[m_pStringGuessFunctionStatic->get(0)])(x, z, d);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "guess");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execBvodeDfsub(double *x, double *z, double *d)
{
    char errorMsg[256];
    if (m_pCallDfsubFunction)
    {
        callBvodeMacroDfsub(x, z, d);
    }
    else if (m_pStringDfsubFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringDfsubFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringDfsubFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((bvode_ddd_t)(func->functionPtr))(x, z, d);
    }
    else if (m_pStringDfsubFunctionStatic)// function static
    {
        ((bvode_ddd_t)m_staticFunctionMap[m_pStringDfsubFunctionStatic->get(0)])(x, z, d);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "fsub");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execBvodeFsub(double *x, double *z, double *d)
{
    char errorMsg[256];
    if (m_pCallFsubFunction)
    {
        callBvodeMacroFsub(x, z, d);
    }
    else if (m_pStringFsubFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFsubFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFsubFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((bvode_ddd_t)(func->functionPtr))(x, z, d);
    }
    else if (m_pStringFsubFunctionStatic) // function static
    {
        ((bvode_ddd_t)m_staticFunctionMap[m_pStringFsubFunctionStatic->get(0)])(x, z, d);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "fsub");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execBvodeDgsub(int *i, double *z, double *g)
{
    char errorMsg[256];
    if (m_pCallDgsubFunction)
    {
        callBvodeMacroDgsub(i, z, g);
    }
    else if (m_pStringDgsubFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringDgsubFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringDgsubFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((bvode_idd_t)(func->functionPtr))(i, z, g);
    }
    else if (m_pStringDgsubFunctionStatic) // function static
    {
        ((bvode_idd_t)m_staticFunctionMap[m_pStringDgsubFunctionStatic->get(0)])(i, z, g);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "gsub");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execBvodeGsub(int *i, double *z, double *g)
{
    char errorMsg[256];
    if (m_pCallGsubFunction)
    {
        callBvodeMacroGsub(i, z, g);
    }
    else if (m_pStringGsubFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringGsubFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringGsubFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((bvode_idd_t)(func->functionPtr))(i, z, g);
    }
    else if (m_pStringGsubFunctionStatic) // function static
    {
        ((bvode_idd_t)m_staticFunctionMap[m_pStringGsubFunctionStatic->get(0)])(i, z, g);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "gsub");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execFevalF(int *nn, double *x1, double *x2, double *xres, int *itype)
{
    char errorMsg[256];
    if (m_pCallFFunction)
    {
        callFevalMacroF(nn, x1, x2, xres, itype);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }

        ((feval_f_t)(func->functionPtr))(nn, x1, x2, xres, itype);
    }
    else if (m_pStringFFunctionStatic) // function static
    {
        ((feval_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(nn, x1, x2, xres, itype);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "f");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execInt3dF(double* x, int* numfun, double* funvls)
{
    char errorMsg[256];
    if (m_pCallFFunction)
    {
        callInt3dMacroF(x, numfun, funvls);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((int3d_f_t)(func->functionPtr))(x, numfun, funvls);
    }
    else if (m_pStringFFunctionStatic) // function static
    {
        ((int3d_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(x, numfun, funvls);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "f");
        throw ast::InternalError(errorMsg);
    }
}

double DifferentialEquationFunctions::execInt2dF(double* x, double* y)
{
    char errorMsg[256];
    if (m_pCallFFunction)
    {
        return callInt2dMacroF(x, y);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        return ((int2d_f_t)(func->functionPtr))(x, y);
    }
    else if (m_pStringFFunctionStatic) // function static
    {
        return ((int2d_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(x, y);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "f");
        throw ast::InternalError(errorMsg);
    }
}

double DifferentialEquationFunctions::execIntgF(double* x)
{
    char errorMsg[256];
    if (m_pCallFFunction)
    {
        return callIntgMacroF(x);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        return ((intg_f_t)(func->functionPtr))(x);
    }
    else if (m_pStringFFunctionStatic) // function static
    {
        return ((intg_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(x);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "f");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execOdeF(int* n, double* t, double* y, double* yout)
{
    char errorMsg[256];
    if (m_pCallFFunction)
    {
        callOdeMacroF(n, t, y, yout);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringFFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }

        if (m_wstrCaller == L"ode")
        {
            ((ode_f_t)(func->functionPtr))(n, t, y, yout);
        }
        else
        {
            ((odedc_f_t)(func->functionPtr))(&m_odedcFlag, n, &m_odedcYDSize, t, y, yout);
        }
    }
    else if (m_pStringFFunctionStatic) // function static
    {
        if (m_wstrCaller == L"ode")
        {
            ((ode_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(n, t, y, yout);
        }
        else // if (m_wstrCaller == L"odedc")
        {
            ((odedc_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(&m_odedcFlag, n, &m_odedcYDSize, t, y, yout);
        }
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "f");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execFunctionJac(int *n, double *t, double *y, int *ml, int *mu, double *J, int *nrpd)
{
    char errorMsg[256];
    if (m_pCallJacFunction)
    {
        callMacroJac(n, t, y, ml, mu, J, nrpd);
    }
    else if (m_pStringJacFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringJacFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringJacFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((func_jac_t)(func->functionPtr))(n, t, y, ml, mu, J, nrpd);
    }
    else if (m_pStringJacFunctionStatic) // function static
    {
        ((func_jac_t)m_staticFunctionMap[m_pStringJacFunctionStatic->get(0)])(n, t, y, ml, mu, J, nrpd);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "jacobian");
        throw ast::InternalError(errorMsg);
    }
}

void DifferentialEquationFunctions::execFunctionG(int* n, double* t, double* y, int* ng, double* gout)
{
    char errorMsg[256];
    if (m_pCallGFunction)
    {
        callMacroG(n, t, y, ng, gout);
    }
    else if (m_pStringGFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringGFunctionDyn->get(0));
        if (func == NULL)
        {
            sprintf(errorMsg, _("Undefined function '%ls'.\n"), m_pStringGFunctionDyn->get(0));
            throw ast::InternalError(errorMsg);
        }
        ((func_g_t)(func->functionPtr))(n, t, y, ng, gout);
    }
    else if (m_pStringGFunctionStatic)// function static
    {
        ((func_g_t)m_staticFunctionMap[m_pStringGFunctionStatic->get(0)])(n, t, y, ng, gout);
    }
    else
    {
        sprintf(errorMsg, _("User function '%s' have not been setted.\n"), "g");
        throw ast::InternalError(errorMsg);
    }
}

//*** setter ***
// set rows cols
void DifferentialEquationFunctions::setOdeYRows(int rows)
{
    m_odeYRows = rows;
}

void DifferentialEquationFunctions::setOdeYCols(int cols)
{
    m_odeYCols = cols;
}

// set odedc yd size
void DifferentialEquationFunctions::setOdedcYDSize(int size)
{
    m_odedcYDSize = size;
}

// set odedc flag
void DifferentialEquationFunctions::setOdedcFlag()
{
    m_odedcFlag = 1;
}

// reset odedc flag
void DifferentialEquationFunctions::resetOdedcFlag()
{
    m_odedcFlag = 0;
}

void DifferentialEquationFunctions::setBvodeM(int _m)
{
    m_bvodeM = _m;
}

void DifferentialEquationFunctions::setBvodeN(int _n)
{
    m_bvodeN = _n;
}

//set function f, jac, g, psol, pjac as types::Callable
void DifferentialEquationFunctions::setFFunction(types::Callable* _odeFFunc)
{
    m_pCallFFunction = _odeFFunc;
}

void DifferentialEquationFunctions::setJacFunction(types::Callable* _odeJacFunc)
{
    m_pCallJacFunction = _odeJacFunc;
}

void DifferentialEquationFunctions::setGFunction(types::Callable* _odeGFunc)
{
    m_pCallGFunction = _odeGFunc;
}

void DifferentialEquationFunctions::setPsolFunction(types::Callable* _pSolFunc)
{
    m_pCallPsolFunction = _pSolFunc;
}

void DifferentialEquationFunctions::setPjacFunction(types::Callable* _pJacFunc)
{
    m_pCallPjacFunction = _pJacFunc;
}

//set function f, jac, g, psol, pjac as types::String
bool DifferentialEquationFunctions::setFFunction(types::String* _odeFFunc)
{
    if (ConfigVariable::getEntryPoint(_odeFFunc->get(0)))
    {
        m_pStringFFunctionDyn = _odeFFunc;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_odeFFunc->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringFFunctionStatic = _odeFFunc;
            return true;
        }
        return false;
    }
}

bool DifferentialEquationFunctions::setJacFunction(types::String* _odeJacFunc)
{
    if (ConfigVariable::getEntryPoint(_odeJacFunc->get(0)))
    {
        m_pStringJacFunctionDyn = _odeJacFunc;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_odeJacFunc->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringJacFunctionStatic = _odeJacFunc;
            return true;
        }
        return false;
    }
}

bool DifferentialEquationFunctions::setGFunction(types::String* _odeGFunc)
{
    if (ConfigVariable::getEntryPoint(_odeGFunc->get(0)))
    {
        m_pStringGFunctionDyn = _odeGFunc;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_odeGFunc->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringGFunctionStatic = _odeGFunc;
            return true;
        }
        return false;
    }
}

bool DifferentialEquationFunctions::setPsolFunction(types::String* _pSolFunc)
{
    if (ConfigVariable::getEntryPoint(_pSolFunc->get(0)))
    {
        m_pStringPsolFunctionDyn = _pSolFunc;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_pSolFunc->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringPsolFunctionStatic = _pSolFunc;
            return true;
        }
        return false;
    }
}

bool DifferentialEquationFunctions::setPjacFunction(types::String* _pJacFunc)
{
    if (ConfigVariable::getEntryPoint(_pJacFunc->get(0)))
    {
        m_pStringPjacFunctionDyn = _pJacFunc;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_pJacFunc->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringPjacFunctionStatic = _pJacFunc;
            return true;
        }
        return false;
    }
}

// set args for f, jac, g, pjac and psol functions
void DifferentialEquationFunctions::setFArgs(types::InternalType* _odeFArg)
{
    m_FArgs.push_back(_odeFArg);
}

void DifferentialEquationFunctions::setJacArgs(types::InternalType* _odeJacArg)
{
    m_JacArgs.push_back(_odeJacArg);
}

void DifferentialEquationFunctions::setGArgs(types::InternalType* _odeGArg)
{
    m_odeGArgs.push_back(_odeGArg);
}

void DifferentialEquationFunctions::setPsolArgs(types::InternalType* _pSolArg)
{
    m_pSolArgs.push_back(_pSolArg);
}

void DifferentialEquationFunctions::setPjacArgs(types::InternalType* _pJacArg)
{
    m_pJacArgs.push_back(_pJacArg);
}

// bvode set function as types::Callable gsub, dgsub, fsub, dfsub, guess
void DifferentialEquationFunctions::setGsubFunction(types::Callable* _func)
{
    m_pCallGsubFunction = _func;
}

void DifferentialEquationFunctions::setDgsubFunction(types::Callable* _func)
{
    m_pCallDgsubFunction = _func;
}

void DifferentialEquationFunctions::setFsubFunction(types::Callable* _func)
{
    m_pCallFsubFunction = _func;
}

void DifferentialEquationFunctions::setDfsubFunction(types::Callable* _func)
{
    m_pCallDfsubFunction = _func;
}

void DifferentialEquationFunctions::setGuessFunction(types::Callable* _func)
{
    m_pCallGuessFunction = _func;
}

// bvode set function as types::String gsub, dgsub, fsub, dfsub, guess
bool DifferentialEquationFunctions::setGsubFunction(types::String* _func)
{
    if (ConfigVariable::getEntryPoint(_func->get(0)))
    {
        m_pStringGsubFunctionDyn = _func;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_func->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringGsubFunctionStatic = _func;
            return true;
        }
        return false;
    }
}

bool DifferentialEquationFunctions::setDgsubFunction(types::String* _func)
{
    if (ConfigVariable::getEntryPoint(_func->get(0)))
    {
        m_pStringDgsubFunctionDyn = _func;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_func->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringDgsubFunctionStatic = _func;
            return true;
        }
        return false;
    }
}

bool DifferentialEquationFunctions::setFsubFunction(types::String* _func)
{
    if (ConfigVariable::getEntryPoint(_func->get(0)))
    {
        m_pStringFsubFunctionDyn = _func;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_func->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringFsubFunctionStatic = _func;
            return true;
        }
        return false;
    }
}

bool DifferentialEquationFunctions::setDfsubFunction(types::String* _func)
{
    if (ConfigVariable::getEntryPoint(_func->get(0)))
    {
        m_pStringDfsubFunctionDyn = _func;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_func->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringDfsubFunctionStatic = _func;
            return true;
        }
        return false;
    }
}

bool DifferentialEquationFunctions::setGuessFunction(types::String* _func)
{
    if (ConfigVariable::getEntryPoint(_func->get(0)))
    {
        m_pStringGuessFunctionDyn = _func;
        return true;
    }
    else
    {
        if (m_staticFunctionMap.find(_func->get(0)) != m_staticFunctionMap.end())
        {
            m_pStringGuessFunctionStatic = _func;
            return true;
        }
        return false;
    }
}

// bvode set set args for gsub, dgsub, fsub, dfsub, guess functions
void DifferentialEquationFunctions::setGsubArgs(types::InternalType* _arg)
{
    m_GsubArgs.push_back(_arg);
}

void DifferentialEquationFunctions::setDgsubArgs(types::InternalType* _arg)
{
    m_DgsubArgs.push_back(_arg);
}

void DifferentialEquationFunctions::setFsubArgs(types::InternalType* _arg)
{
    m_FsubArgs.push_back(_arg);
}

void DifferentialEquationFunctions::setDfsubArgs(types::InternalType* _arg)
{
    m_DfsubArgs.push_back(_arg);
}

void DifferentialEquationFunctions::setGuessArgs(types::InternalType* _arg)
{
    m_GuessArgs.push_back(_arg);
}

// set mu and ml
void DifferentialEquationFunctions::setMu(int mu)
{
    m_bandedJac = true;
    m_mu = mu;
}

void DifferentialEquationFunctions::setMl(int ml)
{
    m_bandedJac = true;
    m_ml = ml;
}

//*** getter ***
// get y rows cols
int DifferentialEquationFunctions::getOdeYRows()
{
    return m_odeYRows;
}

int DifferentialEquationFunctions::getOdeYCols()
{
    return m_odeYCols;
}

// get odedc yd size
int DifferentialEquationFunctions::getOdedcYDSize()
{
    return m_odedcYDSize;
}

// get odedc flag
int DifferentialEquationFunctions::getOdedcFlag()
{
    return m_odedcFlag;
}

/*------------------------------- private -------------------------------------------*/
void DifferentialEquationFunctions::callOdeMacroF(int* n, double* t, double* y, double* ydot)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblY    = NULL;
    types::Double* pDblYC   = NULL;
    types::Double* pDblYD   = NULL;
    types::Double* pDblFlag = NULL;

    // create input args
    types::Double* pDblT = new types::Double(*t);
    pDblT->IncreaseRef();

    if (m_odedcYDSize) // odedc
    {
        pDblYC = new types::Double(*n, 1);
        pDblYC->set(y);
        pDblYC->IncreaseRef();
        pDblYD = new types::Double(m_odedcYDSize, 1);
        pDblYD->set(y + *n);
        pDblYD->IncreaseRef();
        pDblFlag = new types::Double(m_odedcFlag);
        pDblFlag->IncreaseRef();
    }
    else // ode
    {
        pDblY = new types::Double(m_odeYRows, m_odeYCols);
        pDblY->set(y);
        pDblY->IncreaseRef();
    }

    // push_back
    in.push_back(pDblT);
    if (m_odedcYDSize) // odedc
    {
        in.push_back(pDblYC);
        in.push_back(pDblYD);
        in.push_back(pDblFlag);
    }
    else
    {
        in.push_back(pDblY);
    }

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->IncreaseRef();
        in.push_back(m_FArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_FArgs.size(); i++)
        {
            m_FArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    if (m_odedcYDSize) // odedc
    {
        pDblYC->DecreaseRef();
        if (pDblYC->isDeletable())
        {
            delete pDblYC;
        }
        pDblYD->DecreaseRef();
        if (pDblYD->isDeletable())
        {
            delete pDblYD;
        }
        pDblFlag->DecreaseRef();
        if (pDblFlag->isDeletable())
        {
            delete pDblFlag;
        }
    }
    else
    {
        pDblY->DecreaseRef();
        if (pDblY->isDeletable())
        {
            delete pDblY;
        }
    }

    out[0]->DecreaseRef();

    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }
    types::Double* pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->isComplex())
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (m_odedcFlag && m_odedcYDSize)
    {
        C2F(dcopy)(&m_odedcYDSize, pDblOut->get(), &one, ydot, &one);
    }
    else
    {
        C2F(dcopy)(n, pDblOut->get(), &one, ydot, &one);
    }

    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callMacroJac(int* n, double* t, double* y, int* ml, int* mu, double* J, int* nrpd)
{
    char errorMsg[256];
    int iRetCount   = 1;
    int one         = 1;
    int iMaxSize    = (*n) * (*nrpd);

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblY = new types::Double(m_odeYRows, m_odeYCols);
    pDblY->set(y);
    types::Double* pDblT = new types::Double(*t);

    pDblT->IncreaseRef();
    pDblY->IncreaseRef();

    in.push_back(pDblT);
    in.push_back(pDblY);

    for (int i = 0; i < (int)m_JacArgs.size(); i++)
    {
        m_JacArgs[i]->IncreaseRef();
        in.push_back(m_JacArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallJacFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_JacArgs.size(); i++)
        {
            m_JacArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_JacArgs.size(); i++)
    {
        m_JacArgs[i]->DecreaseRef();
    }

    out[0]->IncreaseRef();
    pDblT->DecreaseRef();
    pDblY->DecreaseRef();

    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    if (pDblY->isDeletable())
    {
        delete pDblY;
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }


    types::Double* pDblOut = out[0]->getAs<types::Double>();
    int iSizeOut = pDblOut->getSize();

    if (iSizeOut > iMaxSize)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A size less or equal than %d expected.\n"), pstrName, 1, iMaxSize);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(&iSizeOut, pDblOut->get(), &one, J, &one);
}

void DifferentialEquationFunctions::callMacroG(int* n, double* t, double* y, int* ng, double* gout)
{
    char errorMsg[256];
    int iRetCount   = 1;
    int one         = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblY = new types::Double(m_odeYRows, m_odeYCols);
    pDblY->set(y);
    types::Double* pDblT = new types::Double(*t);

    pDblT->IncreaseRef();
    pDblY->IncreaseRef();

    in.push_back(pDblT);
    in.push_back(pDblY);

    for (int i = 0; i < (int)m_odeGArgs.size(); i++)
    {
        m_odeGArgs[i]->IncreaseRef();
        in.push_back(m_odeGArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallGFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_odeGArgs.size(); i++)
        {
            m_odeGArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_odeGArgs.size(); i++)
    {
        m_odeGArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblT->DecreaseRef();
    pDblY->DecreaseRef();

    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    if (pDblY->isDeletable())
    {
        delete pDblY;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(ng, out[0]->getAs<types::Double>()->get(), &one, gout, &one);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

double DifferentialEquationFunctions::callIntgMacroF(double* t)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    // create input args
    types::Double* pDblT = new types::Double(*t);
    pDblT->IncreaseRef();

    // push_back
    in.push_back(pDblT);

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->IncreaseRef();
        in.push_back(m_FArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_FArgs.size(); i++)
        {
            m_FArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);

    }

    types::Double* pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getSize() != 1)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A Scalar expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    double res = pDblOut->get(0);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }

    return res;
}

double DifferentialEquationFunctions::callInt2dMacroF(double* x, double* y)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    // create input args
    types::Double* pDblX = new types::Double(*x);
    pDblX->IncreaseRef();
    types::Double* pDblY = new types::Double(*y);
    pDblY->IncreaseRef();

    // push_back
    in.push_back(pDblX);
    in.push_back(pDblY);

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->IncreaseRef();
        in.push_back(m_FArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_FArgs.size(); i++)
        {
            m_FArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }

    pDblY->DecreaseRef();
    if (pDblY->isDeletable())
    {
        delete pDblY;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getSize() != 1)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A Scalar expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    double res = pDblOut->get(0);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }

    return res;
}

void DifferentialEquationFunctions::callInt3dMacroF(double* xyz, int* numfun, double* funvls)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    // create input args
    types::Double* pDblXYZ = new types::Double(3, 1);
    pDblXYZ->set(xyz);
    pDblXYZ->IncreaseRef();
    types::Double* pDblNumfun = new types::Double(*numfun);
    pDblNumfun->IncreaseRef();

    // push_back
    in.push_back(pDblXYZ);
    in.push_back(pDblNumfun);

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->IncreaseRef();
        in.push_back(m_FArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_FArgs.size(); i++)
        {
            m_FArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblXYZ->DecreaseRef();
    if (pDblXYZ->isDeletable())
    {
        delete pDblXYZ;
    }

    pDblNumfun->DecreaseRef();
    if (pDblNumfun->isDeletable())
    {
        delete pDblNumfun;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getSize() != *numfun)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: Matrix of size %d expected.\n"), pstrName, 1, *numfun);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(numfun, pDblOut->get(), &one, funvls, &one);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callFevalMacroF(int* nn, double* x1, double* x2, double* xres, int* itype)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblX = NULL;
    types::Double* pDblY = NULL;

    // create input args

    pDblX = new types::Double(*x1);
    pDblX->IncreaseRef();
    in.push_back(pDblX);

    if (*nn == 2)
    {
        pDblY = new types::Double(*x2);
        pDblY->IncreaseRef();
        in.push_back(pDblY);
    }

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->IncreaseRef();
        in.push_back(m_FArgs[i]);
    }
    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_FArgs.size(); i++)
        {
            m_FArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }

    if (*nn == 2)
    {
        pDblY->DecreaseRef();
        if (pDblY->isDeletable())
        {
            delete pDblY;
        }
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);

    }

    types::Double* pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getSize() != 1)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A Scalar expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (pDblOut->isComplex())
    {
        *itype = 1;
        xres[0] = pDblOut->get(0);
        xres[1] = pDblOut->getImg(0);
    }
    else
    {
        *itype = 0;
        xres[0] = pDblOut->get(0);
    }

    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callBvodeMacroGsub(int* i, double* z, double* g)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblI = NULL;
    types::Double* pDblZ = NULL;

    pDblI = new types::Double(*i);
    pDblI->IncreaseRef();
    in.push_back(pDblI);

    pDblZ = new types::Double(m_bvodeM, 1);
    pDblZ->set(z);
    pDblZ->IncreaseRef();
    in.push_back(pDblZ);


    for (int i = 0; i < (int)m_GsubArgs.size(); i++)
    {
        m_GsubArgs[i]->IncreaseRef();
        in.push_back(m_GsubArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallGsubFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_GsubArgs.size(); i++)
        {
            m_GsubArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_GsubArgs.size(); i++)
    {
        m_GsubArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblI->DecreaseRef();
    if (pDblI->isDeletable())
    {
        delete pDblI;
    }

    pDblZ->DecreaseRef();
    if (pDblZ->isDeletable())
    {
        delete pDblZ;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getSize() != 1)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A Scalar expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    *g = pDblOut->get(0);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callBvodeMacroDgsub(int* i, double* z, double* g)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblI = NULL;
    types::Double* pDblZ = NULL;

    pDblI = new types::Double(*i);
    pDblI->IncreaseRef();
    in.push_back(pDblI);

    pDblZ = new types::Double(m_bvodeM, 1);
    pDblZ->set(z);
    pDblZ->IncreaseRef();
    in.push_back(pDblZ);

    for (int i = 0; i < (int)m_DgsubArgs.size(); i++)
    {
        m_DgsubArgs[i]->IncreaseRef();
        in.push_back(m_DgsubArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallDgsubFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_DgsubArgs.size(); i++)
        {
            m_DgsubArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_DgsubArgs.size(); i++)
    {
        m_DgsubArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDgsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblI->DecreaseRef();
    if (pDblI->isDeletable())
    {
        delete pDblI;
    }

    pDblZ->DecreaseRef();
    if (pDblZ->isDeletable())
    {
        delete pDblZ;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDgsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getSize() != m_bvodeM)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDgsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A Matrix of size %d expected.\n"), pstrName, 1, m_bvodeM);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(&m_bvodeM, pDblOut->get(), &one, g, &one);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callBvodeMacroFsub(double* x, double* z, double* d)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblX = NULL;
    types::Double* pDblZ = NULL;

    pDblX = new types::Double(*x);
    pDblX->IncreaseRef();
    in.push_back(pDblX);

    pDblZ = new types::Double(m_bvodeM, 1);
    pDblZ->set(z);
    pDblZ->IncreaseRef();
    in.push_back(pDblZ);

    for (int i = 0; i < (int)m_FsubArgs.size(); i++)
    {
        m_FsubArgs[i]->IncreaseRef();
        in.push_back(m_FsubArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFsubFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_FsubArgs.size(); i++)
        {
            m_FsubArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_FsubArgs.size(); i++)
    {
        m_FsubArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }

    pDblZ->DecreaseRef();
    if (pDblZ->isDeletable())
    {
        delete pDblZ;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOut = out[0]->getAs<types::Double>();
    if (pDblOut->getSize() != m_bvodeN)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d expected.\n"), pstrName, 1, m_bvodeN);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(&m_bvodeN, pDblOut->get(), &one, d, &one);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callBvodeMacroDfsub(double* x, double* z, double* d)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblX = NULL;
    types::Double* pDblZ = NULL;

    pDblX = new types::Double(*x);
    pDblX->IncreaseRef();
    in.push_back(pDblX);

    pDblZ = new types::Double(m_bvodeM, 1);
    pDblZ->set(z);
    pDblZ->IncreaseRef();
    in.push_back(pDblZ);

    for (int i = 0; i < (int)m_DfsubArgs.size(); i++)
    {
        m_DfsubArgs[i]->IncreaseRef();
        in.push_back(m_DfsubArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallDfsubFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_DfsubArgs.size(); i++)
        {
            m_DfsubArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_DfsubArgs.size(); i++)
    {
        m_DfsubArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDfsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }

    pDblZ->DecreaseRef();
    if (pDblZ->isDeletable())
    {
        delete pDblZ;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDfsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOut = out[0]->getAs<types::Double>();
    int size = m_bvodeN * m_bvodeM;
    if (pDblOut->getSize() != size)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallDfsubFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d expected.\n"), pstrName, 1, size);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(&size, pDblOut->get(), &one, d, &one);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callBvodeMacroGuess(double* x, double* z, double* d)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 2;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblX = NULL;

    pDblX = new types::Double(*x);
    pDblX->IncreaseRef();
    in.push_back(pDblX);

    for (int i = 0; i < (int)m_GuessArgs.size(); i++)
    {
        m_GuessArgs[i]->IncreaseRef();
        in.push_back(m_GuessArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallGuessFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_GuessArgs.size(); i++)
        {
            m_GuessArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_GuessArgs.size(); i++)
    {
        m_GuessArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGuessFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();
    out[1]->IncreaseRef();

    pDblX->DecreaseRef();
    if (pDblX->isDeletable())
    {
        delete pDblX;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGuessFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[1]->DecreaseRef();
    if (out[1]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGuessFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 2);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOutZ = out[0]->getAs<types::Double>();
    if (pDblOutZ->getSize() != m_bvodeM)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGuessFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d expected.\n"), pstrName, 1, m_bvodeM);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOutD = out[1]->getAs<types::Double>();
    if (pDblOutD->getSize() != m_bvodeN)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGuessFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d expected.\n"), pstrName, 1, m_bvodeN);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(&m_bvodeM, pDblOutZ->get(), &one, z, &one);
    C2F(dcopy)(&m_bvodeN, pDblOutD->get(), &one, d, &one);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }

    if (out[1]->isDeletable())
    {
        delete out[1];
    }
}

void DifferentialEquationFunctions::callImplMacroF(int* neq, double* t, double* y, double*s, double* r, int* ires)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    *ires = 2;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblT = new types::Double(*t);
    pDblT->IncreaseRef();
    in.push_back(pDblT);

    types::Double* pDblY = new types::Double(*neq, 1);
    pDblY->set(y);
    pDblY->IncreaseRef();
    in.push_back(pDblY);

    types::Double* pDblS = new types::Double(*neq, 1);
    pDblS->set(s);
    pDblS->IncreaseRef();
    in.push_back(pDblS);

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->IncreaseRef();
        in.push_back(m_FArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_FArgs.size(); i++)
        {
            m_FArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    pDblY->DecreaseRef();
    if (pDblY->isDeletable())
    {
        delete pDblY;
    }

    pDblS->DecreaseRef();
    if (pDblS->isDeletable())
    {
        delete pDblS;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOutR = out[0]->getAs<types::Double>();
    if (pDblOutR->getSize() != *neq)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A Matrix of size %d expected.\n"), pstrName, 1, *neq);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(neq, pDblOutR->get(), &one, r, &one);
    *ires = 1;
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callImplMacroG(int* neq, double* t, double* y, double* ml, double* mu, double* p, int* nrowp)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblT = new types::Double(*t);
    pDblT->IncreaseRef();
    in.push_back(pDblT);

    types::Double* pDblY = new types::Double(*neq, 1);
    pDblY->set(y);
    pDblY->IncreaseRef();
    in.push_back(pDblY);

    types::Double* pDblP = new types::Double(*nrowp, *neq);
    pDblP->set(p);
    pDblP->IncreaseRef();
    in.push_back(pDblP);

    for (int i = 0; i < (int)m_odeGArgs.size(); i++)
    {
        m_odeGArgs[i]->IncreaseRef();
        in.push_back(m_odeGArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallGFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_odeGArgs.size(); i++)
        {
            m_odeGArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_odeGArgs.size(); i++)
    {
        m_odeGArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    pDblY->DecreaseRef();
    if (pDblY->isDeletable())
    {
        delete pDblY;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOutP = out[0]->getAs<types::Double>();
    if (pDblOutP->getCols() != *neq || pDblOutP->getRows() != *nrowp)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d x %d expected.\n"), pstrName, 1, *neq, *nrowp);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    int size = *neq **nrowp;
    C2F(dcopy)(&size, pDblOutP->get(), &one, p, &one);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callImplMacroJac(int* neq, double* t, double* y, double* s, double* ml, double* mu, double* p, int* nrowp)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblT = new types::Double(*t);
    pDblT->IncreaseRef();
    in.push_back(pDblT);

    types::Double* pDblY = new types::Double(*neq, 1);
    pDblY->set(y);
    pDblY->IncreaseRef();
    in.push_back(pDblY);

    types::Double* pDblS = new types::Double(*neq, 1);
    pDblS->set(s);
    pDblS->IncreaseRef();
    in.push_back(pDblS);

    for (int i = 0; i < (int)m_JacArgs.size(); i++)
    {
        m_JacArgs[i]->IncreaseRef();
        in.push_back(m_JacArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallJacFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_JacArgs.size(); i++)
        {
            m_JacArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_JacArgs.size(); i++)
    {
        m_JacArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    pDblY->DecreaseRef();
    if (pDblY->isDeletable())
    {
        delete pDblY;
    }

    pDblS->DecreaseRef();
    if (pDblS->isDeletable())
    {
        delete pDblS;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOutP = out[0]->getAs<types::Double>();
    if (pDblOutP->getCols() != *neq || pDblOutP->getRows() != *nrowp)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d x %d expected.\n"), pstrName, 1, *neq, *nrowp);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    int size = *neq **nrowp;
    C2F(dcopy)(&size, pDblOutP->get(), &one, p, &one);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callDasslMacroF(double* t, double* y, double* ydot, double* delta, int* ires, double* rpar, int* ipar)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 2;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblT = new types::Double(*t);
    pDblT->IncreaseRef();
    in.push_back(pDblT);

    types::Double* pDblY = new types::Double(m_odeYRows, 1);
    pDblY->set(y);
    pDblY->IncreaseRef();
    in.push_back(pDblY);

    types::Double* pDblYdot = new types::Double(m_odeYRows, 1);
    pDblYdot->set(ydot);
    pDblYdot->IncreaseRef();
    in.push_back(pDblYdot);

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->IncreaseRef();
        in.push_back(m_FArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallFFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_FArgs.size(); i++)
        {
            m_FArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_FArgs.size(); i++)
    {
        m_FArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();
    out[1]->IncreaseRef();

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    pDblY->DecreaseRef();
    if (pDblY->isDeletable())
    {
        delete pDblY;
    }

    pDblYdot->DecreaseRef();
    if (pDblYdot->isDeletable())
    {
        delete pDblYdot;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[1]->DecreaseRef();
    if (out[1]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 2);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOutDelta = out[0]->getAs<types::Double>();
    if (pDblOutDelta->getSize() != m_odeYRows)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d expected.\n"), pstrName, 1, m_odeYRows);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOutIres = out[1]->getAs<types::Double>();
    if (pDblOutIres->getSize() != 1)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallFFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A Scalar expected.\n"), pstrName, 2);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(&m_odeYRows, pDblOutDelta->get(), &one, delta, &one);
    *ires = (int)pDblOutIres->get(0);

    if (out[0]->isDeletable())
    {
        delete out[0];
    }

    if (out[1]->isDeletable())
    {
        delete out[1];
    }
}

void DifferentialEquationFunctions::callDasslMacroJac(double* t, double* y, double* ydot, double* pd, double* cj, double* rpar, int* ipar)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblT = new types::Double(*t);
    pDblT->IncreaseRef();
    in.push_back(pDblT);

    types::Double* pDblY = new types::Double(m_odeYRows, 1);
    pDblY->set(y);
    pDblY->IncreaseRef();
    in.push_back(pDblY);

    types::Double* pDblYdot = new types::Double(m_odeYRows, 1);
    pDblYdot->set(ydot);
    pDblYdot->IncreaseRef();
    in.push_back(pDblYdot);

    types::Double* pDblCj = new types::Double(*cj);
    pDblCj->IncreaseRef();
    in.push_back(pDblCj);

    for (int i = 0; i < (int)m_JacArgs.size(); i++)
    {
        m_JacArgs[i]->IncreaseRef();
        in.push_back(m_JacArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallJacFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_JacArgs.size(); i++)
        {
            m_JacArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_JacArgs.size(); i++)
    {
        m_JacArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    pDblY->DecreaseRef();
    if (pDblY->isDeletable())
    {
        delete pDblY;
    }

    pDblYdot->DecreaseRef();
    if (pDblYdot->isDeletable())
    {
        delete pDblYdot;
    }

    pDblCj->DecreaseRef();
    if (pDblCj->isDeletable())
    {
        delete pDblCj;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOutPd = out[0]->getAs<types::Double>();
    if ( (pDblOutPd->getCols() != m_odeYRows) ||
            (!m_bandedJac && pDblOutPd->getRows() != m_odeYRows) ||
            (m_bandedJac && pDblOutPd->getRows() != (2 * m_ml + m_mu + 1)))
    {
        char* pstrName = wide_string_to_UTF8(m_pCallJacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d x %d expected.\n"), pstrName, 1, m_odeYRows, (2 * m_ml + m_mu + 1));
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    int size = pDblOutPd->getSize();
    C2F(dcopy)(&size, pDblOutPd->get(), &one, pd, &one);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callDasrtMacroG(int* ny, double* t, double* y, int* ng, double* gout, double* rpar, int* ipar)
{
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 1;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblT = new types::Double(*t);
    pDblT->IncreaseRef();
    in.push_back(pDblT);

    types::Double* pDblY = new types::Double(*ny, 1);
    pDblY->set(y);
    pDblY->IncreaseRef();
    in.push_back(pDblY);

    for (int i = 0; i < (int)m_odeGArgs.size(); i++)
    {
        m_odeGArgs[i]->IncreaseRef();
        in.push_back(m_odeGArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallGFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_odeGArgs.size(); i++)
        {
            m_odeGArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_odeGArgs.size(); i++)
    {
        m_odeGArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    pDblY->DecreaseRef();
    if (pDblY->isDeletable())
    {
        delete pDblY;
    }

    out[0]->DecreaseRef();
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    types::Double* pDblOutGout = out[0]->getAs<types::Double>();
    if (pDblOutGout->getSize() != *ng)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallGFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d expected.\n"), pstrName, 1, *ng);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    C2F(dcopy)(ng, pDblOutGout->get(), &one, gout, &one);
    if (out[0]->isDeletable())
    {
        delete out[0];
    }
}

void DifferentialEquationFunctions::callDaskrMacroPjac(double* res, int* ires, int* neq, double* t, double* y, double* ydot,
        double* rewt, double* savr, double* wk, double* h, double* cj,
        double* wp, int* iwp, int* ier, double* rpar, int* ipar)
{
    // macro : [R, iR, ier] = psol(neq, t, y, ydot, h, cj, rewt, savr)
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 3;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    types::Double* pDblNeq = new types::Double((double)(*neq));
    pDblNeq->IncreaseRef();
    in.push_back(pDblNeq);

    types::Double* pDblT = new types::Double(*t);
    pDblT->IncreaseRef();
    in.push_back(pDblT);

    types::Double* pDblY = new types::Double(m_odeYRows, 1);
    pDblY->set(y);
    pDblY->IncreaseRef();
    in.push_back(pDblY);

    types::Double* pDblYdot = new types::Double(m_odeYRows, 1);
    pDblYdot->set(ydot);
    pDblYdot->IncreaseRef();
    in.push_back(pDblYdot);

    types::Double* pDblH = new types::Double(*h);
    pDblH->IncreaseRef();
    in.push_back(pDblH);

    types::Double* pDblCj = new types::Double(*cj);
    pDblCj->IncreaseRef();
    in.push_back(pDblCj);

    types::Double* pDblRewt = new types::Double(m_odeYRows, 1);
    pDblRewt->set(rewt);
    pDblRewt->IncreaseRef();
    in.push_back(pDblRewt);

    types::Double* pDblSavr = new types::Double(m_odeYRows, 1);
    pDblSavr->set(savr);
    pDblSavr->IncreaseRef();
    in.push_back(pDblSavr);

    for (int i = 0; i < (int)m_pJacArgs.size(); i++)
    {
        m_pJacArgs[i]->IncreaseRef();
        in.push_back(m_pJacArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallPjacFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_pJacArgs.size(); i++)
        {
            m_pJacArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_pJacArgs.size(); i++)
    {
        m_pJacArgs[i]->DecreaseRef();
    }

    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPjacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();
    out[1]->IncreaseRef();
    out[2]->IncreaseRef();

    pDblNeq->DecreaseRef();
    if (pDblNeq->isDeletable())
    {
        delete pDblNeq;
    }

    pDblT->DecreaseRef();
    if (pDblT->isDeletable())
    {
        delete pDblT;
    }

    pDblY->DecreaseRef();
    if (pDblY->isDeletable())
    {
        delete pDblY;
    }

    pDblYdot->DecreaseRef();
    if (pDblYdot->isDeletable())
    {
        delete pDblYdot;
    }

    pDblH->DecreaseRef();
    if (pDblH->isDeletable())
    {
        delete pDblH;
    }

    pDblCj->DecreaseRef();
    if (pDblCj->isDeletable())
    {
        delete pDblCj;
    }

    pDblRewt->DecreaseRef();
    if (pDblRewt->isDeletable())
    {
        delete pDblRewt;
    }

    pDblSavr->DecreaseRef();
    if (pDblSavr->isDeletable())
    {
        delete pDblSavr;
    }

    // check type of output arguments
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPjacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (out[1]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPjacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 2);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (out[2]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPjacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 3);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    //  return [R, iR, ier]
    types::Double* pDblOutWp  = out[0]->getAs<types::Double>();
    types::Double* pDblOutIwp = out[1]->getAs<types::Double>();
    types::Double* pDblOutIer = out[2]->getAs<types::Double>();

    // check size of output argument
    if (pDblOutWp->getSize() != *neq **neq)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPjacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d expected.\n"), pstrName, 1, *neq **neq);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (pDblOutIwp->getSize() != 2 * *neq **neq)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPjacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d expected.\n"), pstrName, 2, 2 * *neq **neq);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (pDblOutIer->isScalar() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPjacFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A Scalar expected.\n"), pstrName, 3);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    // copy output macro results in output variables
    int size = pDblOutWp->getSize();
    C2F(dcopy)(&size, pDblOutWp->get(), &one, wp, &one);

    double* pdblIwp = pDblOutIwp->get();
    for (int i = 0; i < pDblOutIwp->getSize(); i++)
    {
        iwp[i] = (int)pdblIwp[i];
    }

    *ier = (int)(pDblOutIer->get(0));

    // delete output macro result
    out[0]->DecreaseRef();
    if (out[0]->isDeletable())
    {
        delete out[0];
    }

    out[1]->DecreaseRef();
    if (out[1]->isDeletable())
    {
        delete out[1];
    }

    out[2]->DecreaseRef();
    if (out[2]->isDeletable())
    {
        delete out[2];
    }
}

void DifferentialEquationFunctions::callDaskrMacroPsol(int* neq, double* t, double* y, double* ydot, double* savr, double* wk,
        double* cj, double* wght, double* wp, int* iwp, double* b, double* eplin,
        int* ier, double* rpar, int* ipar)
{
    // macro : [b, ier] = psol(R, iR, b)
    char errorMsg[256];
    int one         = 1;
    int iRetCount   = 2;

    types::typed_list in;
    types::typed_list out;
    types::optional_list opt;

    // input arguments psol(R, iR, b)
    types::Double* pDblR = new types::Double(*neq **neq, 1);
    pDblR->set(wp);
    pDblR->IncreaseRef();
    in.push_back(pDblR);

    types::Double* pDblIR = new types::Double(*neq **neq, 2);
    double* pdblIR = pDblIR->get();
    for (int i = 0; i < pDblIR->getSize(); i++)
    {
        pdblIR[i] = (double)iwp[i];
    }
    pDblIR->IncreaseRef();
    in.push_back(pDblIR);

    types::Double* pDblB = new types::Double(*neq, 1);
    pDblB->set(b);
    pDblB->IncreaseRef();
    in.push_back(pDblB);

    // optional arguments
    for (int i = 0; i < (int)m_pSolArgs.size(); i++)
    {
        m_pSolArgs[i]->IncreaseRef();
        in.push_back(m_pSolArgs[i]);
    }

    try
    {
        // new std::wstring(L"") is delete in destructor of ast::CommentExp
        m_pCallPsolFunction->invoke(in, opt, iRetCount, out, ast::CommentExp(Location(), new std::wstring(L"")));
    }
    catch (const ast::InternalError& ie)
    {
        for (int i = 0; i < (int)m_pSolArgs.size(); i++)
        {
            m_pSolArgs[i]->DecreaseRef();
        }

        throw ie;
    }

    for (int i = 0; i < (int)m_pSolArgs.size(); i++)
    {
        m_pSolArgs[i]->DecreaseRef();
    }

    // get output
    if (out.size() != iRetCount)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPsolFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong number of output argument(s): %d expected.\n"), pstrName, iRetCount);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    out[0]->IncreaseRef();
    out[1]->IncreaseRef();

    // free input arguments
    pDblR->DecreaseRef();
    if (pDblR->isDeletable())
    {
        delete pDblR;
    }

    pDblIR->DecreaseRef();
    if (pDblIR->isDeletable())
    {
        delete pDblIR;
    }

    pDblB->DecreaseRef();
    if (pDblB->isDeletable())
    {
        delete pDblB;
    }

    // check output result
    if (out[0]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPsolFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 1);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    if (out[1]->isDouble() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPsolFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong type for output argument #%d: Real matrix expected.\n"), pstrName, 2);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    // return arguments [b, ier] = psol()
    types::Double* pDblOutB  = out[0]->getAs<types::Double>();
    if (pDblOutB->getSize() != *neq) // size of b is neq
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPsolFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A matrix of size %d expected.\n"), pstrName, 1, *neq);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    // get scalar ier
    types::Double* pDblOutIer = out[1]->getAs<types::Double>();
    if (pDblOutIer->isScalar() == false)
    {
        char* pstrName = wide_string_to_UTF8(m_pCallPsolFunction->getName().c_str());
        sprintf(errorMsg, _("%s: Wrong size for output argument #%d: A Scalar expected.\n"), pstrName, 2);
        FREE(pstrName);
        throw ast::InternalError(errorMsg);
    }

    // copy output macro results in output variables
    C2F(dcopy)(neq, pDblOutB->get(), &one, b, &one);
    *ier = (int)(pDblOutIer->get(0));

    // free output arguments
    out[0]->DecreaseRef();
    if (out[0]->isDeletable())
    {
        delete out[0];
    }

    out[1]->DecreaseRef();
    if (out[1]->isDeletable())
    {
        delete out[1];
    }
}
