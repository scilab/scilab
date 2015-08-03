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

#include "api_boolean.hxx"

namespace api_scilab
{
Bool::Bool(bool _bVal)
{
    data = new types::Bool(_bVal ? 1 : 0);
    created = true;
}

Bool::Bool(int _bVal)
{
    data = new types::Bool(_bVal);
    created = true;
}

Bool::Bool(int _iRows, int _iCols)
{
    data = new types::Bool(_iRows, _iCols);
    created = true;
}

Bool::Bool(int _iDims, int* _piDims)
{
    data = new types::Bool(_iDims, _piDims);
    created = true;
}

Bool::Bool(int _iRows, int _iCols, int **_piData)
{
    data = new types::Bool(_iRows, _iCols, _piData);
    created = true;
}

Bool* Bool::getAsBool(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isBool())
    {
        return new Bool(*_pIT->getAs<types::Bool>());
    }

    return NULL;
}
}
