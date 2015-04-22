/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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
class UserType : public GenericType
{
public :
    UserType() {}
    virtual ~UserType() {}

    /*** non virtual function to prevent overriding in user derived class ***/

    inline ScilabType       getType(void)
    {
        return ScilabUserType;
    }

    inline ScilabId         getId(void)
    {
        return IdUserType;
    }

    bool                    isUserType(void)
    {
        return true;
    }

public :
    /*** User will be asked to implement the following methods      ***/
    /*** in order Scilab engine to manage correctly this user type  ***/

    std::wstring    getTypeStr() = 0;
    std::wstring    getShortTypeStr() = 0;
    InternalType*   clone() = 0;

public :
    /*** User can overload these methode ***/
    /*** all methode not overloaded will call scilab overload       ***/

    // hasToString return false so scilab will call overload %..._p
    // and toString method is useless
    // if user overload hasToString for return true, he must overload toString method
    // bool toString(std::wostringstream& ostr)
    virtual bool hasToString()
    {
        return false;
    }

    // overload this methode if hasToString methode return true
    virtual bool toString(std::wostringstream& /*ostr*/)
    {
        return false;
    }

    // insertion by value
    // _pArs is a list of scilab types:: of where we want to extract
    // _pSource is what we wan to insert
    virtual InternalType* insert(typed_list* /*_pArgs*/, InternalType* /*_pSource*/)
    {
        return NULL;
    }

    // extraction by field
    // name is the field name
    // out contain extraction of field
    virtual bool            extract(const std::wstring & /*name*/, InternalType *& /*out*/)
    {
        return false;
    }

    // extraction by value
    // _pArs is a list of scilab types:: of where we want to extract
    // return all element extracted, in case when multiple elements returned
    // these elements must be stored in a types::List
    virtual InternalType* extract(typed_list* /*_pArgs*/)
    {
        return NULL;
    }

    // used to compute the iterator in scilab loop "for"
    // when type is a two dimensions array
    // _iPos is the column position
    virtual GenericType*    getColumnValues(int /*_iPos*/)
    {
        return NULL;
    }

};
}

#endif /* !__USER_HXX__ */
