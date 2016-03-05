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