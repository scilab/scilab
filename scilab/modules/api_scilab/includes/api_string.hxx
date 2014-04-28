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

#ifndef __API_STRING_HXX__
#define __API_STRING_HXX__

#include "string.hxx"
#include "api_variable.hxx"

namespace api_scilab
{
class API_SCILAB_IMPEXP String : public MatrixT<wchar_t*, types::String>
{
private :
    //internal constructor for existing variable
    String(types::String& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    String(int _iRows, int _iCols);
    String(int _iDims, int* _piDims);
    String(int _iRows, int _iCols, wchar_t** _pstData);
    String(const wchar_t *_pstData);
    String(const char *_pstData);

    virtual ~String()
    {
        if (created)
        {
            delete data;
        }
    };

    static String* getAsString(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabString;
    }
};
}

#ifdef _MSC_VER
template class api_scilab::MatrixT<wchar_t*, types::String>;
#endif

#endif /* !__API_STRING_HXX__ */