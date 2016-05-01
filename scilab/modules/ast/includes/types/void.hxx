/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Antoine ELIAS
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

#ifndef __VOID_HXX__
#define __VOID_HXX__

#include "types.hxx"

namespace types
{
class EXTERN_AST Void : public InternalType
{
public :
    Void() {};
    virtual                 ~Void();

    Void*                   clone();

    inline ScilabType       getType(void)
    {
        return ScilabVoid;
    }
    inline ScilabId         getId(void)
    {
        return IdVoid;
    }

    bool                    isVoid()
    {
        return true;
    }

    void                    whoAmI();

    bool                    toString(std::wostringstream& ostr);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"void";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"";
    }
};
}


#endif /* !__VOID_HXX__ */
