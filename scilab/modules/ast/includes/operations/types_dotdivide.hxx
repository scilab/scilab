/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *  Copyright (C) 2014 - Scilab Enterprises - Sylvain GENIN
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
#ifndef __TYPES_DOTDIVIDE_HXX__
#define __TYPES_DOTDIVIDE_HXX__

#include "generic_operations.hxx"
#include "configvariable.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

extern"C"
{
#include "abs.h"
}

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

//x1 ./ x1
template<typename T, typename U, typename O> inline static void dotdiv(T l, U r, O* o)
{
    if ((O)r == 0)
    {
        ConfigVariable::setDivideByZero(true);
    }
    *o = (O)l / (O)r;
}

//x1 ./ x1c
template<typename T, typename U, typename O> inline static void dotdiv(T l, size_t size, U r, U rc, O* o, O* oc)
{
    dotdiv((O)l * (O)r, (O)rc * (O)rc + (O)r * (O)r, o);
    dotdiv((O)l * -(O)rc, (O)rc * (O)rc + (O)r * (O)r, oc);
    //*o  = ((O)l * (O)r) / ((O)rc * (O)rc + (O)r * (O)r) ;
    //*oc = ((O)l * -(O)rc) / ((O)rc * (O)rc + (O)r * (O)r) ;
}

template<> inline void dotdiv<double, double, double>(double l, size_t size, double r, double rc, double* o, double* oc)
{
    if (rc == 0)
    {
        dotdiv<double, double, double>(l, r, o);
        *oc = 0;
    }
    else if (r == 0)
    {
        *o = 0;
        dotdiv<double, double, double>(-l, rc, oc);
    }
    else
    {
        double dblAbsSum    = dabss(r) + dabss(rc);
        double dblReal1Sum  = l  / dblAbsSum;
        double dblReal2Sum  = r  / dblAbsSum;
        double dblImg2Sum   = rc / dblAbsSum;
        double dblSum       = pow(dblReal2Sum, 2) + pow(dblImg2Sum, 2);
        *o                  = (dblReal1Sum * dblReal2Sum) / dblSum;
        *oc                 = (-dblReal1Sum * dblImg2Sum) / dblSum;
    }
}



//x1c ./ x1
template<typename T, typename U, typename O> inline static void dotdiv(T l, T lc, size_t size, U r, O* o, O* oc)
{
    dotdiv<T, U, O>(l, r, o);
    dotdiv<T, U, O>(lc, r, oc);
    //*o = (O)l / (O)r;
    //*oc = (O)lc / (O)r;
}

//x1c ./ x1c
template<typename T, typename U, typename O> inline static void dotdiv(T l, T lc, size_t size, U r, U rc, O* o, O* oc)
{
    dotdiv((O)l * (O)r + (O)lc * (O)rc, (O)rc * (O)rc + (O)r * (O)r, o);
    dotdiv((O)r * (O)lc - (O)rc * (O)l, (O)rc * (O)rc + (O)r * (O)r, oc);
    //*o = ((O)l * (O)r + (O)lc * (O)rc) / ((O)rc * (O)rc + (O)r * (O)r);
    //*oc = ((O)r * (O)lc - (O)rc * (O)l) / ((O)rc * (O)rc + (O)r * (O)r) ;
}

template<> inline void dotdiv<double, double, double>(double l, double lc, size_t size, double r, double rc, double* o, double* oc)
{
    if (rc == 0)
    {
        if (r == 0)
        {
            //got NaN + i NaN
            dotdiv<double, double, double>(rc, r, o);
            *oc = *o;
        }
        else
        {
            *o  = l  / r;
            *oc = lc / r;
        }
    }
    else if (r == 0)
    {
        *o  = lc / rc;
        *oc = -l / rc;
    }
    else
    {
        if (dabss(r) >= dabss(rc))
        {
            double oRatio = rc / r;
            double oSum = r + oRatio * rc;
            *o = (l + lc * oRatio) / oSum;
            *oc = (lc - l * oRatio) / oSum;
        }
        else
        {
            double oRatio = r / rc;
            double oSum = rc + oRatio * r;
            *o = (l * oRatio + lc) / oSum;
            *oc = (lc * oRatio - l) / oSum;
        }
    }
}


//x ./ x
template<typename T, typename U, typename O> inline static void dotdiv(T* l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, U r, O* o)
        dotdiv<T, U, O>(l[i], r[i], &o[i]);
        //o[i] = (O)l[i] / (O)r[i];
    }
}

//xC ./ x
template<typename T, typename U, typename O> inline static void dotdiv(T* l, T* lc, size_t size, U* r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, T lc, size_t size, U r, O* o, O* oc)
        dotdiv<T, U, O>(l[i], lc[i], (size_t)1, r[i], &o[i], &oc[i]);
        //o[i] = (O)l[i] / (O)r[i];
        //oc[i] = (O)lc[i] / (O)r[i];
    }
}

//x ./ xC
template<typename T, typename U, typename O> inline static void dotdiv(T* l, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, size_t size, U r, U rc, O* o, O* oc)
        dotdiv<T, U, O>(l[i], 1, r[i], rc[i], &o[i], &oc[i]);
        //o[i] = ((O)l[i] * (O)r[i]) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
        //oc[i] = ((O)l[i] * -(O)rc[i]) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
    }
}

//xC ./ xC
template<typename T, typename U, typename O> inline static void dotdiv(T* l, T* lc, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, T lc, size_t size, U r, U rc, O* o, O* oc)
        dotdiv<T, U, O>(l[i], lc[i], 1, r[i], rc[i], &o[i], &oc[i]);
        //o[i] =  ((O)l[i] * (O)r[i] + (O)lc[i] * (O)rc[i] ) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
        //oc[i] = ((O)r[i] * (O)lc[i] - (O)rc[i] * (O)l[i] ) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
    }
}

//x ./ x1
template<typename T, typename U, typename O> inline static void dotdiv(T* l, size_t size, U r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, U r, O* o)
        dotdiv<T, U, O>(l[i], r, &o[i]);
        //o[i] = (O)l[i] / (O)r;
    }
}

//x1 ./ x
template<typename T, typename U, typename O> inline static void dotdiv(T l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, U r, O* o)
        dotdiv<T, U, O>(l, r[i], &o[i]);
        //o[i] = (O)l / (O)r[i];
    }
}


//x ./ x1c
template<typename T, typename U, typename O> inline static void dotdiv(T* l, size_t size, U r, U rc, O* o, O* oc)
{
    //O denum = ((O)rc * (O)rc + (O)r * (O)r);
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, size_t size, U r, U rc, O* o, O* oc)
        dotdiv<T, U, O >(l[i], (size_t)1, r, rc, &o[i], &oc[i]);
        //o[i] = ((O)l[i] * (O)r) / denum;
        //oc[i] = ((O)l[i] * -(O)rc) / denum;
    }
}

//x1 ./ xc
template<typename T, typename U, typename O> inline static void dotdiv(T l, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, size_t size, U r, U rc, O* o, O* oc)
        dotdiv<T, U, O>(l, (size_t)1, r[i], rc[i], &o[i], &oc[i]);
        //o[i] = ((O)l * (O)r[i]) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
        //oc[i] = ((O)l * -(O)rc[i]) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
    }
}

//xC ./ x1
template<typename T, typename U, typename O> inline static void dotdiv(T* l, T* lc, size_t size, U r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, T lc, size_t size, U r, O* o, O* oc)
        dotdiv<T, U, O>(l[i], lc[i], (size_t)1, r, &o[i], &oc[i]);
        //o[i] = (O)l[i] / (O)r;
        //oc[i] = (O)lc[i] / (O)r;
    }
}

//x1C ./ x
template<typename T, typename U, typename O> inline static void dotdiv(T l, T lc, size_t size, U* r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, T lc, size_t size, U r, O* o, O* oc)
        dotdiv<T, U, O>(l, lc, (size_t)1, r[i], &o[i], &oc[i]);
        //o[i] = (O)l / (O)r[i];
        //oc[i] = (O)lc / (O)r[i];
    }
}


//xC ./ x1c
template<typename T, typename U, typename O> inline static void dotdiv(T* l, T* lc, size_t size, U r, U rc, O* o, O* oc)
{
    //    O denum = ((O)rc * (O)rc + (O)r * (O)r);
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, T lc, size_t size, U r, O* o, O* oc)
        dotdiv<T, U, O>(l[i], lc[i], (size_t)1, r, rc, &o[i], &oc[i]);
        //o[i] = ((O)l[i] * (O)r + (O)lc[i] * (O)rc) / denum;
        //oc[i] = ((O)r * (O)lc[i] - (O)rc * (O)l[i] ) / denum ;
    }
}

//x1C ./ xc
template<typename T, typename U, typename O> inline static void dotdiv(T l, T lc, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        //dotdiv(T l, T lc, size_t size, U r, O* o, O* oc)
        dotdiv<T, U, O>(l, lc, (size_t)1, r[i], rc[i], &o[i], &oc[i]);
        //o[i] = ((O)l * (O)r[i] + (O)lc * (O)rc[i]) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]);
        //oc[i] = ((O)r[i] * (O)lc - (O)rc[i] * (O)l ) / ((O)rc[i] * (O)rc[i] + (O)r[i] * (O)r[i]) ;
    }
}


#endif /* !__TYPES_DOTDIVIDE_HXX__ */
