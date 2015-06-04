/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "int.hxx"
#include "type_traits.hxx"

namespace types
{
template <class T>
bool Int<T>::neg(InternalType *& out)
{
    out = new Int<T>(this->getDims(), this->getDimsArray());
    type_traits::bin_neg<T, T>(this->m_iSize, this->m_pRealData, static_cast<Int<T> *>(out)->get());

    return true;
}

template <class T>
bool Int<T>::transpose(InternalType *& out)
{
    return type_traits::transpose(*this, out);
}

#ifdef _MSC_VER
template class ArrayOf<char>;
template class ArrayOf<unsigned char>;
template class ArrayOf<short>;
template class ArrayOf<unsigned short>;
template class ArrayOf<unsigned int>;
template class ArrayOf<unsigned long long>;
#endif
}
