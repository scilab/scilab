/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "internal.hxx"

namespace jit
{

template<typename T, typename U>
inline void push(void * list, const T x)
{
    reinterpret_cast<types::typed_list *>(list)->push_back(new U(x));
}

}
