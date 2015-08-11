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

#ifndef __API_HANDLE_HXX__
#define __API_HANDLE_HXX__

#include "graphichandle.hxx"
#include "api_variable.hxx"

namespace api_scilab
{
class API_SCILAB_IMPEXP GraphicHandle : public MatrixT<long long, types::GraphicHandle>
{
private :
    //internal constructor for existing variable
    GraphicHandle(types::GraphicHandle& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    GraphicHandle(long long _handle);
    GraphicHandle(int _iRows, int _iCols);
    GraphicHandle(int _iDims, int* _piDims);
    virtual ~GraphicHandle()
    {
        if (created)
        {
            delete data;
        }
    };

    static GraphicHandle* getAsGraphicHandle(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabHandle;
    }
};
}

#ifdef _MSC_VER
template class api_scilab::MatrixT<long long, types::GraphicHandle>;
#endif

#endif /* !__API_HANDLE_HXX__ */