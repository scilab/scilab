/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *  Copyright (C) 2014 - Scilab Enterprises - Sylvain GENIN
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#ifndef __TYPES_DOTDIVIDE_HXX__
#define __TYPES_DOTDIVIDE_HXX__

#include "generic_operations.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

void fillDotDivFunction();

//define arrays on operation functions
typedef types::InternalType*(*dotdiv_function)(types::InternalType*, types::InternalType*);

#define DECLARE_DOTDIV_PROTO(x) template<class T, class U, class O> types::InternalType* x(T *_pL, U *_pR)

DECLARE_DOTDIV_PROTO(dotdiv_M_M);
DECLARE_DOTDIV_PROTO(dotdiv_M_MC);
DECLARE_DOTDIV_PROTO(dotdiv_M_S);
DECLARE_DOTDIV_PROTO(dotdiv_M_SC);
DECLARE_DOTDIV_PROTO(dotdiv_M_E);
DECLARE_DOTDIV_PROTO(dotdiv_M_I);
DECLARE_DOTDIV_PROTO(dotdiv_M_IC);

DECLARE_DOTDIV_PROTO(dotdiv_MC_M);
DECLARE_DOTDIV_PROTO(dotdiv_MC_MC);
DECLARE_DOTDIV_PROTO(dotdiv_MC_S);
DECLARE_DOTDIV_PROTO(dotdiv_MC_SC);
DECLARE_DOTDIV_PROTO(dotdiv_MC_I);
DECLARE_DOTDIV_PROTO(dotdiv_MC_IC);

DECLARE_DOTDIV_PROTO(dotdiv_S_M);
DECLARE_DOTDIV_PROTO(dotdiv_S_MC);
DECLARE_DOTDIV_PROTO(dotdiv_S_S);
DECLARE_DOTDIV_PROTO(dotdiv_S_SC);
DECLARE_DOTDIV_PROTO(dotdiv_S_I);
DECLARE_DOTDIV_PROTO(dotdiv_S_IC);

DECLARE_DOTDIV_PROTO(dotdiv_SC_M);
DECLARE_DOTDIV_PROTO(dotdiv_SC_MC);
DECLARE_DOTDIV_PROTO(dotdiv_SC_S);
DECLARE_DOTDIV_PROTO(dotdiv_SC_SC);
DECLARE_DOTDIV_PROTO(dotdiv_SC_I);
DECLARE_DOTDIV_PROTO(dotdiv_SC_IC);

//[]
DECLARE_DOTDIV_PROTO(dotdiv_E_M);

//eye
DECLARE_DOTDIV_PROTO(dotdiv_I_M);
DECLARE_DOTDIV_PROTO(dotdiv_I_MC);
DECLARE_DOTDIV_PROTO(dotdiv_I_S);
DECLARE_DOTDIV_PROTO(dotdiv_I_SC);
DECLARE_DOTDIV_PROTO(dotdiv_I_I);
DECLARE_DOTDIV_PROTO(dotdiv_I_IC);

DECLARE_DOTDIV_PROTO(dotdiv_IC_M);
DECLARE_DOTDIV_PROTO(dotdiv_IC_MC);
DECLARE_DOTDIV_PROTO(dotdiv_IC_S);
DECLARE_DOTDIV_PROTO(dotdiv_IC_SC);
DECLARE_DOTDIV_PROTO(dotdiv_IC_I);
DECLARE_DOTDIV_PROTO(dotdiv_IC_IC);

#undef DECLARE_DOTDIV_PROTO

template<> types::InternalType* dotdiv_M_M<types::Sparse, types::Sparse, types::Sparse>(types::Sparse* _pL, types::Sparse* _pR);
template<> types::InternalType* dotdiv_M_M<types::Double, types::Sparse, types::Double>(types::Double* _pL, types::Sparse* _pR);
template<> types::InternalType* dotdiv_M_M<types::Sparse, types::Double, types::Double>(types::Sparse* _pL, types::Double* _pR);
template<> types::InternalType* dotdiv_M_M<types::Double, types::Sparse, types::Sparse>(types::Double* _pL, types::Sparse* _pR);
template<> types::InternalType* dotdiv_M_M<types::Sparse, types::Double, types::Sparse>(types::Sparse* _pL, types::Double* _pR);

template<> types::InternalType* dotdiv_M_M<types::Polynom, types::Polynom, types::Polynom>(types::Polynom* _pL, types::Polynom* _pR);
template<> types::InternalType* dotdiv_M_M<types::Polynom, types::Double, types::Polynom>(types::Polynom* _pL, types::Double* _pR);
template<> types::InternalType* dotdiv_M_M<types::Double, types::Polynom, types::Polynom>(types::Double* _pL, types::Polynom* _pR);

//x ./ x
template<typename T, typename U, typename O> inline static void dotdiv(T* l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] / (O)r[i];
    }
}

//xC ./ x
template<typename T, typename U, typename O> inline static void dotdiv(T* l, T* lc, size_t size, U* r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] / (O)r[i];
        oc[i] = (O)lc[i] / (O)r[i];
    }
}

//x ./ xC
template<typename T, typename U, typename O> inline static void dotdiv(T* l, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = ((O)l[i] * (O)r[i]) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
        oc[i] = ((O)l[i] * -(O)rc[i]) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
    }
}

//xC ./ xC
template<typename T, typename U, typename O> inline static void dotdiv(T* l, T* lc, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] =  ((O)l[i] * (O)r[i] + (O)lc[i] * (O)rc[i] ) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
        oc[i] = ((O)r[i] * (O)lc[i] - (O)rc[i] * (O)l[i] ) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
    }
}

//x1 ./ x1
template<typename T, typename U, typename O> inline static void dotdiv(T l, U r, O* o)
{
    *o = (O)l / (O)r;
}

//x1 ./ x1c
template<typename T, typename U, typename O> inline static void dotdiv(T l, size_t size, U r, U rc, O* o, O* oc)
{
    *o  = ((O)l * (O)r) / ((O)rc * (O)rc + (O)r * (O)r) ;
    *oc = ((O)l * -(O)rc) / ((O)rc * (O)rc + (O)r * (O)r) ;
}

//x1c ./ x1
template<typename T, typename U, typename O> inline static void dotdiv(T l, T lc, size_t size, U r, O* o, O* oc)
{
    *o = (O)l / (O)r;
    *oc = (O)lc / (O)r;
}

//x1c ./ x1c
template<typename T, typename U, typename O> inline static void dotdiv(T l, T lc, size_t size, U r, U rc, O* o, O* oc)
{
    *o  = ((O)l * (O)r + (O)lc * (O)rc ) / ((O)rc * (O)rc + (O)r * (O)r);
    *oc = ((O)r * (O)lc - (O)rc * (O)l ) / ((O)rc * (O)rc + (O)r * (O)r) ;
}

//x ./ x1
template<typename T, typename U, typename O> inline static void dotdiv(T* l, size_t size, U r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] / (O)r;
    }
}

//x1 ./ x
template<typename T, typename U, typename O> inline static void dotdiv(T l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l / (O)r[i];
    }
}


//x ./ x1c
template<typename T, typename U, typename O> inline static void dotdiv(T* l, size_t size, U r, U rc, O* o, O* oc)
{
    O denum = ((O)rc * (O)rc + (O)r * (O)r);
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = ((O)l[i] * (O)r) / denum ;
        oc[i] = ((O)l[i] * -(O)rc) / denum;
    }
}

//x1 ./ xc
template<typename T, typename U, typename O> inline static void dotdiv(T l, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = ((O)l * (O)r[i]) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
        oc[i] = ((O)l * -(O)rc[i]) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
    }
}

//xC ./ x1
template<typename T, typename U, typename O> inline static void dotdiv(T* l, T* lc, size_t size, U r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] / (O)r;
        oc[i] = (O)lc[i] / (O)r;
    }
}

//x1C ./ x
template<typename T, typename U, typename O> inline static void dotdiv(T l, T lc, size_t size, U* r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l / (O)r[i];
        oc[i] = (O)lc / (O)r[i];
    }
}


//xC ./ x1c
template<typename T, typename U, typename O> inline static void dotdiv(T* l, T* lc, size_t size, U r, U rc, O* o, O* oc)
{
    O denum = ((O)rc * (O)rc + (O)r * (O)r);
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] =  ((O)l[i] * (O)r + (O)lc[i] * (O)rc ) / denum ;
        oc[i] = ((O)r * (O)lc[i] - (O)rc * (O)l[i] ) / denum ;
    }
}

//x1C ./ xc
template<typename T, typename U, typename O> inline static void dotdiv(T l, T lc, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] =  ((O)l * (O)r[i] + (O)lc * (O)rc[i] ) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
        oc[i] = ((O)r[i] * (O)lc - (O)rc[i] * (O)l ) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
    }
}


#endif /* !__TYPES_DOTDIVIDE_HXX__ */