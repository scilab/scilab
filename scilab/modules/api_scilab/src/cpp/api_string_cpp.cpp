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

#include "api_string.hxx"

namespace api_scilab
{
String::String(int _iRows, int _iCols)
{
    data = new types::String(_iRows, _iCols);
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
