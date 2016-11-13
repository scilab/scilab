/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *  Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
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

#ifndef __TYPES_OR_H__
#define __TYPES_OR_H__

#include "generic_operations.hxx"
#include "bool.hxx"
#include "sparse.hxx"

void fillOrFunction();

//define arrays on operation functions
typedef types::InternalType*(*or_function)(types::InternalType*, types::InternalType*);

#define DECLARE_OR_PROTO(x) \
    template<class T, class U, class O> \
    inline types::InternalType* x(T *_pL, U *_pR)

DECLARE_OR_PROTO(or_M_M);
DECLARE_OR_PROTO(or_M_S);
DECLARE_OR_PROTO(or_M_E);

DECLARE_OR_PROTO(or_S_M);
DECLARE_OR_PROTO(or_S_S);
DECLARE_OR_PROTO(or_S_E);

//[]
DECLARE_OR_PROTO(or_E_M);

//eye
DECLARE_OR_PROTO(or_I_M);
DECLARE_OR_PROTO(or_I_S);

DECLARE_OR_PROTO(or_int_M_M);
DECLARE_OR_PROTO(or_int_M_S);
DECLARE_OR_PROTO(or_int_S_M);
DECLARE_OR_PROTO(or_int_S_S);

//boolean sparse specialisation
template<> inline types::InternalType* or_M_M<types::SparseBool, types::SparseBool, types::SparseBool>(types::SparseBool* _pL, types::SparseBool* _pR);
template<> inline types::InternalType* or_M_M<types::SparseBool, types::Bool, types::SparseBool>(types::SparseBool* _pL, types::Bool* _pR);
template<> inline types::InternalType* or_M_M<types::Bool, types::SparseBool, types::SparseBool>(types::Bool* _pL, types::SparseBool* _pR);
template<> inline types::InternalType* or_M_E<types::Bool, types::Double, types::Bool>(types::Bool* _pL, types::Double* _pR);
template<> inline types::InternalType* or_E_M<types::Double, types::Bool, types::Bool>(types::Double* _pL, types::Bool* _pR);

//x & x
template<typename T, typename U, typename O> inline static void bit_or(T* l, long long size, U* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (((T)l[i] != (T)0) || ((U)r[i] != (U)0)) ? (O)1 : (O)0;
    }
}

//x1 & x
template<typename T, typename U, typename O> inline static void bit_or(T l, long long size, U* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (((T)l != (T)0) || ((U)r[i] != (U)0)) ? (O)1 : (O)0;
    }
}

//x & x1
template<typename T, typename U, typename O> inline static void bit_or(T* l, long long size, U r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (((T)l[i] != (T)0) || ((U)r != (U)0)) ? (O)1 : (O)0;
    }
}

//x1 & x1
template<typename T, typename U, typename O> inline static void bit_or(T l, U r, O* o)
{
    *o = (((T)l != (T)0) || ((U)r != (U)0)) ? (O)1 : (O)0;
}


//int, real & operation,
//x & x
template<typename T, typename U, typename O> inline static void int_or(T* l, long long size, U* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] | (O)r[i];
    }
}

//x1 & x
template<typename T, typename U, typename O> inline static void int_or(T l, long long size, U* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l | (O)r[i];
    }
}

//x & x1
template<typename T, typename U, typename O> inline static void int_or(T* l, long long size, U r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] | (O)r;
    }
}

//x1 & x1
template<typename T, typename U, typename O> inline static void int_or(T l, U r, O* o)
{
    *o = (O)l | (O)r;
}

// ||
template<typename T>
void isValueTrue(T* _pL, types::Bool** _pOut);
template<>
void isValueTrue(types::Double* _pL, types::Bool** _pOut);
template<>
void isValueTrue(types::SparseBool* _pL, types::Bool** _pOut);
template<>
void isValueTrue(types::Sparse* _pL, types::Bool** _pOut);

#endif /* __TYPES_OR_H__ */
