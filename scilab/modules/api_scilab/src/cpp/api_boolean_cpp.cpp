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
