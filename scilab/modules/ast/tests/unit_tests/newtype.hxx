/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "user.hxx"

class NewType : public types::UserType
{
public:
    NewType()
    {
        bHasToString = true;
    }

    NewType(bool _VariableToString)
    {
        bHasToString = _VariableToString;
    }

    ~NewType() {}

    bool                    hasToString()
    {
        return bHasToString;
    }

    bool                    toString(std::wostringstream& ostr)
    {
        ostr << "This is my type.";
        return true;
    }

    std::wstring            getTypeStr()
    {
        return L"NewType";
    }

    std::wstring            getShortTypeStr()
    {
        return L"nt";
    }

    NewType*    clone()
    {
        return new NewType();
    }

private:
    bool bHasToString;
};
