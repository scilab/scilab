/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - DIGITEO - Cedric DELAMARRE
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

#ifndef __OPTIMIZATIONS_HXX__
#define __OPTIMIZATIONS_HXX__

#include <map>
#include "dynlib_optimization.h"
#include "string.hxx"
#include "callable.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
}

extern "C"
{
    // optim
    void C2F(genros)(int* ind, int* n, double* x, double* f, double* g, int* ti, float* tr, double* td);
    void C2F(topt2) (int* ind, int* n, double* x, double* f, double* g, int* ti, float* tr, double* td);
    void C2F(icsemc)(int* ind, int* n, double* x, double* f, double* g, int* ti, float* tr, double* td);
    void C2F(mcsec) (int* ind, int* n, double* x, double* f, double* g, int* ti, float* tr, double* td);

    // fsolve
    void C2F(fsol1) (int* n, double* x, double* v, int* iflag);
    void C2F(fsolj1)(int* n, double* x, double* jac, int* iflag);

    // lsqrsolve
    void C2F(lsqrsol1)  (int* m, int* n, double* x, double* v, int* iflag);
    void C2F(lsqrsolj1) (int* m, int* n, double* x, double* jac, int* ldjac, int* iflag);

}

class OPTIMIZATION_IMPEXP OptimizationFunctions
{

public :

    OptimizationFunctions(const std::wstring& callerName);
    ~OptimizationFunctions();

    void setXRows(int);
    void setXCols(int);
    int getXRows();
    int getXCols();

    // optim
    void setOptimCostfFunction(types::Callable*);
    bool setOptimCostfFunction(types::String*);
    void setCostfArgs(types::InternalType*);
    void execCostf(int* ind, int* n, double* x, double* f, double* g, int* ti, float* tr, double* td);

    // fsolve & lsqrsolve
    void setFsolveFctFunction(types::Callable*);
    bool setFsolveFctFunction(types::String*);
    void setFsolveFctArgs(types::InternalType*);
    void execFsolveFct(int* n, double* x, double* v, int* iflag);
    void execLsqrsolveFct(int* m, int* n, double* x, double* v, int* iflag);

    void setFsolveJacFunction(types::Callable*);
    bool setFsolveJacFunction(types::String*);
    void setFsolveJacArgs(types::InternalType*);
    void execFsolveJac(int* n, double* x, double* v, double* jac, int* ldjac, int* iflag);
    void execLsqrsolveJac(int* m, int* n, double* x, double* v, double* jac, int* ldjac, int* iflag);

private :

    std::map<std::wstring, void*> m_staticFunctionMap;
    std::wstring m_wstrCaller;
    int m_iXRows;
    int m_iXCols;

    // optim
    types::Callable* m_pCallOptimCostfFunction;
    types::String* m_pStringOptimCostfFunctionDyn;
    types::String* m_pStringOptimCostfFunctionStatic;
    std::vector<types::InternalType*> m_OptimArgs;
    void callCostfMacro(int* ind, int* n, double* x, double* f, double* g, int* ti, float* tr, double* td);

    // fsolve & lsqrsolve
    types::Callable* m_pCallFsolveFctFunction;
    types::String* m_pStringFsolveFctFunctionDyn;
    types::String* m_pStringFsolveFctFunctionStatic;
    std::vector<types::InternalType*> m_fsolveFctArgs;
    void callFsolveFctMacro(int* n, double* x, double* v, int* iflag);
    void callLsqrsolveFctMacro(int* m, int* n, double* x, double* v, int* iflag);

    types::Callable* m_pCallFsolveJacFunction;
    types::String* m_pStringFsolveJacFunctionDyn;
    types::String* m_pStringFsolveJacFunctionStatic;
    std::vector<types::InternalType*> m_fsolveJacArgs;
    void callFsolveJacMacro(int* n, double* x, double* v, double* jac, int* ldjac, int* iflag);
    void callLsqrsolveJacMacro(int* m, int* n, double* x, double* v, double* jac, int* ldjac, int* iflag);

};

class OPTIMIZATION_IMPEXP Optimization
{
    // differential equation functions
private :
    static OptimizationFunctions* m_OptimizationFunctions;

public :
    static void addOptimizationFunctions(OptimizationFunctions* _opFunction);
    static void removeOptimizationFunctions();
    static OptimizationFunctions* getOptimizationFunctions();

};
#endif /* !__OPTIMIZATIONS_HXX__ */

