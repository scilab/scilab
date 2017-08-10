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

#ifndef __TYPES_ADD_H__
#define __TYPES_ADD_H__

#include "generic_operations.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "sparse.hxx"
#include "string.hxx"
extern "C"
{
#include "os_string.h"
}

void fillAddFunction();

//define arrays on operation functions
typedef types::InternalType*(*add_function)(types::InternalType*, types::InternalType*);

#define DECLARE_ADD_PROTO(x) template<class T, class U, class O> types::InternalType* x(T *_pL, U *_pR)
//Double, int , bool
DECLARE_ADD_PROTO(add_M_M);
DECLARE_ADD_PROTO(add_M_MC);
DECLARE_ADD_PROTO(add_M_S);
DECLARE_ADD_PROTO(add_M_SC);
DECLARE_ADD_PROTO(add_M_E);

DECLARE_ADD_PROTO(add_MC_M);
DECLARE_ADD_PROTO(add_MC_MC);
DECLARE_ADD_PROTO(add_MC_S);
DECLARE_ADD_PROTO(add_MC_SC);
DECLARE_ADD_PROTO(add_MC_E);

DECLARE_ADD_PROTO(add_S_M);
DECLARE_ADD_PROTO(add_S_MC);
DECLARE_ADD_PROTO(add_S_S);
DECLARE_ADD_PROTO(add_S_SC);
DECLARE_ADD_PROTO(add_S_E);

DECLARE_ADD_PROTO(add_SC_M);
DECLARE_ADD_PROTO(add_SC_MC);
DECLARE_ADD_PROTO(add_SC_S);
DECLARE_ADD_PROTO(add_SC_SC);
DECLARE_ADD_PROTO(add_SC_E);

//[]
DECLARE_ADD_PROTO(add_E_M);
DECLARE_ADD_PROTO(add_E_MC);
DECLARE_ADD_PROTO(add_E_S);
DECLARE_ADD_PROTO(add_E_SC);
DECLARE_ADD_PROTO(add_E_E);

//eye
DECLARE_ADD_PROTO(add_I_M);
DECLARE_ADD_PROTO(add_I_MC);
DECLARE_ADD_PROTO(add_IC_M);
DECLARE_ADD_PROTO(add_IC_MC);

DECLARE_ADD_PROTO(add_I_S);
DECLARE_ADD_PROTO(add_I_SC);
DECLARE_ADD_PROTO(add_IC_S);
DECLARE_ADD_PROTO(add_IC_SC);

DECLARE_ADD_PROTO(add_M_I);
DECLARE_ADD_PROTO(add_MC_I);
DECLARE_ADD_PROTO(add_M_IC);
DECLARE_ADD_PROTO(add_MC_IC);

DECLARE_ADD_PROTO(add_S_I);
DECLARE_ADD_PROTO(add_SC_I);
DECLARE_ADD_PROTO(add_S_IC);
DECLARE_ADD_PROTO(add_SC_IC);

DECLARE_ADD_PROTO(add_I_I);
DECLARE_ADD_PROTO(add_I_IC);
DECLARE_ADD_PROTO(add_IC_I);
DECLARE_ADD_PROTO(add_IC_IC);

DECLARE_ADD_PROTO(add_I_E);
DECLARE_ADD_PROTO(add_IC_E);
DECLARE_ADD_PROTO(add_E_I);
DECLARE_ADD_PROTO(add_E_IC);

#undef DECLARE_ADD_PROTO

//String specilization

template<> types::InternalType* add_M_M<types::String, types::String, types::String>(types::String* _pL, types::String* _pR);
template<> types::InternalType* add_S_M<types::String, types::String, types::String>(types::String* _pL, types::String* _pR);
template<> types::InternalType* add_M_S<types::String, types::String, types::String>(types::String* _pL, types::String* _pR);
template<> types::InternalType* add_S_S<types::String, types::String, types::String>(types::String* _pL, types::String* _pR);
template<> types::InternalType* add_M_E<types::String, types::Double, types::String>(types::String* _pL, types::Double* _pR);
template<> types::InternalType* add_S_E<types::String, types::Double, types::String>(types::String* _pL, types::Double* _pR);
template<> types::InternalType* add_E_M<types::Double, types::String, types::String>(types::Double* _pL, types::String* _pR);
template<> types::InternalType* add_E_S<types::Double, types::String, types::String>(types::Double* _pL, types::String* _pR);

template<> types::InternalType* add_M_M<types::Polynom, types::Polynom, types::Polynom>(types::Polynom* _pL, types::Polynom* _pR);
template<> types::InternalType* add_M_M<types::Polynom, types::Double, types::Polynom>(types::Polynom* _pL, types::Double* _pR);
template<> types::InternalType* add_M_M<types::Double, types::Polynom, types::Polynom>(types::Double* _pL, types::Polynom* _pR);
template<> types::InternalType* add_M_I<types::Polynom, types::Double, types::Polynom>(types::Polynom* _pL, types::Double* _pR);
template<> types::InternalType* add_I_M<types::Double, types::Polynom, types::Polynom>(types::Double* _pL, types::Polynom* _pR);

template<> types::InternalType* add_M_M<types::Sparse, types::Sparse, types::Sparse>(types::Sparse* _pL, types::Sparse* _pR);
template<> types::InternalType* add_M_M<types::Double, types::Sparse, types::Double>(types::Double* _pL, types::Sparse* _pR);
template<> types::InternalType* add_M_M<types::Sparse, types::Double, types::Double>(types::Sparse* _pL, types::Double* _pR);
template<> types::InternalType* add_M_M<types::Double, types::Sparse, types::Sparse>(types::Double* _pL, types::Sparse* _pR);
template<> types::InternalType* add_M_M<types::Sparse, types::Double, types::Sparse>(types::Sparse* _pL, types::Double* _pR);

//add matrix + matrix ( double, int, bool )
//same type
template<typename T, typename O> inline static void add(T* l, size_t size, T* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r[i];
    }
}

//string version
inline static void add(wchar_t** l, size_t size, wchar_t** r, int* length , wchar_t** o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        os_swprintf(o[i], length[i], L"%ls%ls", l[i], r[i]);
    }
}

//x + x
template<typename T, typename U, typename O> inline static void add(T* l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r[i];
    }
}


//x + xC
template<typename T, typename U, typename O> inline static void add(T* l, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r[i];
        oc[i] = (O)rc[i];
    }
}

//xC + x
template<typename T, typename U, typename O> inline static void add(T* l, T* lc, size_t size, U* r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r[i];
        oc[i] = (O)lc[i];
    }
}

// xC + xC
template<typename T, typename O> inline static void add(T* l, T* lc, size_t size, T* r, T* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r[i];
        oc[i] = (O)lc[i] + (O)rc[i];
    }
}

// x + [] and [] + x
template<typename T, typename O> inline static void add(T* l, size_t size, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i];
    }
}

// xC + [] and [] + xC
template<typename T, typename O> inline static void add(T* l, T* lc, size_t size, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i];
        oc[i] = (O)lc[i];
    }
}

//add matrix + scalar ( double, int, bool )
//x + x1
template<typename T, typename U, typename O> inline static void add(T* l, size_t size, U r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r;
    }
}

//string version
inline static void add(wchar_t** l, size_t size, wchar_t* r, int* length , wchar_t** o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        os_swprintf(o[i], length[i], L"%ls%ls", l[i], r);
    }
}

//xC + x1
template<typename T, typename U, typename O> inline static void add(T* l, T* lc, size_t size, U r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r;
        oc[i] = (O)lc[i];
    }
}

//x + x1C
template<typename T, typename U, typename O> inline static void add(T* l, size_t size, U r, U rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O) r;
        oc[i] = (O)rc;
    }
}

//xC + x1C )
template<typename T, typename O> inline static void add(T* l, T* lc, size_t size, T r, T rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r;
        oc[i] = (O)lc[i] + (O)rc;
    }
}

//add scalar + matrix ( double, int, bool )
//x1 + x
template<typename T, typename U, typename O> inline static void add(T l, size_t size, U* r, O* o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l + (O)r[i];
    }
}

//string version
inline static void add(wchar_t* l, size_t size, wchar_t** r, int* length , wchar_t** o)
{
    for (size_t i = 0; i < size ; ++i)
    {
        os_swprintf(o[i], length[i], L"%ls%ls", l, r[i]);
    }
}

//x1 + xC
template<typename T, typename U, typename O> inline static void add(T l, size_t size, U* r, U* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l + (O)r[i];
        oc[i] = (O)rc[i];
    }
}

//x1C + x
template<typename T, typename U, typename O> inline static void add(T l, T lc, size_t size, U* r, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l + (O)r[i];
        oc[i] = (O)lc;
    }
}

//x1C + xC
template<typename T, typename O> inline static void add(T l, T /*lc*/, size_t size, T* r, T* rc, O* o, O* oc)
{
    for (size_t i = 0; i < size ; ++i)
    {
        o[i] = (O)l + (O)r[i];
        oc[i] = (O)rc[i];
    }
}


//add scalar + scalar ( double, int, bool )
//same type
template<typename T, typename O> inline static void add(T l, T r, O* o)
{
    *o = (O)l + (O)r;
}
//x1 + x1
template<typename T, typename U, typename O> inline static void add(T l, U r, O* o)
{
    *o = (O)l + (O)r;
}

//string version
inline static void add(wchar_t* l, wchar_t* r, int length , wchar_t* o)
{
    os_swprintf(o, length, L"%ls%ls", l, r);
}

//x1C + x1C
template<typename T, typename U, typename O> inline static void add(T l, T lc, U r, U rc, O* o, O* oc)
{
    *o = (O)l + (O)r;
    *oc = (O)lc + (O)rc;
}

//x1 + [] and [] + x1
template<typename T, typename O> inline static void add(T l, O* o)
{
    *o = (O)l;
}

//x1c + [] and [] + x1c
template<typename T, typename O> inline static void add(T l, T lc, O* o, O* oc)
{
    *o = (O)l;
    *oc = (O)lc;
}

//[] + []
inline static void add()
{
}


//Sparse
int AddSparseToSparse(types::Sparse *_pSparse1, types::Sparse *_pSparse2, types::Sparse** _pSparseOut);
int AddSparseToDouble(types::Sparse *_pSparse,  types::Double* _pDouble, types::GenericType** _pDoubleOut);
int AddDoubleToSparse(types::Double* _pDouble, types::Sparse* _pSparse, types::GenericType** _pDoubleOut);

#endif /* __TYPES_ADD_H__ */
