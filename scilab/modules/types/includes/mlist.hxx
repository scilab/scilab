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
#include "tlist.hxx"
#include "dynlib_types.h"

namespace types
{
class TYPES_IMPEXP MList : public TList
{
public :
    MList() : TList() {}
    //Destructor uses ~TList()

    virtual InternalType*           clone()
    {
        return new MList(this);
    }
    void                            whoAmI(void)
    {
        std::cout << "types::MList";
    };

    RealType                        getType(void)
    {
        return RealMList;
    }
    bool                            isMList()
    {
        return true;
    }
    bool                            isTList()
    {
        return false;
    }

protected :
    MList(MList *_oMListCopyMe) : TList(_oMListCopyMe) {}
};
}

#endif /* __MLIST_HH__ */
