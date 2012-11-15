/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 202 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "user.hxx"
#ifndef __POINTER_HXX__
#define __POINTER_HXX__


namespace types
{
    class Pointer : public User<Pointer>
    {
    public :
                                Pointer() { m_pvData = NULL; }
                                Pointer(void* _pvData) { m_pvData = _pvData; }
        virtual	                ~Pointer() {}
        Pointer*                getAsPointer() { return this; }
        bool                    isPointer(void) { return true; }


        bool                    toString(std::wostringstream& ostr) { return true; }
        InternalType*           clone() { return new Pointer(get()); }

        void*                   get() { return m_pvData; }
        void                    set(void* _pvData) { m_pvData = _pvData; }
    protected :

        void*                  m_pvData;
    };
}

#endif /* !__USER_HXX__ */
