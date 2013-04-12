/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "execvisitor.hxx"
#include "string.hxx"
#include "double.hxx"
#include "differentialequationfunctions.hxx"

extern "C"
{
#include "elem_common.h"
#include "scifunctions.h"
#include "Ex-odedc.h"
}

/*
** differential equation functions
** \{
*/

// need the current thread, not the last running thread.

std::map<__threadId, DifferentialEquationFunctions*> DifferentialEquation::m_mapDifferentialEquationFunctions;

void DifferentialEquation::addDifferentialEquationFunctions(DifferentialEquationFunctions* _deFunction)
{
    types::ThreadId* pThread = ConfigVariable::getLastRunningThread();
    m_mapDifferentialEquationFunctions[pThread->getId()] = _deFunction;
}

void DifferentialEquation::removeDifferentialEquationFunctions()
{
    types::ThreadId* pThread = ConfigVariable::getLastRunningThread();
    m_mapDifferentialEquationFunctions.erase(pThread->getId());
}

DifferentialEquationFunctions* DifferentialEquation::getDifferentialEquationFunctions()
{
    types::ThreadId* pThread = ConfigVariable::getLastRunningThread();
    return m_mapDifferentialEquationFunctions[pThread->getId()];
}


/*
** \}
*/



/*--------------------------------------------------------------------------*/
DifferentialEquationFunctions::DifferentialEquationFunctions(std::wstring callerName)
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

    // function extern
    m_pStringFFunctionDyn   = NULL;
    m_pStringJacFunctionDyn = NULL;
    m_pStringGFunctionDyn   = NULL;

    // function static
    m_pStringFFunctionStatic   = NULL;
    m_pStringJacFunctionStatic = NULL;
    m_pStringGFunctionStatic   = NULL;

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
        m_staticFunctionMap[L"fex"]     = (void*) C2F(fex);
        m_staticFunctionMap[L"fex2"]    = (void*) C2F(fex2);
        m_staticFunctionMap[L"fex3"]    = (void*) C2F(fex3);
        m_staticFunctionMap[L"fexab"]   = (void*) C2F(fexab);
        m_staticFunctionMap[L"loren"]   = (void*) C2F(loren);
        m_staticFunctionMap[L"bcomp"]   = (void*) C2F(bcomp);
        m_staticFunctionMap[L"lcomp"]   = (void*) C2F(lcomp);

        m_staticFunctionMap[L"jex"]     = (void*) C2F(jex);
    }
    else if (callerName == L"odedc")
    {
        m_staticFunctionMap[L"fcd"]     = (void*) fcd;
        m_staticFunctionMap[L"fcd1"]    = (void*) fcd1;
        m_staticFunctionMap[L"fexcd"]   = (void*) fexcd;
        m_staticFunctionMap[L"phis"]    = (void*) phis;
        m_staticFunctionMap[L"phit"]    = (void*) phit;

        m_staticFunctionMap[L"jex"]     = (void*) C2F(jex);
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
    else if (callerName == L"dassl" || callerName == L"dasrt")
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
        if (callerName == L"dasrt")
        {
            m_staticFunctionMap[L"gr1"]  = (void*) C2F(gr1);
            m_staticFunctionMap[L"gr2"]  = (void*) C2F(gr2);
        }
    }
}

DifferentialEquationFunctions::~DifferentialEquationFunctions()
{
    m_staticFunctionMap.clear();
}

/*------------------------------- public -------------------------------------------*/
int DifferentialEquationFunctions::execDasrtG(int* ny, double* t, double* y, int* ng, double* gout, double* rpar, int* ipar)
{
    if (m_pCallGFunction)
    {
        return callDasrtMacroG(ny, t, y, ng, gout, rpar, ipar);
    }
    else if (m_pStringGFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringGFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((dasrt_g_t)(func->functionPtr))(ny, t, y, ng, gout, rpar, ipar);
        return 1;
    }
    else
    {
        ((dasrt_g_t)m_staticFunctionMap[m_pStringGFunctionStatic->get(0)])(ny, t, y, ng, gout, rpar, ipar);
        return 1;
    }
    return 0;
}

int DifferentialEquationFunctions::execDasslF(double* t, double* y, double* ydot, double* delta, int* ires, double* rpar, int* ipar)
{
    if (m_pCallFFunction)
    {
        return callDasslMacroF(t, y, ydot, delta, ires, rpar, ipar);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((dassl_f_t)(func->functionPtr))(t, y, ydot, delta, ires, rpar, ipar);
        return 1;
    }
    else
    {
        ((dassl_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(t, y, ydot, delta, ires, rpar, ipar);
        return 1;
    }
    return 0;
}

int DifferentialEquationFunctions::execDasslJac(double* t, double* y, double* ydot, double* pd, double* cj, double* rpar, int* ipar)
{
    if (m_pCallJacFunction)
    {
        return callDasslMacroJac(t, y, ydot, pd, cj, rpar, ipar);
    }
    else if (m_pStringJacFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringJacFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((dassl_jac_t)(func->functionPtr))(t, y, ydot, pd, cj, rpar, ipar);
        return 1;
    }
    else if (m_pStringJacFunctionStatic)
    {
        ((dassl_jac_t)m_staticFunctionMap[m_pStringJacFunctionStatic->get(0)])(t, y, ydot, pd, cj, rpar, ipar);
        return 1;
    }
    return 1;
}

int DifferentialEquationFunctions::execImplF(int* neq, double* t, double* y, double* s, double* r, int* ires)
{
    if (m_pCallFFunction)
    {
        return callImplMacroF(neq, t, y, s, r, ires);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((impl_f_t)(func->functionPtr))(neq, t, y, s, r, ires);
        return 1;
    }
    else
    {
        ((impl_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(neq, t, y, s, r, ires);
        return 1;
    }
    return 0;
}

int DifferentialEquationFunctions::execImplG(int* neq, double* t, double* y, double* ml, double* mu, double* p, int* nrowp)
{
    if (m_pCallGFunction)
    {
        return callImplMacroG(neq, t, y, ml, mu, p, nrowp);
    }
    else if (m_pStringGFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringGFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((impl_g_t)(func->functionPtr))(neq, t, y, ml, mu, p, nrowp);
        return 1;
    }
    else
    {
        ((impl_g_t)m_staticFunctionMap[m_pStringGFunctionStatic->get(0)])(neq, t, y, ml, mu, p, nrowp);
        return 1;
    }
    return 0;
}

int DifferentialEquationFunctions::execImplJac(int* neq, double* t, double* y, double* s, double* ml, double* mu, double* p, int* nrowp)
{
    if (m_pCallJacFunction)
    {
        return callImplMacroJac(neq, t, y, s, ml, mu, p, nrowp);
    }
    else if (m_pStringJacFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringJacFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((impl_jac_t)(func->functionPtr))(neq, t, y, s, ml, mu, p, nrowp);
        return 1;
    }
    else if (m_pStringJacFunctionStatic)
    {
        ((impl_jac_t)m_staticFunctionMap[m_pStringJacFunctionStatic->get(0)])(neq, t, y, s, ml, mu, p, nrowp);
        return 1;
    }
    return 1;
}

int DifferentialEquationFunctions::execBvodeGuess(double *x, double *z, double *d)
{
    if (m_pCallGuessFunction)
    {
        return callBvodeMacroGuess(x, z, d);
    }
    else if (m_pStringGuessFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringGuessFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((bvode_ddd_t)(func->functionPtr))(x, z, d);
        return 1;
    }
    else if (m_pStringGuessFunctionStatic)
    {
        ((bvode_ddd_t)m_staticFunctionMap[m_pStringGuessFunctionStatic->get(0)])(x, z, d);
        return 1;
    }
    return 1;
}

int DifferentialEquationFunctions::execBvodeDfsub(double *x, double *z, double *d)
{
    if (m_pCallDfsubFunction)
    {
        return callBvodeMacroDfsub(x, z, d);
    }
    else if (m_pStringDfsubFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringDfsubFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((bvode_ddd_t)(func->functionPtr))(x, z, d);
        return 1;
    }
    else // function static
    {
        ((bvode_ddd_t)m_staticFunctionMap[m_pStringDfsubFunctionStatic->get(0)])(x, z, d);
        return 1;
    }
}

int DifferentialEquationFunctions::execBvodeFsub(double *x, double *z, double *d)
{
    if (m_pCallFsubFunction)
    {
        return callBvodeMacroFsub(x, z, d);
    }
    else if (m_pStringFsubFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFsubFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((bvode_ddd_t)(func->functionPtr))(x, z, d);
        return 1;
    }
    else // function static
    {
        ((bvode_ddd_t)m_staticFunctionMap[m_pStringFsubFunctionStatic->get(0)])(x, z, d);
        return 1;
    }
}

int DifferentialEquationFunctions::execBvodeDgsub(int *i, double *z, double *g)
{
    if (m_pCallDgsubFunction)
    {
        return callBvodeMacroDgsub(i, z, g);
    }
    else if (m_pStringDgsubFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringDgsubFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((bvode_idd_t)(func->functionPtr))(i, z, g);
        return 1;
    }
    else // function static
    {
        ((bvode_idd_t)m_staticFunctionMap[m_pStringDgsubFunctionStatic->get(0)])(i, z, g);
        return 1;
    }
}

int DifferentialEquationFunctions::execBvodeGsub(int *i, double *z, double *g)
{
    if (m_pCallGsubFunction)
    {
        return callBvodeMacroGsub(i, z, g);
    }
    else if (m_pStringGsubFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringGsubFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        ((bvode_idd_t)(func->functionPtr))(i, z, g);
        return 1;
    }
    else // function static
    {
        ((bvode_idd_t)m_staticFunctionMap[m_pStringGsubFunctionStatic->get(0)])(i, z, g);
        return 1;
    }
}

int DifferentialEquationFunctions::execFevalF(int *nn, double *x1, double *x2, double *xres, int *itype)
{
    if (m_pCallFFunction)
    {
        return callFevalMacroF(nn, x1, x2, xres, itype);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            return 1;
        }
        return ((feval_f_t)(func->functionPtr))(nn, x1, x2, xres, itype);
    }
    else // function static
    {
        return ((feval_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(nn, x1, x2, xres, itype);
    }
}

double DifferentialEquationFunctions::execInt3dF(double* x, int* numfun, double* funvls)
{
    if (m_pCallFFunction)
    {
        return callInt3dMacroF(x, numfun, funvls);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        return ((int3d_f_t)(func->functionPtr))(x, numfun, funvls);
    }
    else // function static
    {
        return ((int3d_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(x, numfun, funvls);
    }
}

double DifferentialEquationFunctions::execInt2dF(double* x, double* y)
{
    if (m_pCallFFunction)
    {
        return callInt2dMacroF(x, y);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        return ((int2d_f_t)(func->functionPtr))(x, y);
    }
    else // function static
    {
        return ((int2d_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(x, y);
    }
}

double DifferentialEquationFunctions::execIntgF(double* x)
{
    if (m_pCallFFunction)
    {
        return callIntgMacroF(x);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        return ((intg_f_t)(func->functionPtr))(x);
    }
    else // function static
    {
        return ((intg_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(x);
    }
}

int DifferentialEquationFunctions::execOdeF(int* n, double* t, double* y, double* yout)
{
    if (m_pCallFFunction)
    {
        return callOdeMacroF(n, t, y, yout);
    }
    else if (m_pStringFFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringFFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        return ((ode_f_t)(func->functionPtr))(n, t, y, yout);
    }
    else // function static
    {
        if (m_wstrCaller == L"ode")
        {
            ((ode_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(n, t, y, yout);
            return 1;
        }
        else if (m_wstrCaller == L"odedc")
        {
            ((odedc_f_t)m_staticFunctionMap[m_pStringFFunctionStatic->get(0)])(&m_odedcFlag, n, &m_odedcYDSize, t, y, yout);
            return 1;
        }
        return 0;
    }
}

int DifferentialEquationFunctions::execFunctionJac(int *n, double *t, double *y, int *ml, int *mu, double *J, int *nrpd)
{
    if (m_pCallJacFunction)
    {
        return callMacroJac(n, t, y, ml, mu, J, nrpd);
    }
    else if (m_pStringJacFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringJacFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        return ((func_jac_t)(func->functionPtr))(n, t, y, ml, mu, J, nrpd);
    }
    else // function static
    {
        ((func_jac_t)m_staticFunctionMap[m_pStringJacFunctionStatic->get(0)])(n, t, y, ml, mu, J, nrpd);
        return 1;
    }
}

int DifferentialEquationFunctions::execFunctionG(int* n, double* t, double* y, int* ng, double* gout)
{
    if (m_pCallGFunction)
    {
        return callMacroG(n, t, y, ng, gout);
    }
    else if (m_pStringGFunctionDyn)
    {
        ConfigVariable::EntryPointStr* func = ConfigVariable::getEntryPoint(m_pStringGFunctionDyn->get(0));
        if (func == NULL)
        {
            return 0;
        }
        return ((func_g_t)(func->functionPtr))(n, t, y, ng, gout);
    }
    else // function static
    {
        ((func_g_t)m_staticFunctionMap[m_pStringGFunctionStatic->get(0)])(n, t, y, ng, gout);
        return 1;
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

//set function f, jac, g as types::Callable
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

//set function f, jac, g as types::String
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

// set args for f, jac and g functions
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
int DifferentialEquationFunctions::callOdeMacroF(int* n, double* t, double* y, double* ydot)
{
    if (m_pCallFFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

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

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallFFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
                delete pDblY;
            }
            pDblYD->DecreaseRef();
            if (pDblYD->isDeletable())
            {
                delete pDblY;
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
        if (out[0]->isDouble())
        {
            if (m_odedcFlag && m_odedcYDSize)
            {
                C2F(dcopy)(&m_odedcYDSize, out[0]->getAs<types::Double>()->get(), &one, ydot, &one);
            }
            else
            {
                C2F(dcopy)(n, out[0]->getAs<types::Double>()->get(), &one, ydot, &one);
            }

            if (out[0]->isDeletable())
            {
                delete out[0];
            }
            return 1;
        }
    }
    return 0;
}

int DifferentialEquationFunctions::callMacroJac(int* n, double* t, double* y, int* ml, int* mu, double* J, int* nrpd)
{
    if (m_pCallJacFunction)
    {
        int iRetCount   = 1;
        int one         = 1;
        int size        = (*n) * (*nrpd);

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        types::Double* pDblY = new types::Double(m_odeYRows, m_odeYCols);
        pDblY->set(y);
        types::Double* pDblT = new types::Double(*t);

        pDblT->IncreaseRef();
        pDblY->IncreaseRef();

        in.push_back(pDblT);
        in.push_back(pDblY);

        for (int i = 0; i < m_JacArgs.size(); i++)
        {
            in.push_back(m_JacArgs[i]);
        }

        bool bOk = m_pCallJacFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
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

        if (out.size() != 1)
        {
            return 0;
        }

        out[0]->DecreaseRef();
        if (out[0]->isDouble())
        {
            // dimension y(*), pd(nrowpd,*)
            C2F(dcopy)(&size, out[0]->getAs<types::Double>()->get(), &one, J, &one);
            return 1;
        }
    }
    return 0;
}

int DifferentialEquationFunctions::callMacroG(int* n, double* t, double* y, int* ng, double* gout)
{
    if (m_pCallGFunction)
    {
        int iRetCount   = 1;
        int one         = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        types::Double* pDblY = new types::Double(m_odeYRows, m_odeYCols);
        pDblY->set(y);
        types::Double* pDblT = new types::Double(*t);

        pDblT->IncreaseRef();
        pDblY->IncreaseRef();

        in.push_back(pDblT);
        in.push_back(pDblY);

        for (int i = 0; i < m_odeGArgs.size(); i++)
        {
            in.push_back(m_odeGArgs[i]);
        }

        bool bOk = m_pCallGFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
        if (out[0]->isDouble())
        {
            C2F(dcopy)(ng, out[0]->getAs<types::Double>()->get(), &one, gout, &one);
            if (out[0]->isDeletable())
            {
                delete out[0];
            }
            return 1;
        }
    }
    return 0;
}

double DifferentialEquationFunctions::callIntgMacroF(double* t)
{
    if (m_pCallFFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        // create input args
        types::Double* pDblT = new types::Double(*t);
        pDblT->IncreaseRef();

        // push_back
        in.push_back(pDblT);

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallFFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
        }

        out[0]->IncreaseRef();

        pDblT->DecreaseRef();
        if (pDblT->isDeletable())
        {
            delete pDblT;
        }

        out[0]->DecreaseRef();
        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            if (pDblOut->getSize() != 1)
            {
                return 0;
            }
            double res = pDblOut->get(0);
            if (out[0]->isDeletable())
            {
                delete out[0];
            }
            return res;
        }
    }
    return 0;
}

double DifferentialEquationFunctions::callInt2dMacroF(double* x, double* y)
{
    if (m_pCallFFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        // create input args
        types::Double* pDblX = new types::Double(*x);
        pDblX->IncreaseRef();
        types::Double* pDblY = new types::Double(*y);
        pDblY->IncreaseRef();

        // push_back
        in.push_back(pDblX);
        in.push_back(pDblY);

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallFFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            if (pDblOut->getSize() != 1)
            {
                return 0;
            }
            double res = pDblOut->get(0);
            if (out[0]->isDeletable())
            {
                delete out[0];
            }
            return res;
        }
    }
    return 0;
}

double DifferentialEquationFunctions::callInt3dMacroF(double* xyz, int* numfun, double* funvls)
{
    if (m_pCallFFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        // create input args
        types::Double* pDblXYZ = new types::Double(3, 1);
        pDblXYZ->set(xyz);
        pDblXYZ->IncreaseRef();
        types::Double* pDblNumfun = new types::Double(*numfun);
        pDblNumfun->IncreaseRef();

        // push_back
        in.push_back(pDblXYZ);
        in.push_back(pDblNumfun);

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallFFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            if (pDblOut->getSize() != *numfun)
            {
                return 0;
            }
            C2F(dcopy)(numfun, pDblOut->get(), &one, funvls, &one);
            if (out[0]->isDeletable())
            {
                delete out[0];
            }
            return 1;
        }
    }
    return 0;
}

int DifferentialEquationFunctions::callFevalMacroF(int* nn, double* x1, double* x2, double* xres, int* itype)
{
    if (m_pCallFFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

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

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallFFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 1;
        }

        if (out.size() != 1)
        {
            return 1;
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
        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            if (pDblOut->getSize() != 1)
            {
                return 1;
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
            return 0;
        }
    }
    return 1;
}

int DifferentialEquationFunctions::callBvodeMacroGsub(int* i, double* z, double* g)
{
    if (m_pCallGsubFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        types::Double* pDblI = NULL;
        types::Double* pDblZ = NULL;

        pDblI = new types::Double(*i);
        pDblI->IncreaseRef();
        in.push_back(pDblI);

        pDblZ = new types::Double(m_bvodeM, 1);
        pDblZ->set(z);
        pDblZ->IncreaseRef();
        in.push_back(pDblZ);


        for (int i = 0; i < m_GsubArgs.size(); i++)
        {
            in.push_back(m_GsubArgs[i]);
        }

        bool bOk = m_pCallGsubFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            if (pDblOut->getSize() != 1)
            {
                return 0;
            }

            *g = pDblOut->get(0);
            if (out[0]->isDeletable())
            {
                delete out[0];
            }
            return 1;
        }
    }
    return 0;
}

int DifferentialEquationFunctions::callBvodeMacroDgsub(int* i, double* z, double* g)
{
    if (m_pCallDgsubFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        types::Double* pDblI = NULL;
        types::Double* pDblZ = NULL;

        pDblI = new types::Double(*i);
        pDblI->IncreaseRef();
        in.push_back(pDblI);

        pDblZ = new types::Double(m_bvodeM, 1);
        pDblZ->set(z);
        pDblZ->IncreaseRef();
        in.push_back(pDblZ);

        for (int i = 0; i < m_DgsubArgs.size(); i++)
        {
            in.push_back(m_DgsubArgs[i]);
        }

        bool bOk = m_pCallDgsubFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            if (pDblOut->getSize() != m_bvodeM)
            {
                return 0;
            }

            C2F(dcopy)(&m_bvodeM, pDblOut->get(), &one, g, &one);
            if (out[0]->isDeletable())
            {
                delete out[0];
            }
            return 1;
        }
    }
    return 0;
}

int DifferentialEquationFunctions::callBvodeMacroFsub(double* x, double* z, double* d)
{
    if (m_pCallFsubFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        types::Double* pDblX = NULL;
        types::Double* pDblZ = NULL;

        pDblX = new types::Double(*x);
        pDblX->IncreaseRef();
        in.push_back(pDblX);

        pDblZ = new types::Double(m_bvodeM, 1);
        pDblZ->set(z);
        pDblZ->IncreaseRef();
        in.push_back(pDblZ);


        for (int i = 0; i < m_FsubArgs.size(); i++)
        {
            in.push_back(m_FsubArgs[i]);
        }

        bool bOk = m_pCallFsubFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            if (pDblOut->getSize() != m_bvodeN)
            {
                return 0;
            }

            C2F(dcopy)(&m_bvodeN, pDblOut->get(), &one, d, &one);
            if (out[0]->isDeletable())
            {
                delete out[0];
            }
            return 1;
        }
    }
    return 0;
}

int DifferentialEquationFunctions::callBvodeMacroDfsub(double* x, double* z, double* d)
{
    if (m_pCallDfsubFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        types::Double* pDblX = NULL;
        types::Double* pDblZ = NULL;

        pDblX = new types::Double(*x);
        pDblX->IncreaseRef();
        in.push_back(pDblX);

        pDblZ = new types::Double(m_bvodeM, 1);
        pDblZ->set(z);
        pDblZ->IncreaseRef();
        in.push_back(pDblZ);


        for (int i = 0; i < m_DfsubArgs.size(); i++)
        {
            in.push_back(m_DfsubArgs[i]);
        }

        bool bOk = m_pCallDfsubFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
        if (out[0]->isDouble())
        {
            types::Double* pDblOut = out[0]->getAs<types::Double>();
            int size = m_bvodeN * m_bvodeM;
            if (pDblOut->getSize() != size)
            {
                return 0;
            }

            C2F(dcopy)(&size, pDblOut->get(), &one, d, &one);
            if (out[0]->isDeletable())
            {
                delete out[0];
            }
            return 1;
        }
    }
    return 0;
}

int DifferentialEquationFunctions::callBvodeMacroGuess(double* x, double* z, double* d)
{
    if (m_pCallGuessFunction)
    {
        int one         = 1;
        int iRetCount   = 2;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        types::Double* pDblX = NULL;

        pDblX = new types::Double(*x);
        pDblX->IncreaseRef();
        in.push_back(pDblX);

        for (int i = 0; i < m_GuessArgs.size(); i++)
        {
            in.push_back(m_GuessArgs[i]);
        }

        bool bOk = m_pCallGuessFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 2)
        {
            return 0;
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
            return 0;
        }

        out[1]->DecreaseRef();
        if (out[1]->isDouble() == false)
        {
            return 0;
        }

        types::Double* pDblOutZ = out[0]->getAs<types::Double>();
        if (pDblOutZ->getSize() != m_bvodeM)
        {
            return 0;
        }

        types::Double* pDblOutD = out[1]->getAs<types::Double>();
        if (pDblOutD->getSize() != m_bvodeN)
        {
            return 0;
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
        return 1;
    }
    return 0;
}

int DifferentialEquationFunctions::callImplMacroF(int* neq, double* t, double* y, double*s, double* r, int* ires)
{
    *ires = 2;
    if (m_pCallFFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

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

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallFFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
            return 0;
        }

        types::Double* pDblOutR = out[0]->getAs<types::Double>();
        if (pDblOutR->getSize() != *neq)
        {
            return 0;
        }

        C2F(dcopy)(neq, pDblOutR->get(), &one, r, &one);
        *ires = 1;
        if (out[0]->isDeletable())
        {
            delete out[0];
        }
        return 1;
    }
    return 0;
}

int DifferentialEquationFunctions::callImplMacroG(int* neq, double* t, double* y, double* ml, double* mu, double* p, int* nrowp)
{
    if (m_pCallGFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

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

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallGFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
            return 0;
        }

        types::Double* pDblOutP = out[0]->getAs<types::Double>();
        if (pDblOutP->getCols() != *neq || pDblOutP->getRows() != *nrowp)
        {
            return 0;
        }

        int size = *neq * *nrowp;
        C2F(dcopy)(&size, pDblOutP->get(), &one, p, &one);
        if (out[0]->isDeletable())
        {
            delete out[0];
        }
        return 1;
    }
    return 0;
}

int DifferentialEquationFunctions::callImplMacroJac(int* neq, double* t, double* y, double* s, double* ml, double* mu, double* p, int* nrowp)
{
    if (m_pCallJacFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

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

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallJacFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
            return 0;
        }

        types::Double* pDblOutP = out[0]->getAs<types::Double>();
        if (pDblOutP->getCols() != *neq || pDblOutP->getRows() != *nrowp)
        {
            return 0;
        }

        int size = *neq * *nrowp;
        C2F(dcopy)(&size, pDblOutP->get(), &one, p, &one);
        if (out[0]->isDeletable())
        {
            delete out[0];
        }
        return 1;
    }
    return 0;
}

int DifferentialEquationFunctions::callDasslMacroF(double* t, double* y, double* ydot, double* delta, int* ires, double* rpar, int* ipar)
{
    if (m_pCallFFunction)
    {
        int one         = 1;
        int iRetCount   = 2;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

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

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallFFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 2)
        {
            return 0;
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
            return 0;
        }

        out[1]->DecreaseRef();
        if (out[1]->isDouble() == false)
        {
            return 0;
        }

        types::Double* pDblOutDelta = out[0]->getAs<types::Double>();
        if (pDblOutDelta->getSize() != m_odeYRows)
        {
            return 0;
        }

        types::Double* pDblOutIres = out[1]->getAs<types::Double>();
        if (pDblOutIres->getSize() != 1)
        {
            return 0;
        }

        C2F(dcopy)(&m_odeYRows, pDblOutDelta->get(), &one, delta, &one);
        *ires = (int)pDblOutIres->get(0);
        if (out[0]->isDeletable())
        {
            delete out[0];
        }
        return 1;
    }
    return 0;
}

int DifferentialEquationFunctions::callDasslMacroJac(double* t, double* y, double* ydot, double* pd, double* cj, double* rpar, int* ipar)
{
    if (m_pCallJacFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

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

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallJacFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
            return 0;
        }

        types::Double* pDblOutPd = out[0]->getAs<types::Double>();
        if ( (pDblOutPd->getCols() != m_odeYRows) ||
                (!m_bandedJac && pDblOutPd->getRows() != m_odeYRows) ||
                (m_bandedJac && pDblOutPd->getRows() != (2 * m_ml + m_mu + 1)))
        {
            return 0;
        }

        int size = pDblOutPd->getSize();
        C2F(dcopy)(&size, pDblOutPd->get(), &one, pd, &one);
        if (out[0]->isDeletable())
        {
            delete out[0];
        }
        return 1;
    }
    return 0;
}

int DifferentialEquationFunctions::callDasrtMacroG(int* ny, double* t, double* y, int* ng, double* gout, double* rpar, int* ipar)
{
    if (m_pCallGFunction)
    {
        int one         = 1;
        int iRetCount   = 1;

        typed_list in;
        typed_list out;
        types::optional_list opt;
        ast::ExecVisitor execFunc;

        types::Double* pDblT = new types::Double(*t);
        pDblT->IncreaseRef();
        in.push_back(pDblT);

        types::Double* pDblY = new types::Double(*ny, 1);
        pDblY->set(y);
        pDblY->IncreaseRef();
        in.push_back(pDblY);

        for (int i = 0; i < m_FArgs.size(); i++)
        {
            in.push_back(m_FArgs[i]);
        }

        bool bOk = m_pCallGFunction->call(in, opt, iRetCount, out, &execFunc) == types::Function::OK;

        if (bOk == false)
        {
            return 0;
        }

        if (out.size() != 1)
        {
            return 0;
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
            return 0;
        }

        types::Double* pDblOutGout = out[0]->getAs<types::Double>();
        if (pDblOutGout->getSize() != *ng)
        {
            return 0;
        }

        C2F(dcopy)(ng, pDblOutGout->get(), &one, gout, &one);
        if (out[0]->isDeletable())
        {
            delete out[0];
        }
        return 1;
    }
    return 0;
}
