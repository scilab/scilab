/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#include "api_pointer.hxx"

namespace api_scilab
{
Pointer::Pointer()
{
    data = new types::Pointer();
    created = true;
}

Pointer::Pointer(const void* _p)
{
    data = new types::Pointer(_p);
    created = true;
}

Pointer* Pointer::getAsPointer(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isPointer())
    {
        return new Pointer(*_pIT->getAs<types::Pointer>());
    }

    return NULL;
}

int Pointer::getSize()
{
    return 1;
}

const void* Pointer::get()
{
    if (data)
    {
        return data->get();
    }

    return NULL;
}

void Pointer::set(const void* _p)
{
    if (data)
    {
        return data->set(_p);
    }
}
}
