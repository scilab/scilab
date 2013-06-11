/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __USER_HXX__
#define __USER_HXX__

#include "types.hxx"

namespace types
{
template <typename T>
class User : public InternalType
{
public :
    User() {}
    virtual	                ~User() {}
    T*                      getAsUserType()
    {
        return this;
    }
    //non virtaul function to prevent overriding in user derived class
    RealType                getType(void)
    {
        return RealUserType;
    }

    /*
    ** User will be asked to implement the following methods
    ** in order Scilab engine to manage correctly this user type
    */
public :
    virtual bool            toString(std::wostringstream& ostr)
    {
        return L"";
    }
    std::wstring            getTypeStr()
    {
        return L"pointer";
    }
    std::wstring            getShortTypeStr()
    {
        return L"p";
    }
    InternalType*           clone()
    {
        return new User();
    }
};
}

#endif /* !__USER_HXX__ */
