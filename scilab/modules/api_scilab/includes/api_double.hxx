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