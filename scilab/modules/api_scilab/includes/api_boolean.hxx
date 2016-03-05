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