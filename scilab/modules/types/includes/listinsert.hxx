/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LISTINSERT_HXX__
#define __LISTINSERT_HXX__

#include "types.hxx"
#include "listoperation.hxx"
#include "dynlib_types.h"

namespace types
{
    class TYPES_IMPEXP ListInsert : public ListOperation
    {
    public :
                                ListInsert(InternalType* _pIT);
        virtual                 ~ListInsert();

        InternalType*           clone();

        RealType                getType(void);
        bool                    isListInsert() { return true; }

        void                    whoAmI();

        std::wstring            toString();
        InternalType*           getInsert();

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring    getTypeStr() {return L"listinsert";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring    getShortTypeStr() {return L"";}

    private :
        InternalType*           m_pInsert;
    };
}


#endif /* !__LISTINSERT_HXX__ */
