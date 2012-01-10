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

#include <vector>
#include "list.hxx"
#include "dynlib_types.h"

namespace types
{
    class TYPES_IMPEXP MList : public List
    {
    public :
                                        MList();
                                        ~MList();

        void                            whoAmI(void) { std::cout << "types::MList"; };

        RealType                        getType(void) { return RealMList; }
        bool                            isMList() { return true; }

        InternalType*                   insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, std::vector<types::InternalType*>* _poSource, bool _bAsVector);
        std::vector<InternalType*>      extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int _iDims, int* _piDimSize, bool _bAsVector);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring            getTypeStr() {return L"mlist";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring            getShortTypeStr() {return L"mlist";}
    };
}

#endif /* __MLIST_HH__ */
