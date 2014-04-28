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

#include "api_double.hxx"

namespace api_scilab
{
Double::Double(double _dbl)
{
    data = new types::Double(_dbl);
    created = true;
}

Double::Double(double _dblR, double _dblI)
{
    data = new types::Double(_dblR, _dblI);
    created = true;
}

Double::Double(int _iRows, int _iCols, bool _bComplex)
{
    data = new types::Double(_iRows, _iCols, _bComplex);
    created = true;
}

Double::Double(int _iRows, int _iCols, double** _pdbl)
{
    data = new types::Double(_iRows, _iCols, _pdbl);
    created = true;
}

Double::Double(int _iRows, int _iCols, double** _pdblR, double** _pdblI)
{
    data = new types::Double(_iRows, _iCols, _pdblR, _pdblI);
    created = true;
}

Double::Double(int _iDims, int* _piDims, bool _bComplex)
{
    data = new types::Double(_iDims, _piDims, _bComplex);
    created = true;
}

Double* Double::getAsDouble(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isDouble())
    {
        return new Double(*_pIT->getAs<types::Double>());
    }

    return NULL;
}
}
