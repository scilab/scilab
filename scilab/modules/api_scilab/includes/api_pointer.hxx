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
    Pointer(void* _p);

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

    void* get();
    void set(void* _p);

};
}

#endif /* !__API_POINTER_HXX__ */