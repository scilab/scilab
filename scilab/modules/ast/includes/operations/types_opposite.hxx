/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __TYPES_OPPOSITE_HXX__
#define __TYPES_OPPOSITE_HXX__

#include "internal.hxx"
#include "sparse.hxx"
#include "polynom.hxx"
#include "bool.hxx"
#include "double.hxx"

void fillOppositeFunction();

//define arrays on operation functions
typedef types::InternalType*(*opposite_function)(types::InternalType*);

#define DECLARE_OPPOSITE_PROTO(x) template<class T, class O> types::InternalType* x(T *_pL)
DECLARE_OPPOSITE_PROTO(opposite_E);
DECLARE_OPPOSITE_PROTO(opposite_I);
DECLARE_OPPOSITE_PROTO(opposite_IC);
DECLARE_OPPOSITE_PROTO(opposite_S);
DECLARE_OPPOSITE_PROTO(opposite_SC);
DECLARE_OPPOSITE_PROTO(opposite_M);
DECLARE_OPPOSITE_PROTO(opposite_MC);

#undef DECLARE_OPPOSITE_PROTO

//do not inline it
template<> types::InternalType* opposite_M<types::Bool, types::Double>(types::Bool* _pL);

template<> types::InternalType* opposite_M<types::Sparse, types::Sparse>(types::Sparse* _pL);
template<> types::InternalType* opposite_MC<types::Sparse, types::Sparse>(types::Sparse* _pL);

template<> types::InternalType* opposite_S<types::Polynom, types::Polynom>(types::Polynom* _pL);
template<> types::InternalType* opposite_SC<types::Polynom, types::Polynom>(types::Polynom* _pL);
template<> types::InternalType* opposite_M<types::Polynom, types::Polynom>(types::Polynom* _pL);
template<> types::InternalType* opposite_MC<types::Polynom, types::Polynom>(types::Polynom* _pL);


template<typename T, typename O> inline static void opposite(T l, O* o)
{
    *o = (O)(-l);
}

template<typename T, typename O> inline static void opposite(T l, T lc, O* o, O* oc)
{
    *o = (O)(-l);
    *oc = (O)(-lc);
}

template<typename T, typename O> inline static void opposite(T* l, long long size, O* o)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)(-l[i]);
    }
}

template<typename T, typename O> inline static void opposite(T* l, T* lc, long long size, O* o, O* oc)
{
    for (int i = 0; i < size ; ++i)
    {
        o[i] = (O)(-l[i]);
        oc[i] = (O)(-lc[i]);
    }
}


#endif /* __TYPES_OPPOSITE_HXX__ */
