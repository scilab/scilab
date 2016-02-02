/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
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

#ifndef __TYPES_COMPARISON_NO_EQUAL_HXX__
#define __TYPES_COMPARISON_NO_EQUAL_HXX__

#include "generic_operations.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "sparse.hxx"
#include "polynom.hxx"
#include "string.hxx"
#include "struct.hxx"
#include "cell.hxx"
#include "graphichandle.hxx"
#include "macrofile.hxx"

void fillComparisonNoEqualFunction();

//define arrays on operation functions
typedef types::InternalType*(*compnoequal_function)(types::InternalType*, types::InternalType*);

#define DECLARE_COMPARISON_NO_EQUAL_PROTO(x) template<class T, class U, class O> types::InternalType* x(T *_pL, U *_pR)


DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_M_M);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_M_MC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_M_S);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_M_SC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_M_E);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_M_I);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_M_IC);

DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_MC_M);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_MC_MC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_MC_S);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_MC_SC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_MC_I);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_MC_IC);

DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_S_M);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_S_MC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_S_S);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_S_SC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_S_I);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_S_IC);

DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_SC_M);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_SC_MC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_SC_S);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_SC_SC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_SC_I);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_SC_IC);

//[]
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_E_M);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_E_E);

//eye
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_I_M);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_I_MC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_I_S);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_I_SC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_I_I);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_I_IC);

DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_IC_M);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_IC_MC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_IC_S);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_IC_SC);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_IC_I);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_IC_IC);

DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_P_M);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_M_P);

DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_SP_M);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_M_SP);

DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_B_M);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_M_B);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_B_S);
DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_S_B);

DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_LT_LT);

DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_MCR_MCR);

DECLARE_COMPARISON_NO_EQUAL_PROTO(compnoequal_UT_UT);


#undef DECLARE_COMPARISON_NO_EQUAL_PROTO

template<> types::InternalType* compnoequal_M_M<types::Sparse, types::Sparse, types::SparseBool>(types::Sparse* _pL, types::Sparse* _pR);

template<> types::InternalType* compnoequal_M_M<types::Polynom, types::Polynom, types::Bool>(types::Polynom* _pL, types::Polynom* _pR);

template<> types::InternalType* compnoequal_M_M<types::String, types::String, types::Bool>(types::String* _pL, types::String* _pR);

template<> types::InternalType* compnoequal_M_M<types::Struct, types::Struct, types::Bool>(types::Struct* _pL, types::Struct* _pR);

template<> types::InternalType* compnoequal_M_M<types::Cell, types::Cell, types::Bool>(types::Cell* _pL, types::Cell* _pR);

template<> types::InternalType* compnoequal_M_M<types::GraphicHandle, types::GraphicHandle, types::Bool>(types::GraphicHandle* _pL, types::GraphicHandle* _pR);

template<> types::InternalType* compnoequal_M_M<types::SparseBool, types::SparseBool, types::SparseBool>(types::SparseBool* _pL, types::SparseBool* _pR);
template<> types::InternalType* compnoequal_M_M<types::Bool, types::SparseBool, types::SparseBool>(types::Bool* _pL, types::SparseBool* _pR);
template<> types::InternalType* compnoequal_M_M<types::SparseBool, types::Bool, types::SparseBool>(types::SparseBool* _pL, types::Bool* _pR);

//x1 != x1
template<typename T, typename U, typename O> inline static void compnoequal(T l, U r, O* o)
{
    *o = ((T)l != (U)r);
}

//X != X
template<typename T, typename U, typename O> inline static void compnoequal(T* l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l[i], (U)r[i], &o[i]);
    }
}

//x != X
template<typename T, typename U, typename O> inline static void compnoequal(T l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l, (U)r[i], &o[i]);
    }
}

//X != x
template<typename T, typename U, typename O> inline static void compnoequal(T* l, size_t size, U r, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l[i], (U)r, &o[i]);
    }
}

//x1c != x1c
template<typename T, typename U, typename O> inline static void compnoequal(T l, T lc, U r, U rc, O* o)
{
    *o = (((T)l != (U)r) || ((T)lc != (U)rc));
}

//X1c != X1c
template<typename T, typename U, typename O> inline static void compnoequal(T* l, T* lc, size_t size, U* r, U* rc, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l[i], (T)lc[i], (U)r[i], (U)rc[i], &o[i]);
    }
}

//X1c != x1c
template<typename T, typename U, typename O> inline static void compnoequal(T* l, T* lc, size_t size, U r, U rc, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l[i], (T)lc[i], (U)r, (U)rc, &o[i]);
    }
}

//x1c != X1c
template<typename T, typename U, typename O> inline static void compnoequal(T l, T lc, size_t size, U* r, U* rc, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l, (T)lc, (U)r[i], (U)rc[i], &o[i]);
    }
}

//X1 != X1c
template<typename T, typename U, typename O> inline static void compnoequal(T* l, size_t size, U* r, U* rc, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l[i], (T)0, (U)r[i], (U)rc[i], &o[i]);
    }
}

//X1 != x1c
template<typename T, typename U, typename O> inline static void compnoequal(T* l, size_t size, U r, U rc, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l[i], (T)0, (U)r, (U)rc, &o[i]);
    }
}

//x1 != X1c
template<typename T, typename U, typename O> inline static void compnoequal(T l, size_t size, U* r, U* rc, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l, (T)0, (U)r[i], (U)rc[i], &o[i]);
    }
}

//X1c != X1
template<typename T, typename U, typename O> inline static void compnoequal(T* l, T* lc, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l[i], (T)lc[i], (U)r[i], (U)0, &o[i]);
    }
}

//X1c != x1
template<typename T, typename U, typename O> inline static void compnoequal(T* l, T* lc, size_t size, U r, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l[i], (T)lc[i], (U)r, (U)0, &o[i]);
    }
}

//x1c != X1
template<typename T, typename U, typename O> inline static void compnoequal(T l, T lc, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size; ++i)
    {
        compnoequal((T)l, (T)lc, (U)r[i], (U)0, &o[i]);
    }
}

#endif /* !__TYPES_COMPARISON_NO_EQUAL_HXX__ */
