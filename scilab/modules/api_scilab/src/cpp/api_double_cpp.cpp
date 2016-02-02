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

#include "api_double.hxx"

namespace api_scilab
{
Double* Double::Empty()
{
    return new Double(0, 0);
}

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
