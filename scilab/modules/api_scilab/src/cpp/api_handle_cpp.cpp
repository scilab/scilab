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

#include "api_handle.hxx"

namespace api_scilab
{
GraphicHandle::GraphicHandle(long long _handle)
{
    data = new types::GraphicHandle(_handle);
    created = true;
}

GraphicHandle::GraphicHandle(int _iRows, int _iCols)
{
    data = new types::GraphicHandle(_iRows, _iCols);
    created = true;
}

GraphicHandle::GraphicHandle(int _iDims, int* _piDims)
{
    data = new types::GraphicHandle(_iDims, _piDims);
    created = true;
}

GraphicHandle* GraphicHandle::getAsGraphicHandle(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isHandle())
    {
        return new GraphicHandle(*_pIT->getAs<types::GraphicHandle>());
    }

    return NULL;
}
}
