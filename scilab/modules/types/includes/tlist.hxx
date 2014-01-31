/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TLIST_HXX__
#define __TLIST_HXX__

#include <list>
#include <vector>
#include "list.hxx"
#include "string.hxx"
#include "dynlib_types.h"

namespace types
{
class TYPES_IMPEXP TList : public List
{
public :
    TList();
    ~TList();
protected :
    TList(TList *_oTListCopyMe);

public :
    void                            whoAmI(void)
    {
        std::cout << "types::TList";
    };

    RealType                        getType(void)
    {
        return RealTList;
    }
    bool                            isTList()
    {
        return true;
    }

    virtual InternalType*           clone();

    bool                            exists(const std::wstring& _sKey);
    InternalType*                   getField(const std::wstring& _sKey);
    int                             getIndexFromString(const std::wstring& _sKey);
    bool                            set(const std::wstring& _sKey, InternalType* _pIT);
    bool                            set(const int _iIndex, InternalType* _pIT);

    std::vector<InternalType*>      extractStrings(const std::list<std::wstring>& _stFields);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring            getTypeStr();
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring            getShortTypeStr();

    String*                         getFieldNames();

    bool                            toString(std::wostringstream& ostr);

private :
};
}

#endif /* __TLIST_HXX__ */
