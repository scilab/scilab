/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#ifndef __USER_HXX__
#define __USER_HXX__

#include "types.hxx"
#include "bool.hxx"

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

    inline bool             isGenericType()
    {
        return false;
    }

public :
    /*** User will be asked to implement the following methods      ***/
    /*** in order Scilab engine to manage correctly this user type  ***/

    virtual std::wstring    getTypeStr() = 0;
    virtual std::wstring    getShortTypeStr() = 0;
    virtual UserType*       clone() = 0;

public :
    /*** User can overload these methods                            ***/
    /*** all methods not overloaded will call scilab overload       ***/

    virtual Bool* equal(UserType*& /*ut*/)
    {
        return NULL;
    }

    // hasToString return false so scilab will call overload %..._p
    // and toString method is useless
    // if user overload hasToString for return true, he must overload toString method
    // bool toString(std::wostringstream& ostr)
    virtual bool hasToString()
    {
        return false;
    }

    // overload this method if hasToString method return true
    virtual bool toString(std::wostringstream& /*ostr*/)
    {
        return false;
    }

    // insertion by value
    // _pArs is a list of scilab types:: of where we want to extract
    // _pSource is what we wan to insert
    virtual UserType* insert(typed_list* /*_pArgs*/, InternalType* /*_pSource*/)
    {
        return NULL;
    }

    // this method is called to perform an extraction by field. ie : a = myUserType.myfield
    // name is the field name
    // out contain extraction of field
    virtual bool extract(const std::wstring& /*name*/, InternalType *& /*out*/)
    {
        return false;
    }

    // extraction by value, this method can be only called by "invoke" method below.
    // _pArs is a list of scilab types:: of where we want to extract
    // return all element extracted, in case when multiple elements returned
    // these elements must be stored in a types::List
    virtual InternalType* extract(typed_list* /*_pArgs*/)
    {
        return NULL;
    }

    // if return false , Scilab will never call "invoke" method
    virtual bool isInvokable() const
    {
        return true;
    }

    // hasInvokeOption must return true to call the user type with optional argument.
    // ie : myUserType(a,b, opt=value, opt2=value2,...)
    // in this case, "types::optional_list& opt" will contain opt and opt2.
    // by default this method return false, the optional list is empty and the input list contains all arguments.
    virtual bool hasInvokeOption() const
    {
        return false;
    }

    // invoke method is called when a UserType is called with "(...)" ie : a = myUserType(...)
    // This implementation allow the use of extract method above, but it can be overloaded.
    // Inputs :
    //  in          : contain input arguments myUserType(arg1,arg2,...)
    //  opt         : contain optional input arguments myUserType(arg1=..., arg2=..., ...)
    //  _iRetCount  : is the number of output arguments (ie : [a,b] = myUserType(...), _iRetCount = 2)
    //  out         : after "invoke" execution, will contain results
    //  execFunc    : is used in case of macro call : Overload::call(L"A_Macro", in, _iRetCount, out, execFunc);
    //  e           : Generally used to return the Location when thowing an error. ie : throw ast::InternalError(L"error message", 999, e.getLocation());
    // Outputs :
    // if false, Scilab will call the macro %UserType_e,where UserType is the string return by the method getShortTypeStr()
    // if true, Scilab will set each elements of out in Scilab variables
    virtual bool invoke(types::typed_list & in, types::optional_list & /*opt*/, int /*_iRetCount*/, types::typed_list & out, const ast::Exp & /*e*/) override
    {
        InternalType* pIT = extract(&in);
        if (pIT)
        {
            out.push_back(pIT);
            return true;
        }

        return false;
    }

    // used to compute the iterator in scilab loop "for"
    // when type is a two dimensions array
    // _iPos is the column position
    virtual GenericType*  getColumnValues(int /*_iPos*/)
    {
        return NULL;
    }

    //called by save to export your usertype as basic scilab type
    //if you do not want to overload this function, save will call %yourtype_save overload function
    virtual InternalType* save()
    {
        return nullptr;
    }

    //load must be done by overload %yourtype_load and must returns a pointer on your UserType
};
}

#endif /* !__USER_HXX__ */
