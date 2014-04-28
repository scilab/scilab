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

#ifndef __API_BOOLEAN_HXX__
#define __API_BOOLEAN_HXX__

#include "bool.hxx"
#include "api_variable.hxx"

namespace api_scilab
{
class API_SCILAB_IMPEXP Bool : public MatrixT<int, types::Bool>
{
private :
    //internal constructor for existing variable
    Bool(types::Bool& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    Bool(bool _bVal);
    Bool(int _bVal);
    Bool(int _iRows, int _iCols);
    Bool(int _iDims, int* _piDims);
    Bool(int _iRows, int _iCols, int **_piData);
    virtual ~Bool()
    {
        if (created)
        {
            delete data;
        }
    };

    static Bool* getAsBool(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabBool;
    }
};
}

#ifdef _MSC_VER
template class api_scilab::MatrixT<int, types::Bool>;
#endif

#endif /* !__API_BOOLEAN_HXX__ */