/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __TYPES_SUB_H__
#define __TYPES_SUB_H__

#include "generic_operations.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "sparse.hxx"

void fillSubtractFunction();

//define arrays on operation functions
typedef types::InternalType*(*sub_function)(types::InternalType*, types::InternalType*);

#define DECLARE_SUB_PROTO(x) template<class T, class U, class O> types::InternalType* x(T *_pL, U *_pR)
//Double, int , bool
DECLARE_SUB_PROTO(sub_M_M);
DECLARE_SUB_PROTO(sub_M_MC);
DECLARE_SUB_PROTO(sub_M_S);
DECLARE_SUB_PROTO(sub_M_SC);
DECLARE_SUB_PROTO(sub_M_E);

DECLARE_SUB_PROTO(sub_MC_M);
DECLARE_SUB_PROTO(sub_MC_MC);
DECLARE_SUB_PROTO(sub_MC_S);
DECLARE_SUB_PROTO(sub_MC_SC);
DECLARE_SUB_PROTO(sub_MC_E);

DECLARE_SUB_PROTO(sub_S_M);
DECLARE_SUB_PROTO(sub_S_MC);
DECLARE_SUB_PROTO(sub_S_S);
DECLARE_SUB_PROTO(sub_S_SC);
DECLARE_SUB_PROTO(sub_S_E);

DECLARE_SUB_PROTO(sub_SC_M);
DECLARE_SUB_PROTO(sub_SC_MC);
DECLARE_SUB_PROTO(sub_SC_S);
DECLARE_SUB_PROTO(sub_SC_SC);
DECLARE_SUB_PROTO(sub_SC_E);

//[]
DECLARE_SUB_PROTO(sub_E_M);
DECLARE_SUB_PROTO(sub_E_MC);
DECLARE_SUB_PROTO(sub_E_E);

//eye
DECLARE_SUB_PROTO(sub_I_M);
DECLARE_SUB_PROTO(sub_I_MC);
DECLARE_SUB_PROTO(sub_IC_M);
DECLARE_SUB_PROTO(sub_IC_MC);

DECLARE_SUB_PROTO(sub_I_S);
DECLARE_SUB_PROTO(sub_I_SC);
DECLARE_SUB_PROTO(sub_IC_S);
DECLARE_SUB_PROTO(sub_IC_SC);

DECLARE_SUB_PROTO(sub_M_I);
DECLARE_SUB_PROTO(sub_MC_I);
DECLARE_SUB_PROTO(sub_M_IC);
DECLARE_SUB_PROTO(sub_MC_IC);

DECLARE_SUB_PROTO(sub_S_I);
DECLARE_SUB_PROTO(sub_SC_I);
DECLARE_SUB_PROTO(sub_S_IC);
DECLARE_SUB_PROTO(sub_SC_IC);

DECLARE_SUB_PROTO(sub_I_I);
DECLARE_SUB_PROTO(sub_I_IC);
DECLARE_SUB_PROTO(sub_IC_I);
DECLARE_SUB_PROTO(sub_IC_IC);

DECLARE_SUB_PROTO(sub_I_E);
DECLARE_SUB_PROTO(sub_IC_E);
DECLARE_SUB_PROTO(sub_E_I);
DECLARE_SUB_PROTO(sub_E_IC);

template<> types::InternalType* sub_M_M<types::Polynom, types::Polynom, types::Polynom>(types::Polynom* _pL, types::Polynom* _pR);
template<> types::InternalType* sub_M_M<types::Polynom, types::Double, types::Polynom>(types::Polynom* _pL, types::Double* _pR);
template<> types::InternalType* sub_M_M<types::Double, types::Polynom, types::Polynom>(types::Double* _pL, types::Polynom* _pR);
template<> types::InternalType* sub_I_M<types::Double, types::Polynom, types::Polynom>(types::Double* _pL, types::Polynom* _pR);
template<> types::InternalType* sub_IC_M<types::Double, types::Polynom, types::Polynom>(types::Double* _pL, types::Polynom* _pR);
template<> types::InternalType* sub_I_MC<types::Double, types::Polynom, types::Polynom>(types::Double* _pL, types::Polynom* _pR);
template<> types::InternalType* sub_IC_MC<types::Double, types::Polynom, types::Polynom>(types::Double* _pL, types::Polynom* _pR);

template<> types::InternalType* sub_M_M<types::Sparse, types::Sparse, types::Sparse>(types::Sparse* _pL, types::Sparse* _pR);
template<> types::InternalType* sub_M_M<types::Double, types::Sparse, types::Double>(types::Double* _pL, types::Sparse* _pR);
template<> types::InternalType* sub_M_M<types::Sparse, types::Double, types::Double>(types::Sparse* _pL, types::Double* _pR);
template<> types::InternalType* sub_M_M<types::Double, types::Sparse, types::Sparse>(types::Double* _pL, types::Sparse* _pR);
template<> types::InternalType* sub_M_M<types::Sparse, types::Double, types::Sparse>(types::Sparse* _pL, types::Double* _pR);

//add matrix - matrix ( double, int, bool )
//same type
template<typename T, typename O> inline static void sub(T* l, size_t size, T* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] - (O)r[i];
    }
}

//x - x
template<typename T, typename U, typename O> inline static void sub(T* l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] - (O)r[i];
    }
}


//x - xC
template<typename T, typename U, typename O> inline static void sub(T* l, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] - (O)r[i];
        oc[i] = (O) - rc[i];
    }
}

//xC - x
template<typename T, typename U, typename O> inline static void sub(T* l, T* lc, size_t size, U* r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] - (O)r[i];
        oc[i] = (O)lc[i];
    }
}

// xC - xC
template<typename T, typename O> inline static void sub(T* l, T* lc, size_t size, T* r, T* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] - (O)r[i];
        oc[i] = (O)lc[i] - (O)rc[i];
    }
}

//add matrix - scalar ( double, int, bool )
//x - x1
template<typename T, typename U, typename O> inline static void sub(T* l, size_t size, U r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] - (O)r;
    }
}

//xC - x1
template<typename T, typename U, typename O> inline static void sub(T* l, T* lc, size_t size, U r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] - (O)r;
        oc[i] = (O)lc[i];
    }
}

//x - x1C
template<typename T, typename U, typename O> inline static void sub(T* l, size_t size, U r, U rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] - (O) r;
        oc[i] = (O) - rc;
    }
}

//xC - x1C
template<typename T, typename O> inline static void sub(T* l, T* lc, size_t size, T r, T rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] - (O)r;
        oc[i] = (O)lc[i] - (O)rc;
    }
}

//add scalar - matrix ( double, int, bool )
//x1 - x
template<typename T, typename U, typename O> inline static void sub(T l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l - (O)r[i];
    }
}

//x1 - xC
template<typename T, typename U, typename O> inline static void sub(T l, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l - (O)r[i];
        oc[i] = (O) - rc[i];
    }
}

//x1C - x
template<typename T, typename U, typename O> inline static void sub(T l, T lc, size_t size, U* r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l - (O)r[i];
        oc[i] = (O)lc;
    }
}

//x1C - xC
template<typename T, typename O> inline static void sub(T l, T lc, size_t size, T* r, T* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l - (O)r[i];
        oc[i] = (O)lc - rc[i];
    }
}


//add scalar - scalar ( double, int, bool )
//same type
template<typename T, typename O> inline static void sub(T l, T r, O* o)
{
    *o = (O)l - (O)r;
}
//x1 - x1
template<typename T, typename U, typename O> inline static void sub(T l, U r, O* o)
{
    *o = (O)l - (O)r;
}

//x1C - x1C
template<typename T, typename U, typename O> inline static void sub(T l, T lc, U r, U rc, O* o, O* oc)
{
    *o = (O)l - (O)r;
    *oc = (O)lc - (O)rc;
}

//x1 - []
template<typename T, typename O> inline static void sub(T l, O* o)
{
    *o = (O)l;
}

//x1c - []
template<typename T, typename O> inline static void sub(T l, T lc, size_t /*size*/, O* o, O* oc)
{
    *o = (O)l;
    *oc = (O)lc;
}

//[] - []
inline static void sub()
{
}

#endif /* __TYPES_SUB_H__ */
