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

#ifndef __API_POINTER_HXX__
#define __API_POINTER_HXX__

#include "pointer.hxx"
#include "api_pointer.hxx"
#include "api_variable.hxx"

namespace api_scilab
{
class API_SCILAB_IMPEXP Pointer : public Variable
{
private :
    types::Pointer* data;
    //internal constructor for existing variable
    Pointer(types::Pointer& _p)
    {
        data = &_p;
        created = false;
    }

public :
    Pointer();
    Pointer(const void* _p);

    virtual ~Pointer()
    {
        if (created)
        {
            delete data;
        }
    };
    void* getReturnVariable()
    {
        return (void*)data;
    }
    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabUserType;
    }
    static Pointer* getAsPointer(types::InternalType* _pIT);
    virtual int getSize();

    const void* get();
    void set(const void* _p);

};
}

#endif /* !__API_POINTER_HXX__ */