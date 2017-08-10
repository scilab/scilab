/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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


#ifndef __SIGNALPROCESSING_HXX__
#define __SIGNALPROCESSING_HXX__

#include <map>
#include "dynlib_signal_processing.h"
#include "string.hxx"
#include "callable.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
}

extern "C"
{
    extern void C2F(tscccf)(double *x, double *y, int *length, double *cxy, double *xymean, int *lag, int *error);
    extern void C2F(cmpse2)(int *iSect, int *iTotalSize, int *iMode, void *pXFunction, void *pYFunction, double *xa, double *xr, double *xi, double *zr, double *zi, int *error);

    extern void C2F(cmpse3)(int *mfft, int *mnx, int *iMode, double *x, double *yi, double *xu, double *xui, double * w, double *wi, int *iErr, int *ichaud, int *nbx);
}

typedef void(*dgetx_f_t)(double*, int*, int*);
typedef void(*dgety_f_t)(double*, int*, int*);

extern void C2F(cmpse2)(int *iSect, int *iTotalSize, int *iMode, dgetx_f_t, dgety_f_t, double *xa, double *xr, double *xi, double *zr, double *zi, int *error);

SIGNAL_PROCESSING_IMPEXP void dgety_f(double* x, int* siz, int* iss);
SIGNAL_PROCESSING_IMPEXP void dgetx_f(double* x, int* siz, int* iss);

class SIGNAL_PROCESSING_IMPEXP Signalprocessingfunctions
{

public :
    Signalprocessingfunctions(const std::wstring& callerName);
    ~Signalprocessingfunctions();

    void setDgetx(types::Callable*);
    void setDgety(types::Callable*);

    void setDgetx(types::String*);
    void setDgety(types::String*);

    void execDgetx(double*, int* , int*);
    void execDgety(double*, int* , int*);

    void callDgetx(double* x, int* siz, int* iss);
    void callDgety(double* y, int* siz, int* iss);

    void execFunctionDgetx(double*, int*, int* );
    void execFunctionDgety(double*, int* , int* );

private :

    std::vector<types::InternalType*> m_FArgs;
    std::map<std::wstring, void*> m_staticFunctionMap;
    std::wstring m_wstrCaller;

    types::Callable* m_pCallDgetx;
    types::Callable* m_pCallDgety;

    types::String* m_pStringDgetxDyn;
    types::String* m_pStringDgetyDyn;

    types::String* m_pStringDgetxStatic;
    types::String* m_pStringDgetyStatic;

    std::vector<types::InternalType*> m_dgetxArgs;
    std::vector<types::InternalType*> m_dgetyArgs;





};

class SIGNAL_PROCESSING_IMPEXP Signalprocessing
{
    // differential equation functions
private :
    static Signalprocessingfunctions* m_Signalprocessingfunctions;

public :
    static void addSignalprocessingfunctions(Signalprocessingfunctions* _spFunction);
    static void removeSignalprocessingfunctions();
    static Signalprocessingfunctions* getSignalprocessingfunctions();
};
#endif /* !__SIGNALPROCESSING_HXX__ */
