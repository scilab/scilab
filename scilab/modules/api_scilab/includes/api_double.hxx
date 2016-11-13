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

#ifdef _MSC_VER
#pragma warning( disable : 4275 )
#endif

#ifndef __API_DOUBLE_HXX__
#define __API_DOUBLE_HXX__

#include "double.hxx"
#include "api_variable.hxx"

namespace api_scilab
{
class API_SCILAB_IMPEXP Double : public MatrixT<double, types::Double>
{
private :
    //internal constructor for existing variable
    Double(types::Double& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    Double(double _dbl);
    Double(double _dblR, double _dblI);
    Double(int _iRows, int _iCols, bool _bComplex = false);
    Double(int _iRows, int _iCols, double** _pdbl);
    Double(int _iRows, int _iCols, double** _pdblR, double** _pdblI);
    Double(int _iDims, int* _piDims, bool _bComplex = false);
    virtual ~Double()
    {
        if (created)
        {
            delete data;
        }
    };

    static Double* getAsDouble(types::InternalType* _pdbl);
    static Double* Empty();

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabDouble;
    }
};
}

#ifdef _MSC_VER
template class api_scilab::MatrixT<double, types::Double>;
#endif

#endif /* !__API_DOUBLE_HXX__ */