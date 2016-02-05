/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab enterprises - Antoine ELIAS
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
#ifndef __TYPES_DOTMULTIPLICATION_HXX__
#define __TYPES_DOTMULTIPLICATION_HXX__

#include "generic_operations.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

void fillDotMulFunction();

//define arrays on operation functions
typedef types::InternalType*(*dotmul_function)(types::InternalType*, types::InternalType*);

#define DECLARE_DOTMUL_PROTO(x) template<class T, class U, class O> types::InternalType* x(T *_pL, U *_pR)
DECLARE_DOTMUL_PROTO(dotmul_M_M);
DECLARE_DOTMUL_PROTO(dotmul_M_MC);
DECLARE_DOTMUL_PROTO(dotmul_M_S);
DECLARE_DOTMUL_PROTO(dotmul_M_SC);
DECLARE_DOTMUL_PROTO(dotmul_M_E);
DECLARE_DOTMUL_PROTO(dotmul_M_I);
DECLARE_DOTMUL_PROTO(dotmul_M_IC);

DECLARE_DOTMUL_PROTO(dotmul_MC_M);
DECLARE_DOTMUL_PROTO(dotmul_MC_MC);
DECLARE_DOTMUL_PROTO(dotmul_MC_S);
DECLARE_DOTMUL_PROTO(dotmul_MC_SC);
DECLARE_DOTMUL_PROTO(dotmul_MC_I);
DECLARE_DOTMUL_PROTO(dotmul_MC_IC);

DECLARE_DOTMUL_PROTO(dotmul_S_M);
DECLARE_DOTMUL_PROTO(dotmul_S_MC);
DECLARE_DOTMUL_PROTO(dotmul_S_S);
DECLARE_DOTMUL_PROTO(dotmul_S_SC);
DECLARE_DOTMUL_PROTO(dotmul_S_I);
DECLARE_DOTMUL_PROTO(dotmul_S_IC);

DECLARE_DOTMUL_PROTO(dotmul_SC_M);
DECLARE_DOTMUL_PROTO(dotmul_SC_MC);
DECLARE_DOTMUL_PROTO(dotmul_SC_S);
DECLARE_DOTMUL_PROTO(dotmul_SC_SC);
DECLARE_DOTMUL_PROTO(dotmul_SC_I);
DECLARE_DOTMUL_PROTO(dotmul_SC_IC);

//[]
DECLARE_DOTMUL_PROTO(dotmul_E_M);

//eye
DECLARE_DOTMUL_PROTO(dotmul_I_M);
DECLARE_DOTMUL_PROTO(dotmul_I_MC);
DECLARE_DOTMUL_PROTO(dotmul_I_S);
DECLARE_DOTMUL_PROTO(dotmul_I_SC);
DECLARE_DOTMUL_PROTO(dotmul_I_I);
DECLARE_DOTMUL_PROTO(dotmul_I_IC);

DECLARE_DOTMUL_PROTO(dotmul_IC_M);
DECLARE_DOTMUL_PROTO(dotmul_IC_MC);
DECLARE_DOTMUL_PROTO(dotmul_IC_S);
DECLARE_DOTMUL_PROTO(dotmul_IC_SC);
DECLARE_DOTMUL_PROTO(dotmul_IC_I);
DECLARE_DOTMUL_PROTO(dotmul_IC_IC);

#undef DECLARE_DOTMUL_PROTO

template<> types::InternalType* dotmul_M_M<types::Sparse, types::Sparse, types::Sparse>(types::Sparse* _pL, types::Sparse* _pR);
template<> types::InternalType* dotmul_M_M<types::Double, types::Sparse, types::Double>(types::Double* _pL, types::Sparse* _pR);
template<> types::InternalType* dotmul_M_M<types::Sparse, types::Double, types::Double>(types::Sparse* _pL, types::Double* _pR);
template<> types::InternalType* dotmul_M_M<types::Double, types::Sparse, types::Sparse>(types::Double* _pL, types::Sparse* _pR);
template<> types::InternalType* dotmul_M_M<types::Sparse, types::Double, types::Sparse>(types::Sparse* _pL, types::Double* _pR);

template<> types::InternalType* dotmul_M_M<types::Polynom, types::Polynom, types::Polynom>(types::Polynom* _pL, types::Polynom* _pR);
template<> types::InternalType* dotmul_M_M<types::Polynom, types::Double, types::Polynom>(types::Polynom* _pL, types::Double* _pR);
template<> types::InternalType* dotmul_M_M<types::Double, types::Polynom, types::Polynom>(types::Double* _pL, types::Polynom* _pR);



//x .* x
template<typename T, typename U, typename O> inline static void dotmul(T* l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] * (O)r[i];
    }
}

//xC .* x
template<typename T, typename U, typename O> inline static void dotmul(T* l, T* lc, size_t size, U* r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] * (O)r[i];
        oc[i] = (O)lc[i] * (O)r[i];
    }
}

//x .* xC
template<typename T, typename U, typename O> inline static void dotmul(T* l, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] * (O)r[i];
        oc[i] = (O)l[i] * (O)rc[i];
    }
}

//xC .* xC
template<typename T, typename U, typename O> inline static void dotmul(T* l, T* lc, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] * (O)r[i] - (O)lc[i] * (O)rc[i];
        oc[i] = (O)l[i] * (O)rc[i] + (O)lc[i] * (O)r[i];
    }
}

//x1 .* x1
template<typename T, typename U, typename O> inline static void dotmul(T l, U r, O* o)
{
    *o = (O)l * (O)r;
}

//x1 .* x1c
template<typename T, typename U, typename O> inline static void dotmul(T l, size_t /*size*/, U r, U rc, O* o, O* oc)
{
    *o = (O)l * (O)r;
    *oc = (O)l * (O)rc;
}

//x1c .* x1
template<typename T, typename U, typename O> inline static void dotmul(T l, T lc, size_t /*size*/, U r, O* o, O* oc)
{
    *o = (O)l * (O)r;
    *oc = (O)lc * (O)r;
}

//x1c .* x1c
template<typename T, typename U, typename O> inline static void dotmul(T l, T lc, size_t /*size*/, U r, U rc, O* o, O* oc)
{
    *o = (O)l * (O)r - (O)lc * (O)rc;
    *oc = (O)lc * (O)r + (O)l * (O)rc;
}

//x .* x1
template<typename T, typename U, typename O> inline static void dotmul(T* l, size_t size, U r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] * (O)r;
    }
}

//x .* x1c
template<typename T, typename U, typename O> inline static void dotmul(T* l, size_t size, U r, U rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] * (O)r;
        oc[i] = (O)l[i] * (O)rc;
    }
}

//xC .* x1
template<typename T, typename U, typename O> inline static void dotmul(T* l, T* lc, size_t size, U r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] * (O)r;
        oc[i] = (O)lc[i] * (O)r;
    }
}

//xC .* x1c
template<typename T, typename U, typename O> inline static void dotmul(T* l, T* lc, size_t size, U r, U rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] * (O)r - (O)lc[i] * (O)rc;
        oc[i] = (O)lc[i] * (O)r + (O)l[i] * (O)rc;
    }
}

#endif /* !__TYPES_DOTMULTIPLICATION_HXX__ */
