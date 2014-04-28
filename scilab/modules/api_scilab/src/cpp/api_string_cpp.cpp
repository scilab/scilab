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

#include "api_string.hxx"

namespace api_scilab
{
String::String(int _iRows, int _iCols)
{
    data = new types::String(_iRows, _iRows);
    created = true;
}

String::String(int _iDims, int* _piDims)
{
    data = new types::String(_iDims, _piDims);
    created = true;
}

String::String(int _iRows, int _iCols, wchar_t** _pwstData)
{
    data = new types::String(_iRows, _iRows, _pwstData);
    created = true;
}

String::String(const wchar_t *_pwstData)
{
    data = new types::String(_pwstData);
    created = true;
}

String::String(const char *_pstData)
{
    data = new types::String(_pstData);
    created = true;
}

String* String::getAsString(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isString())
    {
        return new String(*_pIT->getAs<types::String>());
    }

    return NULL;
}
}
