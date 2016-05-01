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

#ifndef __LISTOPERATION_HXX__
#define __LISTOPERATION_HXX__

#include <list>
#include "types.hxx"

namespace types
{
class EXTERN_AST ListOperation : public InternalType
{
public :
    ListOperation(): InternalType() {};
    virtual                 ~ListOperation();

    ListOperation*          clone();

    inline ScilabType       getType(void)
    {
        return ScilabListOperation;
    }
    inline ScilabId         getId(void)
    {
        return IdListOperation;
    }

    bool                    isListOperation()
    {
        return true;
    }

    void                    whoAmI();

    bool                    toString(std::wostringstream& ostr);
    virtual bool            isAssignable(void)
    {
        return false;
    }

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"listoperation";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"";
    }
};
}


#endif /* !__LISTOPERATION_HXX__ */
