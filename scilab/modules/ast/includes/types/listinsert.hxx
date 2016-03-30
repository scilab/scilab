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

#ifndef __LISTINSERT_HXX__
#define __LISTINSERT_HXX__

#include "types.hxx"
#include "listoperation.hxx"

namespace types
{
class EXTERN_AST ListInsert : public ListOperation
{
public :
    ListInsert(InternalType* _pIT);
    virtual                 ~ListInsert();

    ListInsert*             clone();

    inline ScilabType       getType(void)
    {
        return ScilabListInsertOperation;
    }
    inline ScilabId         getId(void)
    {
        return IdListInsertOperation;
    }


    bool                    isListInsert()
    {
        return true;
    }

    void                    whoAmI();

    bool                    toString(std::wostringstream& ostr);
    InternalType*           getInsert();

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"listinsert";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"";
    }

private :
    InternalType*           m_pInsert;
};
}


#endif /* !__LISTINSERT_HXX__ */
