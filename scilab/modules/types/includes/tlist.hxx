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

namespace types
{
    class TList : public List
    {
    public :
                                        TList();

    private :
                                        TList(TList *_oTListCopyMe);

    public :
        void                            whoAmI(void) { std::cout << "types::TList"; };

        RealType                        getType(void) { return RealTList; }

        TList*                          clone();

        TList*                          getAsTList(void) { return this; }

        bool                            exists(const std::wstring& _sKey);
        InternalType*                   get(const std::wstring& _sKey);
        InternalType*                   get(const int _iIndex);
        int                             getIndexFromString(const std::wstring _sKey);
        bool                            set(const std::wstring& _sKey, InternalType* _pIT);
        bool                            set(const int _iIndex, InternalType* _pIT);

        std::vector<InternalType*>      extract_string(std::list<std::wstring> _stFields);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring             getTypeStr();
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring             getShortTypeStr();

    private :
    };
}

#endif /* __TLIST_HXX__ */
