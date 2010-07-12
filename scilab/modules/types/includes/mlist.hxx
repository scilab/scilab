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

#ifndef __MLIST_HH__
#define __MLIST_HH__

#include <list>
#include "list.hxx"

namespace types
{
    class MList : public List
    {
    public :
                                        MList();
                                        ~MList();

        void                            whoAmI(void) { std::cout << "types::MList"; };

        RealType                        getType(void) { return RealMList; }

//        MList*                          clone();

//        std::string                     toString(int _iPrecision, int _iLineLen);

        MList*                          getAsMList(void) { return this; }

        bool                            insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, vector<types::InternalType*>* _poSource, bool _bAsVector);
        std::vector<InternalType*>      extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring             getTypeStr() {return L"mlist";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring             getShortTypeStr() {return L"mlist";}
    };
}

#endif /* __MLIST_HH__ */
