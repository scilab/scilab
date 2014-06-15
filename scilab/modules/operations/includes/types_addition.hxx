/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __TYPES_ADD_H__
#define __TYPES_ADD_H__

#include "dynlib_operations.hxx"
#include "generic_operations.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "sparse.hxx"
#include "string.hxx"
extern "C"
{
#include "os_swprintf.h"
}

void fillAddFunction();

//define arrays on operation functions
typedef types::InternalType*(*add_function)(types::InternalType*, types::InternalType*);
add_function getAddFunction(types::InternalType::ScilabId leftId, types::InternalType::ScilabId rightId);

//Double, int , bool
template<class T, class U, class O> inline types::InternalType* add_M_M(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_M_MC(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_M_S(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_M_SC(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_M_E(T *_pL, U *_pR);

template<class T, class U, class O> inline types::InternalType* add_MC_M(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_MC_MC(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_MC_S(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_MC_SC(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_MC_E(T *_pL, U *_pR);

template<class T, class U, class O> inline types::InternalType* add_S_M(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_S_MC(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_S_S(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_S_SC(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_S_E(T *_pL, U *_pR);

template<class T, class U, class O> inline types::InternalType* add_SC_M(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_SC_MC(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_SC_S(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_SC_SC(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_SC_E(T *_pL, U *_pR);


template<class T, class U, class O> inline types::InternalType* add_E_M(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_E_MC(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_E_S(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_E_SC(T *_pL, U *_pR);
template<class T, class U, class O> inline types::InternalType* add_E_E(T *_pL, U *_pR);


//String specilization
template<> types::InternalType* add_M_M<types::String, types::String, types::String>(types::String* _pL, types::String* _pR);
template<> types::InternalType* add_S_M<types::String, types::String, types::String>(types::String* _pL, types::String* _pR);
template<> types::InternalType* add_M_S<types::String, types::String, types::String>(types::String* _pL, types::String* _pR);
template<> types::InternalType* add_S_S<types::String, types::String, types::String>(types::String* _pL, types::String* _pR);
template<> types::InternalType* add_M_E<types::String, types::Double, types::String>(types::String* _pL, types::Double* _pR);
template<> types::InternalType* add_S_E<types::String, types::Double, types::String>(types::String* _pL, types::Double* _pR);
template<> types::InternalType* add_E_M<types::Double, types::String, types::String>(types::Double* _pL, types::String* _pR);
template<> types::InternalType* add_E_S<types::Double, types::String, types::String>(types::Double* _pL, types::String* _pR);


//add matrix + matrix ( double, int, bool )
//same type
template<typename T, typename O> inline static void add(T* l, long long size, T* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r[i];
    }
}

//string version
inline static void add(wchar_t** l, long long size, wchar_t** r, int* lenght , wchar_t** o)
{
    for (int i = 0; i < size ; ++i)
    {
        os_swprintf(o[i], lenght[i], L"%ls%ls", l[i], r[i]);
    }
}

//x + x
template<typename T, typename U, typename O> inline static void add(T* l, long long size, U* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r[i];
    }
}


//x + xC
template<typename T, typename U, typename O> inline static void add(T* l, long long size, U* r, U* rc, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r[i];
        oc[i] = (O)rc[i];
    }
}

//xC + x
template<typename T, typename U, typename O> inline static void add(T* l, T* lc, long long size, U* r, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r[i];
        oc[i] = (O)lc[i];
    }
}

// xC + xC
template<typename T, typename O> inline static void add(T* l, T* lc, long long size, T* r, T* rc, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r[i];
        oc[i] = (O)lc[i] + (O)rc[i];
    }
}

// x + [] and [] + x
template<typename T, typename O> inline static void add(T* l, long long size, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i];
    }
}

// xC + [] and [] + xC
template<typename T, typename O> inline static void add(T* l, T* lc, long long size, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i];
        oc[i] = (O)lc[i];
    }
}

//add matrix + scalar ( double, int, bool )
//x + x1
template<typename T, typename U, typename O> inline static void add(T* l, long long size, U r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r;
    }
}

//string version
inline static void add(wchar_t** l, long long size, wchar_t* r, int* lenght , wchar_t** o)
{
    for (int i = 0; i < size ; ++i)
    {
        os_swprintf(o[i], lenght[i], L"%ls%ls", l[i], r);
    }
}

//xC + x1
template<typename T, typename U, typename O> inline static void add(T* l, T* lc, long long size, U r, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r;
        oc[i] = (O)lc[i];
    }
}

//x + x1C
template<typename T, typename U, typename O> inline static void add(T* l, long long size, U r, U rc, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O) r;
        oc[i] = (O)rc;
    }
}

//xC + x1C )
template<typename T, typename O> inline static void add(T* l, T* lc, long long size, T r, T rc, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l[i] + (O)r;
        oc[i] = (O)lc[i] + (O)rc;
    }
}

//add scalar + matrix ( double, int, bool )
//x1 + x
template<typename T, typename U, typename O> inline static void add(T l, long long size, U* r, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l + (O)r[i];
    }
}

//string version
inline static void add(wchar_t* l, long long size, wchar_t** r, int* lenght , wchar_t** o)
{
    for (int i = 0; i < size ; ++i)
    {
        os_swprintf(o[i], lenght[i], L"%ls%ls", l, r[i]);
    }
}

//x1 + xC
template<typename T, typename U, typename O> inline static void add(T l, long long size, U* r, U* rc, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l + (O)r[i];
        oc[i] = (O)rc[i];
    }
}

//x1C + x
template<typename T, typename U, typename O> inline static void add(T l, T lc, long long size, U* r, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)l + (O)r[i];
        oc[i] = (O)lc;
    }
}

//x1C + xC
template<typename T, typename O> inline static void add(T l, T lc, long long size, T* r, T* rc, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
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
inline static void add(wchar_t* l, wchar_t* r, int lenght , wchar_t* o)
{
    os_swprintf(o, lenght, L"%ls%ls", l, r);
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


//Poly
int AddDoubleToPoly(types::Polynom *_pPoly, types::Double *_pDouble, types::Polynom ** _pPolyOut);
int AddPolyToPoly(types::Polynom* pPoly1, types::Polynom* _pPoly2, types::Polynom ** _pPolyOut);

//Sparse
int AddSparseToSparse(types::Sparse *_pSparse1, types::Sparse *_pSparse2, types::Sparse** _pSparseOut);
int AddSparseToDouble(types::Sparse *_pSparse,  types::Double* _pDouble, types::GenericType** _pDoubleOut);
int AddDoubleToSparse(types::Double* _pDouble, types::Sparse* _pSparse, types::GenericType** _pDoubleOut);

#endif /* __TYPES_ADD_H__ */
