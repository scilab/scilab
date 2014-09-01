/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_AND_H__
#define __TYPES_AND_H__

#include "generic_operations.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "bool.hxx"
#include "sparse.hxx"

void fillAndFunction();

//define arrays on operation functions
typedef types::InternalType*(*and_function)(types::InternalType*, types::InternalType*);

#define DECLARE_AND_PROTO(x) \
    template<class T, class U, class O> \
    inline types::InternalType* x(T *_pL, U *_pR)

DECLARE_AND_PROTO(and_M_M);
DECLARE_AND_PROTO(and_M_S);
DECLARE_AND_PROTO(and_M_E);

DECLARE_AND_PROTO(and_S_M);
DECLARE_AND_PROTO(and_S_S);
DECLARE_AND_PROTO(and_S_E);

//[]
DECLARE_AND_PROTO(and_E_M);

//eye
DECLARE_AND_PROTO(and_I_M);
DECLARE_AND_PROTO(and_I_S);

DECLARE_AND_PROTO(and_int_M_M);
DECLARE_AND_PROTO(and_int_M_S);
DECLARE_AND_PROTO(and_int_S_M);
DECLARE_AND_PROTO(and_int_S_S);

//boolean sparse specialisation
template<> inline types::InternalType* and_M_M<types::SparseBool, types::SparseBool, types::SparseBool>(types::SparseBool* _pL, types::SparseBool* _pR);
template<> inline types::InternalType* and_M_M<types::SparseBool, types::Bool, types::SparseBool>(types::SparseBool* _pL, types::Bool* _pR);
template<> inline types::InternalType* and_M_M<types::Bool, types::SparseBool, types::SparseBool>(types::Bool* _pL, types::SparseBool* _pR);

//x & x
template<typename T, typename U, typename O> inline static void bit_and(T* l, long long size, U* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (((O)l[i] != (O)0) && ((O)r[i] != (O)0)) ? (O)1 : (O)0;
    }
}

//x1 & x
template<typename T, typename U, typename O> inline static void bit_and(T l, long long size, U* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (((O)l != (O)0) && ((O)r[i] != (O)0)) ? (O)1 : (O)0;
    }
}

//x & x1
template<typename T, typename U, typename O> inline static void bit_and(T* l, long long size, U r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (((O)l[i] != (O)0) && ((O)r != (O)0)) ? (O)1 : (O)0;
    }
}

//x1 & x1
template<typename T, typename U, typename O> inline static void bit_and(T l, U r, O* o)
{
    *o = (((O)l != (O)0) && ((O)r != (O)0)) ? (O)1 : (O)0;
}


//int, real & operation,
//x & x
template<typename T, typename U, typename O> inline static void int_and(T* l, long long size, U* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] & (O)r[i];
    }
}

//x1 & x
template<typename T, typename U, typename O> inline static void int_and(T l, long long size, U* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l & (O)r[i];
    }
}

//x & x1
template<typename T, typename U, typename O> inline static void int_and(T* l, long long size, U r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] & (O)r;
    }
}

//x1 & x1
template<typename T, typename U, typename O> inline static void int_and(T l, U r, O* o)
{
    *o = (O)l & (O)r;
}

// &&
int IntAndInt(types::InternalType* _pL, types::Bool** _pOut);
int BoolAndBool(types::Bool* _pL, types::Bool** _pOut);
int SparseBoolAndSparseBool(types::InternalType* _pL, types::Bool** _pOut);

#undef DECLARE_AND_PROTO
#endif /* !__TYPES_AND_H__ */


