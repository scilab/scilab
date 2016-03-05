/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
