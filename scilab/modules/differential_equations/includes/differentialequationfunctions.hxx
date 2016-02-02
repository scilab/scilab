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

#ifndef __DIFFERENTIALEQUATIONS_HXX__
#define __DIFFERENTIALEQUATIONS_HXX__

#include <list>
#include <map>

#include "dynlib_differential_equations.h"
#include "string.hxx"
#include "callable.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
}

extern "C"
{
    // jacobian
    void C2F(jex)   (int*, double*, double*, int*, int*, double*, int*);

    // ode
    void C2F(arnol) (int*, double*, double*, double*);
    void C2F(fex)   (int*, double*, double*, double*);
    void C2F(fex2)  (int*, double*, double*, double*);
    void C2F(fex3)  (int*, double*, double*, double*);
    void C2F(fexab) (int*, double*, double*, double*);
    void C2F(loren) (int*, double*, double*, double*);
    void C2F(bcomp) (int*, double*, double*, double*);
    void C2F(lcomp) (int*, double*, double*, double*);

    // odedc
    typedef int(*odedc_f_t)(int*, int*, int*, double*, double*, double*);
    void C2F(fcd)   (int*, int*, int*, double*, double*, double*);
    void C2F(fcd1)  (int*, int*, int*, double*, double*, double*);
    void C2F(fexcd) (int*, int*, int*, double*, double*, double*);
    void C2F(phis)  (int*, int*, int*, double*, double*, double*);
    void C2F(phit)  (int*, int*, int*, double*, double*, double*);

    // intg
    double C2F(intgex)(double*);

    // int2d
    double C2F(int2dex)(double*, double*);

    // int3d
    void C2F(int3dex)(double*, int*, double*);

    // feval
    int C2F(parab)  (int*, double*, double*, double*, int*);
    int C2F(parabc) (int*, double*, double*, double*, int*);

    // bvode
    void C2F(cndg)  (int*, double*, double*);
    void C2F(cng)   (int*, double*, double*);
    void C2F(cnf)   (double*, double*, double*);
    void C2F(cndf)  (double*, double*, double*);
    void C2F(cngu)  (double*, double*, double*);

    // impl
    void C2F(resid) (int*, double*, double*, double*, double*, int*);
    void C2F(aplusp)(int*, double*, double*, double*, double*, double*, int*);
    void C2F(dgbydy)(int*, double*, double*, double*, double*, double*, double*, int*);

    // dassl, dasrt, daskr
    void C2F(res1)  (double*, double*, double*, double*, int*, double*, int*);
    void C2F(res2)  (double*, double*, double*, double*, int*, double*, int*);
    void C2F(dres1) (double*, double*, double*, double*, int*, double*, int*);
    void C2F(dres2) (double*, double*, double*, double*, int*, double*, int*);
    void C2F(jac2)  (double*, double*, double*, double*, double*, double*, int*);
    void C2F(djac2) (double*, double*, double*, double*, double*, double*, int*);
    void C2F(djac1) (double*, double*, double*, double*, double*, double*, int*);

    // dasrt
    void C2F(gr1)(int*, double*, double*, int*, double*, double*, int*);
    void C2F(gr2)(int*, double*, double*, int*, double*, double*, int*);

    // daskr
    void C2F(bpjacd)(double*, int*, int*, double*, double*, double*, double*, double*,
                     double*, double*, double*, double*, int*, int*, double*, int*);
    void C2F(bpsold)(int*, double*, double*, double*, double*, double*, double*,
                     double*, double*, int*, double*, double*, int*, double*, int*);
}

class DIFFERENTIAL_EQUATIONS_IMPEXP DifferentialEquationFunctions
{

public :

    DifferentialEquationFunctions(const std::wstring& callerName);
    ~DifferentialEquationFunctions();

    void setOdeYRows(int);
    void setOdeYCols(int);

    void setOdedcYDSize(int);
    void setOdedcFlag();
    void resetOdedcFlag();

    void setFFunction(types::Callable*);
    void setJacFunction(types::Callable*);
    void setGFunction(types::Callable*);
    void setPsolFunction(types::Callable*);
    void setPjacFunction(types::Callable*);

    bool setFFunction(types::String*);
    bool setJacFunction(types::String*);
    bool setGFunction(types::String*);
    bool setPsolFunction(types::String*);
    bool setPjacFunction(types::String*);

    void setFArgs(types::InternalType*);
    void setJacArgs(types::InternalType*);
    void setGArgs(types::InternalType*);
    void setPsolArgs(types::InternalType*);
    void setPjacArgs(types::InternalType*);

    int getOdeYRows();
    int getOdeYCols();

    int getOdedcYDSize();
    int getOdedcFlag();

    void execOdeF(int* n, double* t, double* y, double* ydot);
    void execFunctionJac(int* n, double* t, double* y, int* ml, int* mu, double* J, int* nrpd);
    void execFunctionG(int* n, double* t, double* y, int* ng, double* gout);

    double execIntgF(double* x);
    double execInt2dF(double* x, double* y);
    void execInt3dF(double* x, int* numfun, double* funvls);

    void execFevalF(int* nn, double* x1, double* x2, double* xres, int* itype);

    // bvode
    void setFsubFunction(types::Callable*);
    void setDfsubFunction(types::Callable*);
    void setGsubFunction(types::Callable*);
    void setDgsubFunction(types::Callable*);
    void setGuessFunction(types::Callable*);

    bool setFsubFunction(types::String*);
    bool setDfsubFunction(types::String*);
    bool setGsubFunction(types::String*);
    bool setDgsubFunction(types::String*);
    bool setGuessFunction(types::String*);

    void setFsubArgs(types::InternalType*);
    void setDfsubArgs(types::InternalType*);
    void setGsubArgs(types::InternalType*);
    void setDgsubArgs(types::InternalType*);
    void setGuessArgs(types::InternalType*);

    void setBvodeM(int);
    void setBvodeN(int);

    void execBvodeGsub(int*, double*, double*);
    void execBvodeDgsub(int*, double*, double*);
    void execBvodeFsub(double*, double*, double*);
    void execBvodeDfsub(double*, double*, double*);
    void execBvodeGuess(double*, double*, double*);

    // impl
    void execImplF(int*, double*, double*, double*, double*, int*);
    void execImplG(int*, double*, double*, double*, double*, double*, int*);
    void execImplJac(int*, double*, double*, double*, double*, double*, double*, int*);

    //dassl dasrt
    void execDasslF(double*, double*, double*, double*, int*, double*, int*);
    void execDasslJac(double*, double*, double*, double*, double*, double*, int*);
    void setMu(int);
    void setMl(int);

    // dasrt
    void execDasrtG(int*, double*, double*, int*, double*, double*, int*);

    // daskr
    void execDaskrPsol(int* neq, double* t, double* y, double* ydot, double* savr, double* wk,
                       double* cj, double* wght, double* wp, int* iwp, double* b, double* eplin,
                       int* iter, double* rpar, int* ipar);
    void execDaskrPjac(double* res, int* ires, int* neq, double* t, double* y, double* ydot,
                       double* rewt, double* savr, double* wk, double* h, double* cj,
                       double* wp, int* iwp, int* ier, double* rpar, int* ipar);


private :

    std::map<std::wstring, void*> m_staticFunctionMap;
    std::wstring m_wstrCaller;

    int m_odeYRows;
    int m_odeYCols;

    // odedc
    int m_odedcYDSize;
    int m_odedcFlag;

    // bvode
    int m_bvodeM;
    int m_bvodeN;

    // dassl dasrt daskr
    int m_mu;
    int m_ml;
    bool m_bandedJac;

    types::Callable* m_pCallFFunction;
    types::Callable* m_pCallJacFunction;
    types::Callable* m_pCallGFunction;
    types::Callable* m_pCallPsolFunction;
    types::Callable* m_pCallPjacFunction;

    types::String* m_pStringFFunctionDyn;
    types::String* m_pStringJacFunctionDyn;
    types::String* m_pStringGFunctionDyn;
    types::String* m_pStringPsolFunctionDyn;
    types::String* m_pStringPjacFunctionDyn;

    types::String* m_pStringFFunctionStatic;
    types::String* m_pStringJacFunctionStatic;
    types::String* m_pStringGFunctionStatic;
    types::String* m_pStringPsolFunctionStatic;
    types::String* m_pStringPjacFunctionStatic;

    std::vector<types::InternalType*> m_FArgs;
    std::vector<types::InternalType*> m_JacArgs;
    std::vector<types::InternalType*> m_odeGArgs;
    std::vector<types::InternalType*> m_pSolArgs;
    std::vector<types::InternalType*> m_pJacArgs;

    // ode / odedc
    void callOdeMacroF(int* n, double* t, double* y, double* ydot);
    void callMacroJac(int *n, double *t, double *y, int *ml, int *mu, double *J, int *nrpd);
    void callMacroG(int* n, double* t, double* y, int* ng, double* gout);

    // intg
    double callIntgMacroF(double* x);

    // int2d
    double callInt2dMacroF(double* x, double* y);

    // int3d
    void callInt3dMacroF(double* x, int* numfun, double* funvls);

    // feval
    void callFevalMacroF(int *nn, double *x1, double *x2, double *xres, int *itype);

    // bvode
    types::Callable* m_pCallFsubFunction;
    types::Callable* m_pCallDfsubFunction;
    types::Callable* m_pCallGsubFunction;
    types::Callable* m_pCallDgsubFunction;
    types::Callable* m_pCallGuessFunction;

    types::String* m_pStringFsubFunctionDyn;
    types::String* m_pStringDfsubFunctionDyn;
    types::String* m_pStringGsubFunctionDyn;
    types::String* m_pStringDgsubFunctionDyn;
    types::String* m_pStringGuessFunctionDyn;

    types::String* m_pStringFsubFunctionStatic;
    types::String* m_pStringDfsubFunctionStatic;
    types::String* m_pStringGsubFunctionStatic;
    types::String* m_pStringDgsubFunctionStatic;
    types::String* m_pStringGuessFunctionStatic;

    std::vector<types::InternalType*> m_FsubArgs;
    std::vector<types::InternalType*> m_DfsubArgs;
    std::vector<types::InternalType*> m_GsubArgs;
    std::vector<types::InternalType*> m_DgsubArgs;
    std::vector<types::InternalType*> m_GuessArgs;

    void callBvodeMacroGsub  (int* i, double* z, double* g);
    void callBvodeMacroDgsub (int* i, double* z, double* g);
    void callBvodeMacroFsub  (double* x, double* z, double* d);
    void callBvodeMacroDfsub (double* x, double* z, double* d);
    void callBvodeMacroGuess (double* x, double* z, double* d);

    // impl
    void callImplMacroF(int*, double*, double*, double*, double*, int*);
    void callImplMacroG(int*, double*, double*, double*, double*, double*, int*);
    void callImplMacroJac(int*, double*, double*, double*, double*, double*, double*, int*);

    // dassl dasrt
    void callDasslMacroF(double*, double*, double*, double*, int*, double*, int*);
    void callDasslMacroJac(double*, double*, double*, double*, double*, double*, int*);

    // dasrt
    void callDasrtMacroG(int*, double*, double*, int*, double*, double*, int*);

    // daskr
    void callDaskrMacroPsol(int* neq, double* t, double* y, double* ydot, double* savr, double* wk,
                            double* cj, double* wght, double* wp, int* iwp, double* b, double* eplin,
                            int* iter, double* rpar, int* ipar);
    void callDaskrMacroPjac(double* res, int* ires, int* neq, double* t, double* y, double* ydot,
                            double* rewt, double* savr, double* wk, double* h, double* cj,
                            double* wp, int* iwp, int* ier, double* rpar, int* ipar);
};

class DIFFERENTIAL_EQUATIONS_IMPEXP DifferentialEquation
{
    // differential equation functions
private :
    static std::list<DifferentialEquationFunctions*> m_DifferentialEquationFunctions;

public :
    static void addDifferentialEquationFunctions(DifferentialEquationFunctions* _deFunction);
    static void removeDifferentialEquationFunctions();
    static DifferentialEquationFunctions* getDifferentialEquationFunctions();
};
#endif /* !__DIFFERENTIALEQUATIONS_HXX__ */

