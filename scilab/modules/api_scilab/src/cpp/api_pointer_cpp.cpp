/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include "api_pointer.hxx"

namespace api_scilab
{
Pointer::Pointer()
{
    data = new types::Pointer();
    created = true;
}

Pointer::Pointer(void* _p)
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

void* Pointer::get()
{
    if (data)
    {
        return data->get();
    }

    return NULL;
}

void Pointer::set(void* _p)
{
    if (data)
    {
        return data->set(_p);
    }
}
}
